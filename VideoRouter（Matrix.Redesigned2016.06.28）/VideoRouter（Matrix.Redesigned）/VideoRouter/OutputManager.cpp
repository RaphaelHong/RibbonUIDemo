// OutputManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "OutputManager.h"

int COutputManager::m_sCurPort = -1;
// COutputManager

COutputManager::COutputManager()
{
}

COutputManager::~COutputManager()
{
}

COutputManager *pOutputManager = NULL;
// COutputManager ��Ա����

// ������Դ�ź�ID
void COutputManager::SetFeederID(const int iKey, const int iFeederID, BOOL bInvs, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	if (bInvs && (m_arrPort[nIndex].m_nFeederID == iFeederID)) {
		m_arrPort[nIndex].m_nFeederID = -1;
	}
	else {
		m_arrPort[nIndex].m_nFeederID = iFeederID;
	}

}

// ������ͬ�����ź�
void COutputManager::SetSameFeederID(const int &iFeederID, BOOL bInvs)
{
	for (int i=0; i<m_arrPort.GetCount(); i++)
	{
		if (bInvs && (m_arrPort[i].m_nFeederID == iFeederID))
		{
			m_arrPort[i].m_nFeederID = -1;
		}
		else
		{
			m_arrPort[i].m_nFeederID = iFeederID;
		}
	}
}

// ��ȡĳ������˵��ź���ԴID
int COutputManager::GetFeederID(const int iKey, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return -1;

	return m_arrPort[nIndex].m_nFeederID;
}

// �����л�
void COutputManager::P2PSwitch(const int iKey, const int &iFeederID, BOOL bInvs, BOOL byID/* =TRUE */)
{
	// ��ȡ����˿ں�
	int iOutChlPort = GetCmdPort(iKey, byID);
	if (iOutChlPort < 0)
		return;

	// ��ȡ����˿ں�
	int iFeederPort = GetInputManager()->GetCmdPort(iFeederID);
	int iSubPort    = GetInputManager()->GetSubID(iFeederID);

	if ((GetFeederID(iKey, byID) == iFeederID) && bInvs)
	{
		iFeederPort = _OFF;	
		iSubPort    = 0;
	}

	// �����л�����
	int nRet = GetCmdManager()->CmdSWCH(SYSCARD_PORT, eCardTypeCtrl, iFeederPort, iSubPort, iOutChlPort);

#ifndef _DEBUG
	if (nRet == CMD_FAILURE)
		return (void)AfxMessageBox(_T("�����л�ʧ�ܣ�"), MB_OK|MB_ICONERROR);
#endif

	//����״̬
	SetFeederID(iKey, iFeederID, bInvs, byID);	
}

// һ�������л�
void COutputManager::AllSwitch(const int &iFeederID, BOOL bInvs)
{
	// ����˿ں�
	int iFeederPort = GetInputManager()->GetCmdPort(iFeederID);
	int iSubPort    = GetInputManager()->GetSubID(iFeederID);
	if (iFeederID == INVALID_VALUE)
	{
		iFeederPort = _OFF;
		iSubPort    = 0;
	}

	// �л�����
	int nRet = GetCmdManager()->CmdSWCH(SYSCARD_PORT, eCardTypeCtrl, iFeederPort, iSubPort);

#ifndef _DEBUG
	if (nRet == CMD_FAILURE)
		return (void)AfxMessageBox(_T("�����л�ʧ�ܣ�"), MB_OK|MB_ICONERROR);
#endif

	//����״̬
	SetSameFeederID(iFeederID, bInvs);
}

// һ�Զ��л�
void COutputManager::MultiSwitch(const int &iFeederID, int *pOutputIDList, int iCount)
{
	if (iCount <= 0)
		return;

	// ����˿ں�
	int iFeederPort = GetInputManager()->GetCmdPort(iFeederID);
	int iSubPort    = GetInputManager()->GetSubID(iFeederID);
	if (iFeederID == INVALID_VALUE)
	{
		iFeederPort = _OFF;
		iSubPort    = 0;
	}

	// ����˿ں�����
	int* pOutChlPort = new int[iCount];
	memset(pOutChlPort, 0, sizeof(int)*iCount);
	for (int i=0; i<iCount; i++)
	{
		pOutChlPort[i] = GetCmdPort(pOutputIDList[i]);
	}

	int nRet = GetCmdManager()->CmdSWCH(SYSCARD_PORT, eCardTypeCtrl, iFeederPort, iSubPort, pOutChlPort, iCount);

#ifndef _DEBUG
	if (nRet == CMD_FAILURE)
		return (void)AfxMessageBox(_T("�����л�ʧ�ܣ�"), MB_OK|MB_ICONERROR);
#endif

	// ����״̬
	for (int i=0; i<iCount; i++)
	{
		SetFeederID(pOutputIDList[i], iFeederID, FALSE);
	}
}

void COutputManager::MappingSwitch(const int &iSize, const INT_PAIR *pPair)
{
	int iMax = max (iSize, GetGlobalManager()->GetDeviceStructure()->m_nMaxOut);
	CMemoryAllocate<int> FeederArr(iMax);
	ZeroMemory(FeederArr.GetAddress(), iMax*sizeof(int));
	//memset(FeederArr.GetAddress(), 0, sizeof(int) * iMax);

	for (int i=0; i<iSize; i++)
	{
		// avoid memory leakage
		if (pPair[i].key-1 >= iSize)
			continue;

		FeederArr.GetAddress()[pPair[i].key-1] = pPair[i].val;
	}

	int nRet = GetCmdManager()->CmdSWCH(SYSCARD_PORT, eCardTypeCtrl, iMax, FeederArr.GetAddress());

#ifndef _DEBUG
	if (nRet == CMD_FAILURE)
		return (void)AfxMessageBox(_T("�����л�ʧ�ܣ�"), MB_OK|MB_ICONERROR);
#endif

	// ����״̬
	int nIndex1 = -1;
	int nIndex2 = -1;
	int nFeederPort = -1;
	int nSubPort = -1;
	for (int i=0; i<iMax; i++)
	{
		nFeederPort = FeederArr.GetAddress()[i] & 0xFFFF;
		nSubPort    = FeederArr.GetAddress()[i] & 0xFFFF0000;
		if      (nSubPort == SUBPORT_A) nSubPort = 1;
		else if (nSubPort == SUBPORT_B) nSubPort = 2;
		else nSubPort = 0;

		nIndex1 = GetInputManager()->FindCmdPort(nFeederPort, nSubPort);
		nIndex2 = i;

		SetFeederID(nIndex2, GetInputManager()->GetID(nIndex1), FALSE, TRUE);
	}
}


void COutputManager::DemoInit()
{
	const int iDemoOutputs = 48;//_MAX_OUTPUT_PORT;

	CString strName;
	OUTPORT sOut;

	RemoveAll();

	for (int i=0; i<iDemoOutputs; i++) {
		sOut.m_nCmdPort = i+1;
		sOut.m_nID = i;
		sOut.m_nSubPort = 0; // �����ź� һ��Ҫ����ֵ���㣡��
		sOut.m_nFeederID = i%GetInputManager()->GetCount();
		sOut.m_timing = OriginalResolutions[i%_MAX_ORG_RESOLUTION];
		sOut.m_nCurHAct = sOut.m_timing.m_HActive;
		sOut.m_nCurVAct = sOut.m_timing.m_VActive;
		sOut.m_nCurHz   = sOut.m_timing.m_Hz;
		strName.Format(_T("���%d"), i+1);
		strncpy_s(sOut.m_szName, _MAX_SIGNAL_NAME, strName.GetBuffer(), strName.GetLength());
		m_arrPort.Add(sOut);
	}
}

BOOL COutputManager::Reset(const int iKey, BOOL byID/*=TRUE*/)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return FALSE;

	return GetCmdManager()->CmdRSTF(m_arrPort[nIndex].m_nCmdPort, eCardTypeOutput);
}

BOOL COutputManager::ResetAll()
{
	for (int i=0; i<m_arrPort.GetSize(); i++)
	{
		if (m_arrPort[i].m_nSubPort > 1)
			continue; // skip all sub ports

		GetCmdManager()->CmdRSTF(m_arrPort[i].m_nCmdPort, eCardTypeOutput);
	}

	return TRUE;
}