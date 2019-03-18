// Hdmi3dDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "Hdmi3dDlg.h"


// CHdmi3dDlg 对话框

IMPLEMENT_DYNAMIC(CHdmi3dDlg, CXTPResizeDialog)

CHdmi3dDlg::CHdmi3dDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CHdmi3dDlg::IDD, pParent)
{

}

CHdmi3dDlg::~CHdmi3dDlg()
{
}

void CHdmi3dDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHdmi3dDlg, CXTPResizeDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_HDMI_TABCONTROL, OnSelectChanged)
END_MESSAGE_MAP()


// CHdmi3dDlg 消息处理程序

BOOL CHdmi3dDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	CreateChildWindows();

	SetResize(IDC_HDMI_TABCONTROL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	return TRUE;
}

void CHdmi3dDlg::CreateChildWindows()
{
	CRect rc;
	GetClientRect(&rc);

	m_wndTab.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rc, this, IDC_HDMI_TABCONTROL);
	m_wndTab.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003); 
	m_wndTab.GetPaintManager()->DisableLunaColors(FALSE); //只有office2003 2007 2010风格能设置Luna colors
	m_wndTab.GetPaintManager()->SetOneNoteColors(TRUE);
	m_wndTab.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTab.GetPaintManager()->m_bShowIcons = TRUE;
	m_wndTab.GetPaintManager()->SetPosition(xtpTabPositionBottom);
	m_wndTab.GetPaintManager()->m_bBoldSelected = TRUE;
	m_wndTab.GetImageManager()->SetIcons(IDB_BITMAP_MG_TAP, NULL,  0, CSize(16,16), xtpImageNormal);

	int nItem = 0;

	m_dlgTiming.Create(IDD_DLG_HDMI_TIMING, &m_wndTab);
	m_dlgTiming.SetOwner(this);
	m_wndTab.InsertItem(nItem++, _T("分辨率"), m_dlgTiming, 0);

	m_dlgOption.Create(IDD_DLG_HDMI_3DSETTING, &m_wndTab);
	m_dlgOption.SetOwner(this);
	m_wndTab.InsertItem(nItem++, _T("3D设置"), m_dlgOption, 7);
}

void CHdmi3dDlg::OnSelectChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);

	int nCurSel = m_wndTab.GetCurSel();

	CXTPTabManagerItem *pTabItem = m_wndTab.GetItem(nCurSel);
	CWnd *pWnd = FromHandle(pTabItem->GetHandle());
	if (pWnd != NULL) pWnd->SetFocus();

	*pResult = 0;
}

void CHdmi3dDlg::Initialize()
{
	if (m_dlgTiming.GetSafeHwnd()) m_dlgTiming.Initialize();
	if (m_dlgOption.GetSafeHwnd()) m_dlgOption.Initailize();
}

BOOL CHdmi3dDlg::SaveAll()
{
	if (theApp.SaveAll(m_boardinfo.GetBoardPort()) == CMD_FAILURE)
		return FALSE;
	else
		return TRUE;
}