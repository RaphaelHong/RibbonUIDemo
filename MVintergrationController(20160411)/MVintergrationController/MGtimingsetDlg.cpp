// MGtimingsetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGtimingsetDlg.h"
#include "MGeoMainDlg.h"


// CMGtimingsetDlg 对话框

IMPLEMENT_DYNAMIC(CMGtimingsetDlg, CXTPResizeDialog)

CMGtimingsetDlg::CMGtimingsetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGtimingsetDlg::IDD, pParent)
{
	memset(&m_curTiming, 0, sizeof(TIMING_STRUCT));

	m_pstrTming = NULL;
	m_pDetailedTiming = NULL;
	m_nTotalTiming = 0;
}

CMGtimingsetDlg::~CMGtimingsetDlg()
{
}

void CMGtimingsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_COMBMODE, m_cmbMode);
	DDX_Control(pDX, IDC_CBOX_TIMING_MG, m_cmbTiming);
	DDX_Control(pDX, IDC_BTN_TIMINGSET_MG, m_btnSet);
	DDX_Control(pDX, IDC_EDIT_TIMINGINFO, m_editTimingInfo);
}


BEGIN_MESSAGE_MAP(CMGtimingsetDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()


// CMGtimingsetDlg 消息处理程序
BOOL CMGtimingsetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
	//m_curTiming = pOwer->m_boardInfo.GetDetailTiming(Product_MG)[0];

	if (m_wndPropertyGrid.GetSafeHwnd() == 0)
	{
		m_wndPropertyGrid.Create( CRect(0, 0, 0, 0), this, 0 );
		m_wndPropertyGrid.SetOwner(this);

		CFont font;
		font.CreatePointFont(60, _T("Marlett"));

		LOGFONT lf;
		font.GetLogFont(&lf);

		CXTPPropertyGridItem* pInfos           = m_wndPropertyGrid.AddCategory(_T("当前分辨率"));
		pInfos->SetDescription(_T("当前输入信号源的信息"));


		CXTPPropertyGridItem* pSignalInfo      = pInfos->AddChildItem(new CXTPPropertyGridItem(_T("信号源类型"), _T("无")));
		CXTPPropertyGridItem* pTimingInfo      = pInfos->AddChildItem(new CXTPPropertyGridItem(_T("信号分辨率"), _T("无")));

		pSignalInfo->SetReadOnly(TRUE);
		pTimingInfo->SetReadOnly(TRUE);

		CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("设置"));
		CXTPPropertyGridItem* pCombModes       = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("分屏模式")));
		CXTPPropertyGridItem* pTimings         = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("分辨率")));
		pSettings->SetDescription(_T("设置输入分辨率"));
		pTimings->SetDescription(_T("从右侧选择框中选择合适的分辨率"));

		
		CXTPPropertyGridItemConstraints* pList = pCombModes->GetConstraints();
		int i=0;
		while(stSplitMode[i].strmode != _T("END"))
		{
			pList->AddConstraint(stSplitMode[i].strmode);
			i++;
		}
		pCombModes->SetFlags(xtpGridItemHasComboButton);
		pCombModes->SetHidden(TRUE);

		//pList = pTimings->GetConstraints();
		//i=0;
		//CString *pStrTiming = pOwer->m_boardInfo.GetTimingName(Product_MG);
		//TIMING_STRUCT *pTiming = pOwer->m_boardInfo.GetDetailTiming(Product_MG);
		//int nFuncTag = (pOwer->m_boardInfo.GetFunCodeByte(0) & 0x0f);
		//while(pStrTiming[i] != _T("END"))
		//{
		//	if (nFuncTag == 1 )
		//	{//标清
		//		if (pTiming[i].m_HActive > 1280 || pTiming[i].m_Hz == 120)  {
		//			i++; 
		//			continue; 
		//		}
		//	}
		//	else if (nFuncTag == 2)
		//	{//高清
		//		if (pTiming[i].m_Hz == 120)
		//		{
		//			i++;
		//			continue;
		//		}
		//	}
		//		
		//	pList->AddConstraint(pStrTiming[i]);
		//	i++;
		//}
		pTimings->SetFlags(xtpGridItemHasComboButton);

		CXTPPropertyGridInplaceButton *pButton = pTimings->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(XTP_BUTTON_SETTIMING));
		pButton->SetCaption(_T("设置"));	
		
		
		m_wndPropertyGrid.SetTheme(xtpGridThemeResource);
		m_wndPropertyGrid.HighlightChangedItems(TRUE);

		ExpandAllGridItems(m_wndPropertyGrid.GetCategories(), TRUE);
	}

	SetResize(IDC_EDIT_TIMINGINFO, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_CBOX_COMBMODE, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_CBOX_TIMING_MG, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BTN_TIMINGSET_MG, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR1_TIMING_MG, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR2_TIMING_MG, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	SetControlsTheme(xtpControlThemeResource);

	m_editTimingInfo.SetSel(-1,-1);
	m_editTimingInfo.ReplaceSel(_T("当前输入信号：DVI信号\n"));
	m_editTimingInfo.SetSel(-1,-1);
	m_editTimingInfo.ReplaceSel(_T("当前分辨率：1920x1080 @ 60Hz"));

	m_cmbMode.ResetContent();
	int i=0;
	while(stSplitMode[i].strmode != _T("END"))
	{
		m_cmbMode.AddString(stSplitMode[i].strmode);
		i++;
	}
	m_cmbMode.SetCurSel(0);

	//创建背景画刷
	m_bkBrush.CreateSolidBrush(constClrDlgBk[1]);


	return TRUE; 
}

void CMGtimingsetDlg::SetControlsTheme(XTPControlTheme theme)
{
	m_editTimingInfo.SetTheme(xtpControlThemeResource);
	m_editTimingInfo.SetFlatStyle(TRUE);
	m_cmbMode.SetTheme(theme);
	m_cmbTiming.SetTheme(theme);
	m_btnSet.SetTheme(theme);
}


HBRUSH CMGtimingsetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(15,15,1));

		return m_bkBrush;
	}

	return hbr;
}

void CMGtimingsetDlg::OnOK()
{
	//防止Esc Enter键退出
	//CXTPResizeDialog::OnOK();
}
void CMGtimingsetDlg::OnCancel()
{
	//防止Esc Enter键退出
	//CXTPResizeDialog::OnCancel();
}


void CMGtimingsetDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if (m_wndPropertyGrid.GetSafeHwnd())
	{
		m_wndPropertyGrid.MoveWindow(0,0,cx, cy);
	}
}

LRESULT CMGtimingsetDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)
	{
		CXTPPropertyGridInplaceButton *pBtn = (CXTPPropertyGridInplaceButton*)lParam;
		if (pBtn->GetID() == XTP_BUTTON_SETTIMING)
		{	
			CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(_T("分辨率"));
			int nSel = pItem->GetConstraints()->GetCurrent();
			if (nSel != -1)
			{
				int nRet = SetTiming(nSel);

				if (nRet == CMD_RTOK)
				{
					pItem = m_wndPropertyGrid.FindItem(_T("信号分辨率"));
					int nPos = 0;
					CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
					pItem->SetValue(pOwer->m_boardInfo.GetTimingName(Product_MG)[nSel].Tokenize(_T("("), nPos));
				}
			}

			return TRUE;
		}		
	}

	if (wParam == XTP_PGN_SELECTION_CHANGED)
	{

	}

	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		
	}
	

	return 0;
}

int CMGtimingsetDlg::SetTiming(int nIndex)
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();

	CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(_T("分辨率"));
	CString strTiming = pItem->GetValue();

	int nCursel = -1;
	for (int i=0; i<m_nTotalTiming; i++)
	{
		if (m_pstrTming[i] == strTiming)
		{
			nCursel =  i;
			break;
		}
	}

	if (nCursel == -1) 
	{ //没找到相应的分辨率 一般不会有这问题
		MessageBox(_T("分辨率参数有误！"), _T("设置分辨率"), MB_OK|MB_ICONSTOP);
		return -1;
	}

	pTIMING_STRUCT pTS;
	pTS = &m_pDetailedTiming[nCursel];

	int nRet = theApp.SetInputTiming(pTS, 3, 0xff, pOwner->m_boardInfo.GetBoardPort());

	if (nRet == CMD_RTOK)
	{
		m_curTiming = *pTS;
		MessageBox(_T("设置分辨率成功，重新插拔输入信号线生效！"), _T("设置分辨率"), MB_OK|MB_ICONINFORMATION);
	}
	else MessageBox(_T("设置分辨率失败！"), _T("设置分辨率"), MB_ICONSTOP);

	return nRet;
}

void CMGtimingsetDlg::Initialize()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();

	m_pDetailedTiming = pOwner->m_boardInfo.GetDetailTiming(Product_MG);
	m_pstrTming       = pOwner->m_boardInfo.GetTimingName(Product_MG);
	m_nTotalTiming    = pOwner->m_boardInfo.GetTotalTimings(Product_MG);

	AddTimingToList();

	//pTIMING_STRUCT pTS  = pOwner->m_boardInfo.GetDetailTiming(Product_MG);
	//CString *pStrTiming = pOwner->m_boardInfo.GetTimingName(Product_MG);
	//int nTotalTiming    = pOwner->m_boardInfo.GetTotalTimings(Product_MG);

	TIMING_STRUCT ts;
	memset(&ts, 0, sizeof(TIMING_STRUCT));

	int nRet = theApp.GetInputTiming(&ts, 3, 0xff, pOwner->m_boardInfo.GetBoardPort());
	//int nRet = theApp.GetCurrencyInputTiming((UINT&)ts.m_Hz, (UINT&)ts.m_HTotal, (UINT&)ts.m_HActive, (UINT&)ts.m_VTotal, (UINT&)ts.m_VActive, pOwner->m_boardInfo.GetBoardPort());

	int nIndex = 0;
	if (nRet == CMD_RTOK)
	{		
		while(nIndex < m_nTotalTiming)
		{
			if (m_pDetailedTiming[nIndex].m_HActive == ts.m_HActive
			  &&m_pDetailedTiming[nIndex].m_VActive == ts.m_VActive
			  &&m_pDetailedTiming[nIndex].m_Hz == ts.m_Hz)
			{
				m_curTiming = m_pDetailedTiming[nIndex];
				break;
			}

			nIndex++;
		}
	}
	else
	{
		nIndex = -1;
		MessageBox(_T("读取输入分辨率失败！"), _T("读取输入分辨率"), MB_ICONERROR);
	}

	CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(_T("信号源类型"));
	pItem->SetValue(_T("DVI信号"));

	if (nIndex < m_nTotalTiming && nIndex != -1)
	{
		int nPos =0;
		pItem = m_wndPropertyGrid.FindItem(_T("信号分辨率"));
		pItem->SetValue(m_pstrTming[nIndex].Tokenize(_T("("), nPos));

		pItem = m_wndPropertyGrid.FindItem(_T("分辨率"));
		pItem->SetValue(m_pstrTming[nIndex]);
	}
	else
	{
		if (ts.m_HActive <=0 || ts.m_VActive <= 0)
		{
			//MessageBox(_T("警告：当前板卡未接入信号，几何校正和多点校正将采用默认分辨率(1024x768 @ 60Hz)！"),_T("几何校正初始化"), MB_OK|MB_ICONERROR);
			MessageBox(_T("没有输入信号！！"), _T("读取输入分辨率"), MB_ICONERROR);
		}
		else
			MessageBox(_T("未定义的输入分辨率！"), _T("读取输入分辨率"), MB_ICONERROR);
	}
}

void CMGtimingsetDlg::AddTimingToList()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();

	CXTPPropertyGridItem* pTimingItem = m_wndPropertyGrid.FindItem(_T("分辨率"));
	CXTPPropertyGridItemConstraints* pList = pTimingItem->GetConstraints();
	pList = pTimingItem->GetConstraints();

	int i=0;
	//CString *pStrTiming = pOwner->m_boardInfo.GetTimingName(Product_MG);
	//TIMING_STRUCT *pTiming = pOwner->m_boardInfo.GetDetailTiming(Product_MG);
	int nFuncTag = (pOwner->m_boardInfo.GetFunCodeByte(0) & 0x0f);
	while(m_pstrTming[i] != _T("END"))
	{
		if (nFuncTag == 1 )
		{//标清
			if (m_pDetailedTiming[i].m_HActive > 1280 || m_pDetailedTiming[i].m_Hz == 120)  {
				i++; 
				continue; 
			}
		}
		else if (nFuncTag == 2)
		{//高清
			if (m_pDetailedTiming[i].m_Hz == 120)
			{
				i++;
				continue;
			}
		}
			
		pList->AddConstraint(m_pstrTming[i]);
		i++;
	}
}