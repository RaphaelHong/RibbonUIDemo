// InitialManager.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "InitialManager.h"

HANDLE hInitThread = NULL;
DWORD WINAPI TransactionProcessor(LPVOID lpvoid);

// CInitialManager

CInitialManager::CInitialManager()
{
	m_eTransaction = eTransaction_Undef;
	m_bInitialized = FALSE;
}

CInitialManager::~CInitialManager()
{
	ReleaseManagers();
}

CInitialManager *pInitManager = NULL;
// CInitialManager 成员函数

void CInitialManager::InitManagers()
{
	if (pGlobalManager == NULL)
	{
		pGlobalManager = new CGlobalManager;
	}

	if (pCmdManager == NULL)
	{
		pCmdManager = new CCommunicateManager;
	}

	if (pInputManager == NULL)
	{
		pInputManager = new CInputManager;

		pInputManager->DemoInit();
	}

	if (pOutputManager == NULL)
	{
		pOutputManager = new COutputManager;

		pOutputManager->DemoInit();
	}

	if (pSceneManager == NULL)
	{
		pSceneManager = new CBaseSceneManager;
		pSceneManager->InitFile();
	}

	if (pLogManager == NULL)
	{
		pLogManager = new CLogManager;
	}
}

void CInitialManager::ReleaseManagers()
{
	SAFE_DELETE(pGlobalManager);
	SAFE_DELETE(pCmdManager);
	SAFE_DELETE(pInputManager);
	SAFE_DELETE(pOutputManager);
	SAFE_DELETE(pSceneManager);
	SAFE_DELETE(pLogManager);
}

void CInitialManager::SetInitializedFlag(BOOL bInit)
{
	m_bInitialized = bInit;
}

BOOL CInitialManager::GetInitializedFlag() const 
{
	return m_bInitialized;
}

BOOL CInitialManager::InitDeviceContents()
{
	if (! CCommunicateManager::IsConnected())
		return FALSE;

	CWaitCursor wait;
	CRedrawLock lock((CWnd*)GetMainFrame()->GetActiveView());

	m_eTransaction = eTransaction_BeginInitDevice;
	BeginTransaction(TRUE);

	//// 输入端口 和 输出端口
	//GetInputManager()->RealInit();

	//// 拼接属性
	//InitSpliceLayout();

	//// 场景信息
	//InitSpliceScene();

	return TRUE;
}

BOOL CInitialManager::InitSpliceLayout()
{
	// 拼接布置
	SPLICELAYOUT layout;

	if (GetCmdManager()->CmdRCN1(SYSBOARD_PORT, eBoardTypeCtrl, &layout) == CMD_RTOK)
		::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_RECV_LAYOUT_INF, WPARAM(&layout), NULL);
	else
		return FALSE;

	// 输出口 - 屏幕映射
	int iScreens = layout.iPhyHorz*layout.iPhyVert;
	pSCREENMAP pscrmap = new SCREENMAP[iScreens];

	for (int i=0; i<iScreens; i++)
	{
		pscrmap[i].iScreenID = i+1;

		GetCmdManager()->CmdGOPS(SYSBOARD_PORT, eBoardTypeCtrl, pscrmap[i]);
	}

	::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_RECV_SCREEN_INF, (WPARAM)pscrmap, (LPARAM)&iScreens);

	SAFE_DELETE_AR(pscrmap);

	return TRUE;
}

BOOL CInitialManager::InitSpliceScene()
{
	int nLen = 0;
	UINT *pData = NULL;

	// 初始化当前场景
	if (GetCmdManager()->CmdCALL(SYSBOARD_PORT, eBoardTypeCtrl, CURRENT_SCENE, pData, nLen, TRUE) == CMD_RTOK)
	{
		::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_RECV_SCENE_DATA, (WPARAM)pData, (LPARAM)&nLen);

		SAFE_DELETE_AR(pData);
		return TRUE;
	}

	SAFE_DELETE_AR(pData);
	return FALSE;

	// 其它场景
	//CSceneHeadInfo* pScene;
	//int nSceneID = 0;
	//CString strName;
	//for (int i=0; i<_MAX_SCENE_COUNT; i++)
	//{
	//	nSceneID = i+1;
	//	if (GetCmdManager()->CmdCALL(SYSBOARD_PORT, eBoardTypeCtrl, nSceneID, data, nLen, TRUE) == CMD_RTOK)
	//	{
	//		// 场景列表中已经存在该场景
	//		if (GetSceneManager()->FindByID(nSceneID) != NULL)
	//			continue;
	//		else
	//		{// 场景列表中没有改场景
	//			strName.Format(_T("场景%d"), nSceneID);
	//			pScene = new CSceneHeadInfo;
	//			pScene->GetID() = nSceneID;
	//			pScene->GetLargeImage() = nSceneID%9;
	//			pScene->GetSmallImage() = nSceneID%9;
	//			pScene->GetExcuteTime() = 5000;
	//			pScene->GetRemainTime() = 5000;
	//			pScene->SetName(strName.GetBuffer());
	//			pScene->GetQuickMenuFlag() = FALSE;
	//			GetSceneManager()->AddItem(pScene);
	//		}
	//	}
	//}

	////更新场景菜单
	//GetMainFrame()->ArrangeSceneMenus();

	return TRUE;
}


void CInitialManager::SetTransactionType(const emTransaction eTransaction)
{
	m_eTransaction = eTransaction;
}

BOOL CInitialManager::BeginTransaction(BOOL bShowWaitWnd)
{
	// 需要连接
#ifndef _DEBUG
	if (!CCommunicateManager::IsConnected())
		return FALSE;
#endif

	// 开启事务处理线程
	hInitThread = CreateThread(NULL, 0, TransactionProcessor, (LPVOID)&m_eTransaction, 0, NULL);

	// 显示一个个性化的等待对话框
	if (bShowWaitWnd)
		GetMainFrame()->ShowWaittingDlg(TRUE, L"Ready");

	return TRUE;
}

void CInitialManager::EndTransacation()
{
	// 关闭等待对话框
	GetMainFrame()->ShowWaittingDlg(FALSE, NULL);

	switch (m_eTransaction)
	{
	case eTransaction_BeginInitDevice:
		// 设备初始化事务

		// 初始化已经完成，开启线程读取所有通道的输入分辨率
		m_eTransaction = eTransaction_BeginInitTiming;
		BeginTransaction(FALSE);

		if (ALIVE_KEEP && m_bInitialized)
			GetMainFrame()->SetTimer(TIMER_KEEP_ALIVE, IDLE_TIME, NULL);
		//跳出函数处理
		return;

		break;
	case eTransaction_RefreshSignal:
		// 信号刷新事务

		break;

	case eTransaction_BeginInitTiming:
		// 端口分辨率初始化事务
		break;

	case eTranscation_ResetDevice:
		// 设备复位事务
		break;

	default:
		break;
	}

	m_eTransaction = eTransaction_Undef;
}

DWORD WINAPI TransactionProcessor(LPVOID lpvoid)
{
	emTransaction Transaction = *((emTransaction*)lpvoid);

	Sleep(100);

	switch (Transaction)
	{
	case eTransaction_BeginInitDevice:
		{// 设备初始化事务

			// 端口初始化
			GetMainFrame()->ShowWaittingDlg(TRUE, L"开始初始化端口信息");
			if (!GetInputManager()->RealInit())
			{
				GetMainFrame()->WaitOver(L"sorry，初始化端口失败，已退出初始化！");
				Sleep(1000);
				break;
			}
			Sleep(200);

			// 拼接背景初始化
			if (!GetInitManager()->InitSpliceLayout())
			{
				GetMainFrame()->WaitOver(L"sorry，背景初始化失败，已退出初始化！");
				Sleep(1000);
				break;
			}

			// 场景初始化
			GetMainFrame()->ShowWaittingDlg(TRUE, L"开始初始化场景");
			GetSceneManager()->InitSceneFromDevice();

			if (!GetSceneManager()->LoadSceneFromDevice(CURRENT_SCENE))
			{
				GetMainFrame()->WaitOver( L"sorry，初始化场景失败！");
				Sleep(1000);
				break;
			}
			Sleep(200);

			// 已初始化完毕
			GetInitManager()->SetInitializedFlag(TRUE);

			GetMainFrame()->WaitOver( L"初始化完毕！");
			Sleep(1000);
		}

		break;
	case eTransaction_RefreshSignal:
		{// 信号刷新事务

			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在刷新信号");

			// 刷新所有信号
			GetInputManager()->RefreshAllSignals();
			Sleep(200);

			GetMainFrame()->WaitOver( L"信号刷新完成！");
			Sleep(1000);
		}

		break;
	case eTransaction_BeginInitTiming:
		{ // 暂时放在端口设置对话框第一次打开的时候初始化

		}

		break;
	case eTranscation_ResetDevice:
		{
			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在复位输出板卡");
			Sleep(100);
			GetOutputManager()->ResetAll();

			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在复位输入板卡");
			Sleep(100);
			GetInputManager()->ResetAll();

			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在复位控制板卡");
			GetCmdManager()->CmdRSTF(SYSBOARD_PORT, eBoardTypeCtrl);
			Sleep(100);

			GetMainFrame()->ShowWaittingDlg(TRUE, L"设备复位等待中");
			Sleep(3000);

			GetMainFrame()->WaitOver(L"复位完成，设备重新上电后生效！");
			Sleep(1000);
		}

		break;
	default:break;
	}	

	GetMainFrame()->SendMessage(WM_USER_TRANS_COMPLETE);

	return 0x2016;
}