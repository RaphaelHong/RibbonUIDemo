

// VedioRouter.cpp : 定义应用程序的类行为。
//

#include <afxwin.h>
#include "stdafx.h"
#include "afxwinappex.h"
#include "VideoRouter.h"
#include "MainFrm.h"
#include "VideoRouterDoc.h"
#include "SwitchPanelView.h"
#include "Dialog components/LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVideoRouterApp

BEGIN_MESSAGE_MAP(CVideoRouterApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CVideoRouterApp::OnAppAbout)
END_MESSAGE_MAP()


// CVideoRouterApp 构造

CVideoRouterApp::CVideoRouterApp()
{
	m_bHiColorIcons = TRUE;

	ApplicationInitialize();
}

// 唯一的一个 CVideoRouterApp 对象

CVideoRouterApp theApp;


// CVideoRouterApp 初始化

BOOL CVideoRouterApp::InitInstance()
{
	GdiplusStartupInput gdiplusinput;
	GdiplusStartup(&gdiplusToken, &gdiplusinput, NULL);

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// 初始化 RichEdit2.0 控件
	if (!AfxInitRichEdit2())
	{
		AfxMessageBox(IDS_RICHEDIT2_INIT_FIALED);
	}

	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(0);  // 加载标准 INI 文件选项

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

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CVideoRouterDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CSwitchPanelView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	//m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生

	return TRUE;
}

BOOL CVideoRouterApp::ExitInstance()
{
	ApplicationCleanUp();

	return CWinAppEx::ExitInstance();
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CVideoRouterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CVideoRouterApp 自定义加载/保存方法

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

// CVideoRouterApp 消息处理程序
void CVideoRouterApp::ApplicationInitialize()
{
	if (pInitManager == NULL)
		pInitManager = new CInitialManager;

	pInitManager->InitManagers();

	//系统配置
	ZeroMemory(&g_CurrencyUser, sizeof(UserData));
	g_CurrencyUser.nUserID = -1;
}

void CVideoRouterApp::ApplicationCleanUp()
{
	SAFE_DELETE(pInitManager);

	// 清理GDI++对象
	GdiplusShutdown(gdiplusToken);

	// 清理自动保存
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