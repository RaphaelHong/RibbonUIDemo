// WndProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "..\GridView.h"
#include "WndProperty.h"


// CWndProperty 对话框

IMPLEMENT_DYNAMIC(CWndProperty, CDialog)

CWndProperty::CWndProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CWndProperty::IDD, pParent)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_nSizeCX(0)
	, m_nSizeCY(0)
{
	m_pPage = NULL;
}

CWndProperty::CWndProperty(CDrawWndPage* pPage, SPLICELAYOUT layout, CWnd* pParent /*=NULL*/)
	: CDialog(CWndProperty::IDD, pParent)
	, m_nPosX(0)
	, m_nPosY(0)
	, m_nSizeCX(0)
	, m_nSizeCY(0)
{
	m_pPage = pPage;
	m_Layout = layout;
}

CWndProperty::~CWndProperty()
{
}

void CWndProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINDOWS, m_ListWnd);
	DDX_Control(pDX, IDC_EDIT_WND_POS_X, m_EditPosX);
	DDX_Control(pDX, IDC_EDIT_WND_POS_Y, m_EditPosY);
	DDX_Control(pDX, IDC_EDIT_WND_SIZE_CX, m_EditSizeCX);
	DDX_Control(pDX, IDC_EDIT_WND_SIZE_CY, m_EditSizeCY);
	DDX_Control(pDX, IDC_SPIN_WND_POS_X, m_SBtnPosX);
	DDX_Control(pDX, IDC_SPIN_WND_POS_Y, m_SBtnPoxY);
	DDX_Control(pDX, IDC_SPIN_WND_SIZE_CX, m_SBtnSizeCX);
	DDX_Control(pDX, IDC_SPIN_WND_SIZE_CY, m_SBtnSizeCY);
	DDX_Control(pDX, IDC_EDIT_WND_NAME, m_EditName);

	DDX_Text(pDX, IDC_EDIT_WND_POS_X, m_nPosX);
	DDX_Text(pDX, IDC_EDIT_WND_POS_Y, m_nPosY);
	DDX_Text(pDX, IDC_EDIT_WND_SIZE_CX, m_nSizeCX);
	DDX_Text(pDX, IDC_EDIT_WND_SIZE_CY, m_nSizeCY);
}


BEGIN_MESSAGE_MAP(CWndProperty, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_WINDOWS, &CWndProperty::OnLbnSelchangeListWindows)
	ON_BN_CLICKED(IDC_BTN_WND_PROPERTY_APPLY, &CWndProperty::OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_WND_PROPERTY_ACTIVE, &CWndProperty::OnBnClickedBtnWndPropertyActive)
	ON_BN_CLICKED(IDC_BTN_WND_PROPERTY_LOCK, &CWndProperty::OnBtnLock)
END_MESSAGE_MAP()


// CWndProperty 消息处理程序
BOOL CWndProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	m_SBtnPosX.SetRange32(-65535, 65535);
	m_SBtnPoxY.SetRange32(-65535, 65535);
	m_SBtnSizeCX.SetRange32(-65535, 65535);
	m_SBtnSizeCY.SetRange32(-65535, 65535);

	InitListBox();

	CString strBkgInfo;
	strBkgInfo.Format(_T("1.背景：水平<%4d * %d = %4d> \n\t  垂直<%4d * %d = %4d> \n\n2.列表按窗口的层叠顺序排列！"),\
		              m_Layout.BkgTiming.m_HActive, m_Layout.iPhyHorz, m_Layout.iPhyHorzPixel,\
					  m_Layout.BkgTiming.m_VActive, m_Layout.iPhyVert, m_Layout.iPhyVertPixel );
	GetDlgItem(IDC_LABEL_BKG_INFO)->SetWindowText(strBkgInfo);

	return TRUE;
}

void CWndProperty::InitListBox()
{
	if (m_pPage == NULL)
		return;

	int nWndNum = m_pPage->GetWndCount();
	CString strItem;
	CDrawWndObj* pWndObj =NULL;

	if (nWndNum == 0)
	{
		m_ListWnd.AddString(_T("空"));
		m_ListWnd.SetCurSel(0);

		m_EditName.SetWindowText(_T("空"));
		m_EditName.SetSel(0, m_EditName.GetWindowTextLength());

		EnableEditPos(FALSE);
		EnableEditName(FALSE);
		return;
	}

	HICON icon,iconLock;
	icon = AfxGetApp()->LoadIcon(IDI_ICON_WINDOWS);
	iconLock = AfxGetApp()->LoadIcon(IDI_ICON_WINDOW_LOCKED);
	m_imagelist.Create(16, 16, ILC_COLOR32|ILC_MASK, 1, 1);
	//m_imagelist.Create(IDB_CATEGORY_MAIN_SMALL, 16, 16, RGB(0, 0, 0));
	m_imagelist.Add(icon);
	m_imagelist.Add(iconLock);

	m_ListWnd.SetImageList(&m_imagelist);

	int nActived = 0;
	for (int i=0; i<nWndNum; i++)
	{
		pWndObj = m_pPage->GetWndObj(i);
		if (pWndObj != NULL)
		{
			strItem.Format(_T("%2d - [%d.%s]"), i+1, pWndObj->GetWndID(), pWndObj->GetName());
			if (pWndObj->IsLocked())
				strItem += _T("(已锁定)");

			m_ListWnd.AddString(strItem);
			m_ListWnd.SetItemImage(i, pWndObj->IsLocked() ? 1 : 0);

			if (pWndObj->IsActived())
				nActived = i;
		}
	}
	
	m_ListWnd.SetCurSel(nActived);
	GetWndInfo(nActived);
}

void CWndProperty::OnBtnApply()
{
	int nCursel = m_ListWnd.GetCurSel();
	if (nCursel == LB_ERR)
		return;

	UpdateData();

	if (!DataCheck())
		return;

	SetWndInfo(nCursel);
}

void CWndProperty::GetWndInfo(int nIndex)
{
	if (m_pPage == NULL)
		return;

	CDrawWndObj* pWndObj = m_pPage->GetWndObj(nIndex);
	if (pWndObj == NULL)
		return;

	CString str;
	str.Format(_T("%s"), pWndObj->GetName());
	m_EditName.SetWindowText(str);

	m_nPosX = pWndObj->GetPhyPos().left;
	m_nPosY = pWndObj->GetPhyPos().top;
	m_nSizeCX = pWndObj->GetPhyPos().Width();
	m_nSizeCY = pWndObj->GetPhyPos().Height();

	EnableEditPos(!pWndObj->IsLocked());

	UpdateData(FALSE);
}

void CWndProperty::SetWndInfo(int nIndex)
{
	if (m_pPage == NULL)
		return;

	CDrawWndObj* pWndObj = m_pPage->GetWndObj(nIndex);
	if (pWndObj == NULL)
		return;

	m_ListWnd.DeleteString(nIndex);

	CString str;
	m_EditName.GetWindowText(str);

	pWndObj->SetName(str.GetBuffer());
	pWndObj->SetPhyPos(CRect(m_nPosX, m_nPosY, m_nPosX+m_nSizeCX, m_nPosY+m_nSizeCY));
	pWndObj->Size();
	m_pPage->Repaint(FALSE);

	str.Format(_T("%2d - [%d.%s]"), nIndex+1, pWndObj->GetWndID(), pWndObj->GetName());
	if (pWndObj->IsLocked())
		str += _T("(已锁定)");

	m_ListWnd.InsertString(nIndex, str);
	m_ListWnd.SetItemImage(nIndex, pWndObj->IsLocked() ? 1 : 0);
	m_ListWnd.SetCurSel(nIndex);
}

void CWndProperty::OnLbnSelchangeListWindows()
{
	int nCursel = m_ListWnd.GetCurSel();

	if (nCursel == LB_ERR)
		return;

	GetWndInfo(nCursel);
}

void CWndProperty::EnableEditPos(BOOL bEnable/* =TRUE */)
{
	m_EditPosX.EnableWindow(bEnable);
	m_EditPosY.EnableWindow(bEnable);
	m_EditSizeCX.EnableWindow(bEnable);
	m_EditSizeCY.EnableWindow(bEnable);

	SetDlgItemText(IDC_BTN_WND_PROPERTY_LOCK, bEnable ? _T("锁定此窗口") : _T("解除锁定"));
}

void CWndProperty::EnableEditName(BOOL bEnable/* =TRUE */)
{
	m_EditName.EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_WND_PROPERTY_LOCK)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_WND_PROPERTY_APPLY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_WND_PROPERTY_ACTIVE)->EnableWindow(bEnable);
}

BOOL CWndProperty::DataCheck()
{
	CString str;
	m_EditName.GetWindowText(str);

	if (str.IsEmpty())
	{
		MessageBox(_T("请输入窗口名称！"));
		m_EditName.SetFocus();
		return FALSE;
	}

	if (m_nSizeCX < 20)
	{
		MessageBox(_T("窗口宽度太小（大于20）！"));
		m_EditSizeCX.SetSel(0, m_EditSizeCX.GetWindowTextLength());
		m_EditSizeCX.SetFocus();
		return FALSE;
	}

	if (m_nSizeCY < 20)
	{
		MessageBox(_T("窗口高度太小（大于20）！"));
		m_EditSizeCY.SetSel(0, m_EditSizeCY.GetWindowTextLength());
		m_EditSizeCY.SetFocus();
		return FALSE;
	}

	return TRUE;
}

void CWndProperty::OnBnClickedBtnWndPropertyActive()
{
	int nCursel = m_ListWnd.GetCurSel();
	if (nCursel == LB_ERR)
		return;

	CDrawWndObj* pObj = m_pPage->GetWndObj(nCursel);
	if (pObj != NULL)
		m_pPage->ActiveWnd(pObj);
}

void CWndProperty::OnBtnLock()
{
	int nCursel = m_ListWnd.GetCurSel();
	if (nCursel == LB_ERR)
		return;

	CDrawWndObj* pObj = m_pPage->GetWndObj(nCursel);
	if (pObj != NULL)
	{
		pObj->SetLock(!pObj->IsLocked());		

		m_ListWnd.DeleteString(nCursel);

		CString str;
		str.Format(_T("%2d - [%d.%s]"), nCursel+1, pObj->GetWndID(), pObj->GetName());
		if (pObj->IsLocked())
			str += _T("(已锁定)");

		m_ListWnd.InsertString(nCursel, str);		
		m_ListWnd.SetItemImage(nCursel, pObj->IsLocked() ? 1 : 0);
		m_ListWnd.SetCurSel(nCursel);

		EnableEditPos(!pObj->IsLocked());

		m_pPage->Repaint(FALSE);
	}	
}

BOOL CWndProperty::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBtnApply();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}