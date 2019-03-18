// MVintergrationController.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "CommandClass.h"


extern int g_AjustThreadWorkMode;

// tagSharedPackage 结构体
// 作用：主线程和校正线程用来传递数据用
#define KEYSTONE_AJUST    0
#define CURVE_AJUST       1
#define DENSITY_AJUST     2
#define GRIDWARP_AJUST    3

#define AJUST_THREAD      0
#define PROGRESS_THREAD   1
typedef struct tagSharedPackage
{
	CWnd *pPaintWnd;                //绘图窗口
	int  nBoardPort;                //当前板卡的通信端口
	int  nAjustMode;                //0:梯形  1:曲面 2:疏密 3:多点
	int  nGridSmooth;               //多点较正是否平滑处理
	int  cx;                        //纵向网格线
	int  cy;                        //横向网格线
	int  cxResIn;                   //水平像素值
	int  cyResIn;                   //垂直像素值
	int  cxScaleIn;                 //输入输出缩放比 水平方向 input_hactive/output_hactive
	int  cyScaleIn;                 //输入输出缩放比 垂直方向 input_vactive/output_vactive
	const CDoublePointArray *pDpaOrigData;     //原始数据地址
	const CDoublePointArray *pDpa4pointData;   //四点校正数据地址
	const CDoublePointArray *pDpaCurveData;    //曲面校正数据地址
	const CDoublePointArray *pDpaDensityData;  //疏密校正数据地址
	const CDoublePointArray *pDpaComplexData;  //多点校正数据地址
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

	//通用弹出对话框背景色和文本色设置
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
	CEthernetPort m_sock;           //网络接口
	CSerialPort m_Com;              //串口接口
	CUSBHIDPort m_USBport;             //USB接口
	int m_nConnType;                //连接方式
	emProductType m_CurrentProductType; //当前要生成的功能页类型

	void SetCmdInterface(CEthernetPort *pSock, CSerialPort*pCom, CUSBHIDPort *pUSB);  /*获取通信接口指针*/

	//**********************************************************//
	//                         系统验证
	//----------------------------------------------------------//
	//获取厂商ID
	int GetManufacture(int nPort=0xff)    
	{
		return m_Cmd.CmdRMVR(nPort);
	}
	 //获取设备ID
	int GetDeviceID(int nPort=0xff)      
	{
		return m_Cmd.CmdRDID(nPort);
	}
	//获取板卡功能码
	int GetBoardFuncCode(int nPort=0xff)  
	{
		return m_Cmd.CmdRDFN(nPort);
	}
	//获取所有板卡及板卡端口号
	int GetAllBoards(int &nBoardNum, int *pPortID, int nPort=0xff)  
	{
		return m_Cmd.CmdRDPT(nBoardNum, pPortID, nPort);
	}

	//**********************************************************//
	//                        分辨率设置
	//----------------------------------------------------------//
	//获取输入分辨率
	int GetInputTiming(pTIMING_STRUCT pTsR, int nEdidport, int nTimingIndex, int nPort) 
	{
		return m_Cmd.CmdRINT(pTsR, nEdidport, nTimingIndex, nPort);
	}
	//设置输入分辨率
	int SetInputTiming(pTIMING_STRUCT pTsW, int nEdidport, int nTimingIndex, int nPort) 
	{
		return m_Cmd.CmdWINT(pTsW, nEdidport, nTimingIndex, nPort);
	}
	//获取即时输入分辨率
	int GetCurrencyInputTiming(UINT &nHz, UINT &nHtotal, UINT &nHactive, UINT &nVtatol, UINT &nVactive, int nPort)
	{
		return m_Cmd.CmdGETR(nHz, nHtotal, nHactive, nVtatol, nVactive, nPort);
	}
	//获取输出分辨率
	int GetOutputTiming(TIMING_STRUCT *pTsR, int nPort)
	{
		return m_Cmd.CmdROUT(pTsR, nPort);
	}
	//设置输出分辨率
	int SetOutputTiming(TIMING_STRUCT *pTsW, int nPort)
	{
		return m_Cmd.CmdWOUT(pTsW, nPort);
	}


	//**********************************************************//
	//                        网络参数设置
	//----------------------------------------------------------//
	//获取网络参数
	int GetNetParams(pNetParams netparamR, int nPort=0xff) 
	{
		return m_Cmd.CmdRNET(netparamR, nPort);
	}
	//设置网络参数
	int SetNetParams(pNetParams netparamW, int nPort=0xff) 
	{
		return m_Cmd.CmdWNET(netparamW, nPort);
	}

	//**********************************************************//
	//                        色差校正
	//----------------------------------------------------------//
	//测试颜色
	int MG_ColorTest(pColorTestParams clrtestparam, int nPort)                 
	{
		return m_Cmd.CmdTCLR(clrtestparam, nPort);
	}
	//取消颜色测试
	int MG_CancelColorTest(int nPort)                                          
	{
		return m_Cmd.CmdCCLR(nPort);
	}
	//写入颜色表
	int MG_WriteColor(UCHAR *pColorData, int nDataLen, int nColortype, int nPort)
	{
		return m_Cmd.CmdWCRA_BIN(pColorData, nDataLen, nColortype, nPort);
	}
	//读取色差数据
	int MG_ReadColor(UCHAR *pColorData, int nColortype, int nPort)                             
	{
		return m_Cmd.CmdRCRA_BIN(pColorData, nColortype, nPort);
	}
	 //复位色差值
	int MG_ResetColor(UINT nColorsel, int nPort)                              
	{
		return m_Cmd.CmdRSTC(nColorsel, nPort);
	}
	//去融合带影响
	int MG_CancelBlending(UINT nBlendOpt, int nPort)                           
	{
		return m_Cmd.CmdCRBD(nBlendOpt, nPort);
	}

	//**********************************************************//
	//                        融合带
	//----------------------------------------------------------//
	//测试融合效果
	int MG_TestBlendParams(pBlendParams pBPW, int nPort)    
	{
		return m_Cmd.CmdWBDP(pBPW, nPort);
	}
	//读取融合参数
	int MG_ReadBlendParams(UINT *pBlendParam, int nPort)    
	{
		return m_Cmd.CmdRBDP(pBlendParam, nPort);
	}
	//开启/关闭融合功能
	int MG_EnableBlend(UINT nEnableW, int nPort)            
	{
		return m_Cmd.CmdWBDE(nEnableW, nPort);
	}
	//读取融合功能是否开启
	int MG_GetBlendEnable(UINT &nEnableR, int nPort)        
	{
		return m_Cmd.CmdRBDE(nEnableR, nPort);
	}
	//设置融合带宽度
	int MG_SetBlendWidth(UINT *pWidthW, int nPort)          
	{
		return m_Cmd.CmdWBDW(pWidthW, nPort);
	}
	//读取融合带宽度
	int MG_GetBlendWidth(UINT *pWidthR, int nPort)          
	{
		return m_Cmd.CmdRBDW(pWidthR, nPort);
	}
	//设置纯白阀值
	int MG_SetWhiteThreshold(UINT nWhiteLvlW, int nPort)    
	{
		return m_Cmd.CmdWBDT(nWhiteLvlW, nPort);
	}
	//读取纯白阀值
	int MG_GetWhiteThreshold(UINT &nWhiteLvlR, int nPort)   
	{
		return m_Cmd.CmdRBDT(nWhiteLvlR, nPort);
	}
	//读取融合值
	int MG_ReadblendParamDatas(UINT *pDatatoRead/*数据地址*/,
		                    int nBlendside,
							int nPureWhite,
							int nBlendWidth,
							int nPort) 
	{
		return m_Cmd.CmdRBPD(pDatatoRead, nBlendside, nPureWhite, nBlendWidth, nPort);
	}
	//写入融合值
	int MG_WriteBlendParamDatas(UINT *pDatatoWrite/*数据地址*/,
		                     int nBegin/*写入起始位置*/,
							 int nEnd/*写入结束位置*/,
							 int nBlendside,
							 int nPureWhite,
							 int nPort) 
	{
		return m_Cmd.CmdWBPD(pDatatoWrite, nBegin, nEnd, nBlendside, nPureWhite, nPort);
	}

	//**********************************************************//
	//                        黑电平
	//----------------------------------------------------------//
	//开启黑电平测试环境
	int MG_EnableBlacklvlTest(UINT nBlackLvl, UINT nEnable, int nPort)   
	{
		return m_Cmd.CmdTCRY(nBlackLvl, nEnable, nPort);
	}
	//写入黑电平
	int MG_WriteBlackLvl(UINT *pBlackLvlDataW, UINT nSegment, int nPort) 
	{
		return m_Cmd.CmdWBLV(pBlackLvlDataW, nSegment, nPort);
	}
	//读取黑电平
	int MG_ReadBlackLvl(UINT *pBlackLvlDataR, UINT nSegment, int nPort)  
	{
		return m_Cmd.CmdRBLV(pBlackLvlDataR, nSegment, nPort);
	}
	//设置漏光带宽度
	int MG_SetLeakageWidth(UINT nLeakageSel, UINT nSide1Width, UINT nSide2Width, int nPort) 
	{
		return m_Cmd.CmdWLKT(nLeakageSel, nSide1Width, nSide2Width, nPort);
	}
	//读取漏光带宽度
	int MG_GetLeakageWidth(UINT *pLeakageWidth, int nPort)      
	{
		return m_Cmd.CmdRLKT(pLeakageWidth, nPort);
	}
	//设置黑电平段值
	int MG_SetBlackLvlTickPos(UINT *pBlkLvlSegmentW, int nPort) 
	{
		return m_Cmd.CmdWLVT(pBlkLvlSegmentW, nPort);
	}
	//读取黑电平段值
	int MG_GetBlackLvlTickPos(UINT *pBlkLvlSegmentR, int nPort) 
	{
		return m_Cmd.CmdRLVT(pBlkLvlSegmentR, nPort);
	}
	//开启关闭黑电平功能
	int MG_EnableBlackLevelFunc(UINT nEnable, int nPort)        
	{
		return m_Cmd.CmdWBLE(nEnable, nPort);
	}
	//读取黑电平功能开启与否
	int MG_GetBlackLevelFuncEnable(UINT &nEnable, int nPort)    
	{
		return m_Cmd.CmdRBLE(nEnable, nPort);
	}
	//复位黑电平
	int MG_ResetBlackLvl(int nPort)
	{
		return m_Cmd.CmdRSTL(nPort);
	}

	//**********************************************************//
	//                        几何校正
	//----------------------------------------------------------//
	//保存到WarpTable
	int MG_SaveWarpTable(UINT nTableID, int nPort)
	{
		return m_Cmd.CmdSWPT(nTableID, nPort);
	}
	//读取四点校正四个点的位置
	int MG_Get4PointPosition(UINT *p4PointPos, int nPort)  
	{
		return m_Cmd.CmdGW4P(p4PointPos, nPort);
	}
	//读取曲面校正九个点的位置
	int MG_GetCurvePostion(UINT *pCurvePos, int nPort)     
	{
		return m_Cmd.CmdGW9P(pCurvePos, nPort);
	}
	//读取疏密校正九个点的位置
	int MG_GetDensityPosition(UINT *pDensityPos, int nPort)
	{
		return m_Cmd.CmdGW5P(pDensityPos, nPort);
	}
	//保存四点校正
	int MG_Save4PointPosition(UINT *p4Points, int nPort)
	{
		return m_Cmd.CmdSW4P(p4Points, nPort);
	}
	//保存曲面校正
	int MG_SaveCurvePostion(UINT *pCurvePos, int nPort)     
	{
		return m_Cmd.CmdSW9P(pCurvePos, nPort);
	}
	//保存疏密校正
	int MG_SaveDensityPosition(UINT *pDensityPos, int nPort)
	{
		return m_Cmd.CmdSW5P(pDensityPos, nPort);
	}

	//**********************************************************//
	//                        图像处理与钝化
	//----------------------------------------------------------//
	//读取钝化值
	int MG_GetFilterValue(UINT &nFilterR, int nPort)
	{
		return m_Cmd.CmdRFLI(nFilterR, nPort);
	}
	//设置钝化值
	int MG_SetFilterValue(UINT nFilterW, int nPort)
	{
		return m_Cmd.CmdWFLI(nFilterW, nPort);
	}

	//**********************************************************//
	//                        复位
	//----------------------------------------------------------//
	int ResetBoards(int nPort)  //板卡复位
	{
		return m_Cmd.CmdRSTF(nPort);
	}

	//**********************************************************//
	//                        固化
	//----------------------------------------------------------//
	int SaveAll(int nPort)  //固化数据
	{
		return m_Cmd.CmdSALL(nPort);
	}


	//**********************************************************//
	//                        开窗指令
	//----------------------------------------------------------//
	//读取背景
	int MVX_ReadBackgrdInfo(UINT *pSignalInfo, int nPort)
	{
		return m_Cmd.CmdRCN1(pSignalInfo, nPort);
	}
	//新建窗口
	int MVX_OpenNewWindow(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
	{
		return m_Cmd.CmdOPEN(nWinID, rcWnd, nSrcID, nSrcType, nPort);
	}
	//关闭窗口
	int MVX_ShutdownWindow(int nWinID, int nPort)
	{
		return m_Cmd.CmdSHUT(nWinID, nPort);
	}
	//移动窗口
	int MVX_MoveTheWindow(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
	{
		return m_Cmd.CmdMove(nWinID, rcWnd, nSrcID, nSrcType, nPort);
	}
	//调整窗口大小
	int MVX_ResizeWindow(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
	{
		return m_Cmd.CmdSize(nWinID, rcWnd, nSrcID, nSrcType, nPort);
	}
	//调用场景
	int MVX_CallWindows(UINT *pReadBuf, int nSceneID, int nPort)
	{
		return m_Cmd.CmdCALL(pReadBuf, nSceneID, nPort);
	}
	//保存场景
	int MVX_SaveWindows(UINT *pWriteBuf, int nBufftoWritten, int nSceneID, int nPort)
	{
		return m_Cmd.CmdSAVE(pWriteBuf, nBufftoWritten, nSceneID, nPort);
	}

	//**********************************************************//
	//                       HDMI_3D 模块指令
	//----------------------------------------------------------//
	//读取输入3D模式
	int HDMI_GetInput3Dmode(int &n3Dmode, int nPort)
	{
		return m_Cmd.CmdRI3D(n3Dmode, nPort);
	}
	//设置输出3D模式
	int HDMI_SetInput3Dmode(int n3Dmode, int nPort)
	{
		return m_Cmd.CmdWI3D(n3Dmode, nPort);
	}
	//读取输出3D立体模式
	int HDMI_GetOutput3Dmode(int &n3Dmode, int nPort)
	{
		return m_Cmd.CmdRO3D(n3Dmode, nPort);
	}
	//设置输出3D立体模式
	int HDMI_SetOutput3Dmode(int n3Dmode, int nPort)
	{
		return m_Cmd.CmdWO3D(n3Dmode, nPort);
	}
	//读取场频倍数
	int HDMI_GetOctave(int &nOctave, int nPort)
	{
		return m_Cmd.CmdROHZ(nOctave, nPort);
	}
	//设置场频倍数
	int HDMI_SetOctave(int nOctave, int nPort)
	{
		return m_Cmd.CmdWOHZ(nOctave, nPort);
	}
	//左右眼切换
	int HDMI_SetLeftRightEye(int nPort)
	{
		return m_Cmd.CmdWLRC(nPort);
	}


public:
	//线程控制区
	CWinThread *m_ThreadAjustHandle; //几何校正线程句柄
	BOOL StartThread(UINT nThreadID = 0); //0:表示校正线程 1：表示进度条线程
	void ResumeThread(UINT nThreadID = 0);
	void SuspendThread(UINT nThreadID = 0);
	void ExitThread(UINT nThreadID = 0);	

	CCriticalSection m_Lock; //共享资源锁
	void GlobleLock(){ m_Lock.Lock(); }         //锁定共享资源
	void GlobleUnlock(){ m_Lock.Unlock(); }     //解锁共享资源

private:
	CList <TaskPackage, TaskPackage&> m_ListTaskPack;                      //线程命令数据包队列

public:
	//int g_TaskNum;
	void  PushPackage(TaskPackage pack);  //命令包进队
	TaskPackage  PopPackage();            //命令包离队
	BOOL  IsTaskListEmpty();              //命令包队列是否为空
	int   GetTaskCount() const;           //命令包中任务数
	void  WaitForTaskOver();              //等待线程执行完毕

public:
	CCommandClass m_Cmd;
 
// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMVintergrationControllerApp theApp;
