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
// �豸����ģʽ
typedef enum __emWorkMode
{
	eWorkModeMatrix = 1,  //����ģʽ
	eWorkModeSplit  = 2,  //����ģʽ
} emWorkMode;



//********************************************************
// �Զ����û���Ϣ
#define WM_USER_SLIDER_LBTNUP    WM_USER + 1       /*���������������Ϣ�������ƻ�����������ɿ���ʱ�򴥷��������*/
#define WM_USER_CANCEL_EDIT      WM_USER + 2       /*Inplace Edit ȡ���༭*/
#define WM_USER_END_EDIT         WM_USER + 3       /*Inplace Edit ��ɱ༭*/
#define WM_USER_RECV_SCENE_DATA  WM_USER + 11      /*�����������ݣ��豸���ݣ�*/
#define WM_USER_UNPACK_SCENE     WM_USER + 12      /*���������ļ� */
#define WM_USER_PACKAGE_SCENE    WM_USER + 13      /*��������ļ� */
#define WM_USER_TRANS_COMPLETE   WM_USER + 1100    /*֪ͨ���̣߳����������*/


//********************************************************
// ͨ�ô��ڶ�ʱ��
#define TIMER_KILLFOCUS    (1000)


//********************************************************
// ͨ�ŷ�ʽ
#define NON_CONNECT  (0)       //ͨ��״̬����ǰ������
#define NET_CONNECT  (1)       //ͨ��״̬����ͨ����������
#define COM_CONNECT  (2)       //ͨ��״̬����ͨ����������
#define USB_CONNECT  (3)       //ͨ��״̬����ͨ��USB����
#define ALIVE_KEEP   (1)       //���߼��ʹ�ܱ�־

#define COM_MAX_OUTTIME (200)  //200��ReadFile()ʧ�ܺ�ָ�ʱ
#define NET_MAX_OUTTIME (10)   //10���׽���Recv()ʧ�ܺ�ָ�ʱ

// ��Ч�Ķ˿ں�
#define INVALID_PORT (0xFFFF)    //ͨ�Ŷ˿ں���Ч
#define SYSCARD_PORT (0x5802)    //ϵͳ�忨�˿ں�
#define SUBPORT_A    (0xA0000)   //���ź�1
#define SUBPORT_B    (0xB0000)   //���ź�2

// ָ��ִ��״̬
#define CMD_RTOK     (0x0)          //ָ�������
#define CMD_TIMEOUT  (0x10000)      //ָ��ִ�г�ʱ
#define CMD_RTER     CMD_TIMEOUT    //ָ��ش���
#define CMD_RTERCODE (0x10001)      //ָ��ش��󣬲����������
#define CMD_FAILURE  (-1)           //ָ��ʧ��

#define SAFE_BUFF    (256)          //��������ʱʹ��ͳһ���Ȳ���


//********************************************************
// ���������Ϣ
//

typedef struct tagIPADDR{
	UINT sub0; //��ַ��0
	UINT sub1; //��ַ��1
	UINT sub2; //��ַ��2
	UINT sub3; //��ַ��3
} IPADDR;

typedef struct tagNetParams {

	UINT mac[6];
	IPADDR ipaddr;
	IPADDR mask;
	IPADDR gateway;

} NETPARAMS,*pNETPARAMS;



//********************************************************
// ����MAX
//

#define _MAX_SIGNAL_NAME (128)      //�ź��������256���ֽڣ���256�����ֽ��ַ���128��˫�ֽ��ַ�
#define _MAX_OUTPUT_PORT (144)      //������ͨ����
#define _MAX_INPUT_PORT  (144)      //��������ź���
#define INVALID_VALUE    (-1)       //��Чֵ��־
#define ALL_SELECTED     (0xA000)   //�������ȫѡ��־



//********************************************************
// �ź�Դ���Ͷ���
//

#define _MAX_VIDEO_TYPE (14)        //�ź�Դ����

typedef enum __emVideoType
{
	vtCVBS    = 0x00,    // CVBS�ź�
	vtYpbpr   = 0x01,    // YPbPr�ź�
	vtVGA     = 0x02,    // VGA�ź�
	vtSDI     = 0x03,    // SDI�ź�
	vtHDMI    = 0x06,    // HDMI�ź�	
	vtDVI     = 0x08,    // DVI�ź�
	vtDP      = 0x09,    // DP�ź�
	vt4K_HDMI = 0x16,    // 4K HDMI
	vtHDBASET = 0x17,    // HDBASET
	vtDLDVI   = 0x18,    // Dual Link DVI
	vtPAL_D   = 0x20,    // 720x576i@50Hz
	vtNTSC_D  = 0x21,    // 720x480i@60Hz
	vtPAL_W   = 0x22,    // 960x576i@50Hz
	vtNTSC_W  = 0x23,    // 960x480i@60Hz
	vtUndef   = 0xff,    // ���ź�

} emVideoType;

CStringW GetVideoName(emVideoType eVT);
COLORREF GetVideoColor(emVideoType eVT);


//********************************************************
// �ֱ�����Ϣ
//

#define DEFAULT_RESOLUTION (0)  //�ڲ�Ĭ�Ϸֱ���
#define CUSTOM_RESOLUTION  (1)  //�û��Զ���ֱ���
#define UNKNOW_RESOULUTION (0xff)  //δ֪�ֱ���

typedef struct tagTiming {

	int m_PixelRate;  //unit 10KHz
	int m_HActive;    //ˮƽ�����	
	int	m_HBlank;     //ˮƽ�հ�
	int	m_VActive;    //��ֱ�����
	int	m_VBlank;	  //��ֱ�հ�
	int	m_HFPorch;    //ˮƽǰ��
	int	m_HSyncWid;   //ˮƽͬ�����
	int	m_VFPorch;    //��ֱǰ��
	int	m_VSyncWid;	  //��ֱͬ�����
	int	m_HSize;      //ˮƽͼ��ߴ�
	int	m_VSize;      //��ֱͼ��ߴ�
	int	m_HPol;       //ˮƽ����
	int	m_VPol;       //��ֱ����  
	int	m_HBorder;    //ˮƽ����
	int	m_VBorder;    //��ֱ����
	int m_Hz;         //ˢ����
	int m_HTotal;     //��ֱ������
	int m_VTotal;     //ˮƽ������
	int m_Type;       //�ֱ������� 0(DEFAULT_RESOLUTION):native; 1(CUSTOM_RESOLUTION):custiomized
	BOOL IsValid();
	BOOL operator==(const tagTiming &Tobj) const;

} TIMING,*pTIMING;

// Ԥ����ֱ�������
#define _MAX_ORG_RESOLUTION 11
// Ԥ����ֱ���
extern TIMING OriginalResolutions[_MAX_ORG_RESOLUTION];


//********************************************************
// ������Ϣ

typedef enum __emBackboardArrangeMode
{
	eBackboardArrangeModeLeftTop     =  1,  //�����ϰ忨��Ŵ������ң���������
	eBackboardArrangeModeLeftBottom  =  2,  //�����ϰ忨��Ŵ������ң���������
	eBackboardArrangeModeRightTop    =  3,  //�����ϰ忨��Ŵ������󣬴�������
	eBackboardArrangeModeRightBottom =  4   //�����ϰ忨��Ŵ������󣬴�������

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

	int m_nHorzNum;        //������������
	int m_nVertNum;        //������������
	int m_nDeviceID;       //�豸ID
	emBackboardArrangeMode m_eArrangeMode; //�����ϰ忨������з�ʽ

} BACKBOARDINFO, *pBACKBOARDINFO;



//********************************************************
// �忨��Ϣ

// �忨����
typedef enum __emCardType
{
	eCardTypeUndef   = 0,  //δ����
	eCardTypeCtrl    = 1,  //���ƿ�
	eCardTypeInput   = 2,  //����ɼ���
	eCardTypeOutput  = 3   //����ɼ���

} emCardType;

// �忨ID����
typedef enum __emCardID
{
	eMM10X_UNDEF     = 0xFFFF,  //δ����
	eMM10X_SYS_CTRL  = 0x5802,  //���ƿ�ID
	eMM10X_4IN_DVIM  = 0x7001,  //4��DVI����ɼ���
	eMM10X_2IN_DPDVI = 0x7101,  //2��DPDVI����ɼ���
	eMM10X_4OUT_DVII = 0x7201,  //4��DVI�����

} emCardID;

// �忨��Ϣ
typedef struct tagCardInfo {

	emCardID m_eCardID;
	emCardType m_eCardType;
	int m_nLogicPorts;

} CARDINFO, *pCARDINFO;

// �����������Ͱ忨����Ϣ
#define _MAX_CARD_DEF    (4)  //�忨����
extern CARDINFO CardInfo[_MAX_CARD_DEF];

// ���ݰ忨ID��ȡ�忨����
emCardType GetCardType(const UINT uCardID);


//////////////////////////////////////////////////////////////////////////
// �豸��Ϣ

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
// ����MAX
//

#define CURRENT_SCENE    (0)      //��ǰ������
#define _MAX_SCENE_NAME  (128)    //��󳡾����ַ�����128��char���ַ�
#define _MAX_SCENE_COUNT (128)    //�ײ�����ܴ洢�ĳ�������

// ���������ֵ��
typedef Pair<int, int> InOutKey;



//********************************************************
// �û���Ϣ
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
	char szUserName[_MAX_UNAME];     // �û���
	char szUserPwd[_MAX_PASSWORD];   // �û�����
	Authority eAuthority;            // Ȩ��

} UserData,* pUserData;



//********************************************************
// ���߳�״̬����
//

#define THREAD_IDLE          (0)  //�߳̿�ת
#define THREAD_WORK          (1)  //�̹߳���
#define THREAD_EXIT          (2)  //�߳��˳�
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
// �˿���Ϣ�ṹ��
//

typedef struct tagBasePort
{
	int         m_nID;                      //�˿�ID     (0 - N)��0��ʼ
	int         m_nCmdPort;                 //ͨ�Ŷ˿ں� (1 - N+1)��1��ʼ
	int         m_nSubPort;                   //��ǰ�߼��Ӷ˿ں� ��1��ʼ�����û���Ӷ˿���Ϊ-1
	char        m_szName[_MAX_SIGNAL_NAME]; //�˿�����
	TIMING      m_timing;                   //�˿ڷֱ���

	// ��ǰ�ź���Ϣ
	emVideoType m_eVT;                      //��ǰ�����ź�����
	int         m_nCurHAct;                 //��ǰ�ź�ˮƽ�ֱ���
	int         m_nCurVAct;                 //��ǰ�źŴ�ֱ�ֱ���
	int         m_nCurHz;                   //��ǰ�ź�Ƶ��

	// ����ֻ������˿���Ч
	int         m_nFeederID;                  //��Դ�ź�ID,
	
} BASEPORT;
// ����һ������˿�����
typedef BASEPORT OUTPORT;
// ����һ������˿�����
typedef BASEPORT INPORT;


//////////////////////////////////////////////////////////////////////////
// ͼ�������Ϣ
//
// ��������
enum emImageTestType
{
	eTestTypeColor = 0,
	eTestTypeGrid  = 2,
	eTestTypeCancelColor = 3,
	eTestTypeCancelGrid  = 4,
};
// ͼ�������Ϣ�ṹ��
typedef struct stImgTestInfo
{
	emImageTestType eTesttype; //��������
	COLORREF rgb;              //������ɫ
	CPoint ptBegin;            //ͼ����ʼ��
	CSize szLineSpace;         //������϶ cxΪˮƽ��϶��cyΪ��ֱ��϶
	int iLineWidth;            //�������

} IMGTESTINFO, *pIMGTESTINFO;


//////////////////////////////////////////////////////////////////////////
// ͼ�������Ϣ
//

#pragma pack(pop)

emCardType GetCardType(const UINT uCardID);

/****************** ϵͳɫ�� ********************/
extern COLORREF ListColorBackground;   //�б�򱳾���ɫ
extern COLORREF ListColorItemSelected;   //�б��ѡ�б�����ɫ
extern COLORREF ListColorActionFailed;   //�б���ִ��ʧ�ܱ�����ɫ
extern COLORREF ListColorActionSucceed;   //�б���ִ�гɹ�������ɫ
extern COLORREF ListColorText;   //�б��������ɫ

extern COLORREF DialogColorBackground;    //�Ի��򱳾���ɫ


// �ػ���
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

// ��̬�����ڴ������
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

/*************** �Ľ���ð������ *****************/
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


// ��������������
int Round(double value);

// ��CBitmapת����GDI+��Image����
BOOL BitmapToImage(CBitmap *pBitmap, Image *&pImg);
// GDI+ ������Դ�ļ� ���ô˺�����ǵ��ͷ�ָ����󣬲�Ȼ�ᵼ�³��������ڼ䲻��ռ���ڴ�
BOOL ImageFromIDResource(UINT nID, LPCTSTR lpType, Image *&pImg);