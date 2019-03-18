// MVintergrationController.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CommandClass.h"


extern int g_AjustThreadWorkMode;

// tagSharedPackage �ṹ��
// ���ã����̺߳�У���߳���������������
#define KEYSTONE_AJUST    0
#define CURVE_AJUST       1
#define DENSITY_AJUST     2
#define GRIDWARP_AJUST    3

#define AJUST_THREAD      0
#define PROGRESS_THREAD   1
typedef struct tagSharedPackage
{
	CWnd *pPaintWnd;                //��ͼ����
	int  nBoardPort;                //��ǰ�忨��ͨ�Ŷ˿�
	int  nAjustMode;                //0:����  1:���� 2:���� 3:���
	int  nGridSmooth;               //�������Ƿ�ƽ������
	int  cx;                        //����������
	int  cy;                        //����������
	int  cxResIn;                   //ˮƽ����ֵ
	int  cyResIn;                   //��ֱ����ֵ
	int  cxScaleIn;                 //����������ű� ˮƽ���� input_hactive/output_hactive
	int  cyScaleIn;                 //����������ű� ��ֱ���� input_vactive/output_vactive
	const CDoublePointArray *pDpaOrigData;     //ԭʼ���ݵ�ַ
	const CDoublePointArray *pDpa4pointData;   //�ĵ�У�����ݵ�ַ
	const CDoublePointArray *pDpaCurveData;    //����У�����ݵ�ַ
	const CDoublePointArray *pDpaDensityData;  //����У�����ݵ�ַ
	const CDoublePointArray *pDpaComplexData;  //���У�����ݵ�ַ
	tagSharedPackage() {
		cxScaleIn       = 1;
		cyScaleIn       = 1;
		pPaintWnd       = NULL;
		pDpaOrigData    = NULL;
		pDpa4pointData  = NULL;
		pDpaCurveData   = NULL;
		pDpaDensityData = NULL;
		pDpaComplexData = NULL;
	}
	bool operator== (const tagSharedPackage &Packobj) {
		return (nBoardPort == Packobj.nBoardPort
			 && nAjustMode == Packobj.nAjustMode
			 && cx == Packobj.cx
			 && cy == Packobj.cy
			 && cxResIn == Packobj.cxResIn
			 && cyResIn == Packobj.cyResIn);
	}

} TaskPackage, *pTaskPackage;

typedef struct stDialogSetting
{
	CBrush bkBrush;
	COLORREF clrText;
} DialogSetting, *pDialogSetting;

// CMVintergrationControllerApp:
class CMVintergrationControllerApp : public CWinApp
{
public:
	CMVintergrationControllerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

	//ͨ�õ����Ի��򱳾�ɫ���ı�ɫ����
	DialogSetting m_DlgSetting;

	//Log File Path
	CString m_strLogFilePath;
	void SetLogFilePath();
	void PrintLog(char* szInfo);
	//Track Popup Menu
	void MvTrackPopupMenu(CPoint ptTrack, UINT nResource, CWnd *pParent);

	//GdiPlus Tokens
	ULONG_PTR m_gdiplusToken;


public:
	CEthernetPort m_sock;           //����ӿ�
	CSerialPort m_Com;              //���ڽӿ�
	CUSBHIDPort m_USBport;             //USB�ӿ�
	int m_nConnType;                //���ӷ�ʽ
	emProductType m_CurrentProductType; //��ǰҪ���ɵĹ���ҳ����

	void SetCmdInterface(CEthernetPort *pSock, CSerialPort*pCom, CUSBHIDPort *pUSB);  /*��ȡͨ�Žӿ�ָ��*/

	//**********************************************************//
	//                         ϵͳ��֤
	//----------------------------------------------------------//
	//��ȡ����ID
	int GetManufacture(int nPort=0xff)    
	{
		return m_Cmd.CmdRMVR(nPort);
	}
	 //��ȡ�豸ID
	int GetDeviceID(int nPort=0xff)      
	{
		return m_Cmd.CmdRDID(nPort);
	}
	//��ȡ�忨������
	int GetBoardFuncCode(int nPort=0xff)  
	{
		return m_Cmd.CmdRDFN(nPort);
	}
	//��ȡ���а忨���忨�˿ں�
	int GetAllBoards(int &nBoardNum, int *pPortID, int nPort=0xff)  
	{
		return m_Cmd.CmdRDPT(nBoardNum, pPortID, nPort);
	}

	//**********************************************************//
	//                        �ֱ�������
	//----------------------------------------------------------//
	//��ȡ����ֱ���
	int GetInputTiming(pTIMING_STRUCT pTsR, int nEdidport, int nTimingIndex, int nPort) 
	{
		return m_Cmd.CmdRINT(pTsR, nEdidport, nTimingIndex, nPort);
	}
	//��������ֱ���
	int SetInputTiming(pTIMING_STRUCT pTsW, int nEdidport, int nTimingIndex, int nPort) 
	{
		return m_Cmd.CmdWINT(pTsW, nEdidport, nTimingIndex, nPort);
	}
	//��ȡ��ʱ����ֱ���
	int GetCurrencyInputTiming(UINT &nHz, UINT &nHtotal, UINT &nHactive, UINT &nVtatol, UINT &nVactive, int nPort)
	{
		return m_Cmd.CmdGETR(nHz, nHtotal, nHactive, nVtatol, nVactive, nPort);
	}
	//��ȡ����ֱ���
	int GetOutputTiming(TIMING_STRUCT *pTsR, int nPort)
	{
		return m_Cmd.CmdROUT(pTsR, nPort);
	}
	//��������ֱ���
	int SetOutputTiming(TIMING_STRUCT *pTsW, int nPort)
	{
		return m_Cmd.CmdWOUT(pTsW, nPort);
	}


	//**********************************************************//
	//                        �����������
	//----------------------------------------------------------//
	//��ȡ�������
	int GetNetParams(pNetParams netparamR, int nPort=0xff) 
	{
		return m_Cmd.CmdRNET(netparamR, nPort);
	}
	//�����������
	int SetNetParams(pNetParams netparamW, int nPort=0xff) 
	{
		return m_Cmd.CmdWNET(netparamW, nPort);
	}

	//**********************************************************//
	//                        ɫ��У��
	//----------------------------------------------------------//
	//������ɫ
	int MG_ColorTest(pColorTestParams clrtestparam, int nPort)                 
	{
		return m_Cmd.CmdTCLR(clrtestparam, nPort);
	}
	//ȡ����ɫ����
	int MG_CancelColorTest(int nPort)                                          
	{
		return m_Cmd.CmdCCLR(nPort);
	}
	//д����ɫ��
	int MG_WriteColor(UCHAR *pColorData, int nDataLen, int nColortype, int nPort)
	{
		return m_Cmd.CmdWCRA_BIN(pColorData, nDataLen, nColortype, nPort);
	}
	//��ȡɫ������
	int MG_ReadColor(UCHAR *pColorData, int nColortype, int nPort)                             
	{
		return m_Cmd.CmdRCRA_BIN(pColorData, nColortype, nPort);
	}
	 //��λɫ��ֵ
	int MG_ResetColor(UINT nColorsel, int nPort)                              
	{
		return m_Cmd.CmdRSTC(nColorsel, nPort);
	}
	//ȥ�ںϴ�Ӱ��
	int MG_CancelBlending(UINT nBlendOpt, int nPort)                           
	{
		return m_Cmd.CmdCRBD(nBlendOpt, nPort);
	}

	//**********************************************************//
	//                        �ںϴ�
	//----------------------------------------------------------//
	//�����ں�Ч��
	int MG_TestBlendParams(pBlendParams pBPW, int nPort)    
	{
		return m_Cmd.CmdWBDP(pBPW, nPort);
	}
	//��ȡ�ںϲ���
	int MG_ReadBlendParams(UINT *pBlendParam, int nPort)    
	{
		return m_Cmd.CmdRBDP(pBlendParam, nPort);
	}
	//����/�ر��ںϹ���
	int MG_EnableBlend(UINT nEnableW, int nPort)            
	{
		return m_Cmd.CmdWBDE(nEnableW, nPort);
	}
	//��ȡ�ںϹ����Ƿ���
	int MG_GetBlendEnable(UINT &nEnableR, int nPort)        
	{
		return m_Cmd.CmdRBDE(nEnableR, nPort);
	}
	//�����ںϴ����
	int MG_SetBlendWidth(UINT *pWidthW, int nPort)          
	{
		return m_Cmd.CmdWBDW(pWidthW, nPort);
	}
	//��ȡ�ںϴ����
	int MG_GetBlendWidth(UINT *pWidthR, int nPort)          
	{
		return m_Cmd.CmdRBDW(pWidthR, nPort);
	}
	//���ô��׷�ֵ
	int MG_SetWhiteThreshold(UINT nWhiteLvlW, int nPort)    
	{
		return m_Cmd.CmdWBDT(nWhiteLvlW, nPort);
	}
	//��ȡ���׷�ֵ
	int MG_GetWhiteThreshold(UINT &nWhiteLvlR, int nPort)   
	{
		return m_Cmd.CmdRBDT(nWhiteLvlR, nPort);
	}
	//��ȡ�ں�ֵ
	int MG_ReadblendParamDatas(UINT *pDatatoRead/*���ݵ�ַ*/,
		                    int nBlendside,
							int nPureWhite,
							int nBlendWidth,
							int nPort) 
	{
		return m_Cmd.CmdRBPD(pDatatoRead, nBlendside, nPureWhite, nBlendWidth, nPort);
	}
	//д���ں�ֵ
	int MG_WriteBlendParamDatas(UINT *pDatatoWrite/*���ݵ�ַ*/,
		                     int nBegin/*д����ʼλ��*/,
							 int nEnd/*д�����λ��*/,
							 int nBlendside,
							 int nPureWhite,
							 int nPort) 
	{
		return m_Cmd.CmdWBPD(pDatatoWrite, nBegin, nEnd, nBlendside, nPureWhite, nPort);
	}

	//**********************************************************//
	//                        �ڵ�ƽ
	//----------------------------------------------------------//
	//�����ڵ�ƽ���Ի���
	int MG_EnableBlacklvlTest(UINT nBlackLvl, UINT nEnable, int nPort)   
	{
		return m_Cmd.CmdTCRY(nBlackLvl, nEnable, nPort);
	}
	//д��ڵ�ƽ
	int MG_WriteBlackLvl(UINT *pBlackLvlDataW, UINT nSegment, int nPort) 
	{
		return m_Cmd.CmdWBLV(pBlackLvlDataW, nSegment, nPort);
	}
	//��ȡ�ڵ�ƽ
	int MG_ReadBlackLvl(UINT *pBlackLvlDataR, UINT nSegment, int nPort)  
	{
		return m_Cmd.CmdRBLV(pBlackLvlDataR, nSegment, nPort);
	}
	//����©������
	int MG_SetLeakageWidth(UINT nLeakageSel, UINT nSide1Width, UINT nSide2Width, int nPort) 
	{
		return m_Cmd.CmdWLKT(nLeakageSel, nSide1Width, nSide2Width, nPort);
	}
	//��ȡ©������
	int MG_GetLeakageWidth(UINT *pLeakageWidth, int nPort)      
	{
		return m_Cmd.CmdRLKT(pLeakageWidth, nPort);
	}
	//���úڵ�ƽ��ֵ
	int MG_SetBlackLvlTickPos(UINT *pBlkLvlSegmentW, int nPort) 
	{
		return m_Cmd.CmdWLVT(pBlkLvlSegmentW, nPort);
	}
	//��ȡ�ڵ�ƽ��ֵ
	int MG_GetBlackLvlTickPos(UINT *pBlkLvlSegmentR, int nPort) 
	{
		return m_Cmd.CmdRLVT(pBlkLvlSegmentR, nPort);
	}
	//�����رպڵ�ƽ����
	int MG_EnableBlackLevelFunc(UINT nEnable, int nPort)        
	{
		return m_Cmd.CmdWBLE(nEnable, nPort);
	}
	//��ȡ�ڵ�ƽ���ܿ������
	int MG_GetBlackLevelFuncEnable(UINT &nEnable, int nPort)    
	{
		return m_Cmd.CmdRBLE(nEnable, nPort);
	}
	//��λ�ڵ�ƽ
	int MG_ResetBlackLvl(int nPort)
	{
		return m_Cmd.CmdRSTL(nPort);
	}

	//**********************************************************//
	//                        ����У��
	//----------------------------------------------------------//
	//���浽WarpTable
	int MG_SaveWarpTable(UINT nTableID, int nPort)
	{
		return m_Cmd.CmdSWPT(nTableID, nPort);
	}
	//��ȡ�ĵ�У���ĸ����λ��
	int MG_Get4PointPosition(UINT *p4PointPos, int nPort)  
	{
		return m_Cmd.CmdGW4P(p4PointPos, nPort);
	}
	//��ȡ����У���Ÿ����λ��
	int MG_GetCurvePostion(UINT *pCurvePos, int nPort)     
	{
		return m_Cmd.CmdGW9P(pCurvePos, nPort);
	}
	//��ȡ����У���Ÿ����λ��
	int MG_GetDensityPosition(UINT *pDensityPos, int nPort)
	{
		return m_Cmd.CmdGW5P(pDensityPos, nPort);
	}
	//�����ĵ�У��
	int MG_Save4PointPosition(UINT *p4Points, int nPort)
	{
		return m_Cmd.CmdSW4P(p4Points, nPort);
	}
	//��������У��
	int MG_SaveCurvePostion(UINT *pCurvePos, int nPort)     
	{
		return m_Cmd.CmdSW9P(pCurvePos, nPort);
	}
	//��������У��
	int MG_SaveDensityPosition(UINT *pDensityPos, int nPort)
	{
		return m_Cmd.CmdSW5P(pDensityPos, nPort);
	}

	//**********************************************************//
	//                        ͼ������ۻ�
	//----------------------------------------------------------//
	//��ȡ�ۻ�ֵ
	int MG_GetFilterValue(UINT &nFilterR, int nPort)
	{
		return m_Cmd.CmdRFLI(nFilterR, nPort);
	}
	//���öۻ�ֵ
	int MG_SetFilterValue(UINT nFilterW, int nPort)
	{
		return m_Cmd.CmdWFLI(nFilterW, nPort);
	}

	//**********************************************************//
	//                        ��λ
	//----------------------------------------------------------//
	int ResetBoards(int nPort)  //�忨��λ
	{
		return m_Cmd.CmdRSTF(nPort);
	}

	//**********************************************************//
	//                        �̻�
	//----------------------------------------------------------//
	int SaveAll(int nPort)  //�̻�����
	{
		return m_Cmd.CmdSALL(nPort);
	}


	//**********************************************************//
	//                        ����ָ��
	//----------------------------------------------------------//
	//��ȡ����
	int MVX_ReadBackgrdInfo(UINT *pSignalInfo, int nPort)
	{
		return m_Cmd.CmdRCN1(pSignalInfo, nPort);
	}
	//�½�����
	int MVX_OpenNewWindow(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
	{
		return m_Cmd.CmdOPEN(nWinID, rcWnd, nSrcID, nSrcType, nPort);
	}
	//�رմ���
	int MVX_ShutdownWindow(int nWinID, int nPort)
	{
		return m_Cmd.CmdSHUT(nWinID, nPort);
	}
	//�ƶ�����
	int MVX_MoveTheWindow(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
	{
		return m_Cmd.CmdMove(nWinID, rcWnd, nSrcID, nSrcType, nPort);
	}
	//�������ڴ�С
	int MVX_ResizeWindow(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
	{
		return m_Cmd.CmdSize(nWinID, rcWnd, nSrcID, nSrcType, nPort);
	}
	//���ó���
	int MVX_CallWindows(UINT *pReadBuf, int nSceneID, int nPort)
	{
		return m_Cmd.CmdCALL(pReadBuf, nSceneID, nPort);
	}
	//���泡��
	int MVX_SaveWindows(UINT *pWriteBuf, int nBufftoWritten, int nSceneID, int nPort)
	{
		return m_Cmd.CmdSAVE(pWriteBuf, nBufftoWritten, nSceneID, nPort);
	}

	//**********************************************************//
	//                       HDMI_3D ģ��ָ��
	//----------------------------------------------------------//
	//��ȡ����3Dģʽ
	int HDMI_GetInput3Dmode(int &n3Dmode, int nPort)
	{
		return m_Cmd.CmdRI3D(n3Dmode, nPort);
	}
	//�������3Dģʽ
	int HDMI_SetInput3Dmode(int n3Dmode, int nPort)
	{
		return m_Cmd.CmdWI3D(n3Dmode, nPort);
	}
	//��ȡ���3D����ģʽ
	int HDMI_GetOutput3Dmode(int &n3Dmode, int nPort)
	{
		return m_Cmd.CmdRO3D(n3Dmode, nPort);
	}
	//�������3D����ģʽ
	int HDMI_SetOutput3Dmode(int n3Dmode, int nPort)
	{
		return m_Cmd.CmdWO3D(n3Dmode, nPort);
	}
	//��ȡ��Ƶ����
	int HDMI_GetOctave(int &nOctave, int nPort)
	{
		return m_Cmd.CmdROHZ(nOctave, nPort);
	}
	//���ó�Ƶ����
	int HDMI_SetOctave(int nOctave, int nPort)
	{
		return m_Cmd.CmdWOHZ(nOctave, nPort);
	}
	//�������л�
	int HDMI_SetLeftRightEye(int nPort)
	{
		return m_Cmd.CmdWLRC(nPort);
	}


public:
	//�߳̿�����
	CWinThread *m_ThreadAjustHandle; //����У���߳̾��
	BOOL StartThread(UINT nThreadID = 0); //0:��ʾУ���߳� 1����ʾ�������߳�
	void ResumeThread(UINT nThreadID = 0);
	void SuspendThread(UINT nThreadID = 0);
	void ExitThread(UINT nThreadID = 0);	

	CCriticalSection m_Lock; //������Դ��
	void GlobleLock(){ m_Lock.Lock(); }         //����������Դ
	void GlobleUnlock(){ m_Lock.Unlock(); }     //����������Դ

private:
	CList <TaskPackage, TaskPackage&> m_ListTaskPack;                      //�߳��������ݰ�����

public:
	//int g_TaskNum;
	void  PushPackage(TaskPackage pack);  //���������
	TaskPackage  PopPackage();            //��������
	BOOL  IsTaskListEmpty();              //����������Ƿ�Ϊ��
	int   GetTaskCount() const;           //�������������
	void  WaitForTaskOver();              //�ȴ��߳�ִ�����

public:
	CCommandClass m_Cmd;
 
// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMVintergrationControllerApp theApp;
