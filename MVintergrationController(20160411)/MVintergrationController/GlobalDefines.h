#pragma once

#pragma pack(push, 1)
//**************** �Զ����û���Ϣ ***********

//******************* ˫��֧�� **************
//#define ENGLISHVER

//******************* ͨ�ŷ�ʽ **************
#define NON_CONNECT  0
#define NET_CONNECT  1
#define COM_CONNECT  2
#define USB_CONNECT  3

#define WAIT_SECOND  20


//*************** �߳�����ģʽ **************
#define IDLE_MODE         0
#define WORK_MODE_AJUST   1
#define WORK_MODE_SHOW    WORK_MODE_AJUST
#define WORK_MODE_HIDE    2
#define EXIT_MODE         3

//******************* ����״̬ **************
#define CMD_RTOK     0x0
#define CMD_TIMEOUT  0x10000
#define CMD_RTER     CMD_TIMEOUT
#define CMD_RTERCODE 0x10001
#define CMD_FAILURE  -1

//**************** ��Ч�Ķ˿ں� *************
#define INVALID_PORT 0xff
#define UNKNOW_DEV_NAME _T("Unknow Device")

//**************** ��Ʒ�ͺ� *****************
typedef enum emProductType
{
	Product_INVALID = 0x00,  // ��Ч�Ĳ�Ʒ
	Product_MV      = 0x10,  // ������ϵ��
	Product_MG      = 0x20,  // ����У��ϵ��
	Product_MP      = 0x30,  // ��Ե�ں�ϵ��
	Product_MD      = 0x40,  // �����ϵ��
	Product_HDMI_3D = 0x50,  // HDMI_3Dת����ϵ��
	Product_MVX     = 0x60,  // �����ָ�ģ��
};

//************** �ֱ��������־ *************
#define WIDE_TIMING			1
#define NON_WIDE_TIMING		0

#define ENABLE_3D_2D        0x10
#define HDCP_TIMING         0x20
#define FLEXVIEW_TIMING     0x30

//**************** �ֱ�����Ϣ ***************
typedef struct __st_timing__ {
	int m_PixelRate;  //unit 10KHHz
	int m_HActive;    //ˮƽ�����	
	int	m_HBlank;     //ˮƽ�հ�
	int	m_VActive;    //��ֱ�����
	int	m_VBlank;	  //��ֱ�հ�
	int	m_HFPorch;    //ˮƽǰ��
	int	m_HSyncWid;   //ˮƽͬ�����
	int	m_VFPorch;    //��ֱǰ��
	int	m_VSyncWid;	  //��ֱͬ�����
	int	m_HSize; 
	int	m_VSize;
	int	m_HPol;       //ˮƽ����
	int	m_VPol;       //��ֱ����  
	int	m_HBorder; 
	int	m_VBorder;
	int m_Hz;         //ˢ����
	int m_HTotal;     //��ֱ������
	int m_VTotal;     //ˮƽ������
} TIMING_STRUCT,*pTIMING_STRUCT;


typedef	struct {

	CString		m_strDev_EDID_Name;  //�豸EDID�е����� 
	CString		m_strDev_Dev_Name;  //�豸�����Ʒ������ 

	int m_Prd_ID ;

	int m_Dev_ID;  //�豸����
	int m_nFunc_Code; //������

	CString *m_pStrTiming;
	TIMING_STRUCT *m_pDetailed_Timing;

}DEVICE_STRUCT;

//**************** ���������Ϣ **************
struct stIPADDR{
	UINT sub0; //��ַ��0
	UINT sub1; //��ַ��1
	UINT sub2; //��ַ��2
	UINT sub3; //��ַ��3
};
typedef struct __st_netparams__ {
	UINT mac[6];
	stIPADDR ipaddr;
	stIPADDR mask;
	stIPADDR gateway;
} NetParams,*pNetParams;



//***************** ɫ��У��  *****************
typedef struct __stColorTest
{
	UINT nRvalue;   //��ɫֵ
	UINT nGvalue;   //��ɫֵ
	UINT nBvalue;   //��ɫֵ
	UINT nRtestpos; //��ɫ���Ե�λ��
	UINT nGtestpos; //��ɫ���Ե�λ��
	UINT nBtestpos; //��ɫ���Ե�λ��
	UINT nTPenable; //������ɫ 0����ɫ 1��ͼ��
} ColorTestParams, *pColorTestParams;

//****************** �ںϲ��� *****************
typedef struct __stBlendParam
{
	UINT nBlendSel;     // �ںϴ�ѡ��
	UINT nPureWhite;    // ����ģʽ
	UINT nAlpha;        // ��һ�����Alphaֵ
	UINT nP;            // ��һ�����Pֵ
	UINT nGamma;        // ��һ�����Gammaֵ

} BlendParams, *pBlendParams;

//******************** ����ģʽ *************
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
	{_T("1x2 Mode ˮƽ"),    2},
	{_T("1x3 Mode ˮƽ"),    3},
	{_T("1x4 Mode ˮƽ"),    4},
	{_T("1x5 Mode ˮƽ"),    5},
	{_T("1x6 Mode ˮƽ"),    6},

	{_T("2x1 Mode ��ֱ"),    2},
	{_T("3x1 Mode ��ֱ"),    3},
	{_T("4x1 Mode ��ֱ"),    4},
	{_T("5x1 Mode ��ֱ"),    5},
	{_T("6x1 Mode ��ֱ"),    6},

	{_T("2x2 Mode ����"),    4},
	{_T("3x2 Mode ����"),    6},
	{_T("2x3 Mode ����"),    6},

	{_T("1x1 Mode ����"),    1},   
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