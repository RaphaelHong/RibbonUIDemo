#pragma once
#include "Communication components/SerialPort.h"
#include "Communication components/EthernetPort.h"

#define TIMER_IDLE 0x5000       //ÿ�η��������Ϳ����ȴ�
#define TIMER_KEEP_ALIVE 0x1000 //�豸���߼��
#define IDLE_TIME (5000)        //5���������û����ײ�ͨ�ţ���ʼ�������߼��
#define ALIVE_INTENAL (2000)    //2�뷢��һ�����߼��ָ��
#define MAX_TIMEOUT_NUM (3)     //���ʱ����


#pragma pack(push, 1)
// IP MAC���ձ�  
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
// ͨѶ���������
//

class CCommunicateManager : public CObject
{
	friend class CGlobalManager;
public:
	CCommunicateManager();
	virtual ~CCommunicateManager();

// ͨ�ſ��Ʋ���
public:

	/* �Ƿ������� */
	static BOOL IsConnected();

	/* �������� */
	BOOL OpenSocket(__in const CString strIP);
	/* �������� */
	BOOL OpenSerial(__in const int iComPort, __in const long iBaudrate=115200);
	/* �ر����� */
	void Close();

	/* ��ȡ���ص��������п��õĴ��ں� */
	void GetAllCOMPort(__out CString *pPortList);
	/* ��ȡ��������ͬ����������ַ */
	void GetDomainHosts(__out pIPMAC hosts, __out int &iHostNum);

	// ͨ��ASCII��ָ��ӿ�
	/* < �����Ͳ����ӿ� > */
	UINT NormalASCII_RW(__in char *cmd,                      /*ָ����*/
                        __in UINT *cmd_para,                 /*����*/
                        __in UINT len=0,                     /*��������*/
                        __in emBoardType eCT=eBoardTypeCtrl, /*ָ��Ŀ�꿨����*/
                        __in UINT port=SYSBOARD_PORT);       /*ָ��Ŀ�꿨ͨ�Ŷ˿ں�*/

	/* < �ַ����Ͳ����ӿ� > */
	UINT NormalASCII_RW(__in char *cmd,
		                __in char *cmd_para,
						__in int nParamFormat, /*������д�����ں�����������*/
						__in UINT len=0,
						__in emBoardType eCT=eBoardTypeCtrl,
						__in UINT port=SYSBOARD_PORT);

	/* ���߼�� */
	BOOL OnlineDetect();

// ���... �������鿴���ͨ��ָ��Э���ĵ�������
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

	/* �л���ʽ */
	//enum emSwitchMode
	//{
	//	eSwitchModeP2P,          //���Ե��л� 
	//	eSwitchModeOnetoMany,    //һ�Զ��л�
	//	eSwitchModeOnetoAll,     //һ�������л�
	//	eSwitchModeOnetoOne,     //һһ��Ӧ�л�
	//	eSwitchModeMapping,      //ӳ��ģʽ һϵ�м�ֵ��
	//};

	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in const int iOutCh, __in emSwitchMode eSM=eSwitchModeP2P);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in int *pOutChList, __in const int iOutChNum, emSwitchMode eSM=eSwitchModeOnetoMany);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iFeederCh, __in const int iSubCh, __in emSwitchMode eSM=eSwitchModeOnetoAll);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in emSwitchMode eSM=eSwitchModeOnetoOne);
	//int CmdSWCH(__in const int iPort, __in const emBoardType eBoardtype, __in const int iOutChNum, __in int *pFeederCh, __in emSwitchMode eSM=eSwitchModeMapping);

	int CmdRSTF(__in const int iPort, __in const emBoardType eBoardtype);

// ��Ա����
private:
	CSerialPort m_Serial;
	CEthernetPort m_Sock;
	static int m_sConnType;
};

// ȫ��ͨ�����������
extern CCommunicateManager* pCmdManager;