// MvxBackgrdSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MvxBackgrdSettingDlg.h"
#include "MvxSplitDlg.h"


// CMvxBackgrdSettingDlg 对话框

IMPLEMENT_DYNAMIC(CMvxBackgrdSettingDlg, CDialog)

CMvxBackgrdSettingDlg::CMvxBackgrdSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMvxBackgrdSettingDlg::IDD, pParent)
	,m_nHPhyScreen(1)
	,m_nVPhyScreen(1)
	,m_nHLogScreen(2)
	,m_nVLogScreen(2)
	,m_bHasCustomizedTiming(FALSE)
{
	memset(&m_SelectedTiming, 0, sizeof(TIMING_STRUCT));
	memset(&m_CustomizedTiming, 0, sizeof(TIMING_STRUCT));

	m_pStrTiming = NULL;
	m_pDetailedTiming = NULL;
}

CMvxBackgrdSettingDlg::~CMvxBackgrdSettingDlg()
{
}

void CMvxBackgrdSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_MVX_BKGTIMING, m_CboxBkgTiming);
	DDX_Control(pDX, IDC_CBOX_MVX_BKG_HPOL, m_CboxHPol);
	DDX_Control(pDX, IDC_CBOX_MVX_BKG_VPOL, m_CboxVPol);
	DDX_Control(pDX, IDC_CBOX_MVX_BKG_SPLITMODE, m_CboxSplitMode);
	DDX_Control(pDX, IDC_BTN_MVX_CUSTOMIZE_TIMING, m_BtnCustomTiming);
	DDX_Control(pDX, IDC_BTN_MVX_SET_BKGTIMING, m_BtnSetBkgTiming);
	DDX_Control(pDX, IDC_BTN_MVX_SET_COMBINATION, m_BtnSetCombiMode);
	DDX_Control(pDX, IDC_SPIN_MVX_BKG_FRESHRATE, m_SpinFreshRate);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_EDIT_MVX_BKG_FRESHRATE, m_EditHz);

	DDX_Text(pDX, IDC_EDIT_MVX_BKG_HACTIVE, m_nHActive);
	DDX_Text(pDX, IDC_EDIT_MVX_BKG_HFRONT, m_nHFront);
	DDX_Text(pDX, IDC_EDIT_MVX_BKG_HSYNC, m_nHSync);
	DDX_Text(pDX, IDC_EDIT_MVX_BKG_HTOTAL, m_nHTotal);
	DDX_CBIndex(pDX, IDC_CBOX_MVX_BKG_HPOL, m_nHPol);

	DDX_Text(pDX, IDC_EDIT_MVX_BKG_VACTIVE, m_nVActive);
	DDX_Text(pDX, IDC_EDIT_MVX_BKG_VFRONT, m_nVFront);
	DDX_Text(pDX, IDC_EDIT_MVX_BKG_VSYNC, m_nVSync);
	DDX_Text(pDX, IDC_EDIT_MVX_BKG_VTOTAL, m_nVTotal);
	DDX_CBIndex(pDX, IDC_CBOX_MVX_BKG_VPOL, m_nVPol);

	DDX_Text(pDX, IDC_EDIT_MVX_BKG_FRESHRATE, m_nHZ);

	DDX_Text(pDX, IDC_EDIT_MVX_PHY_HORZ, m_nHPhyScreen);
	DDX_Text(pDX, IDC_EDIT_MVX_PHY_VERT, m_nVPhyScreen);
	DDX_Text(pDX, IDC_EDIT_MVX_LOG_HORZ, m_nHLogScreen);
	DDX_Text(pDX, IDC_EDIT_MVX_LOG_VERT, m_nVLogScreen);
}


BEGIN_MESSAGE_MAP(CMvxBackgrdSettingDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_CBOX_MVX_BKG_SPLITMODE, OnCbnSelchangeSplitMode)
	ON_CBN_SELCHANGE(IDC_CBOX_MVX_BKGTIMING, OnCbnSelchangeTiming)
	ON_BN_CLICKED(IDC_BTN_MVX_CUSTOMIZE_TIMING, OnBtnClickedCustomTiming)
	ON_BN_CLICKED(IDC_BTN_MVX_SET_BKGTIMING, OnBtnClickedSetBkgTiming)
	ON_BN_CLICKED(IDC_BTN_MVX_SET_COMBINATION, OnBtnClickedSetCombination)
	ON_EN_CHANGE(IDC_EDIT_MVX_BKG_HTOTAL, OnEnChangedEditHTotal)
	ON_EN_CHANGE(IDC_EDIT_MVX_BKG_VTOTAL, OnEnChangedEditVTotal)
	ON_EN_CHANGE(IDC_EDIT_MVX_BKG_FRESHRATE, OnEnChangedEditHz)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MVX_BKG_FRESHRATE, &CMvxBackgrdSettingDlg::OnDeltaposSpinMvxBkgFreshrate)
END_MESSAGE_MAP()


// CMvxBackgrdSettingDlg 消息处理程序

BOOL CMvxBackgrdSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMvxSplitDlg *pParent = (CMvxSplitDlg*)m_pParentWnd;
	m_pStrTiming = pParent->m_boardInfo.GetTimingName(Product_MVX);
	m_pDetailedTiming = pParent->m_boardInfo.GetDetailTiming(Product_MVX);

	if (m_pStrTiming == NULL || m_pDetailedTiming == NULL) OnClose();

	SetTheme(xtpControlThemeResource);
	m_SpinFreshRate.SetBuddy(&m_EditHz);

	AddSplitMode();
	AddTiming();

	return TRUE;
}

HBRUSH CMvxBackgrdSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CMvxBackgrdSettingDlg::SetTheme(XTPControlTheme theme)
{
	m_CboxBkgTiming.SetTheme(theme);
	m_CboxHPol.SetTheme(theme);
	m_CboxVPol.SetTheme(theme);
	m_CboxSplitMode.SetTheme(theme);

	m_BtnSetCombiMode.SetTheme(theme);
	m_BtnSetBkgTiming.SetTheme(theme);
	m_BtnCustomTiming.SetTheme(theme);
	m_BtnCancel.SetTheme(theme);

	m_SpinFreshRate.SetTheme(theme);
}

void CMvxBackgrdSettingDlg::AddSplitMode()
{
	m_CboxSplitMode.ResetContent();

	int i=0;
	while (stSplitMode[i].strmode != _T("END"))
	{
		if (stSplitMode[i].nScreenNum <= 3)
			m_CboxSplitMode.AddString(stSplitMode[i].strmode);

		i++;
	}
}

void CMvxBackgrdSettingDlg::AddTiming()
{
	CMvxSplitDlg *pParent = (CMvxSplitDlg*)m_pParentWnd;

	if (m_pStrTiming == NULL) return;

	//找出与背景信号分辨率一致的分辨率的序号
	int nIndex = 0;
	int nCurSel = -1;
	while (m_pStrTiming[nIndex] != _T("END"))
	{
		if ((m_pDetailedTiming[nIndex].m_HActive == pParent->m_nPhyHorz)
		  &&(m_pDetailedTiming[nIndex].m_VActive == pParent->m_nPhyVert)
		  &&(m_pDetailedTiming[nIndex].m_Hz == pParent->m_BkgTiming.m_Hz)
		  &&(nCurSel == -1))
		{
			nCurSel = nIndex;
			m_SelectedTiming = m_pDetailedTiming[nIndex];
			break;
		}

		nIndex ++;
	}

	CString str;

	if (nCurSel == -1)
	{//预定义的分辨率中没有该分辨率，则将该分辨率自定义列出
		m_bHasCustomizedTiming = TRUE;

		m_CboxSplitMode.SelectString(0, stSplitMode[TOTAL_SPLIT_MODE-2].strmode);
		AddTimingByModeName(stSplitMode[TOTAL_SPLIT_MODE-2].strmode);

		m_SelectedTiming = pParent->m_BkgTiming;
		m_CustomizedTiming = pParent->m_BkgTiming;

		str.Format(_T("Customized:%dx%d @ %dHz"), m_SelectedTiming.m_HActive, m_SelectedTiming.m_VActive, m_SelectedTiming.m_Hz);
		m_CboxBkgTiming.AddString(str);		
		m_CboxBkgTiming.SelectString(0, str);	
	}
	else
	{//在预定义的分辨率中找到了该分辨率
		m_bHasCustomizedTiming = FALSE;
		
		//得到该分辨率的分屏模式
		int nToken = 0;
		str = m_pStrTiming[nCurSel].Tokenize(_T("("), nToken);
		str = m_pStrTiming[nCurSel].Right(m_pStrTiming[nCurSel].GetLength() - nToken);
		nToken = 0;
		str = str.Tokenize(_T(")"), nToken);
		m_CboxSplitMode.SelectString(0, str);

		AddTimingByModeName(str);
		m_CboxBkgTiming.SelectString(0, m_pStrTiming[nCurSel]);
	}

	ShowDetailedTiming();
}

const int CMvxBackgrdSettingDlg::AddTimingByModeName(const CString splitmode)
{
	m_CboxBkgTiming.ResetContent();

	int iIndex=0;
	int iLastIndex = 0;
	while(m_pStrTiming[iIndex] != _T("END"))
	{
		if (m_pStrTiming[iIndex].Find(splitmode) != -1) {
			m_CboxBkgTiming.AddString(m_pStrTiming[iIndex]);
			iLastIndex = iIndex;
		}

		iIndex++;
	}

	return iLastIndex;
}

void CMvxBackgrdSettingDlg::ShowDetailedTiming()
{
	m_nHActive = m_SelectedTiming.m_HActive;
	m_nHFront  = m_SelectedTiming.m_HFPorch;
	m_nHSync   = m_SelectedTiming.m_HSyncWid;
	m_nHTotal  = m_SelectedTiming.m_HActive + m_SelectedTiming.m_HBlank;
	m_nHPol    = m_SelectedTiming.m_HPol;

	m_nVActive = m_SelectedTiming.m_VActive;
	m_nVFront  = m_SelectedTiming.m_VFPorch;
	m_nVSync   = m_SelectedTiming.m_VSyncWid;
	m_nVTotal  = m_SelectedTiming.m_VActive + m_SelectedTiming.m_VBlank;
	m_nVPol    = m_SelectedTiming.m_VPol;

	m_nHZ      = m_SelectedTiming.m_Hz;

	CString strPixClk;
	strPixClk.Format(_T("%3.4f MHz"), m_nHTotal*m_nVTotal*m_nHZ*0.000001);
	GetDlgItem(IDC_TEXT_MVX_BKG_PIXCLOCK)->SetWindowText(strPixClk);

	UpdateData(FALSE);
}

void CMvxBackgrdSettingDlg::OnCbnSelchangeSplitMode()
{
	//获取当前选择的分屏模式
	CString str;
	m_CboxSplitMode.GetWindowText(str);

	int nIndex = AddTimingByModeName(str);

	//如果存在自定义分辨率
	if (m_bHasCustomizedTiming)
	{
		CString strCustom;
		strCustom.Format(_T("Customized:%dx%d @ %dHz"), m_CustomizedTiming.m_HActive, m_CustomizedTiming.m_VActive, m_CustomizedTiming.m_Hz);
		m_CboxBkgTiming.AddString(strCustom);

		m_SelectedTiming = m_CustomizedTiming;
	}
	else
		m_SelectedTiming = m_pDetailedTiming[nIndex];

	m_CboxBkgTiming.SetCurSel(m_CboxBkgTiming.GetCount()-1);

	ShowDetailedTiming();
}

void CMvxBackgrdSettingDlg::OnCbnSelchangeTiming()
{
	int nIndex = GetSelectedTimingIndex();
	
	//可能是自定义分辨率
	if ((nIndex == -1) && m_bHasCustomizedTiming)
		m_SelectedTiming = m_CustomizedTiming;
	else
		m_SelectedTiming = m_pDetailedTiming[nIndex];

	ShowDetailedTiming();
}

const int CMvxBackgrdSettingDlg::GetSelectedTimingIndex()
{
	CString strTiming;
	m_CboxBkgTiming.GetWindowText(strTiming);

	int iSelected = -1;
	int i=0; 
	while (m_pStrTiming[i] != _T("END"))
	{
		if (m_pStrTiming[i] == strTiming) {
			iSelected = i;
			return i;
		}

		i++;
	}

	return iSelected;
}

void CMvxBackgrdSettingDlg::OnBtnClickedSetBkgTiming()
{
	//设置背景分辨率

	CMvxSplitDlg *pParent = (CMvxSplitDlg*)m_pParentWnd;

	UINT nRet = theApp.SetInputTiming(&m_SelectedTiming, 0x03, 0xff, pParent->m_boardInfo.GetBoardPort());

	if (nRet == CMD_RTOK)
		MessageBox(_T("设置输入分辨率成功！请重新拔插输入信号线后，点击刷新！"), _T("设置背景分辨率"), MB_OK|MB_ICONINFORMATION);
	else
		MessageBox(_T("错误，设置输入分辨率失败！"), _T("设置背景分辨率"), MB_OK|MB_ICONSTOP);
}

void CMvxBackgrdSettingDlg::OnBtnClickedCustomTiming()
{
	//添加自定义分辨率
	UpdateData();
	int i=0; 
	while(m_pStrTiming[i] != _T("END"))
	{
		if (m_pDetailedTiming[i].m_HActive == m_nHActive
			&&m_pDetailedTiming[i].m_VActive == m_nVActive
			&&m_pDetailedTiming[i].m_Hz == m_nHZ)
		{
			return (void)MessageBox(_T("警告：自定义的分辨率与预定义的分辨率有冲突！\r\n预定义的分辨率：")+m_pStrTiming[i],\
				_T("自定义分辨率"), MB_OK|MB_ICONWARNING);
		}

		i++;
	}

	m_CustomizedTiming.m_HActive = m_nHActive;
	m_CustomizedTiming.m_HTotal  = m_nHTotal;
	m_CustomizedTiming.m_HBlank  = m_nHTotal - m_nHActive;
	m_CustomizedTiming.m_HFPorch = m_nHFront;
	m_CustomizedTiming.m_HSyncWid= m_nHSync;
	m_CustomizedTiming.m_HPol    = m_CboxHPol.GetCurSel();
	m_CustomizedTiming.m_HSize   = m_nHActive;
	
	m_CustomizedTiming.m_VActive = m_nVActive;
	m_CustomizedTiming.m_VTotal  = m_nVTotal;
	m_CustomizedTiming.m_VBlank  = m_nVTotal - m_nVActive;
	m_CustomizedTiming.m_VFPorch = m_nVFront;
	m_CustomizedTiming.m_VSyncWid= m_nVSync;
	m_CustomizedTiming.m_VPol    = m_CboxVPol.GetCurSel();
	m_CustomizedTiming.m_VSize   = m_nVActive;

	m_CustomizedTiming.m_Hz      = m_nHZ;

	m_CustomizedTiming.m_PixelRate = Round(m_nHTotal*m_nVTotal*m_nHZ/10000);

	CString str;
	str.Format(_T("Customized:%dx%d @ %dHz"), m_nHActive, m_nVActive, m_nHZ);

	if (m_bHasCustomizedTiming)
		m_CboxBkgTiming.DeleteString(m_CboxBkgTiming.GetCount()-1);

	m_CboxBkgTiming.AddString(str);
	m_CboxBkgTiming.SelectString(0, str);

	m_SelectedTiming = m_CustomizedTiming;
}

void CMvxBackgrdSettingDlg::OnBtnClickedSetCombination()
{
	UpdateData();

	//通知父窗口更新网格
	CMvxSplitDlg *pParent = (CMvxSplitDlg*)m_pParentWnd;

	pParent->m_nHorzLines = m_nHLogScreen;
	pParent->m_nVertLines = m_nVLogScreen;

	pParent->CalcAnchorPoint();
	pParent->Invalidate();
}

void CMvxBackgrdSettingDlg::OnEnChangedEditHTotal()
{
	UpdateData();

	UpdatePixClock();
}

void CMvxBackgrdSettingDlg::OnEnChangedEditVTotal()
{
	UpdateData();

	UpdatePixClock();
}

void CMvxBackgrdSettingDlg::OnEnChangedEditHz()
{
	UpdateData();

	if (m_nHZ < 25)  { m_nHZ = 25; UpdateData(FALSE); }
	if (m_nHZ > 120) { m_nHZ = 120; UpdateData(FALSE); }

	UpdatePixClock();
}

void CMvxBackgrdSettingDlg::OnDeltaposSpinMvxBkgFreshrate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int nNewVal = m_nHZ - pNMUpDown->iDelta;

	if (nNewVal < 25) nNewVal = 25;
	if (nNewVal > 120) nNewVal = 120;

	m_nHZ = nNewVal;

	UpdateData(FALSE);

	UpdatePixClock();
	
	*pResult = 0;
}


void CMvxBackgrdSettingDlg::UpdatePixClock()
{
	CString strPixClk;
	strPixClk.Format(_T("%3.4f MHz"), m_nHTotal*m_nVTotal*m_nHZ*0.000001);

	GetDlgItem(IDC_TEXT_MVX_BKG_PIXCLOCK)->SetWindowText(strPixClk);

}