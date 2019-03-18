// MGcolorsetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGcolorsetDlg.h"
#include "MGeoMainDlg.h"


// CMGcolorsetDlg 对话框

IMPLEMENT_DYNAMIC(CMGcolorsetDlg, CXTPResizeDialog)

CMGcolorsetDlg::CMGcolorsetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGcolorsetDlg::IDD, pParent)
	, m_nBlendOpt(0)
{
	m_pColorView = NULL;
}

CMGcolorsetDlg::~CMGcolorsetDlg()
{
}

void CMGcolorsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GBOX_TESTMODE, m_GboxTestMode);
	DDX_Control(pDX, IDC_RADIO_SOURCETEST, m_RadioSrcTest);
	DDX_Control(pDX, IDC_RADIO_IMAGETEST, m_RadioImgTest);

	DDX_Control(pDX, IDC_GBOX_BLENDOPT, m_GboxBlendOpt);
	DDX_Control(pDX, IDC_RADIO_CANCELBLEND, m_RadioCancelB);
	DDX_Control(pDX, IDC_RADIO_BLACKBLEND, m_RadioBlackB);
	DDX_Control(pDX, IDC_RADIO_RESUMEBLEND, m_RadioResumeB);

	DDX_Control(pDX, IDC_GBOX_COLOR, m_GboxColorSel);
	DDX_Control(pDX, IDC_RADIO_RED, m_RadioRed);
	DDX_Control(pDX, IDC_RADIO_GREEN, m_RadioGreen);
	DDX_Control(pDX, IDC_RADIO_BLUE, m_RadioBlue);
	DDX_Control(pDX, IDC_RADIO_WHITE, m_RadioWhite);

	DDX_Control(pDX, IDC_GBOX_AJUSTMETHOD, m_GboxMethod);
	DDX_Control(pDX, IDC_RADIO_LINEAR, m_RadioLinear);
	DDX_Control(pDX, IDC_RADIO_REGION, m_RadioRegion);

	DDX_Control(pDX, IDC_BTN_COLOR_RESET, m_btnClrReset);
}


BEGIN_MESSAGE_MAP(CMGcolorsetDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND_RANGE(IDC_RADIO_SOURCETEST, IDC_RADIO_IMAGETEST, OnRadioTestType)
	ON_COMMAND_RANGE(IDC_RADIO_CANCELBLEND, IDC_RADIO_RESUMEBLEND, OnRadioBlendOpt)
	ON_COMMAND_RANGE(IDC_RADIO_RED, IDC_RADIO_WHITE, OnRadioColorSel)
	ON_COMMAND_RANGE(IDC_RADIO_LINEAR, IDC_RADIO_REGION, OnRadioAjustMethod)
	ON_BN_CLICKED(IDC_BTN_COLOR_RESET, OnBnClickedReset)
END_MESSAGE_MAP()


// CMGcolorsetDlg 消息处理程序

BOOL CMGcolorsetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	m_GboxBlendOpt.SetBorderStyle(xtpGroupBoxSingleLine);
	m_GboxTestMode.SetBorderStyle(xtpGroupBoxSingleLine);
	m_GboxColorSel.SetBorderStyle(xtpGroupBoxSingleLine);
	m_GboxMethod.SetBorderStyle(xtpGroupBoxSingleLine);

	ResizeIt();
	SetControlTheme(xtpControlThemeResource);

	m_RadioRed.SetCheck(1);
	m_RadioSrcTest.SetCheck(1);
	m_RadioLinear.SetCheck(1);

	CreateView();

	//创建背景画刷
	m_redBrush.CreateSolidBrush(RGB(255,40,40));
	m_greenBrush.CreateSolidBrush(RGB(40,200,40));
	m_blueBrush.CreateSolidBrush(RGB(100,100,255));
	//m_whiteBrush.CreateSolidBrush(RGB(240,220,230));
	m_whiteBrush.CreateSolidBrush(RGB(250,175,170));
	m_bkBrush.CreateSolidBrush(constClrDlgBk[2]);

	return TRUE; 
}

void CMGcolorsetDlg::ResizeIt()
{
	SetResize(IDC_PIC_COLORVIEW, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	SetResize(IDC_GBOX_BLENDOPT, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_CANCELBLEND, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_BLACKBLEND, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_RESUMEBLEND, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);

	SetResize(IDC_GBOX_TESTMODE, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_SOURCETEST, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_IMAGETEST, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	
	SetResize(IDC_GBOX_COLOR, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_RED, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_GREEN, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_BLUE, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_WHITE, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);

	SetResize(IDC_GBOX_AJUSTMETHOD, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_LINEAR, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_RADIO_REGION, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);

	SetResize(IDC_BTN_COLOR_RESET, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
}

void CMGcolorsetDlg::SetControlTheme(XTPControlTheme theme)
{
	m_btnClrReset.SetTheme(theme);
}

void CMGcolorsetDlg::CreateView()
{
	CRect viewRect;
	GetDlgItem(IDC_PIC_COLORVIEW)->GetWindowRect(viewRect);
	ScreenToClient(&viewRect);

	CWnd *pFrameWnd = this;
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CcolorsetView);

	m_pColorView = (CcolorsetView *)((CFrameWnd*)pFrameWnd)->CreateView(&context);
	ASSERT(m_pColorView);

	m_pColorView->MoveWindow(viewRect, FALSE);
}

HBRUSH CMGcolorsetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_BTN)
	{
		if      (pWnd == GetDlgItem(IDC_RADIO_RED))   return m_redBrush;
		else if (pWnd == GetDlgItem(IDC_RADIO_GREEN)) return m_greenBrush;
		else if (pWnd == GetDlgItem(IDC_RADIO_BLUE))  return m_blueBrush;
		else if (pWnd == GetDlgItem(IDC_RADIO_WHITE)) return m_whiteBrush;
		else return m_bkBrush;
	}

	return hbr;
}

void CMGcolorsetDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if (m_pColorView != NULL)
	{
		CRect rc;
		GetDlgItem(IDC_PIC_COLORVIEW)->GetWindowRect(&rc);
		ScreenToClient(&rc);

		m_pColorView->MoveWindow(rc);

		RedrawWindow();
	}
}

BOOL CMGcolorsetDlg::OnEraseBkgnd(CDC* pDC)
{
	return CXTPResizeDialog::OnEraseBkgnd(pDC);
}


void CMGcolorsetDlg::OnRadioTestType(UINT nID)
{
	if (m_pColorView == NULL) return;

	if (nID == IDC_RADIO_SOURCETEST)
	{
		if (m_pColorView->GetTestType() == 0) return;
		m_pColorView->SetTestType(0);
	}
	else if (nID == IDC_RADIO_IMAGETEST)
	{
		if (m_pColorView->GetTestType() == 1) return;
		m_pColorView->SetTestType(1);
	}

	m_pColorView->TestColor();
}

void CMGcolorsetDlg::OnRadioBlendOpt(UINT nID)
{
	if (nID == IDC_RADIO_CANCELBLEND)
	{
		m_nBlendOpt = 2;
	}
	else if (nID == IDC_RADIO_BLACKBLEND)
	{
		m_nBlendOpt = 1;
	}
	else if (nID == IDC_RADIO_RESUMEBLEND)
	{
		m_nBlendOpt = 0;
	}

	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();

	if (theApp.MG_CancelBlending(m_nBlendOpt, pDlg->m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("对不起，设置失败！"), _T("设置融合带影响"), MB_OK|MB_ICONERROR);
	}
}

void CMGcolorsetDlg::OnRadioColorSel(UINT nID)
{
	if (m_pColorView == NULL) return;

	if (nID == IDC_RADIO_RED)
		m_pColorView->ExchangeColor(eRed);
	else if (nID == IDC_RADIO_GREEN)
		m_pColorView->ExchangeColor(eGreen);
	else if (nID == IDC_RADIO_BLUE)
		m_pColorView->ExchangeColor(eBlue);
	else if (nID == IDC_RADIO_WHITE)
		m_pColorView->ExchangeColor(eWhite);

	m_pColorView->Invalidate();
}

void CMGcolorsetDlg::OnRadioAjustMethod(UINT nID)
{
	if (m_pColorView == NULL) return;

	if (nID == IDC_RADIO_LINEAR)
		m_pColorView->SetAjustMethod(eLinearMode);
	else if (nID == IDC_RADIO_REGION)
		m_pColorView->SetAjustMethod(eRegionMode);

	m_pColorView->Invalidate();
}

void CMGcolorsetDlg::OnBnClickedReset()
{
	if (m_pColorView == NULL) return;

	m_pColorView->ResetColor();
}

BOOL CMGcolorsetDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			//防止ESC和ENTER键关闭对话框
			return TRUE;
		}

		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{	
			//调整颜色
			if (m_pColorView) m_pColorView->OnVKeyUpDown(pMsg->wParam);

			return TRUE;
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

void CMGcolorsetDlg::Initialize()
{
	UCHAR data[800] = {0};
	CMGeoMainDlg* pOwner = (CMGeoMainDlg*)GetOwner();
	int nPort = pOwner->m_boardInfo.GetBoardPort();

	int uColor[256] = {0};
	if (theApp.MG_ReadColor(data, 0, nPort) != CMD_FAILURE)
	{
		for(int i=0; i<3; i++)
		{
			for (int j=0; j<256; ++j) uColor[j] = data[i*256+j];

			SetColor(uColor, i);
		}
	}

	//初始化纯白为三原色的和除以3
	SetColor(NULL, 3);

	if (m_pColorView)
		m_pColorView->Invalidate();	
}

void CMGcolorsetDlg::SetColor(int *data, int nColorSel)
{
	if (m_pColorView)
		m_pColorView->SetColor(data, nColorSel);
}