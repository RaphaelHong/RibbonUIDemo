#pragma once
#include "Resource.h"

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return FALSE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); }
	virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual BOOL OnShowControlBarMenu(CPoint point);
};

class CMyRibbonBar : public CMFCRibbonBar
{
	virtual BOOL OnShowRibbonQATMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	virtual BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
};