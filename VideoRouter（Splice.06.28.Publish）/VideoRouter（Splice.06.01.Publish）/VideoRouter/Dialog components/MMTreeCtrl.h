#pragma once


// CMMTreeCtrl

class CMMTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMMTreeCtrl)

public:
	CMMTreeCtrl();
	virtual ~CMMTreeCtrl();

	CImageList* m_pImageList;

	int m_nType;

private:
	inline int GetSelectedItemIndex(HTREEITEM hti) const;
	void PopupMenuForInput();
	void PopupMenuForOutput();

	HTREEITEM m_hItemDragS;
	HTREEITEM m_hItemDragD;
	HTREEITEM m_RClkItem;

	DWORD m_dwDragStart;
	BOOL  m_bDragging;

	UINT m_TimerTicks; //处理滚动的定时器所经过的时间
	UINT m_nScrollTimerID; //处理滚动的定时器
	CPoint m_HoverPoint; //鼠标位置
	UINT m_nHoverTimerID; //鼠标敏感定时器

	HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	HTREEITEM CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


