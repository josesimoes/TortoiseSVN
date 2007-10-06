// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2007-2007 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "StdAfx.h"

#include "CacheLogQuery.h"

#include "CachedLogInfo.h"
#include "CopyFollowingLogIterator.h"
#include "StrictLogIterator.h"
#include "LogCachePool.h"

#include "svn_client.h"
#include "svn_opt.h"

#include "UnicodeUtils.h"
#include "PathUtils.h"
#include "TSVNPath.h"
#include "SVN.h"
#include "SVNInfo.h"
#include "SVNError.h"

// make sure, we can iterator over the given range for the given path

void CCacheLogQuery::CLogFiller::MakeRangeIterable ( const CDictionaryBasedPath& path
												   , revision_t startRevision
												   , revision_t count)
{
	// trim the range to cover only missing data
	// (we may already have enough information
	// to iterate over the whole range -> don't
	// ask the server in that case)

	CStrictLogIterator iterator ( cache
								, startRevision + count-1
								, CDictionaryBasedTempPath (path));

	iterator.Retry();
	while (   !iterator.EndOfPath() 
		   && !iterator.DataIsMissing() 
		   && (iterator.GetRevision() >= startRevision))
	{
		iterator.Advance();
	}

	if (iterator.EndOfPath() || !iterator.DataIsMissing())
		return;

	// o.k., some data is missing. Ask for it.

	CLogFiller().FillLog ( cache
						 , URL
						 , svnQuery
						 , iterator.GetRevision()
						 , startRevision
						 , CDictionaryBasedTempPath (path)
						 , 0
						 , true
						 , NULL
                         , false);
}

// implement ILogReceiver

void CCacheLogQuery::CLogFiller::ReceiveLog ( LogChangedPathArray* changes
											, svn_revnum_t rev
											, const CString& author
											, const apr_time_t& timeStamp
											, const CString& message)
{
	// add it to the cache, if it is not in there, yet

	revision_t revision = static_cast<revision_t>(rev);
	if (cache->GetRevisions()[revision] == NO_INDEX)
	{
		// create the revision entry

		cache->Insert ( revision
					  , (const char*)CUnicodeUtils::GetUTF8 (author)
					  , (const char*)CUnicodeUtils::GetUTF8 (message)
					  , timeStamp);

		// add all changes

		for (INT_PTR i = 0, count = changes->GetCount(); i < count; ++i)
		{
			const LogChangedPath* change = changes->GetAt (i);

			CRevisionInfoContainer::TChangeAction action 
				= (CRevisionInfoContainer::TChangeAction)(change->action * 4);
			std::string path 
				= (const char*)SVN::MakeSVNUrlOrPath (change->sPath);
			std::string copyFromPath 
				= (const char*)SVN::MakeSVNUrlOrPath (change->sCopyFromPath);
			revision_t copyFromRevision 
				= change->lCopyFromRev == 0 
				? NO_REVISION 
				: static_cast<revision_t>(change->lCopyFromRev);

			cache->AddChange (action, path, copyFromPath, copyFromRevision);
		}

		// update our path info

		currentPath->RepeatLookup();
	}

	// mark the gap and update the current path

	if (firstNARevision > revision)
	{
		// due to only the parent path being renamed, the currentPath
		// may not have shown up in the log -> don't mark the range
		// as N/A for some *parent*.

		if (currentPath->IsFullyCachedPath())
		{
			cache->AddSkipRange ( currentPath->GetBasePath()
								, revision+1
								, firstNARevision - revision);
		}
		else
		{
			// we must fill this range! Otherwise, we will stumble
			// over this gap and will try to fetch the data again
			// to no avail ... causing an endless loop.

			MakeRangeIterable ( currentPath->GetBasePath()
							  , revision+1
							  , firstNARevision - revision);
		}
	}

	// due to renames / copies, we may continue on a different path

	if (followRenames)
	{
		CCopyFollowingLogIterator iterator (cache, revision, *currentPath);
		iterator.Advance();

		*currentPath = iterator.GetPath();
	}

	// the first revision we may not have information about is the one
	// immediately preceding the on we just received from the server

	firstNARevision = revision-1;

	// hand on to the original log receiver

	if (receiver != NULL)
        if (revs_only)
        {
            static const CString emptyString;
    		receiver->ReceiveLog (NULL, rev, emptyString, 0, emptyString);
        }
        else
        {
    		receiver->ReceiveLog (changes, rev, author, timeStamp, message);
            changes = NULL;
        }

    // clean up

    if (changes != NULL)
    {
		for (INT_PTR i = 0, count = changes->GetCount(); i < count; ++i)
			delete changes->GetAt (i);

        delete changes;
    }
}

// actually call SVN
// return the last revision sent to the receiver

revision_t 
CCacheLogQuery::CLogFiller::FillLog ( CCachedLogInfo* cache
								    , const CStringA& URL
								    , ILogQuery* svnQuery
								    , revision_t startRevision
									, revision_t endRevision
									, const CDictionaryBasedTempPath& startPath
									, int limit
									, bool strictNodeHistory
								    , ILogReceiver* receiver
                                    , bool revs_only)
{
	this->cache = cache;
	this->URL = URL;
	this->svnQuery = svnQuery;
	this->receiver = receiver;
    this->revs_only = revs_only;

	firstNARevision = startRevision;
	currentPath.reset (new CDictionaryBasedTempPath (startPath));
	followRenames = !strictNodeHistory;

    // full path to be passed to SVN.
    // don't append a trailing "/", if the path is empty (i.e. root)

	CTSVNPath path;
    if (startPath.IsRoot())
	    path.SetFromSVN (URL);
    else
	    path.SetFromSVN (URL + startPath.GetPath().c_str());

	svnQuery->Log ( CTSVNPathList (path)
				  , static_cast<long>(startRevision)
				  , static_cast<long>(startRevision)
				  , static_cast<long>(endRevision)
			      , limit
				  , strictNodeHistory
				  , this
                  , false);

	if (firstNARevision == startRevision) 
	{
		// the log was empty

		if (currentPath->IsFullyCachedPath())
		{
			cache->AddSkipRange ( currentPath->GetBasePath()
								, endRevision
								, startRevision - endRevision + 1);
		}
		else
		{
			MakeRangeIterable ( currentPath->GetBasePath()
							  , endRevision
							  , startRevision - endRevision + 1);
		}
	}

	return firstNARevision+1;
}

// when asking SVN to fill our cache, we want it to cover
// the missing revisions as well as the ones that we already
// know to have no info for this path. Otherwise, we may end
// up creating a lot of queries for paths that are seldom
// modified.

revision_t 
CCacheLogQuery::NextAvailableRevision ( const CDictionaryBasedTempPath& path
									  , revision_t startRevision
									  , revision_t endRevision) const
{
	const CRevisionIndex& revisions = cache->GetRevisions();
	revision_t lastRevisionToCheck = min ( endRevision
										 , revisions.GetFirstRevision());

	while ((startRevision >= endRevision) && (startRevision != NO_REVISION))
	{
		// skip known revisions that are irrelevant for path

		CStrictLogIterator iterator (cache, startRevision, path);
		iterator.Retry();
		startRevision = iterator.GetRevision();

		// found the next cache entry for this path?

		if (!iterator.DataIsMissing())
			return startRevision+1;

		// skip N/A revisions

		while (   (startRevision >= lastRevisionToCheck) 
			   && (startRevision != NO_REVISION)
			   && (revisions[startRevision] == NO_INDEX))
		{
			--startRevision;
		}
	}

	// there is no cached data available for this path and revision range

	return endRevision-1;
}

// Determine an end-revision that would fill many cache gaps efficiently

revision_t CCacheLogQuery::FindOldestGap ( const CDictionaryBasedTempPath& path
									     , revision_t startRevision
									     , revision_t endRevision) const
{
	// consider the following trade-off:
	// 1 server round trip takes about as long as receiving
	// 50 log entries and / or 200 changes
	//
	// -> filling cache data gaps of up to 50 revisions
	//    (or 200 changes) apart with a single fetch should 
    //    result in close-to-optimal log performance.

	enum 
    {
        RECEIVE_TO_ROUNTRIP_TRADEOFF_REVS = 50,
        RECEIVE_TO_ROUNTRIP_TRADEOFF_CHANGES = 200
    };

	// find the next "long" section of consecutive cached log info

	revision_t lastMissing = startRevision;

    const CRevisionInfoContainer& logInfo = cache->GetLogInfo();
	const CRevisionIndex& revisions = cache->GetRevisions();
	revision_t lastRevisionToCheck = min ( endRevision
										 , revisions.GetFirstRevision());

    // length of the current sequence of known data

    size_t knownSequenceLength = 0;
    size_t knownSequenceChanges = 0;

	while (   (startRevision >= endRevision) 
           && (startRevision != NO_REVISION)
           && (knownSequenceLength < RECEIVE_TO_ROUNTRIP_TRADEOFF_REVS)
           && (knownSequenceChanges < RECEIVE_TO_ROUNTRIP_TRADEOFF_CHANGES))
	{
		// skip known revisions that are irrelevant for path

		CStrictLogIterator iterator (cache, startRevision, path);
		iterator.Retry();
		startRevision = iterator.GetRevision();

		// found the next cache entry for this path?

		if (iterator.DataIsMissing())
		{
			// move the "last gap" pointer, 
            // restart the sequence counting (until we found the first existing)

			lastMissing = startRevision;
            knownSequenceLength = 0;
            knownSequenceChanges = 0;
		}
		else
		{
			// another known entry 

            ++knownSequenceLength;

            index_t revIndex = revisions[startRevision];
            knownSequenceChanges += logInfo.GetChangesEnd (revIndex) 
                                  - logInfo.GetChangesBegin (revIndex);
		}

        // try the next revision

        --startRevision;
	}

	// fetch everything down to and including the lowest missing revision 
    // scanned (e.g. either the end of the log or before the first large 
    // consecutive block of cached revision data for this path)

    return lastMissing == NO_REVISION ? 0 : lastMissing;
}

// ask SVN to fill the log -- at least a bit
// Possibly, it will stop long before endRevision and limit!

revision_t CCacheLogQuery::FillLog ( revision_t startRevision
								   , revision_t endRevision
								   , const CDictionaryBasedTempPath& startPath
								   , int limit
								   , bool strictNodeHistory
								   , ILogReceiver* receiver
								   , bool revs_only)
{
	// don't try to get a full log; just enough to continue our search

	revision_t nextAvailable = NextAvailableRevision ( startPath
													 , startRevision
													 , endRevision);

	// propose a (possibly) different end-revision to cover more gaps
	// within the desired range (receiving duplicate intermediate
	// log info is less expensive than starting a new log query)

	revision_t cacheOptimalEndRevision = FindOldestGap ( startPath
                                                       , startRevision
													   , endRevision);

	// extend the requested range, if that is probably more efficient
	// (fill many small gaps at once)

	endRevision = min (nextAvailable+1, cacheOptimalEndRevision);

	// now, fill the cache (somewhat) and forward to the receiver

	return CLogFiller().FillLog ( cache
								, URL
								, svnQuery
								, startRevision
								, max (min (startRevision, endRevision), 1)
								, startPath
								, limit
								, strictNodeHistory
								, receiver
                                , revs_only);
}

// fill the receiver's change list buffer 

std::auto_ptr<LogChangedPathArray>
CCacheLogQuery::GetChanges ( CRevisionInfoContainer::CChangesIterator& first
						   , CRevisionInfoContainer::CChangesIterator& last)
{
	std::auto_ptr<LogChangedPathArray> result (new LogChangedPathArray);

	for (; first != last; ++first)
	{
		// find the item in the hash

		std::auto_ptr<LogChangedPath> changedPath (new LogChangedPath);

		// extract the path name
        // use map to cache results (speed-up and reduce memory footprint)

        CDictionaryBasedPath path = first.GetPath();
        TID2String::const_iterator iter (pathToStringMap.find (path.GetIndex()));
        if (iter == pathToStringMap.end())
        {
    		changedPath->sPath = SVN::MakeUIUrlOrPath (path.GetPath().c_str());
            pathToStringMap.insert (path.GetIndex(), changedPath->sPath);
        }
        else
        {
            changedPath->sPath = *iter;
        }

		// decode the action

		CRevisionInfoContainer::TChangeAction action = first.GetAction();
		changedPath->action = (DWORD)action / 4;

		// decode copy-from info

		if (   first.HasFromPath()
			&& (first.GetFromRevision() != NO_REVISION))
		{
			std::string path = first.GetFromPath().GetPath();

			changedPath->lCopyFromRev = first.GetFromRevision();
			changedPath->sCopyFromPath 
				= SVN::MakeUIUrlOrPath (path.c_str());
		}
		else
		{
			changedPath->lCopyFromRev = 0;
		}

		result->Add (changedPath.release());
	} 

	return result;
}

// crawl the history and forward it to the receiver

void CCacheLogQuery::InternalLog ( revision_t startRevision
								 , revision_t endRevision
								 , const CDictionaryBasedTempPath& startPath
								 , int limit
								 , bool strictNodeHistory
								 , ILogReceiver* receiver
                                 , bool revs_only)
{
    // clear string translating caches

    authorToStringMap.clear();
    pathToStringMap.clear();

	// create the right iterator

	std::auto_ptr<ILogIterator> iterator;
	if (strictNodeHistory)
		iterator.reset (new CStrictLogIterator ( cache
											   , startRevision
											   , startPath));
	else
		iterator.reset (new CCopyFollowingLogIterator ( cache
													  , startRevision
													  , startPath));

	// find first suitable entry or cache gap

	iterator->Retry();

	// report starts at startRevision or earlier revision

	revision_t lastReported = startRevision+1;

	// we cannot receive logs for rev 0 (or earlier)

	if (endRevision < 1)
		endRevision = 1;

	// crawl & update the cache, report entries found

	while ((iterator->GetRevision() >= endRevision) && !iterator->EndOfPath())
	{
		if (iterator->DataIsMissing())
		{
			// we must not fetch revisions twice
			// (this may cause an indefinite loop)

			assert (iterator->GetRevision() < lastReported);

			// our cache is incomplete -> fill it.
			// Report entries immediately to the receiver 
			// (as to allow the user to cancel this action).

			lastReported = FillLog ( iterator->GetRevision()
								   , endRevision
								   , iterator->GetPath()
								   , limit
								   , strictNodeHistory
								   , receiver
								   , revs_only);

			iterator->Retry();
		}
		else
		{
			// found an entry. Report it if not already done.

			revision_t revision = iterator->GetRevision();
			if ((revision < lastReported) && (receiver != NULL))
			{
                if (revs_only)
                {
                    // just notify the receiver that we made some progress

                    static const CString emptyString;
				    receiver->ReceiveLog ( NULL
									     , revision
									     , emptyString
									     , 0
									     , emptyString);
                }
                else
                {
				    index_t logIndex = cache->GetRevisions()[revision];
				    const CRevisionInfoContainer& logInfo = cache->GetLogInfo();

                    // extract data for this revision

                    // author

                    CString author;
                    index_t authorID = logInfo.GetAuthorID (logIndex);
                    TID2String::const_iterator iter = authorToStringMap.find (authorID);
                    if (iter == authorToStringMap.end())
                    {
				        author = CUnicodeUtils::GetUnicode (logInfo.GetAuthor (logIndex));
                        authorToStringMap.insert (authorID, author);
                    }
                    else
                    {
                        author = *iter;
                    }

                    // comment

				    CStringA comment = logInfo.GetComment (logIndex).c_str();

                    // change list

				    std::auto_ptr<LogChangedPathArray> changes
					    = GetChanges ( logInfo.GetChangesBegin (logIndex)
								     , logInfo.GetChangesEnd (logIndex));

                    // send it to the receiver

				    receiver->ReceiveLog ( changes.release()
									     , revision
									     , author
									     , logInfo.GetTimeStamp (logIndex)
									     , CUnicodeUtils::GetUnicode (comment));
                }
			}

			// enough?

			if ((limit != 0) && (--limit == 0))
				return;
			else
				iterator->Advance();
		}
	}
}

// follow copy history until the startRevision is reached

CDictionaryBasedTempPath CCacheLogQuery::TranslatePegRevisionPath 
	( revision_t pegRevision
	, revision_t startRevision
	, const CDictionaryBasedTempPath& startPath)
{
	CCopyFollowingLogIterator iterator (cache, pegRevision, startPath);
	iterator.Retry();

	while ((iterator.GetRevision() > startRevision) && !iterator.EndOfPath())
	{
		if (iterator.DataIsMissing())
			FillLog ( iterator.GetRevision()
					, startRevision
					, iterator.GetPath()
					, 0
					, false
					, NULL
					, true);

		iterator.Advance();
	}

	return iterator.GetPath();
}

// extract the repository-relative path of the URL / file name
// and open the cache

CDictionaryBasedTempPath CCacheLogQuery::GetRelativeRepositoryPath (SVNInfoData& info)
{
	// resolve URL

	URL.Empty();
	if (info.reposUUID.IsEmpty())
	{
		URL = CUnicodeUtils::GetUTF8 
				(repositoryInfoCache->GetRepositoryRootAndUUID 
                    ( CTSVNPath (info.url)
					, info.reposUUID));
	}

	if (URL.IsEmpty())
		URL = CUnicodeUtils::GetUTF8 (info.reposRoot);

	// load / create cache

	assert(!info.reposUUID.IsEmpty());
	if (caches != NULL)
	{
		cache = caches->GetCache (info.reposUUID);
	}
	else
	{
		delete tempCache;
		tempCache = new CCachedLogInfo(L"");

		cache = tempCache;
	}

	// workaround for 1.2.x (and older) working copies

	if (URL.IsEmpty())
	{
		URL = CUnicodeUtils::GetUTF8 
				(repositoryInfoCache->GetRepositoryRoot (CTSVNPath (info.url)));
	}

	// get path object 
	// (URLs are always escaped, so we must unescape them)

	CStringA relPath = CUnicodeUtils::GetUTF8 (info.url).Mid (URL.GetLength());
	relPath = CPathUtils::PathUnescape (relPath);

	const CPathDictionary* paths = &cache->GetLogInfo().GetPaths();
	return CDictionaryBasedTempPath (paths, (const char*)relPath);
}

// get UUID & repository-relative path

SVNInfoData& CCacheLogQuery::GetRepositoryInfo ( const CTSVNPath& path
											   , const SVNRev& pegRevision
 								               , SVNInfoData& baseInfo
									           , SVNInfoData& headInfo) const
{
	// already known?

	if (baseInfo.IsValid())
		return baseInfo;

	if (headInfo.IsValid())
		return headInfo;

	// look it up

	SVNInfo info;
	if (path.IsUrl())
	{
		headInfo = *info.GetFirstFileInfo (path, pegRevision, pegRevision);
		return headInfo;
	}
	else
	{
		baseInfo = *info.GetFirstFileInfo (path, SVNRev(), SVNRev());
		return baseInfo;
	}
}

// decode special revisions:
// base / head must be initialized with NO_REVISION
// and will be used to cache these values.

revision_t CCacheLogQuery::DecodeRevision ( const CTSVNPath& path
			  							  , const SVNRev& revision
										  , SVNInfoData& baseInfo
										  , SVNInfoData& headInfo) const
{
	if (!revision.IsValid())
		throw SVNError ( SVN_ERR_CLIENT_BAD_REVISION
					   , "Invalid revision passed to Log().");

	// efficiently decode standard cases: revNum, HEAD, BASE/WORKING

	switch (revision.GetKind())
	{
	case svn_opt_revision_number:
		return static_cast<LONG>(revision);

	case svn_opt_revision_head:
		if (!headInfo.IsValid())
		{
			SVNInfo info;
			const SVNInfoData * pHeadInfo = info.GetFirstFileInfo (path, SVNRev(), revision);
			if (pHeadInfo)
				headInfo = *pHeadInfo;
			else
				throw SVNError(info.GetError());
		}
		return static_cast<LONG>(headInfo.rev);

	case svn_opt_revision_base:
	case svn_opt_revision_working:
		if (!baseInfo.IsValid())
		{
			SVNInfo info;
			const SVNInfoData * pBaseInfo = info.GetFirstFileInfo (path, SVNRev(), revision);
			if (pBaseInfo)
				baseInfo = *pBaseInfo;
			else
				throw SVNError(info.GetError());
		}

		return static_cast<LONG>(baseInfo.rev);
	}

	// more unusual cases:

	SVNInfo infoProvider;
	const SVNInfoData* info 
		= infoProvider.GetFirstFileInfo (path, SVNRev(), revision);
	if (info)
		return static_cast<LONG>(info->rev);
	throw SVNError(infoProvider.GetError());	
}

// get the (exactly) one path from targets
// throw an exception, if there are none or more than one

CTSVNPath CCacheLogQuery::GetPath (const CTSVNPathList& targets) const
{
	if (targets.GetCount() != 1)
		throw SVNError ( SVN_ERR_INCORRECT_PARAMS
					   , "Must specify exactly one path to get the log from.");

	return targets [0];
}

// construction / destruction

CCacheLogQuery::CCacheLogQuery (CLogCachePool* caches, ILogQuery* svnQuery)
	: caches (caches)
    , repositoryInfoCache (&caches->GetRepositoryInfo())
    , cache (NULL)
	, tempCache (NULL)
	, URL()
	, svnQuery (svnQuery)
{
}

CCacheLogQuery::~CCacheLogQuery(void)
{
	delete tempCache;
}

// query a section from log for multiple paths
// (special revisions, like "HEAD", supported)

void CCacheLogQuery::Log ( const CTSVNPathList& targets
						 , const SVNRev& peg_revision
						 , const SVNRev& start
						 , const SVNRev& end
						 , int limit
						 , bool strictNodeHistory
						 , ILogReceiver* receiver
                         , bool revs_only)
{
	// the path to log for

	CTSVNPath path = GetPath (targets);

	// decode revisions

	SVNInfoData baseInfo;
	SVNInfoData headInfo;

	revision_t startRevision = DecodeRevision (path, start, baseInfo, headInfo);
	revision_t endRevision = DecodeRevision (path, end, baseInfo, headInfo);

	// The svn_client_log3() API defaults the peg revision to HEAD for URLs 
	// and WC for local paths if it isn't set explicitly.

	revision_t pegRevision = 0;
	if (!peg_revision.IsValid())
	{
		if (path.IsUrl())
			pegRevision = DecodeRevision (path, SVNRev::REV_HEAD, baseInfo, headInfo);
		else
			pegRevision = DecodeRevision (path, SVNRev::REV_WC, baseInfo, headInfo);
	}
	else
		pegRevision = DecodeRevision (path, peg_revision, baseInfo, headInfo);

	// order revisions

	if (endRevision > startRevision)
		std::swap (endRevision, startRevision);

	if (pegRevision < startRevision)
		pegRevision = startRevision;

	// load cache and find path to start from
    // (don't get the repo info from SVN, if it had to be fetched from the server
    //  -> let GetRelativeRepositoryPath() use our repository property cache)

    SVNInfoData& info = path.IsUrl()
        ? headInfo 
        : GetRepositoryInfo (path, peg_revision, baseInfo, headInfo);

    if (info.url.IsEmpty())
        info.url = path.GetSVNPathString();

	CDictionaryBasedTempPath startPath 
		= TranslatePegRevisionPath ( pegRevision
								   , startRevision
								   , GetRelativeRepositoryPath (info));

	// do it 

	InternalLog ( startRevision
				, endRevision
				, startPath
				, limit
				, strictNodeHistory
				, receiver
                , revs_only);
}

// access to the cache

CCachedLogInfo* CCacheLogQuery::GetCache()
{
	assert (cache != NULL);
	return cache;
}
