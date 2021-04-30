﻿// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2010, 2012, 2021 - TortoiseSVN

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
#pragma once
#include "EditPropBase.h"
#include "StandAloneDlg.h"

class CEditPropNeedsLock : public CStandAloneDialog
    , public EditPropBase
{
    DECLARE_DYNAMIC(CEditPropNeedsLock)

public:
    CEditPropNeedsLock(CWnd* pParent = nullptr); // standard constructor
    ~CEditPropNeedsLock() override;

    enum
    {
        IDD = IDD_EDITPROPNEEDSLOCK
    };

protected:
    void         DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
    BOOL         OnInitDialog() override;
    void         OnOK() override;
    afx_msg void OnBnClickedProprecursive();
    afx_msg void OnBnClickedHelp();

    DECLARE_MESSAGE_MAP()

    INT_PTR DoModal() override { return CStandAloneDialog::DoModal(); }
};
