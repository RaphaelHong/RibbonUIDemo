// MvxWndPropertyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MvxWndPropertyDlg.h"
#include "MvxSplitDlg.h"


// CMvxWndPropertyDlg 对话框

IMPLEMENT_DYNAMIC(CMvxWndPropertyDlg, CDialog)

CMvxWndPropertyDlg::CMvxWndPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMvxWndPropertyDlg::IDD, pParent)
{

}

CMvxWndPropertyDlg::CMvxWndPropertyDlg(BOOL bNewModify, CWnd* pParent)
	: CDialog(CMvxWndPropertyDlg::IDD, pParent)
{
	m_bNewModify = bNewModify;
}

CMvxWndPropertyDlg::~CMvxWndPropertyDlg()
{
}

void CMvxWndPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WNDINDEX, m_CBoxWndIndex);
	DDX_Control(pDX, IDOK, m_BtnOK);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_WNDPOS_X, m_nWinPosX);
	DDX_Text(pDX, IDC_EDIT_WNDPOS_Y, m_nWinPosY);
	DDX_Text(pDX, IDC_EDIT_WNDSIZE_WIDTH, m_nWinSizeX);
	DDX_Text(pDX, IDC_EDIT_WNDSIZE_HEIGHT, m_nWinSizeY);
	DDX_Text(pDX, IDC_EDIT_WNDTITLE, m_strWinName);
}


BEGIN_MESSAGE_MAP(CMvxWndPropertyDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CMvxWndPropertyDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_WNDINDEX, OnCbnSelChangedWnd)
END_MESSAGE_MAP()


// CMvxWndPropertyDlg 消息处理程序

BOOL CMvxWndPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CBoxWndIndex.EnableWindow(!m_bNewModify);

	SetTheme(xtpControlThemeResource);

	return TRUE; 
}

HBRUSH CMvxWndPropertyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CMvxWndPropertyDlg::SetTheme(XTPControlTheme theme)
{
	m_CBoxWndIndex.SetTheme(theme);
	m_BtnOK.SetTheme(theme);
	m_BtnCancel.SetTheme(theme);
}

void CMvxWndPropertyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	CMvxSplitDlg *pParent = (CMvxSplitDlg *)m_pParentWnd;
	if (pParent == NULL) OnClose();

	//列出所有窗口ID，选中当前窗口的ID
	CString str;
	int nCount = pParent->m_ArrWinInfo.GetCount();
	for (int i=0; i<nCount; i++)
	{
		str.Format(_T("%d"), pParent->m_ArrWinInfo[i].WinId);
		m_CBoxWndIndex.AddString(str);
	}

	str.Format(_T("%d"), pParent->m_nFocusID);
	m_CBoxWndIndex.SelectString(0, str);

	//窗口信息
	int nIndex = pParent->GetWndIndex(pParent->m_nFocusID);
	if (nIndex == ErrIndex) OnClose();

	m_nWinPosX = pParent->m_ArrWinInfo[nIndex].rcPhy.left;
	m_nWinPosY = pParent->m_ArrWinInfo[nIndex].rcPhy.top;
	m_nWinSizeX= pParent->m_ArrWinInfo[nIndex].rcPhy.Width();
	m_nWinSizeY= pParent->m_ArrWinInfo[nIndex].rcPhy.Height();

	m_strWinName.Format(_T("%s"), pParent->m_ArrWinInfo[nIndex].szTitle);
	GetDlgItem(IDC_EDIT_WNDTITLE)->SetWindowText(str);

	UpdateData(FALSE);
}

void CMvxWndPropertyDlg::OnBnClickedOk()
{
	UpdateData();

	CMvxSplitDlg *pParent = (CMvxSplitDlg *)m_pParentWnd;
	if (pParent == NULL) OnOK();

	CString str;
	m_CBoxWndIndex.GetWindowText(str);

	int nWinID = atoi(str);

	int nIndex = pParent->GetWndIndex(nWinID);
	if (nIndex == ErrIndex) OnOK();

	CRect rcPhy;

	rcPhy.left   = m_nWinPosX;
	rcPhy.top    = m_nWinPosY;
	rcPhy.right  = m_nWinPosX+m_nWinSizeX;
	rcPhy.bottom = m_nWinPosY+m_nWinSizeY;

	pParent->m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	pParent->m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(pParent->m_nLogHorz, pParent->m_nPhyHorz);

	strncpy_s(pParent->m_ArrWinInfo[nIndex].szTitle, m_strWinName.GetBuffer(), m_strWinName.GetLength());
	m_strWinName.ReleaseBuffer();

	if (m_bNewModify)
		theApp.MVX_OpenNewWindow(nWinID,
                             rcPhy,
                             pParent->m_ArrWinInfo[nIndex].iSource,
                             pParent->m_ArrWinInfo[nIndex].iSource, 
                             pParent->m_boardInfo.GetBoardPort());
	else
		theApp.MVX_ResizeWindow(nWinID,
                            rcPhy,
                            pParent->m_ArrWinInfo[nIndex].iSource,
                            pParent->m_ArrWinInfo[nIndex].iSource, 
                            pParent->m_boardInfo.GetBoardPort());
	
	OnOK();
}


void CMvxWndPropertyDlg::OnCbnSelChangedWnd()
{
	CMvxSplitDlg *pParent = (CMvxSplitDlg *)m_pParentWnd;
	if (pParent == NULL) return;

	CString str;
	m_CBoxWndIndex.GetWindowText(str);

	int nWinID = atoi(str);

	int nIndex = pParent->GetWndIndex(nWinID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = pParent->m_ArrWinInfo[nIndex].rcPhy;
	m_nWinPosX  = rcPhy.left;
	m_nWinPosY  = rcPhy.top;
	m_nWinSizeX = rcPhy.Width();
	m_nWinSizeY = rcPhy.Height();

	m_strWinName.Format(_T("%s"), pParent->m_ArrWinInfo[nIndex].szTitle);

	UpdateData(FALSE);
}