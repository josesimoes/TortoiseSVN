// TortoiseMerge - a Diff/Patch program

// Copyright (C) 2006, 2011-2014 - TortoiseSVN

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
#include "TortoiseMerge.h"
#include "FindDlg.h"


// CFindDlg dialog

IMPLEMENT_DYNAMIC(CFindDlg, CDialog)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CFindDlg::IDD, pParent)
    , m_pParent(pParent)
    , m_bTerminating(false)
    , m_bFindNext(false)
    , m_bMatchCase(FALSE)
    , m_bLimitToDiffs(FALSE)
    , m_bWholeWord(FALSE)
    , m_bSearchUp(FALSE)
    , m_FindMsg(0)
    , m_clrFindStatus(RGB(0, 0, 255))
    , m_regMatchCase(L"Software\\TortoiseMerge\\FindMatchCase", FALSE)
    , m_regLimitToDiffs(L"Software\\TortoiseMerge\\FindLimitToDiffs", FALSE)
    , m_regWholeWord(L"Software\\TortoiseMerge\\FindWholeWord", FALSE)
{
}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_MATCHCASE, m_bMatchCase);
    DDX_Check(pDX, IDC_LIMITTODIFFS, m_bLimitToDiffs);
    DDX_Check(pDX, IDC_WHOLEWORD, m_bWholeWord);
    DDX_Check(pDX, IDC_SEARCHUP, m_bSearchUp);
    DDX_Control(pDX, IDC_FINDCOMBO, m_FindCombo);
    DDX_Control(pDX, IDC_REPLACECOMBO, m_ReplaceCombo);
    DDX_Control(pDX, IDC_FINDSTATUS, m_FindStatus);
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
    ON_CBN_EDITCHANGE(IDC_FINDCOMBO, &CFindDlg::OnCbnEditchangeFindcombo)
    ON_CBN_EDITCHANGE(IDC_REPLACECOMBO, &CFindDlg::OnCbnEditchangeFindcombo)
    ON_BN_CLICKED(IDC_COUNT, &CFindDlg::OnBnClickedCount)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_REPLACE, &CFindDlg::OnBnClickedReplace)
    ON_BN_CLICKED(IDC_REPLACEALL, &CFindDlg::OnBnClickedReplaceall)
END_MESSAGE_MAP()


// CFindDlg message handlers

void CFindDlg::OnCancel()
{
    m_bTerminating = true;
    SetStatusText(L"");
    if (m_pParent)
        m_pParent->SendMessage(m_FindMsg);
    else if (GetParent())
        GetParent()->SendMessage(m_FindMsg);
    DestroyWindow();
}

void CFindDlg::PostNcDestroy()
{
    delete this;
}

void CFindDlg::OnOK()
{
    UpdateData();
    SetStatusText(L"");
    m_FindCombo.SaveHistory();
    m_regMatchCase = m_bMatchCase;
    m_regLimitToDiffs = m_bLimitToDiffs;
    m_regWholeWord = m_bWholeWord;

    if (m_FindCombo.GetString().IsEmpty())
        return;
    m_bFindNext = true;
    if (m_pParent)
        m_pParent->SendMessage(m_FindMsg);
    else if (GetParent())
        GetParent()->SendMessage(m_FindMsg);
    m_bFindNext = false;
}

BOOL CFindDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_FindMsg = RegisterWindowMessage(FINDMSGSTRING);

    m_bMatchCase    = (BOOL)(DWORD)m_regMatchCase;
    m_bLimitToDiffs = (BOOL)(DWORD)m_regLimitToDiffs;
    m_bWholeWord    = (BOOL)(DWORD)m_regWholeWord;
    UpdateData(FALSE);

    m_FindCombo.DisableTrimming();
    m_FindCombo.LoadHistory(L"Software\\TortoiseMerge\\History\\Find", L"Search");
    m_FindCombo.SetCurSel(0);

    m_ReplaceCombo.DisableTrimming();
    m_ReplaceCombo.LoadHistory(L"Software\\TortoiseMerge\\History\\Find", L"Replace");
    m_ReplaceCombo.SetCurSel(0);

    m_FindCombo.SetFocus();

    return FALSE;
}

void CFindDlg::OnCbnEditchangeFindcombo()
{
    UpdateData();
    GetDlgItem(IDOK)->EnableWindow(!m_FindCombo.GetString().IsEmpty());
    GetDlgItem(IDC_REPLACE)->EnableWindow(!m_ReplaceCombo.GetString().IsEmpty());
    GetDlgItem(IDC_REPLACEALL)->EnableWindow(!m_ReplaceCombo.GetString().IsEmpty());
}

void CFindDlg::OnBnClickedCount()
{
    UpdateData();
    SetStatusText(L"");
    m_FindCombo.SaveHistory();
    m_regMatchCase = m_bMatchCase;
    m_regLimitToDiffs = m_bLimitToDiffs;
    m_regWholeWord = m_bWholeWord;

    if (m_FindCombo.GetString().IsEmpty())
        return;
    m_bFindNext = true;
    if (m_pParent)
        m_pParent->SendMessage(m_FindMsg, FindType::Count);
    else if (GetParent())
        GetParent()->SendMessage(m_FindMsg, FindType::Count);
    m_bFindNext = false;
}

HBRUSH CFindDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
    switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        if (pWnd == &m_FindStatus)
        {
            HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
            pDC->SetTextColor(m_clrFindStatus);
            pDC->SetBkMode(TRANSPARENT);
            return hbr;
        }
    default:
        break;
    }
    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CFindDlg::SetStatusText(const CString& str, COLORREF color)
{
    m_clrFindStatus = color;
    m_FindStatus.SetWindowText(str);
}

void CFindDlg::OnBnClickedReplace()
{
    UpdateData();
    SetStatusText(L"");
    m_FindCombo.SaveHistory();
    m_ReplaceCombo.SaveHistory();
    m_regMatchCase = m_bMatchCase;
    m_regLimitToDiffs = m_bLimitToDiffs;
    m_regWholeWord = m_bWholeWord;

    if (m_ReplaceCombo.GetString().IsEmpty())
        return;
    m_bFindNext = true;
    if (m_pParent)
        m_pParent->SendMessage(m_FindMsg, FindType::Replace);
    else if (GetParent())
        GetParent()->SendMessage(m_FindMsg, FindType::Replace);
    m_bFindNext = false;
}


void CFindDlg::OnBnClickedReplaceall()
{
    UpdateData();
    SetStatusText(L"");
    m_FindCombo.SaveHistory();
    m_ReplaceCombo.SaveHistory();
    m_regMatchCase = m_bMatchCase;
    m_regLimitToDiffs = m_bLimitToDiffs;
    m_regWholeWord = m_bWholeWord;

    if (m_ReplaceCombo.GetString().IsEmpty())
        return;
    m_bFindNext = true;
    if (m_pParent)
        m_pParent->SendMessage(m_FindMsg, FindType::ReplaceAll);
    else if (GetParent())
        GetParent()->SendMessage(m_FindMsg, FindType::ReplaceAll);
    m_bFindNext = false;
}
