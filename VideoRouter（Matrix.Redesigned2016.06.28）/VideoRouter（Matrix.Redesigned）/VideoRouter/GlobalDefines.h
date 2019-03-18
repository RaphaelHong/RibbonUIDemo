#pragma once
#include <memory>
#include <afxtempl.h>
#include "WrapInitFile.h"

using namespace std;

template <typename A, typename B> struct Pair {
	A key;
	B val;
	Pair() {}
	Pair(A k, B v) {
		key = k;
		val = v;
	}

	BOOL operator==(const Pair &DestPair)
	{
		return ((key==DestPair.key) && (val==DestPair.val));
	}

	BOOL operator!=(const Pair &DestPair)
	{
		return ((key!=DestPair.key) || (val!=DestPair.val));
	}

	void operator=(const Pair &DestPair)
	{
		key = DestPair.key;
		val = DestPair.val;
	}
};


typedef Pair<int, int> INT_PAIR;
typedef Pair<short, short> SHORT_PAIR;

#pragma pack(push, 1)

//********************************************************
// 设备工作模式
typedef enum __emWorkMode
{
	eWorkModeMatrix = 1,  //矩阵模式
	eWorkModeSplit  = 2,  //开窗模式
} emWorkMode;



//********************************************************
// 自定义用户消息
#define WM_USER_SLIDER_LBTNUP    WM_USER + 1       /*滑动条左键弹起消息处理，控制滑动条在鼠标松开的时候触发处理程序*/
#define WM_USER_CANCEL_EDIT      WM_USER + 2       /*Inplace Edit 取消编辑*/
#define WM_USER_END_EDIT         WM_USER + 3       /*Inplace Edit 完成编辑*/
#define WM_USER_RECV_SCENE_DATA  WM_USER + 11      /*解析场景数据（设备数据）*/
#define WM_USER_UNPACK_SCENE     WM_USER + 12      /*解析场景文件 */
#define WM_USER_PACKAGE_SCENE    WM_USER + 13      /*打包场景文件 */
#define WM_USER_TRANS_COMPLETE   WM_USER + 1100    /*通知主线程，任务已完成*/


//********************************************************
// 通用窗口定时器
#define TIMER_KILLFOCUS    (1000)


//********************************************************
// 通信方式
#define NON_CONNECT  (0)       //通信状态，当前无连接
#define NET_CONNECT  (1)       //通信状态，已通过网络连接
#define COM_CONNECT  (2)       //通信状态，已通过串口连接
#define USB_CONNECT  (3)       //通信状态，已通过USB连接
#define ALIVE_KEEP   (1)       //在线检测使能标志

#define COM_MAX_OUTTIME (200)  //200次ReadFile()失败后，指令超时
#define NET_MAX_OUTTIME (10)   //10次套接字Recv()失败后，指令超时

// 无效的端口号
#define INVALID_PORT (0xFFFF)    //通信端口号无效
#define SYSCARD_PORT (0x5802)    //系统板卡端口号
#define SUBPORT_A    (0xA0000)   //子信号1
#define SUBPORT_B    (0xB0000)   //子信号2

// 指令执行状态
#define CMD_RTOK     (0x0)          //指令返回正常
#define CMD_TIMEOUT  (0x10000)      //指令执行超时
#define CMD_RTER     CMD_TIMEOUT    //指令返回错误
#define CMD_RTERCODE (0x10001)      //指令返回错误，并附错误代码
#define CMD_FAILURE  (-1)           //指令失败

#define SAFE_BUFF    (256)          //发送命令时使用统一长度参数


//********************************************************
// 网络参数信息
//

typedef struct tagIPADDR{
	UINT sub0; //地址段0
	UINT sub1; //地址段1
	UINT sub2; //地址段2
	UINT sub3; //地址段3
} IPADDR;

typedef struct tagNetParams {

	UINT mac[6];
	IPADDR ipaddr;
	IPADDR mask;
	IPADDR gateway;

} NETPARAMS,*pNETPARAMS;



//********************************************************
// 矩阵MAX
//

#define _MAX_SIGNAL_NAME (128)      //信号名称最多256个字节，即256个单字节字符，128个双字节字符
#define _MAX_OUTPUT_PORT (144)      //最大输出通道数
#define _MAX_INPUT_PORT  (144)      //最大输入信号数
#define INVALID_VALUE    (-1)       //无效值标志
#define ALL_SELECTED     (0xA000)   //虚拟矩阵全选标志



//********************************************************
// 信号源类型定义
//

#define _MAX_VIDEO_TYPE (14)        //信号源种类

typedef enum __emVideoType
{
	vtCVBS    = 0x00,    // CVBS信号
	vtYpbpr   = 0x01,    // YPbPr信号
	vtVGA     = 0x02,    // VGA信号
	vtSDI     = 0x03,    // SDI信号
	vtHDMI    = 0x06,    // HDMI信号	
	vtDVI     = 0x08,    // DVI信号
	vtDP      = 0x09,    // DP信号
	vt4K_HDMI = 0x16,    // 4K HDMI
	vtHDBASET = 0x17,    // HDBASET
	vtDLDVI   = 0x18,    // Dual Link DVI
	vtPAL_D   = 0x20,    // 720x576i@50Hz
	vtNTSC_D  = 0x21,    // 720x480i@60Hz
	vtPAL_W   = 0x22,    // 960x576i@50Hz
	vtNTSC_W  = 0x23,    // 960x480i@60Hz
	vtUndef   = 0xff,    // 无信号

} emVideoType;

CStringW GetVideoName(emVideoType eVT);
COLORREF GetVideoColor(emVideoType eVT);


//********************************************************
// 分辨率信息
//

#define DEFAULT_RESOLUTION (0)  //内部默认分辨率
#define CUSTOM_RESOLUTION  (1)  //用户自定义分辨率
#define UNKNOW_RESOULUTION (0xff)  //未知分辨率

typedef struct tagTiming {

	int m_PixelRate;  //unit 10KHz
	int m_HActive;    //水平活动像素	
	int	m_HBlank;     //水平空白
	int	m_VActive;    //垂直活动像素
	int	m_VBlank;	  //垂直空白
	int	m_HFPorch;    //水平前沿
	int	m_HSyncWid;   //水平同步宽度
	int	m_VFPorch;    //垂直前沿
	int	m_VSyncWid;	  //垂直同步宽度
	int	m_HSize;      //水平图像尺寸
	int	m_VSize;      //垂直图像尺寸
	int	m_HPol;       //水平极性
	int	m_VPol;       //垂直极性  
	int	m_HBorder;    //水平后沿
	int	m_VBorder;    //垂直后沿
	int m_Hz;         //刷新率
	int m_HTotal;     //垂直总像素
	int m_VTotal;     //水平总像素
	int m_Type;       //分辨率种类 0(DEFAULT_RESOLUTION):native; 1(CUSTOM_RESOLUTION):custiomized
	BOOL IsValid();
	BOOL operator==(const tagTiming &Tobj) const;

} TIMING,*pTIMING;

// 预定义分辨率种数
#define _MAX_ORG_RESOLUTION 11
// 预定义分辨率
extern TIMING OriginalResolutions[_MAX_ORG_RESOLUTION];


//********************************************************
// 背板信息

typedef enum __emBackboardArrangeMode
{
	eBackboardArrangeModeLeftTop     =  1,  //背板上板卡序号从左至右，从上至下
	eBackboardArrangeModeLeftBottom  =  2,  //背板上板卡序号从左至右，从下至上
	eBackboardArrangeModeRightTop    =  3,  //背板上板卡序号从右至左，从上至下
	eBackboardArrangeModeRightBottom =  4   //背板上板卡序号从右至左，从下至上

} emBackboardArrangeMode;

typedef struct tagBackboardInfo {
	tagBackboardInfo() {}
	tagBackboardInfo(const tagBackboardInfo & DestInfo)
	{
		m_nHorzNum = DestInfo.m_nHorzNum;
		m_nVertNum = DestInfo.m_nVertNum;
		m_nDeviceID= DestInfo.m_nDeviceID;
		m_eArrangeMode = DestInfo.m_eArrangeMode;
	}

	int m_nHorzNum;        //背板横向隔断数
	int m_nVertNum;        //背板纵向插槽数
	int m_nDeviceID;       //设备ID
	emBackboardArrangeMode m_eArrangeMode; //背板上板卡序号排列方式

} BACKBOARDINFO, *pBACKBOARDINFO;



//********************************************************
// 板卡信息

// 板卡类型
typedef enum __emCardType
{
	eCardTypeUndef   = 0,  //未定义
	eCardTypeCtrl    = 1,  //控制卡
	eCardTypeInput   = 2,  //输入采集卡
	eCardTypeOutput  = 3   //输出采集卡

} emCardType;

// 板卡ID定义
typedef enum __emCardID
{
	eMM10X_UNDEF     = 0xFFFF,  //未定义
	eMM10X_SYS_CTRL  = 0x5802,  //控制卡ID
	eMM10X_4IN_DVIM  = 0x7001,  //4口DVI输入采集卡
	eMM10X_2IN_DPDVI = 0x7101,  //2口DPDVI输入采集卡
	eMM10X_4OUT_DVII = 0x7201,  //4口DVI输出卡

} emCardID;

// 板卡信息
typedef struct tagCardInfo {

	emCardID m_eCardID;
	emCardType m_eCardType;
	int m_nLogicPorts;

} CARDINFO, *pCARDINFO;

// 定义所有类型板卡的信息
#define _MAX_CARD_DEF    (4)  //板卡种类
extern CARDINFO CardInfo[_MAX_CARD_DEF];

// 根据板卡ID获取板卡类型
emCardType GetCardType(const UINT uCardID);


//////////////////////////////////////////////////////////////////////////
// 设备信息

typedef struct tagDeviceInfo {
	tagDeviceInfo () {}
	tagDeviceInfo (const tagDeviceInfo &DestInfo) {
		m_nMaxIn = DestInfo.m_nMaxIn;
		m_nMaxOut= DestInfo.m_nMaxOut;
		m_stBBI = DestInfo.m_stBBI;
	}

	int m_nMaxIn;
	int m_nMaxOut;
	BACKBOARDINFO m_stBBI;

} DEVICEINFO, *pDEVICEINFO;



//********************************************************
// 场景MAX
//

#define CURRENT_SCENE    (0)      //当前场景号
#define _MAX_SCENE_NAME  (128)    //最大场景名字符数，128个char型字符
#define _MAX_SCENE_COUNT (128)    //底层最多能存储的场景数量

// 输入输出键值对
typedef Pair<int, int> InOutKey;



//********************************************************
// 用户信息
//

typedef enum emUserAuthority
{
	eAuthorityErr   = 0,
	eSuperAdmin     = 1,
	eAdministrator  = 2,
	eGeneral        = 3,
	eSuperUser      = 4,
} Authority;

#define _MAX_UNAME   (128)
#define _MAX_PASSWORD (20)

typedef struct tagUserData
{

	int  nUserID;                    // ID
	char szUserName[_MAX_UNAME];     // 用户名
	char szUserPwd[_MAX_PASSWORD];   // 用户密码
	Authority eAuthority;            // 权限

} UserData,* pUserData;



//********************************************************
// 子线程状态定义
//

#define THREAD_IDLE          (0)  //线程空转
#define THREAD_WORK          (1)  //线程工作
#define THREAD_EXIT          (2)  //线程退出
#define THD_LOOP_IDLE        THREAD_IDLE
#define THD_LOOP_WORK        THREAD_WORK
#define THD_LOOP_EXIT        THREAD_EXIT

typedef enum __emTransaction
{
	eTransaction_Undef            = 0,
	eTransaction_BeginInitDevice  = 1,
	eTransaction_RefreshSignal    = 2,
	eTransaction_BeginInitTiming  = 3,
	eTranscation_ResetDevice      = 4,

} emTransaction ;



//////////////////////////////////////////////////////////////////////////
// 端口信息结构体
//

typedef struct tagBasePort
{
	int         m_nID;                      //端口ID     (0 - N)从0开始
	int         m_nCmdPort;                 //通信端口号 (1 - N+1)从1开始
	int         m_nSubPort;                   //当前逻辑子端口号 从1开始，如果没有子端口则为-1
	char        m_szName[_MAX_SIGNAL_NAME]; //端口名称
	TIMING      m_timing;                   //端口分辨率

	// 当前信号信息
	emVideoType m_eVT;                      //当前输入信号类型
	int         m_nCurHAct;                 //当前信号水平分辨率
	int         m_nCurVAct;                 //当前信号垂直分辨率
	int         m_nCurHz;                   //当前信号频率

	// 以下只对输出端口有效
	int         m_nFeederID;                  //来源信号ID,
	
} BASEPORT;
// 定义一个输出端口类型
typedef BASEPORT OUTPORT;
// 定义一个输入端口类型
typedef BASEPORT INPORT;


//////////////////////////////////////////////////////////////////////////
// 图像测试信息
//
// 测试类型
enum emImageTestType
{
	eTestTypeColor = 0,
	eTestTypeGrid  = 2,
	eTestTypeCancelColor = 3,
	eTestTypeCancelGrid  = 4,
};
// 图像测试信息结构体
typedef struct stImgTestInfo
{
	emImageTestType eTesttype; //测试类型
	COLORREF rgb;              //测试颜色
	CPoint ptBegin;            //图像起始点
	CSize szLineSpace;         //线条间隙 cx为水平间隙，cy为垂直间隙
	int iLineWidth;            //线条宽度

} IMGTESTINFO, *pIMGTESTINFO;


//////////////////////////////////////////////////////////////////////////
// 图像测试信息
//

#pragma pack(pop)

emCardType GetCardType(const UINT uCardID);

/****************** 系统色彩 ********************/
extern COLORREF ListColorBackground;   //列表框背景颜色
extern COLORREF ListColorItemSelected;   //列表项被选中背景颜色
extern COLORREF ListColorActionFailed;   //列表项执行失败背景颜色
extern COLORREF ListColorActionSucceed;   //列表项执行成功背景颜色
extern COLORREF ListColorText;   //列表框字体颜色

extern COLORREF DialogColorBackground;    //对话框背景颜色


// 重绘锁
class CRedrawLock
{
public:
	CRedrawLock() { m_pWnd = NULL; }
	CRedrawLock(CWnd* pWnd)
	{
		m_pWnd = pWnd;
		if (m_pWnd != NULL) {			
			m_pWnd->SetRedraw(FALSE);
		}
	}

	~CRedrawLock()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->SetRedraw(TRUE);
			m_pWnd->RedrawWindow();
			m_pWnd = NULL;
		}
	}

private:
	CWnd* m_pWnd;
};

// 动态申请内存管理器
template <typename T> class CMemoryAllocate : allocator <T>
{
public:
	CMemoryAllocate() {
		m_pPointer = NULL;
		m_lsize = 0;
	}

	CMemoryAllocate(long lsize) {
		m_pPointer = allocate(lsize);
		m_lsize = lsize;
	}

	~CMemoryAllocate() {
		if (m_pPointer != NULL)
			deallocate(m_pPointer, m_lsize);

		m_pPointer = NULL;
		m_lsize = 0;
	}

	T* GetAddress() {
		return m_pPointer;
	}

	long GetLength() const {
		return m_lsize;
	}

private:
	T* m_pPointer;
	long  m_lsize;
};

/*************** 改进的冒泡排序 *****************/
template <typename T>

void Sort(T *args, int size)
{
	int i= size-1;
	int pos = 0;
	T tmp;
	while(pos > 0)
	{
		pos = 0;
		for (int j=0; j<i; j++) {
			if (args[j] > args[j+1])
			{
				pos = j;
				tmp = args[j];
				args[j] = args[j+1];
				args[j+1] = tmp;
			}
		}

		i = pos;		
	}
}


// 浮点数四舍五入
int Round(double value);

// 将CBitmap转换成GDI+的Image对象
BOOL BitmapToImage(CBitmap *pBitmap, Image *&pImg);
// GDI+ 加载资源文件 调用此函数后记得释放指针对象，不然会导致程序运行期间不断占用内存
BOOL ImageFromIDResource(UINT nID, LPCTSTR lpType, Image *&pImg);