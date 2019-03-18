#pragma once
#include "Communication components/SerialPort.h"
#include "Communication components/EthernetPort.h"

#define TIMER_IDLE 0x5000       //每次发送命令后就开启等待
#define TIMER_KEEP_ALIVE 0x1000 //设备在线检测
#define IDLE_TIME (5000)        //5秒后若程序没有与底层通信，开始发送在线检测
#define ALIVE_INTENAL (2000)    //2秒发送一次在线检测指令
#define MAX_TIMEOUT_NUM (3)     //最大超时次数


#pragma pack(push, 1)
// IP MAC对照表  
typedef struct ipmac  
{  
	char  szIp[16];  
	char  szMac[6];
	void operator=(ipmac &ipmacObj)
	{
		memcpy(szIp, ipmacObj.szIp, 16);
		memcpy(szMac, ipmacObj.szMac, 6);
	}

}IPMAC, *pIPMAC;  

#pragma pack(pop)

//******************************************
// CCommunicateManager
// 通讯事务管理器
//

class CCommunicateManager : public CObject
{
	friend class CGlobalManager;
public:
	CCommunicateManager();
	virtual ~CCommunicateManager();

// 通信控制操作
public:

	/* 是否已连接 */
	static BOOL IsConnected();

	/* 网络连接 */
	BOOL OpenSocket(__in const CString strIP);
	/* 串口连接 */
	BOOL OpenSerial(__in const int iComPort, __in const long iBaudrate=115200);
	/* 关闭连接 */
	void Close();

	/* 获取本地电脑上所有可用的串口号 */
	void GetAllCOMPort(__out CString *pPortList);
	/* 获取域内所有同网段主机地址 */
	void GetDomainHosts(__out pIPMAC hosts, __out int &iHostNum);

	// 通用ASCII码指令接口
	/* < 数字型参数接口 > */
	UINT NormalASCII_RW(__in char *cmd,                      /*指令名*/
                        __in UINT *cmd_para,                 /*参数*/
                        __in UINT len=0,                     /*参数个数*/
                        __in emBoardType eCT=eBoardTypeCtrl, /*指令目标卡类型*/
                        __in UINT port=SYSBOARD_PORT);       /*指令目标卡通信端口号*/

	/* < 字符串型参数接口 > */
	UINT NormalASCII_RW(__in char *cmd,
		                __in char *cmd_para,
						__in int nParamFormat, /*随意填写，用于函数重载区分*/
						__in UINT len=0,
						__in emBoardType eCT=eBoardTypeCtrl,
						__in UINT port=SYSBOARD_PORT);

	/* 在线检测 */
	BOOL OnlineDetect();

// 命令集... 命令含义请查看相关通信指令协议文档。。。
public:
	
	int CmdRMVR(__in const int iPort, __in const emBoardType eBoardtype);
	int CmdRDID(__in const int iPort, __in const emBoardType eBoardtype);
	int CmdRDFN(__in const int iPort, __in const emBoardType eBoardtype);

	int CmdRINT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __out TIMING &stTiming);
	int CmdWINT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __in  TIMING &stTiming);
	int CmdROUT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __out TIMING &stTiming);
	int CmdWOUT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __in  TIMING &stTiming);
	int CmdREDN(__in const int iPort, __in const emBoardType eBoardtype, __in const int iTimingIndex, __out TIMING &stTiming);
	int CmdGETR(__in const int iPort, __in const emBoardType eBoardtype, __out TIMING &sTiming);
	int CmdWEDI(__in const int iPort, __in const emBoardType eBoardtype, __in UINT *pEdidData, __in int iLen);

	int CmdRNET(__in const int iPort, __in const emBoardType eBoardtype, __out NETPARAMS &np);
	int CmdWNET(__in const int iPort, __in const emBoardType eBoardtype, __in const NETPARAMS &np);

	int CmdImageTest(__in const int iPort, __in const emBoardType eBoardtype, __in const pIMGTESTINFO pImgTestInfo);

	int CmdRCN1(__in const int iPort, __in const emBoardType eBoardtype, __out pSPLICELAYOUT pLayout);
	int CmdWCN1(__in const int iPort, __in const emBoardType eBoardtype, __in const SPLICELAYOUT &Layout, __in const int iVideoType=1);

	int CmdGCMG(__in const int iPort, __in const emBoardType eBoardtype, __out UINT &iInputNum, __out UINT &iOutputNum, __out UINT* pInputFlags, __out UINT* pOutputFlags);
	int CmdRPM3(__in const int iPort, __in const emBoardType eBoardtype, __out int &iSignalType);

	int CmdGOPS(__in const int iPort, __in const emBoardType eBoardtype, __out SCREENMAP &scrmap);
	int CmdSOPS(__in const int iPort, __in const emBoardType eBoardtype, __in const SCREENMAP &scrmap);

	int CmdOPEN(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh, __in const int iSourcetype, __in const CRect rcWnd);
	int CmdMOVE(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh, __in const int iSourcetype, __in const CRect rcWnd);
	int CmdSIZE(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh, __in const int iSourcetype, __in const CRect rcWnd);
	int CmdSHUT(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID);
	int CmdMOVZ(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iZorder);
	int CmdMOVS(__in const int iPort, __in const emBoardType eBoardtype, __in const int iWndID, __in const int iSourceCh);

	int CmdSAVE(__in const int iPort, __in const emBoardType eBoardtype, __in const int iSceneID);
	int CmdCALL(__in const int iPort, __in const emBoardType eBoardtype, __in const int iSceneID, __out UINT* &pBuffer, __out int &buffLen, BOOL bAllocBuff=FALSE);

	int CmdYOSD(__in const int iPort, __in const emBoardType eBoardtype);
	int CmdCOSD(__in const int iPort, __in const emBoardType eBoardtype);

	int CmdRST2(__in const int iPort, __in const emBoardType eBoardtype, __out int &nBrightness, __out int &nContrast);
	int CmdRPM2(__in const int iPort, __in const emBoardType eBoardtype, __out int &nBrightness, __out int &nContrast);
	int CmdWPM2(__in const int iPort, __in const emBoardType eBoardtype, __in const int nBrightness, __in const int nContrast);
	int CmdRST1(__in const int iPort, __in const emBoardType eBoardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight);
	int CmdATO1(__in const int iPort, __in const emBoardType eBoardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight);
	int CmdRPM1(__in const int iPort, __in const emBoardType eBoardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight);
	int CmdWPM1(__in const int iPort, __in const emBoardType eBoardtype, __in const int nLeft, __in const int nTop, __in const int nWidth, __in const int nHeight);
	int CmdWICS(__in const int iPort, __in const emBoardType eBoardtype, __in const int nMode);

	/* 切换方式 */
	//enum emSwitchMode
	//{
	//	eSwitchModeP2P,          //单对单切换 
	//	eSwitchModeOnetoMany,    //一对多切换
	//	eSwitchModeOnetoAll,     //一对所有切换
	//	eSwitchModeOnetoOne,     //一一对应切换
	//	eSwitchModeMapping,      //映射模式 一系列键值对
	//};

	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in const int iOutCh, __in emSwitchMode eSM=eSwitchModeP2P);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in int *pOutChList, __in const int iOutChNum, emSwitchMode eSM=eSwitchModeOnetoMany);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in emSwitchMode eSM=eSwitchModeOnetoAll);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in emSwitchMode eSM=eSwitchModeOnetoOne);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iOutChNum, __in int *pFeederCh, __in emSwitchMode eSM=eSwitchModeMapping);

	int CmdRSTF(__in const int iPort, __in const emBoardType eBoardtype);

// 成员变量
private:
	CSerialPort m_Serial;
	CEthernetPort m_Sock;
	static int m_sConnType;
};

// 全局通信事务管理器
extern CCommunicateManager* pCmdManager;