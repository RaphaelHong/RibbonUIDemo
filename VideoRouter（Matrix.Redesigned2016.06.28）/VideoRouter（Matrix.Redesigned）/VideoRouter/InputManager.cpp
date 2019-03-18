// InputManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <memory>
#include "VideoRouter.h"
#include "InputManager.h"
#include "Dialog components\RenameDlg.h"

using namespace std;

int CInputManager::m_sCurPort = -1;
// CInputManager

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
}

CInputManager *pInputManager = NULL;
// CInputManager ��Ա����

// ���Ҵ˶˿�ID�Ƿ���ڣ��������������е����
int CInputManager::FindID(const int nID) const
{
	int nIndex = INVALID_VALUE;
	for (int i=0; i<m_arrPort.GetCount(); i++)
	{
		if (m_arrPort[i].m_nID == nID)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

// �����Ƿ���ڴ�ͨ�ſںŵĶ˿�
int CInputManager::FindCmdPort(const int nCmdPort) const
{
	int nIndex = INVALID_VALUE;
	for (int i=0; i<m_arrPort.GetCount(); i++)
	{
		if (m_arrPort[i].m_nCmdPort == nCmdPort)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

int CInputManager::FindCmdPort(const int nCmdPort, const int nSubPort) const 
{
	int nIndex = -1;

	for (int i=0; i<m_arrPort.GetSize(); i++)
	{
		if (m_arrPort[i].m_nCmdPort == nCmdPort
			&& m_arrPort[i].m_nSubPort == nSubPort)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

// ��ȡ�˿���
int CInputManager::GetCount() const
{
	return m_arrPort.GetCount();
}

// ������ж˿�
void CInputManager::RemoveAll()
{
	m_arrPort.RemoveAll();
}

// ɾ��ָ���˿�
void CInputManager::RemoveAt(const int iKey, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort.RemoveAt(nIndex);
}

// ��Ӷ˿�
void CInputManager::Add(BASEPORT &basePort)
{
	m_arrPort.Add(basePort);
}


// ��ȡ�˿�ID
int CInputManager::GetID(const int nIndex) const
{
	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return INVALID_VALUE;

	return m_arrPort[nIndex].m_nID;
}

void CInputManager::SetID(const int nIndex, const int nID)
{
	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_nID = nID;
}

// ���ö˿�����
void CInputManager::SetName(const int iKey, char* szName, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	//int nLen = strlen(szName);

	CString str;
	str.Format(_T("%s"), szName);

	memset(m_arrPort[nIndex].m_szName, 0, _MAX_SIGNAL_NAME);
	strncpy_s(m_arrPort[nIndex].m_szName, _MAX_SIGNAL_NAME, str.GetBuffer(), str.GetLength());
}

// ��ȡ�˿�����
char* CInputManager::GetName(const int iKey, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return NULL;

	return (char*)m_arrPort[nIndex].m_szName;
}

void CInputManager::Rename(const int nIndex, const CPoint ptDlgPos)
{
	if (nIndex < 0 || nIndex >= m_arrPort.GetSize())
		return;

	CRenameDlg DLG(GetName(nIndex, FALSE), NULL);
	DLG.m_ptPopup = ptDlgPos;
	if (DLG.DoModal() != IDOK)
		return;

	CString strNewname = DLG.m_strOrgName;
	SetName(nIndex, strNewname.GetBuffer(), FALSE);
}

// ���ö˿�����
void CInputManager::SetSignalType(const int iKey, emVideoType eVT, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_eVT = eVT;
}

// ���ö˿�����
emVideoType CInputManager::GetSignalType(const int iKey, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return vtUndef;

	return m_arrPort[nIndex].m_eVT;
}

// ���ö˿ڷֱ���
void CInputManager::SetTiming(const int iKey, TIMING &st, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_timing = st;

	// �����������źŵķֱ���
	if (m_arrPort[nIndex].m_nSubPort == 1)
	{
		m_arrPort[nIndex+1].m_timing = st;
	}
	else if (m_arrPort[nIndex].m_nSubPort == 2)
	{
		m_arrPort[nIndex-1].m_timing = st;
	}

}

// ��ȡ�˿ڷֱ���
TIMING CInputManager::GetTiming(const int iKey, BOOL byID/* =TRUE */) const
{
	TIMING st = {0};
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return st;

	return st = m_arrPort[nIndex].m_timing;
}

// ���ö˿�ͨ�ź�
void CInputManager::SetCmdPort(const int iKey, const int nPort, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_nCmdPort = nPort;
}

// ��ȡ�˿�ͨ�ź�
int CInputManager::GetCmdPort(const int iKey, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return INVALID_VALUE;

	return m_arrPort[nIndex].m_nCmdPort;
}

void CInputManager::SetSubID(const int index, const int iSubID)
{
	if (index < 0 || index >= m_arrPort.GetSize())
		return;

	m_arrPort[index].m_nSubPort = iSubID;
}

int CInputManager::GetSubID(const int iKey, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return INVALID_VALUE;

	return m_arrPort[nIndex].m_nSubPort;
}

void CInputManager::SetCurrencyResolution(const int iKey, const int iHactive, const int iVactive, const int iHz, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_nCurHAct = iHactive;
	m_arrPort[nIndex].m_nCurVAct = iVactive;
	m_arrPort[nIndex].m_nCurHz   = iHz;

}

BOOL CInputManager::GetCurrencyResolution(const int iKey, int &iHactive, int &iVactive, int &iHz, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return FALSE;

	iHactive = m_arrPort[nIndex].m_nCurHAct;
	iVactive = m_arrPort[nIndex].m_nCurVAct;
	iHz      = m_arrPort[nIndex].m_nCurHz;

	return TRUE;
}

int CInputManager::GetRealIndex(const int nID) const
{
	int nIndex = -1;
	for (int i=0; i<m_arrPort.GetSize(); i++)
	{
		nIndex++;

		if (m_arrPort[i].m_nSubPort == 2)
			nIndex --;

		if (m_arrPort[i].m_nID == nID)
		{
			break;
		}
	}

	return nIndex;
}

int CInputManager::GetRealCount() const
{
	int nSize = 0;

	for (int i=0; i<m_arrPort.GetSize(); i++)
	{
		nSize++;

		if (m_arrPort[i].m_nSubPort == 2)
			nSize--;
	}

	return nSize;
}

int CInputManager::LogicIndex2RealIndex(const int nLogicIndex) const
{
	if (nLogicIndex < 0 || nLogicIndex >= m_arrPort.GetSize())
		return -1;

	int nReal = -1;
	for (int nIndex=0; nIndex<m_arrPort.GetSize(); nIndex++)
	{
		if (m_arrPort[nIndex].m_nSubPort < 2) // 0:��ʾ�����źţ�1:��ʾ�����ź�1��Ҳ������źţ��������źŵ�ID > 1
			nReal++;

		if (nIndex == nLogicIndex)
			break;
	}

	return nReal;
}

int CInputManager::RealIndex2LogicIndex(const int nRealIndex) const
{
	if (nRealIndex < 0 || nRealIndex >= m_arrPort.GetSize())
		return -1;

	int nLogic = -1;
	int nReal = -1;
	for (int nIndex=0; nIndex<m_arrPort.GetSize(); nIndex++)
	{
		if (m_arrPort[nIndex].m_nSubPort < 2) // 0:��ʾ�����źţ�1:��ʾ�����ź�1��Ҳ������źţ��������źŵ�ID > 1
			nReal++;

		if (nReal == nRealIndex) {
			nLogic = nIndex;
			break;
		}
	}

	return nLogic;
}

BOOL CInputManager::UpdateEDID(const int iKey, BOOL byID) const
{
	CFileDialog OpenFile(TRUE, _T(".dat"), _T("edid"), OFN_HIDEREADONLY|OFN_READONLY, "dat File(*.dat)|*.dat|All Files (*.*)|*.*|");
	if (OpenFile.DoModal() == IDOK)
	{
		CString strFilePath = OpenFile.GetPathName();
		CStdioFile EdidFile;
		if (!EdidFile.Open(strFilePath, CFile::modeRead|CFile::modeNoTruncate|CFile::typeBinary))
		{
			AfxMessageBox(_T("��Edid�ļ�ʧ�ܣ�"),  MB_OK|MB_ICONERROR);
			return FALSE;
		}

		//�ϴ�EDID���豸
		long nFileSize = (long)EdidFile.GetLength();
		if (nFileSize < 128 || nFileSize > 256)
		{
			AfxMessageBox(_T("�ļ������쳣��ӦΪ128��256���ֽڣ�"), MB_ICONERROR|MB_OK);
			EdidFile.Close();
			return FALSE;
		}

		int nDataLen = 0;
		if (nFileSize > 128)
			nDataLen = 256;
		else if (nFileSize > 0 && nFileSize > 256)
			nDataLen = 128;

		CMemoryAllocate <char> Allocate_char(nFileSize+1);
		EdidFile.Read(Allocate_char.GetAddress(), nDataLen);
		EdidFile.Close();

		CMemoryAllocate <UINT> Allocate_UINT(nDataLen);
		for (int i=0; i<nDataLen; i++)
			Allocate_UINT.GetAddress()[i] = (UINT)Allocate_char.GetAddress()[i];

		if (GetCmdManager()->CmdWEDI(GetInputManager()->GetCmdPort(iKey, byID), eCardTypeInput, Allocate_UINT.GetAddress(), nDataLen) == CMD_FAILURE)
		{
			AfxMessageBox(_T("����ʧ�ܣ�"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
		else
		{
			AfxMessageBox(_T("���³ɹ���"), MB_OK|MB_ICONINFORMATION);
			return TRUE;
		}
	}

	return FALSE;
}


void CInputManager::DemoInit()
{
	const int iDemoInputs = 32;//_MAX_INPUT_PORT;

	emVideoType vt[_MAX_VIDEO_TYPE+1];
	int k=0;
	vt[k++] = vtCVBS;
	vt[k++] = vtYpbpr;
	vt[k++] = vtVGA;
	vt[k++] = vtSDI;
	vt[k++] = vtHDMI;
	vt[k++] = vtDVI;
	vt[k++] = vtDP;
	vt[k++] = vt4K_HDMI;
	vt[k++] = vtHDBASET;
	vt[k++] = vtDLDVI;
	vt[k++] = vtPAL_D;
	vt[k++] = vtNTSC_D;
	vt[k++] = vtPAL_W;
	vt[k++] = vtNTSC_W;
	vt[k++] = vtUndef;

	INPORT sInput;
	CString strName;
	int nID = 0;
	for (int i=0; i<iDemoInputs; i++) {
	
		// ID
		sInput.m_nID = nID++;

		// Video type
		sInput.m_eVT = vt[i%(_MAX_VIDEO_TYPE +1)];//vtUndef;

		// Command port
		sInput.m_nCmdPort = i+1;

		// Timing
		sInput.m_timing = OriginalResolutions[i%_MAX_ORG_RESOLUTION];
		sInput.m_nCurHAct = sInput.m_timing.m_HActive;
		sInput.m_nCurVAct = sInput.m_timing.m_VActive;
		sInput.m_nCurHz   = sInput.m_timing.m_Hz;

		// Sub port
		if (i%5 == 0)
		{
			sInput.m_nSubPort = 1;
			strName.Format(_T("�ź�Դ%d"), i+1);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strName.GetBuffer(), strName.GetLength());

			m_arrPort.Add(sInput);

			sInput.m_nID = nID++;
			sInput.m_nSubPort = 2;

			m_arrPort.Add(sInput);
		}
		else
		{
			sInput.m_nSubPort = 0;  // �����ź� һ��Ҫ����ֵ���㣡��
			// Name
			strName.Format(_T("�ź�Դ%d"), i+1);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strName.GetBuffer(), strName.GetLength());

			m_arrPort.Add(sInput);
		}		
	}

	m_sCurPort = GetCmdPort(0, FALSE);
}

BOOL CInputManager::RealInit()
{
	if (!InitPorts())
		return FALSE;

	if (!InitSignals())
		return FALSE;

	return TRUE;
}

BOOL CInputManager::InitPorts()
{
	UINT nInputs  = 0;
	UINT nOutputs = 0;
	UINT InputFlags[_MAX_INPUT_PORT]   = {0};
	UINT OutputFlags[_MAX_OUTPUT_PORT] = {0};
	UINT InputPorts[_MAX_INPUT_PORT]   = {0};
	UINT OutputPorts[_MAX_OUTPUT_PORT] = {0};

	int nRet = GetCmdManager()->CmdGCMG(SYSCARD_PORT, eCardTypeCtrl, nInputs, nOutputs, InputFlags, OutputFlags);
	if ( nRet == CMD_FAILURE )
	{
		AfxMessageBox(_T("����˿ڳ�ʼ��ʧ�ܣ��޷���ȡ�˿�����"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	int iInNum = 0;
	for (int i=0; i<nInputs; i++)
	{
		if (InputFlags[i] == 1)         //��ʾ������ͨ����û���Ӷ˿�
			InputPorts[iInNum++] = i+1; //�����߼�ͨ�Ŷ˿ںţ���0��ʼ
		else if (InputFlags[i] == 2)    //��ʾ������ͨ�������Ӷ˿�
			InputPorts[iInNum++] = (i+1) | 0xFF0000; //�ø��ֽڱ�ʾ���Ӷ˿�
	}

	int iOutNum = 0;
	for (int i=0; i<nOutputs; i++)
	{
		if (OutputFlags[i] == 1)          //��ʾ�����ͨ����û���Ӷ˿�
			OutputPorts[iOutNum++] = i+1; //����߼�ͨ�Ŷ˿ںţ���0��ʼ
		else if (OutputFlags[i] == 2)     //��ʾ�����ͨ�������Ӷ˿�
			OutputPorts[iOutNum++] = (i+1) | 0xFF0000;
	}

	DEVICEINFO devInfo;
	devInfo.m_nMaxIn  = nInputs;
	devInfo.m_nMaxOut = nOutputs;
	GetGlobalManager()->InitDeviceStructure(&devInfo);

	INPORT sInput;
	CString strSrc;
	RemoveAll();
	int nID = 0;
	for (int i=0; i<iInNum; i++)
	{	
		sInput.m_eVT = vtDVI;

		memset(&sInput.m_timing, 0, sizeof(TIMING));
		sInput.m_timing.m_Type = UNKNOW_RESOULUTION;

		if ((InputPorts[i] & 0xFF0000) == 0xFF0000)
		{
			// ���ź�1
			sInput.m_nID = nID++;
			sInput.m_nSubPort = 1;
			sInput.m_nCmdPort  = (InputPorts[i] & 0xFFFF);

			strSrc.Format(_T("�ź�%d"), InputPorts[i] & 0xFFFF);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strSrc.GetBuffer(), strSrc.GetLength());

			Add(sInput);

			// ���ź�2
			sInput.m_nID = nID++;
			sInput.m_nSubPort = 2;

			strSrc.Format(_T("�ź�%d"), InputPorts[i] & 0xFFFF);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strSrc.GetBuffer(), strSrc.GetLength());

			Add(sInput);
			
		}
		else
		{
			sInput.m_nID = nID++;
			sInput.m_nSubPort = 0; // �����ź� һ��Ҫ����ֵ���㣡��
			sInput.m_nCmdPort  = InputPorts[i];

			strSrc.Format(_T("�ź�%d"), InputPorts[i] & 0xFFFF);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strSrc.GetBuffer(), strSrc.GetLength());

			Add(sInput);
		}		
	}

	// Ĭ�ϵ���һ���ź�Դ
	m_sCurPort = GetCmdPort(0, FALSE);

	CString strName;
	OUTPORT sOut;
	nID = 0;
	GetOutputManager()->RemoveAll();

	for (int i=0; i<iOutNum; i++)
	{
		sOut.m_nCmdPort = OutputPorts[i];
		sOut.m_nID = i;
		sOut.m_nSubPort = 0; // �����ź� һ��Ҫ����ֵ���㣡��
		sOut.m_nFeederID = -1; //i%nTotalInput;
		sOut.m_eVT = vtDVI;

		strName.Format(_T("���%d"), OutputPorts[i] & 0xFFFF);
		strncpy_s(sOut.m_szName, _MAX_SIGNAL_NAME, strName.GetBuffer(), strName.GetLength());

		memset(&sOut.m_timing, 0, sizeof(TIMING));
		sOut.m_timing.m_Type = UNKNOW_RESOULUTION;

		GetOutputManager()->Add(sOut);
	}

	return TRUE;
}

BOOL CInputManager::InitSignals()
{
	int nCount = m_arrPort.GetCount();
	int nVideoType = vtUndef;
	TIMING timing={0};

	for (int i=0; i<nCount; i++)
	{
		if (GetCmdManager()->CmdRPM3(GetCmdPort(i, FALSE), eCardTypeInput, nVideoType) != CMD_FAILURE)
			SetSignalType(i, emVideoType(nVideoType), FALSE);
		else
			SetSignalType(i, vtUndef, FALSE);

		if (GetCmdManager()->CmdGETR(GetCmdPort(i, FALSE), eCardTypeInput, timing) != CMD_FAILURE)
			SetCurrencyResolution(i, timing.m_HActive, timing.m_VActive, timing.m_Hz, FALSE);
		else
			SetCurrencyResolution(i, 0, 0, 0, FALSE);
	}

	return TRUE;
}

BOOL CInputManager::RefreshAllSignals()
{
	InitSignals();

	return TRUE;
}

BOOL CInputManager::RefreshThisSignal(const int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrPort.GetSize())
		return FALSE;

	if (!CCommunicateManager::IsConnected())
		return TRUE;

	// ��ǰ�����ź�����
	int nVideoType = vtUndef;
	if (GetCmdManager()->CmdRPM3(GetCmdPort(nIndex, FALSE), eCardTypeInput, nVideoType) != CMD_FAILURE)
		SetSignalType(nIndex, emVideoType(nVideoType), FALSE);
	else
		SetSignalType(nIndex, vtUndef, FALSE);

	// ��ǰ�����źŷֱ���
	TIMING timing = {0};
	if (GetCmdManager()->CmdGETR(GetCmdPort(nIndex, FALSE), eCardTypeInput, timing) != CMD_FAILURE)
		SetCurrencyResolution(nIndex, timing.m_HActive, timing.m_VActive, timing.m_Hz, FALSE);
	else
		SetCurrencyResolution(nIndex, 0, 0, 0, FALSE);

	return TRUE;
}

BOOL CInputManager::Reset(const int iKey, BOOL byID/*=TRUE*/)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return FALSE;

	return GetCmdManager()->CmdRSTF(m_arrPort[nIndex].m_nCmdPort, eCardTypeInput);
}

BOOL CInputManager::ResetAll()
{
	for (int i=0; i<m_arrPort.GetSize(); i++)
	{
		if (m_arrPort[i].m_nSubPort > 1)
			continue; // skip all sub ports

		GetCmdManager()->CmdRSTF(m_arrPort[i].m_nCmdPort, eCardTypeInput);
	}

	return TRUE;
}