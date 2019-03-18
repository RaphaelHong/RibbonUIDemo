#pragma once

enum bnState
{
	bn_Normal,
	bn_Focused,
	bn_Clicked,
	bn_Disable,
};

// CBnItem

class CBnItem : public CWnd
{
	DECLARE_DYNAMIC(CBnItem)

public:
	CBnItem();
	CBnItem(/*UINT nID, */Rect rc, UINT uNormal, UINT uFocus, UINT uClick, UINT uDis, RotateFlipType RFT=RotateNoneFlipNone );
	virtual ~CBnItem();

public:
	//void SetItemID(UINT nID);
	void SetItemRect(INT x, INT y, INT cx, INT cy);
	void SetItemRect(const Rect rect);
	void SetItemState(bnState state);
	void SetImages(UINT uNormal, UINT uFocus, UINT uClick, UINT uDis);
	void SetRotateFlipType(RotateFlipType RFT);

	void ShowBoxFrame(BOOL bShow=TRUE);
	void Enable(BOOL bEnable=TRUE);

private:
	//{
	//UINT m_uID;
	Rect m_rcBn;  // 此窗口在父窗口中的位置
	bnState m_bnState;
	
	UINT m_uImageNormal;
	UINT m_uImageFocus;
	UINT m_uImageClick;
	UINT m_uImageDisab;

	RotateFlipType m_RFT;
	//}

	BOOL m_bLBnDown;
	BOOL m_bShowFrm;

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL BnCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const Rect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext  = NULL );
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /* = NULL */)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


