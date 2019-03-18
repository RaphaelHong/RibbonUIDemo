// MGblendsetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGblendsetDlg.h"
#include "MGeoMainDlg.h"
#include "MGAdvancedBlendParamSetDlg.h"

// CMGblendsetDlg �Ի���

IMPLEMENT_DYNAMIC(CMGblendsetDlg, CXTPResizeDialog)

CMGblendsetDlg::CMGblendsetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGblendsetDlg::IDD, pParent)
	, m_nBlendSel(0)
	, m_nPureWhite(0)
	, m_nWhiteLvl(240)
	, m_nAlpha(5)
    , m_nP(20)
	, m_nGamma(20)
	, m_nEnableBlending(0)
{
	for (int i=0; i<4; i++)
	{
		m_nBlendwidth[i] = m_stBlendSetting[i].nBlendWidth = 256;
		m_stBlendSetting[i].nPureWhite = 0;
		m_stBlendSetting[i].nWhiteLvl = 240;
		m_stBlendSetting[i].fAlpha[0] = m_stBlendSetting[i].fAlpha[1] = 0.5;
		m_stBlendSetting[i].fP[0] = m_stBlendSetting[i].fP[1] = 2.0;
		m_stBlendSetting[i].fGamma[0] = m_stBlendSetting[i].fGamma[1] = 2.0;
	}
}

CMGblendsetDlg::~CMGblendsetDlg()
{
}

void CMGblendsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LEFT_BLEND, m_CheckBlend[LEFT_BLEND]);
	DDX_Control(pDX, IDC_CHECK_TOP_BLEND, m_CheckBlend[TOP_BLEND]);
	DDX_Control(pDX, IDC_CHECK_RIGHT_BLEND, m_CheckBlend[RIGHT_BLEND]);
	DDX_Control(pDX, IDC_CHECK_BOTTOM_BLEND, m_CheckBlend[BOTTOM_BLEND]);
	DDX_Control(pDX, IDC_EDIT_LEFT_BLEND, m_EditBlend[LEFT_BLEND]);
	DDX_Control(pDX, IDC_EDIT_TOP_BLEND, m_EditBlend[TOP_BLEND]);
	DDX_Control(pDX, IDC_EDIT_RIGHT_BLEND, m_EditBlend[RIGHT_BLEND]);
	DDX_Control(pDX, IDC_EDIT_BOTTOM_BLEND, m_EditBlend[BOTTOM_BLEND]);
	DDX_Control(pDX, IDC_SPIN_LEFT_BLEND, m_SpinBlend[LEFT_BLEND]);
	DDX_Control(pDX, IDC_SPIN_TOP_BLEND, m_SpinBlend[TOP_BLEND]);
	DDX_Control(pDX, IDC_SPIN_RIGHT_BLEND, m_SpinBlend[RIGHT_BLEND]);
	DDX_Control(pDX, IDC_SPIN_BOTTOM_BLEND, m_SpinBlend[BOTTOM_BLEND]);
	DDX_Control(pDX, IDC_RADIO_LEFT_BLEND, m_RadioBlend[LEFT_BLEND]);
	DDX_Control(pDX, IDC_RADIO_TOP_BLEND, m_RadioBlend[TOP_BLEND]);
	DDX_Control(pDX, IDC_RADIO_RIGHT_BLEND, m_RadioBlend[RIGHT_BLEND]);
	DDX_Control(pDX, IDC_RADIO_BOTTOM_BLEND, m_RadioBlend[BOTTOM_BLEND]);
	DDX_Control(pDX, IDC_GBOX_BLENDWIDTH, m_GboxBlendWid);
	DDX_Control(pDX, IDC_GBOX_WHITEMODE, m_GboxWhiteMode);
	DDX_Control(pDX, IDC_GBOX_BLENDPARA, m_GboxBlendPara);
	DDX_Control(pDX, IDC_RADIO_NON_PUREWHITE, m_RadioNonWhite);
	DDX_Control(pDX, IDC_RADIO_PUREWHITE, m_RadioPurWhite);
	DDX_Control(pDX, IDC_EDIT_WHITELVL, m_EditWhiteLvl);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_EditAlpha);
	DDX_Control(pDX, IDC_EDIT_P, m_EditP);
	DDX_Control(pDX, IDC_EDIT_GAMMA, m_EditGamma);
	DDX_Control(pDX, IDC_CHECK_ENABLE_BLENDING, m_CheckEnBlend);
	DDX_Control(pDX, IDC_BTN_SET_WHITELVL, m_BtnSetWhiteLvl);
	DDX_Control(pDX, IDC_BTN_DEF_BLENDPARA, m_BtnDefParam);
	DDX_Control(pDX, IDC_BTN_TEST_BLENDPARA, m_BtnTestParam);
	DDX_Control(pDX, IDC_BTN_SET_BLENDWID, m_BtnSetBlendWid);
	DDX_Control(pDX, IDC_BTN_ADVANCED_PARA, m_BtnAdvanced);
	DDX_Control(pDX, IDC_SLIDER_ALPHA, m_SliderAlpha);
	DDX_Control(pDX, IDC_SLIDER_P, m_SliderP);
	DDX_Control(pDX, IDC_SLIDER_GAMMA, m_SliderGamma);

	DDX_Text(pDX, IDC_EDIT_LEFT_BLEND, m_nBlendwidth[LEFT_BLEND]);
	DDX_Text(pDX, IDC_EDIT_TOP_BLEND, m_nBlendwidth[TOP_BLEND]);
	DDX_Text(pDX, IDC_EDIT_RIGHT_BLEND, m_nBlendwidth[RIGHT_BLEND]);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_BLEND,m_nBlendwidth[BOTTOM_BLEND]);
	DDX_Text(pDX, IDC_EDIT_WHITELVL, m_nWhiteLvl);

	DDX_Radio(pDX, IDC_RADIO_LEFT_BLEND, m_nBlendSel);
	DDX_Check(pDX, IDC_CHECK_ENABLE_BLENDING, m_nEnableBlending);
	DDX_Radio(pDX, IDC_RADIO_NON_PUREWHITE, m_nPureWhite);

	DDX_Slider(pDX, IDC_SLIDER_ALPHA, m_nAlpha);
	DDX_Slider(pDX, IDC_SLIDER_P, m_nP);
	DDX_Slider(pDX, IDC_SLIDER_GAMMA, m_nGamma);
}


BEGIN_MESSAGE_MAP(CMGblendsetDlg, CXTPResizeDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_COMMAND_RANGE(IDC_CHECK_LEFT_BLEND, IDC_CHECK_BOTTOM_BLEND, OnCheckBlendSel)
	ON_COMMAND_RANGE(IDC_RADIO_LEFT_BLEND, IDC_RADIO_BOTTOM_BLEND, OnRadioBlendSel)
	ON_COMMAND_RANGE(IDC_RADIO_NON_PUREWHITE, IDC_RADIO_PUREWHITE, OnRadioPureWhiteSel)
	ON_NOTIFY_RANGE(UDN_DELTAPOS, IDC_SPIN_LEFT_BLEND, IDC_SPIN_BOTTOM_BLEND, OnDeltaposSpinBlendWidth)
	ON_BN_CLICKED(IDC_BTN_SET_BLENDWID, OnBnClickedBtnSetBlendWidth)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_BLENDING, OnBnClickedCheckEnableBlend)
	ON_BN_CLICKED(IDC_BTN_SET_WHITELVL, OnBnClickedBtnSetWhiteLevel)
	ON_BN_CLICKED(IDC_BTN_DEF_BLENDPARA, OnBnClickedBtnDefBlendParam)
	ON_BN_CLICKED(IDC_BTN_TEST_BLENDPARA, OnBnClickedBtnTestBlendPara)
	ON_BN_CLICKED(IDC_BTN_ADVANCED_PARA, OnBnClickedBtnAdvancedParam)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, &CMGblendsetDlg::OnEnChangeEditAlpha)
	ON_EN_CHANGE(IDC_EDIT_P, &CMGblendsetDlg::OnEnChangeEditP)
	ON_EN_CHANGE(IDC_EDIT_GAMMA, &CMGblendsetDlg::OnEnChangeEditGamma)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMGblendsetDlg ��Ϣ�������

BOOL CMGblendsetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	//ֻ��һ���ߵ���Ͽ�
	m_GboxBlendPara.SetBorderStyle(xtpGroupBoxSingleLine);
	m_GboxWhiteMode.SetBorderStyle(xtpGroupBoxSingleLine);

	//���ÿؼ����
	SetControlTheme(xtpControlThemeResource);

	//�ؼ���Ի����С�仯λ��
	ResizeIt();

	//��������ʼ��
	m_SliderAlpha.SetRange(0,10);
	m_SliderP.SetRange(0,160);
	m_SliderGamma.SetRange(0,160);

	m_SliderAlpha.SetTicFreq(1);
	m_SliderP.SetTicFreq(1);
	m_SliderGamma.SetTicFreq(1);

	SetAlphaGammaP(TRUE);

	//��΢����ť�����ڵı༭�����
	m_SpinBlend[0].SetBuddy(&m_EditBlend[0]);
	m_SpinBlend[1].SetBuddy(&m_EditBlend[1]);
	m_SpinBlend[2].SetBuddy(&m_EditBlend[2]);
	m_SpinBlend[3].SetBuddy(&m_EditBlend[3]);

	//����������ˢ
	m_bkBrush.CreateSolidBrush(constClrDlgBk[3]);

	return FALSE; 
}

void CMGblendsetDlg::SetControlTheme(XTPControlTheme theme)
{	
	m_CheckBlend[LEFT_BLEND].SetTheme(theme);
	m_CheckBlend[TOP_BLEND].SetTheme(theme);
	m_CheckBlend[RIGHT_BLEND].SetTheme(theme);
	m_CheckBlend[BOTTOM_BLEND].SetTheme(theme);

	m_RadioBlend[LEFT_BLEND].SetTheme(theme);
	m_RadioBlend[TOP_BLEND].SetTheme(theme);
	m_RadioBlend[RIGHT_BLEND].SetTheme(theme);
	m_RadioBlend[BOTTOM_BLEND].SetTheme(theme);

	m_RadioNonWhite.SetTheme(theme);
	m_RadioPurWhite.SetTheme(theme);

	m_CheckEnBlend.SetTheme(theme);
	m_BtnSetBlendWid.SetTheme(theme);
	m_BtnSetWhiteLvl.SetTheme(theme);
	m_BtnDefParam.SetTheme(theme);
	m_BtnTestParam.SetTheme(theme);
	m_BtnAdvanced.SetTheme(theme);

	m_EditBlend[LEFT_BLEND].SetTheme(theme);
	m_EditBlend[TOP_BLEND].SetTheme(theme);
	m_EditBlend[RIGHT_BLEND].SetTheme(theme);
	m_EditBlend[BOTTOM_BLEND].SetTheme(theme);

	m_EditWhiteLvl.SetTheme(theme);
	m_EditAlpha.SetTheme(theme);
	m_EditP.SetTheme(theme);
	m_EditGamma.SetTheme(theme);

	m_SpinBlend[LEFT_BLEND].SetTheme(theme);
	m_SpinBlend[TOP_BLEND].SetTheme(theme);
	m_SpinBlend[RIGHT_BLEND].SetTheme(theme);
	m_SpinBlend[BOTTOM_BLEND].SetTheme(theme);
}

void CMGblendsetDlg::ResizeIt()
{
	//����ںϴ�
	SetResize(IDC_CHECK_LEFT_BLEND, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_EDIT_LEFT_BLEND, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_SPIN_LEFT_BLEND, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_RADIO_LEFT_BLEND, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_PIC_LEFT_BLEND, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMLEFT);
	//�����ںϴ�
	SetResize(IDC_CHECK_TOP_BLEND, CXTPResizePoint(0.25, 0), CXTPResizePoint(0.25, 0));
	SetResize(IDC_EDIT_TOP_BLEND, CXTPResizePoint(0.25, 0), CXTPResizePoint(0.25, 0));
	SetResize(IDC_SPIN_TOP_BLEND, CXTPResizePoint(0.25, 0), CXTPResizePoint(0.25, 0));
	SetResize(IDC_RADIO_TOP_BLEND, CXTPResizePoint(0.25, 0), CXTPResizePoint(0.25, 0));
	SetResize(IDC_PIC_TOP_BLEND, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	//�Ҳ��ںϴ�
	SetResize(IDC_CHECK_RIGHT_BLEND, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_RIGHT_BLEND, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);	
	SetResize(IDC_SPIN_RIGHT_BLEND, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);	
	SetResize(IDC_RADIO_RIGHT_BLEND, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_PIC_RIGHT_BLEND, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_BOTTOMCENTER);
	//�ײ��ںϴ�
	SetResize(IDC_CHECK_BOTTOM_BLEND, CXTPResizePoint(0.25, 1), CXTPResizePoint(0.25, 1));
	SetResize(IDC_EDIT_BOTTOM_BLEND, CXTPResizePoint(0.25, 1), CXTPResizePoint(0.25, 1));
	SetResize(IDC_SPIN_BOTTOM_BLEND, CXTPResizePoint(0.25, 1), CXTPResizePoint(0.25, 1));
	SetResize(IDC_RADIO_BOTTOM_BLEND, CXTPResizePoint(0.25, 1), CXTPResizePoint(0.25, 1));
	SetResize(IDC_PIC_BOTTOM_BLEND, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMCENTER);
	//���뱳��ͼƬ
	SetResize(IDC_PIC_BK_BLEND, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);

	//��Ͽ�
	SetResize(IDC_GBOX_BLENDWIDTH, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMCENTER);
	SetResize(IDC_GBOX_WHITEMODE, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_GBOX_BLENDPARA, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	//����ģʽ
	SetResize(IDC_RADIO_NON_PUREWHITE, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_RADIO_PUREWHITE, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_EDIT_WHITELVL, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_TEXT_WHITELVL, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_BTN_SET_WHITELVL, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	//Alpha
	SetResize(IDC_EDIT_ALPHA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SLIDER_ALPHA, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_TEXT_MIN_ALPHA, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_TEXT_MAX_ALPHA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_TEXT_ALPHA, CXTPResizePoint(0.75, 0), CXTPResizePoint(0.75, 0));
	//P
	SetResize(IDC_EDIT_P, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SLIDER_P, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_TEXT_MIN_P, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_TEXT_MAX_P, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_TEXT_P, CXTPResizePoint(0.75, 0), CXTPResizePoint(0.75, 0));
	//Gamma
	SetResize(IDC_EDIT_GAMMA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SLIDER_GAMMA, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);	
	SetResize(IDC_TEXT_MIN_GAMMA, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_TEXT_MAX_GAMMA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_TEXT_GAMMA, CXTPResizePoint(0.75, 0), CXTPResizePoint(0.75, 0));
	//�����ںϹ���
	SetResize(IDC_CHECK_ENABLE_BLENDING, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);
	//Ĭ�ϲ���
	SetResize(IDC_BTN_DEF_BLENDPARA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	//�����ں�Ч��
	SetResize(IDC_BTN_TEST_BLENDPARA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	//�����ںϴ����
	SetResize(IDC_BTN_SET_BLENDWID, XTP_ANCHOR_BOTTOMCENTER, XTP_ANCHOR_BOTTOMCENTER);
	//�߼��ںϲ���
	SetResize(IDC_BTN_ADVANCED_PARA, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);	
}


void CMGblendsetDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);
}

void CMGblendsetDlg::OnOK()
{
	//return CXTPResizeDialog::OnOK();
}
void CMGblendsetDlg::OnCancel()
{
	//return CXTPResizeDialog::OnCancel();
}


void CMGblendsetDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXTPResizeDialog::OnShowWindow(bShow, nStatus);
}

HBRUSH CMGblendsetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CMGblendsetDlg::OnCheckBlendSel(UINT nID)
{
	UpdateData();
	int nBlendSel = nID-IDC_CHECK_LEFT_BLEND;
	int nCheck = ((CButton*)GetDlgItem(nID))->GetCheck();

	ShowHideBlendSelCtrls(nBlendSel, nCheck==0 ? SW_HIDE : SW_SHOW);

	if (nCheck) m_nBlendSel = nBlendSel;
	else
	{
		for (int i=0; i<4; i++)
		{
			if (i != nBlendSel && (((CButton*)GetDlgItem(i+IDC_CHECK_LEFT_BLEND))->GetCheck()))
			{
				m_nBlendSel = i;
				break;
			}
		}
	}

	UpdateBlendParam(nBlendSel);
}

inline void CMGblendsetDlg::ShowHideBlendSelCtrls(int nBlendSel, int nShowHide)
{
	GetDlgItem(IDC_PIC_LEFT_BLEND+nBlendSel)->ShowWindow(nShowHide);
	GetDlgItem(IDC_EDIT_LEFT_BLEND+nBlendSel)->ShowWindow(nShowHide);
	GetDlgItem(IDC_SPIN_LEFT_BLEND+nBlendSel)->ShowWindow(nShowHide);
	GetDlgItem(IDC_RADIO_LEFT_BLEND+nBlendSel)->ShowWindow(nShowHide);
}


void CMGblendsetDlg::OnRadioBlendSel(UINT nID)
{
	//ѡ��û�б仯 ��ˢ��
	if (m_nBlendSel == (nID-IDC_RADIO_LEFT_BLEND)) return;

	UpdateData();

	//�л��ںϴ���ˢ�µ�ǰ�ںϴ����ںϲ���
	UpdateBlendParam(m_nBlendSel);
}

void CMGblendsetDlg::OnRadioPureWhiteSel(UINT nID)
{
	//ѡ��û�б仯��ˢ��
	if (m_nPureWhite == (nID-IDC_RADIO_NON_PUREWHITE)) return;

	UpdateData();

	//�л�����ģʽ�������ںϲ���
	m_stBlendSetting[m_nBlendSel].nPureWhite = m_nPureWhite;
	UpdateBlendParam(m_nBlendSel);
}

void CMGblendsetDlg::OnDeltaposSpinBlendWidth(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData();

	//���±༭���е��ںϴ����ֵ���Ƿ��·����������ȷ��
	int nSel = nID-IDC_SPIN_LEFT_BLEND;

	int nNewVal = m_nBlendwidth[nSel] - 2*pNMUpDown->iDelta;

	if (nNewVal > 512)  nNewVal = 512;
	if (nNewVal < 0) nNewVal = 0;

	nNewVal = nNewVal-(nNewVal%2); //żֵ��

	m_nBlendwidth[nSel] = nNewVal;

	UpdateData(FALSE);
}

void CMGblendsetDlg::OnBnClickedBtnSetBlendWidth()
{
	UpdateData();
	if (!ValidateBlendWidth()) return;
	WriteBlendWidth();
}

void CMGblendsetDlg::OnBnClickedCheckEnableBlend()
{
	UpdateData();
	SetOnBlending();
}

void CMGblendsetDlg::OnBnClickedBtnSetWhiteLevel()
{
	UpdateData();
	WriteWhiteLevel();
}

void CMGblendsetDlg::OnBnClickedBtnDefBlendParam()
{
	UpdateData();
	SetAlphaGammaP(TRUE);
	TestAlphaGammaP();
}

void CMGblendsetDlg::OnBnClickedBtnTestBlendPara()
{
	UpdateData();
	TestAlphaGammaP();
}

void CMGblendsetDlg::OnBnClickedBtnAdvancedParam()
{
	UpdateData();
	SetAdvancedBlendParam();
}

// ���л��ںϴ�ʱ �����ںϲ���
void CMGblendsetDlg::UpdateBlendParam(int nBlendSel)
{
	m_nPureWhite = m_stBlendSetting[nBlendSel].nPureWhite;
	m_nAlpha     = int(m_stBlendSetting[nBlendSel].fAlpha[m_nPureWhite]*10);
	m_nP         = int(m_stBlendSetting[nBlendSel].fP[m_nPureWhite]*10);
	m_nGamma     = int(m_stBlendSetting[nBlendSel].fGamma[m_nPureWhite]*10);

	SetAlphaGammaP(FALSE);
	UpdateData(FALSE);
}

// �����͹ر��ںϹ���
void CMGblendsetDlg::SetOnBlending()
{
	CMGeoMainDlg* pDlg = (CMGeoMainDlg*)GetOwner();
	
	if (theApp.MG_EnableBlend(m_nEnableBlending, pDlg->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("��������ʧ�ܣ�"), _T("�����ںϹ���"), MB_OK|MB_ICONSTOP);

		m_nEnableBlending = !m_nEnableBlending;
		UpdateData(FALSE);
	}
}

// �����ںϴ����
void CMGblendsetDlg::WriteBlendWidth()
{
	//����
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();

	int nBW[4] = {0};
	for (int k=0; k<4; k++) 
		nBW[k] = m_CheckBlend[k].GetCheck() ? m_nBlendwidth[k] : 0;

	if (theApp.MG_SetBlendWidth((UINT*)nBW, pDlg->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("���������ںϴ����ʧ�ܣ�"), _T("�ںϴ�����"), MB_OK|MB_ICONSTOP);
	}
	else
	{
		for (int i=0; i<4; i++)
		{
			if (m_CheckBlend[i].GetCheck())
				m_stBlendSetting[i].nBlendWidth = m_nBlendwidth[i];
			else
				m_stBlendSetting[i].nBlendWidth = 0;
		}

		pDlg->NotifyBlendWidthChanged();
	}
}

// ���ô��׷�ֵ
void CMGblendsetDlg::WriteWhiteLevel()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();
	if (theApp.MG_SetWhiteThreshold(m_nWhiteLvl, pDlg->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("���ô��׷�ֵʧ�ܣ�ͨ���쳣�򷵻ش���"), _T("���ô��׷�ֵ"), MB_OK|MB_ICONWARNING);
	}
}

// �����ںϲ���
void CMGblendsetDlg::SetAlphaGammaP(BOOL bDefault/* =FALSE */)
{
	if (bDefault)
	{
		//Ĭ���ںϲ���
		m_nAlpha = 5;
		m_nP = 20;
		m_nGamma = 20;

		UpdateData(FALSE);
	}

	CString str;
	str.Format(_T("%2.1f"), m_nAlpha*0.1);
	m_EditAlpha.SetWindowText(str);

	str.Format(_T("%2.1f"), m_nP*0.1);
	m_EditP.SetWindowText(str);

	str.Format(_T("%2.1f"), m_nGamma*0.1);
	m_EditGamma.SetWindowText(str);
}

void CMGblendsetDlg::TestAlphaGammaP()
{
	// ����
	BlendParams bdp;
	memset(&bdp, 0, sizeof(BlendParams));

	bdp.nBlendSel = m_nBlendSel;
	bdp.nPureWhite = m_nPureWhite;
	bdp.nAlpha = (int)(m_nAlpha*0.1*65535);
	bdp.nP     = (int)(m_nP*0.1*65535/16);
	bdp.nGamma = (int)(m_nGamma*0.1* 65535/16);

	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();
	if (theApp.MG_TestBlendParams(&bdp, pDlg->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("����ʧ�ܣ�ͨ���쳣�򷵻ش���"), _T("�����ں�Ч��"), MB_OK|MB_ICONWARNING);
	}
	else
	{
		//�����óɹ�����ںϲ���ֵ����
		m_stBlendSetting[m_nBlendSel].fAlpha[m_nPureWhite] = m_nAlpha*0.1;
		m_stBlendSetting[m_nBlendSel].fP[m_nPureWhite]     = m_nP*0.1;
		m_stBlendSetting[m_nBlendSel].fGamma[m_nPureWhite] = m_nGamma*0.1;
	}
}

void CMGblendsetDlg::SetAdvancedBlendParam()
{
	//�����߼��ںϲ����Ի���ʱ�����жϵ�ǰͨ����ǰѡ����ںϴ��Ƿ���Ч
	if (m_nBlendwidth[m_nBlendSel] > 1024 || m_nBlendwidth[m_nBlendSel] < 5)
	{
		return (void)AfxMessageBox(_T("�ںϴ�����쳣����Ч��ΧΪ5~1024��"));
	}

	// ���ض�ȡ�ں�ֵ
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();

	unsigned int nBlendPara[1024];
	memset(nBlendPara, 0, sizeof(nBlendPara));
	if (theApp.MG_ReadblendParamDatas(nBlendPara, m_nBlendSel, m_nPureWhite, m_nBlendwidth[m_nBlendSel], pDlg->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		return (void)MessageBox(_T("��ȡ�ں�����ʧ�ܣ�"), _T("��ȡ�ں�����"), MB_OK|MB_ICONSTOP);
	}

	CAdvancedBlendParamSetDlg dlg;
	dlg.m_nBlendWidth = m_nBlendwidth[m_nBlendSel];
	dlg.m_nPort       = pDlg->m_boardInfo.GetBoardPort();
	dlg.m_PureWhite   = m_nPureWhite;
	dlg.blend_alpha   = m_stBlendSetting[m_nBlendSel].fAlpha[m_nPureWhite];
	dlg.blend_p       = m_stBlendSetting[m_nBlendSel].fP[m_nPureWhite];
	dlg.blend_gamma   = m_stBlendSetting[m_nBlendSel].fGamma[m_nPureWhite];
	dlg.m_nBldSide    = m_nBlendSel;
	memcpy(dlg.m_nBlendVal, nBlendPara, 1024*sizeof(UINT));
	dlg.DoModal();
}

void CMGblendsetDlg::OnEnChangeEditAlpha()
{
	CString strAlpha;
	m_EditAlpha.GetWindowText(strAlpha);

	double fAlpha = atof(strAlpha);
	if (fAlpha < 0.0) { fAlpha = 0.0; strAlpha.Format(_T("%2.1f"), fAlpha); m_EditAlpha.SetWindowText(strAlpha); }
	if (fAlpha > 1.0) { fAlpha = 1.0; strAlpha.Format(_T("%2.1f"), fAlpha); m_EditAlpha.SetWindowText(strAlpha); }

	m_SliderAlpha.SetPos(int(fAlpha*10));
}

void CMGblendsetDlg::OnEnChangeEditP()
{
	CString strP;
	m_EditP.GetWindowText(strP);

	double fP = atof(strP);
	if (fP < 0.0)  { fP = 0.0;  strP.Format(_T("%2.1f"), fP); m_EditP.SetWindowText(strP); }
	if (fP > 16.0) { fP = 16.0; strP.Format(_T("%2.1f"), fP); m_EditP.SetWindowText(strP); }

	m_SliderP.SetPos(int(fP*10));
}

void CMGblendsetDlg::OnEnChangeEditGamma()
{
	CString strGamma;
	m_EditGamma.GetWindowText(strGamma);

	double fGamma = atof(strGamma);
	if (fGamma < 0.0)  { fGamma = 0.0;  strGamma.Format(_T("%2.1f"), fGamma); m_EditGamma.SetWindowText(strGamma); }
	if (fGamma > 16.0) { fGamma = 16.0; strGamma.Format(_T("%2.1f"), fGamma); m_EditGamma.SetWindowText(strGamma); }

	m_SliderGamma.SetPos(int(fGamma*10));
}

void CMGblendsetDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->IsKindOf(RUNTIME_CLASS(CSliderCtrl)))
	{
		UpdateData();
		SetAlphaGammaP(FALSE);
	}

	CXTPResizeDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CMGblendsetDlg::ValidateBlendWidth()
{
	if (m_CheckBlend[LEFT_BLEND].GetCheck())
	{
		if (m_nBlendwidth[LEFT_BLEND] < 0 || m_nBlendwidth[LEFT_BLEND] > 512)
		{
			m_EditBlend[LEFT_BLEND].SetFocus();
			m_EditBlend[LEFT_BLEND].SetSel(0, m_EditBlend[LEFT_BLEND].GetWindowTextLength());

			MessageBox(_T("������������ֵ����Ч��ΧΪ0-512��"), _T("��Ч���ںϴ����"), MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	if (m_CheckBlend[TOP_BLEND].GetCheck())
	{
		if (m_nBlendwidth[TOP_BLEND] < 0 || m_nBlendwidth[TOP_BLEND] > 512)
		{
			m_EditBlend[TOP_BLEND].SetFocus();
			m_EditBlend[TOP_BLEND].SetSel(0, m_EditBlend[TOP_BLEND].GetWindowTextLength());

			MessageBox(_T("������������ֵ����Ч��ΧΪ0-512��"), _T("��Ч���ںϴ����"), MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	if (m_CheckBlend[RIGHT_BLEND].GetCheck())
	{
		if (m_nBlendwidth[RIGHT_BLEND] < 0 || m_nBlendwidth[RIGHT_BLEND] > 512)
		{
			m_EditBlend[RIGHT_BLEND].SetFocus();
			m_EditBlend[RIGHT_BLEND].SetSel(0, m_EditBlend[RIGHT_BLEND].GetWindowTextLength());

			MessageBox(_T("������������ֵ����Ч��ΧΪ0-512��"), _T("��Ч���ںϴ����"), MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}
	if (m_CheckBlend[BOTTOM_BLEND].GetCheck())
	{
		if (m_nBlendwidth[BOTTOM_BLEND] < 0 || m_nBlendwidth[BOTTOM_BLEND] > 512)
		{
			m_EditBlend[BOTTOM_BLEND].SetFocus();
			m_EditBlend[BOTTOM_BLEND].SetSel(0, m_EditBlend[BOTTOM_BLEND].GetWindowTextLength());

			MessageBox(_T("������������ֵ����Ч��ΧΪ0-512��"), _T("��Ч���ںϴ����"), MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}

	return TRUE;
}

void CMGblendsetDlg::Initialize()
{
	int nPort = ((CMGeoMainDlg*)GetOwner())->m_boardInfo.GetBoardPort();
	if (nPort == 0xff) return;

	//��ȡ�ںϴ����
	UINT uParam[64] = {0};
	
	if(theApp.MG_GetBlendWidth(uParam, nPort) != CMD_FAILURE)
	{
		for (int i=0; i<4; i++)
			m_nBlendwidth[i] = m_stBlendSetting[i].nBlendWidth = uParam[i];
	}
	else
	{
		MessageBox(_T("��ȡ�ںϴ����ʧ�ܣ�"), _T("�ںϴ���ʼ��"), MB_OK|MB_ICONERROR);
		return; //��ֹ������ȡ�ںϴ�������ص�����
	}

	//��ȡ�ںϲ���
	memset(uParam, 0, sizeof(uParam));

	if (theApp.MG_ReadBlendParams(uParam, nPort) != CMD_FAILURE)
	{
		for (int i=0; i<4; i++)
		{
			//�Ǵ���
			m_stBlendSetting[i].fAlpha[0] = float( int( ((UINT)uParam[i*3+0]/65535.0*1000 +0.5)/1000.0*10 )/10.0 );
			m_stBlendSetting[i].fP[0]     = float( int( ((UINT)uParam[i*3+1]*16/65535.0*1000 +0.5)/1000.0*10 )/10.0 );
			m_stBlendSetting[i].fGamma[0] = float( int( ((UINT)uParam[i*3+2]*16/65535.0*1000 +0.5)/1000.0*10 )/10.0 );
			//����
			m_stBlendSetting[i].fAlpha[1] = float( int( ((UINT)uParam[12+i*3+0]/65535.0*1000 +0.5)/1000.0*10 )/10.0 );
			m_stBlendSetting[i].fP[1]     = float( int( ((UINT)uParam[12+i*3+1]*16/65535.0*1000 +0.5)/1000.0*10 )/10.0 );
			m_stBlendSetting[i].fGamma[1] = float( int( ((UINT)uParam[12+i*3+2]*16/65535.0*1000 +0.5)/1000.0*10 )/10.0 );
		}
	}
	else
	{
		MessageBox(_T("��ȡ�ںϴ�����ʧ�ܣ�"), _T("�ںϴ���ʼ��"), MB_OK|MB_ICONERROR);
		return;
	}

	//��ȡ���׷�ֵ
	memset(uParam, 0, sizeof(uParam));

	if (theApp.MG_GetWhiteThreshold(uParam[0], nPort) != CMD_FAILURE)
	{
		for (int i=0; i<4; i++) m_stBlendSetting[i].nWhiteLvl = uParam[0];
	}
	else
	{
		MessageBox(_T("��ȡ���׷�ֵʧ�ܣ�"), _T("�ںϴ���ʼ��"), MB_OK|MB_ICONERROR);
		return;
	}

	//��ȡ�ںϹ��ܿ������
	memset(uParam, 0, sizeof(uParam));

	if (theApp.MG_GetBlendEnable(uParam[0], nPort) != CMD_FAILURE)
	{
		m_nEnableBlending = uParam[0];
	}
	else
	{
		MessageBox(_T("��ȡ�ںϹ��ܿ�����־ʧ�ܣ�"), _T("�ںϴ���ʼ��"), MB_OK|MB_ICONERROR);
		return;
	}

	//��ʼ������
	int i;
	for (i=0; i<4; i++)
	{	
		if(m_stBlendSetting[i].nBlendWidth > 0) 
			m_nBlendSel = i;

		m_CheckBlend[i].SetCheck(m_stBlendSetting[i].nBlendWidth > 0);

		ShowHideBlendSelCtrls(i, m_stBlendSetting[i].nBlendWidth > 0 ? SW_SHOW : SW_HIDE);
	}

	m_nPureWhite = 0;
	m_nWhiteLvl = m_stBlendSetting[0].nWhiteLvl;

	m_nAlpha = int(m_stBlendSetting[m_nBlendSel].fAlpha[0]*10);
	m_nP = int(m_stBlendSetting[m_nBlendSel].fP[0]*10);
	m_nGamma = int(m_stBlendSetting[m_nBlendSel].fGamma[0]*10);

	SetAlphaGammaP(FALSE);

	UpdateData(FALSE);
}