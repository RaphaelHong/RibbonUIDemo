// InputConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "InputConfigDlg.h"
#include "ResolutionEditDlg.h"
#include "RenameDlg.h"


// CInputConfigDlg 对话框

IMPLEMENT_DYNAMIC(CInputConfigDlg, CDialog)

CInputConfigDlg::CInputConfigDlg(CWnd* pParent /*=NULL*/)
: CDialog(CInputConfigDlg::IDD, pParent)
, m_nTimingIndex(0)
{

}

CInputConfigDlg::~CInputConfigDlg()
{
}

void CInputConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PORTS, m_listInput);
	DDX_Control(pDX, IDC_CBOX_PORT_CFG_HZ, m_CBoxHz);
	DDX_Control(pDX, IDC_SLIDER_PORT_RESOLUTION, m_SliderTiming);
	DDX_Slider(pDX, IDC_SLIDER_PORT_RESOLUTION, m_nTimingIndex);
}


BEGIN_MESSAGE_MAP(CInputConfigDlg, CDialog)
	ON_WM_HSCROLL()
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, SetTipText)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_ALL, &CInputConfigDlg::OnBtnAll)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_NONE, &CInputConfigDlg::OnBtnNone)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_INVERT, &CInputConfigDlg::OnBtnInvert)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PORTS, &CInputConfigDlg::OnNMClickListOutput)
	ON_BN_CLICKED(IDC_BTN_PORT_CFG_APPLY, &CInputConfigDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_ADD_TIMING, &CInputConfigDlg::OnBnClickedBtnAddTiming)
	ON_BN_CLICKED(IDC_BTN_DEL_TIMING, &CInputConfigDlg::OnBnClickedBtnDelTiming)
	ON_BN_CLICKED(IDC_BTN_EDIT_TIMING, &CInputConfigDlg::OnBnClickedBtnEditTiming)
	ON_BN_CLICKED(IDC_BTN_GET_SLAVE_TIMING, &CInputConfigDlg::OnBnClickedBtnGetSlaveTiming)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PORTS, &CInputConfigDlg::OnNMRClickListOutput)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PORTS, &CInputConfigDlg::OnLvnItemChangedListOutput)
	ON_BN_CLICKED(IDC_BTN_PORT_RESET, &CInputConfigDlg::OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_PORT_RESET_ALL, &CInputConfigDlg::OnBtnResetAll)
	//ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CInputConfigDlg 消息处理程序


BOOL CInputConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	CString strTitle;
	GetWindowText(strTitle);
	strTitle += _T(" - 输入");
	SetWindowText(strTitle);

	CWaitCursor wait;

	// 第一次打开配置对话框时需要从底层读取各端口的分辨率
	InitChannelTiming();

	// 加载输入分辨率配置文件
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\InputResolution.ini");
	m_ResManager.SetConfigFilePath(strPath.GetBuffer(0));
	m_ResManager.LoadFromFile(NULL);

	// 初始化输入端口列表
	InitInputList();

	// 分辨率选择滑动条
	TIMING st = GetInputManager()->GetTiming(0, FALSE);
	int nPos = m_ResManager.GetResolutionIndex(st);

	m_SliderTiming.SetRange(0, m_ResManager.GetResolutionCount()-1);
	m_SliderTiming.SetTic(1);
	m_SliderTiming.SetTicFreq(1);
	m_SliderTiming.SetPos(nPos);

	// 提示条
	SetTooltips();

	// 输入设置不显示读取次级分辨率按钮
	GetDlgItem(IDC_BTN_GET_SLAVE_TIMING)->ShowWindow(SW_HIDE);

	// 获取最近设置的分辨率
	CSysConfManager sysconf;
	CString strLastRes = sysconf.GetLastTryResolution(TRUE);
	GetDlgItem(IDC_LABEL_RECENT_RES_NAME)->SetWindowText(strLastRes);

	// 显示默认分辨率到界面
	ShowSelectedTiming(nPos);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CInputConfigDlg::SetTooltips()
{
	EnableToolTips(TRUE);
	m_tips.Create(this);
	m_tips.AddTool(&m_SliderTiming, _T("拖动选择分辨率"));
	m_tips.SetTipBkColor(RGB(200, 200, 255));
	m_tips.SetTipTextColor(RGB(100, 150, 100));
}

void CInputConfigDlg::InitInputList()
{
	//设置列表控件的样式
	m_listInput.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);
	m_listInput.ModifyExtendedStyle(0, LVS_EX_CHECKBOXES);
	m_listInput.ModifyExtendedStyle(0, LVS_EX_GRIDLINES);
	m_listInput.EnableUserSortColor(true);
	m_listInput.EnableUserListColor(true);
	m_listInput.EnableUserRowColor(true);
	m_listInput.SetListBackColor(ListColorBackground);

	//插入列
	int nColumn = 0;
	m_listInput.InsertColumn(nColumn++, _T("序号"), LVCFMT_RIGHT, 50);
	m_listInput.InsertColumn(nColumn++, _T("名称"), LVCFMT_LEFT, 150);
	m_listInput.InsertColumn(nColumn++, _T("分辨率"), LVCFMT_RIGHT, 150);
	m_listInput.InsertColumn(nColumn++, _T("信号状态"), LVCFMT_RIGHT, 120);
	m_listInput.InsertColumn(nColumn++, _T("应用状态"), LVCFMT_CENTER, 120);

	//插入项
	CInputManager *pInput = GetInputManager();
	int nInputNum = pInput->GetCount();
	int nItem = -1;

	CString strItem;
	CStringW wstrSignal;
	char szSignal[_MAX_SIGNAL_NAME];

	TIMING st = {0};

	for (int i=0; i<nInputNum; i++)
	{
		if (pInput->GetSubID(i, FALSE) > 1)
			continue;
		else
			nItem++;

		// 列表中显示的都是真实物理信号的序号
		strItem.Format("%d", nItem+1);
		m_listInput.InsertItem(LVIF_TEXT, nItem, strItem, 0, LVIS_SELECTED, 0, 0);

		// 名称
		strItem.Format("%s", pInput->GetName(i, FALSE));
		m_listInput.SetItemText(nItem, 1, strItem);

		// 分辨率
		st = pInput->GetTiming(i, FALSE);

		// 添加新品种分辨率到配置文件
		if ((m_ResManager.IsExist(st) == INVALID_VALUE) && st.IsValid())
		{
			st.m_Type = CUSTOM_RESOLUTION;
			m_ResManager.AddResolution(FALSE, st);
		}

		m_listInput.SetItemText(nItem, 2, m_ResManager.GetResolutionName(st));

		// 信号状态
		wstrSignal = GetVideoName(pInput->GetSignalType(i, FALSE));
		WideCharToMultiByte(CP_ACP, 0, wstrSignal.GetBuffer(), -1, szSignal, _MAX_SIGNAL_NAME, "", FALSE);
		m_listInput.SetItemText(nItem, 3, szSignal);
	}

	if (nInputNum > 0)
		m_listInput.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

BOOL CInputConfigDlg::SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pRes)  
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

void CInputConfigDlg::ShowSelectedTiming(int nTimingIndex)
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

void CInputConfigDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

BOOL CInputConfigDlg::PreTranslateMessage(MSG* pMsg)
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


void CInputConfigDlg::OnBtnAll()
{
	CRedrawLock lock(&m_listInput);

	for (int i=0; i<m_listInput.GetItemCount(); i++)
	{
		m_listInput.SetCheck(i, 1);
	}
}


void CInputConfigDlg::OnBtnNone()
{
	CRedrawLock lock(&m_listInput);	

	for (int i=0; i<m_listInput.GetItemCount(); i++)
	{
		m_listInput.SetCheck(i, 0);
	}
}


void CInputConfigDlg::OnBtnInvert()
{
	CRedrawLock lock(&m_listInput);

	for (int i=0; i<m_listInput.GetItemCount(); i++)
	{
		m_listInput.SetCheck(i, !m_listInput.GetCheck(i));
	}
}


void CInputConfigDlg::OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem < 0)
	{
		*pResult = 0;
		return;
	}

	TIMING st = {0};
	st = GetInputManager()->GetTiming(pNMItemActivate->iItem, FALSE);

	int nResIndex = m_ResManager.GetResolutionIndex(st);
	if (nResIndex >= 0)
	{
		m_SliderTiming.SetPos(nResIndex);
		m_nTimingIndex = nResIndex;

		ShowSelectedTiming(nResIndex);	
	}

	*pResult = 0;
}


void CInputConfigDlg::OnBnClickedBtnApply()
{
	UpdateData();

	CWaitCursor wait;

	ResetListItemColor();	

	ApplySetting();

	MessageBox(_T("更新完成！"), _T("应用"), MB_OK|MB_ICONINFORMATION);
}

void CInputConfigDlg::ApplySetting()
{
	int nSelectedCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	TIMING st = m_ResManager.GetResolutionDetail(m_nTimingIndex);
	CString strTiming = m_ResManager.GetResolutionName(st);

	//更新分辨率
	int nItemCount = m_listInput.GetItemCount();
	int nLogicIndex = -1;
	for (int i=0; i<nItemCount; i++)
	{
		if (m_listInput.GetCheck(i))
		{
			UpState[nSelectedCount].key = i;

			//发送命令
			nLogicIndex = GetInputManager()->RealIndex2LogicIndex(i);
			nRet = GetCmdManager()->CmdWINT(GetInputManager()->GetCmdPort(nLogicIndex, FALSE), eCardTypeInput, 0, st);

			if (nRet == CMD_FAILURE)				
				UpState[nSelectedCount].val = 0;
			else
			{
				UpState[nSelectedCount].val = 1;
				GetInputManager()->SetTiming(nLogicIndex, st, FALSE);
			}

			nSelectedCount++;

			Sleep(10);
		}
	}

	//显示更新结果
	CRedrawLock lock(&m_listInput);
	for (int i=0; i<nSelectedCount; i++)	{
		if (UpState[i].val > 0)
		{
			m_listInput.SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			m_listInput.SetItemText(UpState[i].key, 2, strTiming);
			m_listInput.SetItemText(UpState[i].key, 4, "成功！");
		}
		else
		{
			m_listInput.SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			m_listInput.SetItemText(UpState[i].key, 4, "失败！");
		}
	}

	CSysConfManager sysconf;
	sysconf.SetLastTryResolution(strTiming, TRUE);
	GetDlgItem(IDC_LABEL_RECENT_RES_NAME)->SetWindowText(strTiming);
}


void CInputConfigDlg::OnBnClickedBtnAddTiming()
{
	CResolutionEditDlg DLG(m_ResManager.GetResolutionDetail(m_nTimingIndex), ADD_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		AddNewResolution(DLG.m_DetailTiming);
	}
}


void CInputConfigDlg::OnBnClickedBtnDelTiming()
{
	CResolutionEditDlg DLG(m_ResManager.GetResolutionDetail(m_nTimingIndex), DEL_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		DeleteResolution(DLG.m_DetailTiming);
	}
}


void CInputConfigDlg::OnBnClickedBtnEditTiming()
{
	CResolutionEditDlg DLG(m_ResManager.GetResolutionDetail(m_nTimingIndex), EDIT_RESOLUTION, this);
	if (DLG.DoModal() == IDOK)
	{
		ModifyResolution(DLG.m_DetailTiming);
	}
}


void CInputConfigDlg::OnBnClickedBtnGetSlaveTiming()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CInputConfigDlg::AddNewResolution(const TIMING &timing)
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

void CInputConfigDlg::ModifyResolution(const TIMING &timing)
{
	m_ResManager.ModifyResolution(m_nTimingIndex, const_cast<TIMING&>(timing));
}

void CInputConfigDlg::DeleteResolution(const TIMING &timing)
{
	m_ResManager.RemoveResolution(timing);

	//由于删除了分辨率，故而需要更新滑动条的位置
	TIMING st = {0};
	int nItem = GetFirstSelectedItemIndex();
	if (nItem < 0)
	{
		nItem = 0;
		m_listInput.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}

	st = GetInputManager()->GetTiming(GetInputManager()->RealIndex2LogicIndex(nItem), FALSE);

	int nResIndex = m_ResManager.GetResolutionIndex(st);
	m_nTimingIndex = (nResIndex < 0) ? 0 : nResIndex;

	m_SliderTiming.SetRange(0, m_ResManager.GetResolutionCount()-1, TRUE);	
	m_SliderTiming.SetPos(m_nTimingIndex);

	ShowSelectedTiming(m_nTimingIndex);	
}

void CInputConfigDlg::OnNMRClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//int nClicked = pNMItemActivate->iItem;

	//CString str;
	//str.Format(_T("%s"), GetInputManager()->GetName(nClicked));

	//CRenameDlg DLG(str, this);
	//if (DLG.DoModal() == IDOK)
	//{
	//	GetInputManager()->SetName(nClicked, DLG.m_strOrgName.GetBuffer());

	//	str.Format(_T("<%02d> "), nClicked+1);
	//	str += DLG.m_strOrgName;

	//	m_listInput.SetItemText(nClicked, 0, str.GetBuffer());
	//}

	*pResult = 0;
}

void CInputConfigDlg::OnLvnItemChangedListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = pNMLV->iItem;

	if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1) )
		&& ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2) ) ) //UnChecked to Checked
	{ 
		m_listInput.SetRowColor(nItem, ListColorText, ListColorItemSelected);
	}
	else if ( ( pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2) )
		&&  ( pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1) ) ) //Checked to UnChecked
	{
		m_listInput.SetRowColor(nItem, ListColorText, ListColorBackground);
	}


	*pResult = 0;
}

void CInputConfigDlg::ResetListItemColor()
{
	for (int i=0; i<m_listInput.GetItemCount(); i++)
	{
		m_listInput.SetItemText(i, 4, "");
		if (m_listInput.GetCheck(i))
		{
			m_listInput.SetRowColor(i, ListColorText, ListColorItemSelected);
		}
		else
		{
			m_listInput.SetRowColor(i, ListColorText, ListColorBackground);
		}
	}
}

HBRUSH CInputConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	return hbr;
}

void CInputConfigDlg::InitChannelTiming()
{	
	TIMING st = {0};

	for (int i=0; i<GetInputManager()->GetCount(); i++)
	{
		if (GetInputManager()->GetTiming(i, FALSE).m_Type == UNKNOW_RESOULUTION)
		{
			GetCmdManager()->CmdRINT(GetInputManager()->GetCmdPort(i,FALSE), eCardTypeInput, 0, st);

			GetInputManager()->SetTiming(i, st, FALSE);
		}
	}
}

int CInputConfigDlg::GetFirstSelectedItemIndex()
{
	POSITION pos = m_listInput.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return INVALID_VALUE;

	int index = m_listInput.GetNextSelectedItem(pos);

	return index;
}

void CInputConfigDlg::OnBtnReset()
{
	CWaitCursor wait;

	CRedrawLock lock(&m_listInput);
	ResetListItemColor();

	int nSelectedCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	TIMING st = m_ResManager.GetResolutionDetail(m_nTimingIndex);
	CString strTiming = m_ResManager.GetResolutionName(st);

	//更新分辨率
	int nItemCount = m_listInput.GetItemCount();
	int nLogicIndex = -1;
	for (int i=0; i<nItemCount; i++)
	{
		if (m_listInput.GetCheck(i))
		{
			UpState[nSelectedCount].key = i;

			//发送命令
			nLogicIndex = GetInputManager()->RealIndex2LogicIndex(i);
			nRet = GetCmdManager()->CmdRSTF(GetInputManager()->GetCmdPort(nLogicIndex, FALSE), eCardTypeInput);

			if (nRet == CMD_FAILURE)				
				UpState[nSelectedCount].val = 0;
			else
			{
				UpState[nSelectedCount].val = 1;
				GetInputManager()->SetTiming(nLogicIndex, st, FALSE);
			}

			nSelectedCount++;

			Sleep(10);
		}
	}

	//显示更新结果

	for (int i=0; i<nSelectedCount; i++)	{
		if (UpState[i].val > 0)
		{
			m_listInput.SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			m_listInput.SetItemText(UpState[i].key, 2, strTiming);
			m_listInput.SetItemText(UpState[i].key, 4, "成功！");
		}
		else
		{
			m_listInput.SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			m_listInput.SetItemText(UpState[i].key, 4, "失败！");
		}
	}

}

void CInputConfigDlg::OnBtnResetAll()
{
	CWaitCursor wait;

	CRedrawLock lock(&m_listInput);
	ResetListItemColor();

	int nSelectedCount = 0;
	int nRet = INVALID_VALUE;
	InOutKey UpState[_MAX_INPUT_PORT];

	TIMING st = m_ResManager.GetResolutionDetail(m_nTimingIndex);
	CString strTiming = m_ResManager.GetResolutionName(st);

	//更新分辨率
	int nItemCount = m_listInput.GetItemCount();
	int nLogicIndex = -1;
	for (int i=0; i<nItemCount; i++)
	{
		UpState[nSelectedCount].key = i;

		//发送命令
		nLogicIndex = GetInputManager()->RealIndex2LogicIndex(i);
		nRet = GetCmdManager()->CmdRSTF(GetInputManager()->GetCmdPort(nLogicIndex, FALSE), eCardTypeInput);

		if (nRet == CMD_FAILURE)				
			UpState[nSelectedCount].val = 0;
		else
		{
			UpState[nSelectedCount].val = 1;
			GetInputManager()->SetTiming(nLogicIndex, st, FALSE);
		}

		nSelectedCount++;
		Sleep(10);
	}

	//显示更新结果

	for (int i=0; i<nSelectedCount; i++)	{
		if (UpState[i].val > 0)
		{
			m_listInput.SetRowColor(UpState[i].key, ListColorText, ListColorActionSucceed);
			m_listInput.SetItemText(UpState[i].key, 2, strTiming);
			m_listInput.SetItemText(UpState[i].key, 4, "成功！");
		}
		else
		{
			m_listInput.SetRowColor(UpState[i].key, ListColorText, ListColorActionFailed);
			m_listInput.SetItemText(UpState[i].key, 4, "失败！");
		}
	}
}