// SourcePropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "SourcePropertyDlg.h"
#include "afxdialogex.h"


// CSourcePropertyDlg 对话框

IMPLEMENT_DYNAMIC(CSourcePropertyDlg, CDialog)

CSourcePropertyDlg::CSourcePropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSourcePropertyDlg::IDD, pParent)
	, m_nLightness(50)
	, m_nContrast(50)
	, m_nPhase(16)
	, m_nFreq(0)
	, m_nLeft(0)
	, m_nTop(0)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nRed(100)
	, m_nGreen(100)
	, m_nBlue(100)
	, m_nMidPosLeft(50)
	, m_nMidPosTop(50)
	, m_nMidPosWidth(50)
	, m_nMidPosHeight(50)
{
	m_nSourcePort = INVALID_VALUE;
	m_eSourceType = vtUndef;
}

CSourcePropertyDlg::~CSourcePropertyDlg()
{
}

void CSourcePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SRC_LIGHTNESS, m_sliderLightness);
	DDX_Control(pDX, IDC_SLIDER_SRC_CONTRAST, m_sliderContrast);
	DDX_Control(pDX, IDC_SLIDER_SRC_PHASE, m_sliderPhase);
	DDX_Control(pDX, IDC_SLIDER_SRC_FREQUENCY, m_sliderFreq);
	DDX_Control(pDX, IDC_SLIDER_SRC_SAMPLE_LEFT, m_sliderLeft);
	DDX_Control(pDX, IDC_SLIDER_SRC_SAMPLE_TOP, m_sliderTop);
	DDX_Control(pDX, IDC_SLIDER_SRC_SAMPLE_WIDTH, m_sliderWidth);
	DDX_Control(pDX, IDC_SLIDER_SRC_SAMPLE_HEIGHT, m_sliderHeight);
	DDX_Control(pDX, IDC_SLIDER_SRC_COLOR_R, m_sliderR);
	DDX_Control(pDX, IDC_SLIDER_SRC_COLOR_G, m_sliderG);
	DDX_Control(pDX, IDC_SLIDER_SRC_COLOR_B, m_sliderB);
	DDX_Text(pDX, IDC_EDIT_SRC_LIGHTNESS, m_nLightness);
	DDX_Text(pDX, IDC_EDIT_SRC_CONTRAST, m_nContrast);
	DDX_Text(pDX, IDC_EDIT_SRC_PHASE, m_nPhase);
	DDX_Text(pDX, IDC_EDIT_SRC_FREQUENCY, m_nFreq);
	DDX_Text(pDX, IDC_EDIT_SRC_SAMPLE_LEFT, m_nLeft);
	DDX_Text(pDX, IDC_EDIT_SRC_SAMPLE_TOP, m_nTop);
	DDX_Text(pDX, IDC_EDIT_SRC_SAMPLE_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_SRC_SAMPLE_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_SRC_COLOR_R, m_nRed);
	DDX_Text(pDX, IDC_EDIT_SRC_COLOR_G, m_nGreen);
	DDX_Text(pDX, IDC_EDIT_SRC_COLOR_B, m_nBlue);
	DDV_MinMaxInt(pDX, m_nLightness, 0, 100);
	DDV_MinMaxInt(pDX, m_nContrast, 0, 100);
	DDV_MinMaxInt(pDX, m_nRed, 0, 255);
	DDV_MinMaxInt(pDX, m_nGreen, 0, 255);
	DDV_MinMaxInt(pDX, m_nBlue, 0, 255);
	DDX_Control(pDX, IDC_CBOX_COLOR_SPACE_MODE, m_cboxColorSpace);
}


BEGIN_MESSAGE_MAP(CSourcePropertyDlg, CDialog)
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_DEFAULT1, &CSourcePropertyDlg::OnBtnDefaultBrightnessContrast)
	ON_BN_CLICKED(IDC_BTN_DETECT1, &CSourcePropertyDlg::OnBtnDetectBrightnessContrast)
	ON_BN_CLICKED(IDC_BTN_APPLY1, &CSourcePropertyDlg::OnBtnApplyBrightnessContrast)
	ON_BN_CLICKED(IDC_BTN_DEFAULT2, &CSourcePropertyDlg::OnBtnDefaultSample)
	ON_BN_CLICKED(IDC_BTN_DETECT_SAMPLE, &CSourcePropertyDlg::OnBtnDetectSample)
	ON_BN_CLICKED(IDC_BTN_AUTO_SAMPLE, &CSourcePropertyDlg::OnBtnAutoSample)
	ON_BN_CLICKED(IDC_BTN_APPLY_SAMPLE, &CSourcePropertyDlg::OnBtnApplySample)
	ON_BN_CLICKED(IDC_BTN_SET_COLORSPACE, &CSourcePropertyDlg::OnBtnSetColorspace)
	ON_MESSAGE(WM_USER_SLIDER_LBTNUP, &CSourcePropertyDlg::OnMsgSliderLBtnUp)
END_MESSAGE_MAP()


// CSourcePropertyDlg 消息处理程序


BOOL CSourcePropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	CString strTitle;
	strTitle.Format(_T("信号属性 - %s"), GetInputManager()->GetName(GetInputManager()->FindCmdPort(m_nSourcePort), FALSE));
	SetWindowText(strTitle);

	InitSlider();

	m_cboxColorSpace.SetCurSel(0);

	EnableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSourcePropertyDlg::EnableControls()
{
	m_sliderLightness.EnableWindow(m_eSourceType==vtCVBS);
	m_sliderContrast.EnableWindow(m_eSourceType==vtCVBS);
	GetDlgItem(IDC_EDIT_SRC_LIGHTNESS)->EnableWindow(m_eSourceType==vtCVBS);
	GetDlgItem(IDC_EDIT_SRC_CONTRAST)->EnableWindow(m_eSourceType==vtCVBS);
	GetDlgItem(IDC_BTN_DEFAULT1)->EnableWindow(m_eSourceType==vtCVBS);
	GetDlgItem(IDC_BTN_DETECT1)->EnableWindow(m_eSourceType==vtCVBS);
	GetDlgItem(IDC_BTN_APPLY1)->EnableWindow(m_eSourceType==vtCVBS);
}

void CSourcePropertyDlg::InitSlider()
{
	m_sliderLightness.SetBuddy(GetDlgItem(IDC_EDIT_SRC_LIGHTNESS), FALSE);
	m_sliderContrast.SetBuddy(GetDlgItem(IDC_EDIT_SRC_CONTRAST), FALSE);
	m_sliderPhase.SetBuddy(GetDlgItem(IDC_EDIT_SRC_PHASE), FALSE);
	m_sliderFreq.SetBuddy(GetDlgItem(IDC_EDIT_SRC_FREQUENCY), FALSE);
	m_sliderLeft.SetBuddy(GetDlgItem(IDC_EDIT_SRC_SAMPLE_LEFT), FALSE);
	m_sliderTop.SetBuddy(GetDlgItem(IDC_EDIT_SRC_SAMPLE_TOP), FALSE);
	m_sliderWidth.SetBuddy(GetDlgItem(IDC_EDIT_SRC_SAMPLE_WIDTH), FALSE);
	m_sliderHeight.SetBuddy(GetDlgItem(IDC_EDIT_SRC_SAMPLE_HEIGHT), FALSE);
	m_sliderR.SetBuddy(GetDlgItem(IDC_EDIT_SRC_COLOR_R), FALSE);
	m_sliderG.SetBuddy(GetDlgItem(IDC_EDIT_SRC_COLOR_G), FALSE);
	m_sliderB.SetBuddy(GetDlgItem(IDC_EDIT_SRC_COLOR_B), FALSE);

	m_sliderLightness.SetRange(0, 100);
	m_sliderLightness.SetTic(5);
	m_sliderLightness.SetTicFreq(5);
	m_sliderLightness.SetPos(m_nLightness);

	m_sliderContrast.SetRange(0, 100);
	m_sliderContrast.SetTic(5);
	m_sliderContrast.SetTicFreq(5);
	m_sliderContrast.SetPos(m_nContrast);

	m_sliderPhase.SetRange(0, 32);
	m_sliderPhase.SetTic(1);
	m_sliderPhase.SetTicFreq(1);
	m_sliderPhase.SetPos(m_nPhase);

	m_sliderFreq.SetRange(-20, 20);
	m_sliderFreq.SetTic(1);
	m_sliderFreq.SetTicFreq(1);
	m_sliderFreq.SetPos(m_nFreq);

	m_sliderLeft.SetRange(0, 100);
	m_sliderLeft.SetTic(5);
	m_sliderLeft.SetTicFreq(5);
	m_sliderLeft.SetPos(m_nLeft);

	m_sliderTop.SetRange(0, 100);
	m_sliderTop.SetTic(5);
	m_sliderTop.SetTicFreq(5);
	m_sliderTop.SetPos(m_nTop);

	m_sliderWidth.SetRange(0, 100);
	m_sliderWidth.SetTic(5);
	m_sliderWidth.SetTicFreq(5);
	m_sliderWidth.SetPos(m_nWidth);

	m_sliderHeight.SetRange(0, 100);
	m_sliderHeight.SetTic(5);
	m_sliderHeight.SetTicFreq(5);
	m_sliderHeight.SetPos(m_nHeight);

	m_sliderR.SetRange(0, 255);
	m_sliderR.SetTic(10);
	m_sliderR.SetTicFreq(10);
	m_sliderR.SetPos(m_nRed);

	m_sliderG.SetRange(0, 255);
	m_sliderG.SetTic(10);
	m_sliderG.SetTicFreq(10);
	m_sliderG.SetPos(m_nGreen);

	m_sliderB.SetRange(0, 255);
	m_sliderB.SetTic(10);
	m_sliderB.SetTicFreq(10);
	m_sliderB.SetPos(m_nBlue);
}

void CSourcePropertyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
	int nTickPos = pSlider->GetPos();
	if (*pSlider == m_sliderLightness)
	{
		m_nLightness = nTickPos;
	}
	else if (*pSlider == m_sliderContrast)
	{
		m_nContrast = nTickPos;
	}
	else if (*pSlider == m_sliderPhase)
	{
		m_nPhase = nTickPos;
	}
	else if (*pSlider == m_sliderFreq)
	{
		m_nFreq = nTickPos;
	}
	else if (*pSlider == m_sliderLeft)
	{
		m_nLeft = nTickPos;
		m_nLeft = m_nLeft < 0 ? 0 : m_nLeft;

		if (m_nLeft == m_nMidPosLeft-50 || m_nLeft == m_nMidPosLeft+50) {
			ReleaseCapture();
			m_nMidPosLeft = m_nLeft;
			m_sliderLeft.SetRange(m_nMidPosLeft-50, m_nMidPosLeft+50);
			m_sliderLeft.SetPos(m_nLeft);
		}
	}
	else if (*pSlider == m_sliderTop)
	{
		m_nTop = nTickPos;
		m_nTop = m_nTop < 0 ? 0 : m_nTop;

		if (m_nTop == m_nMidPosTop-50 || m_nTop == m_nMidPosTop+50)
		{
			ReleaseCapture();
			m_nMidPosTop = m_nTop;
			m_sliderTop.SetRange(m_nMidPosTop-50, m_nMidPosTop+50);
			m_sliderTop.SetPos(m_nTop);
		}
	}
	else if (*pSlider == m_sliderWidth)
	{
		m_nWidth = nTickPos;
		m_nWidth = m_nWidth < 0 ? 0 : m_nWidth;

		if (m_nWidth == m_nMidPosWidth-50 || m_nWidth == m_nMidPosWidth+50)
		{
			ReleaseCapture();
			m_nMidPosWidth = m_nWidth;
			m_sliderWidth.SetRange(m_nMidPosWidth-50, m_nMidPosWidth+50);
			m_sliderWidth.SetPos(m_nWidth);
		}
	}
	else if (*pSlider == m_sliderHeight)
	{
		m_nHeight = nTickPos;
		m_nHeight = m_nHeight < 0 ? 0 : m_nHeight;

		if (m_nHeight == m_nMidPosHeight-50 || m_nHeight == m_nMidPosHeight+50)
		{
			ReleaseCapture();
			m_nMidPosHeight = m_nHeight;
			m_sliderHeight.SetRange(m_nMidPosHeight-50, m_nMidPosHeight+50);
			m_sliderHeight.SetPos(m_nHeight);
		}
	}
	else if (*pSlider == m_sliderR)
	{
		m_nRed = nTickPos;
	}
	else if (*pSlider == m_sliderG)
	{
		m_nGreen = nTickPos;
	}
	else if (*pSlider == m_sliderB)
	{
		m_nBlue = nTickPos;
	}

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSourcePropertyDlg::OnBtnDefaultBrightnessContrast()
{
	UpdateData();

	if (GetCmdManager()->CmdRST2(m_nSourcePort, eCardTypeInput, m_nLightness, m_nContrast) == CMD_FAILURE)
	{
		MessageBox(_T("读取亮度/对比度失败！"), _T("默认亮度/对比度"), MB_OK|MB_ICONERROR);
		return;
	}

	m_nLightness = m_nLightness>100 ? 100 : m_nLightness<0 ? 0 : m_nLightness;
	m_nContrast  = m_nContrast >100 ? 100 : m_nContrast<0 ? 0 : m_nContrast;

	m_sliderLightness.SetPos(m_nLightness);
	m_sliderContrast.SetPos(m_nContrast);

	UpdateData(FALSE);
}

void CSourcePropertyDlg::OnBtnDetectBrightnessContrast()
{
	UpdateData();

	if (GetCmdManager()->CmdRPM2(m_nSourcePort, eCardTypeInput, m_nLightness, m_nContrast) == CMD_FAILURE)
	{
		MessageBox(_T("读取亮度/对比度失败！"), _T("默认亮度/对比度"), MB_OK|MB_ICONERROR);
		return;
	}

	m_nLightness = m_nLightness>100 ? 100 : m_nLightness<0 ? 0 : m_nLightness;
	m_nContrast  = m_nContrast >100 ? 100 : m_nContrast<0 ? 0 : m_nContrast;

	m_sliderLightness.SetPos(m_nLightness);
	m_sliderContrast.SetPos(m_nContrast);

	UpdateData(FALSE);
}

void CSourcePropertyDlg::OnBtnApplyBrightnessContrast()
{
	UpdateData();

	if (GetCmdManager()->CmdWPM2(m_nSourcePort, eCardTypeInput, m_nLightness, m_nContrast) == CMD_FAILURE)
	{
		MessageBox(_T("设置亮度/对比度失败！"), _T("默认亮度/对比度"), MB_OK|MB_ICONERROR);
		return;
	}

	m_sliderLightness.SetPos(m_nLightness);
	m_sliderContrast.SetPos(m_nContrast);
}

void CSourcePropertyDlg::OnBtnDefaultSample()
{
	UpdateData();

	if (GetCmdManager()->CmdRST1(m_nSourcePort, eCardTypeInput, m_nLeft, m_nTop, m_nWidth, m_nHeight) == CMD_FAILURE)
	{
		MessageBox(_T("读取采样属性失败！"), _T("默认采样属性"), MB_OK|MB_ICONERROR);
		return;
	}

	UpdateSlider();

	UpdateData(FALSE);
}

void CSourcePropertyDlg::OnBtnAutoSample()
{
	UpdateData();

	if (GetCmdManager()->CmdATO1(m_nSourcePort, eCardTypeInput, m_nLeft, m_nTop, m_nWidth, m_nHeight) == CMD_FAILURE)
	{
		MessageBox(_T("读取采样属性失败！"), _T("自动采样属性"), MB_OK|MB_ICONERROR);
		return;
	}

	UpdateSlider();

	UpdateData(FALSE);
}

void CSourcePropertyDlg::OnBtnDetectSample()
{
	UpdateData();

	if (GetCmdManager()->CmdRPM1(m_nSourcePort, eCardTypeInput, m_nLeft, m_nTop, m_nWidth, m_nHeight) == CMD_FAILURE)
	{
		MessageBox(_T("读取采样属性失败！"), _T("获取采样属性"), MB_OK|MB_ICONERROR);
		return;
	}

	UpdateSlider();

	UpdateData(FALSE);
}

void CSourcePropertyDlg::OnBtnApplySample()
{
	UpdateData();

	if (GetCmdManager()->CmdWPM1(m_nSourcePort, eCardTypeInput, m_nLeft, m_nTop, m_nWidth, m_nHeight) == CMD_FAILURE)
	{
		MessageBox(_T("设置采样属性失败！"), _T("设置采样属性"), MB_OK|MB_ICONERROR);
		return;
	}

	UpdateSlider();
}

void CSourcePropertyDlg::OnBtnSetColorspace()
{
	UpdateData();

	//0:RGB(16-235); 1:RGB(0-255); 2:YPbPr601; 3:YPbPr701; 4:XVYC601; 5:XVYC701; 6:Auto; 7:Default;
	int nMode = m_cboxColorSpace.GetCurSel();
	if (nMode == 6)
		nMode = 7;   //auto
	else if(nMode == 7)
		nMode = 0xff;  //default

	if (GetCmdManager()->CmdWICS(m_nSourcePort, eCardTypeInput, nMode) == CMD_FAILURE)
	{
		MessageBox(_T("设置色彩模式失败！"), _T("色彩空间转换"), MB_OK|MB_ICONERROR);
		return;
	}
}

void CSourcePropertyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	Initialize();
}

void CSourcePropertyDlg::Initialize()
{
	if (m_eSourceType == vtCVBS)
		OnBtnDetectBrightnessContrast();

	OnBtnDetectSample();
}
BOOL CSourcePropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{

			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CSourcePropertyDlg::OnMsgSliderLBtnUp(WPARAM wParam, LPARAM lParam)
{
	CMySliderCtrl* pCtrl = (CMySliderCtrl*)wParam;
	OnSliderLBtnUp(pCtrl);

	return 0;
}

void CSourcePropertyDlg::OnSliderLBtnUp(CMySliderCtrl* pSlider)
{
	UpdateData();

	if (pSlider == &m_sliderLightness
	 || pSlider == &m_sliderContrast)
	{
		GetCmdManager()->CmdWPM2(m_nSourcePort, eCardTypeInput, m_nLightness, m_nContrast);
	}
	else if (pSlider == &m_sliderContrast
		  || pSlider == &m_sliderLeft
		  || pSlider == &m_sliderTop
		  || pSlider == &m_sliderWidth
		  || pSlider == &m_sliderHeight)
	{
		GetCmdManager()->CmdWPM1(m_nSourcePort, eCardTypeInput, m_nLeft, m_nTop, m_nWidth, m_nHeight);
	}
}

void CSourcePropertyDlg::UpdateSlider()
{
	m_nMidPosLeft   = m_nLeft;
	m_nMidPosTop    = m_nTop;
	m_nMidPosWidth  = m_nWidth;
	m_nMidPosHeight = m_nHeight;

	m_sliderLeft.SetRange(m_nLeft-50, m_nLeft+50);
	m_sliderTop.SetRange(m_nTop-50, m_nTop+50);
	m_sliderWidth.SetRange(m_nWidth-50, m_nWidth+50);
	m_sliderHeight.SetRange(m_nHeight-50, m_nHeight+50);

	m_sliderLeft.SetPos(m_nLeft);
	m_sliderTop.SetPos(m_nTop);
	m_sliderWidth.SetPos(m_nWidth);
	m_sliderHeight.SetPos(m_nHeight);
}