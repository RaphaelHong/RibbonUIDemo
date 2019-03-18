// MMListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "MMListCtrl.h"


// CMMListCtrl

IMPLEMENT_DYNAMIC(CMMListCtrl, CListCtrl)

CMMListCtrl::CMMListCtrl()
{

}

CMMListCtrl::~CMMListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMMListCtrl, CListCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMMListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()



// CMMListCtrl 消息处理程序



void CMMListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	return;
}

void CMMListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{

		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		BOOL bSelect = FALSE;
		POSITION pos = GetFirstSelectedItemPosition(); 
		while(pos) 
		{ 
			int index = GetNextSelectedItem(pos); 
			if(index==nItem) 
			{ 
				bSelect = TRUE; 
				break; 
			} 
		}     
		if(bSelect)
		{
			//clrNewTextColor = RGB(255,0,0);     //Set the text to red
			clrNewTextColor = RGB(255,255,255);     //Set the text to red
			clrNewBkColor = RGB(51,153,255);     //Set the bkgrnd color to blue
		}
		else
		{    
			clrNewTextColor = RGB(0,0,0);     //Leave the text black
			clrNewBkColor = RGB(255,255,255);    //leave the bkgrnd color white
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;


		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;

	}

	//*pResult = 0;
}
