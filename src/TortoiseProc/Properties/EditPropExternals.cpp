﻿// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2010-2015, 2020-2021 - TortoiseSVN

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
#include "stdafx.h"
#include "TortoiseProc.h"
#include "EditPropExternals.h"
#include "EditPropExternalsValue.h"
#include "SVN.h"
#include "SVNLogHelper.h"
#include "SVNInfo.h"
#include "AppUtils.h"
#include "ProgressDlg.h"
#include "IconMenu.h"

#define CMD_ADJUST 1
#define CMD_FETCH_AND_ADJUST 2

int      CEditPropExternals::m_nSortedColumn = -1;
bool     CEditPropExternals::m_bAscending = false;

IMPLEMENT_DYNAMIC(CEditPropExternals, CResizableStandAloneDialog)

CEditPropExternals::CEditPropExternals(CWnd* pParent /*=NULL*/)
    : CResizableStandAloneDialog(CEditPropExternals::IDD, pParent)
    , EditPropBase()
{
    m_columnbuf[0] = 0;
}

CEditPropExternals::~CEditPropExternals()
{
}

void CEditPropExternals::DoDataExchange(CDataExchange* pDX)
{
    CResizableStandAloneDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXTERNALSLIST, m_ExtList);
}


BEGIN_MESSAGE_MAP(CEditPropExternals, CResizableStandAloneDialog)
    ON_BN_CLICKED(IDC_ADD, &CEditPropExternals::OnBnClickedAdd)
    ON_BN_CLICKED(IDC_EDIT, &CEditPropExternals::OnBnClickedEdit)
    ON_BN_CLICKED(IDC_REMOVE, &CEditPropExternals::OnBnClickedRemove)
    ON_NOTIFY(LVN_GETDISPINFO, IDC_EXTERNALSLIST, &CEditPropExternals::OnLvnGetdispinfoExternalslist)
    ON_NOTIFY(NM_DBLCLK, IDC_EXTERNALSLIST, &CEditPropExternals::OnNMDblclkExternalslist)
    ON_BN_CLICKED(IDHELP, &CEditPropExternals::OnBnClickedHelp)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_EXTERNALSLIST, &CEditPropExternals::OnLvnItemchangedExternalslist)
    ON_BN_CLICKED(IDC_FINDHEAD, &CEditPropExternals::OnBnClickedFindhead)
    ON_WM_CONTEXTMENU()
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CEditPropExternals::OnHdnItemclickExternalslist)
END_MESSAGE_MAP()


BOOL CEditPropExternals::OnInitDialog()
{
    CResizableStandAloneDialog::OnInitDialog();
    CAppUtils::MarkWindowAsUnpinnable(m_hWnd);

    ExtendFrameIntoClientArea(IDC_EXTERNALSLIST);
    m_aeroControls.SubclassControl(this, IDC_ADD);
    m_aeroControls.SubclassControl(this, IDC_EDIT);
    m_aeroControls.SubclassControl(this, IDC_REMOVE);
    m_aeroControls.SubclassControl(this, IDC_FINDHEAD);
    m_aeroControls.SubclassOkCancelHelp(this);

    ATLASSERT(m_pathList.GetCount() == 1);

    SVN svn;
    m_url = CTSVNPath(svn.GetURLFromPath(m_pathList[0]));
    m_repoRoot = CTSVNPath(svn.GetRepositoryRoot(m_pathList[0]));

    m_externals.Add(m_pathList[0], m_PropValue, false);

    DWORD exStyle = LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER;
    m_ExtList.SetExtendedStyle(exStyle);

    SetWindowTheme(m_ExtList.GetSafeHwnd(), L"Explorer", NULL);

    m_ExtList.SetRedraw(false);
    m_ExtList.DeleteAllItems();

    int c = m_ExtList.GetHeaderCtrl()->GetItemCount()-1;
    while (c>=0)
        m_ExtList.DeleteColumn(c--);
    CString temp;
    temp.LoadString(IDS_STATUSLIST_COLFILE);
    m_ExtList.InsertColumn(0, temp);
    temp.LoadString(IDS_STATUSLIST_COLURL);
    m_ExtList.InsertColumn(1, temp);
    temp.LoadString(IDS_EXTERNALS_PEG);
    m_ExtList.InsertColumn(2, temp);
    temp.LoadString(IDS_EXTERNALS_OPERATIVE);
    m_ExtList.InsertColumn(3, temp);
    temp.LoadString(IDS_EXTERNALS_HEADREV);
    m_ExtList.InsertColumn(4, temp);
    m_ExtList.SetItemCountEx((int)m_externals.size());

    CRect rect;
    m_ExtList.GetClientRect(&rect);
    m_ExtList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
    int cx = (rect.Width()-240-m_ExtList.GetColumnWidth(0));
    m_ExtList.SetColumnWidth(1, cx);
    m_ExtList.SetColumnWidth(2, 80);
    m_ExtList.SetColumnWidth(3, 80);
    m_ExtList.SetColumnWidth(4, 80);

    m_ExtList.SetRedraw(true);

    CString sWindowTitle;
    GetWindowText(sWindowTitle);
    CAppUtils::SetWindowTitle(m_hWnd, m_pathList.GetCommonRoot().GetUIPathString(), sWindowTitle);

    AddAnchor(IDC_EXTERNALSLIST, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_ADD, BOTTOM_LEFT);
    AddAnchor(IDC_EDIT, BOTTOM_LEFT);
    AddAnchor(IDC_REMOVE, BOTTOM_LEFT);
    AddAnchor(IDC_FINDHEAD, BOTTOM_LEFT);
    AddAnchor(IDOK, BOTTOM_RIGHT);
    AddAnchor(IDCANCEL, BOTTOM_RIGHT);
    AddAnchor(IDHELP, BOTTOM_RIGHT);
    EnableSaveRestore(L"EditPropsExternals");

    return TRUE;
}

void CEditPropExternals::OnOK()
{
    UpdateData();
    m_bChanged = true;
    m_PropValue = m_externals.GetValue(m_pathList[0]);

    CResizableStandAloneDialog::OnOK();
}

void CEditPropExternals::OnBnClickedAdd()
{
    CEditPropExternalsValue dlg;
    dlg.SetURL(m_url);
    dlg.SetRepoRoot(m_repoRoot);
    dlg.SetPathList(m_pathList);
    if (dlg.DoModal() == IDOK)
    {
        SVNExternal ext = dlg.GetExternal();
        ext.path = m_pathList[0];
        m_externals.push_back(ext);
        m_ExtList.SetItemCountEx((int)m_externals.size());
        m_ExtList.Invalidate();
    }
}

void CEditPropExternals::OnBnClickedEdit()
{
    if (m_ExtList.GetSelectedCount() == 1)
    {
        POSITION pos = m_ExtList.GetFirstSelectedItemPosition();
        size_t selIndex = m_ExtList.GetNextSelectedItem(pos);
        SVNExternal ext = m_externals[selIndex];
        if (ext.revision.kind == svn_opt_revision_unspecified)
            ext.revision = ext.origRevision;

        CEditPropExternalsValue dlg;
        dlg.SetURL(m_url);
        dlg.SetRepoRoot(m_repoRoot);
        dlg.SetExternal(ext);
        dlg.SetPathList(m_pathList);
        if (dlg.DoModal() == IDOK)
        {
            ext = dlg.GetExternal();
            m_externals[selIndex] = ext;
            m_ExtList.Invalidate();
        }
    }
}

void CEditPropExternals::OnBnClickedRemove()
{
    std::vector<size_t> indexestoremove;
    size_t selIndex = 0;
    POSITION pos = m_ExtList.GetFirstSelectedItemPosition();
    while (pos)
    {
        selIndex = m_ExtList.GetNextSelectedItem(pos);
        if (m_externals.size() > selIndex)
            indexestoremove.push_back(selIndex);
    }
    for (auto it = indexestoremove.crbegin(); it != indexestoremove.crend(); ++it)
    {
        m_externals.erase(m_externals.begin() + *it);
    }
    m_ExtList.SetItemCountEx((int)m_externals.size());
    m_ExtList.Invalidate();
}

void CEditPropExternals::OnNMDblclkExternalslist(NMHDR * pNMHDR, LRESULT *pResult)
{
    // a double click on an entry in the revision list has happened
    LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
    *pResult = 0;

    if ((lpnmitem->iItem >= 0)&&(lpnmitem->iItem < (int)m_externals.size()))
    {
        SVNExternal ext = m_externals[lpnmitem->iItem];
        if (ext.revision.kind == svn_opt_revision_unspecified)
            ext.revision = ext.origRevision;

        CEditPropExternalsValue dlg;
        dlg.SetURL(m_url);
        dlg.SetRepoRoot(m_repoRoot);
        dlg.SetExternal(ext);
        dlg.SetPathList(m_pathList);
        if (dlg.DoModal() == IDOK)
        {
            ext = dlg.GetExternal();
            m_externals[lpnmitem->iItem] = ext;
            m_ExtList.Invalidate();
        }
    }
}

void CEditPropExternals::OnLvnGetdispinfoExternalslist(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
    *pResult = 0;

    if (pDispInfo)
    {
        if ((pDispInfo->item.iItem >= 0)&&(pDispInfo->item.iItem < (int)m_externals.size()))
        {
            SVNExternal ext = m_externals[pDispInfo->item.iItem];
            if (pDispInfo->item.mask & LVIF_TEXT)
            {
                switch (pDispInfo->item.iSubItem)
                {
                case 0: // folder or file
                    {
                        lstrcpyn(m_columnbuf, ext.targetDir, pDispInfo->item.cchTextMax - 1);
                    }
                    break;
                case 1: // url
                    {
                        lstrcpyn(m_columnbuf, ext.url, pDispInfo->item.cchTextMax - 1);
                        int cWidth = m_ExtList.GetColumnWidth(1);
                        cWidth = max(14, cWidth-14);
                        CDC * pDC = m_ExtList.GetDC();
                        if (pDC != NULL)
                        {
                            CFont * pFont = pDC->SelectObject(m_ExtList.GetFont());
                            PathCompactPath(pDC->GetSafeHdc(), m_columnbuf, cWidth);
                            pDC->SelectObject(pFont);
                            ReleaseDC(pDC);
                        }
                    }
                    break;
                case 2: // peg
                    if ((ext.pegRevision.kind == svn_opt_revision_number) && (ext.pegRevision.value.number >= 0))
                        swprintf_s(m_columnbuf, L"%ld", ext.pegRevision.value.number);
                    else
                        m_columnbuf[0] = 0;
                    break;
                case 3: // operative
                    if ((ext.revision.kind == svn_opt_revision_number) && (ext.revision.value.number >= 0) && (ext.revision.value.number != ext.pegRevision.value.number))
                        swprintf_s(m_columnbuf, L"%ld", ext.revision.value.number);
                    else
                        m_columnbuf[0] = 0;
                    break;
                case 4: // head revision
                    if (ext.headRev != SVN_INVALID_REVNUM)
                        swprintf_s(m_columnbuf, L"%ld", ext.headRev);
                    else
                        m_columnbuf[0] = 0;
                    break;
                default:
                    m_columnbuf[0] = 0;
                }
                pDispInfo->item.pszText = m_columnbuf;
            }
        }
    }
}

void CEditPropExternals::OnBnClickedHelp()
{
    OnHelp();
}


void CEditPropExternals::OnLvnItemchangedExternalslist(NMHDR * /*pNMHDR*/, LRESULT *pResult)
{
    //LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    DialogEnableWindow(IDC_REMOVE, m_ExtList.GetSelectedCount());
    DialogEnableWindow(IDC_EDIT, m_ExtList.GetSelectedCount() == 1);
    *pResult = 0;
}


void CEditPropExternals::OnBnClickedFindhead()
{
    CProgressDlg progDlg;
    progDlg.ShowModal(m_hWnd, TRUE);
    progDlg.SetTitle(IDS_EDITPROPS_PROG_FINDHEADTITLE);
    progDlg.SetLine(1, CString(MAKEINTRESOURCE(IDS_EDITPROPS_PROG_FINDHEADROOTS)));
    DWORD count = 0;
    DWORD total = (DWORD)m_externals.size()*4;
    SVN svn;
    svn.SetPromptParentWindow(m_hWnd);
    SVNInfo svnInfo;
    svnInfo.SetPromptParentWindow(m_hWnd);
    for (auto it = m_externals.begin(); it != m_externals.end(); ++it)
    {
        progDlg.SetProgress(count++, total);
        if (progDlg.HasUserCancelled())
            break;
        if (it->root.IsEmpty())
        {
            CTSVNPath p = it->path;
            p.AppendPathString(it->targetDir);
            it->root = svn.GetRepositoryRoot(p);
        }
    }

    progDlg.SetLine(1, CString(MAKEINTRESOURCE(IDS_EDITPROPS_PROG_FINDHEADREVS)));
    SVNLogHelper logHelper;
    for (auto it = m_externals.begin(); it != m_externals.end(); ++it)
    {
        progDlg.SetProgress(count, total);
        progDlg.SetLine(2, it->url, true);
        if (progDlg.HasUserCancelled())
            break;
        count += 4;
        if (!it->root.IsEmpty())
        {
            auto youngestRev = logHelper.GetYoungestRev(CTSVNPath(it->fullUrl));
            if (!youngestRev.IsValid())
                it->headRev = svn.GetHEADRevision(CTSVNPath(it->fullUrl), true);
            else
                it->headRev = youngestRev;
        }
    }
    progDlg.Stop();
    m_ExtList.Invalidate();
}


void CEditPropExternals::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
    int selIndex = m_ExtList.GetSelectionMark();
    if (selIndex < 0)
        return; // nothing selected, nothing to do with a context menu
    int selCount = m_ExtList.GetSelectedCount();
    if (selCount <= 0)
        return; // nothing selected, nothing to do with a context menu

    // if the context menu is invoked through the keyboard, we have to use
    // a calculated position on where to anchor the menu on
    if ((point.x == -1) && (point.y == -1))
    {
        CRect rect;
        m_ExtList.GetItemRect(selIndex, &rect, LVIR_LABEL);
        m_ExtList.ClientToScreen(&rect);
        point = rect.CenterPoint();
    }

    bool haveHead = true;

    POSITION pos = m_ExtList.GetFirstSelectedItemPosition();
    while (pos)
    {
        int index = m_ExtList.GetNextSelectedItem(pos);
        if ((index >= 0)&&(index < (int)m_externals.size()))
        {
            if (m_externals[index].headRev == SVN_INVALID_REVNUM)
            {
                haveHead = false;
                break;
            }
        }
    }

    CIconMenu popup;
    if (popup.CreatePopupMenu())
    {
        if (haveHead)
            popup.AppendMenuIcon(CMD_ADJUST, IDS_EDITPROPS_ADJUST_TO_HEAD);
        else
            popup.AppendMenuIcon(CMD_FETCH_AND_ADJUST, IDS_EDITPROPS_FETCH_AND_ADJUST_TO_HEAD);

        int cmd = popup.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_NONOTIFY | TPM_RIGHTBUTTON, point.x, point.y, this, 0);

        switch (cmd)
        {
        case CMD_FETCH_AND_ADJUST:
            {
                SVN svn;
                svn.SetPromptParentWindow(m_hWnd);
                SVNInfo svnInfo;
                svnInfo.SetPromptParentWindow(m_hWnd);
                SVNLogHelper logHelper;
                CProgressDlg progDlg;
                progDlg.ShowModal(m_hWnd, TRUE);
                progDlg.SetTitle(IDS_EDITPROPS_PROG_FINDHEADTITLE);
                progDlg.SetLine(1, CString(MAKEINTRESOURCE(IDS_EDITPROPS_PROG_FINDHEADREVS)));
                DWORD count = 0;
                DWORD total = m_ExtList.GetSelectedCount();
                POSITION p = m_ExtList.GetFirstSelectedItemPosition();
                while (p)
                {
                    int index = m_ExtList.GetNextSelectedItem(p);
                    progDlg.SetProgress(count++, total);
                    if ((index >= 0)&&(index < (int)m_externals.size()))
                    {
                        progDlg.SetLine(2, m_externals[index].url, true);
                        if (m_externals[index].headRev == SVN_INVALID_REVNUM)
                        {
                            if (m_externals[index].root.IsEmpty())
                            {
                                CTSVNPath path_ = m_externals[index].path;
                                path_.AppendPathString(m_externals[index].targetDir);
                                m_externals[index].root = svn.GetRepositoryRoot(path_);
                            }
                            auto fullurl = CTSVNPath(m_externals[index].fullUrl);
                            if (!fullurl.IsEmpty())
                            {
                                auto youngestRev = logHelper.GetYoungestRev(fullurl);
                                if (!youngestRev.IsValid())
                                    m_externals[index].headRev = svn.GetHEADRevision(fullurl, true);
                                else
                                    m_externals[index].headRev = youngestRev;
                            }
                        }
                    }
                }
                progDlg.Stop();
            }
            // intentional fall through
        case CMD_ADJUST:
            {
                POSITION p = m_ExtList.GetFirstSelectedItemPosition();
                while (p)
                {
                    int index = m_ExtList.GetNextSelectedItem(p);
                    if ((index >= 0)&&(index < (int)m_externals.size()))
                    {
                        if (m_externals[index].headRev != SVN_INVALID_REVNUM)
                        {
                            if (m_externals[index].revision.kind == svn_opt_revision_number)
                            {
                                m_externals[index].revision.value.number = -1;
                                m_externals[index].revision.kind = svn_opt_revision_unspecified;
                            }
                            m_externals[index].pegRevision.value.number = m_externals[index].headRev;
                            m_externals[index].pegRevision.kind = svn_opt_revision_number;
                        }
                    }
                }
                m_ExtList.Invalidate();
            }
            break;
        }
    }
}

void CEditPropExternals::OnHdnItemclickExternalslist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

    if (m_nSortedColumn == phdr->iItem)
        m_bAscending = !m_bAscending;
    else
        m_bAscending = TRUE;
    m_nSortedColumn = phdr->iItem;

    std::sort(m_externals.begin(), m_externals.end(), &CEditPropExternals::SortCompare);

    m_ExtList.SetRedraw(FALSE);
    m_ExtList.DeleteAllItems();
    m_ExtList.SetItemCountEx((int)m_externals.size());

    CHeaderCtrl * pHeader = m_ExtList.GetHeaderCtrl();
    HDITEM HeaderItem = { 0 };
    HeaderItem.mask = HDI_FORMAT;
    const int itemCount = pHeader->GetItemCount();
    for (int i = 0; i<itemCount; ++i)
    {
        pHeader->GetItem(i, &HeaderItem);
        HeaderItem.fmt &= ~(HDF_SORTDOWN | HDF_SORTUP);
        pHeader->SetItem(i, &HeaderItem);
    }
    pHeader->GetItem(m_nSortedColumn, &HeaderItem);
    HeaderItem.fmt |= (m_bAscending ? HDF_SORTUP : HDF_SORTDOWN);
    pHeader->SetItem(m_nSortedColumn, &HeaderItem);

    m_ExtList.SetRedraw(TRUE);

    *pResult = 0;
}

bool CEditPropExternals::SortCompare(const SVNExternal& Data1, const SVNExternal& Data2)
{
    int result = 0;
    switch (m_nSortedColumn)
    {
        case 0:     // file column
        result = Data1.targetDir.CompareNoCase(Data2.targetDir);
        break;
        case 1:     // url column
        result = Data1.url.CompareNoCase(Data2.url);
        break;
        case 2:     //peg revision column
        result = Data1.pegRevision.value.number < Data2.pegRevision.value.number;
        break;
        case 3:     // operative revision column
        result = Data1.revision.value.number < Data2.revision.value.number;
        break;
        case 4:     // head revision column
        result = Data1.headRev < Data2.headRev;
        break;
        default:
        break;
    }

    // Sort by path if everything else is equal
    if (result == 0)
    {
        result = Data1.path < Data2.path;
    }

    if (!m_bAscending)
        result = -result;
    return result < 0;
}

