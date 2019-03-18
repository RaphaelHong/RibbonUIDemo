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

// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "MainFrm.h"
#include "VideoRouterDoc.h"
#include "VideoRouterView.h"
#include "GridView.h"
#include "Dialog components/ConnectConfigDlg.h"
#include "Dialog components/InputConfigDlg.h"
#include "Dialog components/OutputConfigDlg.h"
#include "Dialog components/SceneLoopDlg.h"
#include "Dialog components/SceneManagerDlg.h"
#include "Dialog components/TestPatternDlg.h"
#include "Dialog components/BackupAndRestoreDlg.h"
#include "Dialog components/UserDlg.h"
#include "Dialog components/LoginDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_COMMAND(ID_VIEW_TASK_BAR, &CMainFrame::OnViewTaskBar)
	ON_COMMAND(ID_RIBBON_CMD_CONNECT, &CMainFrame::OnMenuConnect)
	ON_COMMAND(ID_RIBBON_CMD_SC_LOAD, &CMainFrame::OnMenuLoadScene)
	ON_COMMAND(ID_RIBBON_CMD_SC_SAVE, &CMainFrame::OnMenuSaveScene)
	ON_COMMAND(ID_RIBBON_CMD_REFRESH, &CMainFrame::OnMenuRefreshScene)
	ON_COMMAND(ID_RIBBON_CMD_SC_LOOP, &CMainFrame::OnMenuLoopScene)
	ON_COMMAND(ID_RIBBON_CMD_SC_MANAGE, &CMainFrame::OnMenuManaScene)
	ON_COMMAND(ID_RIBBON_CMD_INPUT_SETUP, &CMainFrame::OnMenuInputSetup)
	ON_COMMAND(ID_RIBBON_CMD_OUTPUT_SETUP, &CMainFrame::OnMenuOutputSetup)
	ON_COMMAND(ID_RIBBON_CMD_IMAGE_TEST, &CMainFrame::OnMenuImageTest)
	ON_COMMAND(ID_RIBBON_CMD_BACKUP, &CMainFrame::OnMenuSysBackup)
	ON_COMMAND(ID_RIBBON_CMD_RESTORE, &CMainFrame::OnMenuSysRestore)
	ON_COMMAND(ID_RIBBON_CMD_POWER, &CMainFrame::OnMenuPowerControl)
	ON_COMMAND(ID_RIBBON_CMD_USER, &CMainFrame::OnMenuUserManage)
	ON_COMMAND(ID_RIBBON_CMD_LANG, &CMainFrame::OnMenuLanguage)
	ON_COMMAND_RANGE(ID_SCENE_ID_BEGIN, ID_SCENE_ID_END, &CMainFrame::OnMenuRangeSceneID)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TASK_BAR, &CMainFrame::OnUpdateViewTaskBar)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_CMD_SC_LOOP, &CMainFrame::OnUpdateCmdLoop)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnCaptionBarOption)
	ON_COMMAND(ID_RIBBON_CMD_BTN_ZOOM, &CMainFrame::OnCmdBtnZoom)
	ON_COMMAND(ID_RIBBON_CMD_SLIDER_ZOOM, &CMainFrame::OnCmdSliderZoom)
	ON_COMMAND(ID_RIBBON_CMD_USER_CHANGE, &CMainFrame::OnCmdUserChange)
	ON_COMMAND(ID_RIBBON_CMD_RESET, &CMainFrame::OnCmdReset)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_TRANS_COMPLETE, &CMainFrame::OnTransactionComplete)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);

	m_strStylesPath = _T("Styles\\");

#if 0
	//XTPSkinManager()->AddColorFilter(new CXTPSkinManagerColorFilterShiftRGB(0, 0, 0));
	XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
	XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"),_T("NormalBlue.ini"));
#endif
}

CMainFrame::~CMainFrame()
{
}

 //BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
 //								CCreateContext* pContext)
 //{
	//return m_wndSplitter.Create(this,
	//	2, 1,               // TODO: ��������������
	//	CSize(10, 10),      // TODO: ������С�����С
	//	pContext);

	//m_wndSplitter.CreateStatic(this, 1, 4);
	//m_wndSplitter.ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	//m_wndSplitter.CreateView(0, 1, pContext->m_pNewViewClass, CSize(300, 0), pContext);

	//CRect r;
	//GetClientRect(&r);
	////m_wndSplitter.SetRowInfo( 0, r.Height()/1, 200);
	//m_wndSplitter.SetColumnInfo( 0, 210, 100 );

	//if (!CreateTaskPanel())
	//	return FALSE;

	//m_wndSplitter.SetSplitterStyle(XTP_SPLIT_NOSIZE);
	//m_wndSplitter.SetTheme(xtpControlThemeResource);

	//SetActiveView((CView*)m_wndSplitter.GetDlgItem(m_wndSplitter.IdFromRowCol(0, 1)));

 //	return TRUE;
 //}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	//���Ribbonʽ�˵�
	m_wndRibbonBar.Create(this);
	InitializeRibbonBar();
	ArrangeSceneMenus();

	// ���ͼ�굽�б�
	InitImageList();

	// ���״̬��
	if (!CreateStatusBar())
	{
		TRACE0("δ�ܴ���״̬��\n");
	}

	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �������񽫴�������࣬��˽���ʱ��������ͣ��:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// ���������á�Outlook��������:
 	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTreeSource, m_wndTreeScreen, m_wndListScene, 250))
 	{
 		TRACE0("δ�ܴ�����������\n");
 		return -1;      // δ�ܴ���
 	}

	// ����������:
	if (!CreateCaptionBar())
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	// �Ѵ��� Outlook ����Ӧ���������ͣ����
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// ���ó������
	CString strAppTitle;
	strAppTitle.LoadString(AFX_IDS_APP_TITLE);
	SetWindowText(strAppTitle);


	return 0;
}

BOOL CMainFrame::CreateStatusBar()
{
	if (!m_wndStatusBar.Create(this))
	{
		return FALSE;      // δ�ܴ���
	}

	CString strTitlePane1;
	CString strTitlePane2;
	BOOL bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	CMFCRibbonLabel *pLabel = new CMFCRibbonLabel(_T("״̬1"));
	m_wndStatusBar.AddElement(pLabel, _T("״̬1"));
	m_wndStatusBar.AddSeparator();

	m_wndStatusBar.AddElement(new CMFCRibbonCheckBox(ID_RIBBON_CMD_SHOW_GRID, _T("������")), _T("������"));

	CMFCRibbonButton* pBtnZoom = new CMFCRibbonButton(ID_RIBBON_CMD_BTN_ZOOM, _T("100%"));
	m_wndStatusBar.AddExtendedElement(pBtnZoom, _T("100%"));

	CMFCRibbonSlider *pSlider = new CMFCRibbonSlider(ID_RIBBON_CMD_SLIDER_ZOOM, 150);
	pSlider->SetZoomButtons(TRUE);
	pSlider->SetRange(10, 100);
	pSlider->SetZoomIncrement(1);
	pSlider->SetPos(10);
	m_wndStatusBar.AddExtendedElement(pSlider, _T("Slider"));

	m_wndStatusBar.AddExtendedElement(new CMFCRibbonLabel(_T("״̬2")), _T("״̬2"));

	return TRUE;
}

void CMainFrame::InitializeRibbonBar()
{
	m_wndRibbonBar.EnablePrintPreview(FALSE);
	//m_wndRibbonBar.EnableToolTips(FALSE, FALSE);
	//m_wndRibbonBar.EnableTrackingToolTips(FALSE);

	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_RIBBON_BUTTONS);
	m_PanelImages.SetAlwaysLight(TRUE);

	// Ӧ�ó���ť
	SetRibbonAppButton();

	// ������ҳ��
	CMFCRibbonCategory *pCategoryMain = m_wndRibbonBar.AddCategory(_T("������"), IDB_CATEGORY_MAIN_SMALL, IDB_CATEGORY_MAIN_LARGE);
	{
		// ͨ�����
		CMFCRibbonPanel *pCommunicatePanel = pCategoryMain->AddPanel(_T("ͨѶ������"), m_PanelImages.ExtractIcon(1));
		{
			pCommunicatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_CONNECT, _T("����"), 14, 14));
		}

		// ���ڹ���
		CMFCRibbonPanel *pWndOrganizePanel = pCategoryMain->AddPanel(_T("���ڹ���"), m_PanelImages.ExtractIcon(3));
		pWndOrganizePanel->SetCenterColumnVert();
		{
			pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_NEW, _T("�½�"), 27, 27));
			pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_CLOSE, _T("�ر�"), 28, -1));
			pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_CLOSE_ALL, _T("���"), 29, -1));
			pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_LOCKALL, _T("����"), 30, 30));
		}

		// ���ڲ���
		CMFCRibbonPanel *pWndOperatePanel = pCategoryMain->AddPanel(_T("���ڲ���"), m_PanelImages.ExtractIcon(4));
		{
			pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_TOP, _T("�ö�"), 23, 23));
			pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_BOTTOM, _T("�õ�"), 26, 26));
			pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_FRONT, _T("����"), 24, 24));
			pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_BACK, _T("����"), 25, 25));			
			pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_PROPERTY, _T("����"), 31, 31));
		}

		// �������
		CMFCRibbonPanel *pScenePanel = pCategoryMain->AddPanel(_T("����������"), m_PanelImages.ExtractIcon(2));
		{
			pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_LOAD, _T("��"), 10, 10));
			pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_SAVE, _T("����"), 9, 9));
			pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_REFRESH, _T("ˢ��"), 12, 12));
			pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_LOOP, _T("��Ѳ"), 13, 13));
			pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_MANAGE, _T("�༭"), 11, 11));
			pScenePanel->Add(new CMFCRibbonSeparator());
		}				

		// ��ͼ���
		CMFCRibbonPanel* pPanelView = pCategoryMain->AddPanel(_T("��ͼ"), m_PanelImages.ExtractIcon (5));
		{
			CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, _T("״̬��"));
			pPanelView->Add(pBtnStatusBar);
			CMFCRibbonButton* pBtnCaptionBar = new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, _T("������"));
			pPanelView->Add(pBtnCaptionBar);
			CMFCRibbonButton* pBtnTaskBar = new CMFCRibbonCheckBox(ID_VIEW_TASK_BAR, _T("�б���"));
			pPanelView->Add(pBtnTaskBar);
		}
		
		//ϵͳ�˳�
		CMFCRibbonPanel *pQuitPanel = pCategoryMain->AddPanel(_T("�˳�"), m_PanelImages.ExtractIcon(6));
		pQuitPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_USER_CHANGE, _T("�л��û�"), 32, 32));
		pQuitPanel->Add(new CMFCRibbonButton(ID_APP_EXIT, _T("�˳�"), 17, 17));
	}


	// ����ҳ��
	CMFCRibbonCategory *pCategorySetting = m_wndRibbonBar.AddCategory(_T("����"), IDB_CATEGORY_MAIN_SMALL, IDB_CATEGORY_MAIN_LARGE);
	{
		// ģʽ�л����
		//CMFCRibbonPanel *pModePanel = pCategorySetting->AddPanel(_T("ģʽѡ����"), m_PanelImages.ExtractIcon(7));
		//{
		//	CMFCRibbonButton *pBtnByPass = new CMFCRibbonButton(ID_RIBBON_CMD_QUICK_MODE, _T("����ģʽ"), 23, 23);
		//	pBtnByPass->AddSubItem(new CMFCRibbonCheckBox(ID_RIBBON_CMD_DEV_PANEL, _T("��ʾ/����ģ�����")));

		//	pModePanel->Add(pBtnByPass);
		//	pModePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SPLIT_MODE, _T("ƴ��ģʽ"), 24, 24));
		//}

		// ƴ���������
		CMFCRibbonPanel *pInputPanel = pCategorySetting->AddPanel(_T("�ź�����"), m_PanelImages.ExtractIcon(5));
		{
			pInputPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_INPUT_SETUP, _T("��������"), 18, 18));
		}

		CMFCRibbonPanel *pOutputPanel = pCategorySetting->AddPanel(_T("�������"), m_PanelImages.ExtractIcon(6));
		{
			pOutputPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SPLIT_SET, _T("ƴ������"), 21, 21));
			pOutputPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_OUTPUT_SETUP, _T("�������"), 15, 15));
		}

		CMFCRibbonPanel *pTestPanel = pCategorySetting->AddPanel(_T("ͼ�����"), m_PanelImages.ExtractIcon(6));
		{
			pTestPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_IMAGE_TEST, _T("ͼ�����"), 19, 19));
		}

		//�����л����
		CMFCRibbonPanel *pLangPanel = pCategorySetting->AddPanel(_T("Language"), m_PanelImages.ExtractIcon(4));
		{
			CMFCRibbonComboBox *pCBox = new CMFCRibbonComboBox(ID_RIBBON_CMD_LANG, FALSE, 100, _T("����"));
			pCBox->AddItem(_T("��������(�й�)"));
			pCBox->AddItem(_T("English(U.S)"));
			pCBox->AddItem(_T("��������(�й�̨��)"));
			pCBox->SelectItem(0);
			pLangPanel->Add(pCBox);
			pLangPanel->SetCenterColumnVert(TRUE);
		}
	}

	// ����ҳ��
	CMFCRibbonCategory *pCategoryMana = m_wndRibbonBar.AddCategory(_T("������"), IDB_CATEGORY_MANAGE_SMALL, IDB_CATEGORY_MANAGE_LARGE);
	{
		//ϵͳ���ݻ�ԭ���
		CMFCRibbonPanel *pBackupPanel = pCategoryMana->AddPanel(_T("�����뻹ԭ"), m_PanelImages.ExtractIcon(1));
		pBackupPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_BACKUP, _T("ϵͳ����"), 0, 0));
		pBackupPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_RESTORE, _T("ϵͳ��ԭ"), 1, 1));

		CMFCRibbonPanel *pResetPanel = pCategoryMana->AddPanel(_T("ϵͳ��λ"), m_PanelImages.ExtractIcon(2));
		pResetPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_RESET, _T("��λ"), 4, 4));

		//��Դ�������
		//CMFCRibbonPanel *pPowerPanel = pCategoryMana->AddPanel(_T("��Դ����"), m_PanelImages.ExtractIcon(2));
		//pPowerPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_POWER, _T("��Դ"), 2, 2));

		//�û����
		CMFCRibbonPanel *pUserPanel = pCategoryMana->AddPanel(_T("Ȩ�޹���"), m_PanelImages.ExtractIcon(3));
		pUserPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_USER, _T("�û�"), 3, 3));
	}

	// ���ٷ�����
	{
		CList <UINT,UINT> lstCmd;
		lstCmd.AddTail(ID_APP_EXIT);

		m_wndRibbonBar.SetQuickAccessCommands(lstCmd);
	}

	// ��ʽ��ť
	CMFCRibbonButton *pStyleBtn = new CMFCRibbonButton(-1, _T("���"), -1, -1);
	pStyleBtn->SetMenu(IDR_THEME_MENU);
 	//pStyleBtn->SetToolTipText(_T("ѡ�񴰿ڷ��\n"));
 	//pStyleBtn->SetDescription(_T("Office 2007 styles"));
	m_wndRibbonBar.AddToTabs(pStyleBtn);

	// ���ڰ�ť
	CMFCRibbonButton *pAboutBtn = new CMFCRibbonButton(ID_APP_ABOUT, NULL, m_PanelImages.ExtractIcon(0));
	m_wndRibbonBar.AddToTabs(pAboutBtn);
}

void CMainFrame::SetRibbonAppButton()
{
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(_T("ϵͳ�˵�"));

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize(46, 46));
	CMFCRibbonMainPanel *pAppPanel = m_wndRibbonBar.AddMainCategory(_T("ϵͳ"), IDB_CATEGORY_MAIN_SMALL, IDB_CATEGORY_MAIN_LARGE);

	pAppPanel->RemoveAll();
	//��Ӵ򿪰�ť
	CMFCRibbonButton *pBtnLoad = new CMFCRibbonButton(ID_RIBBON_CMD_SC_LOAD, _T("��"), 10, 10);
	pBtnLoad->AddSubItem(new CMFCRibbonLabel(_T("Exsiting Scene Files")));

	pAppPanel->Add(pBtnLoad);

	//��ӱ��水ť
	pAppPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_SAVE, _T("����"), 9, 9));

	//��ӹرհ�ť
	pAppPanel->Add(new CMFCRibbonSeparator(TRUE));
	pAppPanel->Add(new CMFCRibbonButton(ID_APP_EXIT, _T("�˳�"), 17, 17));

	//�������򿪵��ĵ�
	pAppPanel->AddRecentFilesList(_T("����򿪵ĳ���"));
	//����˳�����ť
	pAppPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, _T("�˳�"), 17));
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMMTreeCtrl& treesrc, CMMTreeCtrl& treescn, CMMListCtrl& list, int nInitialWidth)
{
	CWindowDC dc(NULL);

	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // fail to create
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	//DWORD dwPaneStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_FLOAT_MULTI;

	// can float, can autohide, can resize, CAN NOT CLOSE
	DWORD dwStyle = /*AFX_CBRS_FLOAT |*/ AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	// �����ź�Դ�б�
	treesrc.Create(dwTreeStyle, rectDummy, &bar, IDC_OUTLOOKBAR_TREE1);
	treesrc.m_nType = 0;
	ResetSourceTreeItem();
	pOutlookBar->AddControl(&treesrc, _T("�ź�Դ�б�"), 0, FALSE, dwStyle);

	// ����������б�
	treescn.Create(dwTreeStyle, rectDummy, &bar, IDC_OUTLOOKBAR_TREE2);
	treescn.m_nType = 1;
	ResetScreenTreeItem();
	pOutlookBar->AddControl(&treescn, _T("����˿��б�"), 1, FALSE, dwStyle);

	// ���������б�
	//list.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_SINGLESEL, rectDummy, &bar, IDC_OUTLOOKBAR_LIST1);
	//list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	//list.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
	//list.InsertColumn(1, _T("������"), LVCFMT_LEFT, 100);

	//CString strItem;
	//for (int i=0; i<10; i++)
	//{
	//	strItem.Format(_T("%d"), i+1);
	//	list.InsertItem(LVIF_TEXT, i, strItem, 0, LVIS_SELECTED, 0, 0);
	//	strItem.Format(_T("����%d"), i+1);
	//	list.SetItemText(i, 1, strItem);
	//}
	//pOutlookBar->AddControl(&list, _T("�����б�"), 2, FALSE, dwStyle);
	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS/* | CBRS_FLYBY*/ | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	//BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	//CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

BOOL CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("δ�ܴ���������\n");
		return FALSE;
	}

	BOOL bNameValid;

	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_RIGHT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnApplicationLook(UINT id)
{
	//return;

	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			//XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"),_T("NormalBlue.ini"));
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			//XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"),_T("NormalBlack.ini"));
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			//XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"),_T("NormalSilver.ini"));
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			//XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"),_T("NormalAqua.ini"));
			break;

		default:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			//XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2007.cjstyles"),_T("NormalBlue.ini"));
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnViewTaskBar()
{
	BOOL bShow = !m_wndNavigationBar.IsVisible();
	m_wndNavigationBar.ShowPane(bShow, bShow, bShow);

	RecalcLayout();
}

void CMainFrame::OnUpdateViewTaskBar(CCmdUI* pCmdUI)
{
	BOOL bShow = m_wndNavigationBar.IsVisible();
	pCmdUI->SetCheck(bShow);
}

void CMainFrame::OnMenuConnect()
{
	//�������öԻ���
	CConnectConfigDlg DLG;
	if (DLG.DoModal() != IDOK)
		return;

	//���ӳɹ���ʼ�����豸����
	GetInitManager()->InitDeviceContents();
}

void CMainFrame::OnMenuLoadScene()
{
	//�������ضԻ���
	CSceneManagerDlg DLG(SCENE_MANAGER_LOAD);
	DLG.DoModal();
}

void CMainFrame::OnMenuSaveScene()
{
	//��������Ի���
	CSceneManagerDlg DLG(SCENE_MANAGER_SAVE);
	DLG.DoModal();
}

void CMainFrame::OnMenuManaScene()
{
	//�����༭�Ի���
	CSceneManagerDlg DLG(SCENE_MANAGER_EDIT);
	DLG.DoModal();
}

void CMainFrame::OnMenuRefreshScene()
{
	LoadScene(0);
}

void CMainFrame::OnMenuLoopScene()
{
	//����ѭ���Ի���
	CSceneLoopDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnMenuInputSetup()
{
	//�������öԻ���
	CInputConfigDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnMenuOutputSetup()
{
	//������öԻ���
	COutputConfigDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnMenuImageTest()
{
	//ͼ����ԶԻ���
	CTestPatternDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnMenuSysBackup()
{
	CBackupAndRestoreDlg DLG;
	DLG.DoModal();
	//m_wndCaptionBar.SetText(_T("����ϵͳ���ݹ��ܣ�"), CMFCCaptionBar::ALIGN_LEFT);
}

void CMainFrame::OnMenuSysRestore()
{
	CBackupAndRestoreDlg DLG;
	DLG.DoModal();
	//m_wndCaptionBar.SetText(_T("����ϵͳ��ԭ���ܣ�"), CMFCCaptionBar::ALIGN_LEFT);
}

void CMainFrame::OnMenuPowerControl()
{
	//m_wndCaptionBar.SetText(_T("���ǵ�Դ���ƹ��ܣ�"), CMFCCaptionBar::ALIGN_LEFT);
}

void CMainFrame::OnCaptionBarOption()
{
	AfxMessageBox(_T("CaptionBar Option!"));
}

void CMainFrame::OnMenuUserManage()
{
	CUserDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnMenuLanguage()
{
	return;

	CMFCRibbonComboBox *pCBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RIBBON_CMD_LANG);
	int nLangSel = pCBox->GetCurSel();
}

void CMainFrame::OnMenuRangeSceneID(UINT nID)
{
	int nSceneID = nID-ID_SCENE_ID_BEGIN+1;
	LoadScene(nSceneID);
}

void CMainFrame::OnClose()
{
#ifndef _DEBUG
	if (MessageBox(_T("�˳��˳���"), _T("����"), MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
#endif

	CFrameWndEx::OnClose();
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnUpdateCmdLoop(CCmdUI *pCmdUI)
{
}

void CMainFrame::ResetRibbonBar()
{
	CMFCRibbonCategory *pCategory = m_wndRibbonBar.GetCategory(0);
	CMFCRibbonPanel *pPanel = NULL;
	CMFCRibbonBaseElement *pElem = NULL;

	if (GetGlobalManager()->GetCurrencyWorkMode() == eWorkModeMatrix)
	{
		pElem = m_wndRibbonBar.FindByID(ID_RIBBON_CMD_WND_NEW);
		ASSERT(pElem);
		pPanel = pCategory->FindPanelWithElem(pElem);
		ASSERT(pPanel);
		pCategory->RemovePanel(pPanel);

		pElem = m_wndRibbonBar.FindByID(ID_RIBBON_CMD_WND_TOP);
		ASSERT(pElem);
		pPanel = pCategory->FindPanelWithElem(pElem);
		ASSERT(pPanel);
		pCategory->RemovePanel(pPanel);
	}
	else if (GetGlobalManager()->GetCurrencyWorkMode() == eWorkModeSplit)
	{
		pElem = m_wndRibbonBar.FindByID(ID_RIBBON_CMD_WND_NEW);
		if (pElem == NULL)
		{
			// ���ڹ���
			CMFCRibbonPanel *pWndOrganizePanel = pCategory->AddPanel(1, _T("���ڹ���"), m_PanelImages.ExtractIcon(3));
			pWndOrganizePanel->SetCenterColumnVert();
			{
				pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_NEW, _T("�½�"), 29, 29));
				pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_CLOSE, _T("�ر�"), 30, -1));
				pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_CLOSE_ALL, _T("���"), 31, -1));
				pWndOrganizePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_LOCKALL, _T("����"), 32, 32));
			}
		}

		pElem = m_wndRibbonBar.FindByID(ID_RIBBON_CMD_WND_TOP);
		if (pElem == NULL)
		{
			// ���ڲ���
			CMFCRibbonPanel *pWndOperatePanel = pCategory->AddPanel(2, _T("���ڲ���"), m_PanelImages.ExtractIcon(4));
			{
				pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_TOP, _T("�ö�"), 25, 25));
				pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_BOTTOM, _T("�õ�"), 28, 28));
				pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_FRONT, _T("����"), 26, 26));
				pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_BACK, _T("����"), 27, 27));
				pWndOperatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_WND_PROPERTY, _T("����"), 33, 33));
			}
		}
	}

	m_wndRibbonBar.ForceRecalcLayout();
}

void CMainFrame::ArrangeSceneMenus()
{
	// AppButton�ϵļ��ز˵�����
	CMFCRibbonCategory* pCategory = m_wndRibbonBar.GetMainCategory();
	if (pCategory == NULL) return;

	CMFCRibbonMainPanel* pAppPanel = (CMFCRibbonMainPanel*)pCategory->GetPanel(0);
	if (pAppPanel == NULL) return;

	CMFCRibbonButton* pAppLoadBtn = (CMFCRibbonButton*)pAppPanel->FindByID(ID_RIBBON_CMD_SC_LOAD);
	if (pAppLoadBtn == NULL)
	{
		pAppLoadBtn = new CMFCRibbonButton(ID_RIBBON_CMD_SC_LOAD, _T("����"), 10, 10);
		pAppPanel->Add(pAppLoadBtn);
	}

	pAppLoadBtn->RemoveAllSubItems();
	pAppLoadBtn->AddSubItem(new CMFCRibbonLabel(_T("�ѱ���ĳ���")));

	// Main Panel�ϵļ��ز˵�����
	CMFCRibbonCategory* pCategoryMain = m_wndRibbonBar.GetCategory(RIBBON_CATEGORY_MAIN);
	if (pCategoryMain == NULL) return;

	CMFCRibbonBaseElement* pElem = pCategoryMain->FindByID(ID_RIBBON_CMD_SC_LOAD);
	if (pElem == NULL) return;

	CMFCRibbonPanel* pScenePanel = pCategoryMain->FindPanelWithElem(pElem);
	if (pElem == NULL) return;

	//�Ƴ����п�ݳ����˵�
	int nCmdCount = pScenePanel->GetCount();
	int nCmdIndex = pScenePanel->GetIndex(pScenePanel->FindByID(ID_RIBBON_CMD_SC_MANAGE))+1;

	for (int i=nCmdCount-1; i>5; i--)
		pScenePanel->Remove(i);

	//�Ƴ����ز˵��ϵĳ�����
	CMFCRibbonButton* pLoadBtn = (CMFCRibbonButton*)pElem;
	pLoadBtn->RemoveAllSubItems();

	//���¸��³�����
	int nImageIndex, nScID;
	CString strScName;
	CSceneHeadInfo* pItem = NULL;
	for (int i=0; i<GetSceneManager()->GetCount(); i++)
	{
		pItem = GetSceneManager()->GetAt(i);
		nScID = pItem->GetID();
		strScName = pItem->GetName();
		nImageIndex = pItem->GetLargeImage();

		pAppLoadBtn->AddSubItem(new CMFCRibbonButton(ID_SCENE_ID_BEGIN + nScID -1, strScName, nImageIndex, -1));
		pLoadBtn->AddSubItem(new CMFCRibbonButton(ID_SCENE_ID_BEGIN + nScID -1, strScName, nImageIndex, -1));

		if (pItem->GetQuickMenuFlag() == TRUE)
			pScenePanel->Add(new CMFCRibbonButton(ID_SCENE_ID_BEGIN + nScID -1, strScName, nImageIndex, nImageIndex));
	}

	m_wndRibbonBar.ForceRecalcLayout();
}

void CMainFrame::ResetSourceTreeItem()
{
	if (m_wndTreeSource.GetSafeHwnd())
	{
		CRedrawLock lock(&m_wndTreeSource);
		m_wndTreeSource.SetImageList(&m_imgTree, TVSIL_NORMAL);
		m_wndTreeSource.DeleteAllItems();

		TV_INSERTSTRUCT tvis;
		tvis.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvis.item.stateMask = TVIS_STATEIMAGEMASK;
		tvis.item.state = INDEXTOSTATEIMAGEMASK(1);

		CString strItem;
		for (int i=0; i<GetInputManager()->GetCount(); i++)
		{
			strItem.Format(_T("%s(P%d)"), GetInputManager()->GetName(i, FALSE), GetInputManager()->GetCmdPort(i, FALSE));
			tvis.hParent = TVI_ROOT;
			tvis.hInsertAfter = TVI_LAST;
			tvis.item.pszText = strItem.GetBuffer(0);
			tvis.item.cchTextMax = strItem.GetLength();
			tvis.item.iImage = (GetInputManager()->GetSignalType(i, FALSE) == vtUndef) ? 1 : 0;
			tvis.item.iSelectedImage = tvis.item.iImage;

			HTREEITEM hti = m_wndTreeSource.InsertItem(&tvis);
			ASSERT(hti);
		}

		if (GetInputManager()->GetCount() > 0)
		{
			m_wndTreeSource.SelectItem(m_wndTreeSource.GetRootItem());
		}
	}
}

void CMainFrame::ResetSourceState()
{
	CRedrawLock lock(&m_wndTreeSource);

	int nIter = 0;
	int nImage = 0;
	HTREEITEM htiNext = m_wndTreeSource.GetRootItem();
	while (htiNext)
	{
		nImage = GetInputManager()->GetSignalType(nIter, FALSE) == vtUndef ? 1 : 0;
		m_wndTreeSource.SetItemImage(htiNext, nImage, nImage);

		htiNext = m_wndTreeSource.GetNextItem(htiNext, TVGN_NEXT);
		nIter++;
	}
}

void CMainFrame::ResetScreenTreeItem()
{
	if (m_wndTreeScreen.GetSafeHwnd())
	{
		CRedrawLock lock(&m_wndTreeScreen);
		m_wndTreeScreen.SetImageList(&m_imgTree, TVSIL_NORMAL);

		m_wndTreeScreen.DeleteAllItems();

		TV_INSERTSTRUCT tvis;
		tvis.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvis.item.stateMask = TVIS_STATEIMAGEMASK;
		tvis.item.state = INDEXTOSTATEIMAGEMASK(1);

		CString strItem;
		for (int i=0; i<GetOutputManager()->GetCount(); i++)
		{
			strItem.Format(_T("%s(P%d)"), GetOutputManager()->GetName(i, FALSE), GetOutputManager()->GetCmdPort(i, FALSE));
			tvis.hParent = TVI_ROOT;
			tvis.hInsertAfter = TVI_LAST;
			tvis.item.pszText = strItem.GetBuffer(0);
			tvis.item.cchTextMax = strItem.GetLength();
			tvis.item.iImage = 2;
			tvis.item.iSelectedImage = 2;

			HTREEITEM hti = m_wndTreeScreen.InsertItem(&tvis);
			ASSERT(hti);
		}
	}
}

void CMainFrame::InitImageList()
{
	HICON icon[3];
	icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_SIGNAL);
	icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_NOSIGNAL);
	icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_MONITOR);

	m_imgTree.Create(32, 32, ILC_COLOR24, 3, 3);
	m_imgTree.Add(icon[0]);
	m_imgTree.Add(icon[1]);
	m_imgTree.Add(icon[2]);
}

void CMainFrame::LoadScene(int nID)
{
	int nSceneID = nID - ID_SCENE_ID_BEGIN + 1;
	GetSceneManager()->LoadSceneFromDevice(nID);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_IDLE:
		{
			KillTimer(nIDEvent);

			if (CCommunicateManager::IsConnected())
			{
				m_nTimeCounter = 0;
				SetTimer(TIMER_KEEP_ALIVE, ALIVE_INTENAL, NULL);
			}
		}		

		break;

	case TIMER_KEEP_ALIVE:
		{
			if (! CCommunicateManager::IsConnected())
			{
				// ��������ѶϿ���رն�ʱ��

				KillTimer(nIDEvent);
			}
			else if (!GetCmdManager()->OnlineDetect())
			{
				// ������ʱ����ʼ����������3�γ�ʱ����Ϊ�����ѶϿ�
				m_nTimeCounter++;

				if (m_nTimeCounter >= MAX_TIMEOUT_NUM)
				{
					KillTimer(nIDEvent);

					GetCmdManager()->Close();

					AfxMessageBox(_T("�����ѶϿ�..."), MB_OK|MB_ICONWARNING);
				}
			}
			else {
				// ����������Ӧ���رյȴ���ʱ����������һ�����߼������
				KillTimer(TIMER_IDLE);
				m_nTimeCounter = 0;
			}
		}		

		break;

	default:break;
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::OnCmdBtnZoom()
{
	CGridView* pView = (CGridView*)GetActiveView();
	pView->SetProperZoom();
}

void CMainFrame::OnCmdSliderZoom()
{
	CMFCRibbonSlider* pSlider = (CMFCRibbonSlider*)m_wndStatusBar.GetExElement(1);
	if (pSlider == NULL)
		return;

	int nPos = pSlider->GetPos();
	CGridView* pView = (CGridView*)GetActiveView();

	double dZoomLvl = nPos*1.0/100;
	pView->SetZoomLevel(dZoomLvl);

	CMFCRibbonButton* pBtnZoom = (CMFCRibbonButton*)m_wndStatusBar.GetExElement(0);
	if (pBtnZoom == NULL)
		return;

	CString strLvl;
	strLvl.Format(_T("%4.1f"), dZoomLvl*100);
	strLvl += _T("%");
	pBtnZoom->SetText(strLvl);

	m_wndStatusBar.ForceRecalcLayout();
}

void CMainFrame::UpdateSliderZoomPos(double dZoomLvl)
{
	CMFCRibbonSlider* pSlider = (CMFCRibbonSlider*)m_wndStatusBar.GetExElement(1);
	if (pSlider == NULL)
		return;

	int nPos = int(dZoomLvl*100);
	if (nPos < 10) nPos = 10;
	if (nPos > 100) nPos = 100;

	pSlider->SetPos(nPos);

	CMFCRibbonButton* pBtnZoom = (CMFCRibbonButton*)m_wndStatusBar.GetExElement(0);
	if (pBtnZoom == NULL)
		return;
	CString strLvl;
	strLvl.Format(_T("%4.1f"), dZoomLvl*100);
	strLvl += _T("%");
	pBtnZoom->SetText(strLvl);

	m_wndStatusBar.ForceRecalcLayout();
}

void CMainFrame::OnCmdUserChange()
{
	CLoginDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdReset()
{

}


void CMainFrame::ShowWaittingDlg(BOOL bShow, CStringW strMsg)
{

	m_WaitDlg.SetMessage(strMsg);

	if (bShow)
	{
		if (m_WaitDlg.GetSafeHwnd())
			m_WaitDlg.SetMessage(strMsg);
		else
			m_WaitDlg.DoModal();
	}
	else
	{
		if (m_WaitDlg.GetSafeHwnd())
			m_WaitDlg.SendMessage(WM_CLOSE);
	}
}

void CMainFrame::WaitOver(CStringW strMsg)
{
	m_WaitDlg.SetMessage(strMsg);
	if (m_WaitDlg.GetSafeHwnd())
	{
		m_WaitDlg.KillTimer(1000);
		m_WaitDlg.Reset();
		m_WaitDlg.Invalidate();
	}
}

LRESULT CMainFrame::OnTransactionComplete(WPARAM wParam, LPARAM lParam)
{
	GetInitManager()->EndTransacation();

	return 0;
}