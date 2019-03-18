#pragma once
#include "Communication components/SerialPort.h"
#include "Communication components/EthernetPort.h"

#define TIMER_IDLE 0x5000  //ÿ�η��������Ϳ����ȴ�
#define TIMER_KEEP_ALIVE 0x1000 //�豸���߼��
#define IDLE_TIME (5000)  //5���������û����ײ�ͨ�ţ���ʼ�������߼��
#define ALIVE_INTENAL (2000) //2�뷢��һ�����߼��ָ��
#define MAX_TIMEOUT_NUM (3) //���ʱ����

#pragma pack(push, 1)
// IP MAC���ձ�  
typedef struct ipmac  
{  
	char  szIp[16];  
	char  szMac[6];
	void operator=(ipmac &IPMACobj)
	{
		memcpy(szIp, IPMACobj.szIp, 16);
		memcpy(szMac, IPMACobj.szMac, 6);
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
	UINT NormalASCII_RW(__in char *cmd,                    /*ָ����*/
                        __in UINT *cmd_para,               /*����*/
                        __in UINT len=0,                   /*��������*/
                        __in emCardType eCT=eCardTypeCtrl, /*ָ��Ŀ�꿨����*/
                        __in UINT port=SYSCARD_PORT);      /*ָ��Ŀ�꿨ͨ�Ŷ˿ں�*/

	/* < �ַ����Ͳ����ӿ� > */
	UINT NormalASCII_RW(__in char *cmd,
		                __in char *cmd_para,
						__in int nParamFormat, /*������д�����ں�����������*/
						__in UINT len=0,
						__in emCardType eCT=eCardTypeCtrl,
						__in UINT port=SYSCARD_PORT);

	/* ���߼�� */
	BOOL OnlineDetect();

	/* ͨ����Դ�� */
	void CommunicationLock() { m_lock.Lock(); }
	void CommunicationUnlock() { m_lock.Unlock(); }

// ���... �������鿴���ͨ��ָ��Э���ĵ�������
public:
	
	int CmdRMVR(__in const int iPort, __in const emCardType eCardtype);
	int CmdRDID(__in const int iPort, __in const emCardType eCardtype);
	int CmdRDFN(__in const int iPort, __in const emCardType eCardtype);

	int CmdRINT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __out TIMING &sTiming);
	int CmdWINT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __in  TIMING &sTiming);
	int CmdROUT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __out TIMING &sTiming);
	int CmdWOUT(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __in  TIMING &sTiming);
	int CmdREDN(__in const int iPort, __in const emCardType eCardtype, __in const int iTimingIndex, __out TIMING &sTiming);
	int CmdGETR(__in const int iPort, __in const emCardType eCardtype, __out TIMING &sTiming);
	int CmdWEDI(__in const int iPort, __in const emCardType eCardtype, __in UINT *pEdidData, __in int iLen);

	int CmdRNET(__in const int iPort, __in const emCardType eCardtype, __out NETPARAMS &np);
	int CmdWNET(__in const int iPort, __in const emCardType eCardtype, __in const NETPARAMS &np);

	int CmdImageTest(__in const int iPort, __in const emCardType eCardtype, __in const pIMGTESTINFO pImgTestInfo);

	int CmdGCMG(__in const int iPort, __in const emCardType eCardtype, __out UINT &iInputNum, __out UINT &iOutputNum, __out UINT* pInputFlags, __out UINT* pOutputFlags);
	int CmdRPM3(__in const int iPort, __in const emCardType eCardtype, __out int &iSignalType);

	int CmdSAVE(__in const int iPort, __in const emCardType eCardtype, __in const int iSceneID);
	int CmdCALL(__in const int iPort, __in const emCardType eCardtype, __in const int iSceneID, __out UINT* &pBuffer, __out int &buffLen, BOOL bAllocBuff=FALSE);

	int CmdYOSD(__in const int iPort, __in const emCardType eCardtype);
	int CmdCOSD(__in const int iPort, __in const emCardType eCardtype);

	int CmdRST2(__in const int iPort, __in const emCardType eCardtype, __out int &nBrightness, __out int &nContrast);
	int CmdRPM2(__in const int iPort, __in const emCardType eCardtype, __out int &nBrightness, __out int &nContrast);
	int CmdWPM2(__in const int iPort, __in const emCardType eCardtype, __in const int nBrightness, __in const int nContrast);
	int CmdRST1(__in const int iPort, __in const emCardType eCardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight);
	int CmdATO1(__in const int iPort, __in const emCardType eCardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight);
	int CmdRPM1(__in const int iPort, __in const emCardType eCardtype, __out int &nLeft, __out int &nTop, __out int &nWidth, __out int &nHeight);
	int CmdWPM1(__in const int iPort, __in const emCardType eCardtype, __in const int nLeft, __in const int nTop, __in const int nWidth, __in const int nHeight);
	int CmdWICS(__in const int iPort, __in const emCardType eCardtype, __in const int nMode);

	/* �л���ʽ */
	enum emSwitchMode
	{
		eSwitchModeP2P,          //���Ե��л� 
		eSwitchModeOnetoMany,    //һ�Զ��л�
		eSwitchModeOnetoAll,     //һ�������л�
		eSwitchModeOnetoOne,     //һһ��Ӧ�л�
		eSwitchModeMapping,      //ӳ��ģʽ һϵ�м�ֵ��
	};

	int CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iFeederCh, __in const int iSubCh, __in const int iOutCh, __in emSwitchMode eSM=eSwitchModeP2P);
	int CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iFeederCh, __in const int iSubCh, __in int *pOutChList, __in const int iOutChNum, emSwitchMode eSM=eSwitchModeOnetoMany);
	int CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iFeederCh, __in const int iSubCh, __in emSwitchMode eSM=eSwitchModeOnetoAll);
	int CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in emSwitchMode eSM=eSwitchModeOnetoOne);
	int CmdSWCH(__in const int iPort, __in const emCardType eCardtype, __in const int iOutChNum, __in int *pFeederCh, __in emSwitchMode eSM=eSwitchModeMapping);

	int CmdRSTF(__in const int iPort, __in const emCardType eCardtype);

// ��Ա����
private:
	CSerialPort m_Serial;
	CEthernetPort m_Sock;
	static int m_sConnType;
	CCriticalSection m_lock;
};

// ȫ��ͨ�����������
extern CCommunicateManager* pCmdManager;