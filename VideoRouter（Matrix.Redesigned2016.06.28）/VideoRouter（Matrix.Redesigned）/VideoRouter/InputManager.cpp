// InputManager.cpp : 实现文件
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
// CInputManager 成员函数

// 查找此端口ID是否存在，返回其在数组中的序号
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

// 查找是否存在此通信口号的端口
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

// 获取端口数
int CInputManager::GetCount() const
{
	return m_arrPort.GetCount();
}

// 清除所有端口
void CInputManager::RemoveAll()
{
	m_arrPort.RemoveAll();
}

// 删除指定端口
void CInputManager::RemoveAt(const int iKey, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort.RemoveAt(nIndex);
}

// 添加端口
void CInputManager::Add(BASEPORT &basePort)
{
	m_arrPort.Add(basePort);
}


// 获取端口ID
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

// 设置端口名称
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

// 获取端口名称
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

// 设置端口类型
void CInputManager::SetSignalType(const int iKey, emVideoType eVT, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_eVT = eVT;
}

// 设置端口类型
emVideoType CInputManager::GetSignalType(const int iKey, BOOL byID/* =TRUE */) const
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return vtUndef;

	return m_arrPort[nIndex].m_eVT;
}

// 设置端口分辨率
void CInputManager::SetTiming(const int iKey, TIMING &st, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_timing = st;

	// 更新所有子信号的分辨率
	if (m_arrPort[nIndex].m_nSubPort == 1)
	{
		m_arrPort[nIndex+1].m_timing = st;
	}
	else if (m_arrPort[nIndex].m_nSubPort == 2)
	{
		m_arrPort[nIndex-1].m_timing = st;
	}

}

// 获取端口分辨率
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

// 设置端口通信号
void CInputManager::SetCmdPort(const int iKey, const int nPort, BOOL byID/* =TRUE */)
{
	int nIndex = iKey;

	if (byID)
		nIndex = FindID(iKey);

	if (nIndex >= m_arrPort.GetCount() || nIndex < 0)
		return;

	m_arrPort[nIndex].m_nCmdPort = nPort;
}

// 获取端口通信号
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
		if (m_arrPort[nIndex].m_nSubPort < 2) // 0:表示无子信号，1:表示主子信号1，也代表该信号，其它子信号的ID > 1
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
		if (m_arrPort[nIndex].m_nSubPort < 2) // 0:表示无子信号，1:表示主子信号1，也代表该信号，其它子信号的ID > 1
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
			AfxMessageBox(_T("打开Edid文件失败！"),  MB_OK|MB_ICONERROR);
			return FALSE;
		}

		//上传EDID到设备
		long nFileSize = (long)EdidFile.GetLength();
		if (nFileSize < 128 || nFileSize > 256)
		{
			AfxMessageBox(_T("文件长度异常，应为128或256个字节！"), MB_ICONERROR|MB_OK);
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
			AfxMessageBox(_T("更新失败！"), MB_OK|MB_ICONERROR);
			return FALSE;
		}
		else
		{
			AfxMessageBox(_T("更新成功！"), MB_OK|MB_ICONINFORMATION);
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
			strName.Format(_T("信号源%d"), i+1);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strName.GetBuffer(), strName.GetLength());

			m_arrPort.Add(sInput);

			sInput.m_nID = nID++;
			sInput.m_nSubPort = 2;

			m_arrPort.Add(sInput);
		}
		else
		{
			sInput.m_nSubPort = 0;  // 无子信号 一定要将此值置零！！
			// Name
			strName.Format(_T("信号源%d"), i+1);
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
		AfxMessageBox(_T("输入端口初始化失败！无法获取端口数！"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	int iInNum = 0;
	for (int i=0; i<nInputs; i++)
	{
		if (InputFlags[i] == 1)         //表示有输入通道，没有子端口
			InputPorts[iInNum++] = i+1; //输入逻辑通信端口号，从0开始
		else if (InputFlags[i] == 2)    //表示有输入通道，有子端口
			InputPorts[iInNum++] = (i+1) | 0xFF0000; //用高字节表示有子端口
	}

	int iOutNum = 0;
	for (int i=0; i<nOutputs; i++)
	{
		if (OutputFlags[i] == 1)          //表示有输出通道，没有子端口
			OutputPorts[iOutNum++] = i+1; //输出逻辑通信端口号，从0开始
		else if (OutputFlags[i] == 2)     //表示有输出通道，有子端口
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
			// 子信号1
			sInput.m_nID = nID++;
			sInput.m_nSubPort = 1;
			sInput.m_nCmdPort  = (InputPorts[i] & 0xFFFF);

			strSrc.Format(_T("信号%d"), InputPorts[i] & 0xFFFF);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strSrc.GetBuffer(), strSrc.GetLength());

			Add(sInput);

			// 子信号2
			sInput.m_nID = nID++;
			sInput.m_nSubPort = 2;

			strSrc.Format(_T("信号%d"), InputPorts[i] & 0xFFFF);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strSrc.GetBuffer(), strSrc.GetLength());

			Add(sInput);
			
		}
		else
		{
			sInput.m_nID = nID++;
			sInput.m_nSubPort = 0; // 无子信号 一定要将此值置零！！
			sInput.m_nCmdPort  = InputPorts[i];

			strSrc.Format(_T("信号%d"), InputPorts[i] & 0xFFFF);
			strncpy_s(sInput.m_szName, _MAX_SIGNAL_NAME, strSrc.GetBuffer(), strSrc.GetLength());

			Add(sInput);
		}		
	}

	// 默认到第一个信号源
	m_sCurPort = GetCmdPort(0, FALSE);

	CString strName;
	OUTPORT sOut;
	nID = 0;
	GetOutputManager()->RemoveAll();

	for (int i=0; i<iOutNum; i++)
	{
		sOut.m_nCmdPort = OutputPorts[i];
		sOut.m_nID = i;
		sOut.m_nSubPort = 0; // 无子信号 一定要将此值置零！！
		sOut.m_nFeederID = -1; //i%nTotalInput;
		sOut.m_eVT = vtDVI;

		strName.Format(_T("输出%d"), OutputPorts[i] & 0xFFFF);
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

	// 当前输入信号类型
	int nVideoType = vtUndef;
	if (GetCmdManager()->CmdRPM3(GetCmdPort(nIndex, FALSE), eCardTypeInput, nVideoType) != CMD_FAILURE)
		SetSignalType(nIndex, emVideoType(nVideoType), FALSE);
	else
		SetSignalType(nIndex, vtUndef, FALSE);

	// 当前输入信号分辨率
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