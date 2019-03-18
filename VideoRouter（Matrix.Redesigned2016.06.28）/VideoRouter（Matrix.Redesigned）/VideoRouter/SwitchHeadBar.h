#pragma once

#include "BnItem.h"

#define WizardLeft   0x0001 // 左导航
#define WizardRight  0x0002 // 右导航
#define WizardTop    0x0004 // 上导航
#define Wizardbottom 0x0008 // 下导航
//////////////////////////////////////////////////////////////////////////
// CSwitchHeadBar 命令目标
// 矩阵切换器的顶部的输出控制栏

class CSwitchHeadBar : public CWnd
{
	friend class CSwitchPage;
	friend class CSwitchSelectBar;

	DECLARE_DYNAMIC(CSwitchHeadBar)

public:
	CSwitchHeadBar();
	virtual ~CSwitchHeadBar();

	static int m_sFirstVisiableItem;

	void SetBarRect(const CRect rc);
	CRect GetBarRect() const { return m_rcBar; }

	void RecalcLayout();
	void ReDrawColumnSec();

	void EnableWizardBn(UINT nWizardCode, BOOL bEnable=TRUE);

	// 鼠标处理
public:
	BOOL m_bLBnDown;
	BOOL m_bResizing;

	int m_FocusItem;
	void OnHitTest(CPoint point);
	void SetFocusItem(int nItem);

	void ResetFlags();


private:
	CRect m_rcBar;  // 此窗口在父窗口中的位置

	CBnItem m_bnFresh;
	CBnItem m_bnFarTop;
	CBnItem m_bnPageUp;
	CBnItem m_bnFarLeft;
	CBnItem m_bnPageLeft;
	CBnItem m_bnLeftReBar;
	CBnItem m_bnRightReBar;
	CBnItem m_bnPageRight;
	CBnItem m_bnFarRight;

	void SetBnItems();

	CToolTipCtrl m_tips;
	void SetTooltips();

protected:
	//void DrawFirstColumn(Graphics* pGraphics, CRect rcColumn);
	void DrawSecondColumn(Graphics* pGraphics, CRect rcColumn);
	//void DrawThirdColumn(Graphics* pGraphics, CRect rcColumn);

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL BarCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext  = NULL );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnRefreshAll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFarTop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPageUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFarLeft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPageLeft(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPageRight(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFarRight(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


