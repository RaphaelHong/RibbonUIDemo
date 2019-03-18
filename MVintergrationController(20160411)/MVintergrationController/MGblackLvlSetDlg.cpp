// MGblackLvlSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGblackLvlSetDlg.h"
#include "MGeoMainDlg.h"

#define TIMER_WAIT      100
#define WAIT_TIME       1500

// CMGblackLvlSetDlg �Ի���

IMPLEMENT_DYNAMIC(CMGblackLvlSetDlg, CXTPResizeDialog)

CMGblackLvlSetDlg::CMGblackLvlSetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGblackLvlSetDlg::IDD, pParent)
	, m_nLeakageSel(0)
	, m_nBlackLvlTestPos(0)
	, m_nEnableTest(0)
	, m_nEnableBlackLvl(0)
	, m_nFixedTestPos(0)
{
	//Ĭ�Ͼ��������ں���
	m_uBlendZone = 0x1FFF;
	//0x1FFF = TOPLEFT | TOPRIGHT | BOTTOMLEFT |  BOTTOMRIGHT | LEFT | TOP | RIGHT | BOTTOM | LEFTLEAK | TOPLEAK | RIGHTLEAK | BOTTOMLEAK | CENTER ;

	//Ĭ�ϵ�ǰ��ʾ��Ϊ���ں���
	m_uCurBlendZone = 12;	//Ĭ����ʾ���ں���

	//��ʼ���ڵ�ƽƫ��ֵ
	memset(m_nData, 0, sizeof(m_nData));

	//��ʼ���ڵ�ƽˮƽ�ε�ֵ
	m_ntickX[0] = 0;
	for (int i=1; i<X_TICK_NUM-1; i++) m_ntickX[i] = 16+i*8;
	m_ntickX[9] = 90;

	//��ʼ����ֱ�̶ȶε�ֵ
	for (int i=0; i<Y_TICK_NUM; i++) m_ntickY[i] = i*16;

	//Ĭ��ѡ�кڵ�ƽ��һ��
	m_nCurSection = 1;

	//Ĭ�ϵ�ǰδѡ���κ�һ��ˮƽ�̶�
	m_nCurTickX = -1;

	//��ʼ�������Ʊ���
	m_bLBtnDownAtTickArea = FALSE;

	//��ʼ����ֵΪFALSE����ʾ��ǰΪ�ڵ�ƽ����������ΪTRUE��ʱ���ʾ�̶ȵ���
	m_bTickOrBlackLvlAjust = FALSE;

	//��̶��Ƿ����ı�
	m_bTickPosChanged = FALSE;

	//�������Ƿ�������״���У�ÿһ����״������һ�κڵ�ƽ
	m_bLBtnDownAtCtrlColomnar = FALSE;

	//��ʼ��©������
	for(int iLeakage=0; iLeakage<4; iLeakage++) m_nLeakageWid[iLeakage][0] = m_nLeakageWid[iLeakage][1] = 0;
}

CMGblackLvlSetDlg::~CMGblackLvlSetDlg()
{
}

void CMGblackLvlSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_TOPLEFT_BLEND, m_btnBlendTL);
	DDX_Control(pDX, IDC_BTN_TOPRIGHT_BLEND, m_btnBlendTR);
	DDX_Control(pDX, IDC_BTN_BOTTOMLEFT_BLEND, m_btnBlendBL);
	DDX_Control(pDX, IDC_BTN_BOTTOMRIGHT_BLEND, m_btnBlendBR);
	DDX_Control(pDX, IDC_BTN_LEFT_BLEND, m_btnBlendL);
	DDX_Control(pDX, IDC_BTN_TOP_BLEND, m_btnBlendT);
	DDX_Control(pDX, IDC_BTN_RIGHT_BLEND, m_btnBlendR);
	DDX_Control(pDX, IDC_BTN_BOTTOM_BLEND, m_btnBlendB);
	DDX_Control(pDX, IDC_BTN_LEFT_LEAKAGE, m_btnLeakageL);
	DDX_Control(pDX, IDC_BTN_TOP_LEAKAGE, m_btnLeakageT);
	DDX_Control(pDX, IDC_BTN_RIGHT_LEAKAGE, m_btnLeakageR);
	DDX_Control(pDX, IDC_BTN_BOTTOM_LEAKAGE, m_btnLeakageB);
	DDX_Control(pDX, IDC_BTN_NON_BLEND, m_btnBlendNon);
	DDX_Control(pDX, IDC_BTN_SET_LEAKAGEWID, m_btnSetLeakWidth);
	DDX_Control(pDX, IDC_BTN_RESET_BLACKLVL, m_btnResetBlackLvl);

	DDX_Control(pDX, IDC_RADIO_LEFT_LEAKAGE, m_RadioLeakage[0]);
	DDX_Control(pDX, IDC_RADIO_TOP_LEAKAGE, m_RadioLeakage[1]);
	DDX_Control(pDX, IDC_RADIO_RIGHT_LEAKAGE, m_RadioLeakage[2]);
	DDX_Control(pDX, IDC_RADIO_BOTTOM_LEAKAGE, m_RadioLeakage[3]);

	DDX_Control(pDX, IDC_EDIT_TOPOF_LEFTLEAKAGE, m_EditLeakageTL);
	DDX_Control(pDX, IDC_EDIT_LEFTOF_TOPLEAKAGE, m_EditLeakageLT);
	DDX_Control(pDX, IDC_EDIT_TOPOF_RIGHTLEAKAGE, m_EditLeakageTR);
	DDX_Control(pDX, IDC_EDIT_LEFTOF_BOTTOMLEAKAGE, m_EditLeakageLB);
	DDX_Control(pDX, IDC_EDIT_RIGHTOF_TOPLEAKAGE, m_EditLeakageRT);
	DDX_Control(pDX, IDC_EDIT_BOTTOMOF_LEFTLEAKAGE, m_EditLeakageBL);
	DDX_Control(pDX, IDC_EDIT_RIGHTOF_BOTTOMLEAKAGE, m_EditLeakageRB);
	DDX_Control(pDX, IDC_EDIT_BOTTOMOF_RIGHTLEAKAGE, m_EditLeakageBR);

	DDX_Control(pDX, IDC_SPIN_TOPOF_LEFTLEAKAGE, m_SpinLeakageTL);
	DDX_Control(pDX, IDC_SPIN_LEFTOF_TOPLEAKAGE, m_SpinLeakageLT);
	DDX_Control(pDX, IDC_SPIN_TOPOF_RIGHTLEAKAGE, m_SpinLeakageTR);
	DDX_Control(pDX, IDC_SPIN_LEFTOF_BOTTOMLEAKAGE, m_SpinLeakageLB);
	DDX_Control(pDX, IDC_SPIN_RIGHTOF_TOPLEAKAGE, m_SpinLeakageRT);
	DDX_Control(pDX, IDC_SPIN_BOTTOMOF_LEFTLEAKAGE, m_SpinLeakageBL);
	DDX_Control(pDX, IDC_SPIN_RIGHTOF_BOTTOMLEAKAGE, m_SpinLeakageRB);
	DDX_Control(pDX, IDC_SPIN_BOTTOMOF_RIGHTLEAKAGE, m_SpinLeakageBR);

	DDX_Control(pDX, IDC_CHECK_ENABLE_BLACKLVL_TEST, m_CheckEnableTest);
	DDX_Control(pDX, IDC_EDIT_TEST_LVLPOS, m_EditTestPos);
	DDX_Control(pDX, IDC_SPIN_TEST_LVLPOS, m_SpinTestPos);
	DDX_Control(pDX, IDC_CHECK_TESTPOS_FIXED, m_CheckFixedTestPos);
	DDX_Control(pDX, IDC_CHECK_ENABLE_BLACKLVL, m_CheckEnableBlackLvl);

	DDX_Radio(pDX, IDC_RADIO_LEFT_LEAKAGE, m_nLeakageSel);
	DDX_Text(pDX, IDC_EDIT_TOPOF_LEFTLEAKAGE,     m_nLeakageWid[0][0]); //���©�������
	DDX_Text(pDX, IDC_EDIT_BOTTOMOF_LEFTLEAKAGE,  m_nLeakageWid[0][1]); //���©����ײ�
	DDX_Text(pDX, IDC_EDIT_LEFTOF_TOPLEAKAGE,     m_nLeakageWid[1][0]); //����©�����
	DDX_Text(pDX, IDC_EDIT_RIGHTOF_TOPLEAKAGE,    m_nLeakageWid[1][1]); //����©����Ҳ�
	DDX_Text(pDX, IDC_EDIT_TOPOF_RIGHTLEAKAGE,    m_nLeakageWid[2][0]); //�ұ�©�������
	DDX_Text(pDX, IDC_EDIT_BOTTOMOF_RIGHTLEAKAGE, m_nLeakageWid[2][1]); //�ұ�©����ײ�
	DDX_Text(pDX, IDC_EDIT_LEFTOF_BOTTOMLEAKAGE,  m_nLeakageWid[3][0]); //�ײ�©�����
	DDX_Text(pDX, IDC_EDIT_RIGHTOF_BOTTOMLEAKAGE, m_nLeakageWid[3][1]); //�ײ�©����Ҳ�
	DDX_Text(pDX, IDC_EDIT_TEST_LVLPOS, m_nBlackLvlTestPos);
	DDX_Check(pDX, IDC_CHECK_ENABLE_BLACKLVL_TEST, m_nEnableTest);
	DDX_Check(pDX, IDC_CHECK_TESTPOS_FIXED, m_nFixedTestPos);
	DDX_Check(pDX, IDC_CHECK_ENABLE_BLACKLVL, m_nEnableBlackLvl);
}


BEGIN_MESSAGE_MAP(CMGblackLvlSetDlg, CXTPResizeDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_COMMAND_RANGE(IDC_BTN_TOPLEFT_BLEND, IDC_BTN_NON_BLEND, OnBnClickedBlendzone)
	ON_NOTIFY_RANGE(UDN_DELTAPOS, IDC_SPIN_TOPOF_LEFTLEAKAGE, IDC_SPIN_RIGHTOF_BOTTOMLEAKAGE, OnDeltaposSpinLeakageWid)
	ON_BN_CLICKED(IDC_BTN_SET_LEAKAGEWID, OnBnClickedSetLeakageWid)
	ON_COMMAND_RANGE(IDC_RADIO_LEFT_LEAKAGE, IDC_RADIO_BOTTOM_LEAKAGE, OnBnRadioLeakageSel)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_BLACKLVL_TEST, OnBnCheckEnableBlackLvlTest)
	ON_BN_CLICKED(IDC_CHECK_TESTPOS_FIXED, &CMGblackLvlSetDlg::OnBnClickedCheckTestposFixed)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_BLACKLVL, &CMGblackLvlSetDlg::OnBnClickedCheckEnableBlacklvl)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TEST_LVLPOS, &CMGblackLvlSetDlg::OnDeltaposSpinTestLvlpos)
	ON_BN_CLICKED(IDC_BTN_RESET_BLACKLVL, &CMGblackLvlSetDlg::OnBnClickedBtnResetBlacklvl)
END_MESSAGE_MAP()


// CMGblackLvlSetDlg ��Ϣ�������

void CMGblackLvlSetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// !!! �õ��Ǿ�̬ͼƬ�ؼ���DC��ͼ�����ǶԻ����DC ��ע�⣡

	CDC *pDC = GetDlgItem(IDC_PIC_BLACKLVL)->GetDC();

	CDC *dcMem = new CDC;
	dcMem->CreateCompatibleDC(pDC);

	//��ͼ��ƫ�� ����ͼƬ�ؼ��ڶԻ����е�λ��
	CRect rcPaint = m_rcPaint;
	rcPaint.OffsetRect(-rcPaint.left, -rcPaint.top);
	rcPaint.InflateRect(-1, -1);

	// �������
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rcPaint.Width(), rcPaint.Height());
	CBitmap *poldbmp= dcMem->SelectObject(&MemBitmap); // ������λͼ
	dcMem->FillSolidRect(0, 0, rcPaint.Width(), rcPaint.Height(), RGB(90,216,250)); // ��ָ����ɫ���λͼ

	if (m_uBlendZone > 0)
	{
		DrawAxis(dcMem);  //������

		DrawTitle(dcMem); //������
 
		DrawData(dcMem);  //����ͼ
	}

	pDC->BitBlt(rcPaint.left, rcPaint.top, rcPaint.Width(), rcPaint.Height(), dcMem, 0, 0, SRCCOPY);

	// Select MemBitmap out of dcMem, and delete MemBitmap object;
	dcMem->SelectObject(poldbmp);
	MemBitmap.DeleteObject();

	dcMem->DeleteDC();
	delete dcMem;
	dcMem = NULL;

	ReleaseDC(pDC);
}

void CMGblackLvlSetDlg::DrawAxis(CDC *pValidDC)
{
	CFont *pOldFont = pValidDC->SelectObject(&m_markFont);
	CString strMark;

	CPen pen, dotpen, markpen, *pOldpen;
	dotpen.CreatePen(PS_DOT, 1, RGB(1,1,1));
	pen.CreatePen(PS_SOLID, 1, RGB(240, 245, 250));
	markpen.CreatePen(PS_SOLID, 1, RGB(240,240,100));

	// ��������
	pValidDC->MoveTo(m_rcChart.left, m_rcChart.top);
	pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom+5);
	pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom);
	pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom);

	pOldpen = pValidDC->SelectObject(&dotpen);
	pValidDC->LineTo(m_rcChart.right+20, m_rcChart.bottom);
	pValidDC->MoveTo(m_rcChart.left, m_rcChart.top);
	pValidDC->LineTo(m_rcChart.left, m_rcChart.top-20);
	pValidDC->SelectObject(pOldpen);

	// ����̶�
	for (int i=0; i<X_TICK_NUM; i++)
	{
		if (m_nCurTickX == i)
		{
			pOldpen = pValidDC->SelectObject(&pen);
			pValidDC->SetTextColor(RGB(240,240,100));
		}
		pValidDC->MoveTo(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale), m_rcChart.bottom+5);

		strMark.Format(_T("%d"), m_ntickX[i]);
		pValidDC->TextOut(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale)-3, m_rcChart.bottom+8, strMark);
		if (m_nCurTickX == i)
		{
			pValidDC->SelectObject(pOldpen);
			pValidDC->SetTextColor(RGB(1,1,1));
		}
	}

	pOldpen = pValidDC->SelectObject(&pen);
	// ����̶�
	for (int i=1; i<Y_TICK_NUM; i++) //��1��ʼ��������һ���ߣ�����ͺ������ظ�
	{
		strMark.Format(_T("%d"), m_ntickY[i]);
		pValidDC->TextOut(m_rcChart.left-20, m_rcChart.bottom-Round(m_ntickY[i]*m_dYscale)-3, strMark);

		pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(m_ntickY[i]*m_dYscale));
		pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom-Round(m_ntickY[i]*m_dYscale));
	}

	pValidDC->MoveTo(m_rcChart.left+10+Round(80*m_dXscale), m_rcChart.bottom);
	pValidDC->LineTo(m_rcChart.left+10+Round(80*m_dXscale), m_rcChart.top+5);

	pValidDC->SelectObject(pOldFont);
	pValidDC->SelectObject(pOldpen);
}

void CMGblackLvlSetDlg::DrawTitle(CDC *pValidDC)
{
	CFont *pOldfont = pValidDC->SelectObject(&m_titleFont);

	CString str;
	GetTitleString(str);

	CSize textExt;
	textExt = pValidDC->GetTextExtent(str);

	pValidDC->SetBkMode(TRANSPARENT);

	pValidDC->SetTextColor(RGB(50,50,50));
	pValidDC->TextOut(int(m_rcChart.left+(m_rcChart.Width()-textExt.cx)*0.5)+1, m_rcChart.top - 20+1, str);
	pValidDC->SetTextColor(RGB(26,120,255));
	pValidDC->TextOut(int(m_rcChart.left+(m_rcChart.Width()-textExt.cx)*0.5), m_rcChart.top - 20, str);


	pValidDC->SelectObject(pOldfont);
}

void CMGblackLvlSetDlg::DrawData(CDC *pValidDC)
{
	CRgn rgn;
	CPoint pt[4];
	CBrush br;
	CString strMark;

	CFont *pOldfont = pValidDC->SelectObject(&m_markFont);
	for (int i=0; i<X_TICK_NUM-1; i++)
	{
		pt[0] = CPoint(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale), m_rcChart.bottom-Round(GetSectionBeginVal(i)*m_dYscale));
		if (i==8) pt[1] = CPoint(m_rcChart.left+10+Round((m_ntickX[i+1])*m_dXscale), m_rcChart.bottom-Round(GetSectionEndVal(i)*m_dYscale));
		else pt[1] = CPoint(m_rcChart.left+10+Round((m_ntickX[i+1])*m_dXscale), m_rcChart.bottom-Round((GetSectionEndVal(i)+1)*m_dYscale));
		pt[2] = CPoint(m_rcChart.left+10+Round((m_ntickX[i+1])*m_dXscale), m_rcChart.bottom);
		pt[3] = CPoint(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale), m_rcChart.bottom);

		// �����״��
		rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
		if (m_nCurSection == i)
		{
			br.CreateSolidBrush(RGB(10, 100, 200));
			pValidDC->SetTextColor(RGB(250,150,50));
		}
		else
		{
			br.CreateSolidBrush(RGB(100+i*15, 100+i*10, 100+i*8));
			pValidDC->SetTextColor(RGB(255,255,100));
		}
		pValidDC->FillRgn(&rgn, &br);
		br.DeleteObject();
		rgn.DeleteObject();

		// ����״���ϱ�ע�ڵ�ƽ��
		if (i==8) strMark.Format(_T("%d-%d"), GetSectionBeginVal(i), GetSectionEndVal(i));
		else strMark.Format(_T("%d-%d"), GetSectionBeginVal(i), GetSectionEndVal(i));

		CSize textExt;
		textExt = pValidDC->GetTextExtent(strMark);
		int nPosY = m_rcChart.bottom-int ((m_rcChart.bottom-pt[1].y)*0.5);
		int nPosX = pt[0].x + int((pt[1].x-pt[0].x-textExt.cx)*0.5);
		pValidDC->SetBkMode(TRANSPARENT);
		pValidDC->TextOut(nPosX, nPosY, strMark);

		strMark.Format(_T("(%d)"), m_nData[m_uCurBlendZone][i]);
		textExt = pValidDC->GetTextExtent(strMark);
		nPosX = pt[0].x + int((pt[1].x-pt[0].x-textExt.cx)*0.5);
		nPosY = m_rcChart.bottom-int ((m_rcChart.bottom-pt[1].y)*0.5)+textExt.cy;
		pValidDC->TextOut(nPosX, nPosY, strMark);
	}

	CPen dotpen, *poldpen;
	dotpen.CreatePen(PS_SOLID, 1, RGB(50,50,30));
	poldpen = pValidDC->SelectObject(&dotpen);
	pValidDC->MoveTo(m_rcChart.left+10, m_rcChart.bottom);
	pValidDC->LineTo(m_rcChart.left+10+Round(80*m_dXscale), m_rcChart.bottom-Round(80*m_dYscale));

	pValidDC->SelectObject(poldpen);
	pValidDC->SelectObject(pOldfont);
}

BOOL CMGblackLvlSetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	//�Զ����ڿؼ���С��λ��
	ResizeIt();

	//���ÿؼ����
	SetControlsTheme(xtpControlThemeResource);

	//��ȡ��ͼ��λ��
	GetPaintRect();

	//������ͼ���õ�����
	CreateCustomFont();

	//��ʾ�������ں���ѡ��ť
	ShowBlendSelBtn();

	//��Ĭ�ϵ��ں���ѡ��ť�����
	m_btnBlendNon.SetChecked(TRUE);

	//����������ˢ
	m_bkBrush.CreateSolidBrush(constClrDlgBk[4]);

	//���༭�������ڵ�΢����ť����
	SetBuddys();

	return TRUE; 
}

void CMGblackLvlSetDlg::CreateCustomFont()
{
	VERIFY(m_markFont.CreateFont(
		10,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename

	VERIFY(m_titleFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		TRUE,                      // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename
}

void CMGblackLvlSetDlg::GetPaintRect()
{
	GetDlgItem(IDC_PIC_BLACKLVL)->GetWindowRect(&m_rcPaint);
	ScreenToClient(&m_rcPaint);

	m_rcChart = m_rcPaint;
	m_rcChart.OffsetRect(-m_rcChart.left, -m_rcChart.top);
	m_rcChart.InflateRect(-1,-1);
	m_rcChart.InflateRect(-25, -25, -20, -25);

	m_dXscale = double(m_rcChart.Width()-15)/90.0;
	m_dYscale = double(m_rcChart.Height()-10)/127.0;
}

void CMGblackLvlSetDlg::ResizeIt()
{
	SetResize(IDC_BTN_TOPLEFT_BLEND, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_TOP_BLEND, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_BTN_TOP_LEAKAGE, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_BTN_TOPRIGHT_BLEND, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BTN_RIGHT_BLEND, XTP_ANCHOR_MIDDLERIGHT, XTP_ANCHOR_MIDDLERIGHT);
	SetResize(IDC_BTN_RIGHT_LEAKAGE, XTP_ANCHOR_MIDDLERIGHT, XTP_ANCHOR_MIDDLERIGHT);
	SetResize(IDC_BTN_NON_BLEND, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BTN_LEFT_BLEND, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_BTN_LEFT_LEAKAGE, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_BTN_BOTTOMLEFT_BLEND, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(IDC_BTN_BOTTOM_BLEND, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(IDC_BTN_BOTTOM_LEAKAGE, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(IDC_BTN_BOTTOMRIGHT_BLEND, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_PIC_BLACKLVL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	for (int i=IDC_GBOX_LEAKAGE_WID; i<=IDC_GBOX_ENABLE_BLACK; i++)
	{
		SetResize(i, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	}
}

void CMGblackLvlSetDlg::SetControlsTheme(XTPControlTheme theme)
{
	m_btnBlendL.SetTheme(theme);
	m_btnBlendT.SetTheme(theme);
	m_btnBlendR.SetTheme(theme);
	m_btnBlendB.SetTheme(theme);
	m_btnBlendTL.SetTheme(theme);
	m_btnBlendTR.SetTheme(theme);
	m_btnBlendBL.SetTheme(theme);
	m_btnBlendBR.SetTheme(theme);
	m_btnLeakageL.SetTheme(theme);
	m_btnLeakageT.SetTheme(theme);
	m_btnLeakageR.SetTheme(theme);
	m_btnLeakageB.SetTheme(theme);
	m_btnBlendNon.SetTheme(theme);
	m_btnSetLeakWidth.SetTheme(theme);
	m_btnResetBlackLvl.SetTheme(theme);
	m_RadioLeakage[0].SetTheme(theme);
	m_RadioLeakage[1].SetTheme(theme);
	m_RadioLeakage[2].SetTheme(theme);
	m_RadioLeakage[3].SetTheme(theme);
	m_EditLeakageTL.SetTheme(theme);
	m_EditLeakageBL.SetTheme(theme);
	m_EditLeakageLT.SetTheme(theme);
	m_EditLeakageRT.SetTheme(theme);
	m_EditLeakageTR.SetTheme(theme);
	m_EditLeakageBR.SetTheme(theme);
	m_EditLeakageLB.SetTheme(theme);
	m_EditLeakageRB.SetTheme(theme);
	m_SpinLeakageTL.SetTheme(theme);
	m_SpinLeakageBL.SetTheme(theme);
	m_SpinLeakageLT.SetTheme(theme);
	m_SpinLeakageRT.SetTheme(theme);
	m_SpinLeakageTR.SetTheme(theme);
	m_SpinLeakageBR.SetTheme(theme);
	m_SpinLeakageLB.SetTheme(theme);
	m_SpinLeakageRB.SetTheme(theme);
	m_CheckEnableTest.SetTheme(theme);
	m_EditTestPos.SetTheme(theme);
	m_SpinTestPos.SetTheme(theme);
	m_CheckFixedTestPos.SetTheme(theme);
	m_CheckEnableBlackLvl.SetTheme(theme);
}

void CMGblackLvlSetDlg::SetBuddys()
{
	m_SpinLeakageTL.SetBuddy(&m_EditLeakageTL);
	m_SpinLeakageBL.SetBuddy(&m_EditLeakageBL);
	m_SpinLeakageLT.SetBuddy(&m_EditLeakageLT);
	m_SpinLeakageRT.SetBuddy(&m_EditLeakageRT);
	m_SpinLeakageTR.SetBuddy(&m_EditLeakageTR);
	m_SpinLeakageBR.SetBuddy(&m_EditLeakageBR);
	m_SpinLeakageLB.SetBuddy(&m_EditLeakageLB);
	m_SpinLeakageRB.SetBuddy(&m_EditLeakageRB);
	m_SpinTestPos.SetBuddy(&m_EditTestPos);
}

HBRUSH CMGblackLvlSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_BTN)
	{
		return m_bkBrush;
	}

	return hbr;
}

void CMGblackLvlSetDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if (GetDlgItem(IDC_PIC_BLACKLVL)->GetSafeHwnd()) GetPaintRect();
}


void CMGblackLvlSetDlg::SetTotalBlendZone()
{
	m_uBlendZone = CENTER; //��ʼ�������и����ں���

	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();

	if (pDlg->GetCurrencyBlendWidth(0) > 0) m_uBlendZone |= (LEFT|LEFTLEAK);    //��������ں���
	if (pDlg->GetCurrencyBlendWidth(1) > 0) m_uBlendZone |= (TOP|TOPLEAK);      //���ж����ں���
	if (pDlg->GetCurrencyBlendWidth(2) > 0) m_uBlendZone |= (RIGHT|RIGHTLEAK);  //�����ұ��ں���
	if (pDlg->GetCurrencyBlendWidth(3) > 0) m_uBlendZone |= (BOTTOM|BOTTOMLEAK);//���еײ��ں���
	if ((pDlg->GetCurrencyBlendWidth(0) > 0) && (pDlg->GetCurrencyBlendWidth(1) > 0)) m_uBlendZone |= TOPLEFT;    //���������ں���
	if ((pDlg->GetCurrencyBlendWidth(1) > 0) && (pDlg->GetCurrencyBlendWidth(2) > 0)) m_uBlendZone |= TOPRIGHT;   //���������ں���
	if ((pDlg->GetCurrencyBlendWidth(0) > 0) && (pDlg->GetCurrencyBlendWidth(3) > 0)) m_uBlendZone |= BOTTOMLEFT; //���������ں���
	if ((pDlg->GetCurrencyBlendWidth(2) > 0) && (pDlg->GetCurrencyBlendWidth(3) > 0)) m_uBlendZone |= BOTTOMRIGHT;//���������ں���

	m_uCurBlendZone = 12; //Ĭ�ϵ����ں���

	InvalidateRect(m_rcChart);

	ShowBlendSelBtn();
}

void CMGblackLvlSetDlg::EnableLeakageWidthControls(int nLeakageSel, BOOL bEnable)
{
	GetDlgItem(IDC_TEXT_TOPOF_LEFTLEAKAGE+nLeakageSel)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TOPOF_LEFTLEAKAGE+nLeakageSel)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_TOPOF_LEFTLEAKAGE+nLeakageSel)->EnableWindow(bEnable);

	GetDlgItem(IDC_TEXT_BOTTOMOF_LEFTLEAKAGE+nLeakageSel)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_BOTTOMOF_LEFTLEAKAGE+nLeakageSel)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_BOTTOMOF_LEFTLEAKAGE+nLeakageSel)->EnableWindow(bEnable);
}

void CMGblackLvlSetDlg::ShowBlendSelBtn()
{
	GetDlgItem(IDC_BTN_TOPLEFT_BLEND)->ShowWindow((m_uBlendZone&TOPLEFT) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_TOPRIGHT_BLEND)->ShowWindow((m_uBlendZone&TOPRIGHT) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_TOP_BLEND)->ShowWindow((m_uBlendZone&TOP) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_LEFT_BLEND)->ShowWindow((m_uBlendZone&LEFT) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_RIGHT_BLEND)->ShowWindow((m_uBlendZone&RIGHT) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_BOTTOM_BLEND)->ShowWindow((m_uBlendZone&BOTTOM) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_BOTTOMLEFT_BLEND)->ShowWindow((m_uBlendZone&BOTTOMLEFT) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_BOTTOMRIGHT_BLEND)->ShowWindow((m_uBlendZone&BOTTOMRIGHT) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_LEFT_LEAKAGE)->ShowWindow((m_uBlendZone&LEFTLEAK) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_TOP_LEAKAGE)->ShowWindow((m_uBlendZone&TOPLEAK) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_RIGHT_LEAKAGE)->ShowWindow((m_uBlendZone&RIGHTLEAK) > 0 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BTN_BOTTOM_LEAKAGE)->ShowWindow((m_uBlendZone&BOTTOMLEAK) > 0 ? SW_SHOW : SW_HIDE);
}

inline int CMGblackLvlSetDlg::GetSectionBeginVal(const int iSec)
{
	return m_ntickX[iSec] + m_nData[m_uCurBlendZone][iSec];
}

inline int CMGblackLvlSetDlg::GetSectionEndVal(const int iSec)
{
	int nVal=0;
	if (iSec == 8)
		nVal = m_ntickX[iSec] + m_nData[m_uCurBlendZone][iSec] + m_ntickX[iSec+1] - m_ntickX[iSec];
	else
		nVal = m_ntickX[iSec] + m_nData[m_uCurBlendZone][iSec] + m_ntickX[iSec+1] - m_ntickX[iSec]-1;

	return nVal;
}

void CMGblackLvlSetDlg::GetTitleString(CString &strtitle)
{
	if (m_uCurBlendZone == 0) strtitle = _T("�����ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 1) strtitle = _T("�����ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 2) strtitle = _T("�����ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 3) strtitle = _T("�����ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 4) strtitle = _T("���ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 5) strtitle = _T("�����ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 6) strtitle = _T("�ҷ��ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 7) strtitle = _T("�ײ��ں��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 8) strtitle = _T("���©��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 9) strtitle = _T("����©��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 10) strtitle = _T("�Ҳ�©��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 11) strtitle = _T("�ײ�©��� �� �ڵ�ƽ");
	else if (m_uCurBlendZone == 12) strtitle = _T("���ں��� �� �ڵ�ƽ");
}

void CMGblackLvlSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	m_bLBtnDownAtCtrlColomnar = IsBtnDownAtCtrlColomnar(point);

	if (!m_bLBtnDownAtCtrlColomnar)
		m_bLBtnDownAtTickArea = IsBtnDownAtTickArea(point);

	CXTPResizeDialog::OnLButtonDown(nFlags, point);
}

void CMGblackLvlSetDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_bLBtnDownAtCtrlColomnar = FALSE;
	m_bLBtnDownAtTickArea = FALSE;

	CXTPResizeDialog::OnLButtonUp(nFlags, point);
}

void CMGblackLvlSetDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	if (m_bLBtnDownAtCtrlColomnar)
	{
		//�����ڵ�ƽ
		int nNewValue = int(double(m_rcChart.bottom-point.y)/m_dYscale);
		SetBlackLevel(nNewValue);
	}
	else if (m_bLBtnDownAtTickArea)
	{
		//�����̶�λ��
		int nNewPos = int(double(point.x - (m_rcChart.left+10))/m_dXscale);
		SetTickPos(nNewPos);
	}

	CXTPResizeDialog::OnMouseMove(nFlags, point);
}

BOOL CMGblackLvlSetDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_bTickOrBlackLvlAjust) SetTickPos(m_ntickX[m_nCurTickX]+zDelta/120);
	else OffsetBlackLevel(zDelta/120);

	return CXTPResizeDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CMGblackLvlSetDlg::IsBtnDownAtCtrlColomnar(CPoint point)
{
	CPoint pt[4];
	CRgn rgn;

	for (int i=0; i<X_TICK_NUM-1; i++)
	{
		pt[0] = CPoint(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale), m_rcChart.bottom-Round(GetSectionBeginVal(i)*m_dYscale));
		if (i==8) pt[1] = CPoint(m_rcChart.left+10+Round((m_ntickX[i+1])*m_dXscale)-1, m_rcChart.bottom-Round(GetSectionEndVal(i)*m_dYscale));
		else pt[1] = CPoint(m_rcChart.left+10+Round((m_ntickX[i+1])*m_dXscale)-1, m_rcChart.bottom-Round((GetSectionEndVal(i)+1)*m_dYscale));
		pt[2] = CPoint(m_rcChart.left+10+Round((m_ntickX[i+1])*m_dXscale)-1, m_rcChart.bottom);
		pt[3] = CPoint(m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale), m_rcChart.bottom);

		// �����״��
		rgn.CreatePolygonRgn(pt, 4, ALTERNATE);

		if (rgn.PtInRegion(point))
		{
			m_nCurSection = i;
			if(m_nFixedTestPos == 0)  //���ڲ��Ե������ܶε�Ӱ��ʱ �Ų��ԣ����ε����ֵ�̶�ʱ�����ٸı����֮
			{
				m_nBlackLvlTestPos = int((m_ntickX[m_nCurSection] + m_ntickX[m_nCurSection+1])*0.5);

				CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
				theApp.MG_EnableBlacklvlTest(m_nBlackLvlTestPos, m_nEnableTest, pOwner->m_boardInfo.GetBoardPort());
			}

			UpdateData(FALSE);

			InvalidateRect(m_rcPaint);
			return TRUE;
		}

		rgn.DeleteObject();
	}

	return FALSE;
}

BOOL CMGblackLvlSetDlg::IsBtnDownAtTickArea(CPoint point)
{
	CRect rcTick;
	for (int i=1 ;i<X_TICK_NUM-1; i++)
	{
		rcTick = CRect (m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale)-4,
			m_rcChart.bottom+1,
			m_rcChart.left+10+Round(m_ntickX[i]*m_dXscale)+4,
			m_rcChart.bottom+20);

		if (PtInRect(rcTick, point))
		{
			m_bLBtnDownAtTickArea = TRUE;
			m_nCurTickX = i;
			InvalidateRect(m_rcPaint);
			if (!m_bTickPosChanged) SetTimer(TIMER_WAIT, WAIT_TIME, NULL);
			m_bTickOrBlackLvlAjust = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}

void CMGblackLvlSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_WAIT)
	{
		if (!m_bTickPosChanged && m_bTickOrBlackLvlAjust)
		{
			m_bTickOrBlackLvlAjust = FALSE;
			m_nCurTickX = -1;
			KillTimer(TIMER_WAIT);
			InvalidateRect(m_rcPaint);
		}

		m_bTickPosChanged = FALSE;
	}

	CXTPResizeDialog::OnTimer(nIDEvent);
}


void CMGblackLvlSetDlg::SetBlackLevel(int nNewLvl)
{
	int nEnd = 0;
	int nStart = 0;
	if (m_nCurSection < 8) //ǰ�˶�
	{
		nStart = m_ntickX[m_nCurSection];
		nEnd = m_ntickX[m_nCurSection+1];

		if (nNewLvl < m_ntickX[m_nCurSection] ) nNewLvl = m_ntickX[m_nCurSection];
		if (nNewLvl+nEnd-nStart > 128) nNewLvl = 128-nEnd+nStart+1;		
	}
	else //���һ��
	{
		nStart = m_ntickX[m_nCurSection];	
		nEnd = m_ntickX[m_nCurSection+1]+1;

		if (nNewLvl < m_ntickX[m_nCurSection] ) nNewLvl = m_ntickX[m_nCurSection];
		if (nNewLvl+nEnd-nStart > 128) nNewLvl = 128-nEnd+nStart+1;
	}

	m_nData[m_uCurBlendZone][m_nCurSection] = nNewLvl - m_ntickX[m_nCurSection];

	//����
	//CmdWBLV();
	WriteBlackLevel();

	InvalidateRect(m_rcPaint);
}

void CMGblackLvlSetDlg::SetTickPos(int nPos)
{
	m_bTickPosChanged = TRUE;

	if(nPos < 0 || nPos > 80) return;

	if (m_nCurTickX == 0)
	{
		if (nPos > m_ntickX[1]-2) return;
	}
	else if (m_nCurTickX == 8)
	{
		if ( nPos < m_ntickX[7]+2) return;
	}
	else
	{
		if (nPos > m_ntickX[m_nCurTickX+1] - 2) return;
		if (nPos < m_ntickX[m_nCurTickX-1] + 2) return;
	}

	m_ntickX[m_nCurTickX] = nPos;

	//CmdWLVT();
	WriteBlackSection();

	InvalidateRect(m_rcPaint);
}

void CMGblackLvlSetDlg::OffsetBlackLevel(int nOffset)
{
	if (m_nCurSection < 8) //ǰ8��
	{
		if (GetSectionEndVal(m_nCurSection)+nOffset > 128 
			||	GetSectionBeginVal(m_nCurSection)+nOffset < m_ntickX[m_nCurSection])
		{	return;	}
	}
	else //���һ�� 
	{
		if (GetSectionEndVal(m_nCurSection)+nOffset > 128
			|| GetSectionBeginVal(m_nCurSection)+nOffset < m_ntickX[m_nCurSection])
		{	return;	}
	}

	m_nData[m_uCurBlendZone][m_nCurSection] += nOffset;

	//����
	//CmdWBLV();
	WriteBlackLevel();

	InvalidateRect(m_rcPaint);
}

BOOL CMGblackLvlSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP)
		{
			OffsetBlackLevel(1);
			return TRUE;
		}
		else if (pMsg->wParam == VK_DOWN)
		{
			OffsetBlackLevel(-1);
			return TRUE;
		}
		else if (pMsg->wParam == VK_RIGHT)
		{
			if (m_bTickOrBlackLvlAjust)
			{
				SetTickPos(m_ntickX[m_nCurTickX]+1);
			}
			else
			{
				m_nCurSection++;
				m_nCurSection = m_nCurSection%9;

				if(m_nFixedTestPos == 0)  //���ڲ��Ե������ܶε�Ӱ��ʱ �Ų��ԣ����ε����ֵ�̶�ʱ�����ٸı����ֵ
				{
					m_nBlackLvlTestPos = int((m_ntickX[m_nCurSection] + m_ntickX[m_nCurSection+1])*0.5);

					CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
					theApp.MG_EnableBlacklvlTest(m_nBlackLvlTestPos, m_nEnableTest, pOwner->m_boardInfo.GetBoardPort());
				}
				UpdateData(FALSE);
			}

			InvalidateRect(m_rcPaint);
			return TRUE;
		}
		else if (pMsg->wParam == VK_LEFT)
		{
			if (m_bTickOrBlackLvlAjust)
			{
				SetTickPos(m_ntickX[m_nCurTickX]-1);
			}
			else
			{
				m_nCurSection = (m_nCurSection--)%9;
				if (m_nCurSection < 0) m_nCurSection +=9; 

				if(m_nFixedTestPos == 0)  //���ڲ��Ե������ܶε�Ӱ��ʱ �Ų��ԣ����ε����ֵ�̶�ʱ�����ٸı����ֵ
				{
					m_nBlackLvlTestPos = int((m_ntickX[m_nCurSection] + m_ntickX[m_nCurSection+1])*0.5);
					CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
					theApp.MG_EnableBlacklvlTest(m_nBlackLvlTestPos, m_nEnableTest, pOwner->m_boardInfo.GetBoardPort());
				}
				UpdateData(FALSE);
			}

			InvalidateRect(m_rcPaint);
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == GetDlgItem(IDC_EDIT_TEST_LVLPOS))
			{
				//�س����������Զε� 
				UpdateData();
				CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
				theApp.MG_EnableBlacklvlTest(m_nBlackLvlTestPos, m_nEnableTest, pOwner->m_boardInfo.GetBoardPort());
			}

			//��ֹEnter���˳�
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			//��ֹEsc���˳�
			return TRUE;
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

BOOL CMGblackLvlSetDlg::OnEraseBkgnd(CDC* pDC)
{
	return CXTPResizeDialog::OnEraseBkgnd(pDC);
}


void CMGblackLvlSetDlg::WriteBlackLevel()
{
	UINT nData[13];
	for (int i=0; i<13; i++) nData[i] = m_nData[i][m_nCurSection];
	CMGeoMainDlg *pOwer = (CMGeoMainDlg *)GetOwner();
	theApp.MG_WriteBlackLvl(nData, m_nCurSection, pOwer->m_boardInfo.GetBoardPort());
}

void CMGblackLvlSetDlg::WriteBlackSection()
{
	UINT nTick[8];
	for (int i=0; i<8; i++) nTick[i] = m_ntickX[i+1];
	CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
	theApp.MG_SetBlackLvlTickPos(nTick, pOwer->m_boardInfo.GetBoardPort());
}

void CMGblackLvlSetDlg::SetLeakageWidth(int nLeakageSel)
{
	UpdateData();

	CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
	theApp.MG_SetLeakageWidth(nLeakageSel, m_nLeakageWid[nLeakageSel][0], m_nLeakageWid[nLeakageSel][1], pOwer->m_boardInfo.GetBoardPort());
}

void CMGblackLvlSetDlg::OnBnClickedBlendzone(UINT nID)
{
	int nZone = nID-IDC_BTN_TOPLEFT_BLEND;
	if (m_uCurBlendZone == nZone) return; //�ںϴ�ѡ��û��

	// ȡ����һ��ѡ��ť�ı��
	CXTPButton *pBtn = (CXTPButton*)GetDlgItem(IDC_BTN_TOPLEFT_BLEND+m_uCurBlendZone);
	pBtn->SetChecked(FALSE);

	// ����ǰѡ��İ�ť�����
	m_uCurBlendZone = nZone;
	pBtn = (CXTPButton *)GetDlgItem(nID);
	pBtn->SetChecked(TRUE);

	InvalidateRect(m_rcPaint);
}

void CMGblackLvlSetDlg::OnBnRadioLeakageSel(UINT nID)
{
	UpdateData();

	for (int iLeakage=0; iLeakage<4; iLeakage++)
		EnableLeakageWidthControls(iLeakage, iLeakage == m_nLeakageSel);
}

void CMGblackLvlSetDlg::OnDeltaposSpinLeakageWid(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nLeakageSel = (nID-IDC_SPIN_TOPOF_LEFTLEAKAGE)%4;
	int nSide = (nID-IDC_SPIN_TOPOF_LEFTLEAKAGE)/4;

	int nNewWid =  m_nLeakageWid[nLeakageSel][nSide] - pNMUpDown->iDelta;
	m_nLeakageWid[nLeakageSel][nSide] = nNewWid < 0 ? 0 : nNewWid;

	UpdateData(FALSE);

	SetLeakageWidth(nLeakageSel);
}

void CMGblackLvlSetDlg::OnBnClickedSetLeakageWid()
{
	UpdateData();

	SetLeakageWidth(m_nLeakageSel);
}

void CMGblackLvlSetDlg::OnBnCheckEnableBlackLvlTest()
{
	UpdateData();

	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	if(theApp.MG_EnableBlacklvlTest(m_nBlackLvlTestPos, m_nEnableTest, pOwner->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("�Բ���, ����ʧ�ܣ�"), _T("�����ڵ�ƽ���Ի���"), MB_OK|MB_ICONSTOP);

		m_nEnableTest = !m_nEnableTest;
		UpdateData(FALSE);
		return;
	}

	GetDlgItem(IDC_TEXT_TEST_LVLPOS)->EnableWindow(m_nEnableTest);
	GetDlgItem(IDC_EDIT_TEST_LVLPOS)->EnableWindow(m_nEnableTest);
	GetDlgItem(IDC_SPIN_TEST_LVLPOS)->EnableWindow(m_nEnableTest);	
}

void CMGblackLvlSetDlg::OnBnClickedCheckEnableBlacklvl()
{
	UpdateData();

	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	if(theApp.MG_EnableBlackLevelFunc(m_nEnableBlackLvl, pOwner->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("�Բ���, ����ʧ�ܣ�"), _T("�����ڵ�ƽ"), MB_OK|MB_ICONSTOP);

		m_nEnableBlackLvl = !m_nEnableBlackLvl;
		UpdateData(FALSE);
	}
}

void CMGblackLvlSetDlg::OnBnClickedCheckTestposFixed()
{
	//ˢ�¿��Ʊ���
	UpdateData();
}

void CMGblackLvlSetDlg::Initialize()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	int nPort = pOwner->m_boardInfo.GetBoardPort();
	if (nPort == 0xff) return;

	//��ȡ�ڵ�ƽ�ε�
	UINT uParam[64] = {0};

	if (theApp.MG_GetBlackLvlTickPos(uParam, nPort) != CMD_FAILURE)
	{
		for (int i=0; i<8; i++)
			m_ntickX[i+1] = uParam[i];
	}
	else
	{
		MessageBox(_T("��ȡ�ڵ�ƽ�ε�̶�ֵʧ�ܣ�"), _T("�ڵ�ƽ��ʼ��"), MB_OK|MB_ICONERROR);
		return;
	}

	//��ȡ�ڵ�ƽֵ
	memset(uParam, 0, sizeof(uParam));

	for (int iSec=0; iSec<9; iSec++)
	{
		if (theApp.MG_ReadBlackLvl(uParam, iSec, nPort) != CMD_FAILURE)
		{
			for (int jBlendZone=0; jBlendZone<13; jBlendZone++)
			{
				m_nData[jBlendZone][iSec] = uParam[jBlendZone];
			}
		}
		else
		{
			MessageBox(_T("��ȡ�ڵ�ƽʧ�ܣ�"), _T("�ڵ�ƽ��ʼ��"), MB_OK | MB_ICONERROR);
			return;
		}
	}
	
	//��ȡ©������
	memset(uParam, 0, sizeof(uParam));

	if (theApp.MG_GetLeakageWidth(uParam, nPort) != CMD_FAILURE)
	{
		for (int i=0; i<4; i++)
		{
			m_nLeakageWid[i][0] = uParam[i*2+0];
			m_nLeakageWid[i][1] = uParam[i*2+1];
		}
	}
	else
	{
		MessageBox(_T("��ȡ©������ʧ�ܣ�"), _T("�ڵ�ƽ��ʼ��"), MB_OK|MB_ICONERROR);
		return;
	}

	//��ȡ�ڵ�ƽ���ܿ������
	memset(uParam, 0, sizeof(uParam));

	if (theApp.MG_GetBlackLevelFuncEnable(uParam[0], nPort) != CMD_FAILURE)
	{
		m_nEnableBlackLvl = uParam[0];
	}
	else
	{
		MessageBox(_T("��ȡ�ڵ�ƽ���ܿ�����־ʧ�ܣ�"), _T("�ڵ�ƽ��ʼ��"), MB_OK|MB_ICONERROR);
		return;
	}

	SetTotalBlendZone(); //��ʾ�ڵ�ƽѡ��ť

	for (int i=0; i<4; i++)
	{
		m_RadioLeakage[i].SetCheck(0);
		EnableLeakageWidthControls(i, FALSE);
	}

	for (int i=0; i<4; i++)
	{
		if (pOwner->GetCurrencyBlendWidth(i) > 0)
		{
			m_nLeakageSel = i;
			m_RadioLeakage[i].SetCheck(1);
			EnableLeakageWidthControls(i, TRUE);
		}
	}

	UpdateData(FALSE);
}

void CMGblackLvlSetDlg::OnDeltaposSpinTestLvlpos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_nBlackLvlTestPos -= pNMUpDown->iDelta;

	m_nBlackLvlTestPos = (m_nBlackLvlTestPos>255) ? 255 : m_nBlackLvlTestPos;
	m_nBlackLvlTestPos = (m_nBlackLvlTestPos<0)   ? 0   : m_nBlackLvlTestPos;

	UpdateData(FALSE);

	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	theApp.MG_EnableBlacklvlTest(m_nBlackLvlTestPos, m_nEnableTest, pOwner->m_boardInfo.GetBoardPort());

	*pResult = 0;
}

void CMGblackLvlSetDlg::OnBnClickedBtnResetBlacklvl()
{
	Reset();
}

void CMGblackLvlSetDlg::Reset()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	if (theApp.MG_ResetBlackLvl(pOwner->m_boardInfo.GetBoardPort()) != CMD_FAILURE) //RTOK
	{
		ResetBlackLevel();
		ResetTickPos();
		InvalidateRect(m_rcPaint);

		MessageBox(_T("��λ�ɹ���"), _T("��λ�ڵ�ƽ"), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("�Բ��𣬸�λʧ�ܣ�����ͨ���Ƿ�������"), _T("��λ�ڵ�ƽ"), MB_OK|MB_ICONSTOP);
	}
}

void CMGblackLvlSetDlg::ResetTickPos()
{
	for (int i=0; i<9; i++) m_nData[m_uCurBlendZone][i] = 0;
}

void CMGblackLvlSetDlg::ResetBlackLevel()
{
	m_ntickX[0] = 0;
	for (int i=1; i<X_TICK_NUM-1; i++) m_ntickX[i] = 16+i*8;
	m_ntickX[9] = 90;
}