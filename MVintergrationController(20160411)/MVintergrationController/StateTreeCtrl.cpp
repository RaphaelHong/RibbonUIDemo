// StateTreeCtrl.cpp : implementation file
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2011 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MVintergrationController.h"
#include "SelectTreeCtrl.h"
#include "StateTreeCtrl.h"
#include "MVintergrationControllerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CStateTreeCtrl

CStateTreeCtrl::CStateTreeCtrl()
{
	m_hCursor = theApp.LoadCursor(IDC_CHECK);
	m_hCursorNone = theApp.LoadCursor(IDC_CHECK_NONE);
}

CStateTreeCtrl::~CStateTreeCtrl()
{
	if (m_hCursor != NULL) {
		::DestroyCursor(m_hCursor);
	}
	if (m_hCursorNone != NULL) {
		::DestroyCursor(m_hCursorNone);
	}

	if( m_bitmap.m_hObject != NULL ) // m_bitmap ΪCBitmap �ͱ��������ڴ�ű���λͼ
		m_bitmap.DeleteObject(); //���m_bitmap ��ԭ������

}

BEGIN_MESSAGE_MAP(CStateTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CStateTreeCtrl)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, &CStateTreeCtrl::OnTvnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CStateTreeCtrl::OnTvnItemexpanding)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStateTreeCtrl message handlers

void CStateTreeCtrl::PreSubclassWindow() 
{
	CTreeCtrl::PreSubclassWindow();

	m_ilStateImages.Create(IDB_STATE_IMAGES, 16, 1, RGB(255,255,255));
	SetImageList(&m_ilStateImages, TVSIL_STATE);
}

HTREEITEM CStateTreeCtrl::InsertItemState(LPCTSTR lpszItem, int nImage, int nSelectedImage, int nState/*=STATE_DISABLED*/, HTREEITEM hParent/*=TVI_ROOT*/, HTREEITEM hInsertAfter/*=TVI_LAST*/)
{
	TV_INSERTSTRUCT tvis;
	tvis.hParent = hParent;
	tvis.hInsertAfter = hInsertAfter;
	tvis.item.mask = TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvis.item.stateMask = TVIS_STATEIMAGEMASK;
	tvis.item.state = INDEXTOSTATEIMAGEMASK(1);
	tvis.item.pszText = (TCHAR*)lpszItem;
	tvis.item.cchTextMax = (int)_tcslen(lpszItem);
	tvis.item.iImage = nImage;
	tvis.item.iSelectedImage = nSelectedImage;
	HTREEITEM hItem = CTreeCtrl::InsertItem(&tvis);

	SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);

	return hItem;
}

int CStateTreeCtrl::GetItemCheck(HTREEITEM hItem) const
{
	return (hItem == NULL)? STATE_NONE: (GetItemState(hItem, TVIS_STATEIMAGEMASK)>>12);
}

BOOL CStateTreeCtrl::SetItemCheck(HTREEITEM hItem, int nState)
{
	BOOL bState = SetItemState(hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
	if(hItem == GetRootItem())
		SetChildCheck(hItem, nState==STATE_CHECKED);
	else
	{
		if (nState == STATE_UNCHECKED && GetItemCheck(GetRootItem()) == STATE_CHECKED)
		{
			SetItemState(GetRootItem(), INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
		}
		else if (nState == STATE_CHECKED && GetItemCheck(GetRootItem()) == STATE_UNCHECKED)
		{
			// ��������ӽڵ㶼ѡ�У���Ѹ��ڵ�Ҳѡ��
			HTREEITEM hRoot = GetRootItem();
			HTREEITEM hChild = GetChildItem(hRoot);
			BOOL bAllChildChecked = TRUE;
			while(hChild != NULL)
			{
				bAllChildChecked = (GetItemCheck(hChild) == STATE_CHECKED);
				if (!bAllChildChecked) break;
				hChild = GetNextItem(hChild, TVGN_NEXT);
			}

			if (bAllChildChecked) 
				SetItemState(GetRootItem(), INDEXTOSTATEIMAGEMASK(STATE_CHECKED), TVIS_STATEIMAGEMASK);
		}
	}
	return bState; 
}

BOOL CStateTreeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if (hItem && TVHT_ONITEMSTATEICON == uFlags)
	{
		int iImage = GetItemCheck(hItem);

		::SetCursor(m_hCursor);

		//if (iImage == STATE_UNCHECKED || iImage == STATE_NONE) {
		//	::SetCursor(m_hCursorNone);
		//}
		//else {
		//	::SetCursor(m_hCursor);
		//}
		return TRUE;
	}
	return CTreeCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CStateTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
    TVHITTESTINFO tvHitTest;
    tvHitTest.pt = point;

    HTREEITEM hItem = HitTest(&tvHitTest);
    if (hItem != NULL)
    {
		if (ToggleCheckState(hItem, tvHitTest.flags))
		{
			return;
		}
    }	
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CStateTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (nChar == VK_SPACE) {
		ToggleCheckState(GetSelectedItem(), TVHT_ONITEMSTATEICON);
	}	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CStateTreeCtrl::ToggleCheckState(HTREEITEM hItem, UINT uFlags)
{
	BOOL bOnStateIcon = ((uFlags & TVHT_ONITEMSTATEICON) == TVHT_ONITEMSTATEICON);

    if ((bOnStateIcon == FALSE) || (hItem == NULL)) {
		return FALSE;
    }


	int nState = GetItemCheck(hItem);
	nState = nState==STATE_CHECKED ? STATE_UNCHECKED : STATE_CHECKED;

	//if (nState > STATE_UNCHECKED)
	//	nState = STATE_CHECKED;

	return SetItemCheck(hItem, nState);
}


void CStateTreeCtrl::SetChildCheck(HTREEITEM hTree,BOOL bCheck)
{
	hTree = GetChildItem(hTree);  //��ȡ������
	while (hTree)
	{

		SetCheck(hTree, bCheck);
		SetChildCheck(hTree,bCheck);     //�ݹ����
		hTree = GetNextSiblingItem(hTree);    //��ȡ�ֵܵľ��
	}
}

BOOL CStateTreeCtrl::SetBKImage(UINT nResource)
{
	if( m_bitmap.m_hObject != NULL ) // m_bitmap ΪCBitmap �ͱ��������ڴ�ű���λͼ
		m_bitmap.DeleteObject(); //���m_bitmap ��ԭ������

	//HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(nResource), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);//����LoadImage ����λͼ�ľ��

	m_bitmap.LoadBitmap(nResource);

	//if( hbmp == NULL ) //���λͼװ��ʧ�ܣ�����FALSE
	//	return FALSE;

	//m_bitmap.Attach(hbmp); //��λͼ�ڳ�Ա����m_bitmap ������ϵ

	return TRUE;

}
void CStateTreeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcclient; // rcclient ���ڴ�����οؼ��Ĵ�С
	GetClientRect(&rcclient); //�õ����οؼ������С

	//���ڴ��д��������豸������
	CDC memdc; //�����ڴ��豸DC
	memdc.CreateCompatibleDC(&dc); //��������ʾ�豸����DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rcclient.Width(), rcclient.Height());
	//���ú���CreateCompatibleBitmap ����λͼ
	memdc.SelectObject( &bitmap ); //λͼ���ѡ�����DC

	//���ڴ�CDC �����϶�TreeView Ĭ�ϻ�ͼ��
	CWnd::DefWindowProc(WM_PAINT, (WPARAM)memdc.m_hDC , 0);
	CDC maskdc;
	maskdc.CreateCompatibleDC(&dc);
	CBitmap maskbitmap;
	maskbitmap.CreateBitmap(rcclient.Width(), rcclient.Height(), 1, 1, NULL); //������ɫλͼ
	maskdc.SelectObject( &maskbitmap );
	maskdc.BitBlt( 0, 0, rcclient.Width(), rcclient.Height(), &memdc, rcclient.left, rcclient.top, SRCCOPY);

	//��λͼ���ص���ʾ�豸DC ��
	CBrush brush;
	brush.CreatePatternBrush(&m_bitmap);
	dc.FillRect(rcclient, &brush);

	//λͼ��Ĭ�ϱ���������������ϳɱ���ͼ
	memdc.SetBkColor(RGB(0,0,0));
	memdc.SetTextColor(RGB(255,255,255));
	memdc.BitBlt(rcclient.left, rcclient.top, rcclient.Width(), rcclient.Height(), &maskdc, rcclient.left, rcclient.top,SRCAND);
	dc.SetBkColor(RGB(255,255,255));
	dc.SetTextColor(RGB(0,0,0));
	dc.BitBlt(rcclient.left, rcclient.top, rcclient.Width(), rcclient.Height(), &maskdc, rcclient.left, rcclient.top, SRCAND);

	//�����οؼ��л�������ͼ
	dc.BitBlt(rcclient.left, rcclient.top, rcclient.Width(), rcclient.Height(), &memdc, rcclient.left, rcclient.top,SRCPAINT);
	brush.DeleteObject();
	SystemParametersInfo(SPI_SETSCREENSAVERRUNNING,0,NULL,0);
}

void CStateTreeCtrl::OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	Invalidate();
	SetRedraw(TRUE);

	*pResult = 0;
}

void CStateTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	SetRedraw(FALSE);

	*pResult = 0;
}

BOOL CStateTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CXTPTreeCtrl::OnEraseBkgnd(pDC);
}
