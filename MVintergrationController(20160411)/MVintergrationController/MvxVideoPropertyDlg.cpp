// MvxVideoPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MvxVideoPropertyDlg.h"
#include "MvxSplitDlg.h"


// CMvxVideoPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CMvxVideoPropertyDlg, CDialog)

CMvxVideoPropertyDlg::CMvxVideoPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMvxVideoPropertyDlg::IDD, pParent)
	, m_nHActive(0)
	, m_nHTotal(0)
	, m_nHFront(0)
	, m_nHSync(0)
	, m_nHPol(0)
	, m_nVActive(0)
	, m_nVFront(0)
	, m_nVSync(0)
	, m_nVTotal(0)
	, m_nVPol(0)
	, m_nHz(0)
{
	m_pstrTiming = NULL;
	m_pDetailedTiming = NULL;
	m_nTotalTiming = 0;

	memset(&m_SelectTiming, 0, sizeof(TIMING_STRUCT));
}

CMvxVideoPropertyDlg::~CMvxVideoPropertyDlg()
{
}

void CMvxVideoPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_MVX_VIDEO_TIMING, m_CboxOutputTiming);
	DDX_Control(pDX, IDC_CBOX_MVX_VIDEO_HPOL, m_CboxHPol);
	DDX_Control(pDX, IDC_CBOX_MVX_VIDEO_VPOL, m_CboxVPol);
	DDX_Control(pDX, IDC_BTN_MVX_VIDEO_SETTIMING, m_BtnSetTiming);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BTN_MVX_VIDEO_CUSTOM, m_BtnCustom);
	DDX_Control(pDX, IDC_SPIN_MVX_VIDEO_HZ, m_SpinHz);

	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_HACTIVE, m_nHActive);
	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_HFRONT, m_nHFront);
	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_HSYNC, m_nHSync);
	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_HTOTAL, m_nHTotal);
	DDX_CBIndex(pDX, IDC_CBOX_MVX_VIDEO_HPOL, m_nHPol);

	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_VACTIVE, m_nVActive);
	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_VFRONT, m_nVFront);
	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_VSYNC, m_nVSync);
	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_VTOTAL, m_nVTotal);
	DDX_CBIndex(pDX, IDC_CBOX_MVX_VIDEO_VPOL, m_nVPol);

	DDX_Text(pDX, IDC_EDIT_MVX_VIDEO_HZ, m_nHz);
}


BEGIN_MESSAGE_MAP(CMvxVideoPropertyDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CBOX_MVX_VIDEO_TIMING, OnCbnSelChangeTiming)
	ON_EN_CHANGE(IDC_EDIT_MVX_VIDEO_HTOTAL, OnEnChangedEditHTotal)
	ON_EN_CHANGE(IDC_EDIT_MVX_VIDEO_VTOTAL, OnEnChangedEditVTotal)
	ON_EN_CHANGE(IDC_EDIT_MVX_VIDEO_HZ, OnEnChangedEditHz)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MVX_VIDEO_HZ, OnDeltaPosSpinHZ)
	ON_BN_CLICKED(IDC_BTN_MVX_VIDEO_SETTIMING, OnBtnClickedSetOutTiming)
	ON_BN_CLICKED(IDC_BTN_MVX_VIDEO_CUSTOM, &CMvxVideoPropertyDlg::OnBnClickedBtnMvxVideoCustom)
END_MESSAGE_MAP()


// CMvxVideoPropertyDlg 消息处理程序

HBRUSH CMvxVideoPropertyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return theApp.m_DlgSetting.bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(theApp.m_DlgSetting.clrText);

		return theApp.m_DlgSetting.bkBrush;
	}
	
	return hbr;
}

BOOL CMvxVideoPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SpinHz.SetBuddy(GetDlgItem(IDC_EDIT_MVX_VIDEO_HZ));

	SetThemes(xtpControlThemeResource);

	AddTimings();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMvxVideoPropertyDlg::SetThemes(XTPControlTheme theme)
{
	m_CboxOutputTiming.SetTheme(theme);
	m_CboxHPol.SetTheme(theme);
	m_CboxVPol.SetTheme(theme);

	m_BtnCustom.SetTheme(theme);
	m_BtnCancel.SetTheme(theme);
	m_BtnSetTiming.SetTheme(theme);

	m_SpinHz.SetTheme(theme);
}

void CMvxVideoPropertyDlg::AddTimings()
{
	CMvxSplitDlg *pParent = (CMvxSplitDlg*)m_pParentWnd;

	m_nTotalTiming = pParent->m_boardInfo.GetTotalTimings(Product_MVX);
	m_pstrTiming   = pParent->m_boardInfo.GetTimingName(Product_MVX);
	m_pDetailedTiming = pParent->m_boardInfo.GetDetailTiming(Product_MVX);

	m_CboxOutputTiming.ResetContent();

	for (int i=0; i<m_nTotalTiming; i++)
	{
		if (m_pstrTiming[i].Find(_T("1x1")) != -1)
			m_CboxOutputTiming.AddString(m_pstrTiming[i]);
	}
}

void CMvxVideoPropertyDlg::OnCbnSelChangeTiming()
{
	int nSel = m_CboxOutputTiming.GetCurSel();
	if (nSel == CB_ERR ) return;

	CString str;
	m_CboxOutputTiming.GetWindowText(str);

	int nIndex = -1;
	for (int i=0; i<m_nTotalTiming; i++)
	{
		if (m_pstrTiming[i] == str)
		{
			nIndex = i;
			break;
		}
	}

	if (nIndex != -1) m_SelectTiming = m_pDetailedTiming[nIndex];

	ShowDetailedTiming();
}

void CMvxVideoPropertyDlg::ShowDetailedTiming()
{
	m_nHActive = m_SelectTiming.m_HActive;
	m_nHFront  = m_SelectTiming.m_HFPorch;
	m_nHSync   = m_SelectTiming.m_HSyncWid;
	m_nHTotal  = m_SelectTiming.m_HActive +  m_SelectTiming.m_HBlank;
	m_nHPol    = m_SelectTiming.m_HPol;

	m_nVActive = m_SelectTiming.m_VActive;
	m_nVFront  = m_SelectTiming.m_VFPorch;
	m_nVSync   = m_SelectTiming.m_VSyncWid;
	m_nVTotal  = m_SelectTiming.m_VActive + m_SelectTiming.m_VBlank;
	m_nVPol    = m_SelectTiming.m_VPol;

	m_nHz      = m_SelectTiming.m_Hz;

	UpdatePixelClock();

	UpdateData(FALSE);
}

void CMvxVideoPropertyDlg::OnEnChangedEditHTotal()
{
	UpdateData();

	UpdatePixelClock();
}

void CMvxVideoPropertyDlg::OnEnChangedEditVTotal()
{
	UpdateData();

	UpdatePixelClock();
}

void CMvxVideoPropertyDlg::OnEnChangedEditHz()
{
	UpdateData();

	if (m_nHz < 25) {m_nHz = 25; UpdateData(FALSE);}
	if (m_nHz > 120) {m_nHz = 120; UpdateData(FALSE);}

	UpdatePixelClock();
}

void CMvxVideoPropertyDlg::UpdatePixelClock()
{
	CString strPix;
	strPix.Format(_T("%3.4f MHz"), m_nHTotal*m_nVTotal*m_nHz*0.000001);
	GetDlgItem(IDC_TEXT_MVX_VIDEO_PIXCLK)->SetWindowText(strPix);
}

void CMvxVideoPropertyDlg::OnDeltaPosSpinHZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nVal = m_nHz - pNMUpDown->iDelta;

	if (nVal < 25) nVal = 25;
	if (nVal > 120) nVal = 120;

	m_nHz = nVal;

	UpdateData(FALSE);
	UpdatePixelClock();
}

void CMvxVideoPropertyDlg::OnBtnClickedSetOutTiming()
{
	UpdateData();

	int nPort = ((CMvxSplitDlg*)m_pParentWnd)->m_boardInfo.GetBoardPort();
	if (theApp.SetOutputTiming(&m_SelectTiming, nPort) != CMD_FAILURE)
	{
		MessageBox(_T("设置输出分辨率成功！"), _T("设置输出分辨率"), MB_OK|MB_ICONINFORMATION);
	}
	else
		MessageBox(_T("错误！设置输出分辨率失败！"), _T("设置输出分辨率"), MB_OK|MB_ICONERROR);
}

void CMvxVideoPropertyDlg::OnBnClickedBtnMvxVideoCustom()
{
	// TODO: 在此添加控件通知处理程序代码
}
