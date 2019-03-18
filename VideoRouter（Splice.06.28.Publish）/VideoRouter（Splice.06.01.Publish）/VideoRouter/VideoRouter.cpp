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

// VedioRouter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "VideoRouter.h"
#include "MainFrm.h"
#include "VideoRouterDoc.h"
#include "GridView.h"
#include "Dialog components/LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVideoRouterApp

BEGIN_MESSAGE_MAP(CVideoRouterApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CVideoRouterApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CVideoRouterApp ����

CVideoRouterApp::CVideoRouterApp()
{
	m_bHiColorIcons = TRUE;

	ApplicationInitialize();
}

// Ψһ��һ�� CVideoRouterApp ����

CVideoRouterApp theApp;


// CVideoRouterApp ��ʼ��

BOOL CVideoRouterApp::InitInstance()
{
	GdiplusStartupInput gdiplusinput;
	GdiplusStartup(&gdiplusToken, &gdiplusinput, NULL);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� RichEdit2.0 �ؼ�
	if (!AfxInitRichEdit2())
	{
		AfxMessageBox(IDS_RICHEDIT2_INIT_FIALED);
	}

	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

#ifndef _DEBUG
	CLoginDlg DLG;
	if (DLG.DoModal() != IDOK)
		return FALSE;
#endif

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVideoRouterDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CGridView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	//m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	return TRUE;
}

BOOL CVideoRouterApp::ExitInstance()
{
	ApplicationCleanUp();

	return CWinAppEx::ExitInstance();
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CVideoRouterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVideoRouterApp �Զ������/���淽��

void CVideoRouterApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CVideoRouterApp::LoadCustomState()
{
}

void CVideoRouterApp::SaveCustomState()
{
}

// CVideoRouterApp ��Ϣ�������
void CVideoRouterApp::ApplicationInitialize()
{
	if (pInitManager != NULL) {
		delete pInitManager;
		pInitManager = NULL;
	}

	pInitManager = new CInitialManager;
	pInitManager->InitManagers();

	//ϵͳ����
	ZeroMemory(&g_CurrencyUser, sizeof(UserData));
	g_CurrencyUser.nUserID = INVALID_VALUE;
}

void CVideoRouterApp::ApplicationCleanUp()
{
	SAFE_DELETE(pInitManager);

	// ����GDI++����
	GdiplusShutdown(gdiplusToken);

	// �����Զ�����
	//CleanState();

}

inline CInitialManager *GetInitManager()
{
	if (pInitManager == NULL)
	{
		pInitManager = new CInitialManager;
	}

	return pInitManager;
}

inline CGlobalManager *GetGlobalManager()
{
	if (pGlobalManager == NULL)
	{
		pGlobalManager = new CGlobalManager;
	}

	return pGlobalManager;
}

inline CCommunicateManager *GetCmdManager()
{
	if (pCmdManager == NULL)
	{
		pCmdManager = new CCommunicateManager;
	}

	return pCmdManager;
}

inline CInputManager *GetInputManager()
{
	if (pInputManager == NULL)
	{
		pInputManager = new CInputManager;
	}

	return pInputManager;
}

inline COutputManager *GetOutputManager()
{
	if (pOutputManager == NULL)
	{
		pOutputManager = new COutputManager;
	}

	return pOutputManager;
}

inline CBaseSceneManager *GetSceneManager()
{
	if (pSceneManager == NULL)
	{
		pSceneManager = new CBaseSceneManager;
	}

	return pSceneManager;
}

inline CLogManager *GetLogManager()
{
	if (pLogManager == NULL)
	{
		pLogManager = new CLogManager;
	}

	return pLogManager;
}

inline CMainFrame *GetMainFrame()
{
	if (theApp.m_pMainWnd->GetSafeHwnd())
		return (CMainFrame*)theApp.m_pMainWnd;
	else
		return NULL;
}