// MGeometricAjustDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGeometricAjustDlg.h"
#include "MGeoMainDlg.h"


// CMGeometricAjustDlg �Ի���

IMPLEMENT_DYNAMIC(CMGeometricAjustDlg, CXTPResizeDialog)

CMGeometricAjustDlg::CMGeometricAjustDlg(CWnd* pParent /*=NULL*/)
: CXTPResizeDialog(CMGeometricAjustDlg::IDD, pParent)
{
	//Ĭ�ϵ�ǰУ��Ϊ�ĵ�У��
	m_eAjustStyle = eKeystone;

	//��ʼ��ָ��
	m_pDpaOrigpoints    = NULL;
	m_pDpa4points       = NULL;
	m_pDpaCurvepoints   = NULL;
	m_pDpaDensitypoints = NULL;

	//�߼�������
	m_bLBtnDownAt4Point = FALSE;
	m_bLBtnDownAtCurve  = FALSE;
	m_bLBtnDownAtDensity= FALSE;

	//��ǰ���Ƶ�ĳ�ʼλ��
	m_ptCursor4point    = CPoint(0,0);
	m_ptCursorCurve     = CPoint(0,0);
	m_ptCursorDensity   = CPoint(int(m_gridsetting.cx/2), int(m_gridsetting.cy/2));

	//��ͼ���������ߴ�
	m_nDeflate = 5;
}

CMGeometricAjustDlg::~CMGeometricAjustDlg()
{
	DeleteDpaData();
}

void CMGeometricAjustDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_KEYSTONE, m_btnKeystone);
	DDX_Control(pDX, IDC_BTN_CURVE, m_btnCurve);
	DDX_Control(pDX, IDC_BTN_DENSITY, m_btnDensity);
	DDX_Control(pDX, IDC_BTN_GEO_RESET, m_btnReset);
	DDX_Control(pDX, IDC_EDIT_TL_POINT_X, m_editTL[0]);
	DDX_Control(pDX, IDC_SPIN_TL_POINT_X, m_spinTL[0]);
	DDX_Control(pDX, IDC_EDIT_TL_POINT_Y, m_editTL[1]);
	DDX_Control(pDX, IDC_SPIN_TL_POINT_Y, m_spinTL[1]);

	DDX_Control(pDX, IDC_EDIT_TC_POINT_X, m_editTC[0]);
	DDX_Control(pDX, IDC_SPIN_TC_POINT_X, m_spinTC[0]);
	DDX_Control(pDX, IDC_EDIT_TC_POINT_Y, m_editTC[1]);
	DDX_Control(pDX, IDC_SPIN_TC_POINT_Y, m_spinTC[1]);

	DDX_Control(pDX, IDC_EDIT_TR_POINT_X, m_editTR[0]);
	DDX_Control(pDX, IDC_SPIN_TR_POINT_X, m_spinTR[0]);
	DDX_Control(pDX, IDC_EDIT_TR_POINT_Y, m_editTR[1]);
	DDX_Control(pDX, IDC_SPIN_TR_POINT_Y, m_spinTR[1]);

	DDX_Control(pDX, IDC_EDIT_LC_POINT_X, m_editLC[0]);
	DDX_Control(pDX, IDC_SPIN_LC_POINT_X, m_spinLC[0]);
	DDX_Control(pDX, IDC_EDIT_LC_POINT_Y, m_editLC[1]);
	DDX_Control(pDX, IDC_SPIN_LC_POINT_Y, m_spinLC[1]);

	DDX_Control(pDX, IDC_EDIT_RC_POINT_X, m_editRC[0]);
	DDX_Control(pDX, IDC_SPIN_RC_POINT_X, m_spinRC[0]);
	DDX_Control(pDX, IDC_EDIT_RC_POINT_Y, m_editRC[1]);
	DDX_Control(pDX, IDC_SPIN_RC_POINT_Y, m_spinRC[1]);

	DDX_Control(pDX, IDC_EDIT_BL_POINT_X, m_editBL[0]);
	DDX_Control(pDX, IDC_SPIN_BL_POINT_X, m_spinBL[0]);
	DDX_Control(pDX, IDC_EDIT_BL_POINT_Y, m_editBL[1]);
	DDX_Control(pDX, IDC_SPIN_BL_POINT_Y, m_spinBL[1]);

	DDX_Control(pDX, IDC_EDIT_BC_POINT_X, m_editBC[0]);
	DDX_Control(pDX, IDC_SPIN_BC_POINT_X, m_spinBC[0]);
	DDX_Control(pDX, IDC_EDIT_BC_POINT_Y, m_editBC[1]);
	DDX_Control(pDX, IDC_SPIN_BC_POINT_Y, m_spinBC[1]);

	DDX_Control(pDX, IDC_EDIT_BR_POINT_X, m_editBR[0]);
	DDX_Control(pDX, IDC_SPIN_BR_POINT_X, m_spinBR[0]);
	DDX_Control(pDX, IDC_EDIT_BR_POINT_Y, m_editBR[1]);
	DDX_Control(pDX, IDC_SPIN_BR_POINT_Y, m_spinBR[1]);

	DDX_Control(pDX, IDC_EDIT_MC_POINT_X, m_editMC[0]);
	DDX_Control(pDX, IDC_SPIN_MC_POINT_X, m_spinMC[0]);
	DDX_Control(pDX, IDC_EDIT_MC_POINT_Y, m_editMC[1]);
	DDX_Control(pDX, IDC_SPIN_MC_POINT_Y, m_spinMC[1]);

	DDX_Control(pDX, IDC_EDIT_EX1_POINT_X, m_editExX);
	DDX_Control(pDX, IDC_SPIN_EX1_POINT_X, m_spinExX);
	DDX_Control(pDX, IDC_EDIT_EX1_POINT_Y, m_editExY[0]);
	DDX_Control(pDX, IDC_SPIN_EX1_POINT_Y, m_spinExY[0]);
	DDX_Control(pDX, IDC_EDIT_EX2_POINT_Y, m_editExY[1]);
	DDX_Control(pDX, IDC_SPIN_EX2_POINT_Y, m_spinExY[1]);
}


BEGIN_MESSAGE_MAP(CMGeometricAjustDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_KEYSTONE, IDC_BTN_DENSITY, OnBnClickedAjustStyle)
	ON_BN_CLICKED(IDC_BTN_GEO_RESET, OnBnClickedReset)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_NOTIFY_RANGE(UDN_DELTAPOS, IDC_SPIN_TL_POINT_X, IDC_SPIN_BC_POINT_Y, &CMGeometricAjustDlg::OnDeltaposSpinPointPos)
END_MESSAGE_MAP()


// CMGeometricAjustDlg ��Ϣ�������

inline int CMGeometricAjustDlg::GetBoardPort()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();

	return pDlg->m_boardInfo.GetBoardPort();
}

BOOL CMGeometricAjustDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	//���µ����ؼ���λ��
	ResizeIt();

	//�ؼ��ķ��
	SetControlsTheme(xtpControlThemeResource);

	//������ʾ��ť
	SetToolTips();

	//��΢����ť��༭�����
	SetBuddys();

	//������ˢ
	m_bkBrush.CreateSolidBrush(constClrDlgBk[5]);

	//��ʼ��ѡ���ĵ�У��
	m_btnKeystone.SetChecked(TRUE);

	//��ʾ�ĵ�У���ؼ�����������
	ShowKeystoneCtrls();

	//��ʼ����������
	InitDpaPoints();

	//��ʼ���༭������
	Show4pointCoords();

	//��ȡ��ͼ����
	GetPaintRect();

	return TRUE; 
}

void CMGeometricAjustDlg::ResizeIt()
{
	//��ͼ�ؼ�
	SetResize(IDC_PIC_GEO, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	//���Ρ���������ܶ��л���ť
	SetResize(IDC_BTN_KEYSTONE, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_CURVE, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_DENSITY, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);

	//��λ��ť
	SetResize(IDC_BTN_GEO_RESET, CXTPResizePoint(.75, 0), CXTPResizePoint(.75, 0));

	//���Ƶ�λ�ñ༭�ؼ�
	//TOPLEFT
	SetResize(IDC_EDIT_TL_POINT_X, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_SPIN_TL_POINT_X, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_EDIT_TL_POINT_Y, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_SPIN_TL_POINT_Y, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	//TOPCENTER
	SetResize(IDC_EDIT_TC_POINT_X, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_SPIN_TC_POINT_X, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_EDIT_TC_POINT_Y, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_SPIN_TC_POINT_Y, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	//TOPRIGHT
	SetResize(IDC_EDIT_TR_POINT_X, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SPIN_TR_POINT_X, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_EDIT_TR_POINT_Y, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SPIN_TR_POINT_Y, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	//LEFTCENTER
	SetResize(IDC_EDIT_LC_POINT_X, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_SPIN_LC_POINT_X, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_EDIT_LC_POINT_Y, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_SPIN_LC_POINT_Y, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	//RIGHTCENTER
	SetResize(IDC_EDIT_RC_POINT_X, XTP_ANCHOR_MIDDLERIGHT, XTP_ANCHOR_MIDDLERIGHT);
	SetResize(IDC_SPIN_RC_POINT_X, XTP_ANCHOR_MIDDLERIGHT, XTP_ANCHOR_MIDDLERIGHT);
	SetResize(IDC_EDIT_RC_POINT_Y, XTP_ANCHOR_MIDDLERIGHT, XTP_ANCHOR_MIDDLERIGHT);
	SetResize(IDC_SPIN_RC_POINT_Y, XTP_ANCHOR_MIDDLERIGHT, XTP_ANCHOR_MIDDLERIGHT);
	//BOTTOMLEFT
	SetResize(IDC_EDIT_BL_POINT_X, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(IDC_SPIN_BL_POINT_X, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(IDC_EDIT_BL_POINT_Y, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(IDC_SPIN_BL_POINT_Y, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	//BOTTOMCENTER
	SetResize(IDC_EDIT_BC_POINT_X, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(IDC_SPIN_BC_POINT_X, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(IDC_EDIT_BC_POINT_Y, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(IDC_SPIN_BC_POINT_Y, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	//BOTTOMRIGHT
	SetResize(IDC_EDIT_BR_POINT_X, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_SPIN_BR_POINT_X, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_EDIT_BR_POINT_Y, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_SPIN_BR_POINT_Y, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	//MIDDLECENTER
	SetResize(IDC_EDIT_MC_POINT_X, CXTPResizePoint(0.75, 1), CXTPResizePoint(0.75, 1));
	SetResize(IDC_SPIN_MC_POINT_X, CXTPResizePoint(0.75, 1), CXTPResizePoint(0.75, 1));
	SetResize(IDC_EDIT_MC_POINT_Y, CXTPResizePoint(0.75, 1), CXTPResizePoint(0.75, 1));
	SetResize(IDC_SPIN_MC_POINT_Y, CXTPResizePoint(0.75, 1), CXTPResizePoint(0.75, 1));
	//EXTRA X
	SetResize(IDC_EDIT_EX1_POINT_X, CXTPResizePoint(.25, 1), CXTPResizePoint(.25, 1));
	SetResize(IDC_SPIN_EX1_POINT_X, CXTPResizePoint(.25, 1), CXTPResizePoint(.25, 1));
	//EXTRA Y 1
	SetResize(IDC_EDIT_EX1_POINT_Y, CXTPResizePoint(1, .25), CXTPResizePoint(1, .25));
	SetResize(IDC_SPIN_EX1_POINT_Y, CXTPResizePoint(1, .25), CXTPResizePoint(1, .25));
	//EXTRA Y 2
	SetResize(IDC_EDIT_EX2_POINT_Y, CXTPResizePoint(1, .75), CXTPResizePoint(1, .75));
	SetResize(IDC_SPIN_EX2_POINT_Y, CXTPResizePoint(1, .75), CXTPResizePoint(1, .75));
}

void CMGeometricAjustDlg::SetControlsTheme(XTPControlTheme theme)
{
	m_btnKeystone.SetTheme(theme);
	m_btnCurve.SetTheme(theme);
	m_btnDensity.SetTheme(theme);
	m_btnReset.SetTheme(theme);
	for (int i=0; i<2; i++)
	{
		m_editTL[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinTL[i].SetTheme(theme);
		
		m_editTC[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinTC[i].SetTheme(theme);

		m_editTR[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinTR[i].SetTheme(theme);

		m_editLC[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinLC[i].SetTheme(theme);

		m_editRC[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinRC[i].SetTheme(theme);

		m_editBL[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinBL[i].SetTheme(theme);

		m_editBC[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinBC[i].SetTheme(theme);

		m_editBR[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinBR[i].SetTheme(theme);

		m_editMC[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinMC[i].SetTheme(theme);

		m_editExY[i].SetTheme(xtpControlThemeUltraFlat);
		m_spinExY[i].SetTheme(theme);
	}

	m_editExX.SetTheme(xtpControlThemeUltraFlat);
	m_spinExX.SetTheme(theme);
}

void CMGeometricAjustDlg::SetToolTips()
{
	m_btnKeystone.SetTooltip(_T("�л�������У��"));
	m_btnCurve.SetTooltip(_T("�л�������У��"));
	m_btnDensity.SetTooltip(_T("�л�������У��"));
	m_btnReset.SetTooltip(_T("��λ��ǰ����"));
}

void CMGeometricAjustDlg::SetBuddys()
{
	for (int i=0; i<2; i++)
	{
		//����
		m_spinTL[i].SetBuddy(&m_editTL[i]);
		//����
		m_spinTC[i].SetBuddy(&m_editTC[i]);
		//����
		m_spinTR[i].SetBuddy(&m_editTR[i]);
		//����
		m_spinLC[i].SetBuddy(&m_editLC[i]);
		//����
		m_spinRC[i].SetBuddy(&m_editRC[i]);
		//����
		m_spinBL[i].SetBuddy(&m_editBL[i]);
		//����
		m_spinBC[i].SetBuddy(&m_editBC[i]);
		//����
		m_spinBR[i].SetBuddy(&m_editBR[i]);
		//����
		m_spinMC[i].SetBuddy(&m_editMC[i]);

		m_spinExY[i].SetBuddy(&m_editExY[i]);
	}

	m_spinExX.SetBuddy(&m_editExX);
}

void CMGeometricAjustDlg::ShowKeystoneCtrls()
{
	for (int i=0; i<2; i++)
	{
		// Show 4 corner controls
		//����
		m_editTL[i].ShowWindow(SW_SHOW);
		m_spinTL[i].ShowWindow(SW_SHOW);
		//����
		m_editTR[i].ShowWindow(SW_SHOW);
		m_spinTR[i].ShowWindow(SW_SHOW);
		//����
		m_editBL[i].ShowWindow(SW_SHOW);
		m_spinBL[i].ShowWindow(SW_SHOW);
		//����
		m_editBR[i].ShowWindow(SW_SHOW);
		m_spinBR[i].ShowWindow(SW_SHOW);

		// Hide the others
		m_editTC[i].ShowWindow(SW_HIDE);
		m_spinTC[i].ShowWindow(SW_HIDE);

		m_editLC[i].ShowWindow(SW_HIDE);
		m_spinLC[i].ShowWindow(SW_HIDE);

		m_editRC[i].ShowWindow(SW_HIDE);
		m_spinRC[i].ShowWindow(SW_HIDE);

		m_editBC[i].ShowWindow(SW_HIDE);
		m_spinBC[i].ShowWindow(SW_HIDE);

		m_editMC[i].ShowWindow(SW_HIDE);
		m_spinMC[i].ShowWindow(SW_HIDE);

		m_editExY[i].ShowWindow(SW_HIDE);
		m_spinExY[i].ShowWindow(SW_HIDE);
	}

	m_editExX.ShowWindow(SW_HIDE);
	m_spinExX.ShowWindow(SW_HIDE);
}

void CMGeometricAjustDlg::ShowCurveCtrls()
{
	for (int i=0; i<2; i++)
	{
		// Show 9 point controls
		//����
		m_editTL[i].ShowWindow(SW_SHOW);
		m_spinTL[i].ShowWindow(SW_SHOW);
		//����
		m_editTC[i].ShowWindow(SW_SHOW);
		m_spinTC[i].ShowWindow(SW_SHOW);
		//����
		m_editTR[i].ShowWindow(SW_SHOW);
		m_spinTR[i].ShowWindow(SW_SHOW);
		//����
		m_editLC[i].ShowWindow(SW_SHOW);
		m_spinLC[i].ShowWindow(SW_SHOW);
		//����
		m_editRC[i].ShowWindow(SW_SHOW);
		m_spinRC[i].ShowWindow(SW_SHOW);
		//����
		m_editBL[i].ShowWindow(SW_SHOW);
		m_spinBL[i].ShowWindow(SW_SHOW);
		//����
		m_editBC[i].ShowWindow(SW_SHOW);
		m_spinBC[i].ShowWindow(SW_SHOW);
		//����
		m_editBR[i].ShowWindow(SW_SHOW);
		m_spinBR[i].ShowWindow(SW_SHOW);
		//����
		m_editMC[i].ShowWindow(SW_SHOW);
		m_spinMC[i].ShowWindow(SW_SHOW);

		// Hide the others
		m_editExY[i].ShowWindow(SW_HIDE);
		m_spinExY[i].ShowWindow(SW_HIDE);
	}

	m_editExX.ShowWindow(SW_HIDE);
	m_spinExX.ShowWindow(SW_HIDE);
}

void CMGeometricAjustDlg::ShowDensityCtrls()
{
	// Show density point controls
	//��1
	m_editBL[0].ShowWindow(SW_SHOW);
	m_spinBL[0].ShowWindow(SW_SHOW);
	//��2
	m_editExX.ShowWindow(SW_SHOW);
	m_spinExX.ShowWindow(SW_SHOW);
	//��3
	m_editBC[0].ShowWindow(SW_SHOW);
	m_spinBC[0].ShowWindow(SW_SHOW);
	//��4
	m_editMC[0].ShowWindow(SW_SHOW);
	m_spinMC[0].ShowWindow(SW_SHOW);
	//��5
	m_editBR[0].ShowWindow(SW_SHOW);
	m_spinBR[0].ShowWindow(SW_SHOW);
	//��1
	m_editTR[1].ShowWindow(SW_SHOW);
	m_spinTR[1].ShowWindow(SW_SHOW);
	//��2
	m_editExY[0].ShowWindow(SW_SHOW);
	m_spinExY[0].ShowWindow(SW_SHOW);
	//��3
	m_editRC[1].ShowWindow(SW_SHOW);
	m_spinRC[1].ShowWindow(SW_SHOW);
	//��4
	m_editExY[1].ShowWindow(SW_SHOW);
	m_spinExY[1].ShowWindow(SW_SHOW);
	//��5
	m_editBR[1].ShowWindow(SW_SHOW);
	m_spinBR[1].ShowWindow(SW_SHOW);


	// Hide the others
	for (int i=0; i<2; i++)
	{
		m_editTL[i].ShowWindow(SW_HIDE);
		m_spinTL[i].ShowWindow(SW_HIDE);

		m_editTC[i].ShowWindow(SW_HIDE);
		m_spinTC[i].ShowWindow(SW_HIDE);

		m_editLC[i].ShowWindow(SW_HIDE);
		m_spinLC[i].ShowWindow(SW_HIDE);
	}

	m_editBL[1].ShowWindow(SW_HIDE);
	m_spinBL[1].ShowWindow(SW_HIDE);

	m_editBC[1].ShowWindow(SW_HIDE);
	m_spinBC[1].ShowWindow(SW_HIDE);

	m_editMC[1].ShowWindow(SW_HIDE);
	m_spinMC[1].ShowWindow(SW_HIDE);

	m_editTR[0].ShowWindow(SW_HIDE);
	m_spinTR[0].ShowWindow(SW_HIDE);

	m_editRC[0].ShowWindow(SW_HIDE);
	m_spinRC[0].ShowWindow(SW_HIDE);
}

void CMGeometricAjustDlg::GetPaintRect()
{
	GetDlgItem(IDC_PIC_GEO)->GetWindowRect(&m_rcPaint);
	ScreenToClient(&m_rcPaint);

	m_rcChart = m_rcPaint;
	m_rcChart.OffsetRect(-m_rcChart.left, -m_rcChart.top);
	m_rcChart.InflateRect(-m_nDeflate,-m_nDeflate);

	SetGraphicSize();
}

void CMGeometricAjustDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if (GetDlgItem(IDC_PIC_GEO)->GetSafeHwnd()) GetPaintRect();
}

HBRUSH CMGeometricAjustDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return HBRUSH(m_bkBrush);
	}

	return hbr;
}

void CMGeometricAjustDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//��ȡ��ͼDC
	CDC *pDC = GetDlgItem(IDC_PIC_GEO)->GetDC();

	//�����ڴ�DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//�����ڴ�λͼ
	CBitmap MemBmp, *pOldBmp;
	MemBmp.CreateCompatibleBitmap(pDC, m_rcPaint.Width(), m_rcPaint.Height());
	pOldBmp = pMemDC->SelectObject(&MemBmp);

	//��䱳��ɫ
	pMemDC->FillSolidRect(0, 0, m_rcPaint.Width(), m_rcPaint.Height(), RGB(205,225,255));

	//������
	DrawBkgGrid(pMemDC);

	//�ֱ�����Ϣ
	DrawResolution(pMemDC);

	//������������������
	if (m_eAjustStyle == eKeystone) DrawKeystoneData(pMemDC);
	else if (m_eAjustStyle == eCurve) DrawCurveData(pMemDC);
	else if (m_eAjustStyle == eDensity) DrawDensityData(pMemDC);

	//�����ڴ�DC���ݵ���ͼDC��
	pDC->BitBlt(0, 0, m_rcPaint.Width(), m_rcPaint.Height(), pMemDC, 0, 0, SRCCOPY);

	//�ͷ���Դ
	pMemDC->DeleteDC();
	delete pMemDC;
	pMemDC = NULL;

	ReleaseDC(pDC);
}


void CMGeometricAjustDlg::DrawBkgGrid(CDC *pValidDC)
{
	//���������ϱ���ɫ
	pValidDC->FillSolidRect(m_rcChart, RGB(50,30,60));

	CPen GoldPen, *pOldpen;
	GoldPen.CreatePen(PS_DOT, 1, RGB(34, 50, 170)); //RGB(194,200,40)
	pOldpen = pValidDC->SelectObject(&GoldPen);

	double dScaleX = double(m_rcChart.Width()) / 16;
	double dScaleY = double(m_rcChart.Height())/ 16;

	for (int i=0; i<17; i++) {
		pValidDC->MoveTo(m_rcChart.left, m_rcChart.top+Round(i*dScaleY));
		pValidDC->LineTo(m_rcChart.right, m_rcChart.top+Round(i*dScaleY));

		pValidDC->MoveTo(m_rcChart.left+Round(dScaleX*i), m_rcChart.top);
		pValidDC->LineTo(m_rcChart.left+Round(dScaleX*i), m_rcChart.bottom);
	}

	pValidDC->SelectObject(pOldpen);

}

void CMGeometricAjustDlg::DrawKeystoneData(CDC *pValidDC)
{
	CPen Redpen, Greenpen, Graypen, *pOldpen;
	Redpen.CreatePen(PS_SOLID, 2, RGB(255,50,30));
	Greenpen.CreatePen(PS_SOLID, 2, RGB(50,255,30));
	Graypen.CreatePen(PS_SOLID, 1, RGB(147,186,138));

	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//�м������� �Ұ�
	pValidDC->SelectObject(&Graypen);
	//��������
	for (int x=1; x<cx-1; x++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpa4points->X(x, 0)*m_xScale),    m_nDeflate+Round(m_pDpa4points->Y(x, 0)*m_yScale));
		pValidDC->LineTo(m_nDeflate+Round(m_pDpa4points->X(x, cy-1)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(x, cy-1)*m_yScale));
	}
	//��������
	for (int y=1; y<cy-1; y++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpa4points->X(0, y)*m_xScale),    m_nDeflate+Round(m_pDpa4points->Y(0, y)*m_yScale));
		pValidDC->LineTo(m_nDeflate+Round(m_pDpa4points->X(cx-1, y)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(cx-1, y)*m_yScale));
	}

	pValidDC->MoveTo(m_nDeflate+Round(m_pDpa4points->X(0, 0)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(0, 0)*m_yScale));
	//�������� ��ɫ
	pOldpen = pValidDC->SelectObject(&Redpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpa4points->X(cx-1, 0)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(cx-1, 0)*m_yScale));
	//�������� ��ɫ
	pValidDC->SelectObject(&Greenpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpa4points->X(cx-1, cy-1)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(cx-1, cy-1)*m_yScale));
	//�������� ��ɫ
	pValidDC->SelectObject(&Redpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpa4points->X(0, cy-1)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(0, cy-1)*m_yScale));
	//�������� ��ɫ
	pValidDC->SelectObject(&Greenpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpa4points->X(0, 0)*m_xScale), m_nDeflate+Round(m_pDpa4points->Y(0, 0)*m_yScale));

	//�ĸ����ϵĿ��Ƶ�
	CBrush br, brCtrl, *pOldBr;
	br.CreateSolidBrush(RGB(255,255,100));
	brCtrl.CreateSolidBrush(RGB(100,100,255));
	int nRadius = 7; //����Բ��뾶��С
	//���Ͻ�
	pOldBr = pValidDC->SelectObject((m_ptCursor4point == CPoint(0,0)) ? &brCtrl : &br);
	pValidDC->Ellipse(m_nDeflate+Round(m_pDpa4points->X(0, 0)*m_xScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->Y(0, 0)*m_yScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->X(0, 0)*m_xScale)+nRadius,
		m_nDeflate+Round(m_pDpa4points->Y(0, 0)*m_yScale)+nRadius);
	//���Ͻ�
	pValidDC->SelectObject((m_ptCursor4point == CPoint(cx-1, 0)) ? &brCtrl : &br);
	pValidDC->Ellipse(m_nDeflate+Round(m_pDpa4points->X(cx-1, 0)*m_xScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->Y(cx-1, 0)*m_yScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->X(cx-1, 0)*m_xScale)+nRadius,
		m_nDeflate+Round(m_pDpa4points->Y(cx-1, 0)*m_yScale)+nRadius);
	//���½�
	pValidDC->SelectObject((m_ptCursor4point == CPoint(0, cy-1)) ? &brCtrl : &br);
	pValidDC->Ellipse(m_nDeflate+Round(m_pDpa4points->X(0, cy-1)*m_xScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->Y(0, cy-1)*m_yScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->X(0, cy-1)*m_xScale)+nRadius,
		m_nDeflate+Round(m_pDpa4points->Y(0, cy-1)*m_yScale)+nRadius);
	//���½�
	pValidDC->SelectObject((m_ptCursor4point == CPoint(cx-1, cy-1)) ? &brCtrl : &br);
	pValidDC->Ellipse(m_nDeflate+Round(m_pDpa4points->X(cx-1, cy-1)*m_xScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->Y(cx-1, cy-1)*m_yScale)-nRadius, 
		m_nDeflate+Round(m_pDpa4points->X(cx-1, cy-1)*m_xScale)+nRadius,
		m_nDeflate+Round(m_pDpa4points->Y(cx-1, cy-1)*m_yScale)+nRadius);

	pValidDC->SelectObject(pOldpen);
}

void CMGeometricAjustDlg::DrawCurveData(CDC *pValidDC)
{
	CPen Redpen, Greenpen, Graypen, *pOldpen;
	Redpen.CreatePen(PS_SOLID, 2, RGB(255,50,30));
	Greenpen.CreatePen(PS_SOLID, 2, RGB(50,255,30));
	Graypen.CreatePen(PS_SOLID, 1, RGB(147,186,138));

	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//�м������� �Ұ�
	pOldpen = pValidDC->SelectObject(&Graypen);
	//��������
	for (int x=1; x<cx; x++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpaCurvepoints->X(x, 0)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(x, 0)*m_yScale));
		for (int y=1; y<cy; y++)
			pValidDC->LineTo(m_nDeflate+Round(m_pDpaCurvepoints->X(x, y)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(x, y)*m_yScale));
	}
	//��������
	for (int y=1; y<cy; y++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpaCurvepoints->X(0, y)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(0, y)*m_yScale));
		for (int x=1; x<cx; x++)
			pValidDC->LineTo(m_nDeflate+Round(m_pDpaCurvepoints->X(x, y)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(x, y)*m_yScale));
	}

	//��������
	pValidDC->SelectObject(&Redpen);
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaCurvepoints->X(0,0)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(0,0)*m_yScale));
	for (int x=1; x<cx; x++)
		pValidDC->LineTo(m_nDeflate+Round(m_pDpaCurvepoints->X(x,0)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(x,0)*m_yScale));
	//��������
	pValidDC->SelectObject(&Greenpen);
	for (int y=1; y<cy; y++)
		pValidDC->LineTo(m_nDeflate+Round(m_pDpaCurvepoints->X(cx-1, y)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(cx-1, y)*m_yScale));
	//��������
	pValidDC->SelectObject(&Redpen);
	for (int x=cx-2; x>=0; x--)
		pValidDC->LineTo(m_nDeflate+Round(m_pDpaCurvepoints->X(x, cy-1)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(x, cy-1)*m_yScale));
	//��������
	pValidDC->SelectObject(&Greenpen);
	for (int y=cy-2; y>=0; y--)
		pValidDC->LineTo(m_nDeflate+Round(m_pDpaCurvepoints->X(0, y)*m_xScale), m_nDeflate+Round(m_pDpaCurvepoints->Y(0, y)*m_yScale));

	//�Ÿ����Ƶ�
	CBrush br, brCtrl, *pOldBr;
	br.CreateSolidBrush(RGB(255,255,100));
	brCtrl.CreateSolidBrush(RGB(100,100,255));
	int nRadius = 7; //����Բ��뾶��С
	pOldBr = pValidDC->SelectObject(&br);
	for (int y=0; y<3; y++) {
		for (int x=0; x<3; x++)
		{
			//��ǰ���Ƶ�
			if ((m_ptCursorCurve.x == x*(cx/2)) && (m_ptCursorCurve.y == y*(cy/2)))	{
				pValidDC->SelectObject(&brCtrl); //���⻭ˢ
				pValidDC->Ellipse(m_nDeflate+Round(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*m_xScale)-nRadius, 
					m_nDeflate+Round(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*m_yScale)-nRadius, 
					m_nDeflate+Round(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*m_xScale)+nRadius,
					m_nDeflate+Round(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*m_yScale)+nRadius);
				pValidDC->SelectObject(&br);     //��ԭ��������ˢ
			}
			else
				pValidDC->Ellipse(m_nDeflate+Round(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*m_xScale)-nRadius, 
				m_nDeflate+Round(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*m_yScale)-nRadius, 
				m_nDeflate+Round(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*m_xScale)+nRadius,
				m_nDeflate+Round(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*m_yScale)+nRadius);
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

void CMGeometricAjustDlg::DrawDensityData(CDC *pValidDC)
{
	CPen Redpen, Greenpen, Graypen, *pOldpen;
	Redpen.CreatePen(PS_SOLID, 2, RGB(255,50,30));
	Greenpen.CreatePen(PS_SOLID, 2, RGB(50,255,30));
	Graypen.CreatePen(PS_SOLID, 1, RGB(147,186,138));

	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//�м������� �Ұ�
	pOldpen = pValidDC->SelectObject(&Graypen);
	//��������
	for (int x=1; x<cx-1; x++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpaDensitypoints->X(x, 0)*m_xScale),    m_nDeflate+Round(m_pDpaDensitypoints->Y(x, 0)*m_yScale));
		pValidDC->LineTo(m_nDeflate+Round(m_pDpaDensitypoints->X(x, cy-1)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(x, cy-1)*m_yScale));
	}
	//��������
	for (int y=1; y<cy-1; y++)  {
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpaDensitypoints->X(0, y)*m_xScale),    m_nDeflate+Round(m_pDpaDensitypoints->Y(0, y)*m_yScale));
		pValidDC->LineTo(m_nDeflate+Round(m_pDpaDensitypoints->X(cx-1, y)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(cx-1, y)*m_yScale));
	}

	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaDensitypoints->X(0, 0)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(0, 0)*m_yScale));
	//�������� ��ɫ
	pOldpen = pValidDC->SelectObject(&Redpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpaDensitypoints->X(cx-1, 0)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(cx-1, 0)*m_yScale));
	//�������� ��ɫ
	pValidDC->SelectObject(&Greenpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpaDensitypoints->X(cx-1, cy-1)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(cx-1, cy-1)*m_yScale));
	//�������� ��ɫ
	pValidDC->SelectObject(&Redpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpaDensitypoints->X(0, cy-1)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(0, cy-1)*m_yScale));
	//�������� ��ɫ
	pValidDC->SelectObject(&Greenpen);
	pValidDC->LineTo(m_nDeflate+Round(m_pDpaDensitypoints->X(0, 0)*m_xScale), m_nDeflate+Round(m_pDpaDensitypoints->Y(0, 0)*m_yScale));

	//���Ƶ�
	CBrush brGreen, brRed, brCtrl, *pOldBr;
	brGreen.CreateSolidBrush(RGB(100,255,100));
	brRed.CreateSolidBrush(RGB(255,100,100));
	brCtrl.CreateSolidBrush(RGB(100,100,255));
	int nRadius = 7; //����Բ��뾶��С

	pOldBr = pValidDC->SelectObject(&brRed);
	for (int x=0; x<cx; x++)  {
		for (int y=0; y<cy; y++)
		{
			if (m_ptCursorDensity == CPoint(x, y))
			{
				//��ǰ���Ƶ�
				pValidDC->SelectObject(&brCtrl);
				pValidDC->Ellipse(m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)-nRadius, 
					m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)-nRadius, 
					m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)+nRadius,
					m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)+nRadius);
				pValidDC->SelectObject(&brRed);
			}
			else
			{
				if ((x%(cx/4) == 0) && y == (cy/2))      {// ˮƽ���ܿ��Ƶ�	
					pValidDC->SelectObject(&brGreen);
					pValidDC->Ellipse(m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)-nRadius, 
						m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)-nRadius, 
						m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)+nRadius,
						m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)+nRadius);
				}
				else if (x == (cx/2) && (y%(cy/4) == 0)) {// ��ֱ���ܿ��Ƶ�
					pValidDC->SelectObject(&brRed);
					pValidDC->Ellipse(m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)-nRadius, 
						m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)-nRadius, 
						m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)+nRadius,
						m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)+nRadius);
				}
			}

		}
	}

	pValidDC->SelectObject(pOldBr);
	pValidDC->SelectObject(pOldpen);
}

void CMGeometricAjustDlg::DrawResolution(CDC *pValidDC)
{
	CFont markFont, *pOldFont;
	VERIFY(markFont.CreateFont(
		40,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		TRUE,                      // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	pOldFont = pValidDC->SelectObject(&markFont);

	CString strRes;
	strRes.Format(_T("%dx%d @ %dHz"), m_gridsetting.cxResIn, m_gridsetting.cyResIn, m_gridsetting.nHz);

	CSize txtExt;
	txtExt = pValidDC->GetTextExtent(strRes);

	CPoint ptMark;
	ptMark.x = m_rcPaint.CenterPoint().x - int(txtExt.cx*0.5);
	ptMark.y = m_rcPaint.CenterPoint().y - int(txtExt.cy*0.5);
	ptMark.Offset(-m_rcPaint.left, -m_rcPaint.top);

	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(255,255,200));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	ptMark.Offset(1,1);
	pValidDC->SetTextColor(RGB(178,145,240));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	pValidDC->SelectObject(pOldFont);
}

void CMGeometricAjustDlg::OnBnClickedAjustStyle(UINT nID)
{
	int nStyle = nID-IDC_BTN_KEYSTONE;
	if (nStyle == m_eAjustStyle) return; //У����ʽû��

	// ȡ���ϴ�ѡ��İ�ť�ı��
	CXTPButton *pBtn = (CXTPButton*)GetDlgItem(IDC_BTN_KEYSTONE+m_eAjustStyle);
	pBtn->SetChecked(FALSE);

	// ����ǰѡ��İ�ť�����
	m_eAjustStyle = (emAjustStyle)nStyle;
	pBtn = (CXTPButton *)GetDlgItem(nID);
	pBtn->SetChecked( TRUE);

	// ���´���
	if (m_eAjustStyle == eKeystone) { ShowKeystoneCtrls(); Show4pointCoords(); }
	else if (m_eAjustStyle == eCurve) { ShowCurveCtrls(); ShowCurveCoords(); }
	else if (m_eAjustStyle == eDensity) { ShowDensityCtrls(); ShowDensityCoords();}
}


BOOL CMGeometricAjustDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE
			|| pMsg->wParam == VK_RETURN)
		{
			//��ENTER����������
			if (VKEnterInEditCtrls(pMsg->hwnd, 0.0)) {
				if (m_eAjustStyle == eKeystone) On4pointAjust(pMsg->pt, FALSE);
				else if (m_eAjustStyle == eCurve) OnCurveAjust(pMsg->pt, FALSE);
				else if (m_eAjustStyle == eDensity) OnDensityAjust(pMsg->pt, FALSE);
			}

			return TRUE;
		}
		else if (pMsg->wParam == VK_LEFT
			|| pMsg->wParam == VK_UP
			|| pMsg->wParam == VK_DOWN
			|| pMsg->wParam == VK_RIGHT)
		{

			//����������ÿ��Ƶ�����
			OnVKArrowAjust(pMsg->wParam);

			return TRUE;
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

BOOL CMGeometricAjustDlg::VKEnterInEditCtrls(HWND hWnd, double dOffset, BOOL bEnterActive)
{
	double dNewCoords = .0;

	if (bEnterActive)
	{
		//��ȡ�༭���е�ֵ
		CWnd *pWnd = FromHandle(hWnd);
		CString strCoords;
		if (pWnd->IsKindOf(RUNTIME_CLASS(CXTPEdit)))
		{
			((CXTPEdit*)pWnd)->GetWindowText(strCoords);
			dNewCoords = atof(strCoords);
		}
		else return FALSE;
	}
	else
		dNewCoords = dOffset;

	//��ȡ����������
	CDoublePointArray *pPoints = NULL;
	CPoint *pFocusPt = NULL;        //��ȡ���Ƶ�����
	CDoublePoint *pFocusPos = NULL; //��ȡ���Ƶ�λ��
	if (m_eAjustStyle == eKeystone) 
	{
		pPoints   = m_pDpa4points;
		pFocusPt  = &m_ptCursor4point;
		pFocusPos = &m_dpaCursorPos4point;
	}
	else if (m_eAjustStyle == eCurve)
	{
		pPoints   = m_pDpaCurvepoints;
		pFocusPt  = &m_ptCursorCurve;
		pFocusPos = &m_dpaCursorPosCurve;
	}
	else if (m_eAjustStyle == eDensity)
	{
		pPoints   = m_pDpaDensitypoints;
		pFocusPt  = &m_ptCursorDensity;
		pFocusPos = &m_dpaCursorPosDensity;
	}

	//��Ч���ݵ�ַ
	if (pPoints == NULL) return FALSE;

	//��ǰ����
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//������or�����ꣿ
	BOOL isXCoords = FALSE;	

	//ENTER�����ĸ��༭���а���
	if (hWnd == m_editTL[0].m_hWnd)
	{ //���Ͻ�X
		isXCoords = TRUE;
		pFocusPt->x = 0;
		pFocusPt->y = 0;
	}
	else if (hWnd == m_editTL[1].m_hWnd)
	{ //���Ͻ�Y
		pFocusPt->x = 0; 
		pFocusPt->y = 0;
	}
	else if (hWnd == m_editTC[0].m_hWnd)
	{ //���в�X
		isXCoords = TRUE;
		pFocusPt->x = cx/2;
		pFocusPt->y = 0;
	}
	else if (hWnd == m_editTC[1].m_hWnd)
	{ //���в�Y
		pFocusPt->x = cx/2;
		pFocusPt->y = 0;
	}
	else if (hWnd == m_editTR[0].m_hWnd)
	{ //���Ͻ�X
		isXCoords = TRUE;
		pFocusPt->x = cx-1;
		pFocusPt->y = 0;
	}
	else if (hWnd == m_editTR[1].m_hWnd)
	{ //���Ͻ�Y
		if (m_eAjustStyle == eDensity) {pFocusPt->x = cx/2; pFocusPt->y =0;}
		else {pFocusPt->x = cx-1; pFocusPt->y =0;}
	}
	else if (hWnd == m_editLC[0].m_hWnd)
	{ //���в�X
		isXCoords = TRUE;
		pFocusPt->x = 0;
		pFocusPt->y = cy/2;
		if (m_eAjustStyle == eCurve) m_pDpaCurvepoints->X(0, cy/2) = dNewCoords;
	}
	else if (hWnd == m_editLC[1].m_hWnd)
	{ //���в�Y
		pFocusPt->x = 0;
		pFocusPt->y = cy/2;
	}
	else if (hWnd == m_editRC[0].m_hWnd)
	{ //���в�X
		isXCoords = TRUE;
		pFocusPt->x = cx-1;
		pFocusPt->y = cy/2;
	}
	else if (hWnd == m_editRC[1].m_hWnd)
	{ //���в�Y
		if (m_eAjustStyle == eDensity) {pFocusPt->x = cx/2; pFocusPt->y = cy/2;}
		else {pFocusPt->x = cx-1; pFocusPt->y = cy/2;}
	}
	else if (hWnd == m_editBL[0].m_hWnd)
	{ //���½�X
		isXCoords = TRUE;
		if (m_eAjustStyle == eDensity) {pFocusPt->x = 0; pFocusPt->y = cy/2;}
		else {pFocusPt->x = 0; pFocusPt->y = cy-1;}
	}
	else if (hWnd == m_editBL[1].m_hWnd)
	{ //���½�Y
		pFocusPt->x = 0;
		pFocusPt->y = cy-1;
	}
	else if (hWnd == m_editBC[0].m_hWnd)
	{ 
		isXCoords = TRUE;
		if (m_eAjustStyle == eDensity) {pFocusPt->x = cx/2; pFocusPt->y = cy/2;}
		else {pFocusPt->x = cx/2; pFocusPt->y = cy-1;} 
	}
	else if (hWnd == m_editBC[1].m_hWnd)
	{ //���в�Y
		pFocusPt->x = cx/2;
		pFocusPt->y = cy-1;
	}
	else if (hWnd == m_editBR[0].m_hWnd) 
	{ 
		isXCoords = TRUE;
		if (m_eAjustStyle == eDensity) {pFocusPt->x = cx-1; pFocusPt->y = cy/2;}
		else {pFocusPt->x = cx-1; pFocusPt->y = cy-1;}
	}
	else if (hWnd == m_editBR[1].m_hWnd)
	{ 
		if (m_eAjustStyle == eDensity) {pFocusPt->x = cx/2, pFocusPt->y = cy-1;}
		else {pFocusPt->x = cx-1; pFocusPt->y = cy-1;}
	}
	else if (hWnd == m_editMC[0].m_hWnd)
	{ //����X //��4
		isXCoords = TRUE;
		if (m_eAjustStyle == eDensity) {pFocusPt->x = cx/4*3; pFocusPt->y = cy/2;}
		else {pFocusPt->x = cx/2; pFocusPt->y = cy/2;} 
	}
	else if (hWnd == m_editMC[1].m_hWnd)
	{ //����Y
		pFocusPt->x = cx/2;
		pFocusPt->y = cy/2;
	}
	else if (hWnd == m_editExX.m_hWnd)
	{ //��2
		isXCoords = TRUE;
		pFocusPt->x = cx/4;
		pFocusPt->y = cy/2;
	}
	else if (hWnd == m_editExY[0].m_hWnd)
	{ //��2
		pFocusPt->x = cx/2;
		pFocusPt->y = cy/4;
	}
	else if (hWnd == m_editExY[1].m_hWnd)
	{ //��4
		pFocusPt->x = cx/2;
		pFocusPt->y = cy/4*3;
	}
	else return FALSE;

	//��ֵ
	if (bEnterActive)
	{
		if (isXCoords) pPoints->X(pFocusPt->x, pFocusPt->y) = dNewCoords;
		else pPoints->Y(pFocusPt->x, pFocusPt->y) = dNewCoords;
	}
	else
	{
		if (isXCoords) pPoints->X(pFocusPt->x, pFocusPt->y) -= dNewCoords;
		else pPoints->Y(pFocusPt->x, pFocusPt->y) += dNewCoords;
	}

	pFocusPos->x = pPoints->X(pFocusPt->x, pFocusPt->y);
	pFocusPos->y = pPoints->Y(pFocusPt->x, pFocusPt->y);

	return TRUE;
}


void CMGeometricAjustDlg::OnVKArrowAjust(UINT nVKcode)
{
	//��������ƫ����
	double xOffset = .0;
	double yOffset = .0;

	xOffset = (nVKcode == VK_LEFT ? -0.5 : (nVKcode== VK_RIGHT ? 0.5 : 0.0));
	yOffset = (nVKcode == VK_UP ? -0.5 : (nVKcode== VK_DOWN ? 0.5 : 0.0));

	//��ȡ��ǰ���Ʊ�����ַ
	CDoublePoint *pFocusPos = NULL;
	CDoublePointArray *pPoints = NULL;
	CPoint *pFocusPt = NULL;
	if (m_eAjustStyle == eKeystone)
	{
		pFocusPos = &m_dpaCursorPos4point;
		pPoints   = m_pDpa4points;
		pFocusPt  = &m_ptCursor4point;
	}
	else if (m_eAjustStyle == eCurve)
	{
		pFocusPos = &m_dpaCursorPosCurve;
		pPoints   = m_pDpaCurvepoints;
		pFocusPt  = &m_ptCursorCurve;
	}
	else if (m_eAjustStyle == eDensity)
	{
		pFocusPos = &m_dpaCursorPosDensity;
		pPoints   = m_pDpaDensitypoints;
		pFocusPt  = &m_ptCursorDensity;

		//����У��ֻ���м�Ŀ��Ƶ�������������ƶ�������ֻ����һ�����귽�����ƶ�
		int cx = m_gridsetting.cx;
		int cy = m_gridsetting.cy;
		if (nVKcode == VK_UP || nVKcode == VK_DOWN)
		{
			if (((pFocusPt->x % 4) == 0) && (pFocusPt->x != cx/2))
			{
				//ˮƽ���ܶȵ��ڵ�ֻ�������ƶ�
				return;
			}
		}
		else if (nVKcode == VK_LEFT || nVKcode == VK_RIGHT)
		{
			if (((pFocusPt->y % 4) == 0) && (pFocusPt->y != cy/2))
			{
				//��ֱ���ܶȵ��ڵ�ֻ�������ƶ�
				return;
			}
		}
	}

	//��ֵ
	pFocusPos->x += xOffset;
	pFocusPos->y += yOffset;

	theApp.GlobleLock();   //��Դ��

	pPoints->X(pFocusPt->x, pFocusPt->y) = pFocusPos->x;
	pPoints->Y(pFocusPt->x, pFocusPt->y) = pFocusPos->y;

	//У������
	if (m_eAjustStyle == eKeystone)
	{
		Warp_Auto_Kestone();
	}
	else if (m_eAjustStyle == eCurve)
	{
		Fitting();
	}
	else if (m_eAjustStyle == eDensity)
	{
		Warp_Auto_Ranks_Translation();
	}

	//��װ�����
	TaskPackage pack;
	Packaging(pack, (int)m_eAjustStyle);
	theApp.PushPackage(pack);

	//�߳��е�����ģʽ
	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock(); //����

	//���±༭������
	if (m_eAjustStyle == eKeystone) Show4pointCoords();
	else if (m_eAjustStyle == eCurve) ShowCurveCoords();
	else if (m_eAjustStyle == eDensity) ShowDensityCoords();

	//�ػ�
	InvalidateRect(m_rcPaint);
}

void CMGeometricAjustDlg::InitDpaPoints()
{
	// �����ڴ�
	if (m_pDpaOrigpoints == NULL)
		m_pDpaOrigpoints = new CDoublePointArray(m_gridsetting.cx, m_gridsetting.cy);

	if (m_pDpa4points == NULL)
		m_pDpa4points = new CDoublePointArray(m_gridsetting.cx, m_gridsetting.cy);

	if (m_pDpaCurvepoints == NULL)
		m_pDpaCurvepoints = new CDoublePointArray(m_gridsetting.cx, m_gridsetting.cy);

	if (m_pDpaDensitypoints == NULL)
		m_pDpaDensitypoints = new CDoublePointArray(m_gridsetting.cx, m_gridsetting.cy);

	// ��ʼ��������
	for (int y=0; y<m_gridsetting.cy; y++)
		for (int x=0; x<m_gridsetting.cx; x++)
		{
			m_pDpaOrigpoints->X(x,y) = OrignalPos(x, y).x;
			m_pDpaOrigpoints->Y(x,y) = OrignalPos(x, y).y;

			m_pDpa4points->X(x,y) = OrignalPos(x, y).x;
			m_pDpa4points->Y(x,y) = OrignalPos(x, y).y;

			m_pDpaCurvepoints->X(x,y) = OrignalPos(x, y).x;
			m_pDpaCurvepoints->Y(x,y) = OrignalPos(x, y).y;

			m_pDpaDensitypoints->X(x,y) = OrignalPos(x, y).x;
			m_pDpaDensitypoints->Y(x,y) = OrignalPos(x, y).y;
		}

		//��ʼ�����Ƶ������
		m_dpaCursorPos4point.x = m_pDpa4points->X(m_ptCursor4point.x, m_ptCursor4point.y);
		m_dpaCursorPos4point.y = m_pDpa4points->Y(m_ptCursor4point.x, m_ptCursor4point.y);

		m_dpaCursorPosCurve.x  = m_pDpaCurvepoints->X(m_ptCursorCurve.x, m_ptCursorCurve.y);
		m_dpaCursorPosCurve.y  = m_pDpaCurvepoints->Y(m_ptCursorCurve.x, m_ptCursorCurve.y);

		m_dpaCursorPosDensity.x= m_pDpaDensitypoints->X(m_ptCursorDensity.x, m_ptCursorDensity.y);
		m_dpaCursorPosDensity.y= m_pDpaDensitypoints->Y(m_ptCursorDensity.x, m_ptCursorDensity.y);

}

CDoublePoint CMGeometricAjustDlg::OrignalPos(int x, int y)
{
	ASSERT( x < m_gridsetting.cx && y < m_gridsetting.cy );
	double xVal = double(x * m_gridsetting.cxResIn) / double(m_gridsetting.cx-1);
	double yVal = double(y * m_gridsetting.cyResIn) / double(m_gridsetting.cy-1);
	return CDoublePoint(xVal,yVal);
}

void CMGeometricAjustDlg::DeleteDpaData()
{
	if (m_pDpa4points != NULL)
	{
		delete m_pDpa4points;
		m_pDpa4points = NULL;
	}
	if (m_pDpaCurvepoints != NULL)
	{
		delete m_pDpaCurvepoints;
		m_pDpaCurvepoints = NULL;
	}
	if (m_pDpaDensitypoints != NULL)
	{
		delete m_pDpaDensitypoints;
		m_pDpaDensitypoints = NULL;
	}
	if (m_pDpaOrigpoints != NULL)
	{
		delete m_pDpaOrigpoints;
		m_pDpaOrigpoints = NULL;
	}
}

void CMGeometricAjustDlg::SetGraphicSize()
{
	// ����У����ص�������Ϣ
	m_gridsetting.cx4PointGraphSize = m_rcChart.Width();
	m_gridsetting.cy4PointGraphSize = m_rcChart.Height();

	m_gridsetting.cxCurveGraphSize = m_rcChart.Width();
	m_gridsetting.cyCurveGraphSize = m_rcChart.Height();

	m_gridsetting.cxDensityGraphSize = m_rcChart.Width();
	m_gridsetting.cyDensityGraphSize = m_rcChart.Height();

	m_xScale = double(m_rcChart.Width()) / double(m_gridsetting.cxResIn);
	m_yScale = double(m_rcChart.Height()) / double(m_gridsetting.cyResIn);
}

void CMGeometricAjustDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture(); //ֻ����ǰ���ڽ��������Ϣ

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	// ���������ڿ��Ƶ��ڣ�
	if (m_eAjustStyle == eKeystone)
	{
		m_bLBtnDownAt4Point = IsLBtnDownAt4Point(point);
	}
	else if (m_eAjustStyle == eCurve)
	{
		m_bLBtnDownAtCurve = IsLBtnDownAtCurve(point);
	}
	else if (m_eAjustStyle == eDensity)
	{
		m_bLBtnDownAtDensity = IsLBtnDownAtDensity(point);
	}

	// ֪ͨ�����ػ�
	if (m_bLBtnDownAt4Point || m_bLBtnDownAtCurve || m_bLBtnDownAtDensity) InvalidateRect(m_rcPaint);

	CXTPResizeDialog::OnLButtonDown(nFlags, point);
}

void CMGeometricAjustDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	point.Offset(-(m_rcPaint.left), -(m_rcPaint.top));

	// �����߼����Ʊ���
	if (m_eAjustStyle == eKeystone)
	{
		m_bLBtnDownAt4Point = FALSE;
	}
	else if (m_eAjustStyle == eCurve)
	{
		m_bLBtnDownAtCurve = FALSE;
	}
	else if (m_eAjustStyle == eDensity)
	{
		m_bLBtnDownAtDensity = FALSE;
	}

	theApp.GlobleLock();
	// ���
	TaskPackage pack;
	Packaging(pack, int(m_eAjustStyle));
	theApp.PushPackage(pack);

	//���߳��е�����ģʽ
	g_AjustThreadWorkMode = WORK_MODE_AJUST;
	theApp.GlobleUnlock();

	CXTPResizeDialog::OnLButtonUp(nFlags, point);
}

void CMGeometricAjustDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	point.Offset(-(m_rcPaint.left), -(m_rcPaint.top));

	if (m_eAjustStyle == eKeystone)
	{
		//�����ĵ�У��
		if (m_bLBtnDownAt4Point)
		{
			On4pointAjust(point);
		}
	}
	else if (m_eAjustStyle == eCurve)
	{
		//��������У��
		if (m_bLBtnDownAtCurve)
		{
			OnCurveAjust(point);
		}
	}
	else if (m_eAjustStyle == eDensity)
	{
		//��������У��
		if (m_bLBtnDownAtDensity)
		{
			OnDensityAjust(point);
		}
	}

	CXTPResizeDialog::OnMouseMove(nFlags, point);
}

BOOL CMGeometricAjustDlg::IsLBtnDownAt4Point(CPoint point)
{
	CRect rcCtrlPoint;
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	int nRadius = 7;

	for (int x=0; x<2; x++)	{
		for (int y=0; y<2; y++)	{
			rcCtrlPoint = CRect(m_nDeflate+Round(m_pDpa4points->X(x*(cx-1), y*(cy-1))*m_xScale),
				m_nDeflate+Round(m_pDpa4points->Y(x*(cx-1), y*(cy-1))*m_yScale),
				m_nDeflate+Round(m_pDpa4points->X(x*(cx-1), y*(cy-1))*m_xScale),
				m_nDeflate+Round(m_pDpa4points->Y(x*(cx-1), y*(cy-1))*m_yScale));
			rcCtrlPoint.InflateRect(nRadius,nRadius);

			if (PtInRect(rcCtrlPoint, point))
			{
				m_ptCursor4point = CPoint(x*(cx-1), y*(cy-1));
				m_dpaCursorPos4point = CDoublePoint(m_pDpa4points->X(x*(cx-1), y*(cy-1)), m_pDpa4points->Y(x*(cx-1), y*(cy-1)));

				return TRUE;
			}

		}
	}

	return FALSE;
}

BOOL CMGeometricAjustDlg::IsLBtnDownAtCurve(CPoint point)
{
	CRect rcCtrlPoint;
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	int nRadius = 7;

	for (int x=0; x<3; x++) {
		for (int y=0; y<3; y++)
		{
			rcCtrlPoint = CRect(m_nDeflate+Round(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*m_xScale)-nRadius, 
				m_nDeflate+Round(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*m_yScale)-nRadius, 
				m_nDeflate+Round(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*m_xScale)+nRadius,
				m_nDeflate+Round(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*m_yScale)+nRadius);
			if (PtInRect(rcCtrlPoint, point))
			{
				m_ptCursorCurve     = CPoint(x*(cx/2), y*(cy/2));
				m_dpaCursorPosCurve = CDoublePoint(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2)), m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2)));
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CMGeometricAjustDlg::IsLBtnDownAtDensity(CPoint point)
{
	CRect rcCtrlPoint;
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	int nRadius = 7;

	for (int x=0; x<cx; x++) {
		for (int y=0; y<cy; y++)
		{
			if (((x%(cx/4) == 0) && (y == (cy/2)))    // ˮƽ���ܵ�����
				||  ((x == (cx/2)) && ((y%(cy/4) == 0))) )// ��ֱ���ܵ�����
			{					
				rcCtrlPoint = CRect(m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)-nRadius, 
					m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)-nRadius, 
					m_nDeflate+Round(m_pDpaDensitypoints->X(x, y)*m_xScale)+nRadius,
					m_nDeflate+Round(m_pDpaDensitypoints->Y(x, y)*m_yScale)+nRadius);

				if (PtInRect(rcCtrlPoint, point))
				{
					m_ptCursorDensity     = CPoint(x, y);
					m_dpaCursorPosDensity = CDoublePoint(m_pDpaDensitypoints->X(x,y), m_pDpaDensitypoints->Y(x, y));
					return TRUE;
				}				
			}
		}
	}


	return FALSE;
}

void CMGeometricAjustDlg::On4pointAjust(CPoint point, BOOL bMouseActive)
{
	if (bMouseActive)
	{
		point.Offset(-m_nDeflate,-m_nDeflate);

		double Xoffset = min(m_gridsetting.cxResIn, point.x/m_xScale-m_dpaCursorPos4point.x);
		double Yoffset = min(m_gridsetting.cyResIn, point.y/m_yScale-m_dpaCursorPos4point.y);

		m_dpaCursorPos4point.x += Xoffset;
		m_dpaCursorPos4point.y += Yoffset;

		if (m_dpaCursorPos4point.x < 0) m_dpaCursorPos4point.x = 0;
		if (m_dpaCursorPos4point.x > m_gridsetting.cxResIn) m_dpaCursorPos4point.x = m_gridsetting.cxResIn;
		if (m_dpaCursorPos4point.y < 0) m_dpaCursorPos4point.y = 0;
		if (m_dpaCursorPos4point.y > m_gridsetting.cyResIn) m_dpaCursorPos4point.y = m_gridsetting.cyResIn;
	}

	theApp.GlobleLock();   //��Դ��

	if (bMouseActive)
	{
		m_pDpa4points->X(m_ptCursor4point.x, m_ptCursor4point.y) = m_dpaCursorPos4point.x;
		m_pDpa4points->Y(m_ptCursor4point.x, m_ptCursor4point.y) = m_dpaCursorPos4point.y;
	}

	//���������
	Warp_Auto_Kestone();

	if (!bMouseActive)
	{
		//���
		TaskPackage pack;
		Packaging(pack, KEYSTONE_AJUST);
		theApp.PushPackage(pack);

		//���߳��е�����ģʽ
		g_AjustThreadWorkMode = WORK_MODE_AJUST;
	}

	theApp.GlobleUnlock(); //�����Դ��			

	//���½���
	Show4pointCoords();
	InvalidateRect(m_rcPaint);
}

void CMGeometricAjustDlg::Show4pointCoords()
{
	if (m_pDpa4points == NULL) return;

	CString strXCoords, strYCoords;
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//����
	strXCoords.Format(_T("%.1f"), m_pDpa4points->X(0,0));       m_editTL[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpa4points->Y(0,0));       m_editTL[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpa4points->X(cx-1,0));    m_editTR[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpa4points->Y(cx-1,0));    m_editTR[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpa4points->X(0,cy-1));    m_editBL[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpa4points->Y(0,cy-1));    m_editBL[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpa4points->X(cx-1,cy-1)); m_editBR[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpa4points->Y(cx-1,cy-1)); m_editBR[1].SetWindowText(strYCoords);
}

void CMGeometricAjustDlg::OnCurveAjust(CPoint point, BOOL bMouseActive)
{
	if (bMouseActive)
	{
		point.Offset(-m_nDeflate,-m_nDeflate);

		double Xoffset = min(m_gridsetting.cxResIn, point.x/m_xScale-m_dpaCursorPosCurve.x);
		double Yoffset = min(m_gridsetting.cyResIn, point.y/m_yScale-m_dpaCursorPosCurve.y);

		m_dpaCursorPosCurve.x += Xoffset;
		m_dpaCursorPosCurve.y += Yoffset;

		if (m_dpaCursorPosCurve.x < 0) m_dpaCursorPosCurve.x = 0;
		if (m_dpaCursorPosCurve.x > m_gridsetting.cxResIn) m_dpaCursorPosCurve.x = m_gridsetting.cxResIn;
		if (m_dpaCursorPosCurve.y < 0) m_dpaCursorPosCurve.y = 0;
		if (m_dpaCursorPosCurve.y > m_gridsetting.cyResIn) m_dpaCursorPosCurve.y = m_gridsetting.cyResIn;
	}

	theApp.GlobleLock();    //��Դ��

	if (bMouseActive)
	{
		m_pDpaCurvepoints->X(m_ptCursorCurve.x, m_ptCursorCurve.y) = m_dpaCursorPosCurve.x;
		m_pDpaCurvepoints->Y(m_ptCursorCurve.x, m_ptCursorCurve.y) = m_dpaCursorPosCurve.y;
	}

	// �������
	Fitting();

	if (!bMouseActive)
	{
		// ���
		TaskPackage pack;
		Packaging(pack, CURVE_AJUST);
		theApp.PushPackage(pack);

		//���߳��е�����ģʽ
		g_AjustThreadWorkMode = WORK_MODE_AJUST;
	}

	theApp.GlobleUnlock();  //������Դ��

	ShowCurveCoords();
	InvalidateRect(m_rcPaint);
}

void CMGeometricAjustDlg::ShowCurveCoords()
{
	if (m_pDpaCurvepoints == NULL) return;

	CString strXCoords, strYCoords;
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(0,0));       m_editTL[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(0,0));       m_editTL[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(cx/2,0));    m_editTC[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(cx/2,0));    m_editTC[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(cx-1,0));    m_editTR[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(cx-1,0));    m_editTR[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(0,cy/2));    m_editLC[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(0,cy/2));    m_editLC[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(cx/2,cy/2)); m_editMC[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(cx/2,cy/2)); m_editMC[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(cx-1,cy/2)); m_editRC[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(cx-1,cy/2)); m_editRC[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(0,cy-1));    m_editBL[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(0,cy-1));    m_editBL[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(cx/2,cy-1)); m_editBC[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(cx/2,cy-1)); m_editBC[1].SetWindowText(strYCoords);
	//����
	strXCoords.Format(_T("%.1f"), m_pDpaCurvepoints->X(cx-1,cy-1)); m_editBR[0].SetWindowText(strXCoords);
	strYCoords.Format(_T("%.1f"), m_pDpaCurvepoints->Y(cx-1,cy-1)); m_editBR[1].SetWindowText(strYCoords);
}

void CMGeometricAjustDlg::OnDensityAjust(CPoint point, BOOL bMouseActive)
{
	if (bMouseActive)
	{
		point.Offset(-m_nDeflate,-m_nDeflate);

		double Xoffset = min(m_gridsetting.cxResIn, point.x/m_xScale-m_dpaCursorPosDensity.x);
		double Yoffset = min(m_gridsetting.cyResIn, point.y/m_yScale-m_dpaCursorPosDensity.y);

		if (m_ptCursorDensity.x == m_gridsetting.cx/2) m_dpaCursorPosDensity.y += Yoffset;
		if (m_ptCursorDensity.y == m_gridsetting.cy/2) m_dpaCursorPosDensity.x += Xoffset;

		if (m_dpaCursorPosDensity.x < 0) m_dpaCursorPosDensity.x = 0;
		if (m_dpaCursorPosDensity.x > m_gridsetting.cxResIn) m_dpaCursorPosDensity.x = m_gridsetting.cxResIn;
		if (m_dpaCursorPosDensity.y < 0) m_dpaCursorPosDensity.y = 0;
		if (m_dpaCursorPosDensity.y > m_gridsetting.cyResIn) m_dpaCursorPosDensity.y = m_gridsetting.cyResIn;
	}

	theApp.GlobleLock();   //��Դ��

	if (bMouseActive)
	{
		m_pDpaDensitypoints->X(m_ptCursorDensity.x, m_ptCursorDensity.y) = m_dpaCursorPosDensity.x;
		m_pDpaDensitypoints->Y(m_ptCursorDensity.x, m_ptCursorDensity.y) = m_dpaCursorPosDensity.y;
	}

	Warp_Auto_Ranks_Translation();

	if (!bMouseActive)
	{
		// ���
		TaskPackage pack;
		Packaging(pack, DENSITY_AJUST);
		theApp.PushPackage(pack);

		//���߳��е�����ģʽ
		g_AjustThreadWorkMode = WORK_MODE_AJUST;
	}

	theApp.GlobleUnlock(); //�����Դ��

	ShowDensityCoords();
	InvalidateRect(m_rcPaint);
}

void CMGeometricAjustDlg::ShowDensityCoords()
{
	if (m_pDpaDensitypoints == NULL) return;

	CString strCoords;
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//��1
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->X(0, cy/2));     m_editBL[0].SetWindowText(strCoords);
	//��2
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->X(cx/4, cy/2));  m_editExX.SetWindowText(strCoords);
	//��3
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->X(cx/2, cy/2));  m_editBC[0].SetWindowText(strCoords);
	//��4
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->X(cx/4*3, cy/2));m_editMC[0].SetWindowText(strCoords);
	//��5
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->X(cx-1, cy/2));  m_editBR[0].SetWindowText(strCoords);
	//��1
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->Y(cx/2, 0));     m_editTR[1].SetWindowText(strCoords);
	//��2
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->Y(cx/2, cy/4));  m_editExY[0].SetWindowText(strCoords);
	//��3
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->Y(cx/2, cy/2));  m_editRC[1].SetWindowText(strCoords);
	//��4
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->Y(cx/2, cy/4*3));m_editExY[1].SetWindowText(strCoords);
	//��5
	strCoords.Format(_T("%.1f"), m_pDpaDensitypoints->Y(cx/2, cy-1));  m_editBR[1].SetWindowText(strCoords);
}

void CMGeometricAjustDlg::OnBnClickedReset()
{
	if (m_eAjustStyle == eKeystone) {Reset4pointArray(); Show4pointCoords();}
	else if (m_eAjustStyle == eCurve) {ResetCurveArray(); ShowCurveCoords();}
	else if (m_eAjustStyle == eDensity) {ResetDensityArray(); ShowDensityCoords();}

	theApp.GlobleLock();    //��Դ��

	//���
	TaskPackage pack;
	Packaging(pack, (int)m_eAjustStyle);
	theApp.PushPackage(pack);

	//���߳��е�����ģʽ
	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock();  //�����Դ��

	InvalidateRect(m_rcPaint);
}

void CMGeometricAjustDlg::Reset4pointArray()
{
	theApp.GlobleLock();

	for (int y=0; y<m_gridsetting.cy; y++) {
		for (int x=0; x<m_gridsetting.cx; x++)
		{
			m_pDpa4points->X(x,y) = OrignalPos(x, y).x;
			m_pDpa4points->Y(x,y) = OrignalPos(x, y).y;
		}
	}

	theApp.GlobleUnlock();
}

void CMGeometricAjustDlg::ResetCurveArray()
{
	theApp.GlobleLock();

	for (int y=0; y<m_gridsetting.cy; y++) {
		for (int x=0; x<m_gridsetting.cx; x++)
		{
			m_pDpaCurvepoints->X(x,y) = OrignalPos(x, y).x;
			m_pDpaCurvepoints->Y(x,y) = OrignalPos(x, y).y;
		}
	}
	theApp.GlobleUnlock();
}

void CMGeometricAjustDlg::ResetDensityArray()
{
	theApp.GlobleLock();

	for (int y=0; y<m_gridsetting.cy; y++) {
		for (int x=0; x<m_gridsetting.cx; x++)
		{
			m_pDpaDensitypoints->X(x,y) = OrignalPos(x, y).x;
			m_pDpaDensitypoints->Y(x,y) = OrignalPos(x, y).y;
		}
	}

	theApp.GlobleUnlock();
}

void CMGeometricAjustDlg::Packaging(TaskPackage &pack, int nMode)
{
	pack.cx = m_gridsetting.cx;
	pack.cy = m_gridsetting.cy;
	pack.cxResIn = m_gridsetting.cxResIn;
	pack.cyResIn = m_gridsetting.cyResIn;
	pack.nAjustMode = nMode;
	pack.nBoardPort = GetBoardPort();
	pack.pDpa4pointData = m_pDpa4points;
	pack.pDpaCurveData  = m_pDpaCurvepoints;
	pack.pDpaDensityData= m_pDpaDensitypoints;
	pack.pDpaOrigData   = m_pDpaOrigpoints;
	pack.pPaintWnd = this;
}

//�ĵ�Keystone,�Զ�������Ե������,�ı���ֱ�ߡ�
void CMGeometricAjustDlg::Warp_Auto_Kestone()
{
	int i,j;
	double tmp_x,tmp_y;

	CDoublePointArray* pPoints = m_pDpa4points;
	int Warp_CY  = m_gridsetting.cy;
	int Warp_CX  = m_gridsetting.cx;

	//��0�� ����
	tmp_x = (pPoints->Val(Warp_CX-1,0).x - pPoints->Val(0,0).x)/(Warp_CX-1);
	tmp_y = (pPoints->Val(Warp_CX-1,0).y - pPoints->Val(0,0).y)/(Warp_CX-1);
	for(i =0;i<Warp_CX;i++) 
	{
		pPoints->X(i,0) = pPoints->Val(0,0).x + tmp_x*i;
		pPoints->Y(i,0) = pPoints->Val(0,0).y + tmp_y*i;
	}
	//��Warp_CY-1 �� ����
	tmp_x = (pPoints->Val(Warp_CX-1,Warp_CY-1).x - pPoints->Val(0,Warp_CY-1).x)/(Warp_CX-1);
	tmp_y = (pPoints->Val(Warp_CX-1,Warp_CY-1).y - pPoints->Val(0,Warp_CY-1).y)/(Warp_CX-1);
	for(i =0;i<Warp_CX;i++) 
	{
		pPoints->X(i,Warp_CY-1) = pPoints->Val(0,Warp_CY-1).x + tmp_x*i;
		pPoints->Y(i,Warp_CY-1) = pPoints->Val(0,Warp_CY-1).y + tmp_y*i;
	}

	//��0 �� ����
	tmp_x = (pPoints->Val(0,Warp_CY-1).x - pPoints->Val(0,0).x)/(Warp_CY-1);
	tmp_y = (pPoints->Val(0,Warp_CY-1).y - pPoints->Val(0,0).y)/(Warp_CY-1);
	for(i =0;i<Warp_CY;i++) 
	{
		pPoints->X(0,i) = pPoints->Val(0,0).x + tmp_x*i;
		pPoints->Y(0,i) = pPoints->Val(0,0).y + tmp_y*i;
	}

	//��Warp_CX-1 �� ����
	tmp_x = (pPoints->Val(Warp_CX-1,Warp_CY-1).x - pPoints->Val(Warp_CX-1,0).x)/(Warp_CY-1);
	tmp_y = (pPoints->Val(Warp_CX-1,Warp_CY-1).y - pPoints->Val(Warp_CX-1,0).y)/(Warp_CY-1);
	for(i =0;i<Warp_CY;i++) 
	{
		pPoints->X(Warp_CX-1,i) = pPoints->Val(Warp_CX-1,0).x + tmp_x*i;
		pPoints->Y(Warp_CX-1,i) = pPoints->Val(Warp_CX-1,0).y + tmp_y*i;
	}

	//�Զ������м����������㡣
	//0-- Warp_CY-1�� �� ���� 
	for(i=0;i<Warp_CY;i++)
	{
		Warp_Auto_RowX(i);
	}

	//0-- Warp_CX-1�� �� ���� 
	for(j=0;j<Warp_CX;j++)
	{
		Warp_Auto_RowY(j);
	}
}

//����ĳһ�е�ˮƽ�ļ�ࡣ x����ľ���
void CMGeometricAjustDlg::Warp_Auto_RowX(int row_y_index)
{
	int j;
	double tmp_x,tmp_value;
	//double tmp_diff_x;
	CDoublePointArray* pPoints = m_pDpa4points;
	int Warp_CY  = m_gridsetting.cy;
	int Warp_CX  = m_gridsetting.cx;

	tmp_x = (pPoints->Val(Warp_CX-1,row_y_index).x - pPoints->Val(0,row_y_index).x)/(Warp_CX-1);
	for(j=0;j<Warp_CX;j++)
	{
		tmp_value=  pPoints->Val(0,row_y_index).x + tmp_x*j;
		pPoints->X(j,row_y_index) = tmp_value;
	}
}

//����ĳһ�еĴ�ֱ��ࡣy����ľ���
void CMGeometricAjustDlg::Warp_Auto_RowY(int column_x_index)
{
	int i;
	double tmp_y,tmp_value;
	//	double tmp_diff_x,tmp_diff_y;
	CDoublePointArray* pPoints = m_pDpa4points;
	int Warp_CY  = m_gridsetting.cy;
	int Warp_CX  = m_gridsetting.cx;

	tmp_y = (pPoints->Val(column_x_index,Warp_CY-1).y - pPoints->Val(column_x_index,0).y)/(Warp_CY-1);
	for(i=0;i<Warp_CY;i++)
	{
		tmp_value=  pPoints->Val(column_x_index,0).y + tmp_y*i;
		pPoints->Y(column_x_index,i) = tmp_value;
	}
}

//���߼����㷨 1: ������������ 2: �������վֲ� 3: �������β�ֵ 4: ���Ӷ���ʽ��ֵ 5���ɵ�������ֵ 3			
void CMGeometricAjustDlg::Fitting()
{
	if(m_pDpaCurvepoints == NULL)
		return;

	double SetX[10];
	double SetY[10];

	int cy  = m_gridsetting.cy;
	int cx  = m_gridsetting.cx;
	CDoublePointArray* pdpacur = m_pDpaCurvepoints;

	int x_index = m_ptCursorCurve.x; // //���µ�Ϊ�ڼ����� 
	int y_index = m_ptCursorCurve.y;
	int NUM = 3;

	SetX[0] = pdpacur->X(0,y_index);
	SetY[0] = pdpacur->Y(0,y_index);
	SetX[1] = pdpacur->X((cx-1)/2,y_index);
	SetY[1] = pdpacur->Y((cx-1)/2,y_index);
	SetX[2] = pdpacur->X(cx-1,y_index);
	SetY[2] = pdpacur->Y(cx-1,y_index);

	//x�������
	for(int i=0;i<(cx-1)/2;i++)//ˮƽ����ļ��� 
	{
		double xxx = (SetX[1] - SetX[0]) / ((cx-1)/2) * i + SetX[0];
		pdpacur->X(i,y_index) = xxx;
	}
	for(int i=(cx-1)/2;i<cx;i++)//ˮƽ����ļ��� 
	{
		pdpacur->X(i,y_index) = (SetX[2] - SetX[1]) / ((cx-1)/2) * (i-(cx-1)/2) + SetX[1];
	}

	for(int i=0;i<cx;i++)//ˮƽ����ļ��� 
	{
		CDoublePoint &dbPoint = pdpacur->Val(i,y_index);

		dbPoint.y = Interpol_2Pow_3Point(SetX,SetY,dbPoint.x,NUM);
	}

	SetX[0] = pdpacur->X(x_index,0);
	SetY[0] = pdpacur->Y(x_index,0);
	SetX[1] = pdpacur->X(x_index,(cy-1)/2);
	SetY[1] = pdpacur->Y(x_index,(cy-1)/2);
	SetX[2] = pdpacur->X(x_index,cy-1);
	SetY[2] = pdpacur->Y(x_index,cy-1);

	//y�������
	for(int i=0;i<(cy-1)/2;i++) 
	{
		pdpacur->Y(x_index,i) = (SetY[1] - SetY[0]) / ((cy-1)/2) * i + SetY[0];
	}
	for(int i=(cy-1)/2+1;i<cy;i++) 
	{
		pdpacur->Y(x_index,i) = (SetY[2] - SetY[1]) / ((cy-1)/2) * (i-(cy-1)/2) + SetY[1];
	}

	for(int i=0;i<cy;i++)
	{
		CDoublePoint &dbPoint = pdpacur->Val(x_index,i);

		dbPoint.x = Interpol_2Pow_3Point(SetY,SetX,dbPoint.y,NUM);

	}

	Warp_Auto_XY();
}

void CMGeometricAjustDlg::Warp_Auto_XY()
{
	int i,j;
	double tmp_value;
	double tmp_diff_x,tmp_diff_y;

	int cy = m_gridsetting.cy;
	int cx = m_gridsetting.cx;

	CDoublePointArray* pdpacur = m_pDpaCurvepoints;

	//y������� 
	for(i=0;i<cx;i++)
	{	
		tmp_diff_y = (pdpacur->Y(i,(cy-1)/2) - pdpacur->Y(i,0) ) / ((cy-1)/2);
		for(j=0;j<(cy-1)/2;j++)
		{
			tmp_value=  pdpacur->Y(i,0) + tmp_diff_y*j;
			pdpacur->Y(i,j) = tmp_value;
		}

		tmp_diff_y = (pdpacur->Y(i,cy-1) - pdpacur->Y(i,(cy-1)/2)) / ((cy-1)/2);
		for(j=(cy-1)/2;j<cy;j++)
		{
			tmp_value=  pdpacur->Y(i,(cy-1)/2) + tmp_diff_y*(j-(cy-1)/2);
			pdpacur->Y(i,j) = tmp_value;
		}
	}

	CDoublePoint test[17*17];
	for(int i=0;i<17;i++)//y
	{
		for(int j=0;j<17;j++)//x
		{
			test[i*17+j].x = pdpacur->Val(j,i).x;
			test[i*17+j].y = pdpacur->Val(j,i).y;
		}
	}

	//x������� 
	for(i=0;i<cy;i++)
	{	
		tmp_diff_x = (pdpacur->X((cx-1)/2,i) - pdpacur->X(0,i) ) / ((cx-1)/2);
		for(j=0;j<(cx-1)/2;j++)
		{
			tmp_value=  pdpacur->X(0,i) + tmp_diff_x*j;
			pdpacur->X(j,i) = tmp_value;
		}

		tmp_diff_x = (pdpacur->X(cx-1,i) - pdpacur->X((cx-1)/2,i))/ ((cx-1)/2);
		for(j=(cx-1)/2;j<cx;j++)
		{
			tmp_value=  pdpacur->X((cx-1)/2,i) + tmp_diff_x*(j-(cx-1)/2);
			pdpacur->X(j,i) = tmp_value;
		}
	}
}

//������ƽ�ƣ���ֱ��ˮƽ�ƶ���ͬ���ƶ���
void CMGeometricAjustDlg::Warp_Auto_Ranks_Translation()
{
	double q = 1.0;//1.0;
	int i,x,y;
	double x_offset,y_offset;

	CDoublePointArray* pPoints = m_pDpaDensitypoints;

	int cy = m_gridsetting.cy;
	int cx = m_gridsetting.cx;

	int pt_cur_x = m_ptCursorDensity.x;
	int pt_cur_y = m_ptCursorDensity.y;

	int nAvr_x = cx/4;
	int nAvr_y = cy/4;

	// ************************ y���� ����  ***************************************************/
	for(i=0;i<cx;i++) //y����ƽ�� , yֵ�仯��xֵ���仯 
	{
		if(i != pt_cur_x)
		{
			pPoints->Y(i,pt_cur_y) =  pPoints->Val(pt_cur_x, pt_cur_y).y;
		}			
	}

	if(pt_cur_y != 0)//���Ƕ���һ�У����ϱ߾��� 
	{
		y_offset = (pPoints->Y(0,pt_cur_y) - pPoints->Y(0,pt_cur_y-nAvr_y))/nAvr_y ;

		for(y=pt_cur_y-nAvr_y+1;y<pt_cur_y;y++)
		{
			for(x=0;x<cx;x++)
			{
				pPoints->Y(x,y) =  pPoints->Val(pt_cur_x, pt_cur_y-nAvr_y).y + y_offset*( y - (pt_cur_y-nAvr_y) );
			}
		}
	}

	if(pt_cur_y != (cy-1))//������ײ�һ�У����±߾��� 
	{
		y_offset = (pPoints->Y(0,pt_cur_y+nAvr_y) - pPoints->Y(0,pt_cur_y))/nAvr_y ;

		for(y=pt_cur_y+1;y<pt_cur_y+nAvr_y;y++)
		{
			for(x=0;x<cx;x++)
			{
				pPoints->Y(x,y) =  pPoints->Val(pt_cur_x, pt_cur_y).y + y_offset*( y - pt_cur_y );
			}
		}
	}

	// ************************ x���� ����  ***************************************************
	for(i=0;i<cy;i++) //x����ƽ�� ,xֵ�仯��yֵ���仯 
	{
		if(i != pt_cur_y)
		{
			pPoints->X(pt_cur_x,i) =  pPoints->Val(pt_cur_x, pt_cur_y).x;
		}			
	}

	if(pt_cur_x != 0)//��������ߵ�һ�У�����߾���
	{
		x_offset = (pPoints->X(pt_cur_x,0) - pPoints->X(pt_cur_x-nAvr_x,0))/nAvr_x ;

		for(x=pt_cur_x-nAvr_x+1;x<pt_cur_x;x++)
		{
			for(y=0;y<cy;y++)
			{
				pPoints->X(x,y) =  pPoints->Val(pt_cur_x-nAvr_x,pt_cur_y).x + x_offset*( x - (pt_cur_x-nAvr_x) );
			}
		}
	}
	if(pt_cur_x != (cx-1))//�������ұߵ�һ�У����ұ߾���
	{
		x_offset = (pPoints->X(pt_cur_x+nAvr_x,0) - pPoints->X(pt_cur_x,0))/nAvr_x ;

		for(x=pt_cur_x+1;x<pt_cur_x+nAvr_x;x++)
		{
			for(y=0;y<cy;y++)
			{
				pPoints->X(x,y) =  pPoints->Val(pt_cur_x, pt_cur_y).x + x_offset*( x - pt_cur_x );
			}
		}
	}

}
void CMGeometricAjustDlg::OnDeltaposSpinPointPos(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	CXTPSpinButtonCtrl *pSpin = (CXTPSpinButtonCtrl*)FromHandle(pNMHDR->hwndFrom);
	CXTPEdit *pEdit = (CXTPEdit*)pSpin->GetBuddy();

	CString strVal;
	pEdit->GetWindowText(strVal);
	double dVal = atof(strVal);

	dVal = pNMUpDown->iDelta*0.5;

	VKEnterInEditCtrls(pEdit->m_hWnd, dVal, FALSE);

	if (m_eAjustStyle == eKeystone) On4pointAjust(CPoint(0,0), FALSE);
	else if(m_eAjustStyle == eCurve) OnCurveAjust(CPoint(0,0), FALSE);
	else if(m_eAjustStyle == eDensity) OnDensityAjust(CPoint(0,0), FALSE);

	*pResult = 0;
}


void CMGeometricAjustDlg::Initialize()
{
	CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
	//MG����ͨ�Ŷ˿ں�
	int nPort = pOwer->m_boardInfo.GetBoardPort();

	//��ȡ��ǰ�ֱ���
	TIMING_STRUCT ts;
	//pOwer->GetCurrencyTiming(ts);
	if (theApp.GetCurrencyInputTiming((UINT&)ts.m_Hz, (UINT&)ts.m_HTotal, (UINT&)ts.m_HActive, (UINT&)ts.m_VTotal, (UINT&)ts.m_VActive, nPort) != CMD_FAILURE)
	{		
		if (ts.m_HActive <= 0 || ts.m_VActive <= 0 || ts.m_Hz <= 0)
		{
			MessageBox(_T("���棺��ǰ�忨δ�����źţ�����У��������Ĭ�Ϸֱ���(1024x768 @ 60Hz)��"),_T("����У����ʼ��"), MB_OK|MB_ICONERROR);
			ts = pOwer->m_boardInfo.GetDetailTiming(Product_MG)[0];
		}
	}
	else
	{
		MessageBox(_T("����δ�ܻ�ȡ����ǰ�忨����ֱ��ʣ�����У��������Ĭ�Ϸֱ���(1024x768 @ 60Hz)��"),_T("����У����ʼ��"), MB_OK|MB_ICONERROR);
		ts = pOwer->m_boardInfo.GetDetailTiming(Product_MG)[0];
	}

	//��������֮��������ֱ���
	m_gridsetting.cxResIn  = ts.m_HActive;
	m_gridsetting.cyResIn  = ts.m_VActive;
	m_gridsetting.cxResOut = ts.m_HActive;
	m_gridsetting.cyResOut = ts.m_VActive;	
	m_gridsetting.nHz      = ts.m_Hz;

	SetGraphicSize();

	//��ȡWarp����
	UINT nWarpData[64] = {0};

	if (theApp.MG_Get4PointPosition(nWarpData, nPort) != CMD_FAILURE)	{
		Init4PointPosition(nWarpData);
	}
	else	{
		return (void)MessageBox(_T("������ʼ���ĵ�У������ʧ�ܣ�"), _T("����У����ʼ��"), MB_OK|MB_ICONSTOP);
	}

	memset(nWarpData, 0, sizeof(nWarpData));
	if (theApp.MG_GetCurvePostion(nWarpData, nPort) != CMD_FAILURE)	{
		InitCurvePosition(nWarpData);
	}
	else	{
		return (void)MessageBox(_T("������ʼ������У������ʧ�ܣ�"), _T("����У����ʼ��"), MB_OK|MB_ICONSTOP);
	}

	memset(nWarpData, 0, sizeof(nWarpData));
	if (theApp.MG_GetDensityPosition(nWarpData, nPort) != CMD_FAILURE)	{
		InitDensityPosition(nWarpData);
	}
	else	{
		return (void)MessageBox(_T("������ʼ������У������ʧ�ܣ�"), _T("����У����ʼ��"), MB_OK|MB_ICONSTOP);
	}

	//�ػ�
	InvalidateRect(m_rcPaint);

	//ˢ�±༭���е��������ֵ 
	if (m_eAjustStyle == eKeystone)	    Show4pointCoords();
	else if (m_eAjustStyle == eCurve)   ShowCurveCoords();
	else if (m_eAjustStyle == eDensity) ShowDensityCoords();
}

void CMGeometricAjustDlg::Init4PointPosition(UINT *p4PointPos)
{
	//��ֵ
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	if (p4PointPos[0] == 0xff) //������Ч
	{
		m_pDpa4points->X(0,0)         =     0;         
		m_pDpa4points->Y(0,0)         =     0;         
		m_pDpa4points->X(cx-1,0)      =     m_gridsetting.cxResIn;          
		m_pDpa4points->Y(cx-1,0)      =     0;          
		m_pDpa4points->X(0,cy-1)      =     0;          
		m_pDpa4points->Y(0,cy-1)      =     m_gridsetting.cyResIn;          
		m_pDpa4points->X(cx-1,cy-1)   =     m_gridsetting.cxResIn;          
		m_pDpa4points->Y(cx-1,cy-1)   =     m_gridsetting.cyResIn; 
	}
	else
	{
		m_pDpa4points->X(0,0)         =     p4PointPos[1]/16.0;          
		m_pDpa4points->Y(0,0)         =     p4PointPos[2]/16.0;          
		m_pDpa4points->X(cx-1,0)      =     p4PointPos[3]/16.0;          
		m_pDpa4points->Y(cx-1,0)      =     p4PointPos[4]/16.0;          
		m_pDpa4points->X(0,cy-1)      =     p4PointPos[5]/16.0;          
		m_pDpa4points->Y(0,cy-1)      =     p4PointPos[6]/16.0;          
		m_pDpa4points->X(cx-1,cy-1)   =     p4PointPos[7]/16.0;          
		m_pDpa4points->Y(cx-1,cy-1)   =     p4PointPos[8]/16.0; 
	}

	//���������
	Warp_Auto_Kestone();
}

void CMGeometricAjustDlg::InitCurvePosition(UINT *pCurvePos)
{
	//��ֵ
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	if (pCurvePos[0] == 0xff) //������Ч
	{
		m_pDpaCurvepoints->X(0,0)                =      0;       
		m_pDpaCurvepoints->Y(0,0)                =      0;       
		m_pDpaCurvepoints->X((cx-1)/2,0)         =      m_gridsetting.cxResIn/2.0;      
		m_pDpaCurvepoints->Y((cx-1)/2,0)         =      0;       
		m_pDpaCurvepoints->X(cx-1,0)             =      m_gridsetting.cxResIn;    
		m_pDpaCurvepoints->Y(cx-1,0)             =      0;

		m_pDpaCurvepoints->X(0,(cy-1)/2)         =      0;       
		m_pDpaCurvepoints->Y(0,(cy-1)/2)         =      m_gridsetting.cyResIn/2.0;       
		m_pDpaCurvepoints->X((cx-1)/2,(cy-1)/2)  =      m_gridsetting.cxResIn/2.0;       
		m_pDpaCurvepoints->Y((cx-1)/2,(cy-1)/2)  =      m_gridsetting.cyResIn/2.0;       
		m_pDpaCurvepoints->X(cx-1,(cy-1)/2)      =      m_gridsetting.cxResIn;       
		m_pDpaCurvepoints->Y(cx-1,(cy-1)/2)      =      m_gridsetting.cyResIn/2.0; 

		m_pDpaCurvepoints->X(0,cy-1)             =      0;      
		m_pDpaCurvepoints->Y(0,cy-1)             =      m_gridsetting.cyResIn;       
		m_pDpaCurvepoints->X((cx-1)/2,cy-1)      =      m_gridsetting.cxResIn/2.0;      
		m_pDpaCurvepoints->Y((cx-1)/2,cy-1)      =      m_gridsetting.cyResIn;       
		m_pDpaCurvepoints->X(cx-1,cy-1)          =      m_gridsetting.cxResIn;       
		m_pDpaCurvepoints->Y(cx-1,cy-1)          =      m_gridsetting.cyResIn;    
	}
	else
	{
		m_pDpaCurvepoints->X(0,0)                =      pCurvePos[1] /16.0;       
		m_pDpaCurvepoints->Y(0,0)                =      pCurvePos[2] /16.0;       
		m_pDpaCurvepoints->X((cx-1)/2,0)         =      pCurvePos[3] /16.0;       
		m_pDpaCurvepoints->Y((cx-1)/2,0)         =      pCurvePos[4] /16.0;       
		m_pDpaCurvepoints->X(cx-1,0)             =      pCurvePos[5] /16.0;       
		m_pDpaCurvepoints->Y(cx-1,0)             =      pCurvePos[6] /16.0;       
		m_pDpaCurvepoints->X(0,(cy-1)/2)         =      pCurvePos[7] /16.0;       
		m_pDpaCurvepoints->Y(0,(cy-1)/2)         =      pCurvePos[8] /16.0;       
		m_pDpaCurvepoints->X((cx-1)/2,(cy-1)/2)  =      pCurvePos[9] /16.0;       
		m_pDpaCurvepoints->Y((cx-1)/2,(cy-1)/2)  =      pCurvePos[10]/16.0;       
		m_pDpaCurvepoints->X(cx-1,(cy-1)/2)      =      pCurvePos[11]/16.0;       
		m_pDpaCurvepoints->Y(cx-1,(cy-1)/2)      =      pCurvePos[12]/16.0;       
		m_pDpaCurvepoints->X(0,cy-1)             =      pCurvePos[13]/16.0;       
		m_pDpaCurvepoints->Y(0,cy-1)             =      pCurvePos[14]/16.0;       
		m_pDpaCurvepoints->X((cx-1)/2,cy-1)      =      pCurvePos[15]/16.0;       
		m_pDpaCurvepoints->Y((cx-1)/2,cy-1)      =      pCurvePos[16]/16.0;       
		m_pDpaCurvepoints->X(cx-1,cy-1)          =      pCurvePos[17]/16.0;       
		m_pDpaCurvepoints->Y(cx-1,cy-1)          =      pCurvePos[18]/16.0;
	}

	//����
	for (int i=0; i<3; i++)
	{
		m_ptCursorCurve = CPoint(i*(cx/2), i*(cy/2));
		Fitting();
	}
}

void CMGeometricAjustDlg::InitDensityPosition(UINT *pDensityPos)
{
	//��ֵ
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	if (pDensityPos[0] == 0xff) //������Ч
	{
		for(int i=0;i<5;i++)
		{
			//ˮƽ���ܶȵ��ڵ�λ��
			m_pDpaDensitypoints->X((cx-1)/4*i,(cy-1)/2) = m_gridsetting.cxResIn*i/4.0;
			//��ֱ���ܶȵ��ڵ�λ��
			m_pDpaDensitypoints->Y((cx-1)/2,(cy-1)/4*i)  = m_gridsetting.cyResIn*i/4.0;
		}
	}
	else
	{
		for(int i=0;i<5;i++)
		{
			//ˮƽ���ܶȵ��ڵ�λ��
			m_pDpaDensitypoints->X((cx-1)/4*i,(cy-1)/2) = pDensityPos[i+1]/16.0;
			//��ֱ���ܶȵ��ڵ�λ��
			m_pDpaDensitypoints->Y((cx-1)/2,(cy-1)/4*i)  = pDensityPos[i+6]/16.0;
		}
	}

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			m_pDpaDensitypoints->X((cx-1)/4*i,(cy-1)/4*j) = m_pDpaDensitypoints->X((cx-1)/4*i,(cy-1)/2);
		}
	}

	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			m_pDpaDensitypoints->Y((cx-1)/4*j,(cy-1)/4*i) = m_pDpaDensitypoints->Y((cx-1)/2,(cy-1)/4*i);
		}
	}

	//����
	for (int i=0; i<5; i++)
	{
		m_ptCursorDensity = CPoint(i*(cx/4), cy/2);
		Warp_Auto_Ranks_Translation();
	}
	for (int i=0; i<5; i++)
	{
		m_ptCursorDensity = CPoint(cx/2, i*(cy/4));
		Warp_Auto_Ranks_Translation();
	}
}

BOOL CMGeometricAjustDlg::SaveGeoDatas()
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	int nPort = ((CMGeoMainDlg*)GetOwner())->m_boardInfo.GetBoardPort();

	UINT nData[64] = {0};
	int nTick = 0;
	nData[nTick++] = 0; //Ĭ��Warp���Ϊ0
	
	//�̻�������Warp��
	if (theApp.MG_SaveWarpTable(nData[0], nPort) == CMD_FAILURE) return FALSE;

	//�����ĵ�
	for (int y=0; y<2; y++)	{
		for (int x=0; x<2; x++)		{
			nData[nTick++] = int(m_pDpa4points->X(x*(cx-1), y*(cy-1))*16);
			nData[nTick++] = int(m_pDpa4points->Y(x*(cx-1), y*(cy-1))*16);
		}			
	}

	if (theApp.MG_Save4PointPosition(nData, nPort) == CMD_FAILURE) return FALSE;

	//��������
	nTick = 1;
	for (int y=0; y<3; y++)	{
		for (int x=0; x<3; x++)	{
			nData[nTick++] = int(m_pDpaCurvepoints->X(x*(cx/2), y*(cy/2))*16);
			nData[nTick++] = int(m_pDpaCurvepoints->Y(x*(cx/2), y*(cy/2))*16);
		}
	}

	if (theApp.MG_SaveCurvePostion(nData, nPort) == CMD_FAILURE) return FALSE;

	//��������
	nTick = 1;
	for (int x=0; x<5; x++)
		nData[nTick++] = int(m_pDpaDensitypoints->X(x*(cx/4), cy/2)*16);
	for (int y=0; y<5; y++)
		nData[nTick++] = int(m_pDpaDensitypoints->Y(cx/2, y*(cy/4))*16);

	if (theApp.MG_SaveDensityPosition(nData, nPort) == CMD_FAILURE) return FALSE;

	return TRUE;
}