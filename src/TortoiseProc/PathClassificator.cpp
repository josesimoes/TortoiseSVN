#include "StdAfx.h"
#include "PathClassificator.h"

static inline bool CompareCI (char lhs, char rhs)
{
    // lower 5 bits must be equal if same or only different in case

    char diffs = lhs ^ rhs;
    if ((diffs & 0x1f) != 0)
        return false;

    // maybe the same, different in case or just a mismatch in bits 5..7

    if (diffs != 0)
    {
        // not the same but maybe only different in case

        char lowLhs = lhs;
        if ((lowLhs >= 'A') && (lowLhs <= 'Z'))
            lowLhs += 'a' - 'A';

        // rhs must be normalized to lower case

        assert ((rhs < 'A') || (rhs > 'Z'));

        // mismatch?

        if (lowLhs != rhs)
            return false;
    }

    return true;
}

bool CPathClassificator::CWildCardPattern::WildCardMatch 
    (const char* s, const char* pattern) const
{
    while (1)
    {
        // consume one pattern char per loop

        char c = *s;
        char p = *pattern;

        ++p;
        switch (p)
        {
        case '*':
            {
                // asterisk at the end of a pattern matches everything

                if (*pattern == 0)
                    return true;

                // try to find a matching position

                while (c != 0)
                {
                    if (WildCardMatch (s, pattern))
                        return true;

                    c = *(++s);
                }

                // end of string -> failed

                return false;
            }
        case '?':
            {
                // string must contain at least one char

                if (c == 0)
                    return false;
                else
                    ++s;

                break;
            }
        case 0:
            {
                // end of pattern

                return c == 0;
            }
        default:
            {
                // ordinary char vs. char match

                if (CompareCI (c, p) == false)
                    return false;
            }
        }
        
        // next source char

        ++s;
    }

    // we should never get here

    assert (0);
    return false;
}

bool CPathClassificator::CWildCardPattern::StraightMatch 
    (const char* s, size_t length) const
{
    // that will short-cut most comparisons

    if (pattern.length() != length)
        return false;

    // compare all chars until we either find a mismatch or run out of chars

    for (const char* p = pattern.c_str(); *p != 0; ++p, ++s)
        if (CompareCI (*s, *p) == false)
            return false;

    // no mismatch found

    return true;
}

// construction utility

void CPathClassificator::CWildCardPattern::NormalizePattern()
{
    // normalize case

    for (size_t i = 0, count = pattern.length(); i < count; ++i)
    {
        char& c = pattern[i];
        if ((c >= 'A') && (c <= 'Z'))
            c += 'a' - 'A';
    }

    // replace all "**", "*?" by "*"

    size_t i = 0;
    while (i+1 < pattern.length())
    {
        if ((pattern[i] == '*') && (   (pattern[i+1] == '*') 
                                    || (pattern[i+1] == '?')))
            pattern.erase (i+1);
        else 
            ++i;
    }
}

// construction

CPathClassificator::CWildCardPattern::CWildCardPattern 
    ( const std::string& pattern)
    : pattern (pattern)
    , hasWildCards (pattern.find_first_of ("?*") != std::string::npos)
{
    if (hasWildCards)
        NormalizePattern();
}

// match s against the pattern

bool CPathClassificator::CWildCardPattern::Match 
    (const char* s, size_t length) const
{
    return hasWildCards
        ? WildCardMatch (s, pattern.c_str())
        : StraightMatch (s, length);
};

// construction utility

CPathClassificator::TPatterns 
CPathClassificator::ParsePatterns (const std::string& patterns) const
{
    TPatterns result;

    for (size_t start = 0; start < patterns.length();)
    {
        // find next delimiter

        size_t end = patterns.find (';', start);
        if (end == std::string::npos)
            end = patterns.length();

        // extract pattern and add it to the list

        std::string pattern = patterns.substr (start, end - start);
        result.push_back (CWildCardPattern (pattern));

        // to next pattern

        start = end+1;
    }

    return result;
}

bool CPathClassificator::Matches 
    ( TPatterns::const_iterator firstPattern
    , TPatterns::const_iterator lastPattern
    , const char* element
    , size_t length) const
{
    for ( TPatterns::const_iterator iter = firstPattern
        ; iter != lastPattern
        ; ++iter)
    {
        if (iter->Match (element, length))
            return true;
    }

    return false;
}

void CPathClassificator::ClassifyPathElements 
    ( const LogCache::CStringDictionary& elements
    , const TPatterns& patterns
    , unsigned char classification)
{
    TPatterns::const_iterator firstPattern = patterns.begin();
    TPatterns::const_iterator lastPattern = patterns.end();

    // classify

    for (size_t i = 0, count = elements.size(); i < count; ++i)
        if (Matches (firstPattern, lastPattern, elements[i], elements.GetLength(i)))
            pathElementClassification[i] |= classification;
}

void CPathClassificator::ClassifyPathElements 
    (const LogCache::CStringDictionary& elements)
{
    // initialize

    pathElementClassification.clear();
    pathElementClassification.insert ( pathElementClassification.end()
                                     , elements.size()
                                     , 0);

    // one class at a time

    ClassifyPathElements (elements, trunkPatterns, IS_TRUNK);
    ClassifyPathElements (elements, branchesPatterns, IS_BRANCH);
    ClassifyPathElements (elements, tagsPatterns, IS_TAG);
}

void CPathClassificator::ClassifyPaths (const LogCache::CPathDictionary& paths)
{
    // initialize

    pathClassification.clear();
    pathClassification.insert ( pathClassification.end()
                              , paths.size()
                              , 0);

    // fill (root is always "other")

    for (size_t i = 1, count = paths.size(); i < count; ++i)
    {
        LogCache::index_t parentID = paths.GetParent(i);
        LogCache::index_t elementID = paths.GetPathElementID(i);

        pathClassification[i] = pathClassification [parentID]
                              | pathElementClassification [elementID];
    }

    // set "other" classification where there is no classification, yet

    typedef std::vector<unsigned char>::iterator IT;
    for ( IT iter = pathClassification.begin(), end = pathClassification.end()
        ; iter != end
        ; ++iter)
    {
        if (*iter == 0)
            *iter = IS_OTHER;
    }
}

// construction / destruction

CPathClassificator::CPathClassificator ( const LogCache::CPathDictionary& paths
                                       , const std::string& trunkPatterns
                                       , const std::string& branchesPatterns
                                       , const std::string& tagsPatterns)
    : trunkPatterns (ParsePatterns (trunkPatterns))
    , branchesPatterns (ParsePatterns (branchesPatterns))
    , tagsPatterns (ParsePatterns (tagsPatterns))
{
    // match all paths against all patterns

    ClassifyPathElements (paths.GetPathElements());
    ClassifyPaths (paths);
}

CPathClassificator::~CPathClassificator(void)
{
}

// get the classification for a given path

unsigned char CPathClassificator::GetClassification 
    (const LogCache::CDictionaryBasedTempPath& path) const
{
    // use the path classification we already have

    unsigned char result = GetClassification (path.GetBasePath().GetIndex());
    if (path.IsFullyCachedPath())
        return result;

    // try to classify the remaining elements as efficient as possible

    const std::vector<std::string>& relPathElements 
        = path.GetRelPathElements();
    const LogCache::CStringDictionary& pathElements 
        = path.GetDictionary()->GetPathElements();

    for (size_t i = 0, count = relPathElements.size(); i < count; ++i)
    {
        // we should have a classification for this element
        // (this may not be true, if part of the log history is missing)

        const std::string& relPathElement = relPathElements[i];
        const char* element = relPathElement.c_str();

        LogCache::index_t elementID = pathElements.Find (element);
        if (elementID != LogCache::NO_INDEX)
        {
            result |= pathElementClassification[elementID];
        }
        else
        {
            size_t length = relPathElement.length();
            if (Matches (trunkPatterns.begin(), trunkPatterns.end(), element, length))
                result |= IS_TRUNK;
            if (Matches (branchesPatterns.begin(), branchesPatterns.end(), element, length))
                result |= IS_BRANCH;
            if (Matches (tagsPatterns.begin(), tagsPatterns.end(), element, length))
                result |= IS_TAG;
        }
    }

    // ready

    return result;
}
