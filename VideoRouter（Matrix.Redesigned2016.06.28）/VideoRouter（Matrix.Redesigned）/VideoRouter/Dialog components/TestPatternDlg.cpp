// TestPatternDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "..\MainFrm.h"
#include "..\VideoRouterDoc.h"
#include "TestPatternDlg.h"
#include "afxdialogex.h"

using namespace Gdiplus;

#define BoolType(b) b?true:false

// CTestPatternDlg 对话框

IMPLEMENT_DYNAMIC(CTestPatternDlg, CDialog)

CTestPatternDlg::CTestPatternDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestPatternDlg::IDD, pParent)
, m_nHasGrid(0)
, m_nGridlineWid(1)
, m_nGridHorzSpace(32)
, m_nGridVertSpace(32)
, m_bPreview(FALSE)
, m_ptBegin(0, 0)
{
	m_BkgSize.cx = 1024;
	m_BkgSize.cy = 768;
}

CTestPatternDlg::~CTestPatternDlg()
{
}

void CTestPatternDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLORBTN_PATTERN_BK_COLOR, m_clrbtnBackgrd);
	DDX_Control(pDX, IDC_COLORBTN_GRIDLINE_COLOR, m_clrbtnGridline);
	DDX_Control(pDX, IDC_CBOX_PATTERN_GRIDLINE_WID, m_cboxlineWid);
	DDX_Control(pDX, IDC_CBOX_PATTERN_HORZ_SPACE, m_cboxHorzSpace);
	DDX_Control(pDX, IDC_CBOX_PATTERN_VERT_SPACE, m_cboxVertSpace);
	DDX_Text(pDX, IDC_EDIT_PATTERN_GRIDLINE_WID, m_nGridlineWid);
	DDX_Text(pDX, IDC_EDIT_PATTERN_HORZ_SPACE, m_nGridHorzSpace);
	DDX_Text(pDX, IDC_EDIT_PATTERN_VERT_SPACE, m_nGridVertSpace);
	DDX_Text(pDX, IDC_EDIT_GRID_START_POSX, m_ptBegin.x);
	DDX_Text(pDX, IDC_EDIT_GRID_START_POSY, m_ptBegin.y);
	DDV_MinMaxInt(pDX, m_nGridlineWid, 1, 64);
	DDV_MinMaxInt(pDX, m_nGridHorzSpace, 2, 256);
	DDV_MinMaxInt(pDX, m_nGridVertSpace, 2, 128);
	DDX_Check(pDX, IDC_CHECK_HAS_GRID_PATTERN, m_nHasGrid);
	DDX_Control(pDX, IDC_TAB_PORT, m_Tab);
}


BEGIN_MESSAGE_MAP(CTestPatternDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CBOX_PATTERN_GRIDLINE_WID, &CTestPatternDlg::OnSelChangeGridLineWid)
	ON_CBN_SELCHANGE(IDC_CBOX_PATTERN_HORZ_SPACE, &CTestPatternDlg::OnSelChangeGridHorzSpace)
	ON_CBN_SELCHANGE(IDC_CBOX_PATTERN_VERT_SPACE, &CTestPatternDlg::OnSelChangeGridVertSpace)
	ON_BN_CLICKED(IDC_CHECK_HAS_GRID_PATTERN, &CTestPatternDlg::OnBnClickedCheckHasGridPattern)
	ON_BN_CLICKED(IDC_BTN_PATTERN_PREVIEW, &CTestPatternDlg::OnBtnPreView)
	ON_BN_CLICKED(IDC_BTN_PATTERN_ALL, &CTestPatternDlg::OnBtnAll)
	ON_BN_CLICKED(IDC_BTN_PATTERN_NON, &CTestPatternDlg::OnBtnNon)
	ON_BN_CLICKED(IDC_BTN_PATTERN_INVERT, &CTestPatternDlg::OnBtnInv)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PATTERN_OUTPUT, &CTestPatternDlg::OnLvnItemchangedListPatternOutput)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PATTERN_INPUT, &CTestPatternDlg::OnLvnItemchangedListPatternInput)	
	ON_BN_CLICKED(IDC_BTN_PATTERN_TEST, &CTestPatternDlg::OnBtnTest)
	ON_BN_CLICKED(IDC_BTN_CANCEL_TEST, &CTestPatternDlg::OnBtnCancelTest)
	ON_BN_CLICKED(IDC_COLORBTN_PATTERN_BK_COLOR, &CTestPatternDlg::OnBtnBkColor)
	ON_BN_CLICKED(IDC_COLORBTN_GRIDLINE_COLOR, &CTestPatternDlg::OnBtnLineColor)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PORT, &CTestPatternDlg::OnTcnSelchangeTabPort)
	ON_BN_CLICKED(IDC_BTN_PATTERN_PRINT_OSD, &CTestPatternDlg::OnBnClickedBtnPatternPrintOsd)
	ON_BN_CLICKED(IDC_BTN_PATTERN_CANCEL_OSD, &CTestPatternDlg::OnBnClickedBtnPatternCancelOsd)
END_MESSAGE_MAP()


// CTestPatternDlg 消息处理程序
BOOL CTestPatternDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	m_clrbtnBackgrd.SetColor(0x0);
	m_clrbtnBackgrd.EnableOtherButton(_T("Other"));
	m_clrbtnGridline.SetColor(0x00ffffff);
	m_clrbtnGridline.EnableOtherButton(_T("Other"));

	CString strText;
	strText.Format(_T("%d"), m_nGridlineWid);
	m_cboxlineWid.SelectString(m_cboxlineWid.GetCount()-1, strText);

	strText.Format(_T("%d"), m_nGridHorzSpace);
	m_cboxHorzSpace.SelectString(m_cboxHorzSpace.GetCount()-1, strText);

	strText.Format(_T("%d"), m_nGridVertSpace);
	m_cboxVertSpace.SelectString(m_cboxVertSpace.GetCount()-1, strText);

	EnableGridEdit(m_nHasGrid);

	InitTabCtrl();

	return TRUE;
}

void CTestPatternDlg::OnSelChangeGridLineWid()
{
	int nSel = m_cboxlineWid.GetCurSel();
	if (nSel == CB_ERR)
		return;

	CString str;
	m_cboxlineWid.GetWindowText(str);

	m_nGridlineWid = atoi(str);

	UpdateData(FALSE);
	Invalidate();
}

void CTestPatternDlg::OnSelChangeGridHorzSpace()
{
	int nSel = m_cboxHorzSpace.GetCurSel();
	if (nSel == CB_ERR)
		return;

	CString str;
	m_cboxHorzSpace.GetWindowText(str);

	m_nGridHorzSpace = atoi(str);

	UpdateData(FALSE);
	Invalidate();
}

void CTestPatternDlg::OnSelChangeGridVertSpace()
{
	int nSel = m_cboxVertSpace.GetCurSel();
	if (nSel == CB_ERR)
		return;

	CString str;
	m_cboxVertSpace.GetWindowText(str);

	m_nGridVertSpace = atoi(str);

	UpdateData(FALSE);
	Invalidate();
}

void CTestPatternDlg::EnableGridEdit(BOOL bHasGrid)
{
	GetDlgItem(IDC_EDIT_GRID_START_POSX)->EnableWindow(bHasGrid);
	GetDlgItem(IDC_EDIT_GRID_START_POSY)->EnableWindow(bHasGrid);
	m_clrbtnGridline.EnableWindow(bHasGrid);
	m_cboxlineWid.EnableWindow(bHasGrid);
	m_cboxHorzSpace.EnableWindow(bHasGrid);
	m_cboxVertSpace.EnableWindow(bHasGrid);
	GetDlgItem(IDC_EDIT_PATTERN_GRIDLINE_WID)->EnableWindow(bHasGrid);
	GetDlgItem(IDC_EDIT_PATTERN_HORZ_SPACE)->EnableWindow(bHasGrid);
	GetDlgItem(IDC_EDIT_PATTERN_VERT_SPACE)->EnableWindow(bHasGrid);

	GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->Invalidate();
}

void CTestPatternDlg::OnBnClickedCheckHasGridPattern()
{
	UpdateData();

	EnableGridEdit(m_nHasGrid);
}


void CTestPatternDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC *pDC = GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->GetDC();
	CRect rcPic;
	GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->GetClientRect(&rcPic);

	COLORREF clrBk = m_clrbtnBackgrd.GetColor();

	SolidBrush bkBrush(Color(255, GetRValue(clrBk), GetGValue(clrBk), GetBValue(clrBk)));

	Graphics gp(pDC->m_hDC);
	gp.FillRectangle(&bkBrush, rcPic.left, rcPic.top, rcPic.Width(), rcPic.Height());

	if (m_nHasGrid)
	{
		DrawGridLine(pDC);
	}
}

void CTestPatternDlg::DrawGridLine(CDC *pDC)
{
	Graphics gp(pDC->m_hDC);

	CRect rcPic;
	GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->GetClientRect(&rcPic);

	int nLineWid = m_nGridlineWid;
	int nHSpace = m_nGridHorzSpace;
	int nVSpace = m_nGridVertSpace;

	COLORREF clrLine = m_clrbtnGridline.GetColor();
	Pen pen(Color(255, GetRValue(clrLine), GetGValue(clrLine), GetBValue(clrLine)), (REAL)nLineWid);

	int nLineCountV = rcPic.Width()/(nLineWid+nHSpace)+1;
	int nLineCountH = rcPic.Height()/(nLineWid+nVSpace)+1;

	Point ptStart, ptEnd;

	for (int i=0; i<nLineCountV; i++)
	{
		ptStart.X = rcPic.left + nLineWid/2 + i*(nLineWid+nHSpace);
		ptStart.Y = rcPic.top;
		ptEnd.X = ptStart.X;
		ptEnd.Y = rcPic.bottom;
		gp.DrawLine(&pen, ptStart, ptEnd);
	}

	for (int i=0; i<nLineCountH; i++)
	{
		ptStart.X = rcPic.left;
		ptStart.Y = rcPic.top + nLineWid/2 + i*(nLineWid+nVSpace);
		ptEnd.X = rcPic.right;
		ptEnd.Y = ptStart.Y;
		gp.DrawLine(&pen, ptStart, ptEnd);
	}
}

BOOL CTestPatternDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			if (FromHandle(pMsg->hwnd) == GetDlgItem(IDC_EDIT_PATTERN_HORZ_SPACE)
				|| FromHandle(pMsg->hwnd) == GetDlgItem(IDC_EDIT_PATTERN_VERT_SPACE)
				|| FromHandle(pMsg->hwnd) == GetDlgItem(IDC_EDIT_PATTERN_GRIDLINE_WID))
			{
				OnVKreturn();
			}

			return TRUE;
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		if (FromHandle(pMsg->hwnd) == GetDlgItem(IDC_COLORBTN_PATTERN_BK_COLOR)
			|| FromHandle(pMsg->hwnd) == GetDlgItem(IDC_COLORBTN_GRIDLINE_COLOR))
		{
			GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->Invalidate();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTestPatternDlg::OnVKreturn()
{
	UpdateData();

	if (m_nGridlineWid < 1) m_nGridlineWid = 1;
	if (m_nGridlineWid > 64) m_nGridlineWid = 64;

	if (m_nGridHorzSpace < 2) m_nGridHorzSpace = 2;
	if (m_nGridHorzSpace > 256) m_nGridHorzSpace = 256;

	if (m_nGridVertSpace < 2) m_nGridVertSpace = 2;
	if (m_nGridVertSpace > 128) m_nGridVertSpace = 128;

	CString str;
	str.Format(_T("%d"), m_nGridlineWid);
	m_cboxlineWid.SelectString(m_cboxlineWid.GetCount()-1, str);

	str.Format(_T("%d"), m_nGridHorzSpace);
	m_cboxHorzSpace.SelectString(m_cboxHorzSpace.GetCount()-1, str);

	str.Format(_T("%d"), m_nGridVertSpace);
	m_cboxVertSpace.SelectString(m_cboxVertSpace.GetCount()-1, str);

	UpdateData(FALSE);
	Invalidate();
}

void CTestPatternDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	OnPreview();
}

void CTestPatternDlg::OnPreview()
{
	CRect rc,rcNew;
	GetWindowRect(&rc);
	rcNew = rc;

	CRect rcSeparator;
	GetDlgItem(IDC_SEPARATOR)->GetWindowRect(rcSeparator);
	//ScreenToClient(rcSeparator);

	CRect rcPic;
	GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->GetWindowRect(rcPic);
	//ScreenToClient(rcPic);

	if (m_bPreview)
	{
		rcNew.right = rcPic.right;
	}
	else
	{
		rcNew.right = rcSeparator.left;
	}

	MoveWindow(rcNew);

	//CenterWindow(GetDesktopWindow());
}

void CTestPatternDlg::OnBtnPreView()
{
	m_bPreview = !m_bPreview;
	OnPreview();

	CString str;
	CWnd *pWnd = GetDlgItem(IDC_BTN_PATTERN_PREVIEW);
	pWnd->GetWindowText(str);
	str.Trim(">> <<");

	if (m_bPreview)
		str.Append("<<");
	else
		str.Append(">>");

	pWnd->SetWindowText(str);
}

void CTestPatternDlg::OnBtnAll()
{
	CXTPListCtrl* pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		pList = &m_listOutput;
	}

	CRedrawLock lock(pList);

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		pList->SetCheck(i, TRUE);
	}
}

void CTestPatternDlg::OnBtnNon()
{
	CXTPListCtrl* pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		pList = &m_listOutput;
	}

	CRedrawLock lock(pList);

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		pList->SetCheck(i, FALSE);
	}
}

void CTestPatternDlg::OnBtnInv()
{
	CXTPListCtrl* pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		pList = &m_listOutput;
	}

	CRedrawLock lock(pList);

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		pList->SetCheck(i, !pList->GetCheck(i));
	}
}

void CTestPatternDlg::InitTabCtrl()
{
	m_Tab.InsertItem(0, _T("输入端口"));
	m_Tab.InsertItem(1, _T("输出端口"));

	m_Tab.ShowWindow(SW_SHOW);

	InitListCtrl();
}

void CTestPatternDlg::InitListCtrl()
{
	CRect rcTab;
	m_Tab.GetClientRect(rcTab);

	CRect rcList;
	rcList = rcTab;

	CRect rcItem;
	m_Tab.GetItemRect(0, rcItem);
	rcList.InflateRect(-2, -rcItem.Height()-5, -3, -2);

	m_listInput.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS, rcList, &m_Tab, IDC_LIST_PATTERN_INPUT);

	m_listInput.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);
	m_listInput.ModifyExtendedStyle(0, LVS_EX_CHECKBOXES);
	m_listInput.ModifyExtendedStyle(0, LVS_EX_GRIDLINES);
	m_listInput.EnableUserSortColor(true);
	m_listInput.EnableUserListColor(true);
	m_listInput.EnableUserRowColor(true);

	m_listInput.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listInput.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 100);
	m_listInput.InsertColumn(2, _T("结果"), LVCFMT_CENTER, 80);

	int nCount = GetInputManager()->GetRealCount();
	CString strItem;
	for (int i=0; i<nCount; i++)
	{
		strItem.Format(_T("%d"), i+1);
		m_listInput.InsertItem(LVIF_TEXT, i, strItem, 0, LVIS_SELECTED, 0, 0);
		strItem.Format("%s", GetInputManager()->GetName(GetInputManager()->RealIndex2LogicIndex(i), FALSE));
		m_listInput.SetItemText(i, 1, strItem);
	}
	m_listInput.ShowWindow(SW_SHOW);

	m_listOutput.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_SINGLESEL|LVS_SHOWSELALWAYS, rcList, &m_Tab, IDC_LIST_PATTERN_INPUT);
	m_listOutput.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);
	m_listOutput.ModifyExtendedStyle(0, LVS_EX_CHECKBOXES);
	m_listOutput.ModifyExtendedStyle(0, LVS_EX_GRIDLINES);
	m_listOutput.EnableUserSortColor(true);
	m_listOutput.EnableUserListColor(true);
	m_listOutput.EnableUserRowColor(true);

	m_listOutput.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
	m_listOutput.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 100);
	m_listOutput.InsertColumn(2, _T("结果"), LVCFMT_CENTER, 80);

	nCount = GetOutputManager()->GetRealCount();
	for (int i=0; i<nCount; i++)
	{
		strItem.Format("%d", i+1);
		m_listOutput.InsertItem(LVIF_TEXT, i, strItem, 0, LVIS_SELECTED, 0, 0);
		strItem.Format("%s", GetOutputManager()->GetName(GetOutputManager()->RealIndex2LogicIndex(i), FALSE));
		m_listOutput.SetItemText(i, 1, strItem);
	}

	m_listOutput.ShowWindow(SW_HIDE);
}

void CTestPatternDlg::OnLvnItemchangedListPatternInput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = pNMLV->iItem;

	CXTPListCtrl* pList = NULL;
	if (m_Tab.GetCurSel()==0)
		pList = &m_listInput;
	else if (m_Tab.GetCurSel()==1)
		pList = &m_listOutput;

	if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1) )
		&& ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2) ) ) //UnChecked to Checked
	{ 
		pList->SetRowColor(nItem, ListColorText, ListColorItemSelected);
	}
	else if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2) )
		&&  ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1) ) ) //Checked to UnChecked
	{
		pList->SetRowColor(nItem, ListColorText, ListColorBackground);
	}


	*pResult = 0;
}

void CTestPatternDlg::OnLvnItemchangedListPatternOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = pNMLV->iItem;

	if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1) )
		&& ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2) ) ) //UnChecked to Checked
	{ 
		m_listOutput.SetRowColor(nItem, ListColorText, ListColorItemSelected);
	}
	else if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2) )
		&&  ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1) ) ) //Checked to UnChecked
	{
		m_listOutput.SetRowColor(nItem, ListColorText, ListColorBackground);
	}


	*pResult = 0;
}

void CTestPatternDlg::OnBtnTest()
{
	ResetListItemColor();

	UpdateData();

	CInputManager *pManager = NULL;
	CXTPListCtrl *pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
		pManager = (CInputManager*)GetInputManager();
	}
	else
	{
		pList = &m_listOutput;
		pManager = (COutputManager*)GetOutputManager();
	}

	// 测试数据
	IMGTESTINFO tstinfo;
	tstinfo.ptBegin = m_ptBegin;
	tstinfo.iLineWidth = m_nGridlineWid;
	tstinfo.szLineSpace = CSize(m_nGridHorzSpace, m_nGridVertSpace);
	if (m_nHasGrid)
	{
		tstinfo.rgb = m_clrbtnGridline.GetColor();
		tstinfo.eTesttype = eTestTypeGrid;
	}
	else
	{
		tstinfo.rgb = m_clrbtnBackgrd.GetColor();
		tstinfo.eTesttype = eTestTypeColor;
	}

	// 命令执行状态跟踪
	InOutKey UpState[_MAX_INPUT_PORT];
	int nSelectCount = 0;
	int nRet = INVALID_VALUE;

	int nID = INVALID_VALUE;
	int nCmdPort = INVALID_VALUE;
	for (int i=0; i<pList->GetItemCount(); i++)
	{
		if (pList->GetCheck(i))
		{
			UpState[nSelectCount].key = i;

			nID = pManager->GetID(i);
			nCmdPort = pManager->GetCmdPort(nID);			

			nRet = GetCmdManager()->CmdImageTest(nCmdPort, m_Tab.GetCurSel()==0 ? eCardTypeInput:eCardTypeOutput, &tstinfo);

			if ( nRet == CMD_FAILURE )
				UpState[nSelectCount].val = 0;
			else
				UpState[nSelectCount].val = 1;

			nSelectCount++;
		}
	}

	//显示更新结果
	CRedrawLock lock(pList);

	for (int i=0; i<nSelectCount; i++)	{
		if (UpState[i].val > 0) {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			pList->SetItemText(UpState[i].key, 2, "成功！");
		}
		else {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			pList->SetItemText(UpState[i].key, 2, "失败！");
		}
	}
}

void CTestPatternDlg::OnBtnCancelTest()
{
	ResetListItemColor();

	UpdateData();

	CInputManager *pManager = NULL;
	CXTPListCtrl *pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
		pManager = (CInputManager*)GetInputManager();
	}
	else
	{
		pList = &m_listOutput;
		pManager = (COutputManager*)GetOutputManager();
	}

	// 测试数据
	IMGTESTINFO tstinfo;
	tstinfo.ptBegin = m_ptBegin;
	tstinfo.iLineWidth = m_nGridlineWid;
	tstinfo.szLineSpace = CSize(m_nGridHorzSpace, m_nGridVertSpace);

	if (m_nHasGrid) {
		tstinfo.rgb = m_clrbtnGridline.GetColor();
		tstinfo.eTesttype = eTestTypeCancelGrid;
	}
	else {
		tstinfo.rgb = m_clrbtnBackgrd.GetColor();
		tstinfo.eTesttype = eTestTypeCancelColor;
	}

	InOutKey UpState[_MAX_INPUT_PORT];
	int nSelectCount = 0;
	int nRet = INVALID_VALUE;
	int nID = INVALID_VALUE;
	int nCmdPort = INVALID_VALUE;

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		if (pList->GetCheck(i))
		{
			nID = pManager->GetID(i);
			nCmdPort = pManager->GetCmdPort(nID);

			UpState[nSelectCount].key = i;

			nRet = GetCmdManager()->CmdImageTest(nCmdPort, m_Tab.GetCurSel()==0 ? eCardTypeInput:eCardTypeOutput, &tstinfo);

			if ( nRet == CMD_FAILURE )
				UpState[nSelectCount].val = 0;
			else
				UpState[nSelectCount].val = 1;

			nSelectCount++;
		}
	}

	//显示更新结果
	CRedrawLock lock(pList);

	for (int i=0; i<nSelectCount; i++)	{
		if (UpState[i].val > 0) {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			pList->SetItemText(UpState[i].key, 2, "成功！");
		}
		else {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			pList->SetItemText(UpState[i].key, 2, "失败！");
		}
	}
}


void CTestPatternDlg::ResetListItemColor()
{
	CXTPListCtrl* pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		pList = &m_listOutput;
	}

	CRedrawLock lock(pList);

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		pList->SetItemText(i, 2, "");
		if (pList->GetCheck(i))
		{
			pList->SetRowColor(i, ListColorText, ListColorItemSelected);
		}
		else
		{
			pList->SetRowColor(i, ListColorText, ListColorBackground);
		}
	}
}

void CTestPatternDlg::OnBtnBkColor()
{
	GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->Invalidate();
}

void CTestPatternDlg::OnBtnLineColor()
{
	GetDlgItem(IDC_PIC_PATTERN_PREVIEW)->Invalidate();
}

void CTestPatternDlg::OnTcnSelchangeTabPort(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_Tab.GetCurSel() == 0)
	{
		m_listInput.ShowWindow(SW_SHOW);
		m_listOutput.ShowWindow(SW_HIDE);
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		m_listInput.ShowWindow(SW_HIDE);
		m_listOutput.ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

void CTestPatternDlg::OnBnClickedBtnPatternPrintOsd()
{
	ResetListItemColor();
	UpdateData();	

	CInputManager* pManager = NULL;
	CXTPListCtrl *pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
		pManager = (CInputManager*)GetInputManager();
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		pList = &m_listOutput;
		pManager = (COutputManager*)GetOutputManager();
	}

	int nID = INVALID_VALUE;
	int nCmdPort = INVALID_VALUE;
	int nSelectCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		if (pList->GetCheck(i))
		{
			nID = pManager->GetID(i);
			nCmdPort = pManager->GetCmdPort(nID);

			UpState[nSelectCount].key = i;

			nRet = GetCmdManager()->CmdYOSD(nCmdPort, m_Tab.GetCurSel()==0 ? eCardTypeInput:eCardTypeOutput);

			UpState[nSelectCount].val = (nRet==INVALID_VALUE) ? 0 : 1;

			nSelectCount++;
		}
	}

	//显示更新结果
	CRedrawLock lock(pList);

	for (int i=0; i<nSelectCount; i++)	{
		if (UpState[i].val > 0) {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			pList->SetItemText(UpState[i].key, 2, "成功！");
		}
		else {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			pList->SetItemText(UpState[i].key, 2, "失败！");
		}
	}
}

void CTestPatternDlg::OnBnClickedBtnPatternCancelOsd()
{
	ResetListItemColor();
	UpdateData();

	CInputManager* pManager = NULL;
	CXTPListCtrl *pList = NULL;
	if (m_Tab.GetCurSel() == 0)
	{
		pList = &m_listInput;
		pManager = (CInputManager*)GetInputManager();
	}
	else if (m_Tab.GetCurSel() == 1)
	{
		pList = &m_listOutput;
		pManager = (COutputManager*)GetOutputManager();
	}

	int nID = INVALID_VALUE;
	int nCmdPort = INVALID_VALUE;
	int nSelectCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	for (int i=0; i<pList->GetItemCount(); i++)
	{
		if (pList->GetCheck(i))
		{
			nID = pManager->GetID(i);
			nCmdPort = pManager->GetCmdPort(nID);

			UpState[nSelectCount].key = i;

			nRet = GetCmdManager()->CmdCOSD(nCmdPort, m_Tab.GetCurSel()==0 ? eCardTypeInput:eCardTypeOutput);

			UpState[nSelectCount].val = (nRet==INVALID_VALUE) ? 0 : 1;

			nSelectCount++;
		}
	}

	//显示更新结果
	CRedrawLock lock(pList);

	for (int i=0; i<nSelectCount; i++)	{
		if (UpState[i].val > 0) {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			pList->SetItemText(UpState[i].key, 2, "成功！");
		}
		else {
			pList->SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			pList->SetItemText(UpState[i].key, 2, "失败！");
		}
	}
}
