#pragma once
#include "Resource.h"

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return FALSE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); }
};

class CMyRibbonQuickAccessToolBar : public CMFCRibbonQuickAccessToolBar  
{  
	friend class CMyRibbonBar;  
};  

//////////////////////////////////////////////////////////////////////////  
// CKofMFCRibbonBar  

class CMyRibbonBar : public CMFCRibbonBar  
{  
	DECLARE_DYNAMIC(CMyRibbonBar)  

public:  
	CMyRibbonBar(BOOL bReplaceFrameCaption = TRUE);  
	virtual ~CMyRibbonBar();  

protected:  
	//virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);  
	virtual BOOL OnShowRibbonQATMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	virtual BOOL OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit);
	DECLARE_MESSAGE_MAP()  
}; 