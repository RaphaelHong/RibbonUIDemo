#pragma once

#pragma pack(push, 1)
//**************** 自定义用户消息 ***********

//******************* 双语支持 **************
//#define ENGLISHVER

//******************* 通信方式 **************
#define NON_CONNECT  0
#define NET_CONNECT  1
#define COM_CONNECT  2
#define USB_CONNECT  3

#define WAIT_SECOND  20


//*************** 线程运行模式 **************
#define IDLE_MODE         0
#define WORK_MODE_AJUST   1
#define WORK_MODE_SHOW    WORK_MODE_AJUST
#define WORK_MODE_HIDE    2
#define EXIT_MODE         3

//******************* 命令状态 **************
#define CMD_RTOK     0x0
#define CMD_TIMEOUT  0x10000
#define CMD_RTER     CMD_TIMEOUT
#define CMD_RTERCODE 0x10001
#define CMD_FAILURE  -1

//**************** 无效的端口号 *************
#define INVALID_PORT 0xff
#define UNKNOW_DEV_NAME _T("Unknow Device")

//**************** 产品型号 *****************
typedef enum emProductType
{
	Product_INVALID = 0x00,  // 无效的产品
	Product_MV      = 0x10,  // 多屏保系列
	Product_MG      = 0x20,  // 几何校正系列
	Product_MP      = 0x30,  // 边缘融合系列
	Product_MD      = 0x40,  // 多分器系列
	Product_HDMI_3D = 0x50,  // HDMI_3D转换器系列
	Product_MVX     = 0x60,  // 三屏分割模块
};

//************** 分辨率种类标志 *************
#define WIDE_TIMING			1
#define NON_WIDE_TIMING		0

#define ENABLE_3D_2D        0x10
#define HDCP_TIMING         0x20
#define FLEXVIEW_TIMING     0x30

//**************** 分辨率信息 ***************
typedef struct __st_timing__ {
	int m_PixelRate;  //unit 10KHHz
	int m_HActive;    //水平活动像素	
	int	m_HBlank;     //水平空白
	int	m_VActive;    //垂直活动像素
	int	m_VBlank;	  //垂直空白
	int	m_HFPorch;    //水平前沿
	int	m_HSyncWid;   //水平同步宽度
	int	m_VFPorch;    //垂直前沿
	int	m_VSyncWid;	  //垂直同步宽度
	int	m_HSize; 
	int	m_VSize;
	int	m_HPol;       //水平极性
	int	m_VPol;       //垂直极性  
	int	m_HBorder; 
	int	m_VBorder;
	int m_Hz;         //刷新率
	int m_HTotal;     //垂直总像素
	int m_VTotal;     //水平总像素
} TIMING_STRUCT,*pTIMING_STRUCT;


typedef	struct {

	CString		m_strDev_EDID_Name;  //设备EDID中的名称 
	CString		m_strDev_Dev_Name;  //设备对外产品的名称 

	int m_Prd_ID ;

	int m_Dev_ID;  //设备代码
	int m_nFunc_Code; //功能码

	CString *m_pStrTiming;
	TIMING_STRUCT *m_pDetailed_Timing;

}DEVICE_STRUCT;

//**************** 网络参数信息 **************
struct stIPADDR{
	UINT sub0; //地址段0
	UINT sub1; //地址段1
	UINT sub2; //地址段2
	UINT sub3; //地址段3
};
typedef struct __st_netparams__ {
	UINT mac[6];
	stIPADDR ipaddr;
	stIPADDR mask;
	stIPADDR gateway;
} NetParams,*pNetParams;



//***************** 色差校正  *****************
typedef struct __stColorTest
{
	UINT nRvalue;   //红色值
	UINT nGvalue;   //绿色值
	UINT nBvalue;   //蓝色值
	UINT nRtestpos; //红色测试点位置
	UINT nGtestpos; //绿色测试点位置
	UINT nBtestpos; //蓝色测试点位置
	UINT nTPenable; //开启纯色 0：纯色 1：图像
} ColorTestParams, *pColorTestParams;

//****************** 融合参数 *****************
typedef struct __stBlendParam
{
	UINT nBlendSel;     // 融合带选择
	UINT nPureWhite;    // 纯白模式
	UINT nAlpha;        // 归一化后的Alpha值
	UINT nP;            // 归一化后的P值
	UINT nGamma;        // 归一化后的Gamma值

} BlendParams, *pBlendParams;

//******************** 分屏模式 *************
typedef struct 
{
	CString strmode;
	int     nScreenNum;
} stScreenSplitMode, *pstScreenSplitMode;

#define TOTAL_SPLIT_MODE 15
const stScreenSplitMode stSplitMode[TOTAL_SPLIT_MODE] = {
	// Mode name          Screen numbers
#ifdef ENGLISHVER
	{_T("1x2 Mode Horizontal"),    2}, 
	{_T("1x3 Mode Horizontal"),    3},
	{_T("1x4 Mode Horizontal"),    4}, 
	{_T("1x5 Mode Horizontal"),    5}, 
	{_T("1x6 Mode Horizontal"),    6}, 

	{_T("2x1 Mode Vertical"),    2},
	{_T("3x1 Mode Vertical"),    3},
	{_T("4x1 Mode Vertical"),    4},
	{_T("5x1 Mode Vertical"),    5},
	{_T("6x1 Mode Vertical"),    6},

	{_T("2x2 Mode Grid"),    4},
	{_T("3x2 Mode Grid"),    6},
	{_T("2x3 Mode Grid"),    6},

	{_T("1x1 Mode Single-screen"), 1},
#else
	{_T("1x2 Mode 水平"),    2},
	{_T("1x3 Mode 水平"),    3},
	{_T("1x4 Mode 水平"),    4},
	{_T("1x5 Mode 水平"),    5},
	{_T("1x6 Mode 水平"),    6},

	{_T("2x1 Mode 垂直"),    2},
	{_T("3x1 Mode 垂直"),    3},
	{_T("4x1 Mode 垂直"),    4},
	{_T("5x1 Mode 垂直"),    5},
	{_T("6x1 Mode 垂直"),    6},

	{_T("2x2 Mode 网格"),    4},
	{_T("3x2 Mode 网格"),    6},
	{_T("2x3 Mode 网格"),    6},

	{_T("1x1 Mode 单屏"),    1},   
#endif
	{_T("END") ,             0}
};

#define BKCOLORS 8
const COLORREF constClrDlgBk[BKCOLORS] = {
	RGB(216,171,192),
	RGB(138,168,228),
	RGB(255,219,117),
	RGB(189,205,159),
	RGB(240,158,159),
	RGB(186,166,225),
	RGB(154,191,180),
	RGB(247,181,131)
	
	//RGB(98,213,246)
};

typedef struct stCustomButton{
	UINT  nID;
	CRect rcWnd;
	BOOL  bClicked;
	BOOL  bFocused;
	CString strSurfaceName;
	stCustomButton() {
		bClicked = FALSE;
		bFocused = FALSE;
		strSurfaceName = _T("Button");
	}
} CustBtn,*pCustBtn;

#pragma pack(pop)