#include "StdAfx.h"
#include "Resource.h"
#include ".\bottomview.h"

IMPLEMENT_DYNCREATE(CBottomView, CBaseView)

CBottomView::CBottomView(void)
{
	m_pwndBottom = this;
	m_nStatusBarID = ID_INDICATOR_BOTTOMVIEW;
}

CBottomView::~CBottomView(void)
{
}

BOOL CBottomView::IsStateSelectable(CDiffData::DiffStates state)
{
	//The bottom view is not visible in one and two-way diff...
	if (!this->IsWindowVisible())
	{
		return FALSE;
	}

	//The bottom view is always "Merged" in three-way diff
	switch (state)
	{
	case CDiffData::DIFFSTATE_ADDED:
	case CDiffData::DIFFSTATE_REMOVED:
	case CDiffData::DIFFSTATE_CONFLICTED:
	case CDiffData::DIFFSTATE_CONFLICTEMPTY:
	case CDiffData::DIFFSTATE_CONFLICTADDED:
		return TRUE;
	default:
		return FALSE;
	} // switch (state) 
	return FALSE;
}

void CBottomView::OnContextMenu(CPoint point, int nLine)
{
	CMenu popup;
	if (popup.CreatePopupMenu())
	{
#define ID_USETHEIRBLOCK 1
#define ID_USEYOURBLOCK 2
		UINT uEnabled = MF_ENABLED;
		if ((m_nSelBlockStart == -1)||(m_nSelBlockEnd == -1))
			uEnabled |= MF_DISABLED | MF_GRAYED;
		CString temp;
		temp.LoadString(IDS_VIEWCONTEXTMENU_USETHEIRBLOCK);
		popup.AppendMenu(MF_STRING | uEnabled, ID_USETHEIRBLOCK, temp);
		temp.LoadString(IDS_VIEWCONTEXTMENU_USEYOURBLOCK);
		popup.AppendMenu(MF_STRING | uEnabled, ID_USEYOURBLOCK, temp);

		int cmd = popup.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN | TPM_NONOTIFY, point.x, point.y, this, 0);
		switch (cmd)
		{
		case ID_USETHEIRBLOCK:
			{
				for (int i=m_nSelBlockStart; i<=m_nSelBlockEnd; i++)
				{
					m_arDiffLines->SetAt(i, m_pwndLeft->m_arDiffLines->GetAt(i));
					m_arLineStates->SetAt(i, m_pwndLeft->m_arLineStates->GetAt(i));
				} // for (int i=m_nSelBlockStart; i<=m_nSelBlockEnd; i++) 
			} 
			break;
		case ID_USEYOURBLOCK:
			{
				for (int i=m_nSelBlockStart; i<=m_nSelBlockEnd; i++)
				{
					m_arDiffLines->SetAt(i, m_pwndRight->m_arDiffLines->GetAt(i));
					m_arLineStates->SetAt(i, m_pwndRight->m_arLineStates->GetAt(i));
				} // for (int i=m_nSelBlockStart; i<=m_nSelBlockEnd; i++) 
			}
			break;
		} // switch (cmd) 
	} // if (popup.CreatePopupMenu()) 
}
