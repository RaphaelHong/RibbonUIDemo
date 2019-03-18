#pragma once


// CMMListCtrl

class CMMListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMMListCtrl)

public:
	CMMListCtrl();
	virtual ~CMMListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


