// OutputConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "OutputConfigDlg.h"
#include "ResolutionEditDlg.h"
#include "RenameDlg.h"

using namespace Gdiplus;
// COutputConfigDlg 对话框

IMPLEMENT_DYNAMIC(COutputConfigDlg, CDialog)

COutputConfigDlg::COutputConfigDlg(CWnd* pParent /*=NULL*/)
: CDialog(COutputConfigDlg::IDD, pParent)
, m_nTimingIndex(0)
{

}

COutputConfigDlg::~COutputConfigDlg()
{
}

void COutputConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_PORT_RESOLUTION, m_SliderTiming);
	DDX_Control(pDX, IDC_LIST_PORTS, m_listOutput);
	DDX_Control(pDX, IDC_CBOX_PORT_CFG_HZ, m_CBoxHz);
}


BEGIN_MESSAGE_MAP(COutputConfigDlg, CDialog)
	ON_WM_HSCROLL()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, SetTipText)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_ALL, &COutputConfigDlg::OnBtnAll)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_NONE, &COutputConfigDlg::OnBtnNone)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_INVERT, &COutputConfigDlg::OnBtnInvert)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PORTS, &COutputConfigDlg::OnNMClickListOutput)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_APPLY, &COutputConfigDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_ADD_TIMING, &COutputConfigDlg::OnBnClickedBtnAddTiming)
	ON_BN_CLICKED(IDC_BTN_DEL_TIMING, &COutputConfigDlg::OnBnClickedBtnDelTiming)
	ON_BN_CLICKED(IDC_BTN_EDIT_TIMING, &COutputConfigDlg::OnBnClickedBtnEditTiming)
	ON_BN_CLICKED(IDC_BTN_GET_SLAVE_TIMING, &COutputConfigDlg::OnBnClickedBtnGetSlaveTiming)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PORTS, &COutputConfigDlg::OnNMRClickListOutput)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PORTS, &COutputConfigDlg::OnLvnItemChangedListOutput)
	ON_BN_CLICKED(IDC_BTN_PORT_RESET, &COutputConfigDlg::OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_PORT_RESET_ALL, &COutputConfigDlg::OnBtnResetAll)
END_MESSAGE_MAP()


// COutputConfigDlg 消息处理程序


BOOL COutputConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	CString strTitle;
	GetWindowText(strTitle);
	strTitle += _T(" - 输出");
	SetWindowText(strTitle);

	CWaitCursor wait;
	// 第一次配置需读取各端口的分辨率
	InitChannelTiming();

	// 加载输出分辨率配置文件
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\OutputResolution.ini");
	m_ResManager.SetConfigFilePath(strPath.GetBuffer(0));
	m_ResManager.LoadFromFile(NULL);

	// 初始化输出通道列表
	InitOutputList();	

	// 分辨率选择滑动条
	TIMING st = GetOutputManager()->GetTiming(0, FALSE);
	int nPos = m_ResManager.GetResolutionIndex(st);

	m_SliderTiming.SetRange(0, m_ResManager.GetResolutionCount()-1);
	m_SliderTiming.SetTic(1);
	m_SliderTiming.SetTicFreq(1);
	m_SliderTiming.SetPos(nPos);

	// 提示条
	SetTooltips();

	// 读取最近设置的分辨率
	CSysConfManager sysconf;
	CString strLastRes = sysconf.GetLastTryResolution(FALSE);
	GetDlgItem(IDC_LABEL_RECENT_RES_NAME)->SetWindowText(strLastRes);

	// 显示默认的分辨率
	ShowSelectedTiming(nPos);

	return TRUE;
}

void COutputConfigDlg::SetTooltips()
{
	EnableToolTips(TRUE);
	m_tips.Create(this);
	m_tips.AddTool(&m_SliderTiming, _T("拖动选择分辨率"));
	m_tips.SetTipBkColor(RGB(200, 200, 255));
	m_tips.SetTipTextColor(RGB(100, 150, 100));
}

void COutputConfigDlg::InitOutputList()
{
	//设置列表控件的样式
	m_listOutput.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);
	m_listOutput.ModifyExtendedStyle(0, LVS_EX_CHECKBOXES);
	m_listOutput.ModifyExtendedStyle(0, LVS_EX_GRIDLINES);
	m_listOutput.EnableUserSortColor(true);
	m_listOutput.EnableUserListColor(true);
	m_listOutput.EnableUserRowColor(true);
	m_listOutput.SetListBackColor(ListColorBackground);

	//插入列
	int nColumn = 0;
	m_listOutput.InsertColumn(nColumn++, _T("序号"), LVCFMT_RIGHT, 50);
	m_listOutput.InsertColumn(nColumn++, _T("名称"), LVCFMT_LEFT, 150);
	m_listOutput.InsertColumn(nColumn++, _T("分辨率"), LVCFMT_RIGHT, 150);
	m_listOutput.InsertColumn(nColumn++, _T("应用状态"), LVCFMT_CENTER, 120);

	CRedrawLock lock(&m_listOutput);

	//插入项
	COutputManager* pOutput = GetOutputManager();
	int nOutNum = GetOutputManager()->GetCount();
	CString strItem;
	TIMING st = {0};
	for (int i=0; i<nOutNum; i++)
	{
		// 列表显示的是物理信号的序号
		strItem.Format("%d", i+1);
		m_listOutput.InsertItem(LVIF_TEXT, i, strItem, 0, LVIS_SELECTED, 0, 0);

		// 名称
		strItem.Format("%s", GetOutputManager()->GetName(i, FALSE));
		m_listOutput.SetItemText(i, 1, strItem);

		// 添加新品种分辨率到配置文件
		st = GetOutputManager()->GetTiming(i, FALSE);
		if (m_ResManager.IsExist(st) == INVALID_VALUE && st.IsValid())
		{
			st.m_Type = CUSTOM_RESOLUTION;
			m_ResManager.AddResolution(FALSE, st);
		}

		m_listOutput.SetItemText(i, 2, m_ResManager.GetResolutionName(st));
	}

	if (nOutNum > 0)
		m_listOutput.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

BOOL COutputConfigDlg::SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pRes)  
{  
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;      
	UINT nID =pTTTStruct->idFrom;   
	if (pTTT->uFlags & TTF_IDISHWND)  
	{  
		nID = ::GetDlgCtrlID((HWND)nID);  
		switch(nID)  
		{  
		case(IDC_SLIDER_PORT_RESOLUTION):  
			{  
				CString str;
				str.Format("%s", m_ResManager.GetResolutionName(m_nTimingIndex));
				strncpy_s(pTTT->lpszText, 80, str.GetBuffer(), str.GetLength());
				return TRUE;  
			}
			break; 
		default:break;
		}  
	}  
	return FALSE;  
}  


void COutputConfigDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->IsKindOf(RUNTIME_CLASS(CSliderCtrl)))
	{
		// Get the minimum and maximum scroll-bar positions.
		CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
		int minpos;
		int maxpos;
		pSlider->GetRange(minpos, maxpos);

		// Get the current position of scroll box.
		int curpos = m_nTimingIndex;

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
		m_nTimingIndex = curpos;
		ShowSelectedTiming(m_nTimingIndex);

	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COutputConfigDlg::ShowSelectedTiming(int nTimingIndex)
{
	SetDlgItemText(IDC_LABEL_SELECTED_RESOLUTION, m_ResManager.GetResolutionName(nTimingIndex));

	TIMING st = {0};
	st = m_ResManager.GetResolutionDetail(nTimingIndex);

	CString str;
	str.Format(_T("%d"), st.m_Hz);

	m_CBoxHz.ResetContent();
	m_CBoxHz.AddString(str);
	m_CBoxHz.SelectString(0, str);

	GetDlgItem(IDC_BTN_DEL_TIMING)->EnableWindow(st.m_Type == CUSTOM_RESOLUTION);
}

BOOL COutputConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tips.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE
			||pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void COutputConfigDlg::OnBtnAll()
{
	CRedrawLock lock(&m_listOutput);

	for (int i=0; i<m_listOutput.GetItemCount(); i++)
	{
		m_listOutput.SetCheck(i, 1);
	}
}


void COutputConfigDlg::OnBtnNone()
{
	CRedrawLock lock(&m_listOutput);

	for (int i=0; i<m_listOutput.GetItemCount(); i++)
	{
		m_listOutput.SetCheck(i, 0);
	}
}


void COutputConfigDlg::OnBtnInvert()
{
	CRedrawLock lock(&m_listOutput);

	for (int i=0; i<m_listOutput.GetItemCount(); i++)
	{
		m_listOutput.SetCheck(i, !m_listOutput.GetCheck(i));
	}
}


void COutputConfigDlg::OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem < 0)
	{
		*pResult = 0;
		return;
	}

	TIMING st = {0};
	st = GetOutputManager()->GetTiming(pNMItemActivate->iItem, FALSE);

	int nResIndex = m_ResManager.GetResolutionIndex(st);
	if (nResIndex >= 0)
	{
		m_SliderTiming.SetPos(nResIndex);
		m_nTimingIndex = nResIndex;

		ShowSelectedTiming(nResIndex);	
	}

	*pResult = 0;
}

void COutputConfigDlg::OnLvnItemChangedListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = pNMLV->iItem;

	if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1) )
		&& ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2) ) ) //UnChecked to Checked
	{ 
		m_listOutput.SetRowColor(nItem, ListColorText, ListColorItemSelected);
		//m_listOutput.SetRowColor(nItem, ListColorItemSelected, ListColorBackground);
	}
	else if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2) )
		&&  ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1) ) ) //Checked to UnChecked
	{
		m_listOutput.SetRowColor(nItem, ListColorText, ListColorBackground);
		//m_listOutput.SetRowColor(nItem, ListColorText, ListColorBackground);
	}

	*pResult = 0;
}


void COutputConfigDlg::OnBnClickedBtnApply()
{
	UpdateData();	

	CWaitCursor wait;

	ResetListItemColor();

	ApplySetting();

	MessageBox(_T("更新完成！"), _T("应用"), MB_OK|MB_ICONINFORMATION);
}

void COutputConfigDlg::ApplySetting()
{
	int nSelectedCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_OUTPUT_PORT];	

	TIMING st = m_ResManager.GetResolutionDetail(m_nTimingIndex);
	CString strTiming = m_ResManager.GetResolutionName(st);

	//更新分辨率
	int nItemCount = m_listOutput.GetItemCount();
	for (int i=0; i<nItemCount; i++)
	{
		if (m_listOutput.GetCheck(i))
		{
			UpState[nSelectedCount].key = i;

			//发送命令
			nRet = GetCmdManager()->CmdWOUT(GetOutputManager()->GetCmdPort(i, FALSE), eBoardTypeOutput, 0, st);
			if (nRet == CMD_FAILURE)				
				UpState[nSelectedCount].val = 0;
			else
			{
				UpState[nSelectedCount].val = 1;
				GetOutputManager()->SetTiming(i, st, FALSE);
			}

			nSelectedCount++;

			Sleep(10);
		}
	}

	//显示更新结果
	CRedrawLock lock(&m_listOutput);

	for (int i=0; i<nSelectedCount; i++)	{
		if (UpState[i].val > 0)
		{
			m_listOutput.SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			//m_listOutput.SetRowColor(UpState[i].key, ListColorActionSucceed, ListColorBackground);
			m_listOutput.SetItemText(UpState[i].key, 2, strTiming);
			m_listOutput.SetItemText(UpState[i].key, 3, "成功！");
		}
		else {
			m_listOutput.SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			m_listOutput.SetItemText(UpState[i].key, 3, "失败！");
		}
		//m_listOutput.SetRowColor(UpState[i].m_Key, ListColorActionFailed, ListColorBackground);
	}

	// 保存最近设置的分辨率
	CSysConfManager sysconf;
	sysconf.SetLastTryResolution(strTiming, FALSE);
	GetDlgItem(IDC_LABEL_RECENT_RES_NAME)->SetWindowText(strTiming);
}


void COutputConfigDlg::OnBnClickedBtnAddTiming()
{
	CResolutionEditDlg DLG(m_ResManager.GetResolutionDetail(m_nTimingIndex), ADD_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		AddNewResolution(DLG.m_DetailTiming);
	}
}


void COutputConfigDlg::OnBnClickedBtnDelTiming()
{
	CResolutionEditDlg DLG(m_ResManager.GetResolutionDetail(m_nTimingIndex), DEL_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		DeleteResolution(DLG.m_DetailTiming);
	}
}


void COutputConfigDlg::OnBnClickedBtnEditTiming()
{
	CResolutionEditDlg DLG(m_ResManager.GetResolutionDetail(m_nTimingIndex), EDIT_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		ModifyResolution(DLG.m_DetailTiming);
	}
}


void COutputConfigDlg::OnBnClickedBtnGetSlaveTiming()
{
	int nItem = GetFirstSelectedItemIndex();
	if (nItem < 0)
	{
		AfxMessageBox(_T("请在左边输出端口列表中选中一个端口，不是在前面打钩！"));
		return;
	}

	TIMING st = {0};
	if (GetCmdManager()->CmdREDN(GetOutputManager()->GetCmdPort(nItem, FALSE), eBoardTypeOutput, 0, st) != CMD_RTOK)
	{
		AfxMessageBox(_T("读取次级分辨率失败！"));
		return;
	}

	if (!st.IsValid())
	{
		CString str;
		str.Format(_T("无效的分辨率：%d x %d @ %dHz"), st.m_HActive, st.m_HActive, st.m_Hz);
		AfxMessageBox(str);
		return;
	}

	int nPos = m_ResManager.IsExist(st);
	if (nPos < 0)
		nPos = m_ResManager.AddResolution(FALSE, const_cast<TIMING&>(st));

	if (nPos < 0)
		return;	

	m_SliderTiming.SetRange(0, m_ResManager.GetResolutionCount()-1, TRUE);

	m_nTimingIndex = nPos;
	m_SliderTiming.SetPos(nPos);	

	ShowSelectedTiming(m_nTimingIndex);
}


void COutputConfigDlg::AddNewResolution(const TIMING &timing)
{
	if (m_ResManager.AddResolution(FALSE, const_cast<TIMING&>(timing)) == INVALID_VALUE)
		return;

	//由于添加了新的分辨率，故而需要更新滑动条的位置
	int nResIndex = m_ResManager.GetResolutionIndex(timing);
	m_nTimingIndex = (nResIndex < 0) ? m_nTimingIndex : nResIndex;

	m_SliderTiming.SetRange(0, m_ResManager.GetResolutionCount()-1, TRUE);
	m_SliderTiming.SetPos(m_nTimingIndex);	

	ShowSelectedTiming(m_nTimingIndex);
}

void COutputConfigDlg::ModifyResolution(const TIMING &timing)
{
	m_ResManager.ModifyResolution(m_nTimingIndex, const_cast<TIMING&>(timing));
}

void COutputConfigDlg::DeleteResolution(const TIMING &timing)
{
	m_ResManager.RemoveResolution(timing);

	//由于删除了分辨率，故而需要更新滑动条的位置
	TIMING st = {0};
	int nItem = GetFirstSelectedItemIndex();
	if (nItem < 0)
	{
		nItem = 0;
		m_listOutput.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);	
	}

	st = GetOutputManager()->GetTiming(nItem, FALSE);

	int nResIndex = m_ResManager.GetResolutionIndex(st);
	m_nTimingIndex = (nResIndex < 0) ? 0 : nResIndex;

	m_SliderTiming.SetRange(0, m_ResManager.GetResolutionCount()-1, TRUE);	
	m_SliderTiming.SetPos(m_nTimingIndex);

	ShowSelectedTiming(m_nTimingIndex);	
}

void COutputConfigDlg::OnNMRClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;

	CString str;
	str.Format(_T("%s"), GetOutputManager()->GetName(nItem, FALSE));

	CRenameDlg DLG(str, this);
	if (DLG.DoModal() == IDOK)
	{
		GetOutputManager()->SetName(nItem, DLG.m_strOrgName.GetBuffer(), FALSE);
		//str.Format(_T("<%03d> "), nLogicIndex+1);
		str = DLG.m_strOrgName;
		m_listOutput.SetItemText(nItem, 1, str.GetBuffer());
	}

	*pResult = 0;
}


void COutputConfigDlg::ResetListItemColor()
{
	for (int i=0; i<m_listOutput.GetItemCount(); i++)
	{
		m_listOutput.SetItemText(i, 3, "");
		if (m_listOutput.GetCheck(i))
		{
			m_listOutput.SetRowColor(i, ListColorText, ListColorItemSelected);
		}
		else
		{
			m_listOutput.SetRowColor(i, ListColorText, ListColorBackground);
		}
	}
}

void COutputConfigDlg::InitChannelTiming()
{
	TIMING st = {0};
	for (int i=0; i<GetOutputManager()->GetCount(); i++)
	{
		if (GetOutputManager()->GetTiming(i, FALSE).m_Type == UNKNOW_RESOULUTION)
		{
			GetCmdManager()->CmdROUT(GetOutputManager()->GetCmdPort(i, FALSE), eBoardTypeOutput, 0, st);

			GetOutputManager()->SetTiming(i, st, FALSE);
		}
	}
}

int COutputConfigDlg::GetFirstSelectedItemIndex()
{
	POSITION pos = m_listOutput.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return INVALID_VALUE;

	int index = m_listOutput.GetNextSelectedItem(pos);

	return index;
}

void COutputConfigDlg::OnBtnReset()
{
	CWaitCursor wait;

	CRedrawLock lock(&m_listOutput);
	ResetListItemColor();

	int nSelectedCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	TIMING st = m_ResManager.GetResolutionDetail(m_nTimingIndex);
	CString strTiming = m_ResManager.GetResolutionName(st);

	//更新分辨率
	int nItemCount = m_listOutput.GetItemCount();
	int nLogicIndex = -1;
	for (int i=0; i<nItemCount; i++)
	{
		if (m_listOutput.GetCheck(i))
		{
			UpState[nSelectedCount].key = i;

			//发送命令
			nRet = GetCmdManager()->CmdRSTF(GetOutputManager()->GetCmdPort(i, FALSE), eBoardTypeInput);

			if (nRet == CMD_FAILURE)				
				UpState[nSelectedCount].val = 0;
			else
			{
				UpState[nSelectedCount].val = 1;
				GetOutputManager()->SetTiming(i, st, FALSE);
			}

			nSelectedCount++;

			Sleep(10);
		}
	}

	//显示更新结果

	for (int i=0; i<nSelectedCount; i++)	{
		if (UpState[i].val > 0)
		{
			m_listOutput.SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			m_listOutput.SetItemText(UpState[i].key, 2, strTiming);
			m_listOutput.SetItemText(UpState[i].key, 3, "成功！");
		}
		else
		{
			m_listOutput.SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			m_listOutput.SetItemText(UpState[i].key, 3, "失败！");
		}
	}

}

void COutputConfigDlg::OnBtnResetAll()
{
	CWaitCursor wait;

	CRedrawLock lock(&m_listOutput);
	ResetListItemColor();

	int nSelectedCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	TIMING st = m_ResManager.GetResolutionDetail(m_nTimingIndex);
	CString strTiming = m_ResManager.GetResolutionName(st);

	//更新分辨率
	int nItemCount = m_listOutput.GetItemCount();
	for (int i=0; i<nItemCount; i++)
	{
		UpState[nSelectedCount].key = i;

		//发送命令
		nRet = GetCmdManager()->CmdRSTF(GetOutputManager()->GetCmdPort(i, FALSE), eBoardTypeInput);

		if (nRet == CMD_FAILURE)				
			UpState[nSelectedCount].val = 0;
		else
		{
			UpState[nSelectedCount].val = 1;
			GetOutputManager()->SetTiming(i, st, FALSE);
		}

		nSelectedCount++;
		Sleep(10);
	}

	//显示更新结果

	for (int i=0; i<nSelectedCount; i++)	{
		if (UpState[i].val > 0)
		{
			m_listOutput.SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			m_listOutput.SetItemText(UpState[i].key, 2, strTiming);
			m_listOutput.SetItemText(UpState[i].key, 3, "成功！");
		}
		else
		{
			m_listOutput.SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			m_listOutput.SetItemText(UpState[i].key, 3, "失败！");
		}
	}
}