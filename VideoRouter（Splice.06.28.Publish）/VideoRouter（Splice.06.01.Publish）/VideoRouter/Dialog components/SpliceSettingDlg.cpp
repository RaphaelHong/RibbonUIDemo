// SpliceSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "..\GridView.h"
#include "SpliceSettingDlg.h"
#include "ResolutionEditDlg.h"


// CSpliceSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSpliceSettingDlg, CDialog)

CSpliceSettingDlg::CSpliceSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpliceSettingDlg::IDD, pParent)
	, m_iPhyHorz(2)
	, m_iPhyVert(2)
	, m_iLogHorz(2)
	, m_iLogVert(2)
	, m_iSpaceH(0)
	, m_iSpaceV(0)
	, m_iScrPosX(0)
	, m_iScrPosY(0)
	, m_iScrWidth(0)
	, m_iScrHeight(0)
	, m_iBlendWidH(0)
	, m_iBlendWidV(0)
	, m_iTimingIndex(0)
{
	// 加载输出分辨率配置文件
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\OutputResolution.ini");
	m_resManager.SetConfigFilePath(strPath.GetBuffer(0));
	m_resManager.LoadFromFile(NULL);

	m_pLayout = NULL;
	m_pPage = NULL;

	m_eScrOpt = eScrSimplySetup;

	m_pScrInf = NULL;
	m_iScreens = 0;
	m_iCurselScreen = -1;
	m_bDataSaved = TRUE;

	m_pHorzBlend = NULL;
	m_pVertBlend = NULL;

	m_iHPos = 0;
	m_iVPos = 0;

	m_iMaxHScroll = m_iPhyHorz;
	m_iMaxVScroll = m_iPhyVert;

	m_icurselBlendH = 0;
	m_icurselBlendV = 0;
}

CSpliceSettingDlg::CSpliceSettingDlg(pSPLICELAYOUT pLayout, CDrawWndPage* pPage, CWnd* pParent)
	: CDialog(CSpliceSettingDlg::IDD, pParent)
	, m_iBlendWidH(0)
	, m_iBlendWidV(0)
	, m_iSpaceH(0)
	, m_iSpaceV(0)
{
	m_iPhyHorz = pLayout->iPhyHorz;
	m_iPhyVert = pLayout->iPhyVert;
	m_iLogHorz = pLayout->iLogHorz;
	m_iLogVert = pLayout->iLogVert;

	// 加载输出分辨率配置文件
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\OutputResolution.ini");
	m_resManager.SetConfigFilePath(strPath.GetBuffer(0));
	m_resManager.LoadFromFile(NULL);

	m_iTimingIndex = m_resManager.IsExist(pLayout->BkgTiming);
	if (m_iTimingIndex < 0) {
		pLayout->BkgTiming.m_Type = CUSTOM_RESOLUTION;
		m_iTimingIndex = m_resManager.AddResolution(FALSE, pLayout->BkgTiming);
	}

	m_pLayout = pLayout;
	m_pPage = pPage;

	m_eScrOpt = eScrSimplySetup;

	m_iScreens = m_iPhyHorz * m_iPhyVert;
	m_pScrInf = new SCREENMAP[m_iScreens];

	Rect rect;
	for (int i=0; i<m_iScreens; i++)
	{
		m_pScrInf[i].iScreenID = m_pPage->GetScreenObj(i)->GetID();
		m_pScrInf[i].iOutputCh = m_pPage->GetScreenObj(i)->GetOutPort();

		rect = m_pPage->GetScreenObj(i)->GetPhyCoords();
		m_pScrInf[i].rcScreenPos = CRect(rect.X, rect.Y, rect.GetRight(), rect.GetBottom());
	}

	m_iCurselScreen = -1;
	m_bDataSaved = TRUE;

	m_pHorzBlend = NULL;
	m_pVertBlend = NULL;

	m_iHPos = 0;
	m_iVPos = 0;

	m_iMaxHScroll = m_iPhyHorz;
	m_iMaxVScroll = m_iPhyVert;

	m_icurselBlendH = 0;
	m_icurselBlendV = 0;
}

CSpliceSettingDlg::~CSpliceSettingDlg()
{
	if (m_pScrInf)
	{
		SAFE_DELETE_AR(m_pScrInf);
	}

	if (m_pHorzBlend)
	{
		SAFE_DELETE_AR(m_pHorzBlend);
	}

	if (m_pVertBlend)
	{
		SAFE_DELETE_AR(m_pVertBlend);
	}
}

void CSpliceSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PHY_HORZ_SCREEN, m_EditPhyHorz);
	DDX_Control(pDX, IDC_EDIT_PHY_VERT_SCREEN, m_EditPhyVert);
	DDX_Control(pDX, IDC_EDIT_LOG_HORZ_SCREEN, m_EditLogHorz);
	DDX_Control(pDX, IDC_EDIT_LOG_VERT_SCREEN, m_EditLogVert);
	DDX_Control(pDX, IDC_SPIN_PHY_HORZ_SCREEN, m_spinPhyHorz);
	DDX_Control(pDX, IDC_SPIN_PHY_VERT_SCREEN, m_spinPhyVert);
	DDX_Control(pDX, IDC_SPIN_LOG_HORZ_SCREEN, m_spinLogHorz);
	DDX_Control(pDX, IDC_SPIN_LOG_VERT_SCREEN, m_spinLogVert);
	DDX_Text(pDX, IDC_EDIT_PHY_HORZ_SCREEN, m_iPhyHorz);
	DDX_Text(pDX, IDC_EDIT_PHY_VERT_SCREEN, m_iPhyVert);
	DDX_Text(pDX, IDC_EDIT_LOG_HORZ_SCREEN, m_iLogHorz);
	DDX_Text(pDX, IDC_EDIT_LOG_VERT_SCREEN, m_iLogVert);

	DDX_Control(pDX, IDC_EDIT_SCREEN_SPACE_HORZ, m_EditSpaceH);
	DDX_Control(pDX, IDC_EDIT_SCREEN_SPACE_VERT, m_EditSpaceV);
	DDX_Control(pDX, IDC_SPIN_SCREEN_SPACE_HORZ, m_spinSpaceH);
	DDX_Control(pDX, IDC_SPIN_SCREEN_SPACE_VERT, m_spinSpaceV);
	DDX_Text(pDX, IDC_EDIT_SCREEN_SPACE_HORZ, m_iSpaceH);
	DDX_Text(pDX, IDC_EDIT_SCREEN_SPACE_VERT, m_iSpaceV);

	DDX_Control(pDX, IDC_EDIT_BLEND_WID_HORZ, m_EditBlendWidH);
	DDX_Control(pDX, IDC_EDIT_BLEND_WID_VERT, m_EditBlendWidV);
	DDX_Control(pDX, IDC_SPIN_BLEND_WID_HORZ, m_spinBlendWidH);
	DDX_Control(pDX, IDC_SPIN_BLEND_WID_VERT, m_spinBlendWidV);
	DDX_Text(pDX, IDC_EDIT_BLEND_WID_HORZ, m_iBlendWidH);
	DDX_Text(pDX, IDC_EDIT_BLEND_WID_VERT, m_iBlendWidV);

	DDX_Control(pDX, IDC_EDIT_SCREEN_POS_X, m_EditScrPosX);
	DDX_Control(pDX, IDC_EDIT_SCREEN_POS_Y, m_EditScrPosY);
	DDX_Control(pDX, IDC_EDIT_SCREEN_WIDTH, m_EditScrWidth);
	DDX_Control(pDX, IDC_EDIT_SCREEN_HEIGHT, m_EditScrHeight);
	DDX_Control(pDX, IDC_SPIN_SCREEN_POS_X, m_spinScrPosX);
	DDX_Control(pDX, IDC_SPIN_SCREEN_POS_Y, m_spinScrPosY);
	DDX_Control(pDX, IDC_SPIN_SCREEN_WIDTH, m_spinScrWidth);
	DDX_Control(pDX, IDC_SPIN_SCREEN_HEIGHT, m_spinScrHeight);
	DDX_Text(pDX, IDC_EDIT_SCREEN_POS_X, m_iScrPosX);
	DDX_Text(pDX, IDC_EDIT_SCREEN_POS_Y, m_iScrPosY);
	DDX_Text(pDX, IDC_EDIT_SCREEN_WIDTH, m_iScrWidth);
	DDX_Text(pDX, IDC_EDIT_SCREEN_HEIGHT, m_iScrHeight);

	DDX_Control(pDX, IDC_SLIDER_SPLICE_TIMING, m_sliderTiming);
	DDX_Control(pDX, IDC_SCROLLBAR_SPLICE_HORZ, m_HScroll);
	DDX_Control(pDX, IDC_SCROLLBAR_SPLICE_VERT, m_VScroll);

	DDX_Control(pDX, IDC_RADIO_CUSTOMIZE, m_RadioCustomize);
	DDX_Control(pDX, IDC_RADIO_SIMPLY, m_RadioSimplySetup);
	DDX_Control(pDX, IDC_RADIO_COMPLEX, m_RadioComplexSetup);
	DDX_Control(pDX, IDC_LIST_SCREENS, m_ListScr);
}

BEGIN_MESSAGE_MAP(CSpliceSettingDlg, CDialog)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, SetTipText)
	ON_BN_CLICKED(IDOK, &CSpliceSettingDlg::OnOK)
	ON_BN_CLICKED(IDC_BTN_SPLICE_ADD, &CSpliceSettingDlg::OnBtnClickedAddTiming)
	ON_BN_CLICKED(IDC_BTN_SPLICE_DEL, &CSpliceSettingDlg::OnBtnClickedDelTiming)
	ON_BN_CLICKED(IDC_BTN_SPLICE_MOD, &CSpliceSettingDlg::OnBtnClickedModTiming)
	ON_BN_CLICKED(IDC_BTN_APPLY_SCREEN_SETTINGS, &CSpliceSettingDlg::OnBtnApplyScrSettings)
	ON_BN_CLICKED(IDC_BTN_UPDATE_GRIDLINE, &CSpliceSettingDlg::OnBnClickedBtnUpdateGridline)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_PHY_HORZ_SCREEN, IDC_EDIT_LOG_VERT_SCREEN, &CSpliceSettingDlg::OnRangeEnChangeEditCombineMode)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_SCREEN_SPACE_HORZ, IDC_EDIT_SCREEN_SPACE_VERT, &CSpliceSettingDlg::OnRangeEnChangeEditScrSpace)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_SCREEN_POS_X, IDC_EDIT_SCREEN_HEIGHT, &CSpliceSettingDlg::OnRangeEnChangeEditScrPosition)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_BLEND_WID_HORZ, IDC_EDIT_BLEND_WID_VERT, &CSpliceSettingDlg::OnRangeEnChangeEditScrBlend)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_SIMPLY, IDC_RADIO_CUSTOMIZE, &CSpliceSettingDlg::OnRangeRadioScreenOption)
	ON_LBN_SELCHANGE(IDC_LIST_SCREENS, &CSpliceSettingDlg::OnLbnChangeListScreen)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CSpliceSettingDlg 消息处理程序

BOOL CSpliceSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	LoadState();

	InitImageList();
	ResetListBox();

	// 微调按钮的范围
	m_spinPhyHorz.SetRange(1, 100);
	m_spinPhyVert.SetRange(1, 100);
	m_spinLogHorz.SetRange(1, 100);
	m_spinLogVert.SetRange(1, 100);
	m_spinSpaceH.SetRange32(-m_pLayout->BkgTiming.m_HActive, 65535);
	m_spinSpaceV.SetRange32(-m_pLayout->BkgTiming.m_VActive, 65535);
	m_spinScrPosX.SetRange32(0, 500000);
	m_spinScrPosY.SetRange32(0, 500000);
	m_spinScrWidth.SetRange32(1, 500000);
	m_spinScrHeight.SetRange32(1, 500000);
	m_spinBlendWidH.SetRange(0, m_pLayout->BkgTiming.m_HActive);
	m_spinBlendWidV.SetRange(0, m_pLayout->BkgTiming.m_VActive);

	//分辨率选择滑动条
	m_sliderTiming.SetRange(0, m_resManager.GetResolutionCount()-1);
	m_sliderTiming.SetTic(1);
	m_sliderTiming.SetTicFreq(1);
	m_sliderTiming.SetPos(m_iTimingIndex);

	//提示条
	EnableToolTips(TRUE);
	m_tips.Create(this);
	m_tips.AddTool(&m_sliderTiming, _T("拖动选择分辨率"));
	m_tips.SetTipBkColor(RGB(200, 200, 255));
	m_tips.SetTipTextColor(RGB(100, 150, 100));

	// 显示当前分辨率信息
	ShowSelectedTiming(m_iTimingIndex);

	if (m_eScrOpt == eScrSimplySetup) {
		m_RadioSimplySetup.SetCheck(1);
		OnRadioSimplySetup();
	}
	else if (m_eScrOpt == eScrComplexSetup) {
		m_RadioComplexSetup.SetCheck(1);
		OnRadioComplexSetup();
	}
	else if (m_eScrOpt == eScrCustomized) {
		m_RadioCustomize.SetCheck(1);
		OnRadioCustomize();
	}

	// 预装载位图
	VERIFY(m_bmpMonitor.LoadBitmap(IDB_BMP_MONITOR32));
	m_bmpMonitor.GetBitmap(&m_bmpInfo);

	// 设置绘图时每个屏幕的尺寸度量值
	SetScreenItemMargin();

	// 设置滚动条的范围和位置
	ResetScrollRange();

	return TRUE;
}

void CSpliceSettingDlg::ResetScrollRange()
{
	m_iMaxHScroll = m_iPhyHorz - GetMaxVisibleScreens().key;
	m_iMaxVScroll = m_iPhyVert - GetMaxVisibleScreens().val;

	m_HScroll.SetScrollRange(0, m_iMaxHScroll, TRUE);
	m_VScroll.SetScrollRange(0, m_iMaxVScroll, TRUE);

	m_iHPos = 0;
	m_iVPos = 0;

	m_HScroll.SetScrollPos(m_iHPos);
	m_VScroll.SetScrollPos(m_iVPos);

	m_HScroll.EnableWindow(m_iMaxHScroll>0);
	m_VScroll.EnableWindow(m_iMaxVScroll>0);
}

void CSpliceSettingDlg::SetScreenItemMargin()
{
	m_ScrItemMargin.nItemWidth = m_bmpInfo.bmWidth + 12;
	m_ScrItemMargin.nItemHeight = m_bmpInfo.bmHeight + 15;
	m_ScrItemMargin.nItemSpace = 15;
}

INT_PAIR CSpliceSettingDlg::GetMaxVisibleScreens()
{
	CRect rcPic;
	GetDlgItem(IDC_PIC_SCREEN_COMBINE)->GetClientRect(&rcPic);
	rcPic.InflateRect(-2,-2);

	INT_PAIR maxinfo;

	maxinfo.key = min(rcPic.Width()/(m_ScrItemMargin.nItemWidth+m_ScrItemMargin.nItemSpace) +1 , m_iPhyHorz);   //界面最多可显示的水平屏幕数 额外的15个像素是为融合带做准备的
	maxinfo.val = min(rcPic.Height()/(m_ScrItemMargin.nItemHeight+m_ScrItemMargin.nItemSpace) +1, m_iPhyVert);  //界面最多可显示的垂直屏幕数

	return maxinfo;
}

void CSpliceSettingDlg::InitImageList()
{
	m_imglist.Create(16, 16, ILC_COLOR32|ILC_MASK, 1, 1);
	HICON icon;
	icon = AfxGetApp()->LoadIcon(IDI_ICON_SIGNAL);
	m_imglist.Add(icon);

	m_ListScr.SetImageList(&m_imglist);
}

void CSpliceSettingDlg::ResetListBox()
{
	CRedrawLock lock(&m_ListScr);

	m_ListScr.ResetContent();

	int iScreens = m_iPhyHorz * m_iPhyVert;

	CString strScrName;
	for (int i=0; i<iScreens; i++)
	{
		strScrName.Format(_T("屏%d"), i+1);

		m_ListScr.AddString(strScrName);
		m_ListScr.SetItemImage(i, 0);
	}

	if (iScreens > 0) {
		m_ListScr.SetCurSel(0);
		m_iCurselScreen = 0;
		GetScreenInfo(0);
	}
}

BOOL CSpliceSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd* pWndFrom = FromHandle(pMsg->hwnd);

			if (pWndFrom == GetDlgItem(IDC_EDIT_SCREEN_SPACE_HORZ)
			 || pWndFrom == GetDlgItem(IDC_EDIT_SCREEN_SPACE_VERT))
			{
				AutoScreenInfo();
			}
			else if (pWndFrom == GetDlgItem(IDC_EDIT_BLEND_WID_HORZ)
				  || pWndFrom == GetDlgItem(IDC_EDIT_BLEND_WID_VERT))
			{
				SetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
			}
			else if (pWndFrom == GetDlgItem(IDC_EDIT_SCREEN_POS_X)
				|| pWndFrom == GetDlgItem(IDC_EDIT_SCREEN_POS_Y)
				|| pWndFrom == GetDlgItem(IDC_EDIT_SCREEN_WIDTH)
				|| pWndFrom == GetDlgItem(IDC_EDIT_SCREEN_HEIGHT))
			{
				SetScreenInfo(m_iCurselScreen);
			}

			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CSpliceSettingDlg::SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pRes)  
{  
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;      
	UINT nID =pTTTStruct->idFrom;   
	if (pTTT->uFlags & TTF_IDISHWND)  
	{  
		nID = ::GetDlgCtrlID((HWND)nID);  
		switch(nID)  
		{  
		case(IDC_SLIDER_SPLICE_TIMING):  
			{  
				CString str;
				str.Format("%s", m_resManager.GetResolutionName(m_iTimingIndex));
				strncpy_s(pTTT->lpszText, 80, str.GetBuffer(), str.GetLength());
				return TRUE;  
			}
			break; 
		default:break;
		}  
	}  
	return FALSE;  
}  

void CSpliceSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->IsKindOf(RUNTIME_CLASS(CSliderCtrl)))
	{
		OnScrollTiming(nSBCode, nPos, pScrollBar);
	}
	else if (pScrollBar->IsKindOf(RUNTIME_CLASS(CScrollBar)))
	{
		OnScrollScreen(nSBCode, nPos, pScrollBar, SB_HORZ);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSpliceSettingDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->IsKindOf(RUNTIME_CLASS(CScrollBar)))
	{
		OnScrollScreen(nSBCode, nPos, pScrollBar, SB_VERT);
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CSpliceSettingDlg::OnScrollTiming(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Get the minimum and maximum scroll-bar positions.
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
	int minpos;
	int maxpos;
	pSlider->GetRange(minpos, maxpos);

	// Get the current position of scroll box.
	int curpos = m_iTimingIndex;

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;

	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;

	case SB_LINELEFT:      // Scroll left.
		curpos--;
		if (curpos < minpos) {curpos = minpos;}
		pSlider->SetPos(curpos);
		break;

	case SB_LINERIGHT:   // Scroll right.
		curpos++;
		if (curpos > maxpos) {curpos = maxpos;}
		pSlider->SetPos(curpos);
		break;

	case SB_PAGELEFT:    // Scroll one page left.
		curpos--;
		if (curpos < minpos) {curpos = minpos;}
		pSlider->SetPos(curpos);
		break;

	case SB_PAGERIGHT:      // Scroll one page right.
		curpos++;
		if (curpos > maxpos) {curpos = maxpos;}
		pSlider->SetPos(curpos);
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}

	// Set the new position of the thumb (scroll box).
	m_iTimingIndex = curpos;
	ShowSelectedTiming(m_iTimingIndex);

	// Reset screen positions
	if (m_eScrOpt == eScrComplexSetup)
		SetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
	else
		AutoScreenInfo();
}

void CSpliceSettingDlg::OnScrollScreen(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar, UINT nSBType)
{
	// Get the minimum and maximum scroll-bar positions.
	int minpos = 0;
	int maxpos = 0;
	pScrollBar->GetScrollRange(&minpos, &maxpos);

	// Get the current position of scroll box.
	int curpos = 0;
	if (nSBType == SB_HORZ)
		curpos = m_iHPos;
	else
		curpos = m_iVPos;

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;

	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;

	case SB_LINELEFT:      // Scroll left.
		curpos--;
		if (curpos < minpos) {curpos = minpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_LINERIGHT:   // Scroll right.
		curpos++;
		if (curpos > maxpos) {curpos = maxpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_PAGELEFT:    // Scroll one page left.
		curpos--;
		if (curpos < minpos) {curpos = minpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_PAGERIGHT:      // Scroll one page right.
		curpos++;
		if (curpos > maxpos) {curpos = maxpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		pScrollBar->SetScrollPos(curpos);
		break;
	}

	// Set the new position of the thumb (scroll box).
	if (nSBType == SB_HORZ)
		m_iHPos = curpos;
	else
		m_iVPos = curpos;

	RepaintPic();
}

void CSpliceSettingDlg::ShowSelectedTiming(int nTimingIndex)
{
	SetDlgItemText(IDC_LABEL_SPLICE_TIMING, m_resManager.GetResolutionName(nTimingIndex));

	TIMING st = {0};
	st = m_resManager.GetResolutionDetail(nTimingIndex);

	UpdateSpinRange(st);

	GetDlgItem(IDC_BTN_SPLICE_DEL)->EnableWindow(st.m_Type == CUSTOM_RESOLUTION);
}

void CSpliceSettingDlg::UpdateSpinRange(const TIMING &st)
{
	m_spinSpaceH.SetRange32(-st.m_HActive, 65535);
	m_spinSpaceV.SetRange32(-st.m_VActive, 65535);
}

void CSpliceSettingDlg::OnBtnClickedAddTiming()
{
	CResolutionEditDlg DLG(m_resManager.GetResolutionDetail(m_iTimingIndex), ADD_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		AddNewResolution(DLG.m_DetailTiming);
	}
}

void CSpliceSettingDlg::OnBtnClickedDelTiming()
{
	CResolutionEditDlg DLG(m_resManager.GetResolutionDetail(m_iTimingIndex), DEL_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		DeleteResolution(DLG.m_DetailTiming);
	}
}

void CSpliceSettingDlg::OnBtnClickedModTiming()
{
	CResolutionEditDlg DLG(m_resManager.GetResolutionDetail(m_iTimingIndex), EDIT_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		ModifyResolution(DLG.m_DetailTiming);
	}
}

void CSpliceSettingDlg::AddNewResolution(const TIMING &timing)
{
	if (m_resManager.AddResolution(FALSE, const_cast<TIMING&>(timing)) == INVALID_VALUE)
		return;

	//由于添加了新的分辨率，故而需要更新滑动条的位置
	int nResIndex = m_resManager.GetResolutionIndex(timing);
	if (nResIndex != INVALID_VALUE)
		m_iTimingIndex = nResIndex;
	else
		m_iTimingIndex = 0;

	m_sliderTiming.SetRange(0, m_resManager.GetResolutionCount()-1, TRUE);
	m_sliderTiming.SetPos(m_iTimingIndex);	

	ShowSelectedTiming(m_iTimingIndex);
}

void CSpliceSettingDlg::ModifyResolution(const TIMING &timing)
{
	m_resManager.ModifyResolution(m_iTimingIndex, const_cast<TIMING&>(timing));
}

void CSpliceSettingDlg::DeleteResolution(const TIMING &timing)
{
	m_resManager.RemoveResolution(timing);

	//由于删除了分辨率，故而需要更新滑动条的位置
	TIMING st = {0};

	// 删除分辨率后自动显示前一个分辨率
	int nIndex = m_iTimingIndex;
	if (nIndex > 0)
		nIndex--;
	else
		nIndex = 0;

	st = m_resManager.GetResolutionDetail(nIndex);

	int nResIndex = m_resManager.GetResolutionIndex(st);
	if (nResIndex != INVALID_VALUE)
		m_iTimingIndex = nResIndex;
	else
		m_iTimingIndex = 0;

	m_sliderTiming.SetRange(0, m_resManager.GetResolutionCount()-1, TRUE);	
	m_sliderTiming.SetPos(m_iTimingIndex);

	ShowSelectedTiming(m_iTimingIndex);	
}


void CSpliceSettingDlg::OnOK()
{
	UpdateData();

	TIMING st = m_resManager.GetResolutionDetail(m_iTimingIndex);
	if (!st.IsValid())
		return;

	if (m_iPhyHorz < 1) m_iPhyHorz = 1;
	if (m_iPhyVert < 1) m_iPhyVert = 1;
	if (m_iLogHorz < 1) m_iLogHorz = 1;
	if (m_iLogVert < 1) m_iLogVert = 1;
	if (m_iSpaceH  < -st.m_HActive) m_iSpaceH = -st.m_HActive;
	//if (m_iSpaceV  > 100)  m_iSpaceV = 100;

	if (m_pLayout == NULL)
	{
		AfxMessageBox(_T("拼接属性：无效的参数指针！"));
		return CDialog::OnOK();
	}

	if (m_iPhyHorz*m_iPhyVert > GetOutputManager()->GetCount())
	{
		CString str;
		str.Format(_T("拼接屏数[%d] > 总的输出口数[%d] !"), m_iPhyVert*m_iPhyHorz, GetOutputManager()->GetCount());
		AfxMessageBox(str);
		return;
	}

	if (!m_bDataSaved)
	{
		if (m_eScrOpt == eScrCustomized) SetScreenInfo(m_iCurselScreen);
		else if (m_eScrOpt == eScrSimplySetup) AutoScreenInfo();
		else if (m_eScrOpt == eScrComplexSetup) SetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
		m_bDataSaved = TRUE;
	}

	CRect rcBkg = CDrawWndPage::GetBkgSizeAccordingtoScrInf(m_pScrInf, m_iScreens, FALSE);
	SPLICELAYOUT layout;
	layout.iPhyHorz = m_iPhyHorz;
	layout.iPhyVert = m_iPhyVert;
	layout.iLogHorz = m_iLogHorz;
	layout.iLogVert = m_iLogVert;
	layout.BkgTiming = st;
	//layout.iScnSpaceH = m_iSpaceH;
	//layout.iScnSpaceV = m_iSpaceV;
	layout.iPhyHorzPixel = rcBkg.Width();
	layout.iPhyVertPixel = rcBkg.Height();

	if (GetCmdManager()->CmdWCN1(SYSBOARD_PORT, eBoardTypeCtrl, layout) == CMD_FAILURE)
	{
#ifndef _DEBUG
		if (MessageBox(_T("拼接设置失败，是否重试？"), _T("拼接设置"), MB_YESNO|MB_ICONERROR) == IDYES)
			return;
		else
			return CDialog::OnOK();
#endif
	}

	// 更新拼接配置
	memcpy(m_pLayout, &layout, sizeof(SPLICELAYOUT));

	CDrawWndObj::Shut(0);
	m_pPage->RemoveAllWnd();
	m_pPage->SetLayoutInfo(layout);

	m_pPage->ResetScreens(m_pScrInf, m_iScreens, FALSE);

	CGridView* pView = (CGridView*)m_pParentWnd;
	pView->SetProperZoom();

	SaveState();

	CDialog::OnOK();
}

void CSpliceSettingDlg::OnRangeEnChangeEditCombineMode(UINT nID)
{
	if (m_EditPhyHorz.GetSafeHwnd())
	{
		int iHorzP = m_iPhyHorz;
		int iVertP = m_iPhyVert;
		int iHorzL = m_iLogHorz;
		int iVertL = m_iLogVert;

		UpdateData();

		// 数据没变化
		if ( iHorzP == m_iPhyHorz 
		  && iVertP == m_iPhyVert
		  && iHorzL == m_iLogHorz
		  && iVertL == m_iLogVert )
		  return;

		if (m_iPhyHorz < 1) m_iPhyHorz = 1;
		if (m_iPhyVert < 1) m_iPhyVert = 1;
		if (m_iLogHorz < 1) m_iLogHorz = 1;
		if (m_iLogVert < 1) m_iLogVert = 1;

		UpdateData(FALSE);

		if (nID == IDC_EDIT_PHY_HORZ_SCREEN
		 || nID == IDC_EDIT_PHY_VERT_SCREEN)
		{
			// 重新设置列表框中的屏幕项
			ResetListBox();
			// 重新设置滚动条的范围
			ResetScrollRange();

			m_iScreens = m_iPhyHorz * m_iPhyVert;

			// 重置屏幕信息数组
			if (m_pScrInf) {
				delete [] m_pScrInf;
				m_pScrInf = new SCREENMAP[m_iScreens];
			}

			// 重置融合信息数组
			if (m_pHorzBlend) {
				SAFE_DELETE_AR(m_pHorzBlend);
			}
			if (m_pVertBlend) {
				SAFE_DELETE_AR(m_pVertBlend);
			}

			if (m_eScrOpt == eScrComplexSetup) {
				if (m_iPhyHorz > 1) {
					m_pHorzBlend = new int[m_iPhyHorz+1];
					memset(m_pHorzBlend, 0, sizeof(int)*(m_iPhyHorz+1));
				}
				if (m_iPhyVert > 1) {
					m_pVertBlend = new int[m_iPhyVert+1];
					memset(m_pVertBlend, 0, sizeof(int)*(m_iPhyVert+1));
				}

				m_icurselBlendH = 0;
				m_icurselBlendV = 0;

				GetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
			}

			// 计算出默认的屏幕信息
			AutoScreenInfo();
		}

		// 更新绘图
		RepaintPic();
	}
}

void CSpliceSettingDlg::OnRangeEnChangeEditScrSpace(UINT nID)
{
	if (m_EditSpaceH.GetSafeHwnd())
	{
		int iSpaceH = m_iSpaceH;
		int iSpaceV = m_iSpaceV;

		UpdateData();

		// 数据没变化
		if (m_iSpaceH == iSpaceH && m_iSpaceV == iSpaceV)
			return;

		TIMING st = m_resManager.GetResolutionDetail(m_iTimingIndex);
		if (!st.IsValid())
			return;

		if (m_iSpaceH < -st.m_HActive) m_iSpaceH = -st.m_HActive;
		if (m_iSpaceV < -st.m_VActive) m_iSpaceV = -st.m_VActive;

		m_bDataSaved = FALSE;
	}
}

void CSpliceSettingDlg::OnRangeEnChangeEditScrPosition(UINT nID)
{
	if (m_EditScrPosX.GetSafeHwnd())
	{
		int iPosx = m_iScrPosX;
		int iPosy = m_iScrPosY;
		int iWid  = m_iScrWidth;
		int iHei  = m_iScrHeight;

		UpdateData();

		// 数值是否有变化
		if ( iPosx == m_iScrPosX 
			&& iPosy == m_iScrPosY
			&& iWid  == m_iScrWidth
			&& iHei  == m_iScrHeight )
			return;

		m_bDataSaved = FALSE;
	}
}

void CSpliceSettingDlg::OnRangeEnChangeEditScrBlend(UINT nID)
{
	if (m_EditBlendWidH.GetSafeHwnd())
	{
		int iBlendWidH = m_iBlendWidH;
		int iBlendWidV = m_iBlendWidV;

		UpdateData();

		if ( iBlendWidH == m_iBlendWidH 
		  && iBlendWidV == m_iBlendWidV )
		  return;

		m_bDataSaved = FALSE;
	}
}

void CSpliceSettingDlg::RepaintPic()
{
	CRect rc;
	GetDlgItem(IDC_PIC_SCREEN_COMBINE)->GetWindowRect(rc);
	ScreenToClient(&rc);
	rc.InflateRect(-2, -2);
	InvalidateRect(rc);
}

void CSpliceSettingDlg::OnPaint()
{
	CPaintDC dc(this);

	CWnd* pWnd = GetDlgItem(IDC_PIC_SCREEN_COMBINE);
	CRect rcPic;
	pWnd->GetClientRect(rcPic);

	CDC* pDC = pWnd->GetDC();

	CDC* pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap bkBmp, *pOldbkbmp;
	bkBmp.CreateCompatibleBitmap(pDC, rcPic.Width(), rcPic.Height());
	pOldbkbmp = pMemDC->SelectObject(&bkBmp);

	DrawPicture(pMemDC);

	pDC->BitBlt(0,0,rcPic.Width(),rcPic.Height(), pMemDC, 0, 0, SRCCOPY);

	pMemDC->SelectObject(pOldbkbmp);

	ReleaseDC(pMemDC);
	SAFE_DELETE(pMemDC);

	ReleaseDC(pDC);
}

void CSpliceSettingDlg::DrawPicture(CDC* pDC)
{
	ASSERT(pDC);

	Graphics gp(pDC->m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighQuality);

	CWnd* pWnd = GetDlgItem(IDC_PIC_SCREEN_COMBINE);
	CRect rcPic;
	pWnd->GetClientRect(rcPic);

	SolidBrush whitebr(Color(255, 255, 255, 255));
	gp.FillRectangle(&whitebr, rcPic.left, rcPic.top, rcPic.Width(), rcPic.Height());
	rcPic.InflateRect(-2, -2);

	const int iItemWidth  = m_ScrItemMargin.nItemWidth;  //绘制一个屏幕项的宽度 
	const int iItemHeight = m_ScrItemMargin.nItemHeight; //绘制一个屏幕项的高度

	const int iSpace = m_ScrItemMargin.nItemSpace;

	int iMaxVisibleHItem = min(rcPic.Width()/(iItemWidth+iSpace) +1 , m_iPhyHorz);   //界面最多可显示的水平屏幕数 外的15个像素是为融合带做准备的
	int iMaxVisibleVItem = min(rcPic.Height()/(iItemHeight+iSpace) +1, m_iPhyVert);  //界面最多可显示的垂直屏幕数

	CRect rcItem;  //屏幕位置
	int iScrIndex; //屏幕号
	CString strIndex;

	// 准备一个选择标示画刷
	LinearGradientBrush StateBr(Point(0, 0), Point(0, iItemHeight+iSpace), Color(220, 180, 195, 252), Color(220, 153, 179, 252));
	Pen StatePen(Color(255, 125, 162, 206), 1);

	CPoint ptLocate; // 定位点
	CSize textExt;   // 文本在屏幕上的尺寸

	// 准备一个位图画刷
	CDC* pBmpDC = new CDC;
	pBmpDC->CreateCompatibleDC(pDC);

	CBitmap bmpMask;
	bmpMask.LoadBitmap(IDB_BMP_MONITOR_MASK);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(113, 164, 125));

	for (int h=0; h<iMaxVisibleHItem; h++)
	{
		for (int v=0; v<iMaxVisibleVItem; v++)
		{
			iScrIndex = m_iHPos + (m_iVPos+v)*m_iPhyHorz + h;
			strIndex.Format(_T("%d"), iScrIndex+1);
			textExt = pDC->GetTextExtent(strIndex);

			rcItem.left   = rcPic.left + h*iItemWidth + h*iSpace;
			rcItem.right  = rcItem.left + iItemWidth;
			rcItem.top    = rcPic.top + v*iItemHeight + v*iSpace;
			rcItem.bottom = rcItem.top + iItemHeight;

			// 当前选中屏幕
			if (iScrIndex == m_iCurselScreen)
			{
				gp.DrawRectangle(&StatePen, rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());
				StatePen.SetColor(Color(255, 228, 240, 253));
				gp.DrawRectangle(&StatePen, rcItem.left+1, rcItem.top+1, rcItem.Width()-2, rcItem.Height()-2);

				gp.FillRectangle(&StateBr, rcItem.left+1, rcItem.top+1, rcItem.Width()-2, rcItem.Height()-2);

				pBmpDC->SelectObject(&bmpMask);
			}
			else
				pBmpDC->SelectObject(&m_bmpMonitor);

			// 画屏幕位图
			ptLocate.x = rcItem.left + (rcItem.Width()-m_bmpInfo.bmWidth)/2;
			ptLocate.y = rcItem.top + 2;
			pDC->BitBlt(ptLocate.x, ptLocate.y, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, pBmpDC, 0, 0, SRCCOPY);

			// 画每个屏幕的屏幕号
			ptLocate.x = rcItem.left + (rcItem.Width()-textExt.cx)/2;
			ptLocate.y = ptLocate.y + m_bmpInfo.bmHeight +1;
			pDC->TextOut(ptLocate.x, ptLocate.y, strIndex);
		}
	}

	if (m_eScrOpt == eScrComplexSetup)
	{
		// 为融合带准备的绘制画刷
		LinearGradientBrush HorzBlendBr(Point(iItemWidth, 0), Point(iItemWidth + iSpace, 0), Color(255, 128, 128, 128), Color(255, 192, 192, 192));
		LinearGradientBrush HorzSelectBr(Point(iItemWidth, 0), Point(iItemWidth + iSpace, 0), Color(255, 128, 128, 128), Color(255, 192, 192, 192));
		LinearGradientBrush VertBlendBr(Point(0, iItemHeight), Point(0, iItemHeight + iSpace), Color(255, 128, 128, 128), Color(255, 192, 192, 192));
		LinearGradientBrush VertSelectBr(Point(0, iItemHeight), Point(0, iItemHeight + iSpace), Color(255, 128, 128, 128), Color(255, 192, 192, 192));

		Color colors[] = {
			Color(200, 230, 255, 245),
			Color(255, 200, 225, 215),
			Color(200, 230, 255, 245),
		};

		REAL positions[] = {
			0.0f,
			0.5f,
			1.0f
		};

		HorzBlendBr.SetInterpolationColors(colors, positions, 3);
		VertBlendBr.SetInterpolationColors(colors, positions, 3);

		Color selcolors[] = {
			Color(200, 180, 215, 195),
			Color(255, 150, 185, 165),
			Color(200, 180, 215, 195),
		};

		HorzSelectBr.SetInterpolationColors(selcolors, positions, 3);
		VertSelectBr.SetInterpolationColors(selcolors, positions, 3);

		CRect rcHorzBlend, rcVertBlend; // 融合带的位置

		int iselBlend; // 当前选择的融合带

		for (int h=0; h<iMaxVisibleHItem-1; h++)
		{
			iselBlend = m_iHPos + h;

			rcHorzBlend.left   = rcPic.left + h*iItemWidth + h*iSpace + iItemWidth;
			rcHorzBlend.right  = rcHorzBlend.left + iSpace;
			rcHorzBlend.top    = rcPic.top;
			rcHorzBlend.bottom = rcPic.bottom;

			if (iselBlend == m_icurselBlendH)
				gp.FillRectangle(&HorzSelectBr, rcHorzBlend.left, rcHorzBlend.top, rcHorzBlend.Width(), rcHorzBlend.Height());
			else
				gp.FillRectangle(&HorzBlendBr, rcHorzBlend.left, rcHorzBlend.top, rcHorzBlend.Width(), rcHorzBlend.Height());
		}

		for (int v=0; v<iMaxVisibleVItem-1; v++)
		{
			iselBlend = m_iVPos + v;

			rcVertBlend.left   = rcPic.left;
			rcVertBlend.right  = rcPic.right;
			rcVertBlend.top    = rcPic.top + v*iItemHeight + v*iSpace + iItemHeight;
			rcVertBlend.bottom = rcVertBlend.top + iSpace;

			if (iselBlend == m_icurselBlendV)
				gp.FillRectangle(&VertSelectBr, rcVertBlend.left, rcVertBlend.top, rcVertBlend.Width(), rcVertBlend.Height());
			else
				gp.FillRectangle(&VertBlendBr, rcVertBlend.left, rcVertBlend.top, rcVertBlend.Width(), rcVertBlend.Height());
		}
	}

	
	ReleaseDC(pBmpDC);
	SAFE_DELETE(pBmpDC);
}

void CSpliceSettingDlg::OnBnClickedBtnUpdateGridline()
{
	UpdateData(TRUE);

	if (m_iLogHorz < 1) m_iLogHorz = 1;
	if (m_iLogVert < 1) m_iLogVert = 1;

	CGridView* pView = (CGridView*)m_pParentWnd;
	if (pView->IsKindOf(RUNTIME_CLASS(CGridView)))
	{
		pView->UpdateAligningLine(m_iLogHorz, m_iLogVert);
	}

	// 更新配置文件
	CSysConfManager SysConf;
	// 对齐线
	m_iLogHorz = SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryHorzAligningLine"), m_iLogHorz);
	m_iLogVert = SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryVertAligningLine"), m_iLogVert);
}

void CSpliceSettingDlg::OnRangeRadioScreenOption(UINT nID)
{
	if (nID == IDC_RADIO_CUSTOMIZE) {
		OnRadioCustomize();
	}
	else if (nID == IDC_RADIO_SIMPLY) {
		OnRadioSimplySetup();
	}
	else if (nID == IDC_RADIO_COMPLEX) {
		OnRadioComplexSetup();
	}
	else return;

	GetScreenInfo(m_iCurselScreen);
}

void CSpliceSettingDlg::OnRadioCustomize()
{
	m_eScrOpt = eScrCustomized;

	EnableControls();	
}

void CSpliceSettingDlg::OnRadioSimplySetup()
{
	m_eScrOpt = eScrSimplySetup;

	EnableControls();
}

void CSpliceSettingDlg::OnRadioComplexSetup()
{
	m_eScrOpt = eScrComplexSetup;

	if (m_pHorzBlend == NULL)
	{
		m_pHorzBlend = new int[m_iPhyHorz+1];
		memset(m_pHorzBlend, 0, sizeof(int)*(m_iPhyHorz+1));
		m_icurselBlendH = 0;
	}

	if (m_pVertBlend == NULL)
	{
		m_pVertBlend = new int[m_iPhyVert+1];
		memset(m_pVertBlend, 0, sizeof(int)*(m_iPhyVert+1));
		m_icurselBlendV = 0;
	}

	ASSERT(m_pHorzBlend);
	ASSERT(m_pVertBlend);

	EnableControls();

	GetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
}

void CSpliceSettingDlg::EnableControls()
{
	GetDlgItem(IDC_EDIT_SCREEN_POS_X)->EnableWindow(m_eScrOpt==eScrCustomized);
	GetDlgItem(IDC_EDIT_SCREEN_POS_Y)->EnableWindow(m_eScrOpt==eScrCustomized);
	GetDlgItem(IDC_EDIT_SCREEN_WIDTH)->EnableWindow(m_eScrOpt==eScrCustomized);
	GetDlgItem(IDC_EDIT_SCREEN_HEIGHT)->EnableWindow(m_eScrOpt==eScrCustomized);

	GetDlgItem(IDC_EDIT_SCREEN_SPACE_HORZ)->EnableWindow(m_eScrOpt==eScrSimplySetup);
	GetDlgItem(IDC_EDIT_SCREEN_SPACE_VERT)->EnableWindow(m_eScrOpt==eScrSimplySetup);

	GetDlgItem(IDC_EDIT_BLEND_WID_HORZ)->EnableWindow(m_eScrOpt==eScrComplexSetup);
	GetDlgItem(IDC_EDIT_BLEND_WID_VERT)->EnableWindow(m_eScrOpt==eScrComplexSetup);
}

void CSpliceSettingDlg::OnLbnChangeListScreen()
{
	RemindtoApplySetting();

	int nCurSel = m_ListScr.GetCurSel();

	if (nCurSel == LB_ERR)
		return;

	m_iCurselScreen = nCurSel;

	GetScreenInfo(nCurSel);

	RepaintPic();
}

void CSpliceSettingDlg::RemindtoApplySetting()
{
	if (!m_bDataSaved && m_iCurselScreen != -1)
	{
		if (AfxMessageBox(_T("你有未应用的设置，是否应用？[YES] 应用; [NO] 放弃;"), MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			if (m_eScrOpt == eScrCustomized) SetScreenInfo(m_iCurselScreen);
			else if (m_eScrOpt == eScrSimplySetup) AutoScreenInfo();
			else if (m_eScrOpt == eScrComplexSetup) SetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);	
		}

		m_bDataSaved = TRUE;
	}
}

void CSpliceSettingDlg::GetScreenInfo(int iScreen)
{
	ASSERT(iScreen >= 0 && iScreen < m_iScreens);

	m_iScrPosX   = m_pScrInf[iScreen].rcScreenPos.left;
	m_iScrPosY   = m_pScrInf[iScreen].rcScreenPos.top;
	m_iScrWidth  = m_pScrInf[iScreen].rcScreenPos.Width();
	m_iScrHeight = m_pScrInf[iScreen].rcScreenPos.Height();

	UpdateData(FALSE);
}

void CSpliceSettingDlg::SetScreenInfo(int iScreen)
{
	ASSERT(iScreen >= 0 && iScreen < m_iScreens);

	m_pScrInf[iScreen].rcScreenPos = CRect(m_iScrPosX, m_iScrPosY, m_iScrPosX+m_iScrWidth, m_iScrPosY+m_iScrHeight);
}

void CSpliceSettingDlg::AutoScreenInfo()
{
	if (!m_pScrInf)
		return;

	ASSERT( m_iScreens == (m_iPhyHorz * m_iPhyVert) );

	memset(m_pScrInf, 0, m_iPhyHorz*m_iPhyVert*sizeof(SCREENMAP));

	TIMING st = m_resManager.GetResolutionDetail(m_iTimingIndex);
	if (!st.IsValid())
		return;

	int iPhyWid = st.m_HActive;
	int iPhyHei = st.m_VActive;

	int iSpaH, iSpaV;
	if (m_eScrOpt == eScrSimplySetup) {
		iSpaH = m_iSpaceH;
		iSpaV = m_iSpaceV;
	}
	else
		iSpaH = iSpaV = 0;

	int   nIndex;
	CRect rcPhy;

	for (int v=0; v<m_iPhyVert; v++)
		for (int h=0; h<m_iPhyHorz; h++)
		{
			nIndex = v*m_iPhyHorz + h;

			m_pScrInf[nIndex].iOutputCh = GetOutputManager()->GetCmdPort(nIndex, FALSE);
			m_pScrInf[nIndex].iScreenID = nIndex+1;

			rcPhy.left   = iPhyWid*h + h*iSpaH;
			rcPhy.right  = rcPhy.left + iPhyWid;
			rcPhy.top    = iPhyHei*v + v*iSpaV;
			rcPhy.bottom = rcPhy.top + iPhyHei;

			m_pScrInf[nIndex].rcScreenPos = rcPhy;
		}
}

void CSpliceSettingDlg::GetBlendInfo( int icurselBlendH, int icurselBlendV )
{
	// 显示可以编辑的融合带，隐藏不可以设置的融合带控件
	EnableBlendEdit(icurselBlendH, icurselBlendV);

	// 显示相应屏幕的融合带值
	if (m_pHorzBlend) m_iBlendWidH = m_pHorzBlend[icurselBlendH];
	if (m_pVertBlend) m_iBlendWidV = m_pVertBlend[icurselBlendV];	

	UpdateData(FALSE);
}

void CSpliceSettingDlg::SetBlendInfo(int icurselBlendH, int icurselBlendV)
{
	UpdateData(TRUE);

	if (m_pHorzBlend) m_pHorzBlend[icurselBlendH] = m_iBlendWidH;
	if (m_pVertBlend) m_pVertBlend[icurselBlendV] = m_iBlendWidV;

	// 根据融合带计算各屏幕的坐标
	CalcScreenPosAccordingtoBlendInfo();
}

void CSpliceSettingDlg::CalcScreenPosAccordingtoBlendInfo()
{
	ASSERT(m_pScrInf);
	memset(m_pScrInf, 0, m_iPhyHorz*m_iPhyVert*sizeof(SCREENMAP));

	TIMING st = m_resManager.GetResolutionDetail(m_iTimingIndex);
	if (!st.IsValid())
		return;

	int iPhyWid = st.m_HActive; // 单屏宽度
	int iPhyHei = st.m_VActive; // 单屏高度

	int iScrIndex = -1;
	int iOutPort  = -1;

	int iXoffset = 0; // 屏幕水平偏移
	int iYoffset = 0; // 屏幕垂直偏移

	for (int h=0; h<m_iPhyHorz; h++)
	{
		iXoffset += m_pHorzBlend[h];
		iYoffset = 0;

		for (int v=0; v<m_iPhyVert; v++)
		{
			iYoffset += m_pVertBlend[v];

			iScrIndex = v*m_iPhyHorz + h;

			m_pScrInf[iScrIndex].iScreenID = iScrIndex + 1;
			m_pScrInf[iScrIndex].iOutputCh = GetOutputManager()->GetCmdPort(iScrIndex, FALSE);

			m_pScrInf[iScrIndex].rcScreenPos.left   = iPhyWid*h - iXoffset;
			m_pScrInf[iScrIndex].rcScreenPos.right  = m_pScrInf[iScrIndex].rcScreenPos.left + iPhyWid;
			m_pScrInf[iScrIndex].rcScreenPos.top    = iPhyHei*v - iYoffset;
			m_pScrInf[iScrIndex].rcScreenPos.bottom = m_pScrInf[iScrIndex].rcScreenPos.top + iPhyHei;
		}
	}
}

void CSpliceSettingDlg::EnableBlendEdit(int icurselBlendH, int icurselBlendV)
{
	if (icurselBlendH <= 0 || icurselBlendH > m_iPhyHorz-1) {
		GetDlgItem(IDC_EDIT_BLEND_WID_HORZ)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_BLEND_WID_HORZ)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_EDIT_BLEND_WID_HORZ)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_BLEND_WID_HORZ)->EnableWindow(TRUE);
	}

	if (icurselBlendV <= 0 || icurselBlendV > m_iPhyVert-1) {
		GetDlgItem(IDC_EDIT_BLEND_WID_VERT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_BLEND_WID_VERT)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_EDIT_BLEND_WID_VERT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_BLEND_WID_VERT)->EnableWindow(TRUE);
	}
}

void CSpliceSettingDlg::OnBtnApplyScrSettings()
{
	UpdateData(TRUE);

	int nCursel = m_ListScr.GetCurSel();
	if (nCursel == LB_ERR) return;

	if (m_eScrOpt == eScrCustomized)
	{
		SetScreenInfo(nCursel);
	}
	else if (m_eScrOpt == eScrSimplySetup)
	{
		AutoScreenInfo();
	}
	else if (m_eScrOpt == eScrComplexSetup)
	{
		SetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
	}

	m_bDataSaved = TRUE;
}

void CSpliceSettingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcPic;
	GetDlgItem(IDC_PIC_SCREEN_COMBINE)->GetWindowRect(rcPic);
	ScreenToClient(&rcPic);

	if (!rcPic.PtInRect(point))
		return CDialog::OnLButtonDown(nFlags, point);

	CPoint pt = point;

	pt.Offset(-rcPic.TopLeft());
	OnLBtnDownProc(pt);

	CDialog::OnLButtonDown(nFlags, point);
}

void CSpliceSettingDlg::OnLBtnDownProc(CPoint point)
{
	int nCursel = m_ListScr.GetCurSel();

	CRect rcPic;
	GetDlgItem(IDC_PIC_SCREEN_COMBINE)->GetClientRect(rcPic);

	int nItemWidth  = GetScreenItemMargin().nItemWidth;
	int nItemHeight = GetScreenItemMargin().nItemHeight;
	int nItemSpace  = GetScreenItemMargin().nItemSpace;

	//int nMaxVisibleScrH = GetMaxVisibleScreens().key;
	//int nMaxVisibleScrV = GetMaxVisibleScreens().val;

	int hSpa = nItemWidth+nItemSpace;
	int vSpa = nItemHeight+nItemSpace;

	int h = point.x/hSpa;
	int v = point.y/vSpa;

	if ((point.x - h*hSpa) <= nItemWidth && (point.y - v*vSpa) <= nItemHeight)
	{
		if (h >= m_iPhyHorz) return;
		if (v >= m_iPhyVert) return;

		nCursel = h + m_iHPos + (m_iVPos + v)*m_iPhyHorz;

		if (nCursel < 0 || nCursel >= m_iPhyVert*m_iPhyHorz)
			return;

		RemindtoApplySetting();

		m_ListScr.SetCurSel(nCursel);
		m_iCurselScreen = nCursel;

		m_ListScr.SetTopIndex(nCursel);

		GetScreenInfo(nCursel);
	}
	else if (m_eScrOpt == eScrComplexSetup)
	{
		if ((point.x - h*hSpa) > nItemWidth) {

			if (m_iHPos+h < m_iPhyHorz-1) {
				RemindtoApplySetting();
				m_icurselBlendH = m_iHPos+h;
			}
		}			

		if ((point.y - v*vSpa) > nItemHeight) {
			
			if (m_iVPos+v < m_iPhyVert-1) {
				RemindtoApplySetting();
				m_icurselBlendV = m_iVPos+v;
			}
		}

		GetBlendInfo(m_icurselBlendH+1, m_icurselBlendV+1);
	}
		
	RepaintPic();
}

BOOL CSpliceSettingDlg::SaveState()
{
	CSysConfManager SysConf;
	// ---------------------------------------------------------------------
	// 基本的拼接配置
	// 屏幕组合
	SysConf.SetInt(_T("LastSpliceModel"), _T("LastTryHorzScreenNumber"), m_iPhyHorz);
	SysConf.SetInt(_T("LastSpliceModel"), _T("LastTryVertScreenNumber"), m_iPhyVert);

	// 对齐线
	SysConf.SetInt(_T("LastSpliceModel"), _T("LastTryHorzAligningLine"), m_iLogHorz);
	SysConf.SetInt(_T("LastSpliceModel"), _T("LastTryVertAligningLine"), m_iLogVert);

	// 拼接模式选择
	SysConf.SetInt(_T("LastSpliceModel"), _T("LastTryMode"), int(m_eScrOpt));

	// 简易融合的参数
	if (m_eScrOpt == eScrSimplySetup)
	{
		SysConf.SetInt(_T("LastSpliceModel"), _T("LastSimpleSetting.m_hSpace"), m_iSpaceH);
		SysConf.SetInt(_T("LastSpliceModel"), _T("LastSimpleSetting.m_vSpace"), m_iSpaceV);
	}

	// 复杂融合的参数
	if (m_eScrOpt == eScrComplexSetup)
	{
		CString strVal, strTmp;
		SysConf.SetInt(_T("LastSpliceModel"), _T("LastComplexSetting.m_horzblend.number"), m_iPhyHorz+1);
		if (m_pHorzBlend)
		{
			for (int i=0; i<m_iPhyHorz+1; i++) {
				strTmp.Format(_T("%d@"), m_pHorzBlend[i]);
				strVal += strTmp;
			}
		}
		SysConf.SetString(_T("LastSpliceModel"), _T("LastComplexSetting.m_horzblend.value"), strVal);

		SysConf.SetInt(_T("LastSpliceModel"), _T("LastComplexSetting.m_vertblend.number"), m_iPhyVert+1);
		if (m_pVertBlend)
		{
			strVal.Empty();

			for (int i=0; i<m_iPhyVert+1; i++) {
				strTmp.Format(_T("%d@"), m_pVertBlend[i]);
				strVal += strTmp;
			}
		}
		SysConf.SetString(_T("LastSpliceModel"), _T("LastComplexSetting.m_vertblend.value"), strVal);
	}

	// ---------------------------------------------------------------------
	// 另外的数据信息保存到一个*.lts文件
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\LastTrySpliceModel.lts");
	SysConf.SetConfigFilePath(strPath.GetBuffer());

	long nFileLen = sizeof(SPLICELAYOUT) + sizeof(SCREENMAP)*m_iScreens;
	CMemoryAllocate<char> alloc(nFileLen);

	long lpos = 0;
	memcpy(alloc.GetAddress()+lpos, m_pLayout, sizeof(SPLICELAYOUT)); lpos += sizeof(SPLICELAYOUT);
	memcpy(alloc.GetAddress()+lpos, m_pScrInf, sizeof(SCREENMAP)*m_iScreens); lpos += sizeof(SCREENMAP)*m_iScreens;

	return SysConf.WriteToFile(NULL, alloc.GetAddress(), nFileLen, TRUE);
}

BOOL CSpliceSettingDlg::LoadState()
{
	CSysConfManager SysConf;

	// ---------------------------------------------------------------------
	// 基本的拼接配置
	// 对齐线
	m_iLogHorz = SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryHorzAligningLine"), m_iLogHorz);
	m_iLogVert = SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryVertAligningLine"), m_iLogVert);

	// 拼接模式选择
	m_eScrOpt = (emScrOption)SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryMode"), int(m_eScrOpt));

	if (m_eScrOpt == eScrSimplySetup)
	{// 简易融合的参数
		m_iSpaceH = SysConf.GetInt(_T("LastSpliceModel"), _T("LastSimpleSetting.m_hSpace"), m_iSpaceH);
		m_iSpaceV = SysConf.GetInt(_T("LastSpliceModel"), _T("LastSimpleSetting.m_vSpace"), m_iSpaceV);
	}
	else if (m_eScrOpt == eScrComplexSetup)
	{// 复杂融合的参数
		CString strVal, strTmp;
		int iIter, iTok;
		iIter = iTok = 0;

		int iHBlends, iVBlends;
		iHBlends = iVBlends = 0;

		iHBlends = SysConf.GetInt(_T("LastSpliceModel"), _T("LastComplexSetting.m_horzblend.number"), iHBlends);
		strVal = SysConf.GetString(_T("LastSpliceModel"), _T("LastComplexSetting.m_horzblend.value"), strVal);

		if (iHBlends <= m_iPhyHorz+1)
		{
			if (!m_pHorzBlend) {
				m_pHorzBlend = new int[iHBlends];
				memset(m_pHorzBlend, 0, sizeof(int)*iHBlends);
			}

			strTmp = strVal.Tokenize(_T("@"), iTok);
			while(!strTmp.IsEmpty())
			{
				m_pHorzBlend[iIter++] = atoi(strTmp);
				strTmp = strVal.Tokenize(_T("@"), iTok);

				if (iIter > iHBlends-1)
					break;
			}
		}		

		strVal.Empty();
		strTmp.Empty();
		iVBlends = SysConf.GetInt(_T("LastSpliceModel"), _T("LastComplexSetting.m_vertblend.number"), iVBlends);
		strVal = SysConf.GetString(_T("LastSpliceModel"), _T("LastComplexSetting.m_vertblend.value"), strVal);

		iTok = iIter = 0;
		if (iVBlends <= m_iPhyVert+1)
		{
			if (!m_pVertBlend) {
				m_pVertBlend = new int[iVBlends];
				memset(m_pVertBlend, 0, sizeof(int)*iVBlends);
			}

			strTmp = strVal.Tokenize(_T("@"), iTok);
			while(!strTmp.IsEmpty())
			{
				m_pVertBlend[iIter++] = atoi(strTmp);
				strTmp = strVal.Tokenize(_T("@"), iTok);

				if (iIter > iVBlends-1)
					break;
			}
		}		
	}

	UpdateData(FALSE);

	return TRUE;
}