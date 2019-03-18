// MMTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "MMTreeCtrl.h"
#include "..\InputManager.h"
#include "..\OutputManager.h"
#include "..\MainFrm.h"
#include "..\GridView.h"
#include "RenameDlg.h"
#include "SourcePropertyDlg.h"

#define DRAG_DELAY 60
// CMMTreeCtrl

IMPLEMENT_DYNAMIC(CMMTreeCtrl, CTreeCtrl)

CMMTreeCtrl::CMMTreeCtrl()
{
	m_nType = 0; //信号列表 =1 输出列表

	m_RClkItem = NULL;
	m_hItemDragS = NULL;
	m_hItemDragD = NULL;

	m_pImageList = NULL;

	m_bDragging = FALSE;
	m_dwDragStart = 0;
}

CMMTreeCtrl::~CMMTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMMTreeCtrl, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMMTreeCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMMTreeCtrl::OnNMDblclk)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CMMTreeCtrl::OnTvnBegindrag)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CMMTreeCtrl 消息处理程序
void CMMTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	//CPoint pt;
	//GetCursorPos(&pt);
	//ScreenToClient(&pt);

	//UINT nFlags = 0;
	//m_RClkItem = HitTest(pt, &nFlags);
	//if (m_RClkItem == NULL)
	//	return;

	//SelectItem(m_RClkItem);

	//int nIndex = GetSelectedItemIndex(m_RClkItem);

	//if (0 == m_nType)
	//	CInputManager::m_sCurPort = GetInputManager()->GetCmdPort(nIndex, FALSE);
	//else if (1 == m_nType)
	//	COutputManager::m_sCurPort = GetOutputManager()->GetCmdPort(nIndex, FALSE);

	//if (0 == m_nType)
	//	PopupMenuForInput();
	//else if (1 == m_nType)
	//	PopupMenuForOutput();
}

void CMMTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	UINT flags;
	HTREEITEM selItem = HitTest(point, &flags);

	if((flags & TVHT_ONITEMRIGHT) || (flags & TVHT_ONITEMINDENT) ||
		(flags & TVHT_ONITEM))
	{
		SelectItem(selItem);
	}    
	
	int nIndex = GetSelectedItemIndex(selItem);

	if (0 == m_nType)
		CInputManager::m_sCurPort = GetInputManager()->GetCmdPort(nIndex, FALSE);
	else if (1 == m_nType)
		COutputManager::m_sCurPort = GetOutputManager()->GetCmdPort(nIndex, FALSE);

	m_dwDragStart = GetTickCount();

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CMMTreeCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{	
	HTREEITEM selItem = GetSelectedItem();

	int nIndex = GetSelectedItemIndex(selItem);

	CView* pView = ((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	if (!pView->IsKindOf(RUNTIME_CLASS(CGridView)))
	{
		*pResult = 0;
		return;
	}

	if (0 == m_nType) {
		CInputManager::m_sCurPort = GetInputManager()->GetCmdPort(nIndex, FALSE);
		::SendMessage(pView->m_hWnd, WM_USER_SOURCECHANGED, (WPARAM)&CInputManager::m_sCurPort, 0);
	}
	else if (1 == m_nType) {
		COutputManager::m_sCurPort = GetOutputManager()->GetCmdPort(nIndex, FALSE);

		SCREENMAP scrmap;
		scrmap.iScreenID = -1;
		scrmap.iOutputCh = COutputManager::m_sCurPort;

		::SendMessage(pView->m_hWnd, WM_USER_OUTPORTCHANGED, (WPARAM)&scrmap, 0);
	}

	*pResult = 0;
}

inline int CMMTreeCtrl::GetSelectedItemIndex(HTREEITEM hti) const
{	
	HTREEITEM htiChild = GetRootItem();
	int index = 0;
	BOOL bFound = FALSE;
	while(htiChild != NULL && !bFound)
	{
		if (htiChild == hti) { 
			bFound = TRUE;
			break; 
		}

		htiChild = GetNextItem(htiChild,TVGN_NEXT);

		index++;
	}

	if (!bFound)
		index = -1;


	return index;
}

void CMMTreeCtrl::PopupMenuForInput()
{
	//弹出菜单
	if (afxContextMenuManager == NULL)
		return;

	CWnd* pWnd = GetParent();
	if (pWnd == NULL)
		return;

	CMFCPopupMenu* pPopupMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, pWnd);
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, ID_LOAD_EDID, _T("加载EDID"));
	menu.AppendMenu(MF_STRING, ID_VGA_PROPERTY, _T("VGA属性"));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_RECHECK_SIGNAL, _T("刷新信号源"));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_RENAME, _T("重命名"));

	afxContextMenuManager->SetDontCloseActiveMenu(TRUE);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	int nMenuResult = afxContextMenuManager->TrackPopupMenu(menu, ptCursor.x, ptCursor.y, pWnd);

	afxContextMenuManager->SetDontCloseActiveMenu(FALSE);

	int nIndex = GetInputManager()->FindCmdPort(CInputManager::m_sCurPort);

	switch (nMenuResult)
	{
	case ID_LOAD_EDID:
		{
			GetInputManager()->UpdateEDID(nIndex, FALSE);
		}
		break;
	case ID_VGA_PROPERTY:
		{
			CSourcePropertyDlg DLG;
			DLG.m_nSourcePort = CInputManager::m_sCurPort;
			DLG.m_eSourceType = GetInputManager()->GetSignalType(nIndex, FALSE);
			DLG.DoModal();
		}
		break;
	case ID_RENAME:
		{
			GetInputManager()->Rename(nIndex, ptCursor);
			CString str;
			str.Format(_T("%s(P%d)"), GetInputManager()->GetName(nIndex, FALSE), CInputManager::m_sCurPort);
			SetItemText(m_RClkItem, str.GetBuffer());
		}
		break;
	case ID_RECHECK_SIGNAL:
		{
			GetInitManager()->SetTransactionType(eTransaction_RefreshSignal);
			GetInitManager()->BeginTransaction(TRUE);
		}
		break;
	default:break;
	}

	if (pPopupMenu != NULL)
	{
		pPopupMenu->SendMessage(WM_CLOSE);
	}
}

void CMMTreeCtrl::PopupMenuForOutput()
{
	//弹出菜单
	if (afxContextMenuManager == NULL)
		return;

	CWnd* pWnd = GetParent();
	if (pWnd == NULL)
		return;

	CMFCPopupMenu* pPopupMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, pWnd);
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, ID_RENAME, _T("重命名"));

	afxContextMenuManager->SetDontCloseActiveMenu(TRUE);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	int nMenuResult = afxContextMenuManager->TrackPopupMenu(menu, ptCursor.x, ptCursor.y, pWnd);

	afxContextMenuManager->SetDontCloseActiveMenu(FALSE);

	int nIndex = GetOutputManager()->FindCmdPort(COutputManager::m_sCurPort);

	switch(nMenuResult)
	{
	case ID_RENAME:
		{
			GetOutputManager()->Rename(nIndex, ptCursor);

			CString str;
			str.Format(_T("%s(P%d)"), GetOutputManager()->GetName(nIndex, FALSE), COutputManager::m_sCurPort);
			SetItemText(m_RClkItem, str.GetBuffer());
		}		
		break;

	default:break;
	}

	if (pPopupMenu != NULL)
	{
		pPopupMenu->SendMessage(WM_CLOSE);
	}
}

void CMMTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_RClkItem = HitTest(point, &nFlags);
	if (m_RClkItem == NULL)
		return CTreeCtrl::OnRButtonDown(nFlags, point);

	SelectItem(m_RClkItem);

	int nIndex = GetSelectedItemIndex(m_RClkItem);

	if (0 == m_nType)
		CInputManager::m_sCurPort = GetInputManager()->GetCmdPort(nIndex, FALSE);
	else if (1 == m_nType)
		COutputManager::m_sCurPort = GetOutputManager()->GetCmdPort(nIndex, FALSE);

	if (0 == m_nType)
		PopupMenuForInput();
	else if (1 == m_nType)
		PopupMenuForOutput();

	//SendMessage(WM_CONTEXTMENU);

	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CMMTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	if (GetTickCount() - m_dwDragStart < DRAG_DELAY)
		return;

	m_hItemDragS = pNMTreeView->itemNew.hItem;
	//m_hItemDragD = NULL;
	
	m_pImageList = CreateDragImage(m_hItemDragS);
	if (!m_pImageList)
		return;

	m_bDragging = TRUE;

	m_pImageList->BeginDrag(0, CPoint(8, 8));

	CPoint pt = pNMTreeView->ptDrag;

	ClientToScreen( &pt );
	m_pImageList->DragEnter(NULL, pt); //"this"将拖动操作限制在该窗口
	SetCapture();

	//m_nScrollTimerID = SetTimer(2, 40, NULL);
}

void CMMTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	//HTREEITEM hItem;
	//UINT flags;

	//if (m_nHoverTimerID)
	//{
	//	KillTimer(m_nHoverTimerID);
	//	m_nHoverTimerID = 0;
	//}

	//m_nHoverTimerID = SetTimer(1, 800, NULL);
	//m_HoverPoint = point;

	if (m_bDragging)
	{
		CPoint pt = point;
		GetCursorPos(&pt);
		CImageList::DragMove(pt);

		//CImageList::DragShowNolock(false);
		//if ((hItem = HitTest(point, &flags)) != NULL)
		//{
		//	SelectDropTarget(hItem);
		//	m_hItemDragD = hItem;
		//}

		//CImageList::DragShowNolock(true);


		//CRect rect;
		//GetClientRect(&rect);
		//if (point.x < rect.left + 20)
		//{
		//	m_hItemDragD = NULL;
		//}
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CMMTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonUp(nFlags, point);

	if (m_bDragging)
	{
		m_bDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pImageList;

		int nIndex = GetSelectedItemIndex(m_hItemDragS);

		if (0 == m_nType)
		{
			CInputManager::m_sCurPort = GetInputManager()->GetCmdPort(nIndex, FALSE);
			::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_ON_SOURCE_DROP, WPARAM(&CInputManager::m_sCurPort), 0);
		}
		else if (1 == m_nType)
		{
			COutputManager::m_sCurPort = GetOutputManager()->GetCmdPort(nIndex, FALSE);
			::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_ON_OUTPUT_DROP, WPARAM(&COutputManager::m_sCurPort), 0);
		}


		//SelectDropTarget(NULL);

		//if (m_hItemDragS == m_hItemDragD)
		//{
		//	KillTimer(m_nScrollTimerID);
		//	return;
		//}
	}
}

//拷贝条目
HTREEITEM CMMTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
	CString sText;

	//得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength ();
	tvstruct.item.pszText = sText.LockBuffer ();

	//将条目插入到合适的位置
	tvstruct.hParent = htiNewParent;
	tvstruct.hInsertAfter = htiAfter;
	tvstruct.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem( &tvstruct );
	sText.ReleaseBuffer ();

	//限制拷贝条目数据和条目状态
	SetItemData( hNewItem,GetItemData(hItem) );
	SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}

//拷贝分支
HTREEITEM CMMTreeCtrl::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	HTREEITEM hChild;
	HTREEITEM hNewItem = CopyItem( htiBranch,htiNewParent,htiAfter );
	hChild = GetChildItem( htiBranch );

	while( hChild != NULL )
	{
		CopyBranch( hChild,hNewItem,htiAfter );
		hChild = GetNextSiblingItem( hChild );
	}

	return hNewItem;
}


void CMMTreeCtrl::OnTimer(UINT nIDEvent) 
{
	//鼠标敏感节点
	if( nIDEvent == m_nHoverTimerID )
	 {
		 KillTimer( m_nHoverTimerID );
		 m_nHoverTimerID = 0;
		 HTREEITEM trItem = 0;
		 UINT uFlag = 0;
		 trItem = HitTest( m_HoverPoint,&uFlag );
		 if( trItem && m_bDragging )
		 {
			SelectItem( trItem );
			Expand( trItem,TVE_EXPAND );
		}
	 }
	 //处理拖曳过程中的滚动问题
	 else if( nIDEvent == m_nScrollTimerID )
	  {
		  m_TimerTicks++;
		  CPoint pt;
		  GetCursorPos( &pt );
		  CRect rect;
		  GetClientRect( &rect );
		  ClientToScreen( &rect );
		  HTREEITEM hItem = GetFirstVisibleItem();

		  if( pt.y < rect.top +10 )
		  {
			  //向上滚动
			  int slowscroll = 6 - (rect.top + 10 - pt.y )/20;
			  if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
			  {
				  CImageList::DragShowNolock ( false );
				  SendMessage( WM_VSCROLL,SB_LINEUP );
				  SelectDropTarget( hItem );
				  m_hItemDragD = hItem;
				  CImageList::DragShowNolock ( true );
			  }
		  }
		  else if( pt.y > rect.bottom - 10 )
		  {
			  //向下滚动
			  int slowscroll = 6 - (pt.y - rect.bottom + 10)/20;
			  if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
			  {
				  CImageList::DragShowNolock ( false );
				  SendMessage( WM_VSCROLL,SB_LINEDOWN );
				  int nCount = GetVisibleCount();
				  for( int i=0 ; i<nCount-1 ; i++ )
					  hItem = GetNextVisibleItem( hItem );
				  if( hItem )
					  SelectDropTarget( hItem );
				  m_hItemDragD = hItem;
				  CImageList::DragShowNolock ( true );
			  }
		  }
	}
	 else
		 CTreeCtrl::OnTimer(nIDEvent);
}

void CMMTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	//第一种自绘
	LPNMHDR pNmhdr = (LPNMHDR)pNMHDR;         
	// To make sure that the notification is coming
	// from the right control (GGH)
	// CLeftView *pView = GetLeftPane();

	if(this->m_hWnd != pNmhdr->hwndFrom)
		return;

	LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	switch (pCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW, 
		// otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH) 
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;

	case CDDS_ITEMPREPAINT:
		if(this->IsWindowEnabled()==1)
		{
			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0
				&&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
			{ 
				pCustomDraw->clrTextBk=RGB(251,153,55);
				pCustomDraw->clrText = RGB(0, 0, 0);
			}
			*pResult = CDRF_NOTIFYPOSTPAINT;
			return;
		}
		else{
			*pResult = CDRF_DODEFAULT ;
			return;
		}
	case CDDS_ITEMPOSTPAINT:
		if(this->IsWindowEnabled()==1)
		{
			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0
				&&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
			{
				CRect   rcText;  
				HTREEITEM hItem=(HTREEITEM) pCustomDraw->nmcd.dwItemSpec;
				this->GetItemRect(hItem,   &rcText,   true); 
				CPen penBlue(PS_SOLID, 2, RGB(51,153,255));
				CDC* pDC=CDC::FromHandle(pCustomDraw->nmcd.hdc); 
				CBrush* pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush* pOldBrush = pDC->SelectObject(pBrush);
				CPen* pOldPen = pDC->SelectObject(&penBlue);
				rcText.InflateRect(-1, -1);
				pDC->Rectangle(&rcText);

				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
			}
			*pResult = CDRF_SKIPDEFAULT;
			return;
		}
		else{
			*pResult = CDRF_DODEFAULT ;
			return;
		}
	}
}