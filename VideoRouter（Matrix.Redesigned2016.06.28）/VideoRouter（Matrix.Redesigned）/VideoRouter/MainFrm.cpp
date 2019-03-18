// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "MainFrm.h"
#include "VideoRouterDoc.h"
#include "VideoRouterView.h"
#include "SwitchPanelView.h"
#include "Dialog components/ConnectConfigDlg.h"
#include "Dialog components/InputConfigDlg.h"
#include "Dialog components/OutputConfigDlg.h"
#include "Dialog components/SceneLoopDlg.h"
#include "Dialog components/SceneManagerDlg.h"
#include "Dialog components/TestPatternDlg.h"
#include "Dialog components/LoginDlg.h"
#include "Dialog components/BackupAndRestoreDlg.h"
#include "Dialog components/UserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_RIBBON_CMD_CONNECT, &CMainFrame::OnCmdConnect)
	ON_COMMAND(ID_RIBBON_CMD_SC_LOAD, &CMainFrame::OnCmdLoadScene)
	ON_COMMAND(ID_RIBBON_CMD_SC_SAVE, &CMainFrame::OnCmdSaveScene)
	ON_COMMAND(ID_RIBBON_CMD_REFRESH, &CMainFrame::OnCmdRefreshScene)
	ON_COMMAND(ID_RIBBON_CMD_SC_LOOP, &CMainFrame::OnCmdLoopScene)
	ON_COMMAND(ID_RIBBON_CMD_SC_MANAGE, &CMainFrame::OnCmdManaScene)
	ON_COMMAND(ID_RIBBON_CMD_INPUT_SETUP, &CMainFrame::OnCmdInputSetup)
	ON_COMMAND(ID_RIBBON_CMD_OUTPUT_SETUP, &CMainFrame::OnCmdOutputSetup)
	ON_COMMAND(ID_RIBBON_CMD_IMAGE_TEST, &CMainFrame::OnCmdOutputTest)
	ON_COMMAND(ID_RIBBON_CMD_BACKUP, &CMainFrame::OnCmdSysBackup)
	ON_COMMAND(ID_RIBBON_CMD_RESTORE, &CMainFrame::OnCmdSysRestore)
	//ON_COMMAND(ID_RIBBON_CMD_POWER, &CMainFrame::OnCmdPowerControl)

	ON_COMMAND(ID_RIBBON_CMD_USER, &CMainFrame::OnCmdUserManage)
	ON_COMMAND(ID_RIBBON_CMD_LANG, &CMainFrame::OnCmdLanguage)
	ON_COMMAND(ID_RIBBON_CMD_USER_CHANGE, &CMainFrame::OnCmdUserChange)
	ON_COMMAND_RANGE(ID_RIBBON_SCENE_ID_BEGIN, ID_RIBBON_SCENE_ID_END, &CMainFrame::OnCmdRangeSceneID)
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_RIBBON_CMD_MATRIX_MODE, &CMainFrame::OnCmdMatrixMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RIBBON_CMD_SC_LOAD, ID_RIBBON_CMD_SC_MANAGE, &CMainFrame::OnUpdateCmdRangeScene)
	ON_WM_TIMER()
	ON_COMMAND(ID_RIBBON_SLIDER_ZOOM, &CMainFrame::OnSliderZoom)
	ON_MESSAGE(WM_USER_TRANS_COMPLETE, &CMainFrame::OnTransactionComplete)
	ON_COMMAND(ID_RIBBON_CMD_RESET, &CMainFrame::OnCmdReset)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_CMD_BACKUP, &CMainFrame::OnUpdateCmdBackup)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_CMD_RESTORE, &CMainFrame::OnUpdateCmdRestore)
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);

	m_nTimeoutCount = 0;

	m_pWaittingDlg = NULL;
}

CMainFrame::~CMainFrame()
{
	if (m_pWaittingDlg != NULL)
	{
		m_pWaittingDlg->DestroyWindow();
		SAFE_DELETE(m_pWaittingDlg);
	}
}

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

	//BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	//���Ribbonʽ�˵�
	m_wndRibbonBar.Create(this);
	//InitializeRibbon();
	SetRibbonBar();
	ArrangeSceneMenus();

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
	//if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, m_wndCalendar, 250))
	//{
	//	TRACE0("δ�ܴ�����������\n");
	//	return -1;      // δ�ܴ���
	//}

	// ����������:
	//if (!CreateCaptionBar())
	//{
	//	TRACE0("δ�ܴ���������\n");
	//	return -1;      // δ�ܴ���
	//}

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
	//m_wndStatusBar.AddSeparator();
	//m_wndStatusBar.AddElement(new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, _T("������")), _T("������"));
	//m_wndStatusBar.AddSeparator();
	//pLabel = new CMFCRibbonLabel(_T("100%"));
	//m_wndStatusBar.AddElement(pLabel, _T("100%"));
	//CMFCRibbonSlider *pSlider = new CMFCRibbonSlider(1111, 150);
	//pSlider->SetZoomButtons(TRUE);
	//pSlider->SetRange(-5, 5);
	//pSlider->SetZoomIncrement(1);
	//pSlider->SetPos(0);
	//m_wndStatusBar.AddElement(pSlider, _T("Slider"));
	CMFCRibbonSlider *pSlider = new CMFCRibbonSlider(ID_RIBBON_SLIDER_ZOOM, 150);
	pSlider->SetZoomButtons(TRUE);
	pSlider->SetRange(1, 3);
	pSlider->SetZoomIncrement(1);
	pSlider->SetPos(CSwitchPage::m_sZoomLvl);
	m_wndStatusBar.AddExtendedElement(pSlider, _T("Slider"));
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonLabel(_T("״̬2")), _T("״̬2"));

	return TRUE;
}

void CMainFrame::SetRibbonBar()
{
	m_wndRibbonBar.EnablePrintPreview(FALSE);
	//m_wndRibbonBar.EnableToolTips(FALSE, FALSE);
	//m_wndRibbonBar.EnableTrackingToolTips(FALSE);

	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_RIBBON_BUTTONS);
	m_PanelImages.SetAlwaysLight(TRUE);

	//------------- Ӧ�ó���ť ---------------//
	SetRibbonAppButton();

	//------------- ������ ��� ------------//
	{
	CMFCRibbonCategory *pCategoryMain = m_wndRibbonBar.AddCategory(_T("������ҳ"), IDB_CATEGORY_MAIN_SMALL, IDB_CATEGORY_MAIN_LARGE);

	//ģʽ�л����
	CMFCRibbonPanel *pModePanel = pCategoryMain->AddPanel(_T("����ģʽ"), m_PanelImages.ExtractIcon(7));
	CMFCRibbonButton *pBtnByPass = new CMFCRibbonButton(ID_RIBBON_CMD_MATRIX_MODE, _T("����ģʽ"), 21, 21);
	//pBtnByPass->AddSubItem(new CMFCRibbonCheckBox(ID_RIBBON_CMD_SHOW_MODEL, _T("��ʾ/����ģ�����")));
	pModePanel->Add(pBtnByPass);

	//ͨ�����
	CMFCRibbonPanel *pCommunicatePanel = pCategoryMain->AddPanel(_T("ͨѶ������"), m_PanelImages.ExtractIcon(1));
	pCommunicatePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_CONNECT, _T("����"), 14, 14));

	//�������
	CMFCRibbonPanel *pScenePanel = pCategoryMain->AddPanel(_T("����������"), m_PanelImages.ExtractIcon(2));
	pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_LOAD, _T("��ȡ"), 10, 10));
	pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_SAVE, _T("����"), 9, 9));
	pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_REFRESH, _T("ˢ��"), 12, 12));
	pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_LOOP, _T("��Ѳ"), 13, 13));
	pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_SC_MANAGE, _T("�༭"), 11, 11));
	pScenePanel->Add(new CMFCRibbonSeparator());

	//�������
	CMFCRibbonPanel *pInputPanel = pCategoryMain->AddPanel(_T("�˿ڹ���"), m_PanelImages.ExtractIcon(3));
	pInputPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_INPUT_SETUP, _T("�������"), 18, 18));
	pInputPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_OUTPUT_SETUP, _T("�������"), 15, 15));

	pInputPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_IMAGE_TEST, _T("ͼ�����"), 19, 19));

	//������
	//CMFCRibbonPanel *pOutputPanel = pCategoryMain->AddPanel(_T("���������"), m_PanelImages.ExtractIcon(4));
	

	//��ͼ���
/*	CMFCRibbonPanel* pPanelView = pCategoryMain->AddPanel(_T("��ͼ"), m_PanelImages.ExtractIcon (5));
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, _T("״̬��"));
	pPanelView->Add(pBtnStatusBar);
	CMFCRibbonButton* pBtnCaptionBar = new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, _T("������"));
	pPanelView->Add(pBtnCaptionBar);
*/
	//ϵͳ�˳�
	CMFCRibbonPanel *pQuitPanel = pCategoryMain->AddPanel(_T("�˳�"), m_PanelImages.ExtractIcon(6));
	pQuitPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_USER_CHANGE, _T("�л��û�"), 22, 22));
	pQuitPanel->Add(new CMFCRibbonButton(ID_APP_EXIT, _T("�˳�"), 17, 17));
	}

	//------------- ������ ��� --------------//
	//{
	CMFCRibbonCategory *pCategoryMana = m_wndRibbonBar.AddCategory(_T("ϵͳ����"), IDB_CATEGORY_MANAGE_SMALL, IDB_CATEGORY_MANAGE_LARGE);
	//ϵͳ���ݻ�ԭ���
	CMFCRibbonPanel *pBackupPanel = pCategoryMana->AddPanel(_T("�����뻹ԭ"), m_PanelImages.ExtractIcon(1));
	pBackupPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_BACKUP, _T("ϵͳ����"), 0, 0));
	pBackupPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_RESTORE, _T("ϵͳ��ԭ"), 1, 1));

	//��λ����
	CMFCRibbonPanel *pResetPanel = pCategoryMana->AddPanel(_T("�豸��λ"), m_PanelImages.ExtractIcon(4));
	pResetPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_RESET, _T("��λ"), 4, 4));

	//��Դ�������
	//CMFCRibbonPanel *pPowerPanel = pCategoryMana->AddPanel(_T("��Դ����"), m_PanelImages.ExtractIcon(2));
	//pPowerPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_POWER, _T("��Դ"), 2, 2));

	//�û����
	CMFCRibbonPanel *pUserPanel = pCategoryMana->AddPanel(_T("Ȩ�޹���"), m_PanelImages.ExtractIcon(3));
	pUserPanel->Add(new CMFCRibbonButton(ID_RIBBON_CMD_USER, _T("�û�"), 3, 3));


	//�����л����
	//CMFCRibbonPanel *pLangPanel = pCategoryMana->AddPanel(_T("Language"), m_PanelImages.ExtractIcon(4));
	//CMFCRibbonComboBox *pCBox = new CMFCRibbonComboBox(ID_RIBBON_CMD_LANG, FALSE, 100, _T("����"));
	//pCBox->AddItem(_T("��������(�й�)"));
	//pCBox->AddItem(_T("English(U.S)"));
	//pCBox->AddItem(_T("��������(�й�̨��)"));
	//pCBox->SelectItem(0);
	//pLangPanel->Add(pCBox);
	//pLangPanel->SetCenterColumnVert(TRUE);
	//}

	//------------- ���ٷ����� -------------//
	{
	CList <UINT,UINT> lstCmd;
	lstCmd.AddTail(ID_APP_EXIT);

	m_wndRibbonBar.SetQuickAccessCommands(lstCmd);

	//------------- ��ʽ��ť ---------------//
	CMFCRibbonButton *pStyleBtn = new CMFCRibbonButton(-1, _T("���"), -1, -1);
	pStyleBtn->SetMenu(IDR_THEME_MENU);
// 	pStyleBtn->SetToolTipText(_T("ѡ�񴰿ڷ��\n"));
// 	pStyleBtn->SetDescription(_T("Office 2007 styles"));
	m_wndRibbonBar.AddToTabs(pStyleBtn);

	//------------- ���ڰ�ť ---------------//
	CMFCRibbonButton *pAboutBtn = new CMFCRibbonButton(ID_APP_ABOUT, NULL, m_PanelImages.ExtractIcon(0));
	m_wndRibbonBar.AddToTabs(pAboutBtn);
	}

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

	//CString strScName;
	//int nImageIndex;
	//int nScID;
	//for (int i=0; i<m_SceneManager.GetCount(); i++)
	//{
	//	nScID = m_SceneManager.GetSceneID(i);
	//	strScName = m_SceneManager.GetName(FALSE, i);
	//	nImageIndex = m_SceneManager.GetLargeImageIndex(FALSE, i);

	//	pBtnLoad->AddSubItem(new CMFCRibbonButton(ID_SCENE_1+nScID, strScName, nImageIndex, nImageIndex));
	//}

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


//BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth)
//{
//	CWindowDC dc(NULL);
//
//	bar.SetMode2003();
//
//	BOOL bNameValid;
//	CString strTemp;
//	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
//	ASSERT(bNameValid);
//	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
//	{
//		return FALSE; // fail to create
//	}
//
//	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();
//
//	if (pOutlookBar == NULL)
//	{
//		ASSERT(FALSE);
//		return FALSE;
//	}
//
//	pOutlookBar->EnableInPlaceEdit(TRUE);
//
//	static UINT uiPageID = 1;
//
//	DWORD dwPaneStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_FLOAT_MULTI;
//
//	// can float, can autohide, can resize, CAN NOT CLOSE
//	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;
//
//	CRect rectDummy(0, 0, 0, 0);
//	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
//
//	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
//	bNameValid = strTemp.LoadString(IDS_FOLDERS);
//	ASSERT(bNameValid);
//	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);
//
//	calendar.Create(rectDummy, &bar, 1201);
//	bNameValid = strTemp.LoadString(IDS_CALENDAR);
//	ASSERT(bNameValid);
//	pOutlookBar->AddControl(&calendar, strTemp, 3, TRUE, dwStyle);
//
//	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
//
//	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
//	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
//	pOutlookBar->RecalcLayout();
//
//	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
//	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);
//
//	bar.SetButtonsFont(&afxGlobalData.fontBold);
//
//	return TRUE;
//}

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
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
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


void CMainFrame::OnCmdConnect()
{
	CConnectConfigDlg DLG;
	if (DLG.DoModal() != IDOK)
		return;

	//���ӳɹ���ʼ�����豸����
	if (CCommunicateManager::IsConnected())
	{
		GetInitManager()->SetTransactionType(eTransaction_BeginInitDevice);
		GetInitManager()->BeginTransaction(TRUE);
	}
}

void CMainFrame::OnCmdLoadScene()
{
	CSceneManagerDlg DLG(SCENE_MANAGER_LOAD);
	DLG.DoModal();
}

void CMainFrame::OnCmdSaveScene()
{
	CSceneManagerDlg DLG(SCENE_MANAGER_SAVE);
	DLG.DoModal();
}

void CMainFrame::OnCmdRefreshScene()
{
	GetSceneManager()->LoadSceneFromDevice(0);
}

void CMainFrame::OnCmdLoopScene()
{
	CSceneLoopDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdManaScene()
{
	CSceneManagerDlg DLG(SCENE_MANAGER_EDIT);
	DLG.DoModal();
}

void CMainFrame::OnCmdInputSetup()
{
	CInputConfigDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdOutputSetup()
{
	COutputConfigDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdOutputTest()
{
	CTestPatternDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdUserChange()
{
	CLoginDlg DLG;
	if (DLG.DoModal() != IDOK)
		return;
}

void CMainFrame::OnCmdSysBackup()
{
	CBackupAndRestoreDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdSysRestore()
{
	CBackupAndRestoreDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdPowerControl()
{

}

void CMainFrame::OnCmdUserManage()
{
	CUserDlg DLG;
	DLG.DoModal();
}

void CMainFrame::OnCmdLanguage()
{
	return;

	CMFCRibbonComboBox *pCBox = (CMFCRibbonComboBox*)m_wndRibbonBar.FindByID(ID_RIBBON_CMD_LANG);
	int nLangSel = pCBox->GetCurSel();

}

void CMainFrame::OnCmdRangeSceneID(UINT nID)
{
	int nSceneID = nID - ID_RIBBON_SCENE_ID_BEGIN + 1;
	GetSceneManager()->LoadSceneFromDevice(nSceneID);
}

void CMainFrame::OnCmdReset()
{
	GetInitManager()->SetTransactionType(eTranscation_ResetDevice);
	GetInitManager()->BeginTransaction(TRUE);
}

void CMainFrame::OnUpdateCmdBackup(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateCmdRestore(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnClose()
{
	if (MessageBox(_T("�˳��˳���"), _T("����"), MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	CFrameWndEx::OnClose();
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;

	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnCmdMatrixMode()
{
	
}

void CMainFrame::OnUpdateCmdRangeScene(CCmdUI *pCmdUI)
{
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
				m_nTimeoutCount = 0;
				SetTimer(TIMER_KEEP_ALIVE, ALIVE_INTENAL, NULL);
			}
		}		

		break;

	case TIMER_KEEP_ALIVE:
		{
			if (!CCommunicateManager::IsConnected())
			{
				// ��������ѶϿ���رն�ʱ��

				KillTimer(nIDEvent);
			}
			else if (!GetCmdManager()->OnlineDetect())
			{
				// ������ʱ����ʼ����������3�γ�ʱ����Ϊ�����ѶϿ�
				m_nTimeoutCount++;

				if (m_nTimeoutCount >= MAX_TIMEOUT_NUM)
				{
					KillTimer(nIDEvent);

					GetCmdManager()->Close();

					AfxMessageBox(_T("�����ѶϿ�..."), MB_OK|MB_ICONWARNING);
				}
			}
			else {
				// ����������Ӧ���رյȴ���ʱ����������һ�����߼������
				KillTimer(TIMER_IDLE);
				m_nTimeoutCount = 0;
			}
		}		

		break;

	default:break;
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


void CMainFrame::ShowWaittingDlg(BOOL bShow, CStringW strMsg)
{
	//if (m_pWaittingDlg == NULL)
	//{
	//	m_pWaittingDlg = new CWaittingMsg();
	//	m_pWaittingDlg->Create(IDD_DLG_WAITING, this);
	//}

	//if (bShow)
	//{
	//	m_pWaittingDlg->SetMessage(strMsg);
	//	m_pWaittingDlg->ShowWindow(SW_SHOW);
	//	m_pWaittingDlg->CenterWindow();

	//	CRect rc;
	//	m_pWaittingDlg->GetWindowRect(rc);
	//	m_pWaittingDlg->SetWindowPos(&wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	//	m_pWaittingDlg->Reset();
	//	m_pWaittingDlg->SetTimer(1000, 200, NULL);
	//}
	//else
	//{
	//	m_pWaittingDlg->ShowWindow(SW_HIDE);
	//	m_pWaittingDlg->KillTimer(1000);
	//}

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

void CMainFrame::OnSliderZoom()
{
	CSwitchPanelView* pView = (CSwitchPanelView*)GetActiveView();

	CMFCRibbonSlider* pSlider = (CMFCRibbonSlider*)m_wndStatusBar.GetExElement(0);
	int nPos = pSlider->GetPos();

	pView->SetZoomLevel(nPos);
}


LRESULT CMainFrame::OnTransactionComplete(WPARAM wParam, LPARAM lParam)
{
	GetInitManager()->EndTransacation();

	return 0;
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

		pAppLoadBtn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_SCENE_ID_BEGIN + nScID - 1, strScName, nImageIndex, -1));
		pLoadBtn->AddSubItem(new CMFCRibbonButton(ID_RIBBON_SCENE_ID_BEGIN + nScID - 1, strScName, nImageIndex, -1));

		if (pItem->GetQuickMenuFlag() == TRUE)
			pScenePanel->Add(new CMFCRibbonButton(ID_RIBBON_SCENE_ID_BEGIN + nScID - 1, strScName, nImageIndex, nImageIndex));
	}

	m_wndRibbonBar.ForceRecalcLayout();
}