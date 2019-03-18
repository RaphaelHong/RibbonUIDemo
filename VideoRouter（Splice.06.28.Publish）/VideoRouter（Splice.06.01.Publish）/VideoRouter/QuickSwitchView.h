// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// QuickSwitchView.h : CQuickSwitchView ��Ľӿ�
//
#pragma once

#include "VirtualSwitcher.h"
#include "Dialog components/MachinePanelDlg.h"

class CVideoRouterDoc;

class CQuickSwitchView : public CScrollView
{
public: // �������л�����
	CQuickSwitchView();
	DECLARE_DYNCREATE(CQuickSwitchView)

public:
	CVideoRouterDoc *GetDocument() const;

	// ����
public:

	//������ͼ����
	CPoint m_ptViewOrg;  //��ͼ����ԭ��
	CSize m_sizeTotal;   //������ͼ�ܴ�С���ڴ��൱�������������ߴ�
	CSize m_sizePage;    //����һҳ����ͼ�ƶ��ߴ�
	CSize m_sizeLine;    //����һ�У���ͼ�ƶ��ߴ�

	void OnVKScroll(UINT nVKCode); //�������Home����End����Page��������ͼ

public:
	//�ڴ�DC
	CDC *m_pMemDC;    //�ڴ�DC
	CSize m_LastSize; //��ͼ���һ�ε����ߴ��Ĵ�С

	//��궯������
	BOOL m_bResizeFirstCol;           //�Ƿ������һ�п��
	BOOL m_bLBtnDown;                 //��ǰ�������Ƿ���
	BOOL m_bShowHint;                 //������ͼ�Ƿ������ʾ

	CPoint m_ptLBtnDown;              //����������λ��
	CPoint m_ptRBtnUp;                //����Ҽ�����λ��
	CPoint m_ptCursor;                //��굱ǰλ��

	CPoint m_ptCaptureMatrix;         //��굱ǰ��ͣ�����
	CPoint m_ptLastCapMatrix;         //�����һ����ͣ�����

	int m_nCurInputID;              //��¼���������������ں�
	int m_nCurSubInput;
	int m_nCurOutputID;             //��¼���������������ں�
	int m_nCurSubOutput;

	void InitVariables();                     //��ʼ�����г�Ա����
	void ResetVariables();                    //���ÿ�����
	void OnMouseHangOver(CPoint ptMouse);     //�����ͣ�¼�
	BOOL IsResizeFirstColumn(CPoint ptMouse); //������һ�п���¼����������������̬
	void ResizeFirstColumn(CPoint ptMouse);   //������һ�п�� 

	void GetSelectedPortID(__in int nRowIndex, __in int nColIndex, __out int &nInputID, __out int &nSubInput, __out int &nOutputID, __out int &nSubOutput);
	int GetGridRowEx(__in CPoint ptSignalID);

	// ����
public:
	CVirtualSwitcher *m_pSwitcher;      //�����������������

	int m_nOpacity; //ͼ�㲻͸����

	void InitMatrix();
	void InitMatrixEx(const int nInCount, const UINT* pInLogicPort, const int nOutCount, const UINT* pOutLogicPort);
	//void DrawVirtualMatrix(CDC *pValidDC);
	void DrawHint(CDC *pValidDC);
	void CreateMemDC(CDC *pDevDC, CRect rcBmp);        //�����ڴ�DC
	void DrawBk(CDC *pValidDC);           //����Ҫ�ػ��Ĳ���
	void DrawShadowFrame(CDC *pValidDC);  //����Ҫ�ػ��Ĳ���
	//����Ҫ�ػ�Ĳ��������
	void DrawSourceName(CDC *pValidDC);   //���ź�Դ������ͼ��
	void DrawMarkedGrid(CDC *pValidDC);   //�����л����
	void DrawResizeBar(CDC *pValidDC);    //�п�������

public:
	//���Ӻ��ʼ��
	BOOL InitDevice();

	//��ȡ���������в�۵��������
	int CmdGBMG(int iPort, BACKBOARDINFO &bbi);
	//��ȡÿ������ϰ忨����Ϣ
	int CmdRSMG(int iPort, int iSlotPort, UINT &iCardID, UINT *pLogicPorts, UINT &iLogicPortCount);
	//���泡��
	int CmdSAVE(int iPort, const UINT iSceneID, const UINT iOutportCount, UINT *pInputPorts);
	//����EDID
	int CmdWEDI(int iPort, UINT *pEdidData, int iLen);

public:
	CMachinePanelDlg *m_pMachinePanel;
	BOOL CreateMachinePanel();

	// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// ʵ��
public:
	virtual ~CQuickSwitchView();
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
	virtual void OnInitialUpdate();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuVGAProperty();
	afx_msg void OnMenuOSDDesign();
	afx_msg void OnMenuLoadEDID();
	afx_msg void OnMenuRename();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMenuShowDevPanel();
	afx_msg void OnUpdateCmdShowDevPanel(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // VedioRouterView.cpp �еĵ��԰汾
inline CVideoRouterDoc* CQuickSwitchView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif