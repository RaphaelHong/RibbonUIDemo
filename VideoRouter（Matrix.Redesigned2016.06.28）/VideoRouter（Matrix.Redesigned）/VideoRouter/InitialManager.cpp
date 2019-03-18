// InitialManager.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "SwitchPanelView.h"
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

void CInitialManager::SetTransactionType(const emTransaction eTransaction)
{
	m_eTransaction = eTransaction;
}

BOOL CInitialManager::BeginTransaction(BOOL bShowWaitWnd)
{
	// 需要连接设备
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
		{
			// 设备初始化事务

			((CSwitchPanelView*)(GetMainFrame()->GetActiveView()))->ResetPage();

			// 初始化已经完成，开启线程读取所有通道的输入分辨率
			m_eTransaction = eTransaction_BeginInitTiming;
			BeginTransaction(FALSE);

			if (ALIVE_KEEP && m_bInitialized)
				GetMainFrame()->SetTimer(TIMER_KEEP_ALIVE, IDLE_TIME, NULL);

			//跳出函数处理
			return;

		}
		break;
	case eTransaction_RefreshSignal:
		{
			// 信号刷新事务

			((CSwitchPanelView*)(GetMainFrame()->GetActiveView()))->RefreshComplete();

		}

		break;

	case eTransaction_BeginInitTiming:
		{
			// 端口分辨率初始化事务
		}
		break;

	case eTranscation_ResetDevice:
		{
			// 设备复位事务
		}
		
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

			GetMainFrame()->ShowWaittingDlg(TRUE, L"开始初始化端口信息");

			// 端口初始化
			if (!GetInputManager()->RealInit())
			{
				GetMainFrame()->WaitOver(L"sorry，初始化端口失败，已退出初始化！");
				Sleep(1000);
				break;
			}
			Sleep(200);

			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在读取端口分辨率");
			Sleep(200);

			// 场景初始化
			GetMainFrame()->ShowWaittingDlg(TRUE, L"开始初始化场景");

			// 获取所有已存储的场景
			GetSceneManager()->InitSceneFromDevice();

			// 获取当前场景，并回显
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
			if (!GetInputManager()->RefreshAllSignals())
			{
				GetMainFrame()->WaitOver( L"sorry，刷新失败！");
				Sleep(1000);
				break;
			}
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
		{ // 复位事务处理

			// 输出复位中
			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在复位输出板卡");
			Sleep(100);
			GetOutputManager()->ResetAll();

			// 输入复位中
			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在复位输入板卡");
			Sleep(100);
			GetInputManager()->ResetAll();

			// 控制卡复位中
			GetMainFrame()->ShowWaittingDlg(TRUE, L"正在复位控制板卡");
			GetCmdManager()->CmdRSTF(SYSCARD_PORT, eCardTypeCtrl);
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