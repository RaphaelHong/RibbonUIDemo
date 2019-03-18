// SwitchPanelView.h : CSwitchPanelView ��Ľӿ�
//
#pragma once
#include "SwitchPage.h"

#define ENABLE_SCROLLVIEW 0

class CVideoRouterDoc;

class CSwitchPanelView : public CView
{
protected: // �������л�����
	CSwitchPanelView();
	DECLARE_DYNCREATE(CSwitchPanelView)

public:
	CVideoRouterDoc *GetDocument() const;

// ����
public:
	CPoint m_ptViewOrg;  //��ͼ����ԭ��
	CSize m_sizeTotal;   //������ͼ�ܴ�С
	CSize m_sizePage;    //����һҳ����ͼ�ƶ��ߴ�
	CSize m_sizeLine;    //����һ�У���ͼ�ƶ��ߴ�

	CSwitchPage m_page;

	// �������ű���
	void SetZoomLevel(int nLvl);
	// �����л���
	void ResetPage();
	// �ź�ˢ����β����
	void RefreshComplete();

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CSwitchPanelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // VedioRouterView.cpp �еĵ��԰汾
inline CVideoRouterDoc* CSwitchPanelView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif