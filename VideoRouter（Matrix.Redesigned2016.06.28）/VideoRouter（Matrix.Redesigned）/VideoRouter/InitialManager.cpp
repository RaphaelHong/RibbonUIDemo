// InitialManager.cpp : ʵ���ļ�
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

void CInitialManager::SetTransactionType(const emTransaction eTransaction)
{
	m_eTransaction = eTransaction;
}

BOOL CInitialManager::BeginTransaction(BOOL bShowWaitWnd)
{
	// ��Ҫ�����豸
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
		{
			// �豸��ʼ������

			((CSwitchPanelView*)(GetMainFrame()->GetActiveView()))->ResetPage();

			// ��ʼ���Ѿ���ɣ������̶߳�ȡ����ͨ��������ֱ���
			m_eTransaction = eTransaction_BeginInitTiming;
			BeginTransaction(FALSE);

			if (ALIVE_KEEP && m_bInitialized)
				GetMainFrame()->SetTimer(TIMER_KEEP_ALIVE, IDLE_TIME, NULL);

			//������������
			return;

		}
		break;
	case eTransaction_RefreshSignal:
		{
			// �ź�ˢ������

			((CSwitchPanelView*)(GetMainFrame()->GetActiveView()))->RefreshComplete();

		}

		break;

	case eTransaction_BeginInitTiming:
		{
			// �˿ڷֱ��ʳ�ʼ������
		}
		break;

	case eTranscation_ResetDevice:
		{
			// �豸��λ����
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
		{// �豸��ʼ������

			GetMainFrame()->ShowWaittingDlg(TRUE, L"��ʼ��ʼ���˿���Ϣ");

			// �˿ڳ�ʼ��
			if (!GetInputManager()->RealInit())
			{
				GetMainFrame()->WaitOver(L"sorry����ʼ���˿�ʧ�ܣ����˳���ʼ����");
				Sleep(1000);
				break;
			}
			Sleep(200);

			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڶ�ȡ�˿ڷֱ���");
			Sleep(200);

			// ������ʼ��
			GetMainFrame()->ShowWaittingDlg(TRUE, L"��ʼ��ʼ������");

			// ��ȡ�����Ѵ洢�ĳ���
			GetSceneManager()->InitSceneFromDevice();

			// ��ȡ��ǰ������������
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
			if (!GetInputManager()->RefreshAllSignals())
			{
				GetMainFrame()->WaitOver( L"sorry��ˢ��ʧ�ܣ�");
				Sleep(1000);
				break;
			}
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
		{ // ��λ������

			// �����λ��
			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڸ�λ����忨");
			Sleep(100);
			GetOutputManager()->ResetAll();

			// ���븴λ��
			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڸ�λ����忨");
			Sleep(100);
			GetInputManager()->ResetAll();

			// ���ƿ���λ��
			GetMainFrame()->ShowWaittingDlg(TRUE, L"���ڸ�λ���ư忨");
			GetCmdManager()->CmdRSTF(SYSCARD_PORT, eCardTypeCtrl);
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