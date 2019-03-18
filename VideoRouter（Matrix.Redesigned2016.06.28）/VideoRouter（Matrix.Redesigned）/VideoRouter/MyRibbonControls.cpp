#include "stdafx.h"
#include "MyRibbonControls.h"

static const int nXTabMargin = 14;  
static const int nYTabMargin = 5;  

static const int nXMargin = 2;  
static const int nYMargin = 2;  


//////////////////////////////////////////////////////////////////////////  
// CKofMFCRibbonBar  

IMPLEMENT_DYNAMIC(CMyRibbonBar, CMFCRibbonBar)  

CMyRibbonBar::CMyRibbonBar(BOOL bReplaceFrameCaption /*= TRUE*/)  
: CMFCRibbonBar(bReplaceFrameCaption)  
{  
}  

CMyRibbonBar::~CMyRibbonBar()  
{  
}  

BEGIN_MESSAGE_MAP(CMyRibbonBar, CMFCRibbonBar)  
END_MESSAGE_MAP()  

BOOL CMyRibbonBar::OnShowRibbonQATMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit)
{
	return CMFCRibbonBar::OnShowRibbonQATMenu(pWnd, x, y, pHit);
	return FALSE;
}

/*
CSize CMyRibbonBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )  
{  
	ASSERT_VALID(this);  

	CClientDC dc(this);  

	CFont* pOldFont = dc.SelectObject(GetFont());  
	ENSURE(pOldFont != NULL);  

	TEXTMETRIC tm;  
	dc.GetTextMetrics(&tm);  

	//“标题”的高度  
	m_nCaptionHeight = 0;  

	if (m_bReplaceFrameCaption)  
	{  
		m_nCaptionHeight = GetSystemMetrics(SM_CYCAPTION) + 1;  

		if (afxGlobalData.DwmIsCompositionEnabled())  
		{  
			m_nCaptionHeight += GetSystemMetrics(SM_CYSIZEFRAME);  
		}  
	}  

	int cy = 0;  

	CSize sizeMainButton = m_sizeMainButton;  
	double scale = afxGlobalData.GetRibbonImageScale();  
	if (scale > 1.)  
	{  
		sizeMainButton.cx = (int)(.5 + scale * sizeMainButton.cx);  
		sizeMainButton.cy = (int)(.5 + scale * sizeMainButton.cy);  
	}  

	if (m_dwHideFlags & AFX_RIBBONBAR_HIDE_ALL)  
	{  
		cy = m_nCaptionHeight;  
	}  
	else  
	{  
		if (m_bRecalcCategoryHeight)  
		{  
			m_nCategoryHeight = 0;  
		}  

		//“选项卡”的高度  
		m_nTabsHeight = tm.tmHeight + 2 * nYTabMargin;  

		if (m_bRecalcCategoryHeight)  
		{  
			for (int i = 0; i < m_arCategories.GetSize(); i++)  
			{  
				CMFCRibbonCategory* pCategory = m_arCategories [i];  
				ASSERT_VALID(pCategory);  

				m_nCategoryHeight = max(m_nCategoryHeight, pCategory->GetMaxHeight(&dc));  
			}  
			m_bRecalcCategoryHeight = FALSE;  
		}  

		CMyRibbonQuickAccessToolBar *pQAToolbar = (CMyRibbonQuickAccessToolBar *)&m_QAToolbar;  
		const CSize sizeAQToolbar = pQAToolbar->GetRegularSize(&dc);  

		if (IsQuickAccessToolbarOnTop())  
		{  
			m_nCaptionHeight = max(m_nCaptionHeight, sizeAQToolbar.cy + (2 * nYMargin));  
		}  

		//“快速访问工具栏”的高度  
		const int nQuickAceesToolbarHeight = IsQuickAccessToolbarOnTop() ? 0 : sizeAQToolbar.cy;  
		//“分类”的高度  
		const int nCategoryHeight = (m_dwHideFlags & AFX_RIBBONBAR_HIDE_ELEMENTS) ? 0 : m_nCategoryHeight;  

		cy = nQuickAceesToolbarHeight + nCategoryHeight + max( m_nCaptionHeight + m_nTabsHeight, m_sizeMainButton.cy + nYMargin);  
	}  

	if (afxGlobalData.DwmIsCompositionEnabled())  
	{  
		if (GetParent()->IsZoomed() && m_bReplaceFrameCaption)  
		{  
			cy += ::GetSystemMetrics(SM_CYSIZEFRAME) - 2;  
		}  
	}  

	dc.SelectObject(pOldFont);  
	return CSize(32767, cy);  
}  
*/


BOOL CMyRibbonBar::OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CMFCRibbonBaseElement* pHit)
{
	return CMFCRibbonBar::OnShowRibbonContextMenu(pWnd, x, y, pHit);
	/*

	DeactivateKeyboardFocus();

	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	if (m_bAutoCommandTimer)
	{
		KillTimer(1);
		m_bAutoCommandTimer = FALSE;
	}

	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 && (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0) // Both left and right mouse buttons are pressed
	{
		return FALSE;
	}

	if (afxContextMenuManager == NULL)
	{
		return FALSE;
	}

	//if (pHit != NULL)
	//{
	//	ASSERT_VALID(pHit);

	//	if (!pHit->IsHighlighted())
	//	{
	//		pHit->m_bIsHighlighted = TRUE;
	//		pHit->Redraw();
	//	}
	//}

	CMFCPopupMenu* pPopupMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, pWnd->GetParent());

	CFrameWnd* pParentFrame = GetParentFrame();
	ASSERT_VALID(pParentFrame);

	const UINT idCustomize = (UINT) -102;
	const UINT idQATOnBottom = (UINT) -103;
	const UINT idQATOnTop = (UINT) -104;
	const UINT idAddToQAT = (UINT) -105;
	const UINT idRemoveFromQAT = (UINT) -106;
	const UINT idMinimize = (UINT) -107;
	const UINT idRestore = (UINT) -108;

	CMenu menu;
	menu.CreatePopupMenu();

	{
		CString strItem;

		if (m_bIsCustomizeMenu)
		{
			ENSURE(strItem.LoadString(IDS_AFXBARRES_CUSTOMIZE_QAT_TOOLTIP));
			menu.AppendMenu(MF_STRING, (UINT) AFX_MENU_GROUP_ID, _T("Command Quick Access Toolbar"));

			//int uiMenuCmd = -((int)ID_APP_EXIT);
			//menu.AppendMenu(MF_STRING, uiMenuCmd, _T("退出"));

			//if (m_QAToolbar.GetButton(0)!= NULL)
			//	menu.CheckMenuItem(uiMenuCmd, MF_CHECKED);

			for (int i = 0; i < m_QAToolbar.GetCommandCount(); i++)
			{
				const UINT uiCmd = m_QAToolbar.GetCommandID(i);

				CMFCRibbonBaseElement* pElement = FindByID(uiCmd, FALSE);
				if (pElement != NULL)
				{
					ASSERT_VALID(pElement);

					strItem = pElement->GetText();

					if (strItem.IsEmpty())
					{
						pElement->UpdateTooltipInfo();
						strItem = pElement->GetToolTipText();
					}

					int uiMenuCmd = -((int) uiCmd);

					menu.AppendMenu(MF_STRING, uiMenuCmd, strItem);

					for (int iButton=0; iButton < m_QAToolbar.GetCount(); iButton++)
					{
						if (m_QAToolbar.GetButton(iButton)->GetID() == uiCmd)
						{
							menu.CheckMenuItem(uiMenuCmd, MF_CHECKED);
							break;
						}
					}
					//if (m_QAToolbar.FindByID(uiCmd) != NULL)
					//{
					//	menu.CheckMenuItem(uiMenuCmd, MF_CHECKED);
					//}
				}
			}
		}
		else if (pHit != NULL)
		{
			ASSERT_VALID(pHit);

			UINT nID = pHit->GetQuickAccessToolBarID();

			BOOL bQuickAccessMode = pHit->IsQATMode();
			if (bQuickAccessMode)
			{
				ENSURE(strItem.LoadString(IDS_AFXBARRES_REMOVE_FROM_QAT));
				menu.AppendMenu(MF_STRING, idRemoveFromQAT, _T("Remove from Quick Access Toolbar"));
			}
			else if (pHit->CanBeAddedToQuickAccessToolBar())
			{
				ENSURE(strItem.LoadString(IDS_AFXBARRES_ADD_TO_QAT));
				menu.AppendMenu(MF_STRING, idAddToQAT, _T("Add to Quick Access Toolbar"));

				for (int i=0; i<m_QAToolbar.GetCount(); i++)
				{
					if (m_QAToolbar.GetButton(i)->GetID() == nID)
					{
						// Already on QAT, disable this item
						menu.EnableMenuItem(idAddToQAT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
						break;
					}
				}
			}
		}

		if (menu.GetMenuItemCount() > 0)
		{
			menu.AppendMenu(MF_SEPARATOR);
		}

		//ENSURE(strItem.LoadString(m_bIsCustomizeMenu ? IDS_AFXBARRES_MORE_COMMANDS : IDS_AFXBARRES_CUSTOMIZE_QAT));

		//menu.AppendMenu(MF_STRING, idCustomize, strItem);

		if (IsQuickAccessToolbarOnTop())
		{
			//ENSURE(strItem.LoadString(m_bIsCustomizeMenu ? IDS_AFXBARRES_PLACE_BELOW_RIBBON : IDS_AFXBARRES_PLACE_QAT_BELOW_RIBBON));
			if (m_bIsCustomizeMenu)
				strItem = _T("Show Below the Ribbonbar");
			else
				strItem = _T("Show Quick Access Toolbar Below the Ribbonbar");
			menu.AppendMenu(MF_STRING, idQATOnBottom, strItem);
		}
		else
		{
			//ENSURE(strItem.LoadString(m_bIsCustomizeMenu ? IDS_AFXBARRES_PLACE_ABOVE_RIBBON : IDS_AFXBARRES_PLACE_QAT_ABOVE_RIBBON));
			if (m_bIsCustomizeMenu)
				strItem = _T("Show Above the Ribbonbar");
			else
				strItem = _T("Show Quick Access Toolbar Above the Ribbonbar");
			menu.AppendMenu(MF_STRING, idQATOnTop, strItem);
		}

		menu.AppendMenu(MF_SEPARATOR);

		if (m_dwHideFlags == AFX_RIBBONBAR_HIDE_ELEMENTS)
		{
			ENSURE(strItem.LoadString(IDS_AFXBARRES_MINIMIZE_RIBBON));
			menu.AppendMenu(MF_STRING, idRestore, _T("Minimize the Ribbonbar"));
			menu.CheckMenuItem(idRestore, MF_CHECKED);
		}
		else
		{
			ENSURE(strItem.LoadString(IDS_AFXBARRES_MINIMIZE_RIBBON));
			menu.AppendMenu(MF_STRING, idMinimize, _T("Minimize the Ribbonbar"));
		}
	}

	HWND hwndThis = pWnd->GetSafeHwnd();

	if (pPopupMenu != NULL)
	{
		afxContextMenuManager->SetDontCloseActiveMenu();
	}

	int nMenuResult = afxContextMenuManager->TrackPopupMenu(menu, x, y, pWnd);

	if (pPopupMenu != NULL)
	{
		afxContextMenuManager->SetDontCloseActiveMenu(FALSE);
	}

	if (!::IsWindow(hwndThis))
	{
		return FALSE;
	}

	//if (pHit != NULL)
	//{
	//	ASSERT_VALID(pHit);

	//	pHit->m_bIsHighlighted = FALSE;

	//	CMFCRibbonBaseElement* pDroppedDown = pHit->GetDroppedDown();

	//	if (pDroppedDown != NULL)
	//	{
	//		ASSERT_VALID(pDroppedDown);

	//		pDroppedDown->ClosePopupMenu();
	//		pHit->m_bIsDroppedDown = FALSE;
	//	}

	//	pHit->Redraw();
	//}

	BOOL bRecalcLayout = FALSE;

	switch (nMenuResult)
	{
	case idCustomize:
		{
			if (pHit == m_pHighlighted)
			{
				m_pHighlighted = NULL;
			}

			if (pHit == m_pPressed)
			{
				m_pPressed = NULL;
			}

			if (pPopupMenu != NULL)
			{
				pPopupMenu->SendMessage(WM_CLOSE);
			}

			if (pParentFrame->SendMessage(AFX_WM_ON_RIBBON_CUSTOMIZE, 0, (LPARAM)this) == 0)
			{
				CMFCRibbonCustomizeDialog* pDlg = new CMFCRibbonCustomizeDialog (pParentFrame, this);
				ENSURE(pDlg != NULL);

				pDlg->DoModal();
				delete pDlg;
			}

			return TRUE;
		}
		break;

	case idAddToQAT:
		if (pHit != NULL)
		{
			ASSERT_VALID(pHit);

			BOOL bIsDefaultMenuLook = pHit->IsDefaultMenuLook();
			pHit->IsDefaultMenuLook();
			if (bIsDefaultMenuLook)
			{
				CMFCRibbonBaseElement* pElem = FindByID(pHit->GetID(), FALSE);
				if (pElem != NULL)
				{
					ASSERT_VALID(pElem);
					pHit = pElem;
				}
			}

			bRecalcLayout = pHit->OnAddToQAToolbar(m_QAToolbar);

		}
		break;

	case idRemoveFromQAT:
		ASSERT_VALID(pHit);

		if (pHit == m_pHighlighted)
		{
			m_pHighlighted = NULL;
		}

		if (pHit == m_pPressed)
		{
			m_pPressed = NULL;
		}

		m_QAToolbar.RemoveEx(pHit);
		bRecalcLayout = TRUE;
		break;

	case idQATOnBottom:
		SetQuickAccessToolbarOnTop(FALSE);
		bRecalcLayout = TRUE;
		break;

	case idQATOnTop:
		SetQuickAccessToolbarOnTop(TRUE);
		bRecalcLayout = TRUE;
		break;

	case idMinimize:
		if (m_pActiveCategory != NULL)
		{
			ASSERT_VALID(m_pActiveCategory);
			//m_pActiveCategory->ShowElements(FALSE);
			m_dwHideFlags = AFX_RIBBONBAR_HIDE_ELEMENTS;

			int nPanels = m_pActiveCategory->GetPanelCount();
			for (int i=0; i<nPanels; i++)
			{
				CArray <CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arrElem;
				m_pActiveCategory->GetPanel(i)->GetElements(arrElem);
				for (int j=0; j<arrElem.GetCount(); j++)
				{
					arrElem[j]->OnShow(FALSE);
				}
			}
			GetParentFrame()->RecalcLayout();
			RedrawWindow();
		}
		break;

	case idRestore:
		if (m_pActiveCategory != NULL)
		{
			ASSERT_VALID(m_pActiveCategory);
			//this->m_pActiveCategory->ShowElements();
			m_dwHideFlags = 0;

			int nPanels = m_pActiveCategory->GetPanelCount();
			for (int i=0; i<nPanels; i++)
			{
				CArray <CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arrElem;
				m_pActiveCategory->GetPanel(i)->GetElements(arrElem);
				for (int j=0; j<arrElem.GetCount(); j++)
				{
					arrElem[j]->OnShow(TRUE);
				}
			}
			GetParentFrame()->RecalcLayout();
			RedrawWindow();
		}
		break;

	default:
		if (m_bIsCustomizeMenu)
		{
			UINT uiCmd = -nMenuResult;

			if (uiCmd != 0)
			{
				CMFCRibbonBaseElement* pElement = FindByID(uiCmd, FALSE);
				if (pElement != NULL)
				{
					ASSERT_VALID(pElement);

					BOOL bFindID = FALSE;
					for (int i=0; i<m_QAToolbar.GetCount(); i++)
					{
						if (m_QAToolbar.GetButton(i)->GetID() == uiCmd)
						{
							bFindID = TRUE;
							break;
						}
					}

					if (bFindID)
					{
						m_QAToolbar.RemoveEx(pElement);
					}
					else
					{
						m_QAToolbar.AddEx(pElement);
					}

					//if (m_QAToolbar.FindByID(uiCmd) != NULL)
					//{
					//	m_QAToolbar.RemoveEx(pElement);
					//}
					//else
					//{
					//	m_QAToolbar.AddEx(pElement);
					//}

					bRecalcLayout = TRUE;
					break;
				}
			}
		}

		if (pPopupMenu != NULL)
		{
			CMFCPopupMenu *pActiveMenu = NULL;
			pActiveMenu = CMFCPopupMenu::GetActiveMenu();
			pActiveMenu = pPopupMenu;
			//CMFCPopupMenu::m_pActivePopupMenu = pPopupMenu;
		}

		return FALSE;
	}

	if (pPopupMenu != NULL)
	{
		pPopupMenu->SendMessage(WM_CLOSE);
	}

	if (bRecalcLayout)
	{
		m_bForceRedraw = TRUE;
		RecalcLayout();

		if (pParentFrame->GetSafeHwnd() != NULL)
		{
			pParentFrame->RecalcLayout();
			pParentFrame->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
		}
	}

	return FALSE;

*/
}