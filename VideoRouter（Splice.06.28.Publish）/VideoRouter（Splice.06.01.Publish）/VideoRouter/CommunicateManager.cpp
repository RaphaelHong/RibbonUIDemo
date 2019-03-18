// CommunicateManager.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "CommunicateManager.h"
#include <IPHlpApi.h>
#pragma comment(lib, "iphlpapi.lib")

using namespace std;

///////////////////////////////////////////////////////////////////////////////////
// 以下是用于获取局域网所有在线主机地址代码段
//

IPMAC             host[254]; //254个IP地址信息 
int               host_k;    //有效IP计数
char              *pLocalIP; //本机IP地址
CRITICAL_SECTION  g_cs;      //临界控制变量

DWORD WINAPI ArpThread(LPVOID  lParam) ; 

//获取本机IP地址和MAC地址信息
char* GetLocalHostAddr()
{
	char szHostname[256] = {0};

	//获取主机名
	if (gethostname(szHostname, 256) != 0)
	{
		return NULL;
	}

	//获取主机信息
	hostent *pHost = gethostbyname(szHostname);
	if (pHost == NULL)
	{
		return NULL;
	}

	//获取本机IP
	in_addr addr;
	char *p = pHost->h_addr_list[0];
	memcpy(&addr.S_un.S_addr, p, pHost->h_length);
	char *szIP = inet_ntoa(addr);

	return szIP;
}


/*线程函数，用来查询IP对应的MAC地址*/  
DWORD WINAPI ArpThread(LPVOID  lParam)  
{  
	char *szIp=(char *)lParam;  
	ULONG pMac[2];  
	ULONG pulen=6;

	int ret;    
	if ((ret=SendARP(inet_addr(szIp), inet_addr(pLocalIP), pMac, &pulen))==0)  
	{  
		EnterCriticalSection(&g_cs);  //多线程同步，呵呵:0  

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
		AfxMessageBox(_T("错误，获取本机地址信息失败！"),  MB_OK|MB_ICONWARNING);
		return;
	}

	//为ARP线程赋本机IP地址
	pLocalIP = pHostAddr;

	CString str;
	str.Format(_T("%s"), pHostAddr);
	int nTok = str.ReverseFind(_T('.'));
	CString  IpSuffix = str.Left(nTok) + _T(".");

	HANDLE  hthread[254];              // 线程句柄 "192.168.1.1" - "192.168.1.254"
	CString  strIp[254];               // 254个不同的IP地址
	InitializeCriticalSection(&g_cs);  //初始化临界量
	host_k = 0; //初始化IP计数
	for  (int  i=0;i<254;i++)  
	{  
		//创建ARP线程
		strIp[i].Format("%d",i+1);  
		strIp[i]=IpSuffix+strIp[i];  
		hthread[i] = CreateThread(NULL,0, ArpThread,strIp[i].GetBuffer(0),0,NULL);    
	}  
	/*呵呵，因为一次只能等待  64个内核对象，所以只有分几次了*/  
	/*当然也可以用循环了*/  
	WaitForMultipleObjects(64,hthread,TRUE,INFINITE);  
	WaitForMultipleObjects(64,&hthread[64],TRUE,INFINITE);  
	WaitForMultipleObjects(64,&hthread[128],TRUE,INFINITE);  
	WaitForMultipleObjects(61,&hthread[192],TRUE,INFINITE);  
	DeleteCriticalSection(&g_cs); 

	char rbuf[256] = {0};
	//剔除重复IP内容
	for (int i=0; i<host_k-1; i++)	{
		for (int j=i+1; j<host_k; j++)	{
			if (host[i].szIp == host[j].szIp)
			{
				//如果第j个元素与第i个元素重复，则将数组最后一个元素覆盖第j个元素，数组计数减一
				host[j] = host[host_k-1];
				host_k--;
			}
		}
	}

	//根据连接后返回的字符串判断是否是本公司的设备
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
				//将可用IP地址放入返回列表中
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


// CCommunicateManager 成员函数
BOOL CCommunicateManager::IsConnected()
{
	return (m_sConnType != NON_CONNECT);
}

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

	if(lReg!=ERROR_SUCCESS) //成功时返回ERROR_SUCCESS，
		return;

	lReg=RegQueryInfoKeyA(hKey, 
		NULL, 
		NULL, 
		NULL, 
		NULL,
		NULL, 
		NULL, 
		&dwValueNumber, //返回和hKey关联的值
		&MaxValueLength, 
		NULL, 
		NULL, 
		NULL); 

	if(lReg!=ERROR_SUCCESS) //没有成功
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

BOOL CCommunicateManager::OpenSerial(const int iComPort, const long iBaudrate)
{
	BOOL bOpen = m_Serial.OpenPort(iComPort, iBaudrate);

	if (bOpen)
		m_sConnType = COM_CONNECT;
	else
		m_sConnType = NON_CONNECT;

	return bOpen;
}

void CCommunicateManager::Close()
{
	if (m_Sock.b_Connect)
		m_Sock.CloseSocket();

	if (m_Serial.m_bOpen)
		m_Serial.ClosePort();

	m_sConnType = NON_CONNECT;

	GetInitManager()->SetInitializedFlag(FALSE);
}

UINT CCommunicateManager::NormalASCII_RW(__in char *cmd, __in UINT *cmd_para, __in UINT len, __in emBoardType eCT, __in UINT port)
{
	//确定是否存在通信渠道
	if (m_sConnType == NON_CONNECT)	return CMD_TIMEOUT;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//指令帧封装 
	//命令头

	if (eCT == eBoardTypeCtrl)
		sprintf_s(wbuf, "<%s", cmd);
	else if (eCT == eBoardTypeInput)
		sprintf_s(wbuf, "<%dX,%s", port, cmd);
	else if (eCT == eBoardTypeOutput)
		sprintf_s(wbuf, "<%dY,%s", port, cmd);
	else
		sprintf_s(wbuf, "<%d,%s", port, cmd);

	//命令参数
	for(unsigned int i=0;i<len;i++)
	{	sprintf_s(wbuf,"%s,%d", wbuf, cmd_para[i]); }
	//命令尾
	sprintf_s(wbuf,"%s>", wbuf);

	//发送指令
	if(m_Serial.m_bOpen)
		rbuf_len = m_Serial.SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_Sock.b_Connect) 
		rbuf_len = m_Sock.Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else
		return CMD_TIMEOUT;

	GetLogManager()->PrintLog(wbuf, strlen(wbuf));
	GetLogManager()->PrintLog(rbuf, strlen(rbuf));

	if(rbuf_len & CMD_TIMEOUT) //操作超时
	{
		return rbuf_len;
	}

	int start_index,end_index;
	CString str,str_tmp;
	str.Format("%s", rbuf);
	if(str.Find("RTOK") != -1) //返回 执行完成标志 "RTOK"
	{
		return CMD_RTOK;
	}
	else if(str.Find("RTER") != -1) //返回 执行失败/错误标志 "RTER"
	{
		start_index = str.Find("RTER")+4;  //过4个cmd命令字符 1个逗号字符 
		end_index   = str.Find('>',start_index); //获取结束符的位置

		if(end_index > start_index)//表示存在错误代码
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
	else  //返回具体的数据 
	{
		rbuf_len = 0;
		start_index = str.Find(cmd)+1+4;  //过4个cmd命令字符 1个逗号字符 
		end_index   = str.Find(',',start_index);

		int iSubStr = 0;

		while(end_index != -1)//获取','之间的数值 十进制
		{
			str_tmp = str.Mid(start_index,end_index-start_index);

			if (str_tmp.Find('A', iSubStr) != -1)
			{
				cmd_para[rbuf_len] = atoi(str_tmp.Right(str_tmp.GetLength()-iSubStr));
				cmd_para[rbuf_len] |= 0xA0000;
				rbuf_len++;
			}
			else if (str_tmp.Find('B', iSubStr) != -1)
			{
				cmd_para[rbuf_len] = atoi(str_tmp.Right(str_tmp.GetLength()-iSubStr));
				cmd_para[rbuf_len] |= 0xB0000;
				rbuf_len++;
			}
			else
				cmd_para[rbuf_len++] = atoi(str_tmp);

			start_index = end_index+1;
			end_index   = str.Find(',',start_index);
		}

		end_index   = str.Find('>',start_index);
		if(end_index!=-1) //获取','与'>'之间的数值 十进制 
		{
			str_tmp = str.Mid(start_index,end_index-start_index);	
			cmd_para[rbuf_len++] = (UINT)strtoul(str_tmp, NULL, 10);
		}
		return rbuf_len;
	}

	return CMD_RTOK;
}


UINT CCommunicateManager::NormalASCII_RW(__in char *cmd, __in char *cmd_para, __in int nParamFormat, __in UINT len, __in emBoardType eCT, __in UINT port)
{
	//确定是否存在通信渠道
	if (m_sConnType == NON_CONNECT)	return CMD_TIMEOUT;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//指令帧封装 
	//命令头

	if (eCT == eBoardTypeCtrl)
		sprintf_s(wbuf, "<%s", cmd);
	else if (eCT == eBoardTypeInput) {
		sprintf_s(wbuf, "<%dX,%s", port, cmd);
	}
	else if (eCT == eBoardTypeOutput)
		sprintf_s(wbuf, "<%dY,%s", port, cmd);
	else
		sprintf_s(wbuf, "<%d,%s", port, cmd);

	//命令参数
	sprintf_s(wbuf, "%s,%s>", wbuf, cmd_para);

	//发送指令
	if(m_Serial.m_bOpen)
		rbuf_len = m_Serial.SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_Sock.b_Connect) 
		rbuf_len = m_Sock.Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else
		return CMD_TIMEOUT;

	GetLogManager()->PrintLog(wbuf, strlen(wbuf));
	GetLogManager()->PrintLog(rbuf, strlen(rbuf));

	if(rbuf_len & CMD_TIMEOUT) //操作超时
	{
		return rbuf_len;
	}

	int start_index,end_index;
	CString str,str_tmp;
	str.Format("%s", rbuf);
	if(str.Find("RTOK") != -1) //返回 执行完成标志 "RTOK"
	{
		return CMD_RTOK;
	}
	else if(str.Find("RTER") != -1) //返回 执行失败/错误标志 "RTER"
	{
		start_index = str.Find("RTER")+4;  //过4个cmd命令字符 1个逗号字符 
		end_index   = str.Find('>',start_index); //获取结束符的位置

		if(end_index > start_index)//表示存在错误代码
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
	else  //返回具体的数据 
	{
		rbuf_len = 0;
		start_index = str.Find(cmd)+1+4;  //过4个cmd命令字符 1个逗号字符 
		end_index   = str.Find(',',start_index);
		int iSubStr = 0;

		while(end_index != -1)//获取','之间的数值 十进制
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
		if(end_index!=-1) //获取','与'>'之间的数值 十进制 
		{
			str_tmp = str.Mid(start_index,end_index-start_index);	
			cmd_para[rbuf_len++] = (UINT)strtoul(str_tmp, NULL, 10);
		}
		return rbuf_len;
	}

	return CMD_RTOK;
}

BOOL CCommunicateManager::OnlineDetect()
{
	//确定是否存在通信渠道
	if (m_sConnType == NON_CONNECT)	return FALSE;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//指令帧封装 
	//命令头
	sprintf_s(wbuf, "<RMVR>");

	//发送指令
	if(m_Serial.m_bOpen)
		rbuf_len = m_Serial.SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_Sock.b_Connect) 
		rbuf_len = m_Sock.Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else
		return FALSE;

	if(rbuf_len & CMD_TIMEOUT) //操作超时
		return FALSE;
	else
		return TRUE;

	return TRUE;
}

int CCommunicateManager::CmdRMVR(__in const int iPort, __in const emBoardType eBoardtype)
{
	UINT nParam[SAFE_BUFF]={0};
	UINT nRet = NormalASCII_RW("RMVR", nParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

int CCommunicateManager::CmdRDID(__in const int iPort, __in const emBoardType eBoardtype)
{
	UINT nParam[SAFE_BUFF]={0};
	UINT nRet = NormalASCII_RW("RDID", nParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

int CCommunicateManager::CmdRDFN(__in const int iPort, __in const emBoardType eBoardtype)
{
	UINT nParam[SAFE_BUFF]={0};
	UINT nRet = NormalASCII_RW("RDFN", nParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return nParam[0];
}

int CCommunicateManager::CmdRINT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __out TIMING &stTiming)
{
	int nLen = 0;
	UINT nParam[SAFE_BUFF] = {0};
	
	if (iTimingIndex != 0xff)
		nParam[nLen++] = iTimingIndex;

	UINT nRet = NormalASCII_RW("RINT", nParam, nLen, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	if (nParam[4] <= 0) return CMD_FAILURE;

	int nTick = 1;
	if (iTimingIndex == 0xff) nTick = 0; //不分主次
	//按命令中的参数排序取值
	stTiming.m_Hz        = nParam[nTick++];
	stTiming.m_HTotal    = nParam[nTick++];
	stTiming.m_HActive   = nParam[nTick++];
	stTiming.m_HFPorch   = nParam[nTick++];
	stTiming.m_HSyncWid  = nParam[nTick++];
	stTiming.m_HPol      = nParam[nTick++];
	stTiming.m_VTotal    = nParam[nTick++];
	stTiming.m_VActive   = nParam[nTick++];
	stTiming.m_VFPorch   = nParam[nTick++];
	stTiming.m_VSyncWid  = nParam[nTick++];
	stTiming.m_VPol      = nParam[nTick++];
	stTiming.m_PixelRate = nParam[nTick++];

	stTiming.m_HBlank    = stTiming.m_HTotal - stTiming.m_HActive;
	stTiming.m_VBlank    = stTiming.m_VTotal - stTiming.m_VActive;

	return CMD_RTOK;
}

int CCommunicateManager::CmdWINT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __in TIMING &stTiming)
{
	UINT nParam[SAFE_BUFF] = {0};
	stTiming.m_HTotal = stTiming.m_HActive+stTiming.m_HBlank;
	stTiming.m_VTotal = stTiming.m_VActive+stTiming.m_VBlank;

	int nTick = 0;
	//多屏保写的时候需要加分辨率序号，其余统一用0xff表示默认分辨率
	if (iTimingIndex != 0xff) nParam[nTick++] = iTimingIndex;
	nParam[nTick++] = stTiming.m_Hz;
	nParam[nTick++] = stTiming.m_HTotal;
	nParam[nTick++] = stTiming.m_HActive;
	nParam[nTick++] = stTiming.m_HFPorch;
	nParam[nTick++] = stTiming.m_HSyncWid;
	nParam[nTick++] = stTiming.m_HPol;
	nParam[nTick++] = stTiming.m_VTotal;
	nParam[nTick++] = stTiming.m_VActive;
	nParam[nTick++] = stTiming.m_VFPorch;
	nParam[nTick++] = stTiming.m_VSyncWid;
	nParam[nTick++] = stTiming.m_VPol;
	nParam[nTick++] = stTiming.m_PixelRate;

	UINT nRet = NormalASCII_RW("WINT", nParam, nTick, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdROUT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __out TIMING &stTiming)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("ROUT", nParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i=1;

	stTiming.m_Hz        = nParam[i++];
	stTiming.m_HTotal    = nParam[i++];
	stTiming.m_HActive   = nParam[i++];
	stTiming.m_HFPorch   = nParam[i++];
	stTiming.m_HSyncWid  = nParam[i++];
	stTiming.m_HPol      = nParam[i++];
	stTiming.m_VTotal    = nParam[i++];
	stTiming.m_VActive   = nParam[i++];
	stTiming.m_VFPorch   = nParam[i++];
	stTiming.m_VSyncWid  = nParam[i++];
	stTiming.m_PixelRate = nParam[i++];

	stTiming.m_HBlank    = stTiming.m_HTotal - stTiming.m_HActive;
	stTiming.m_VBlank    = stTiming.m_VTotal - stTiming.m_VActive;

	return CMD_RTOK;
}

int CCommunicateManager::CmdWOUT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __in TIMING &stTiming)
{
	UINT nParam[SAFE_BUFF] = {0};
	stTiming.m_HTotal = stTiming.m_HActive+stTiming.m_HBlank;
	stTiming.m_VTotal = stTiming.m_VActive+stTiming.m_VBlank;

	int nTick = 0;
	nParam[nTick++] = 1; //Different from input
	nParam[nTick++] = stTiming.m_Hz;
	nParam[nTick++] = stTiming.m_HTotal;
	nParam[nTick++] = stTiming.m_HActive;
	nParam[nTick++] = stTiming.m_HFPorch;
	nParam[nTick++] = stTiming.m_HSyncWid;
	nParam[nTick++] = stTiming.m_HPol;
	nParam[nTick++] = stTiming.m_VTotal;
	nParam[nTick++] = stTiming.m_VActive;
	nParam[nTick++] = stTiming.m_VFPorch;
	nParam[nTick++] = stTiming.m_VSyncWid;
	nParam[nTick++] = stTiming.m_VPol;
	nParam[nTick++] = stTiming.m_PixelRate;

	UINT nRet = NormalASCII_RW("WOUT", nParam, nTick, eBoardtype, iPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

int CCommunicateManager::CmdGETR(__in const int iPort, __in const emBoardType eBoardtype, __out TIMING &stTiming)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("GETR", nParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i = 0;
	stTiming.m_Hz      = nParam[i++];
	stTiming.m_HTotal  = nParam[i++];
	stTiming.m_HActive = nParam[i++];
	stTiming.m_VTotal  = nParam[i++];
	stTiming.m_VActive = nParam[i++];

	return CMD_RTOK;
}

int CCommunicateManager::CmdREDN(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __out TIMING &stTiming)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("REDN", uParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i=0;

	stTiming.m_HTotal    = uParam[i++];
	stTiming.m_HActive   = uParam[i++];
	stTiming.m_HFPorch   = uParam[i++];
	stTiming.m_HSyncWid  = uParam[i++];
	stTiming.m_HPol      = uParam[i++];
	stTiming.m_VTotal    = uParam[i++];
	stTiming.m_VActive   = uParam[i++];
	stTiming.m_VFPorch   = uParam[i++];
	stTiming.m_VSyncWid  = uParam[i++];
	stTiming.m_VPol      = uParam[i++];
	stTiming.m_PixelRate = uParam[i++];

	stTiming.m_Hz        = int(stTiming.m_PixelRate*10000.0/(stTiming.m_HTotal*stTiming.m_VTotal)+0.05);

	stTiming.m_HBlank    = stTiming.m_HTotal - stTiming.m_HActive;
	stTiming.m_VBlank    = stTiming.m_VTotal - stTiming.m_VActive;

	return CMD_RTOK;
}

int CCommunicateManager::CmdWEDI(__in const int iPort, __in const emBoardType eBoardtype, __in UINT *pEdidData, __in int iLen)
{
	UINT nRet = NormalASCII_RW("WEDI", pEdidData, iLen, eBoardtype, iPort);
	
	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdRNET(__in const int iPort, __in const emBoardType eBoardtype, __out NETPARAMS &np)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RNET", nParam, 0, eBoardtype, iPort);

	if (nRet == CMD_RTER || nRet== CMD_RTERCODE || nRet == CMD_TIMEOUT) return CMD_FAILURE;

	if (nRet != 18) return CMD_FAILURE; //正常网络参数命令应返回18个整数

	memcpy(&np, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

int CCommunicateManager::CmdWNET(__in const int iPort, __in const emBoardType eBoardtype, __in const NETPARAMS &np)
{
	UINT nParam[SAFE_BUFF] = {0};

	UINT nLen = sizeof(NETPARAMS)/sizeof(UINT);

	memcpy(nParam, &np, sizeof(NETPARAMS));

	UINT nRet = NormalASCII_RW("WNET", nParam, nLen, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}


int CCommunicateManager::CmdImageTest(__in const int iPort, __in const emBoardType eBoardtype, __in const pIMGTESTINFO pImgTestInfo)
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

		nRet = NormalASCII_RW("TST2", uParam, nTick, eBoardtype, iPort);
	}
	else if (pImgTestInfo->eTesttype == eTestTypeColor)
	{
		nRet = NormalASCII_RW("TST0", uParam, nTick, eBoardtype, iPort);
	}
	else if (pImgTestInfo->eTesttype == eTestTypeCancelColor)
	{
		nRet = NormalASCII_RW(("TST0"), uParam, nTick, eBoardtype, iPort);
	}
	else if (pImgTestInfo->eTesttype == eTestTypeCancelGrid)
	{
		nRet = NormalASCII_RW(("TST2"), uParam, nTick, eBoardtype, iPort);
	}
	

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdRCN1(__in int iPort, __in emBoardType eCT, __out pSPLICELAYOUT pLayout)
{
	UINT uParam[SAFE_BUFF] = {0};

	int nRet = NormalASCII_RW("RCN1", uParam);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	int nIter = 0;

	int iVideoType = uParam[nIter++]; //0：本地信号 1：外部信号

	pLayout->BkgTiming.m_Hz      = uParam[nIter++];
	pLayout->BkgTiming.m_HTotal  = uParam[nIter++];
	pLayout->BkgTiming.m_HActive = uParam[nIter++];
	pLayout->BkgTiming.m_HFPorch = uParam[nIter++];
	pLayout->BkgTiming.m_HSyncWid= uParam[nIter++];
	pLayout->BkgTiming.m_HPol    = uParam[nIter++];

	pLayout->BkgTiming.m_VTotal  = uParam[nIter++];
	pLayout->BkgTiming.m_VActive = uParam[nIter++];
	pLayout->BkgTiming.m_VFPorch = uParam[nIter++];
	pLayout->BkgTiming.m_VSyncWid= uParam[nIter++];
	pLayout->BkgTiming.m_VPol    = uParam[nIter++];

	pLayout->BkgTiming.m_PixelRate = uParam[nIter++];
	pLayout->BkgTiming.m_HBlank    = pLayout->BkgTiming.m_HTotal - pLayout->BkgTiming.m_HActive;
	pLayout->BkgTiming.m_VBlank    = pLayout->BkgTiming.m_VTotal - pLayout->BkgTiming.m_VActive;

	pLayout->BkgTiming.m_Type = DEFAULT_RESOLUTION;

	if (!pLayout->BkgTiming.IsValid())
		return CMD_FAILURE;

	pLayout->iPhyVert    = uParam[nIter++];
	pLayout->iPhyHorz    = uParam[nIter++];
	pLayout->iEdgeWidth  = uParam[nIter++];
	pLayout->iEdgeHeight = uParam[nIter++];
	pLayout->iLogVert    = uParam[nIter++];
	pLayout->iLogHorz    = uParam[nIter++];

	pLayout->iPhyHorzPixel = pLayout->iPhyHorz*pLayout->BkgTiming.m_HActive;
	pLayout->iPhyVertPixel = pLayout->iPhyVert*pLayout->BkgTiming.m_VActive;

	return CMD_RTOK;
}

int CCommunicateManager::CmdWCN1(__in int iPort, __in emBoardType eCT, __in const SPLICELAYOUT &Layout, __in const int iVideoType)
{
	UINT uParam[SAFE_BUFF] = {0};

	int nIter = 0;
	uParam[nIter++] = iVideoType;
	uParam[nIter++] = Layout.BkgTiming.m_Hz;
	uParam[nIter++] = Layout.BkgTiming.m_HTotal;
	uParam[nIter++] = Layout.BkgTiming.m_HActive;
	uParam[nIter++] = Layout.BkgTiming.m_HFPorch;
	uParam[nIter++] = Layout.BkgTiming.m_HSyncWid;
	uParam[nIter++] = Layout.BkgTiming.m_HPol;

	uParam[nIter++] = Layout.BkgTiming.m_VTotal;
	uParam[nIter++] = Layout.BkgTiming.m_VActive;
	uParam[nIter++] = Layout.BkgTiming.m_VFPorch;
	uParam[nIter++] = Layout.BkgTiming.m_VSyncWid;
	uParam[nIter++] = Layout.BkgTiming.m_VPol;
	uParam[nIter++] = Layout.BkgTiming.m_PixelRate;

	uParam[nIter++] = Layout.iPhyVert;
	uParam[nIter++] = Layout.iPhyHorz;
	uParam[nIter++] = Layout.iEdgeWidth;
	uParam[nIter++] = Layout.iEdgeHeight;
	uParam[nIter++] = Layout.iLogVert;
	uParam[nIter++] = Layout.iLogHorz;

	int nRet = NormalASCII_RW("WCN1", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdGCMG(__in const int iPort, __in const emBoardType eBoardtype, __out UINT &iInputNum, __out UINT &iOutputNum, __out UINT* pInputFlags, __out UINT* pOutputFlags)
{
	UINT uParams[SAFE_BUFF] = {0};

	int nRet = NormalASCII_RW("GCMG", uParams);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	UINT iTick = 0;
	iInputNum  = uParams[iTick++]; //输入通道个数
	iOutputNum = uParams[iTick++]; //输出通道个数

	for (int i=0; i< iInputNum; i++ )
		pInputFlags[i] = uParams[iTick++]; //输入通道存在标志

	for (int i=0; i<iOutputNum; i++)
		pOutputFlags[i] = uParams[iTick++]; //输出通道存在标志

	return CMD_RTOK;
}

// <RPM3,channel,signaltype,change_flag>
int CCommunicateManager::CmdRPM3(__in const int iPort, __in const emBoardType eBoardtype, __out int &iSignalType)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RPM3", uParam, 0, eBoardtype, iPort);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	iSignalType = uParam[0];

	return CMD_RTOK;
}

int CCommunicateManager::CmdGOPS(__in const int iPort, __in const emBoardType eBoardtype, __out SCREENMAP &scrmap)
{
	UINT uParam[SAFE_BUFF] = {0};

	int nIter = 0;
	uParam[nIter++] = scrmap.iScreenID;

	UINT nRet = NormalASCII_RW("GOPS", uParam, nIter);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	nIter = 0;
	if (scrmap.iScreenID != uParam[nIter++])
		return CMD_FAILURE;

	scrmap.iOutputCh = uParam[nIter++];

	scrmap.rcScreenPos.left   = uParam[nIter++];
	scrmap.rcScreenPos.top    = uParam[nIter++];
	scrmap.rcScreenPos.right  = uParam[nIter++];
	scrmap.rcScreenPos.bottom = uParam[nIter++];

	return CMD_RTOK;
}

int CCommunicateManager::CmdSOPS(__in const int iPort, __in const emBoardType eBoardtype, __in const SCREENMAP &scrmap)
{
	UINT uParam[SAFE_BUFF] = {0};

	int nIter = 0;
	uParam[nIter++] = scrmap.iScreenID;
	uParam[nIter++] = scrmap.iOutputCh;
	uParam[nIter++] = scrmap.rcScreenPos.left;
	uParam[nIter++] = scrmap.rcScreenPos.top;
	uParam[nIter++] = scrmap.rcScreenPos.right;
	uParam[nIter++] = scrmap.rcScreenPos.bottom;

	int nRet = NormalASCII_RW("SOPS", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdOPEN(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh, __in const int iSourcetype, __in const CRect rcWnd)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iWndID;
	uParam[nIter++] = iSourceCh;
	uParam[nIter++] = iSourcetype;
	uParam[nIter++] = rcWnd.left;
	uParam[nIter++] = rcWnd.top;
	uParam[nIter++] = rcWnd.right;
	uParam[nIter++] = rcWnd.bottom;

	int nRet = NormalASCII_RW("OPEN", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdMOVE(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh, __in const int iSourcetype, __in const CRect rcWnd)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iWndID;
	uParam[nIter++] = iSourceCh;
	uParam[nIter++] = iSourcetype;
	uParam[nIter++] = rcWnd.left;
	uParam[nIter++] = rcWnd.top;
	uParam[nIter++] = rcWnd.right;
	uParam[nIter++] = rcWnd.bottom;

	int nRet = NormalASCII_RW("MOVE", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdSIZE(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh, __in const int iSourcetype, __in const CRect rcWnd)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iWndID;
	uParam[nIter++] = iSourceCh;
	uParam[nIter++] = iSourcetype;
	uParam[nIter++] = rcWnd.left;
	uParam[nIter++] = rcWnd.top;
	uParam[nIter++] = rcWnd.right;
	uParam[nIter++] = rcWnd.bottom;

	int nRet = NormalASCII_RW("SIZE", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdSHUT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iWndID;

	int nRet = NormalASCII_RW("SHUT", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdMOVZ(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iZorder)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iWndID;
	uParam[nIter++] = iZorder;

	int nRet = NormalASCII_RW("MOVZ", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdMOVS(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iWndID;
	uParam[nIter++] = iSourceCh;

	int nRet = NormalASCII_RW("MOVS", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdSAVE(__in const int iPort, __in const emBoardType eBoardtype, __in const int iSceneID)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iSceneID;

	int nRet = NormalASCII_RW("SAVE", uParam, nIter);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdCALL(__in const int iPort, __in const emBoardType eBoardtype, __in const int iSceneID, __out UINT* &pBuffer, __out int &buffLen, BOOL bAllocBuff)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nIter = 0;
	uParam[nIter++] = iSceneID;

	int nRet = NormalASCII_RW("CALL", uParam, nIter);

	if ((nRet & CMD_RTER) != 0)
		return CMD_FAILURE;

	// 第一个值为场景号
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

int CCommunicateManager::CmdYOSD(__in const int iPort, __in const emBoardType eBoardtype)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;

	uParam[nTick++] = 0x10;

	UINT nRet = NormalASCII_RW("YOSD", uParam, nTick, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdCOSD(__in const int iPort, __in const emBoardType eBoardtype)
{
	UINT nRet = NormalASCII_RW("COSD", NULL, 0, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdRST2(__in const int iPort, __in const emBoardType eBoardtype, __out int &nBrightness, __out int &nContrast)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RST2", uParam, 0, eBoardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nBrightness = uParam[0];
	nContrast = uParam[1];

	return CMD_RTOK;
}

int CCommunicateManager::CmdRPM2(__in const int iPort, __in const emBoardType eBoardtype, __out int &nBrightness, __out int &nContrast)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RPM2", uParam, 0, eBoardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nBrightness = uParam[0];
	nContrast = uParam[1];

	return CMD_RTOK;
}

int CCommunicateManager::CmdWPM2(__in const int iPort, __in const emBoardType eBoardtype, __in const int nBrightness, __in const int nContrast)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	uParam[nTick++] = nBrightness;
	uParam[nTick++] = nContrast;

	UINT nRet = NormalASCII_RW("WPM2", uParam, nTick, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdRST1(__in const int iPort, __in const emBoardType eBoardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RST2", uParam, 0, eBoardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nLeft = uParam[0];
	nTop = uParam[1];
	nWidth = uParam[2];
	nHeight = uParam[3];

	return CMD_RTOK;
}

int CCommunicateManager::CmdATO1(__in const int iPort, __in const emBoardType eBoardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("ATO1", uParam, 0, eBoardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nLeft = uParam[0];
	nTop = uParam[1];
	nWidth = uParam[2];
	nHeight = uParam[3];

	return CMD_RTOK;
}

int CCommunicateManager::CmdRPM1(__in const int iPort, __in const emBoardType eBoardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};

	UINT nRet = NormalASCII_RW("RPM1", uParam, 0, eBoardtype, iPort);

	if ((nRet&CMD_RTER) != 0)
		return CMD_FAILURE;

	nLeft = uParam[0];
	nTop = uParam[1];
	nWidth = uParam[2];
	nHeight = uParam[3];

	return CMD_RTOK;
}

int CCommunicateManager::CmdWPM1(__in const int iPort, __in const emBoardType eBoardtype, __in const int nLeft, __in const int nTop, __in const int nWidth, __in const int nHeight)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	uParam[nTick++] = nLeft;
	uParam[nTick++] = nTop;
	uParam[nTick++] = nWidth;
	uParam[nTick++] = nHeight;

	UINT nRet = NormalASCII_RW("WPM1", uParam, nTick, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

int CCommunicateManager::CmdWICS(__in const int iPort, __in const emBoardType eBoardtype, __in const int nMode)
{
	UINT uParam[SAFE_BUFF] = {0};
	int nTick = 0;
	uParam[nTick++] = nMode;

	UINT nRet = NormalASCII_RW("WICS", uParam, nTick, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}

//
//// 端对端切换
//int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in const int iOutCh, __in emSwitchMode eSM/* =eSwitchModeP2P */)
//{
//	int  nLen = SAFE_BUFF;
//	char cParam[SAFE_BUFF] = {0};
//
//	if (iSubCh == 0)
//		sprintf_s(cParam, SAFE_BUFF, "%d,%d,%d", int(eSM), iFeederCh, iOutCh);
//	else
//		sprintf_s(cParam, SAFE_BUFF, "%d,%d%c,%d", int(eSM), iFeederCh, iSubCh==1 ? 'A' : 'B', iOutCh);
//
//	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eBoardtype, iPort);
//
//	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
//}
//
//// 一对多切换
//int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in int *pOutChList, __in const int iOutChNum, emSwitchMode eSM/* =eSwitchModeOnetoMany */)
//{
//	int  nLen = SAFE_BUFF*4;
//	char cParam[SAFE_BUFF*4] = {0};
//
//	if (iSubCh == 0)
//		sprintf_s(cParam, nLen, "%d,%d", int(eSM), iFeederCh);
//	else
//		sprintf_s(cParam, nLen, "%d,%d%c", int(eSM), iFeederCh, iSubCh==1 ? 'A' : 'B');
//
//	for (int i=0; i<iOutChNum; i++)
//		sprintf_s(cParam, nLen, "%s,%d", cParam, pOutChList[i]);
//
//	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eBoardtype, iPort);
//
//	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
//}
//
//// 一对所有切换
//int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in emSwitchMode eSM/* =eSwitchModeOnetoAll */)
//{
//	int  nLen = SAFE_BUFF;
//	char cParam[SAFE_BUFF] = {0};
//
//	if (iSubCh == 0)
//		sprintf_s(cParam, nLen, "%d,%d", int(eSM), iFeederCh);
//	else
//		sprintf_s(cParam, nLen, "%d,%d%c", int(eSM), iFeederCh, iSubCh==1 ? 'A' : 'B');
//
//	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eBoardtype, iPort);
//
//	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
//}
//
//// 一一对应切换
//int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in emSwitchMode eSM/* =eSwitchModeOnetoOne */)
//{
//	int  nLen = 0;
//	UINT nParam[SAFE_BUFF] = {0};
//	nParam[nLen++] = int(eSM);
//
//	UINT nRet = NormalASCII_RW("SWCH", nParam, nLen, eBoardtype, iPort);
//
//	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
//}
//
//// 映射式切换
//int CCommunicateManager::CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iOutChNum, __in int *pFeederCh, __in emSwitchMode eSM/* =eSwitchModeMapping */)
//{
//	int  nLen = SAFE_BUFF*8;
//	char cParam[SAFE_BUFF*8] = {0};
//
//	sprintf_s(cParam, nLen, "%d,%d", int(eSM), iOutChNum);
//
//	for (int i=0; i<iOutChNum; i++)
//	{
//		if ((pFeederCh[i] & 0xFFFF0000) == SUBPORT_A)
//			sprintf_s(cParam, nLen, "%s,%d%c", cParam, pFeederCh[i]&0xFFFF, 'A');
//		else if ((pFeederCh[i] & 0xFFFF0000) == SUBPORT_B)
//			sprintf_s(cParam, nLen, "%s,%d%c", cParam, pFeederCh[i]&0xFFFF, 'B');
//		else
//			sprintf_s(cParam, nLen, "%s,%d", cParam, pFeederCh[i]&0xFFFF);
//	}
//
//	UINT nRet = NormalASCII_RW("SWCH", cParam, 1, nLen, eBoardtype, iPort);
//
//	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
//}


// 复位
int CCommunicateManager::CmdRSTF(__in const int iPort, __in const emBoardType eBoardtype)
{
	UINT nRet = NormalASCII_RW("RSTF", NULL, 0, eBoardtype, iPort);

	return (nRet==CMD_RTOK ? nRet : CMD_FAILURE);
}