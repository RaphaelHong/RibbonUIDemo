// InitialManager.cpp : ʵ���ļ�
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
// CInitialManager ��Ա����

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

	//// ����˿� �� ����˿�
	//GetInputManager()->RealInit();

	//// ƴ������
	//InitSpliceLayout();

	//// ������Ϣ
	//InitSpliceScene();

	return TRUE;
}

BOOL CInitialManager::InitSpliceLayout()
{
	// ƴ�Ӳ���
	SPLICELAYOUT layout;

	if (GetCmdManager()->CmdRCN1(SYSBOARD_PORT, eBoardTypeCtrl, &layout) == CMD_RTOK)
		::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_RECV_LAYOUT_INF, WPARAM(&layout), NULL);
	else
		return FALSE;

	// ����� - ��Ļӳ��
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

	// ��ʼ����ǰ����
	if (GetCmdManager()->CmdCALL(SYSBOARD_PORT, eBoardTypeCtrl, CURRENT_SCENE, pData, nLen, TRUE) == CMD_RTOK)
	{
		::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_RECV_SCENE_DATA, (WPARAM)pData, (LPARAM)&nLen);

		SAFE_DELETE_AR(pData);
		return TRUE;
	}

	SAFE_DELETE_AR(pData);
	return FALSE;

	// ��������
	//CSceneHeadInfo* pScene;
	//int nSceneID = 0;
	//CString strName;
	//for (int i=0; i<_MAX_SCENE_COUNT; i++)
	//{
	//	nSceneID = i+1;
	//	if (GetCmdManager()->CmdCALL(SYSBOARD_PORT, eBoardTypeCtrl, nSceneID, data, nLen, TRUE) == CMD_RTOK)
	//	{
	//		// �����б����Ѿ����ڸó���
	//		if (GetSceneManager()->FindByID(nSceneID) != NULL)
	//			continue;
	//		else
	//		{// �����б���û�иĳ���
	//			strName.Format(_T("����%d"), nSceneID);
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

	////���³����˵�
	//GetMainFrame()->ArrangeSceneMenus();

	return TRUE;
}


void CInitialManager::SetTransactionType(const emTransaction eTransaction)
{
	m_eTransaction = eTransaction;
}

BOOL CInitialManager::BeginTransaction(BOOL bShowWaitWnd)
{
	// ��Ҫ����
#ifndef _DEBUG
	if (!CCommunicateManager::IsConnected())
		return FALSE;
#endif

	// �����������߳�
	hInitThread = CreateThread(NULL, 0, TransactionProcessor, (LPVOID)&m_eTransaction, 0, NULL);

	// ��ʾһ�����Ի��ĵȴ��Ի���
	if (bShowWaitWnd)
		GetMainFrame()->ShowWaittingDlg(TRUE, L"Ready");

	return TRUE;
}

void CInitialManager::EndTransacation()
{
	// �رյȴ��Ի���
	GetMainFrame()->ShowWaittingDlg(FALSE, NULL);

	switch (m_eTransaction)
	{
	case eTransaction_BeginInitDevice:
		// �豸��ʼ������

		// ��ʼ���Ѿ���ɣ������̶߳�ȡ����ͨ��������ֱ���
		m_eTransaction = eTransaction_BeginInitTiming;
		BeginTransaction(FALSE);

		if (ALIVE_KEEP && m_bInitialized)
			GetMainFrame()->SetTimer(TIMER_KEEP_ALIVE, IDLE_TIME, NULL);
		//������������
		return;

		break;
	case eTransaction_RefreshSignal:
		// �ź�ˢ������

		break;

	case eTransaction_BeginInitTiming:
		// �˿ڷֱ��ʳ�ʼ������
		break;

	case eTranscation_ResetDevice:
		// �豸��λ����
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
		{// �豸��ʼ������

			// �˿ڳ�ʼ��
			GetMainFrame()->ShowWaittingDlg(TRUE, L"��ʼ��ʼ���˿���Ϣ");
			if (!GetInputManager()->RealInit())
			{
				GetMainFrame()->WaitOver(L"sorry����ʼ���˿�ʧ�ܣ����˳���ʼ����");
				Sleep(1000);
				break;
			}
			Sleep(200);

			// ƴ�ӱ�����ʼ��
			if (!GetInitManager()->InitSpliceLayout())
			{
				GetMainFrame()->WaitOver(L"sorry��������ʼ��ʧ�ܣ����˳���ʼ����");
				Sleep(1000);
				break;
			}

			// ������ʼ��
			GetMainFrame()->ShowWaittingDlg(TRUE, L"��ʼ��ʼ������");
			GetSceneManager()->InitSceneFromDevice();

			if (!GetSceneManager()->LoadSceneFromDevice(CURRENT_SCENE))
			{
				GetMainFrame()->WaitOver( L"sorry����ʼ������ʧ�ܣ�");
				Sleep(1000);
				break;
			}
			Sleep(200);

			// �ѳ�ʼ�����
			GetInitManager()->SetInitializedFlag(TRUE);

			GetMainFrame()->WaitOver( L"��ʼ����ϣ�");
			Sleep(1000);
		}

		break;
	case eTransaction_RefreshSignal:
		{// �ź�ˢ������

			GetMainFrame()->ShowWaittingDlg(TRUE, L"����ˢ���ź�");

			// ˢ�������ź�
			GetInputManager()->RefreshAllSignals();
			Sleep(200);

			GetMainFrame()->WaitOver( L"�ź�ˢ����ɣ�");
			Sleep(1000);
		}

		break;
	case eTransaction_BeginInitTiming:
		{ // ��ʱ���ڶ˿����öԻ����һ�δ򿪵�ʱ���ʼ��

		}

		break;
	case eTranscation_ResetDevice:
		{
			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڸ�λ����忨");
			Sleep(100);
			GetOutputManager()->ResetAll();

			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڸ�λ����忨");
			Sleep(100);
			GetInputManager()->ResetAll();

			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڸ�λ���ư忨");
			GetCmdManager()->CmdRSTF(SYSBOARD_PORT, eBoardTypeCtrl);
			Sleep(100);

			GetMainFrame()->ShowWaittingDlg(TRUE, L"�豸��λ�ȴ���");
			Sleep(3000);

			GetMainFrame()->WaitOver(L"��λ��ɣ��豸�����ϵ����Ч��");
			Sleep(1000);
		}

		break;
	default:break;
	}	

	GetMainFrame()->SendMessage(WM_USER_TRANS_COMPLETE);

	return 0x2016;
}