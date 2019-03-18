// HdmiTimingSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "HdmiTimingSetDlg.h"
#include "Hdmi3dDlg.h"


// CHdmiTimingSetDlg 对话框

IMPLEMENT_DYNAMIC(CHdmiTimingSetDlg, CXTPResizeDialog)

CHdmiTimingSetDlg::CHdmiTimingSetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CHdmiTimingSetDlg::IDD, pParent)
	, m_nHact(0)
	, m_nVact(0)
	, m_nHfront(0)
	, m_nVfront(0)
	, m_nHsync(0)
	, m_nVsync(0)
	, m_nHpol(0)
	, m_nVpol(0)
	, m_nHtotal(0)
	, m_nVtotal(0)
	, m_nHz(0)
{
	m_pTimings = NULL;
	m_pszTiming = NULL;
	m_nTotalTiming = 0;

	memset(&m_InTiming, 0, sizeof(TIMING_STRUCT));
	memset(&m_OutTiming, 0, sizeof(TIMING_STRUCT));
}

CHdmiTimingSetDlg::~CHdmiTimingSetDlg()
{
}

void CHdmiTimingSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GBOX_HDMI_INPUTTIMING, m_GboxInput);
	DDX_Control(pDX, IDC_GBOX_HDMI_OUTPUTTIMING, m_GboxOutput);
	DDX_Control(pDX, IDC_BTN_HDMI_SET_INPUTTIMING, m_BtnSetInput);
	DDX_Control(pDX, IDC_BTN_HDMI_SET_OUTPUTTIMING, m_BtnSetOutput);
	DDX_Control(pDX, IDC_BTN_CUSTOM_TIMING, m_BtnCustomIn);
	DDX_Control(pDX, IDC_BTN_HDMI_CUSTOM_OUT, m_BtnCustomOut);
	DDX_Control(pDX, IDC_CBOX_HDMI_INPUTTIMING, m_CboxInputTiming);
	DDX_Control(pDX, IDC_CBOX_HDMI_OUTPUTTIMING, m_CboxOutputTiming);
	DDX_Control(pDX, IDC_CBOX_HDMI_HPOL, m_CboxHPol);
	DDX_Control(pDX, IDC_CBOX_HDMI_VPOL, m_CboxVPol);
	DDX_Control(pDX, IDC_SPIN_HDMI_HZ, m_SpinHz);
	DDX_Control(pDX, IDC_EDIT_HDMI_HZ, m_EditHz);
	DDX_Text(pDX, IDC_EDIT_HDMI_HACTIVE, m_nHact);
	DDX_Text(pDX, IDC_EDIT_HDMI_HSYNC, m_nHsync);
	DDX_Text(pDX, IDC_EDIT_HDMI_HFRONT, m_nHfront);
	DDX_Text(pDX, IDC_EDIT_HDMI_HTOTAL, m_nHtotal);
	DDX_Text(pDX, IDC_EDIT_HDMI_VACTIVE, m_nVact);
	DDX_Text(pDX, IDC_EDIT_HDMI_VSYNC, m_nVsync);
	DDX_Text(pDX, IDC_EDIT_HDMI_VFRONT, m_nVfront);
	DDX_Text(pDX, IDC_EDIT_HDMI_VTOTAL, m_nVtotal);
	DDX_Text(pDX, IDC_EDIT_HDMI_HZ, m_nHz);
	DDX_CBIndex(pDX, IDC_CBOX_HDMI_HPOL, m_nHpol);
	DDX_CBIndex(pDX, IDC_CBOX_HDMI_VPOL, m_nVpol);
}


BEGIN_MESSAGE_MAP(CHdmiTimingSetDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_HDMI_HTOTAL, &CHdmiTimingSetDlg::OnEnChangeEditHdmiHtotal)
	ON_EN_CHANGE(IDC_EDIT_HDMI_VTOTAL, &CHdmiTimingSetDlg::OnEnChangeEditHdmiVtotal)
	ON_EN_CHANGE(IDC_EDIT_HDMI_HZ, &CHdmiTimingSetDlg::OnEnChangeEditHdmiHz)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HDMI_HZ, &CHdmiTimingSetDlg::OnDeltaposSpinHdmiHz)
	ON_BN_CLICKED(IDC_BTN_CUSTOM_TIMING, &CHdmiTimingSetDlg::OnBnClickedBtnCustomInTiming)
	ON_BN_CLICKED(IDC_BTN_HDMI_CUSTOM_OUT, &CHdmiTimingSetDlg::OnBnClickedBtnCustomOutTiming)
	ON_CBN_SELCHANGE(IDC_CBOX_HDMI_INPUTTIMING, &CHdmiTimingSetDlg::OnCbnSelchangeCboxHdmiInputtiming)
	ON_CBN_SELCHANGE(IDC_CBOX_HDMI_OUTPUTTIMING, &CHdmiTimingSetDlg::OnCbnSelchangeCboxHdmiOutputtiming)
	ON_BN_CLICKED(IDC_BTN_HDMI_SET_INPUTTIMING, &CHdmiTimingSetDlg::OnBnClickedBtnHdmiSetInputtiming)
	ON_BN_CLICKED(IDC_BTN_HDMI_SET_OUTPUTTIMING, &CHdmiTimingSetDlg::OnBnClickedBtnHdmiSetOutputtiming)
END_MESSAGE_MAP()


// CHdmiTimingSetDlg 消息处理程序

HBRUSH CHdmiTimingSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

	return hbr;
}


void CHdmiTimingSetDlg::SetTheme(XTPControlTheme theme)
{
	m_GboxInput.SetTheme(theme);
	m_GboxOutput.SetTheme(theme);
	m_BtnCustomIn.SetTheme(theme);
	m_BtnSetInput.SetTheme(theme);
	m_BtnSetOutput.SetTheme(theme);
	m_BtnCustomOut.SetTheme(theme);
	m_CboxInputTiming.SetTheme(theme);
	m_CboxOutputTiming.SetTheme(theme);
	m_CboxHPol.SetTheme(theme);
	m_CboxVPol.SetTheme(theme);
	m_SpinHz.SetTheme(theme);
}

BOOL CHdmiTimingSetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	m_bkBrush.CreateSolidBrush(constClrDlgBk[3]);

	ResizeAll();
	SetTheme(xtpControlThemeResource);
	m_SpinHz.SetBuddy(&m_EditHz);

	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	m_pTimings = pOwner->m_boardinfo.GetDetailTiming(Product_HDMI_3D);
	m_pszTiming = pOwner->m_boardinfo.GetTimingName(Product_HDMI_3D);
	m_nTotalTiming = pOwner->m_boardinfo.GetTotalTimings(Product_HDMI_3D);

	AddTiming();

	return TRUE;
}

void CHdmiTimingSetDlg::ResizeAll()
{
	SetResize(IDC_GBOX_HDMI_INPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_GBOX_HDMI_OUTPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_INPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_OUTPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_HPOL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_VPOL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_SET_INPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_SET_OUTPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_CUSTOM_TIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_CUSTOM_OUT, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_SPIN_HDMI_HZ, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);

	SetResize(IDC_EDIT_HDMI_HACTIVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_VACTIVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_HFRONT, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_VFRONT, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_HSYNC, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_VSYNC, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_HTOTAL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_VTOTAL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_HDMI_HZ, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	
	SetResize(IDC_TEXT_HDMI_INPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_OUTPUTTIMING, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_HACTIVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_HFRONT, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_HSYNC, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_HTOTAL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_POL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_HZ, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_FRESH, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_PIXEL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_PIXEL_VAL, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_HORZ, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_VERT, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
}

void CHdmiTimingSetDlg::AddTiming()
{
	m_CboxInputTiming.ResetContent();
	m_CboxOutputTiming.ResetContent();

	for (int i=0; i<m_nTotalTiming; i++) {
		m_CboxInputTiming.AddString(m_pszTiming[i]);
		m_CboxOutputTiming.AddString(m_pszTiming[i]);
	}
}

void CHdmiTimingSetDlg::Initialize()
{
	CHdmi3dDlg *pOwer = (CHdmi3dDlg*)GetOwner();
	int iPort = pOwer->m_boardinfo.GetBoardPort();

	TIMING_STRUCT inTiming, outTiming;
	CString strCurTiming;
	int index = 0;
	memset(&inTiming, 0, sizeof(TIMING_STRUCT));
	memset(&outTiming, 0, sizeof(TIMING_STRUCT));
	//获取输入分辨率
	if (theApp.GetInputTiming(&inTiming, 3, 0xff, iPort) == CMD_FAILURE) {
		MessageBox(_T("获取输入分辨率失败！"), _T("HDMI_3D初始化"), MB_OK|MB_ICONERROR);
	}
	else {
		for (index=0; index<m_nTotalTiming; index++) {
			if (inTiming.m_HActive == m_pTimings[index].m_HActive 
			 && inTiming.m_VActive == m_pTimings[index].m_VActive
			 && inTiming.m_Hz == m_pTimings[index].m_Hz) {
				 m_CboxInputTiming.SetCurSel(index);
				 break;
			}			 
		}
		//该分辨率可能是自定义的
		if (index == m_nTotalTiming) {
			strCurTiming.Format(_T("自定义：%dx%d @%dHz"), inTiming.m_HActive, inTiming.m_VTotal, inTiming.m_Hz);
			m_CboxInputTiming.AddString(strCurTiming);
			m_CboxInputTiming.SelectString(0, strCurTiming);
		}

		m_InTiming = inTiming;
		ShowDetailTiming(&inTiming);
	}

	//获取输出分辨率
	if (theApp.GetOutputTiming(&outTiming, iPort) == CMD_FAILURE) {
		MessageBox(_T("获取输出分辨率失败！"), _T("HDMI_3D初始化"), MB_OK|MB_ICONERROR);
	}
	else {
		for (index=0; index<m_nTotalTiming; index++) {
			if (outTiming.m_HActive == m_pTimings[index].m_HActive 
			 && outTiming.m_VActive == m_pTimings[index].m_VActive
			 && outTiming.m_Hz == m_pTimings[index].m_Hz) {
					m_CboxOutputTiming.SetCurSel(index);
					break;
			}			 
		}
		//该分辨率可能是自定义的
		if (index == m_nTotalTiming) {
			strCurTiming.Format(_T("自定义：%dx%d @%dHz"), outTiming.m_HActive, outTiming.m_VTotal, outTiming.m_Hz);
			m_CboxOutputTiming.AddString(strCurTiming);
			m_CboxOutputTiming.SelectString(0, strCurTiming);
		}

		m_OutTiming = outTiming;
	}
}


void CHdmiTimingSetDlg::ShowDetailTiming(pTIMING_STRUCT pTiming)
{
	m_nHact = pTiming->m_HActive;
	m_nVact = pTiming->m_VActive;
	m_nHfront = pTiming->m_HFPorch;
	m_nVfront = pTiming->m_VFPorch;
	m_nHsync = pTiming->m_HSyncWid;
	m_nVsync = pTiming->m_VSyncWid;
	m_nHtotal = pTiming->m_HTotal;
	m_nVtotal = pTiming->m_VTotal;
	m_nHpol = pTiming->m_HPol;
	m_nVpol = pTiming->m_VPol;
	m_nHz = pTiming->m_Hz;
	UpdatePixelClock();

	UpdateData(FALSE);
}

void CHdmiTimingSetDlg::UpdatePixelClock()
{
	CString str;
	str.Format(_T("%3.4f MHz"), m_nHtotal*m_nVtotal*m_nHz*0.000001);
	GetDlgItem(IDC_TEXT_HDMI_PIXEL_VAL)->SetWindowText(str);
}

void CHdmiTimingSetDlg::OnEnChangeEditHdmiHtotal()
{
	UpdateData();

	UpdatePixelClock();
}

void CHdmiTimingSetDlg::OnEnChangeEditHdmiVtotal()
{
	UpdateData();

	UpdatePixelClock();
}

void CHdmiTimingSetDlg::OnEnChangeEditHdmiHz()
{
	UpdateData();

	UpdatePixelClock();
}

void CHdmiTimingSetDlg::OnDeltaposSpinHdmiHz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	int nNewVal = m_nHz - pNMUpDown->iDelta;

	m_nHz = nNewVal;

	UpdateData(FALSE);

	UpdatePixelClock();

	*pResult = 0;
}

void CHdmiTimingSetDlg::OnBnClickedBtnCustomInTiming()
{
	if (!ValidateTiming()) return;

	CString strCustomTiming;
	strCustomTiming.Format(_T("自定义：%dx%d @%dHz"), m_nHact, m_nVact, m_nHz);
	m_CboxInputTiming.DeleteString(m_nTotalTiming);
	m_CboxInputTiming.AddString(strCustomTiming);
	m_CboxInputTiming.SelectString(0, strCustomTiming);

	m_InTiming.m_HActive = m_nHact;
	m_InTiming.m_VActive = m_nVact;
	m_InTiming.m_HFPorch = m_nHfront;
	m_InTiming.m_VFPorch = m_nVfront;
	m_InTiming.m_HSyncWid = m_nHsync;
	m_InTiming.m_VSyncWid = m_nVsync;
	m_InTiming.m_HTotal = m_nHtotal;
	m_InTiming.m_VTotal = m_nVtotal;
	m_InTiming.m_HPol = m_nHpol;
	m_InTiming.m_VPol = m_nVpol;
	m_InTiming.m_Hz = m_nHz;
	m_InTiming.m_PixelRate = int(m_nHtotal*m_nVtotal*m_nHz*0.0001);

}

void CHdmiTimingSetDlg::OnBnClickedBtnCustomOutTiming()
{
	if (!ValidateTiming()) return;

	CString strCustomTiming;
	strCustomTiming.Format(_T("自定义：%dx%d @%dHz"), m_nHact, m_nVact, m_nHz);
	m_CboxOutputTiming.DeleteString(m_nTotalTiming);
	m_CboxOutputTiming.AddString(strCustomTiming);
	m_CboxOutputTiming.SelectString(0, strCustomTiming);

	m_OutTiming.m_HActive = m_nHact;
	m_OutTiming.m_VActive = m_nVact;
	m_OutTiming.m_HFPorch = m_nHfront;
	m_OutTiming.m_VFPorch = m_nVfront;
	m_OutTiming.m_HSyncWid = m_nHsync;
	m_OutTiming.m_VSyncWid = m_nVsync;
	m_OutTiming.m_HTotal = m_nHtotal;
	m_OutTiming.m_VTotal = m_nVtotal;
	m_OutTiming.m_HPol = m_nHpol;
	m_OutTiming.m_VPol = m_nVpol;
	m_OutTiming.m_Hz = m_nHz;
	m_OutTiming.m_PixelRate = int(m_nHtotal*m_nVtotal*m_nHz*0.0001);
}

bool CHdmiTimingSetDlg::ValidateTiming()
{
	UpdateData();

	CString strMsg;
	if (IsExistTiming()) {
		strMsg.Format(_T("列表中已存在\" %dx%d @%dHz \"的相同分辨率，请不要重复添加！"), m_nHact, m_nVact, m_nHz);
		MessageBox(strMsg, _T("分辨率无效"), MB_OK|MB_ICONWARNING);
		return false;
	}

	bool bValidate = true;

	CEdit *pEdit = NULL;
	if (m_nHact >= m_nHtotal) {
		MessageBox(_T("活动像素应小于总像素！"), _T("分辨率无效"), MB_OK|MB_ICONWARNING);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HDMI_HACTIVE);
		bValidate = false;
	}

	if (bValidate) {
		if (m_nVact >= m_nVtotal)
		{
			MessageBox(_T("活动像素应小于总像素！"), _T("分辨率无效"), MB_OK|MB_ICONWARNING);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HDMI_VACTIVE);
			bValidate = false;
		}
	}

	if (bValidate) {
		if (m_nHact <= 0)
		{
			MessageBox(_T("活动像素不能为 0 ！"), _T("分辨率无效"), MB_OK|MB_ICONWARNING);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HDMI_HACTIVE);
			bValidate = false;
		}
	}

	if (bValidate) {
		if (m_nVact <= 0)
		{
			MessageBox(_T("活动像素不能为 0 ！"), _T("分辨率无效"), MB_OK|MB_ICONWARNING);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HDMI_VACTIVE);
			bValidate = false;
		}
	}

	if (bValidate) {
		if (m_nHz < 25 || m_nHz >  120)
		{
			MessageBox(_T("刷新率有效范围为 25-120 Hz！"), _T("分辨率无效"), MB_OK|MB_ICONWARNING);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HDMI_HZ);
			bValidate = false;
		}
	}

	if (!bValidate) {
		pEdit->SetFocus();
		pEdit->SetSel(0, pEdit->GetWindowTextLength());
	}

	return bValidate;
}

bool CHdmiTimingSetDlg::IsExistTiming()
{
	for (int i=0; i<m_nTotalTiming; i++) {
		if (   m_nHz     == m_pTimings[i].m_Hz
			&& m_nHact   == m_pTimings[i].m_HActive
			&& m_nVact   == m_pTimings[i].m_VActive
			&& m_nHtotal == m_pTimings[i].m_HTotal
			&& m_nVtotal == m_pTimings[i].m_VTotal
			&& m_nHfront == m_pTimings[i].m_HFPorch
			&& m_nVfront == m_pTimings[i].m_VFPorch
			&& m_nHsync  == m_pTimings[i].m_HSyncWid
			&& m_nVsync  == m_pTimings[i].m_VSyncWid
			&& m_nHpol   == m_pTimings[i].m_HPol
			&& m_nVpol   == m_pTimings[i].m_VPol )
		{
			return true;
		}
	}

	return false;
}

void CHdmiTimingSetDlg::OnCbnSelchangeCboxHdmiInputtiming()
{
	int nCursel = m_CboxInputTiming.GetCurSel();
	m_InTiming = m_pTimings[nCursel];
	ShowDetailTiming(&m_InTiming);
}

void CHdmiTimingSetDlg::OnCbnSelchangeCboxHdmiOutputtiming()
{
	int nCursel = m_CboxOutputTiming.GetCurSel();
	m_OutTiming = m_pTimings[nCursel];
	ShowDetailTiming(&m_OutTiming);
}

void CHdmiTimingSetDlg::OnBnClickedBtnHdmiSetInputtiming()
{
	if (m_CboxInputTiming.GetCurSel() == -1) {
		MessageBox(_T("请选择输入分辨率！"), _T("设置输入分辨率"), MB_OK|MB_ICONWARNING);
		return;
	}

	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();

	if (theApp.SetInputTiming(&m_InTiming, 3, 0, nPort) == CMD_FAILURE) {
		MessageBox(_T("设置输入分辨率失败！"), _T("设置输入分辨率"), MB_OK|MB_ICONERROR);
	}
	else {
		MessageBox(_T("设置输入分辨率成功！"), _T("设置输入分辨率"), MB_OK|MB_ICONINFORMATION);
	}
}

void CHdmiTimingSetDlg::OnBnClickedBtnHdmiSetOutputtiming()
{
	if (m_CboxOutputTiming.GetCurSel() == -1) {
		MessageBox(_T("请选择输出分辨率！"), _T("设置输出分辨率"), MB_OK|MB_ICONWARNING);
		return;
	}

	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();

	if (theApp.SetOutputTiming(&m_OutTiming, nPort) == CMD_FAILURE) {
		MessageBox(_T("设置输出分辨率失败！"), _T("设置输出分辨率"), MB_OK|MB_ICONERROR);
	}
	else {
		MessageBox(_T("设置输出分辨率成功！"), _T("设置输出分辨率"), MB_OK|MB_ICONINFORMATION);
	}
}
