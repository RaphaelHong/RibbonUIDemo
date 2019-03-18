// SwitchPanelView.h : CSwitchPanelView 类的接口
//
#pragma once
#include "SwitchPage.h"

#define ENABLE_SCROLLVIEW 0

class CVideoRouterDoc;

class CSwitchPanelView : public CView
{
protected: // 仅从序列化创建
	CSwitchPanelView();
	DECLARE_DYNCREATE(CSwitchPanelView)

public:
	CVideoRouterDoc *GetDocument() const;

// 属性
public:
	CPoint m_ptViewOrg;  //视图坐标原点
	CSize m_sizeTotal;   //滚动视图总大小
	CSize m_sizePage;    //滚动一页，视图移动尺寸
	CSize m_sizeLine;    //滚动一行，视图移动尺寸

	CSwitchPage m_page;

	// 设置缩放比例
	void SetZoomLevel(int nLvl);
	// 重置切换器
	void ResetPage();
	// 信号刷新收尾处理
	void RefreshComplete();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CSwitchPanelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	//afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
#if ENABLE_SCROLLVIEW
	virtual void OnInitialUpdate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
#endif	
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnUnpackSceneFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPackageSceneFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecieveSceneInf(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // VedioRouterView.cpp 中的调试版本
inline CVideoRouterDoc* CSwitchPanelView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif