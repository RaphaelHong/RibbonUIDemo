#pragma once
#include "DrawWndPage.h"

class CVideoRouterDoc;


// CGridView ��ͼ

class CGridView : public CScrollView
{
	friend class CDrawWndPage;
	friend class CSpliceSettingDlg;
	friend class CWndProperty;

	DECLARE_DYNCREATE(CGridView)

public:
	CGridView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CGridView();

	CVideoRouterDoc *GetDocument() const;

	//��ͼ����
protected:
	SPLICELAYOUT m_Layout; //����������Ϣ
	BOOL LoadState();

	CPoint m_ptViewOrg;     //��ͼ����ԭ��
	CPoint m_ptLastViewOrg; //��ͼ�ߴ�仯ǰ��ԭ��
	CSize m_sizeTotal;      //��������
	CSize m_sizePage;       //����ҳ��
	CSize m_sizeLine;       //��������

	// ���߼��������ŵ���ͼ�ͻ�������
	BOOL m_bFitWidth;  //�����߼��������ͼ�������
	BOOL m_bFitHeight; //�����߼��߶�����ͼ�߶�����
	double m_dScale;   //��������ߴ����ű�

	void OnMouseScroll(int zDelta);
	void OnMouseZoom(int zDelta);

	//���ڹ���
public:
	CDrawWndPage m_Page;  //һ��������ƹ�����
	CPageMouseManager m_MouseManager; //�����Ϣ������
	void RedrawGraphic();
	void UpdateAligningLine(const int iHorz, const int iVert);

	void SetZoomLevel(double dZoomLvl);
	// ���߼��������ŵ���ͼ�ͻ�������
	void SetProperZoom();

	//���ƴ���
protected:
	CRect m_rcPage;   //������������ͼ�ϵ�λ��
	//CBitmap m_MemBmp; //�洢����λͼ��һֱ�������ڴ��У���ֹˢ�¹���
	//CDC *m_pMemDC;    //һ���ڴ�DC��һ�¼������ڴ��У���ֹˢ�¹���
	

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��
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


#ifndef _DEBUG  // GridView.cpp �еĵ��԰汾
inline CVideoRouterDoc* CGridView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif