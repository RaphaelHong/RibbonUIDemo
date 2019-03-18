// SceneTourDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "..\MainFrm.h"
#include "SceneLoopDlg.h"
#include "afxdialogex.h"

#define TIMER_LOOP 1000

inline CString MilliSecond2String(long lTimeMS)
{
	int nTm = lTimeMS/1000;
	CString str;
	str.Format(_T("%d:%02d:%02d"), nTm/3600, (nTm%3600)/60, nTm%60);

	return str;
}

// CSceneLoopDlg 对话框

IMPLEMENT_DYNAMIC(CSceneLoopDlg, CDialog)

CSceneLoopDlg::CSceneLoopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneLoopDlg::IDD, pParent)
	, m_nHour(0)
	, m_nMinute(0)
	, m_nSecond(5)
{

}

CSceneLoopDlg::~CSceneLoopDlg()
{
}

void CSceneLoopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_TIME_H, m_spinH);
	DDX_Control(pDX, IDC_SPIN_TIME_M, m_spinM);
	DDX_Control(pDX, IDC_SPIN_TIME_S, m_spinS);
	DDX_Control(pDX, IDC_EDIT_TIME_H, m_editHour);
	DDX_Control(pDX, IDC_EDIT_TIME_M, m_editMinute);
	DDX_Control(pDX, IDC_EDIT_TIME_S, m_editSecond);
	DDX_Control(pDX, IDC_LIST_SCENE_TOUR, m_Listscene);
	DDX_Text(pDX, IDC_EDIT_TIME_H, m_nHour);
	DDX_Text(pDX, IDC_EDIT_TIME_M, m_nMinute);
	DDX_Text(pDX, IDC_EDIT_TIME_S, m_nSecond);
	//DDV_MinMaxInt(pDX, m_nHour, 0, 23);
	//DDV_MinMaxInt(pDX, m_nMinute, 0, 59);
	//DDV_MinMaxInt(pDX, m_nSecond, 0, 59);	
}


BEGIN_MESSAGE_MAP(CSceneLoopDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ALL, &CSceneLoopDlg::OnBtnAll)
	ON_BN_CLICKED(IDC_BTN_NON, &CSceneLoopDlg::OnBtnNon)
	ON_BN_CLICKED(IDC_BTN_INVERT, &CSceneLoopDlg::OnBtnInv)
	ON_BN_CLICKED(IDC_BTN_LIST_UP, &CSceneLoopDlg::OnBtnMoveItemUp)
	ON_BN_CLICKED(IDC_BTN_LIST_DOWN, &CSceneLoopDlg::OnBtnMoveItemDown)
	ON_BN_CLICKED(IDC_BTN_LIST_TOP, &CSceneLoopDlg::OnBtnMoveItemTop)
	ON_BN_CLICKED(IDC_BTN_LIST_BOTTOM, &CSceneLoopDlg::OnBtnMoveItemBottom)
	ON_BN_CLICKED(IDOK, &CSceneLoopDlg::OnOK)
	ON_BN_CLICKED(IDCANCEL, &CSceneLoopDlg::OnCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCENE_TOUR, &CSceneLoopDlg::OnNMClickListItem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCENE_TOUR, &CSceneLoopDlg::OnLvnItemChangeList)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_TIME_H, OnEnChangeEditHour)
	ON_EN_CHANGE(IDC_EDIT_TIME_M, OnEnChangeEditMinute)
	ON_EN_CHANGE(IDC_EDIT_TIME_S, OnEnChangeEditSecond)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSceneLoopDlg 消息处理程序
BOOL CSceneLoopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	m_spinH.SetRange(0, 23);
	m_spinM.SetRange(0, 59);
	m_spinS.SetRange(0, 59);

	InitListContent();

	return TRUE;
}


void CSceneLoopDlg::InitListContent()
{
	DWORD dwStyle;
	dwStyle = m_Listscene.GetExtendedStyle(); 
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES|LVS_EX_CHECKBOXES;
	m_Listscene.SetExtendedStyle(dwStyle); 

	m_Listscene.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);
	m_Listscene.ModifyExtendedStyle(0, LVS_EX_CHECKBOXES);
	m_Listscene.EnableUserSortColor(true);
	m_Listscene.EnableUserListColor(true);
	m_Listscene.EnableUserRowColor(true);
//	m_Listscene.SetListTextColor(RGB(128, 0, 255));
	m_Listscene.SetListBackColor(ListColorBackground);
	m_Listscene.RedrawWindow();

	//场景管理对话框中的图标资源与RibbonBar所用的图标来源不一样
	HICON icon[9];
	icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_BOARD);
	icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_EXCEL);
	icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_FILE);
	icon[3] = AfxGetApp()->LoadIcon(IDI_ICON_GRAPH);
	icon[4] = AfxGetApp()->LoadIcon(IDI_ICON_MEDIA);
	icon[5] = AfxGetApp()->LoadIcon(IDI_ICON_STRUCT);
	icon[6] = AfxGetApp()->LoadIcon(IDI_ICON_VISO);
	icon[7] = AfxGetApp()->LoadIcon(IDI_ICON_WINDOWS);
	icon[8] = AfxGetApp()->LoadIcon(IDI_ICON_WORD);

	m_imglist16.Create(16, 16, ILC_COLOR24, 9, 9);
	m_imglist16.Add(icon[0]);
	m_imglist16.Add(icon[1]);
	m_imglist16.Add(icon[2]);
	m_imglist16.Add(icon[3]);
	m_imglist16.Add(icon[4]);
	m_imglist16.Add(icon[5]);
	m_imglist16.Add(icon[6]);
	m_imglist16.Add(icon[7]);
	m_imglist16.Add(icon[8]);

	int nColumn = 0;
	m_Listscene.InsertColumn(nColumn++, _T("序号"), LVIF_TEXT , 50);
	m_Listscene.InsertColumn(nColumn++, _T("场景ID"), LVCFMT_RIGHT, 100);
	m_Listscene.InsertColumn(nColumn++, _T("场景名称"), LVCFMT_RIGHT, 100);
	m_Listscene.InsertColumn(nColumn++, _T("执行时间"), LVCFMT_RIGHT, 80);
	m_Listscene.InsertColumn(nColumn++, _T("剩余时间"), LVCFMT_RIGHT, 80);

	m_Listscene.SetImageList(&m_imglist16, LVSIL_SMALL);

	UpdateListItem(0);
}

void CSceneLoopDlg::UpdateListItem(int nFocusedItem)
{
	//先清除列表内容
	m_Listscene.DeleteAllItems();

	int nCount = GetSceneManager()->GetCount();

	int nImage;
	CString strText;
	CSceneHeadInfo* pScHeadInfo;
	
	for (int i=0; i<nCount; i++)
	{
		pScHeadInfo = GetSceneManager()->GetAt(i);
		ASSERT(pScHeadInfo);
		//插入新值
		strText.Format(_T("%d"), i);
		m_Listscene.InsertItem(i, strText, -1);
		//场景ID
		strText.Format(_T("%d"), pScHeadInfo->GetID());
		nImage = pScHeadInfo->GetSmallImage();
		m_Listscene.SetItem(i, 1, LVIF_TEXT|LVIF_IMAGE,  strText, nImage, 0, 0, 0, 0);
		//场景名称
		strText.Format(_T("%s"), pScHeadInfo->GetName());
		m_Listscene.SetItem(i, 2, LVIF_TEXT|LVIF_IMAGE, strText, -1, 0, 0, 0, 0);
		//执行时间
		strText = MilliSecond2String(pScHeadInfo->GetExcuteTime());
		m_Listscene.SetItem(i, 3, LVIF_TEXT|LVIF_IMAGE, strText, -1, 0, 0, 0, 0);
		//剩余时间
		strText = MilliSecond2String(pScHeadInfo->GetRemainTime());
		m_Listscene.SetItem(i, 4, LVIF_TEXT, strText, -1, 0, 0, 0, 0);
	}

	if (nCount > 0 && nFocusedItem < nCount)
	{
		m_Listscene.SetItemState(nFocusedItem, LVIS_SELECTED, LVIS_SELECTED);

		pScHeadInfo = GetSceneManager()->GetAt(0);
		m_nHour = pScHeadInfo->GetExcuteTime()/3600000;
		m_nMinute = (pScHeadInfo->GetExcuteTime()/1000)/60;
		m_nSecond = (pScHeadInfo->GetExcuteTime()/1000)%60;
	}

	UpdateData(FALSE);
}

void CSceneLoopDlg::OnBtnAll()
{
	m_Listscene.SetRedraw(FALSE);

	for (int i=0; i<m_Listscene.GetItemCount(); i++)
	{
		m_Listscene.SetCheck(i, TRUE);
	}

	m_Listscene.SetRedraw(TRUE);
	m_Listscene.RedrawWindow();
}

void CSceneLoopDlg::OnBtnNon()
{
	m_Listscene.SetRedraw(FALSE);

	for (int i=0; i<m_Listscene.GetItemCount(); i++)
	{
		m_Listscene.SetCheck(i, FALSE);
	}

	m_Listscene.SetRedraw(TRUE);
	m_Listscene.RedrawWindow();
}

void CSceneLoopDlg::OnBtnInv()
{
	m_Listscene.SetRedraw(FALSE);

	for (int i=0; i<m_Listscene.GetItemCount(); i++)
	{
		m_Listscene.SetCheck(i, !m_Listscene.GetCheck(i));
	}

	m_Listscene.SetRedraw(TRUE);
	m_Listscene.RedrawWindow();
}

void CSceneLoopDlg::OnBtnMoveItemUp()
{
	int nItem = GetSelectItem();
	if (nItem == INVALID_VALUE) return;

	//已经在最上面
	if (nItem == 0)
		return;
	
	//SwapListItem(nItem, nItem-1);
	MoveItemTo(nItem, nItem-1);
}

void CSceneLoopDlg::OnBtnMoveItemDown()
{
	int nItem = GetSelectItem();
	if (nItem == INVALID_VALUE) return;

	//已经在最下面
	if (nItem == m_Listscene.GetItemCount()-1)
		return;

	MoveItemTo(nItem, nItem+1);
	//SwapListItem(nItem, nItem+1);
}

void CSceneLoopDlg::OnBtnMoveItemTop()
{
	int nItem = GetSelectItem();
	if (nItem == INVALID_VALUE) return;

	//已经在最上面
	if (nItem == 0)
		return;

	MoveItemTo(nItem, 0);
}

void CSceneLoopDlg::OnBtnMoveItemBottom()
{
	int nItem = GetSelectItem();
	if (nItem == INVALID_VALUE) return;

	//已经在最下面
	if (nItem == m_Listscene.GetItemCount()-1)
		return;

	MoveItemTo(nItem, m_Listscene.GetItemCount()-1);
}

int CSceneLoopDlg::GetSelectItem() const
{
	int nItem = INVALID_VALUE;

	for (int i=0; i<m_Listscene.GetItemCount(); i++)
	{
		if (m_Listscene.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			nItem = i;
			break;
		}
	}

	return nItem;
}

void CSceneLoopDlg::SwapListItem(int nItemSrc, int nItemDest)
{
	BOOL bCheckSrc  = m_Listscene.GetCheck(nItemSrc);
	BOOL bCheckDest = m_Listscene.GetCheck(nItemDest);

	LVITEM lviItemA, lviItemB, lviItemC;
	lviItemA.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;
	lviItemA.pszText = new CHAR[_MAX_SCENE_NAME];
	lviItemA.cchTextMax = _MAX_SCENE_NAME;
	lviItemA.iItem = nItemSrc;

	lviItemB.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;
	lviItemB.pszText = new CHAR[_MAX_SCENE_NAME];
	lviItemB.cchTextMax = _MAX_SCENE_NAME;
	lviItemB.iItem = nItemDest;

	lviItemC.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;

	int nColumn = m_Listscene.GetHeaderCtrl()->GetItemCount();
	for (int iCol=0; iCol<nColumn; iCol++)
	{
		lviItemA.iSubItem = iCol;
		lviItemB.iSubItem = iCol;
		m_Listscene.GetItem(&lviItemA);
		m_Listscene.GetItem(&lviItemB);

		lviItemC = lviItemA;
		lviItemC.iItem = nItemDest;
		if (iCol == 0)
			lviItemC.pszText = lviItemB.pszText;
		m_Listscene.SetItem(&lviItemC);

		lviItemC = lviItemB;
		lviItemC.iItem = nItemSrc;
		if (iCol == 0)
			lviItemC.pszText = lviItemA.pszText;
		m_Listscene.SetItem(&lviItemC);
	}

	m_Listscene.SetCheck(nItemSrc, bCheckDest);
	m_Listscene.SetCheck(nItemDest, bCheckSrc);

	m_Listscene.SetItemState(nItemDest, LVIS_SELECTED, LVIS_SELECTED);
	m_Listscene.SetItemState(nItemSrc, ~LVIS_SELECTED, LVIS_SELECTED);

	delete [] lviItemA.pszText;
	delete [] lviItemB.pszText;
}

void CSceneLoopDlg::MoveItemTo(int nItem, int nPos)
{
	m_Listscene.SetRedraw(FALSE);

	CString strID, strName, strLoopTotalTime, strLoopLeftTime;
	int iImage;
	BOOL bCheckedOrg, bCheckedDst;

	//获取该项信息
	LVITEM lvi = {0};
	lvi.mask = LVIF_IMAGE|LVIF_TEXT;
	lvi.iItem = nItem;
	lvi.iSubItem = 1;
	m_Listscene.GetItem(&lvi);
	iImage = lvi.iImage;

	strID = m_Listscene.GetItemText(nItem, 1);
	strName = m_Listscene.GetItemText(nItem, 2);
	strLoopTotalTime = m_Listscene.GetItemText(nItem, 3);
	strLoopLeftTime = m_Listscene.GetItemText(nItem, 4);

	bCheckedOrg = m_Listscene.GetCheck(nItem);
	bCheckedDst = m_Listscene.GetCheck(nPos);

	//删除该项
	m_Listscene.DeleteItem(nItem);

	//将该项插入到新的位置
	//插入新值
	m_Listscene.InsertItem(nPos, _T("0"), -1);
	//场景ID
	m_Listscene.SetItem(nPos, 1, LVIF_TEXT|LVIF_IMAGE,  strID, iImage, 0, 0, 0, 0);
	//场景名称
	m_Listscene.SetItem(nPos, 2, LVIF_TEXT, strName, -1, 0, 0, 0, 0);
	//执行时间
	m_Listscene.SetItem(nPos, 3, LVIF_TEXT, strLoopTotalTime, -1, 0, 0, 0, 0);
	//剩余时间
	m_Listscene.SetItem(nPos, 4, LVIF_TEXT, strLoopLeftTime, -1, 0, 0, 0, 0);

	//更新索引
	CString strIndex;
	for (int i=0; i<m_Listscene.GetItemCount(); i++)
	{
		strIndex.Format(_T("%d"), i);
		m_Listscene.SetItemText(i, 0, strIndex);
	}

	m_Listscene.SetCheck(nPos, bCheckedOrg);
	m_Listscene.SetItemState(nPos, LVIS_SELECTED, LVIS_SELECTED);

	if (bCheckedDst)
		m_Listscene.SetRowColor(nItem, ListColorText, ListColorItemSelected);
	else
		m_Listscene.SetRowColor(nItem, ListColorText, ListColorBackground);

	m_Listscene.SetRedraw(TRUE);
	m_Listscene.RedrawWindow();
}

void CSceneLoopDlg::OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;

	if (nItem < 0)
		EnableTimeEditor(FALSE);
	else
	{
		EnableTimeEditor(TRUE);

		char szTime[10];
		m_Listscene.GetItemText(nItem, 3, szTime, 10);

		sscanf_s(szTime, "%d:%02d:%02d", &m_nHour, &m_nMinute, &m_nSecond);

		UpdateData(FALSE);
	}

	*pResult = 0;
}

void CSceneLoopDlg::OnLvnItemChangeList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = pNMLV->iItem;

	if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1) )
		&& ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2) ) ) //UnChecked to Checked
	{ 
		m_Listscene.SetRowColor(nItem, ListColorText, ListColorItemSelected);
	}
	else if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2) )
		&&  ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1) ) ) //Checked to UnChecked
	{
		m_Listscene.SetRowColor(nItem, ListColorText, ListColorBackground);
	}


	*pResult = 0;
}

void CSceneLoopDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_LOOP)
	{
		if (!CSceneLoopManager::IsLoopping())
			return CDialog::OnTimer(nIDEvent);

		UpdateRemainTime();
	}

	CDialog::OnTimer(nIDEvent);
}


void CSceneLoopDlg::EnableTimeEditor(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_TIME_H)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TIME_M)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TIME_S)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_TIME_H)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_TIME_M)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_TIME_S)->EnableWindow(bEnable);

	GetDlgItem(IDC_BTN_LIST_TOP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_LIST_UP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_LIST_BOTTOM)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_LIST_DOWN)->EnableWindow(bEnable);
}

void CSceneLoopDlg::OnOK()
{
	CSceneLoopManager::Lock();

	if (CSceneLoopManager::IsLoopping())
	{//停止循环
		
		CSceneLoopManager::SetLoopState(THD_LOOP_IDLE);
		KillTimer(TIMER_LOOP);
	}
	else
	{//开始循环   
		UpdateLoopItem();

		if (GetSceneManager()->BeginLoop())
		{
			SetTimer(TIMER_LOOP, 1000, NULL);
		}		
	}

	//更新按钮名
	if (CSceneLoopManager::IsLoopping())
	{
		GetDlgItem(IDOK)->SetWindowText(_T("停止(&P)"));
		GetDlgItem(IDC_LIST_SCENE_TOUR)->EnableWindow(FALSE);
		EnableTimeEditor(FALSE);
	}
	else
	{
		GetDlgItem(IDOK)->SetWindowText(_T("开始(&S)"));
		GetDlgItem(IDC_LIST_SCENE_TOUR)->EnableWindow(TRUE);
		EnableTimeEditor(TRUE);
	}

	CSceneLoopManager::UnLock();

	//CDialog::OnOK();
}

void CSceneLoopDlg::UpdateLoopItem()
{

	int hour, minite, sec;
	int nLen = 0;
	int nSize = m_Listscene.GetItemCount();
	long dwTime = 0;
	CString strItem;

	CMemoryAllocate<int> alloc(nSize);
	ZeroMemory(alloc.GetAddress(), sizeof(int)*nSize);
	
	for (int i=0; i<nSize; i++)
	{
		if (m_Listscene.GetCheck(i))
		{
			// 选中场景的ID
			strItem = m_Listscene.GetItemText(i, 1);
			alloc.GetAddress()[nLen++] = atoi(strItem);

			// 选中场景的执行时间
			strItem = m_Listscene.GetItemText(i, 3);
			sscanf_s(strItem.GetBuffer(), _T("%d:%02d:%02d"), &hour, &minite, &sec);

			// 重置剩余时间
			m_Listscene.SetItemText(i, 4, strItem);

			//dwTime = (hour*3600+minite*60+sec)*1000;
		}
	}

	GetSceneManager()->PrepareLoop(alloc.GetAddress(), nLen);
}

void CSceneLoopDlg::UpdateRemainTime()
{	

	int nIndex = 0;
	int nSize = GetSceneManager()->GetLoopManager()->GetCount();
	CLoopItem* pItem = GetSceneManager()->GetLoopManager()->GetOnturnItem(nIndex);
	if (pItem == NULL)
		return;

	int nPrevID = -1;
	if (nIndex == 0)
		nIndex = nSize > 1 ? nSize-1 : nIndex;
	else
		nIndex--;

	nPrevID = GetSceneManager()->GetLoopManager()->GetLoopItem(nIndex)->nID;

	CString strItem;
	int nID = 0;
	for (int i=0; i<m_Listscene.GetItemCount(); i++)
	{
		if (m_Listscene.GetCheck(i))
		{
			strItem = m_Listscene.GetItemText(i, 1);
			nID = atoi(strItem);

			if (nID == pItem->nID)
			{
				m_Listscene.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

				strItem = MilliSecond2String(pItem->dwRemain);

				m_Listscene.SetItemText(i, 4, strItem);
			}
			else if (nID == nPrevID)
			{
				m_Listscene.SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
			}
		}
	}
}

void CSceneLoopDlg::OnEnChangeEditHour()
{
	if (m_editHour.GetSafeHwnd())
	{
		UpdateExcuteTime();
	}
}

void CSceneLoopDlg::OnEnChangeEditMinute()
{
	if (m_editMinute.GetSafeHwnd())
	{
		UpdateExcuteTime();
	}
}

void CSceneLoopDlg::OnEnChangeEditSecond()
{
	if (m_editSecond.GetSafeHwnd())
	{
		UpdateExcuteTime();
	}
}

void CSceneLoopDlg::UpdateExcuteTime()
{
	UpdateData();

	if (m_nHour < 0)    m_nHour = 0;
	if (m_nHour > 23)   m_nHour = 23;
	if (m_nMinute < 0)  m_nMinute = 0;
	if (m_nMinute > 59) m_nMinute = 59;
	if (m_nSecond < 0)  m_nSecond = 0;
	if (m_nSecond > 59) m_nSecond = 59;

	if (m_nHour == 0 && m_nMinute == 0 && m_nSecond < 5) m_nSecond = 5;

	CString str;
	str.Format(_T("%d:%02d:%02d"), m_nHour, m_nMinute, m_nSecond);

	POSITION pos = m_Listscene.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{ 
		int nIndex = m_Listscene.GetNextSelectedItem(pos);
		m_Listscene.SetItemText(nIndex, 3, str);
		m_Listscene.SetItemText(nIndex, 4, str);

		int nID = atoi(m_Listscene.GetItemText(nIndex, 1));

		GetSceneManager()->FindItemByID(nID)->GetExcuteTime() = \
			(m_nHour*3600+m_nMinute*60+m_nSecond)*1000;
	}

	UpdateData(FALSE);
}

void CSceneLoopDlg::OnClose()
{
	if (LoopRemind())
		CDialog::OnClose();
}

void CSceneLoopDlg::OnCancel()
{
	if (LoopRemind())
		CDialog::OnCancel();
}

BOOL CSceneLoopDlg::LoopRemind()
{
	// 退出对话框时提醒用户正在执行场景循环
	if (CSceneLoopManager::IsLoopping())
	{
		AfxMessageBox(_T("正在执行场景轮巡，请先停止轮巡后再关闭对话框！"), MB_OK|MB_ICONWARNING);

		return FALSE;
	}

	// 先退出线程再退出对话框
	GetSceneManager()->GetLoopManager()->ExitLoopThread();

	return TRUE;
}