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
//#include "DragTreeCtrl.h"
#include "Dialog components/MMListCtrl.h"
#include "Dialog components/MMTreeCtrl.h"
#include "WaittingMsg.h"

//The Category Index of the Ribbon Bar
#define RIBBON_CATEGORY_MAIN    0   //��������
#define RIBBON_CATEGORY_SET     1   //����
#define RIBBON_CATEGORY_SYS     2   //ϵͳ����

//The Panel Index of Category Main
//#define RIBBON_PANEL_MODE       0
//#define RIBBON_PANEL_CONNECT    1   //ͨ��
//#define RIBBON_PANEL_SCENE      2   //����
//#define RIBBON_PANEL_INPUT      3   //����
//#define RIBBON_PANEL_OUTPUT     4   //���
//#define RIBBON_PANEL_STATUS     5   //״̬

//The Panel Index of Category Manager
//#define RIBBON_PANEL_BACKUP     0   //�����뻹ԭ
//#define RIBBON_PANEL_POWER      1   //��Դ
//#define RIBBON_PANEL_USER       2   //Ȩ��
//#define RIBBON_PANEL_LANG       3   //����



class CMainFrame : public CFrameWndEx
{

protected: // �������л�����
	DECLARE_DYNCREATE(CMainFrame)
	CMainFrame();

// ����
protected:
	//CSplitterWnd m_wndSplitter;
	CString m_strStylesPath;
	int m_nTimeCounter;

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // �ؼ���Ƕ���Ա
	CMFCRibbonBar      m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar   m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	//CMFCShellTreeCtrl m_wndTree;
	CMFCCaptionBar    m_wndCaptionBar;

	CMMTreeCtrl m_wndTreeSource;
	CMMTreeCtrl m_wndTreeScreen;
	CMMListCtrl m_wndListScene;
	CImageList m_imgTree;

	//�������ó����йصĲ˵���
	void ArrangeSceneMenus();

	// ���οؼ���ͼ��
	void InitImageList(); 
	// �����ź�Դ��
	void ResetSourceTreeItem(); 
	// ˢ���ź�Դ״̬��ʾ
	void ResetSourceState();
	// �����������
	void ResetScreenTreeItem(); 

	// ����������Ϣ
	void UpdateSliderZoomPos(double dZoomLvl);

public:
	void LoadScene(int nID); // ���س���

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnViewTaskBar();
	afx_msg void OnUpdateViewTaskBar(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	void InitializeRibbonBar();
	void ResetRibbonBar();
	void SetRibbonAppButton();
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMMTreeCtrl& treesrc, CMMTreeCtrl& treescn,CMMListCtrl& list, int nInitialWidth);
	BOOL CreateCaptionBar();
	BOOL CreateStatusBar();

	CXTPTaskPanelGroup* CreateToolboxGroup(UINT nID, LPCTSTR lpGroupName);
	void ResetToolboxItems();

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

public:
	CWaittingMsg m_WaitDlg;
	void ShowWaittingDlg(BOOL bShow, CStringW strMsg);
	void WaitOver(CStringW strMsg);

public:
	afx_msg void OnMenuConnect();
	afx_msg void OnMenuLoadScene();
	afx_msg void OnMenuSaveScene();
	afx_msg void OnMenuRefreshScene();
	afx_msg void OnMenuLoopScene();
	afx_msg void OnMenuManaScene();
	afx_msg void OnMenuInputSetup();
	afx_msg void OnMenuOutputSetup();
	afx_msg void OnMenuImageTest();
	afx_msg void OnMenuSysBackup();
	afx_msg void OnMenuSysRestore();
	afx_msg void OnMenuPowerControl();
	afx_msg void OnMenuUserManage();
	afx_msg void OnMenuLanguage();
	afx_msg void OnMenuRangeSceneID(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnUpdateCmdLoop(CCmdUI *pCmdUI);
	afx_msg void OnCaptionBarOption();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCmdBtnZoom();
	afx_msg void OnCmdSliderZoom();
	afx_msg void OnCmdUserChange();
	afx_msg void OnCmdReset();
	afx_msg LRESULT OnTransactionComplete(WPARAM wParam, LPARAM lParam);
};


