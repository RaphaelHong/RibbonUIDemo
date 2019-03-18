// CommunicateManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "CommunicateManager.h"
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

///////////////////////////////////////////////////////////////////////////////////
// ���������ڻ�ȡ��������������������ַ�����
//

IPMAC host[254];     //254��IP��ַ��Ϣ 
int   host_k;           //��ЧIP����
char *pLocalIP; //����IP��ַ
CRITICAL_SECTION  g_cs;//�ٽ���Ʊ���

DWORD WINAPI ArpThread(LPVOID  lParam) ; 

//��ȡ����IP��ַ��MAC��ַ��Ϣ
char* GetLocalHostAddr()
{
	char szHostname[256] = {0};

	//��ȡ������
	if (gethostname(szHostname, 256) != 0)
	{
		return NULL;
	}

	//��ȡ������Ϣ
	hostent *pHost = gethostbyname(szHostname);
	if (pHost == NULL)
	{
		return NULL;
	}

	//��ȡ����IP
	in_addr addr;
	char *p = pHost->h_addr_list[0];
	memcpy(&addr.S_un.S_addr, p, pHost->h_length);
	char *szIP = inet_ntoa(addr);

	return szIP;
}


/*�̺߳�����������ѯIP��Ӧ��MAC��ַ*/  
DWORD WINAPI ArpThread(LPVOID  lParam)  
{  
	char *szIp=(char *)lParam;  
	ULONG pMac[2];  
	ULONG pulen=6;

	int ret;    
	if ((ret=SendARP(inet_addr(szIp), inet_addr(pLocalIP), pMac, &pulen))==0)  
	{  
		EnterCriticalSection(&g_cs);  //���߳�ͬ�����Ǻ�:0  

		strcpy_s(host[host_k].szIp, 16, szIp);  
		PBYTE  pbyte = (PBYTE)pMac;  
		for (int i=0; i<5; i++)  
		{	host[host_k].szMac[i] = pbyte[i];   }

		host[host_k].szMac[5] = pbyte[5];  
		host_k++; 

		LeaveCriticalSection(&g_cs);  
	} 

	in_addr addr;
	addr.S_un.S_addr = inet_addr(szIp);

	return  addr.S_un.S_un_b.s_b4;  
} 

void CCommunicateManager::GetDomainHosts(pIPMAC hosts, int &iHostNum)
{
	char *pHostAddr = GetLocalHostAddr();
	if (pHostAddr == NULL)
	{
		AfxMessageBox(_T("���󣬻�ȡ������ַ��Ϣʧ�ܣ�"),  MB_OK|MB_ICONWARNING);
		return;
	}

	//ΪARP�̸߳�����IP��ַ
	pLocalIP = pHostAddr;

	CString str;
	str.Format(_T("%s"), pHostAddr);
	int nTok = str.ReverseFind(_T('.'));
	CString  IpSuffix = str.Left(nTok) + _T(".");

	HANDLE  hthread[254];              // �߳̾�� "192.168.1.1" - "192.168.1.254"
	CString  strIp[254];               // 254����ͬ��IP��ַ
	InitializeCriticalSection(&g_cs);  //��ʼ���ٽ���
	host_k = 0; //��ʼ��IP����
	for  (int  i=0;i<254;i++)  
	{  
		//����ARP�߳�
		strIp[i].Format("%d",i+1);  
		strIp[i]=IpSuffix+strIp[i];  
		hthread[i] = CreateThread(NULL,0, ArpThread,strIp[i].GetBuffer(0),0,NULL);    
	}
	/*�Ǻǣ���Ϊһ��ֻ�ܵȴ�  64���ں˶�������ֻ�зּ�����*/  
	/*��ȻҲ������ѭ����*/  
	WaitForMultipleObjects(64,hthread,TRUE,INFINITE);  
	WaitForMultipleObjects(64,&hthread[64],TRUE,INFINITE);  
	WaitForMultipleObjects(64,&hthread[128],TRUE,INFINITE);  
	WaitForMultipleObjects(61,&hthread[192],TRUE,INFINITE);  
	DeleteCriticalSection(&g_cs); 

	char rbuf[256] = {0};
	//�޳��ظ�IP����
	for (int i=0; i<host_k-1; i++)	{
		for (int j=i+1; j<host_k; j++)	{
			if (host[i].szIp == host[j].szIp)
			{
				//�����j��Ԫ�����i��Ԫ���ظ������������һ��Ԫ�ظ��ǵ�j��Ԫ�أ����������һ
				host[j] = host[host_k-1];
				host_k--;
			}
		}
	}

	//�������Ӻ󷵻ص��ַ����ж��Ƿ��Ǳ���˾���豸
	iHostNum = 0;

	for (int i=0; i<host_k; i++)
	{
		if (strstr(host[i].szIp, pHostAddr) != NULL)
			continue;

		m_Sock.m_ipaddr.Format(_T("%s"), host[i].szIp);
		if (m_Sock.OpenSocket())
		{
			recv(m_Sock.m_sockClient, rbuf, 256, 0);
			if (strstr(rbuf, "www.mviewtech.com") != NULL)
			{
				//������IP��ַ���뷵���б���
				hosts[iHostNum++] = host[i];
			}

			m_Sock.CloseSocket();
		}
	}
}
//---------------------------------------------------------------


int CCommunicateManager::m_sConnType = NON_CONNECT;
CCommunicateManager* pCmdManager = NULL;

// CCommunicateManager

CCommunicateManager::CCommunicateManager()
{
}

CCommunicateManager::~CCommunicateManager()
{
}


// CCommunicateManager ��Ա����

BOOL CCommunicateManager::IsConnected()
{
	return (m_sConnType != NON_CONNECT);
}

// ͳ�Ƽ���������д��ں�
void CCommunicateManager::GetAllCOMPort(CString *pPortList)
{
	long   lReg; 
	HKEY   hKey; 
	DWORD   MaxValueLength; 
	DWORD   dwValueNumber; 

	lReg=RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
		"HARDWARE\\DEVICEMAP\\SERIALCOMM", 
		0, 
		KEY_QUERY_VALUE, 
		&hKey);

	if(lReg!=ERROR_SUCCESS) //�ɹ�ʱ����ERROR_SUCCESS��
		return;

	lReg=RegQueryInfoKeyA(hKey, 
		NULL, 
		NULL, 
		NULL, 
		NULL,
		NULL, 
		NULL, 
		&dwValueNumber, //���غ�hKey������ֵ
		&MaxValueLength, 
		NULL, 
		NULL, 
		NULL); 

	if(lReg!=ERROR_SUCCESS) //û�гɹ�
		return; 

	LPSTR   pValueName,pCOMNumber; 
	DWORD   cchValueName,dwValueSize=6; 

	for(int i=0; i < (int)dwValueNumber; i++) 
	{ 
		cchValueName=MaxValueLength+1; 
		dwValueSize=6; 
		pValueName=(LPSTR)VirtualAlloc(NULL,cchValueName,MEM_COMMIT,PAGE_READWRITE); 
		lReg=RegEnumValueA(hKey, 
			i, 
			pValueName, 
			&cchValueName, 
			NULL, 
			NULL, 
			NULL, 
			NULL); 

		if((lReg!=ERROR_SUCCESS)&&(lReg!=ERROR_NO_MORE_ITEMS)) 
		{ 
			CString slReg;
			slReg.Format(_T("%d"),lReg);

			return;
		} 

		pCOMNumber=(LPSTR)VirtualAlloc(NULL,6,MEM_COMMIT,PAGE_READWRITE); 
		lReg=RegQueryValueExA(hKey, 
			pValueName, 
			NULL, 
			NULL, 
			(LPBYTE)pCOMNumber, 
			&dwValueSize); 

		if(lReg!=ERROR_SUCCESS)
			return;

		CString str2(pCOMNumber);//,str3;//str1(pPortList),
		*pPortList += _T(",") + str2;                     

		VirtualFree(pValueName,0,MEM_RELEASE); 
		VirtualFree(pCOMNumber,0,MEM_RELEASE); 
	} 
}

// ��������
BOOL CCommunicateManager::OpenSocket(const CString strIP)
{
	m_Sock.m_ipaddr = strIP;

	BOOL bOpen = m_Sock.OpenSocket();

	if (bOpen)
		m_sConnType = NET_CONNECT;
	else
		m_sConnType = NON_CONNECT;

	return bOpen;
}

// ��������
BOOL CCommunicateManager::OpenSerial(const int iComPort, const long iBaudrate)
{
	BOOL bOpen = m_Serial.OpenPort(iComPort, iBaudrate);

	if (bOpen)
		m_sConnType = COM_CONNECT;
	else
		m_sConnType = NON_CONNECT;

	return bOpen;
}

// �Ͽ�ͨ��
void CCommunicateManager::Close()
{
	if (m_Sock.b_Connect)
		m_Sock.CloseSocket();

	if (m_Serial.m_bOpen)
		m_Serial.ClosePort();

	m_sConnType = NON_CONNECT;

	// ����ʼ����־��ΪFALSE
	GetInitManager()->SetInitializedFlag(FALSE);
}

// ͨ��ASCIIͨ������ӿ� �����Ͳ���
UINT CCommunicateManager::NormalASCII_RW(__in char *cmd, __in UINT *cmd_para, __in UINT len, __in emCardType eCT, __in UINT port)
{
	//ȷ���Ƿ����ͨ������
	if (m_sConnType == NON_CONNECT)	return CMD_TIMEOUT;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//ָ��֡��װ 
	//����ͷ

	if (eCT == eCardTypeCtrl)
		sprintf_s(wbuf, "<%s", cmd);
	else if (eCT == eCardTypeInput)
		sprintf_s(wbuf, "<%dX,%s", port, cmd);
	else if (eCT == eCardTypeOutput)
		sprintf_s(wbuf, "<%dY,%s", port, cmd);
	else
		sprintf_s(wbuf, "<%d,%s", port, cmd);

	//�������
	for(unsigned int i=0;i<len;i++)
	{	sprintf_s(wbuf,"%s,%d", wbuf, cmd_para[i]); }
	//����β
	sprintf_s(wbuf,"%s>", wbuf);

	//����ָ��
	if(m_Serial.m_bOpen)
		rbuf_len = m_Serial.SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_Sock.b_Connect) 
		rbuf_len = m_Sock.Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else
		return CMD_TIMEOUT;

	GetLogManager()->PrintLog(wbuf, strlen(wbuf));
	GetLogManager()->PrintLog(rbuf, strlen(rbuf));

	if(rbuf_len & CMD_TIMEOUT) { //������ʱ
		return rbuf_len;
	}

	int start_index,end_index;
	CString str,str_tmp;
	str.Format("%s", rbuf);
	if(str.Find("RTOK") != -1) //���� ִ����ɱ�־ "RTOK"
	{
		return CMD_RTOK;
	}
	else if(str.Find("RTER") != -1) //���� ִ��ʧ��/�����־ "RTER"
	{
		start_index = str.Find("RTER")+4;  //��4��cmd�����ַ� 1�������ַ� 
		end_index   = str.Find('>',start_index); //��ȡ��������λ��

		if(end_index > start_index)//��ʾ���ڴ������
		{
			str_tmp = str.Mid(start_index+1,end_index-start_index-1);	
			cmd_para[0] = atoi(str_tmp);
			return CMD_RTERCODE;
		}
		else
		{
			return CMD_RTER;
		}
	}
	else  //���ؾ�������� 
	{
		rbuf_len = 0;
		start_index = str.Find(cmd)+1+4;  //��4��cmd�����ַ� 1�������ַ� 
		end_index   = str.Find(',',start_index);

		int iSubStr = 0;

		while(end_index != -1)//��ȡ','֮�����ֵ ʮ����
		{
			str_tmp = str.Mid(start_index,end_index-start_index);

			if (str_tmp.Find('A', iSubStr) != -1)
			{
				cmd_para[rbuf_len] = atoi(str_tmp.Right(str_tmp.GetLength()-iSubStr));
				cmd_para[rbuf_len] |= SUBPORT_A;
				rbuf_len++;
			}
			else if (str_tmp.Find('B', iSubStr) != -1)
			{
				cmd_para[rbuf_len] = atoi(str_tmp.Right(str_tmp.GetLength()-iSubStr));
				cmd_para[rbuf_len] |= SUBPORT_B;
				rbuf_len++;
			}
			else
				cmd_para[rbuf_len++] = atoi(str_tmp);

			start_index = end_index+1;
			end_index   = str.Find(',',start_index);
		}

		end_index   = str.Find('>',start_index);
		if(end_index!=-1) //��ȡ','��'>'֮�����ֵ ʮ���� 
		{
			str_tmp = str.Mid(start_index,end_index-start_index);	
			cmd_para[rbuf_len++] = (UINT)strtoul(str_tmp, NULL, 10);
		}
		return rbuf_len;
	}

	return CMD_RTOK;
}

// ͨ��ASCIIͨ������ӿ� �ַ����Ͳ���
UINT CCommunicateManager::NormalASCII_RW(__in char *cmd, __in char *cmd_para, __in int nParamFormat, __in UINT len, __in emCardType eCT, __in UINT port)
{
	//ȷ���Ƿ����ͨ������
	if (m_sConnType == NON_CONNECT)	return CMD_TIMEOUT;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//ָ��֡��װ 
	//����ͷ

	if (eCT == eCardTypeCtrl)
		sprintf_s(wbuf, "<%s", cmd);
	else if (eCT == eCardTypeInput) {
		sprintf_s(wbuf, "<%dX,%s", port, cmd);
	}
	else if (eCT == eCardTypeOutput)
		sprintf_s(wbuf, "<%dY,%s", port, cmd);
	else
		sprintf_s(wbuf, "<%d,%s", port, cmd);

	//�������
	sprintf_s(wbuf, "%s,%s>", wbuf, cmd_para);

	//����ָ��
	if(m_Serial.m_bOpen)
		rbuf_len = m_Serial.SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_Sock.b_Connect) 
		rbuf_len = m_Sock.Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else
		return CMD_TIMEOUT;

	GetLogManager()->PrintLog(wbuf, strlen(wbuf));
	GetLogManager()->PrintLog(rbuf, strlen(rbuf));

	if(rbuf_len & CMD_TIMEOUT) { //������ʱ 
		return rbuf_len;
	}

	int start_index,end_index;
	CString str,str_tmp;
	str.Format("%s", rbuf);
	if(str.Find("RTOK") != -1) //���� ִ����ɱ�־ "RTOK"
	{
		return CMD_RTOK;
	}
	else if(str.Find("RTER") != -1) //���� ִ��ʧ��/�����־ "RTER"
	{
		start_index = str.Find("RTER")+4;  //��4��cmd�����ַ� 1�������ַ� 
		end_index   = str.Find('>',start_index); //��ȡ��������λ��

		if(end_index > start_index)//��ʾ���ڴ������
		{
			str_tmp = str.Mid(start_index+1,end_index-start_index-1);	
			cmd_para[0] = atoi(str_tmp);
			return CMD_RTERCODE;
		}
		else
		{
			return CMD_RTER;
		}
	}
	else  //���ؾ�������� 
	{
		rbuf_len = 0;
		start_index = str.Find(cmd)+1+4;  //��4��cmd�����ַ� 1�������ַ� 
		end_index   = str.Find(',',start_index);
		int iSubStr = 0;

		while(end_index != -1)//��ȡ','֮�����ֵ ʮ����
		{
			str_tmp = str.Mid(start_index,end_index-start_index);

			if (str_tmp.Find('A', iSubStr) != -1)
			{
				cmd_para[rbuf_len] = atoi(str_tmp.Right(str_tmp.GetLength()-iSubStr));
				cmd_para[rbuf_len] |= 0xAFFFF;
				rbuf_len++;
			}
			else if (str_tmp.Find('B', iSubStr) != -1)
			{
				cmd_para[rbuf_len] = atoi(str_tmp.Right(str_tmp.GetLength()-iSubStr));
				cmd_para[rbuf_len] |= 0xBFFFF;
				rbuf_len++;
			}
			else
				cmd_para[rbuf_len++] = atoi(str_tmp);

			start_index = end_index+1;
			end_index   = str.Find(',',start_index);
		}

		end_index   = str.Find('>',start_index);
		if(end_index!=-1) //��ȡ','��'>'֮�����ֵ ʮ���� 
		{
			str_tmp = str.Mid(start_index,end_index-start_index);	
			cmd_para[rbuf_len++] = (UINT)strtoul(str_tmp, NULL, 10);
		}
		return rbuf_len;
	}

	return CMD_RTOK;
}

// ���߼��
BOOL CCommunicateManager::OnlineDetect()
{
	//ȷ���Ƿ����ͨ������
	if (m_sConnType == NON_CONNECT)	return FALSE;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//ָ��֡��װ 
	//����ͷ
	sprintf_s(wbuf, "<RMVR>");

	// ���߼��������ָ��ͨ���ٽ���
	CommunicationLock();

	//����ָ��
	if(m_Serial.m_bOpen)
		rbuf_len = m_Serial.SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_Sock.b_Connect) 
		rbuf_len = m_Sock.Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);

	CommunicationUnlock();

	// ��������ִ��״̬
	return ((rbuf_len & CMD_RTER) != CMD_RTER);
}

// ������
int CCommunicateManager::CmdRMVR(__in const int iPort, __in const emCardType eCardtype)
{
	UINT nParam[SAFE_BUFF]={0};
	UINT nRet = NormalASCII_RW("RMVR", nParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

// �忨ID
int CCommunicateManager::CmdRDID(__in const int iPort, __in const emCardType eCardtype)
{
	UINT nParam[SAFE_BUFF]={0};
	UINT nRet = NormalASCII_RW("RDID", nParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

// �忨������
int CCommunicateManager::CmdRDFN(__in const int iPort, __in const emCardType eCardtype)
{
	UINT nParam[SAFE_BUFF]={0};
	UINT nRet = NormalASCII_RW("RDFN", nParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

// ������ֱ���
int CCommunicateManager::CmdRINT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __out TIMING &sTiming)
{
	int nLen = 0;
	UINT nParam[SAFE_BUFF] = {0};
	
	if (iTimingIndex != 0xff)
		nParam[nLen++] = iTimingIndex;

	UINT nRet = NormalASCII_RW("RINT", nParam, nLen, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	if (nParam[4] <= 0) return CMD_FAILURE;

	int nTick = 1;
	if (iTimingIndex == 0xff) nTick = 0; //��������
	//�������еĲ�������ȡֵ
	sTiming.m_Hz        = nParam[nTick++];
	sTiming.m_HTotal    = nParam[nTick++];
	sTiming.m_HActive   = nParam[nTick++];
	sTiming.m_HFPorch   = nParam[nTick++];
	sTiming.m_HSyncWid  = nParam[nTick++];
	sTiming.m_HPol      = nParam[nTick++];
	sTiming.m_VTotal    = nParam[nTick++];
	sTiming.m_VActive   = nParam[nTick++];
	sTiming.m_VFPorch   = nParam[nTick++];
	sTiming.m_VSyncWid  = nParam[nTick++];
	sTiming.m_VPol      = nParam[nTick++];
	sTiming.m_PixelRate = nParam[nTick++];

	sTiming.m_HBlank    = sTiming.m_HTotal - sTiming.m_HActive;
	sTiming.m_VBlank    = sTiming.m_VTotal - sTiming.m_VActive;

	return CMD_RTOK;
}

// д����ֱ���
int CCommunicateManager::CmdWINT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __in TIMING &sTiming)
{
	UINT nParam[SAFE_BUFF] = {0};
	sTiming.m_HTotal = sTiming.m_HActive+sTiming.m_HBlank;
	sTiming.m_VTotal = sTiming.m_VActive+sTiming.m_VBlank;

	int nTick = 0;
	//������д��ʱ����Ҫ�ӷֱ�����ţ�����ͳһ��0xff��ʾĬ�Ϸֱ���
	if (iTimingIndex != 0xff) nParam[nTick++] = iTimingIndex;
	nParam[nTick++] = sTiming.m_Hz;
	nParam[nTick++] = sTiming.m_HTotal;
	nParam[nTick++] = sTiming.m_HActive;
	nParam[nTick++] = sTiming.m_HFPorch;
	nParam[nTick++] = sTiming.m_HSyncWid;
	nParam[nTick++] = sTiming.m_HPol;
	nParam[nTick++] = sTiming.m_VTotal;
	nParam[nTick++] = sTiming.m_VActive;
	nParam[nTick++] = sTiming.m_VFPorch;
	nParam[nTick++] = sTiming.m_VSyncWid;
	nParam[nTick++] = sTiming.m_VPol;
	nParam[nTick++] = sTiming.m_PixelRate;

	UINT nRet = NormalASCII_RW("WINT", nParam, nTick, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ������ֱ���
int CCommunicateManager::CmdROUT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __out TIMING &sTiming)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("ROUT", nParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i=1;

	sTiming.m_Hz        = nParam[i++];
	sTiming.m_HTotal    = nParam[i++];
	sTiming.m_HActive   = nParam[i++];
	sTiming.m_HFPorch   = nParam[i++];
	sTiming.m_HSyncWid  = nParam[i++];
	sTiming.m_HPol      = nParam[i++];
	sTiming.m_VTotal    = nParam[i++];
	sTiming.m_VActive   = nParam[i++];
	sTiming.m_VFPorch   = nParam[i++];
	sTiming.m_VSyncWid  = nParam[i++];
	sTiming.m_PixelRate = nParam[i++];

	sTiming.m_HBlank    = sTiming.m_HTotal - sTiming.m_HActive;
	sTiming.m_VBlank    = sTiming.m_VTotal - sTiming.m_VActive;

	return CMD_RTOK;
}

// д����ֱ���
int CCommunicateManager::CmdWOUT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __in TIMING &sTiming)
{
	UINT nParam[SAFE_BUFF] = {0};
	sTiming.m_HTotal = sTiming.m_HActive+sTiming.m_HBlank;
	sTiming.m_VTotal = sTiming.m_VActive+sTiming.m_VBlank;

	int nTick = 0;
	nParam[nTick++] = 1; //Different from input
	nParam[nTick++] = sTiming.m_Hz;
	nParam[nTick++] = sTiming.m_HTotal;
	nParam[nTick++] = sTiming.m_HActive;
	nParam[nTick++] = sTiming.m_HFPorch;
	nParam[nTick++] = sTiming.m_HSyncWid;
	nParam[nTick++] = sTiming.m_HPol;
	nParam[nTick++] = sTiming.m_VTotal;
	nParam[nTick++] = sTiming.m_VActive;
	nParam[nTick++] = sTiming.m_VFPorch;
	nParam[nTick++] = sTiming.m_VSyncWid;
	nParam[nTick++] = sTiming.m_VPol;
	nParam[nTick++] = sTiming.m_PixelRate;

	UINT nRet = NormalASCII_RW("WOUT", nParam, nTick, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ����ʱ�ֱ���
int CCommunicateManager::CmdGETR(__in const int iPort, __in const emCardType eCardtype, __out TIMING &sTiming)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("GETR", nParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i = 0;
	sTiming.m_Hz      = nParam[i++];
	sTiming.m_HTotal  = nParam[i++];
	sTiming.m_HActive = nParam[i++];
	sTiming.m_VTotal  = nParam[i++];
	sTiming.m_VActive = nParam[i++];

	return CMD_RTOK;
}

// ���μ��豸EDID�ķֱ���
int CCommunicateManager::CmdREDN(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __out TIMING &sTiming)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("REDN", uParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i=0;

	sTiming.m_HTotal    = uParam[i++];
	sTiming.m_HActive   = uParam[i++];
	sTiming.m_HFPorch   = uParam[i++];
	sTiming.m_HSyncWid  = uParam[i++];
	sTiming.m_HPol      = uParam[i++];
	sTiming.m_VTotal    = uParam[i++];
	sTiming.m_VActive   = uParam[i++];
	sTiming.m_VFPorch   = uParam[i++];
	sTiming.m_VSyncWid  = uParam[i++];
	sTiming.m_VPol      = uParam[i++];
	sTiming.m_PixelRate = uParam[i++];

	sTiming.m_Hz        = int(sTiming.m_PixelRate*10000.0/(sTiming.m_HTotal*sTiming.m_VTotal)+0.05);

	sTiming.m_HBlank    = sTiming.m_HTotal - sTiming.m_HActive;
	sTiming.m_VBlank    = sTiming.m_VTotal - sTiming.m_VActive;

	return CMD_RTOK;
}

// д��EDID���ݿ�
int CCommunicateManager::CmdWEDI(__in const int iPort, __in const emCardType eCardtype, __in UINT *pEdidData, __in int iLen)
{
	UINT nRet = NormalASCII_RW("WEDI", pEdidData, iLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ����������
int CCommunicateManager::CmdRNET(__in const int iPort, __in const emCardType eCardtype, __out NETPARAMS &np)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RNET", nParam, 0, eCardtype, iPort);

	if (nRet == CMD_RTER || nRet== CMD_RTERCODE || nRet == CMD_TIMEOUT) return CMD_FAILURE;

	if (nRet != 18) return CMD_FAILURE; //���������������Ӧ����18������

	memcpy(&np, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// д��������
int CCommunicateManager::CmdWNET(__in const int iPort, __in const emCardType eCardtype, __in const NETPARAMS &np)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nLen = sizeof(NETPARAMS)/sizeof(UINT);

	memcpy(nParam, &np, sizeof(NETPARAMS));

	UINT nRet = NormalASCII_RW("WNET", nParam, nLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ͼ������ۺ����� ����TST0(��ɫ����)��TST2(�������)����
int CCommunicateManager::CmdImageTest(__in const int iPort, __in const emCardType eCardtype, __in const pIMGTESTINFO pImgTestInfo)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	UINT nRet;

	uParam[nTick++] = (pImgTestInfo->eTesttype == eTestTypeCancelGrid || pImgTestInfo->eTesttype == eTestTypeCancelColor) ? 255 : pImgTestInfo->eTesttype;
	uParam[nTick++] = GetRValue(pImgTestInfo->rgb);
	uParam[nTick++] = GetGValue(pImgTestInfo->rgb);
	uParam[nTick++] = GetBValue(pImgTestInfo->rgb);

	if (pImgTestInfo->eTesttype == eTestTypeGrid)
	{
		uParam[nTick++] = pImgTestInfo->ptBegin.x;
		uParam[nTick++] = pImgTestInfo->ptBegin.y;
		uParam[nTick++] = pImgTestInfo->szLineSpace.cx;
		uParam[nTick++] = pImgTestInfo->szLineSpace.cy;
		uParam[nTick++] = pImgTestInfo->iLineWidth;

		nRet = NormalASCII_RW("TST2", uParam, nTick, eCardtype, iPort);
	}
	else if (pImgTestInfo->eTesttype == eTestTypeColor)
	{
		nRet = NormalASCII_RW("TST0", uParam, nTick, eCardtype, iPort);
	}
	else if (pImgTestInfo->eTesttype == eTestTypeCancelColor)
	{
		nRet = NormalASCII_RW(("TST0"), uParam, nTick, eCardtype, iPort);
	}
	else if (pImgTestInfo->eTesttype == eTestTypeCancelGrid)
	{
		nRet = NormalASCII_RW(("TST2"), uParam, nTick, eCardtype, iPort);
	}
	

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// �˿�ͳ��
int CCommunicateManager::CmdGCMG(__in const int iPort, __in const emCardType eCardtype, __out UINT &iInputNum, __out UINT &iOutputNum, __out UINT* pInputFlags, __out UINT* pOutputFlags)
{
	UINT uParams[SAFE_BUFF] = {0};

	int nRet = NormalASCII_RW("GCMG", uParams);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	UINT iTick = 0;
	iInputNum  = uParams[iTick++]; //����ͨ������
	iOutputNum = uParams[iTick++]; //���ͨ������

	for (int i=0; i< iInputNum; i++ )
		pInputFlags[i] = uParams[iTick++]; //����ͨ�����ڱ�־

	for (int i=0; i<iOutputNum; i++)
		pOutputFlags[i] = uParams[iTick++]; //���ͨ�����ڱ�־

	return CMD_RTOK;
}

// ��ȡ�˿��ź�����<RPM3,channel,signaltype,change_flag>
int CCommunicateManager::CmdRPM3(__in const int iPort, __in const emCardType eCardtype, __out int &iSignalType)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RPM3", uParam, 0, eCardtype, iPort);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	iSignalType = uParam[0];

	return CMD_RTOK;
}

// ��������
int CCommunicateManager::CmdSAVE(__in const int iPort, __in const emCardType eCardtype, __in const int iSceneID)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iSceneID;

	int nRet = NormalASCII_RW("SAVE", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ������ȡ
int CCommunicateManager::CmdCALL(__in const int iPort, __in const emCardType eCardtype, __in const int iSceneID, __out UINT* &pBuffer, __out int &buffLen, BOOL bAllocBuff)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iSceneID;

	int nRet = NormalASCII_RW("CALL", uParam, nIter);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	// ��һ��ֵΪ������
	ASSERT(uParam[0] == iSceneID);

	if (bAllocBuff)
	{
		buffLen = nRet-1;
		pBuffer = new UINT[buffLen];
		ZeroMemory(pBuffer, buffLen);

		//ASSERT(nRet < buffLen);

		memcpy_s(pBuffer, buffLen*sizeof(UINT), &uParam[1], buffLen*sizeof(UINT));
	}

	return CMD_RTOK;
}

// OSD��ʾ
int CCommunicateManager::CmdYOSD(__in const int iPort, __in const emCardType eCardtype)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;

	uParam[nTick++] = 0x10;

	UINT nRet = NormalASCII_RW("YOSD", uParam, nTick, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// OSDȡ��
int CCommunicateManager::CmdCOSD(__in const int iPort, __in const emCardType eCardtype)
{
	UINT nRet = NormalASCII_RW("COSD", NULL, 0, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// Ĭ������/�Աȶ�
int CCommunicateManager::CmdRST2(__in const int iPort, __in const emCardType eCardtype, __out int &nBrightness, __out int &nContrast)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RST2", uParam, 0, eCardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nBrightness = uParam[0];
	nContrast = uParam[1];

	return CMD_RTOK;
}

// ������/�Աȶ�
int CCommunicateManager::CmdRPM2(__in const int iPort, __in const emCardType eCardtype, __out int &nBrightness, __out int &nContrast)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RPM2", uParam, 0, eCardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nBrightness = uParam[0];
	nContrast = uParam[1];

	return CMD_RTOK;
}

// д����/�Աȶ�
int CCommunicateManager::CmdWPM2(__in const int iPort, __in const emCardType eCardtype, __in const int nBrightness, __in const int nContrast)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	uParam[nTick++] = nBrightness;
	uParam[nTick++] = nContrast;

	UINT nRet = NormalASCII_RW("WPM2", uParam, nTick, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// Ĭ�ϲ���
int CCommunicateManager::CmdRST1(__in const int iPort, __in const emCardType eCardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RST2", uParam, 0, eCardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nLeft = uParam[0];
	nTop = uParam[1];
	nWidth = uParam[2];
	nHeight = uParam[3];

	return CMD_RTOK;
}

// �Զ�����
int CCommunicateManager::CmdATO1(__in const int iPort, __in const emCardType eCardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("ATO1", uParam, 0, eCardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nLeft = uParam[0];
	nTop = uParam[1];
	nWidth = uParam[2];
	nHeight = uParam[3];

	return CMD_RTOK;
}

// ����������
int CCommunicateManager::CmdRPM1(__in const int iPort, __in const emCardType eCardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RPM1", uParam, 0, eCardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nLeft = uParam[0];
	nTop = uParam[1];
	nWidth = uParam[2];
	nHeight = uParam[3];

	return CMD_RTOK;
}

// д��������
int CCommunicateManager::CmdWPM1(__in const int iPort, __in const emCardType eCardtype, __in const int nLeft, __in const int nTop, __in const int nWidth, __in const int nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	uParam[nTick++] = nLeft;
	uParam[nTick++] = nTop;
	uParam[nTick++] = nWidth;
	uParam[nTick++] = nHeight;

	UINT nRet = NormalASCII_RW("WPM1", uParam, nTick, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ɫ�ʿռ�ת��
int CCommunicateManager::CmdWICS(__in const int iPort, __in const emCardType eCardtype, __in const int nMode)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	uParam[nTick++] = nMode;

	UINT nRet = NormalASCII_RW("WICS", uParam, nTick, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// �˶Զ��л�
int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iFeederCh, __in const int iSubCh, __in const int iOutCh, __in emSwitchMode eSM/* =eSwitchModeP2P */)
{
	int  nLen = SAFE_BUFF;
	char cParam[SAFE_BUFF] = {0};
	
	if (iSubCh == 0)
		sprintf_s(cParam, SAFE_BUFF, "%d,%d,%d", int(eSM), iFeederCh, iOutCh);
	else
		sprintf_s(cParam, SAFE_BUFF, "%d,%d%c,%d", int(eSM), iFeederCh, iSubCh==1 ? 'A' : 'B', iOutCh);

	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// һ�Զ��л�
int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iFeederCh, __in const int iSubCh, __in int *pOutChList, __in const int iOutChNum, emSwitchMode eSM/* =eSwitchModeOnetoMany */)
{
	int  nLen = SAFE_BUFF*4;
	char cParam[SAFE_BUFF*4] = {0};

	if (iSubCh == 0)
		sprintf_s(cParam, nLen, "%d,%d", int(eSM), iFeederCh);
	else
		sprintf_s(cParam, nLen, "%d,%d%c", int(eSM), iFeederCh, iSubCh==1 ? 'A' : 'B');

	for (int i=0; i<iOutChNum; i++)
		sprintf_s(cParam, nLen, "%s,%d", cParam, pOutChList[i]);

	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// һ�������л�
int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iFeederCh, __in const int iSubCh, __in emSwitchMode eSM/* =eSwitchModeOnetoAll */)
{
	int  nLen = SAFE_BUFF;
	char cParam[SAFE_BUFF] = {0};

	if (iSubCh == 0)
		sprintf_s(cParam, nLen, "%d,%d", int(eSM), iFeederCh);
	else
		sprintf_s(cParam, nLen, "%d,%d%c", int(eSM), iFeederCh, iSubCh==1 ? 'A' : 'B');

	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// һһ��Ӧ�л�
int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in emSwitchMode eSM/* =eSwitchModeOnetoOne */)
{
	int  nLen = 0;
	UINT nParam[SAFE_BUFF] = {0};
	nParam[nLen++] = int(eSM);

	UINT nRet = NormalASCII_RW("SWCH", nParam, nLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ӳ��ʽ�л�
int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iOutChNum, __in int *pFeederCh, __in emSwitchMode eSM/* =eSwitchModeMapping */)
{
	int  nLen = SAFE_BUFF*8;
	char cParam[SAFE_BUFF*8] = {0};

	sprintf_s(cParam, nLen, "%d,%d", int(eSM), iOutChNum);

	for (int i=0; i<iOutChNum; i++)
	{
		if ((pFeederCh[i] & 0xFFFF0000) == SUBPORT_A)
			sprintf_s(cParam, nLen, "%s,%d%c", cParam, pFeederCh[i]&0xFFFF, 'A');
		else if ((pFeederCh[i] & 0xFFFF0000) == SUBPORT_B)
			sprintf_s(cParam, nLen, "%s,%d%c", cParam, pFeederCh[i]&0xFFFF, 'B');
		else
			sprintf_s(cParam, nLen, "%s,%d", cParam, pFeederCh[i]&0xFFFF);
	}

	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

// ��λ
int CCommunicateManager::CmdRSTF(__in const int iPort, __in const emCardType eCardtype)
{
	UINT nRet = NormalASCII_RW("RSTF", NULL, 0, eCardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}