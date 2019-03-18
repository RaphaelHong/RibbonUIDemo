// ResolutionEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "ResolutionEditDlg.h"
#include "OutputConfigDlg.h"
#include "InputConfigDlg.h"


// CResolutionEditDlg 对话框

IMPLEMENT_DYNAMIC(CResolutionEditDlg, CDialog)

CResolutionEditDlg::CResolutionEditDlg(CWnd* pParent /*=NULL*/)
: CDialog(CResolutionEditDlg::IDD, pParent)
{
	m_bOrg = TRUE;
}

CResolutionEditDlg::CResolutionEditDlg(const TIMING &Timing, int nMode, CWnd* pParent /*=NULL*/)
: CDialog(CResolutionEditDlg::IDD, pParent)
{
	m_bOrg = TRUE;

	m_nMode = nMode;
	m_DetailTiming = Timing;
	m_nHActive = Timing.m_HActive;
	m_nVActive = Timing.m_VActive;
	m_nHz = Timing.m_Hz;

	if (nMode == ADD_RESOLUTION)
	{//添加模式
		// 旧算法
		//m_DetailTiming.m_HBlank   = int(m_nHActive*0.3);
		//m_DetailTiming.m_HFPorch  = int(m_nHActive/25);
		//m_DetailTiming.m_HSyncWid = int(m_nHActive/12.5);

		// 新算法
		m_DetailTiming.m_HBlank   = 160;
		m_DetailTiming.m_HFPorch  = 48;
		m_DetailTiming.m_HSyncWid = 32;

		m_DetailTiming.m_VBlank   = 38;
		m_DetailTiming.m_VFPorch  = 3;
		m_DetailTiming.m_VSyncWid = 6;		

		m_DetailTiming.m_HTotal = m_nHActive + m_DetailTiming.m_HBlank;
		m_DetailTiming.m_VTotal = m_nVActive + m_DetailTiming.m_VBlank;
		m_DetailTiming.m_PixelRate = int(m_DetailTiming.m_HTotal * m_DetailTiming.m_VTotal * m_nHz * 0.0001);
	}
}

CResolutionEditDlg::~CResolutionEditDlg()
{
}

void CResolutionEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HACTIVE, m_nHActive);
	DDX_Text(pDX, IDC_EDIT_VACTIVE, m_nVActive);
	DDX_Text(pDX, IDC_EDIT_HZ, m_nHz);

	DDX_Text(pDX, IDC_EDIT_HACTIVE_DETAIL, m_DetailTiming.m_HActive);
	DDX_Text(pDX, IDC_EDIT_VACTIVE_DETAIL, m_DetailTiming.m_VActive);
	DDX_Text(pDX, IDC_EDIT_HFRONT_DETAIL, m_DetailTiming.m_HFPorch);
	DDX_Text(pDX, IDC_EDIT_VFRONT_DETAIL, m_DetailTiming.m_VFPorch);
	DDX_Text(pDX, IDC_EDIT_HSYNC_DETAIL, m_DetailTiming.m_HSyncWid);
	DDX_Text(pDX, IDC_EDIT_VSYNC_DETAIL, m_DetailTiming.m_VSyncWid);
	DDX_Text(pDX, IDC_EDIT_HTOTAL_DETAIL, m_DetailTiming.m_HTotal);
	DDX_Text(pDX, IDC_EDIT_VTOTAL_DETAIL, m_DetailTiming.m_VTotal);
	DDX_CBIndex(pDX, IDC_CBOX_HPOL_DETAIL, m_DetailTiming.m_HPol);
	DDX_CBIndex(pDX, IDC_CBOX_VPOL_DETAIL, m_DetailTiming.m_VPol);
	DDX_Text(pDX, IDC_EDIT_HZ_DETAIL, m_DetailTiming.m_Hz);

	DDX_Control(pDX, IDC_SPIN_HACTIVE, m_spHact);
	DDX_Control(pDX, IDC_SPIN_VACTIVE, m_spVact);
	DDX_Control(pDX, IDC_SPIN_HZ, m_spHz);
	DDX_Control(pDX, IDC_SPIN_HACTIVE_DETAIL, m_spHact_detail);
	DDX_Control(pDX, IDC_SPIN_VACTIVE_DETAIL, m_spVact_detail);
	DDX_Control(pDX, IDC_SPIN_HFRONT_DETAIL, m_spHfront_detail);
	DDX_Control(pDX, IDC_SPIN_VFRONT_DETAIL, m_spVfront_detail);
	DDX_Control(pDX, IDC_SPIN_HSYNC_DETAIL, m_spHsync_detail);
	DDX_Control(pDX, IDC_SPIN_VSYNC_DETAIL, m_spVsync_detail);
	DDX_Control(pDX, IDC_SPIN_HTOTAL_DETAIL, m_spHtotal_detail);
	DDX_Control(pDX ,IDC_SPIN_VTOTAL_DETAIL, m_spVtotal_detail);
	DDX_Control(pDX, IDC_SPIN_HZ_DETAIL, m_spHz_detail);
}


BEGIN_MESSAGE_MAP(CResolutionEditDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_EDIT_HACTIVE, &CResolutionEditDlg::OnEnChangedHActive)
	ON_EN_CHANGE(IDC_EDIT_VACTIVE, &CResolutionEditDlg::OnEnChangedVActive)
	ON_EN_CHANGE(IDC_EDIT_HZ, &CResolutionEditDlg::OnEnChangedHz)
	ON_EN_CHANGE(IDC_EDIT_HTOTAL_DETAIL, &CResolutionEditDlg::OnEnChangedDetailHtotal)
	ON_EN_CHANGE(IDC_EDIT_VTOTAL_DETAIL, &CResolutionEditDlg::OnEnChangedDetailVtotal)
	ON_BN_CLICKED(IDOK, &CResolutionEditDlg::OnOK)
	ON_BN_CLICKED(IDC_BTN_RESTORE_TIMING, &CResolutionEditDlg::OnBnClickedBtnRestoreTiming)
END_MESSAGE_MAP()


// CResolutionEditDlg 消息处理程序


BOOL CResolutionEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	//Initialize Pixel Clock
	CString strPix;
	strPix.Format(_T("%3.4f"), (double)(m_DetailTiming.m_PixelRate/100.0));
	GetDlgItem(IDC_LABEL_PIXCLK_VAL_DETAIL)->SetWindowText(strPix);

	//Initialize SpinButtonCtrls
	m_spHact.SetRange(0, 10000);
	m_spVact.SetRange(0, 10000);
	m_spHz.SetRange(0, 120);
	m_spHact_detail.SetRange(0, 10000);
	m_spVact_detail.SetRange(0, 10100);
	m_spHfront_detail.SetRange(0, 10100);
	m_spVfront_detail.SetRange(0, 10100);
	m_spHsync_detail.SetRange(0, 10100);
	m_spVsync_detail.SetRange(0, 10100);
	m_spHtotal_detail.SetRange(0, 10100);
	m_spVtotal_detail.SetRange(0, 10100);
	m_spHz_detail.SetRange(0, 120);

	//Initialize Pixel Scanning mode
	CComboBox *pCBox = (CComboBox*)GetDlgItem(IDC_CBOX_SCAN_MODE);
	pCBox->SetCurSel(0);

	m_bOrg = FALSE;

	return TRUE;
}


void CResolutionEditDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	PreShowWindow();
}


void CResolutionEditDlg::PreShowWindow()
{
	if (m_nMode == ADD_RESOLUTION)
	{//添加分辨率模式
		GetDlgItem(IDC_BTN_RESTORE_TIMING)->ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->SetWindowText(_T("添加(&A)"));
	}
	else if (m_nMode == EDIT_RESOLUTION)
	{//编辑分辨率模式		
		if (m_DetailTiming.m_Type == DEFAULT_RESOLUTION)
			GetDlgItem(IDC_BTN_RESTORE_TIMING)->ShowWindow(SW_SHOW);
		else
			GetDlgItem(IDC_BTN_RESTORE_TIMING)->ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->SetWindowText(_T("修改(&M)"));
	}
	else if (m_nMode == DEL_RESOLUTION)
	{//删除分辨率模式		
		GetDlgItem(IDC_BTN_RESTORE_TIMING)->ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->SetWindowText(_T("删除(&R)"));
	}

	//------------------ 总纲 -----------------------------//
	//HACTIVE
	GetDlgItem(IDC_EDIT_HACTIVE)->EnableWindow(m_nMode == ADD_RESOLUTION);
	GetDlgItem(IDC_SPIN_HACTIVE)->EnableWindow(m_nMode == ADD_RESOLUTION);
	//VACTIVE
	GetDlgItem(IDC_EDIT_VACTIVE)->EnableWindow(m_nMode == ADD_RESOLUTION);
	GetDlgItem(IDC_SPIN_VACTIVE)->EnableWindow(m_nMode == ADD_RESOLUTION);
	//HZ
	GetDlgItem(IDC_EDIT_HZ)->EnableWindow(m_nMode == ADD_RESOLUTION);
	GetDlgItem(IDC_SPIN_HZ)->EnableWindow(m_nMode == ADD_RESOLUTION);
	//------------------ 详情 -----------------------------//
	//HACTIVE
	GetDlgItem(IDC_EDIT_HACTIVE_DETAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_HACTIVE_DETAIL)->EnableWindow(FALSE);
	//VACTIVE
	GetDlgItem(IDC_EDIT_VACTIVE_DETAIL)->EnableWindow(FALSE);	
	GetDlgItem(IDC_SPIN_VACTIVE_DETAIL)->EnableWindow(FALSE);
	//HZ
	GetDlgItem(IDC_EDIT_HZ_DETAIL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_HZ_DETAIL)->EnableWindow(FALSE);
	//HFRONT
	GetDlgItem(IDC_EDIT_HFRONT_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	GetDlgItem(IDC_SPIN_HFRONT_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//VFRONT
	GetDlgItem(IDC_EDIT_VFRONT_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	GetDlgItem(IDC_SPIN_VFRONT_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//HSYNCWIDTH
	GetDlgItem(IDC_EDIT_HSYNC_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	GetDlgItem(IDC_SPIN_HSYNC_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//VSYNCWIDTH
	GetDlgItem(IDC_EDIT_VSYNC_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	GetDlgItem(IDC_SPIN_VSYNC_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//HTOTAL
	GetDlgItem(IDC_EDIT_HTOTAL_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	GetDlgItem(IDC_SPIN_HTOTAL_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//VTOTAL
	GetDlgItem(IDC_EDIT_VTOTAL_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	GetDlgItem(IDC_SPIN_VTOTAL_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//HPOLARITY
	GetDlgItem(IDC_CBOX_HPOL_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);
	//VPOLARITY
	GetDlgItem(IDC_CBOX_VPOL_DETAIL)->EnableWindow(m_nMode == EDIT_RESOLUTION);	
}


void CResolutionEditDlg::OnEnChangedHActive()
{
	if (m_bOrg)
		return;

	if (m_nMode == ADD_RESOLUTION)
	{//添加分辨率模式
		//更新详细分辨率 HActive, Hfront, Hsyncwid, Htotal
		//对话框刚创建时会调用OnEnchanged类型函数，此时参数不需要Update
		UpdateData(TRUE);

		m_DetailTiming.m_HActive = m_nHActive;

		// 旧算法
		//m_DetailTiming.m_HFPorch = int(m_nHActive/25);
		//m_DetailTiming.m_HSyncWid = int(m_nHActive/12.5);
		//m_DetailTiming.m_HBlank = int(m_nHActive*0.3);
		// 新算法
		m_DetailTiming.m_HFPorch  = 48;
		m_DetailTiming.m_HSyncWid = 32;
		m_DetailTiming.m_HBlank   = 160;

		m_DetailTiming.m_HTotal = m_nHActive+m_DetailTiming.m_HBlank;
		UpdatePixClock();

		UpdateData(FALSE);
	}
}

void CResolutionEditDlg::OnEnChangedVActive()
{
	if (m_bOrg)
		return;

	if (m_nMode == ADD_RESOLUTION)
	{//添加分辨率模式
		//更新详细分辨率 VActive, Vtotal
		UpdateData();

		m_DetailTiming.m_VActive = m_nVActive;

		// 新算法
		m_DetailTiming.m_VFPorch  = 3;
		m_DetailTiming.m_VSyncWid = 6;
		m_DetailTiming.m_VBlank   = 38;

		m_DetailTiming.m_VTotal = m_nVActive+m_DetailTiming.m_VBlank;
		UpdatePixClock();

		UpdateData(FALSE);
	}
}

void CResolutionEditDlg::OnEnChangedHz()
{
	if (m_bOrg)
		return;

	if (m_nMode == ADD_RESOLUTION)
	{//添加分辨率模式
		//更新详细分辨率 Hz,Pixclock
		UpdateData();

		m_DetailTiming.m_Hz = m_nHz;
		UpdatePixClock();

		UpdateData(FALSE);
	}
}

void CResolutionEditDlg::OnEnChangedDetailHtotal()
{
	if (m_bOrg)
		return;

	if (m_nMode == EDIT_RESOLUTION)
	{//编辑模式
		//更新PixClock,HBlank
		UpdateData();

		m_DetailTiming.m_HBlank = m_DetailTiming.m_HTotal - m_DetailTiming.m_HActive;
		UpdatePixClock();

		UpdateData(FALSE);
	}
}

void CResolutionEditDlg::OnEnChangedDetailVtotal()
{
	if (m_bOrg)
		return;

	if (m_nMode == EDIT_RESOLUTION)
	{//编辑模式
		//更新PixClock
		UpdateData();

		m_DetailTiming.m_VBlank = m_DetailTiming.m_VTotal - m_DetailTiming.m_VActive;
		UpdatePixClock();

		UpdateData(FALSE);
	}
}

void CResolutionEditDlg::UpdatePixClock()
{
	m_DetailTiming.m_PixelRate = int((m_DetailTiming.m_HTotal*m_DetailTiming.m_VTotal*m_nHz)*0.0001);

	CString strClk;
	strClk.Format(_T("%3.4f"), m_DetailTiming.m_PixelRate*0.01);
	GetDlgItem(IDC_LABEL_PIXCLK_VAL_DETAIL)->SetWindowText(strClk);
}

void CResolutionEditDlg::OnOK()
{
	UpdateData();

	if (m_nMode == ADD_RESOLUTION)
	{//添加模式
		m_DetailTiming.m_Type = CUSTOM_RESOLUTION;
	}

	CDialog::OnOK();
}

void CResolutionEditDlg::OnBnClickedBtnRestoreTiming()
{
	if (m_nMode == EDIT_RESOLUTION)
	{//编辑模式
		m_DetailTiming = CResolutionManager::GetDefaultResolution(m_DetailTiming.m_HActive, m_DetailTiming.m_VActive, m_DetailTiming.m_Hz);
		if (m_DetailTiming.IsValid())
		{
			UpdatePixClock();
			UpdateData(FALSE);
		}		
	}
}
