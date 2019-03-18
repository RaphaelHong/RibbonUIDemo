#pragma once
#include "DrawWndPage.h"

class CVideoRouterDoc;


// CGridView 视图

class CGridView : public CScrollView
{
	friend class CDrawWndPage;
	friend class CSpliceSettingDlg;
	friend class CWndProperty;

	DECLARE_DYNCREATE(CGridView)

public:
	CGridView();           // 动态创建所使用的受保护的构造函数
	virtual ~CGridView();

	CVideoRouterDoc *GetDocument() const;

	//视图控制
protected:
	SPLICELAYOUT m_Layout; //大屏背景信息
	BOOL LoadState();

	CPoint m_ptViewOrg;     //视图坐标原点
	CPoint m_ptLastViewOrg; //视图尺寸变化前的原点
	CSize m_sizeTotal;      //滚动总量
	CSize m_sizePage;       //滚动页量
	CSize m_sizeLine;       //滚动行量

	// 将逻辑背景缩放到视图客户区以内
	BOOL m_bFitWidth;  //背景逻辑宽度在视图宽度以内
	BOOL m_bFitHeight; //背景逻辑高度在视图高度以内
	double m_dScale;   //背景物理尺寸缩放比

	void OnMouseScroll(int zDelta);
	void OnMouseZoom(int zDelta);

	//窗口管理
public:
	CDrawWndPage m_Page;  //一个窗体设计管理器
	CPageMouseManager m_MouseManager; //鼠标消息处理器
	void RedrawGraphic();
	void UpdateAligningLine(const int iHorz, const int iVert);

	void SetZoomLevel(double dZoomLvl);
	// 将逻辑背景缩放到视图客户区以内
	void SetProperZoom();

	//绘制处理
protected:
	CRect m_rcPage;   //大屏背景在视图上的位置
	//CBitmap m_MemBmp; //存储背景位图，一直加载在内存中，防止刷新过慢
	//CDC *m_pMemDC;    //一个内存DC，一致加载在内存中，防止刷新过慢
	

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuSpliceSet();
	afx_msg void OnMenuNew();
	afx_msg void OnMenuCloseAll();
	afx_msg void OnMenuClose();
	afx_msg void OnMenuLock();
	afx_msg void OnMenuMoveFront();
	afx_msg void OnMenuMovetoTop();
	afx_msg void OnMenuMoveBack();
	afx_msg void OnMenuMoveToBtm();
	afx_msg void OnMenuProperty();
	afx_msg void OnMenuShowGridLines();
	afx_msg void OnUpdateUICmdClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUICmdLock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUICmdShowGridLines(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSourceChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOutPortChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecieveLayoutInf(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecieveScreenInf(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecieveSceneInf(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUnpackSceneFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPackageSceneFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropSource(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropOutput(WPARAM wParam, LPARAM lParam);	
};


#ifndef _DEBUG  // GridView.cpp 中的调试版本
inline CVideoRouterDoc* CGridView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif