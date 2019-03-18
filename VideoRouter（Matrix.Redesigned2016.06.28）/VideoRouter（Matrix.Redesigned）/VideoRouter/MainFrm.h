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

// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "Resource.h"
#include "MyRibbonControls.h"
#include "WaittingMsg.h"

//The Category Index of the Ribbon Bar
#define RIBBON_CATEGORY_MAIN    0   //��������
#define RIBBON_CATEGORY_MANAGER 1   //����

//The Panel Index of Category Main
#define RIBBON_PANEL_MODE       0
#define RIBBON_PANEL_CONNECT    1   //ͨ��
#define RIBBON_PANEL_SCENE      2   //����
#define RIBBON_PANEL_INPUT      3   //����
#define RIBBON_PANEL_OUTPUT     4   //���
#define RIBBON_PANEL_STATUS     5   //״̬

//The Panel Index of Category Manager
#define RIBBON_PANEL_BACKUP     0   //�����뻹ԭ
#define RIBBON_PANEL_POWER      1   //��Դ
#define RIBBON_PANEL_USER       2   //Ȩ��
#define RIBBON_PANEL_LANG       3   //����



class CMainFrame : public CFrameWndEx
{

protected: // �������л�����
	DECLARE_DYNCREATE(CMainFrame)
	CMainFrame();

// ����
protected:
	//CSplitterWnd m_wndSplitter;
	int m_nTimeoutCount;

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // �ؼ���Ƕ���Ա
	CMyRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	//COutlookBar       m_wndNavigationBar;
	//CMFCShellTreeCtrl m_wndTree;
	//CCalendarBar      m_wndCalendar;
	CMFCCaptionBar    m_wndCaptionBar;

	//�������ó����йصĲ˵���
	void ArrangeSceneMenus();

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	void SetRibbonBar();
	void SetRibbonAppButton();
	//BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	BOOL CreateCaptionBar();
	BOOL CreateStatusBar();

public:
	CWaittingMsg *m_pWaittingDlg;
	CWaittingMsg m_WaitDlg;
	void ShowWaittingDlg(BOOL bShow, CStringW strMsg);
	void WaitOver(CStringW strMsg);

public:
	afx_msg void OnCmdConnect();
	afx_msg void OnCmdLoadScene();
	afx_msg void OnCmdSaveScene();
	afx_msg void OnCmdRefreshScene();
	afx_msg void OnCmdLoopScene();
	afx_msg void OnCmdManaScene();
	afx_msg void OnCmdInputSetup();
	afx_msg void OnCmdOutputSetup();
	afx_msg void OnCmdOutputTest();
	afx_msg void OnCmdSysBackup();
	afx_msg void OnCmdSysRestore();
	afx_msg void OnCmdPowerControl();
	afx_msg void OnCmdUserManage();
	afx_msg void OnCmdLanguage();
	afx_msg void OnCmdRangeSceneID(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnCmdMatrixMode();
	afx_msg void OnUpdateCmdRangeScene(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSliderZoom();
	afx_msg LRESULT OnTransactionComplete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCmdUserChange();
	afx_msg void OnCmdReset();
	afx_msg void OnUpdateCmdBackup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdRestore(CCmdUI* pCmdUI);
};


