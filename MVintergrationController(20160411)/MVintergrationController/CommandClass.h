#pragma once

#include "SerialPort.h"
#include "EthernetPort.h"
#include "USBHIDPort.h"

#define SAFE_BUFF_LEN (256)

#pragma pack(push,1)
/******************************* 二进制通信封装 类 ******************************/
class PC_Command
{

public:
	unsigned char	head1;		//帧头1			1Byte
	unsigned char	head2;		//帧头2			1Byte
	unsigned char	inst;		//指令			1Byte
	unsigned int	para;		//地址 参数		4Byte
	unsigned short  length;		//长度			2Byte
	unsigned char   data[2100];	//数据			2048Byte
	unsigned char   check;		//校验			1Byte
	unsigned char   tail;		//帧尾			1Byte

	void CalcCheck()
	{
		check=head1^head2^inst^(para&0xFF)^((para>>8)&0xFF)^((para>>16)&0xFF)^((para>>24)&0xFF)^(length&0xFF)^((length>>8)&0xFF)^tail;
		for(int i=0; i<length; i++)
			check=check^data[i];
	};
};

class STM_Command
{
public:
	unsigned char	head1;		//帧头1			1Byte
	unsigned char	head2;		//帧头2			1Byte
	unsigned char	inst;		//指令			1Byte
	unsigned int	para;		//地址 参数		4Byte
	unsigned short  length;		//长度			2Byte
	unsigned char   data[2100];	//数据			2048Byte
	unsigned char   check;		//校验			1Byte
	unsigned char   tail;		//帧尾			1Byte


	BOOL isCorrect()
	{
		unsigned char ck = 0;
		ck=head1^head2^inst^(para&0xFF)^((para>>8)&0xFF)^((para>>16)&0xFF)^((para>>24)&0xFF)^(length&0xFF)^((length>>8)&0xFF)^check^tail;

		for(int i=0; i<((length == 0x8001)?1:length); i++)
			ck=ck^data[i];
		if(0x00 == ck)
			return true;
		return false;
	}
};
#pragma pack(pop)

//******************** 二进制读写命令 ***********
#define BINCODE_SPI             0x00 //SPI 操作 
#define BINCODE_CMD             0x03 //cmd 指令操作 
#define BINCODE_FLASH           0x02 //Flash 操作 
#define BINCODE_FCN             0x08 //功能操作

// 色差数据加载命令
#define BINEXE_COLOR_WRITE      0x00
#define BINEXE_COLOR_READ       0x10

// 高级融合参数加载
#define BINEXE_BLENDPARA_WRITE  0x20
#define BINEXE_BLENDPARA_READ   0x30

// 几何校正数据加载
#define BINEXE_WARP_ENABLE      0x80 //c786 warp execute end  or start
#define BINEXE_WARP_WRITE       0x90
#define BINEXE_WARP_SAVE        0x70  //保存 Warp 4Point 9Point  5x5疏密度 

// 多点校正数据加载与保存
#define BINEXE_GRID_ENABLE      0xc0  //Grid 写入 
#define BINEXE_GRID_WRITE       0xd0  
#define BINEXE_GRID_READ        0xe0 


class CCommandClass
{
public:
	CCommandClass();

public:
	void SetInterface(CEthernetPort *pSock, CSerialPort *pCom, CUSBHIDPort *pUSB);

	//*********************** 控制板卡 *************************//
	//
	// 网络参数
	int CmdRNET(pNetParams netparamW, int nPort=0xff); /*读取网络参数*/
	int CmdWNET(pNetParams netparamR, int nPort=0xff); /*设置网络参数*/

	//*********************** 所有板卡通用命令 *************************//
	//
	// 系统验证
	int CmdRMVR(int nPort=0xff);  /*读取生产商信息*/
	int CmdRDID(int nPort=0xff);  /*读取设备ID*/
	int CmdRDFN(int nPort=0xff);  /*读取设备功能码*/
	int CmdRDPT(int &nBoards, int *nPortID, int nPort=0xff); /*读取设备所有板卡数及各板卡的端口号， 只针对一体机*/

	// 分辨率
	int CmdRINT(TIMING_STRUCT *pTsR, int nEdidPort/*EDID口*/, int nTimingIndex/*主次分辨率*/, int nPort);  /*读取输入分辨率*/
	int CmdWINT(TIMING_STRUCT *pTsW, int nEdidport, int nTimingIndex, int nPort);                          /*设置输入分辨率*/
	int CmdGETR(UINT &nHz, UINT &nHtotal, UINT &nHactive, UINT &nVtatol, UINT &nVactive, int nPort); /*读取即时信号的分辨率*/
	int CmdROUT(TIMING_STRUCT *pTsR, int nPort); /*读取输出分辨率*/
	int CmdWOUT(TIMING_STRUCT *pTsW, int nPort); /*设置输出分辨率*/	

	//*********************** MG300 模块 *************************//
	//
	// 色差
	int CmdTCLR(pColorTestParams clrtestParam, int nPort);                  /*颜色测试*/
	int CmdCCLR(int nPort);                                                 /*取消颜色测试*/
	int CmdWCRA_ASC(UINT *pColorData, int nDataLen, int nColortype, int nPort); /*写入色差数据*/
	int CmdRCRA_ASC(UINT *pColorData, int nColortype, int nPort);               /*读取色差数据*/
	int CmdWCRA_BIN(UCHAR *pColorData, int nColortype, int nLen, int nPort);
	int CmdRCRA_BIN(UCHAR *pColorData, int nColortype, int nPort);
	int CmdRSTC(UINT nColorsel, int nPort);                                 /*复位色差*/
	int CmdCRBD(UINT nBlendOpt, int nPort);                                 /*取消融合带影响 nBlendOpt:0-取消融合带 1-擦黑融合带 2-恢复融合带*/

	// 融合带
	int CmdWBDP(pBlendParams pBPW, int nPort);  /*测试融合参数Alpha,P,Gamma测试*/
	int CmdRBDP(UINT *pBlendParam, int nPort);  /*读取融合参数Alpha,P,Gamma*/
	int CmdWBDE(UINT nEnableW, int nPort);      /*开启/关闭融合功能*/
	int CmdRBDE(UINT &nEnableR, int nPort);     /*读取融合功能是否开启*/
	int CmdWBDW(UINT *pWidthW, int nPort);      /*设置融合带宽度*/
	int CmdRBDW(UINT *pWidthR, int nPort);      /*读取融合带宽度*/
	int CmdWBDT(UINT nWhiteLvlW, int nPort);    /*设置纯白阀值*/
	int CmdRBDT(UINT &nWhiteLvlR, int nPort);   /*读取纯白阀值*/
	int CmdRBPD(UINT *pDatatoRead/*数据地址*/, int nBlendside, int nPureWhite, int nBlendWidth, int nPort); /*读取融合值*/
	int CmdWBPD(UINT *pDatatoWrite/*数据地址*/, int nBegin/*写入起始位置*/, int nEnd/*写入结束位置*/, int nBlendside, int nPureWhite, int nPort); /*写入融合值*/

	// 黑电平
	int CmdTCRY(UINT nBlackLvl/*测试黑电平位置*/, UINT nEnable/*开启关闭*/, int nPort);  /*开启黑电平测试环境*/
	int CmdWBLV(UINT *pBlackLvlDataW, UINT nSegment, int nPort);  /*写入黑电平, 每个融合区当前段的偏移值*/
	int CmdRBLV(UINT *pBlackLvlDataR, UINT nSegment, int nPort);  /*读取黑电平, 每个融合区当前段的偏移值*/
	int CmdWLKT(UINT nLeakageSel, UINT nSide1Width, UINT nSide2Width, int nPort);  /*设置漏光带宽度*/
	int CmdRLKT(UINT *pLeakageWidth, int nPort);    /*读取漏光带宽度*/
	int CmdWLVT(UINT *pBlkLvlSegmentW, int nPort);  /*设置黑电平段点值*/
	int CmdRLVT(UINT *pBlkLvlSegmentR, int nPort);  /*读取黑电平段点值*/
	int CmdWBLE(UINT nEnable, int nPort);           /*开启关闭黑电平功能*/
	int CmdRBLE(UINT &nEnable, int nPort);          /*读取黑电平功能是否开启*/
	int CmdRSTL(int nPort);                         /*复位黑电平*/

	// 几何校正
	int CmdGW4P(UINT *p4PointPos, int nPort);   /*读取四点校正 控制点位置*/
	int CmdGW9P(UINT *pCurvePos,  int nPort);   /*读取曲面校正 控制点位置*/
	int CmdGW5P(UINT *pDensityPos,int nPort);   /*读取疏密校正 控制点位置*/
	int CmdSWPT(UINT nTableID,    int nPort);   /*保存Warp表*/
	int CmdSW4P(UINT *p4PointPos, int nPort);   /*保存四点校正 控制点位置*/
	int CmdSW9P(UINT *pCurvePos,  int nPort);   /*保存曲面校正 控制点位置*/
	int CmdSW5P(UINT *pDensityPos,int nPort);   /*保存疏密校正 控制点位置*/

	// 图像处理
	int CmdRFLI(UINT &nFilter, int nPort);  /*读取钝化值*/
	int CmdWFLI(UINT nFilter, int nPort);   /*设置钝化值*/

	// 复位功能
	int CmdRSTF(int nPort);  /*板卡复位*/

	// 固化保存
	int CmdSALL(int nPort);  /*固化参数*/


	//*********************** MVX303 三屏开窗模块 *************************//
	//
	int CmdRCN1(UINT *pBuf, int nPort);  //读取背景信息
	int CmdCALL(UINT *pSceneBufR, int nSceneID, int nPort); //调用场景
	int CmdSAVE(UINT *pSceneBufW, int nLen, int nSceneID, int nPort); //保存场景
	int CmdOPEN(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort); //开窗命令
	int CmdMove(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort); //移动命令
	int CmdSize(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort); //调整尺寸
	int CmdSHUT(int nWinID, int nPort); //关闭窗口

	//*********************** HDMI_3D 模块 *************************//
	//
	int CmdWOUI(int nForceTiming, int nPort);     //设置输出分辨率
	int CmdROUI(int &nForceTiming, int &nTimingIndex, int &nHactive, int &nVactive, int nPort); //获取输出分辨率
	int CmdWI3D(int n3Dmode, int nPort);   //设置输入3D模式
	int CmdRI3D(int &n3Dmode, int nPort);  //读取输入3D模式
	int CmdWO3D(int n3Dmode, int nPort);   //设置输出3D立体模式
	int CmdRO3D(int &n3Dmode, int nPort);  //读取输出3D立体模式
	int CmdWOHZ(int nTimes, int nPort);    //设置场频倍数
	int CmdROHZ(int &nTimes, int nPort);   //读取场频倍数
	int CmdWLRC(int nPort);                //左右眼切换

public:
	CEthernetPort *m_pCmdSock;
	CSerialPort *m_pCmdCom;
	CUSBHIDPort *m_pCmdUSB;
	UINT NormalASCII_RW(char *cmd, UINT *cmd_para, UINT len=0, UINT port=0xff, int usbRW = 0/*0:USB可读可写 1:USB只写不读*/);
	UINT BinCommand_RW(UCHAR type, UINT addr, UCHAR *data, UINT len, UCHAR port, int usbRW = 0); //二进制指令的读写操作 
	BOOL ConvertSTM_Cmd(STM_Command* pCmd_STM,char *pFrame,int len);
	void ConvertPC_Cmd(PC_Command* pCmd_PC, char *pFrame, UINT &len);
};
