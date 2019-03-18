// BnItem.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "BnItem.h"


// CBnItem

IMPLEMENT_DYNAMIC(CBnItem, CWnd)

CBnItem::CBnItem()
{
	//*********************
	//m_uID = 0;
	m_rcBn = Rect(0, 0, 0, 0);
	m_RFT = RotateNoneFlipNone;
	m_bnState = bn_Normal;

	m_uImageNormal = 0;
	m_uImageFocus  = 0;
	m_uImageClick  = 0;

	//*********************
	m_bLBnDown = FALSE;
	m_bShowFrm = TRUE;
}

CBnItem::CBnItem(/*UINT nID, */Rect rc, UINT uNormal, UINT uFocus, UINT uClick, UINT uDis, RotateFlipType RFT)
{
	//*********************
	//m_uID = nID;
	m_rcBn = rc;
	m_RFT = RFT;
	m_bnState = bn_Normal;

	SetImages(uNormal, uFocus, uClick, uDis);

	//*********************
	m_bLBnDown = FALSE;
	m_bShowFrm = TRUE;
}

CBnItem::~CBnItem()
{
}

//void CBnItem::SetItemID(UINT nID)
//{
//	m_uID = nID;
//}

void CBnItem::SetItemRect(INT x, INT y, INT cx, INT cy)
{
	m_rcBn.X = x;
	m_rcBn.Y = y;
	m_rcBn.Width  = cx;
	m_rcBn.Height = cy;
}

void CBnItem::SetItemRect(const Rect rect)
{
	m_rcBn = rect;
}

void CBnItem::SetItemState(bnState state)
{
	m_bnState = state;
}

void CBnItem::Enable(BOOL bEnable/* =TRUE */)
{
	m_bnState =bEnable ? bn_Normal : bn_Disable;

	if (GetSafeHwnd())
	{
		EnableWindow(bEnable);
		Invalidate();
	}
}

void CBnItem::SetImages(UINT uNormal, UINT uFocus, UINT uClick, UINT uDis)
{
	m_uImageNormal = uNormal;
	m_uImageFocus  = uFocus;
	m_uImageClick  = uClick;
	m_uImageDisab  = uDis;
}

void CBnItem::SetRotateFlipType(RotateFlipType RFT)
{
	m_RFT = RFT;
}

void CBnItem::ShowBoxFrame(BOOL bShow/* =TRUE */)
{
	m_bShowFrm = bShow;
}

BEGIN_MESSAGE_MAP(CBnItem, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBnItem 消息处理程序

BOOL CBnItem::BnCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const Rect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /* = NULL */ )
{
	RECT rcWnd;
	rcWnd.left = rect.GetLeft();
	rcWnd.right = rect.GetRight();
	rcWnd.top = rect.GetTop();
	rcWnd.bottom = rect.GetBottom();

	return CreateEx(0,
		AfxRegisterWndClass(0, LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))),
		0,
		dwStyle, 
		rcWnd,
		pParentWnd,
		nID);
	//return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rcWnd, pParentWnd, nID, pContext);
}

void CBnItem::OnPaint()
{
	CPaintDC dc(this);

	Graphics gp(dc.m_hDC);

	CRect rcCli;
	GetClientRect(&rcCli);

	if (m_bShowFrm)
	{
		Color clrBkS = ((m_bnState == bn_Normal || m_bnState == bn_Disable) ? Color(255, 218, 234, 255) : Color(255, 234, 234, 218));
		Color clrBkE = ((m_bnState == bn_Normal || m_bnState == bn_Disable) ? Color(255, 183, 215, 255) : Color(255, 255, 255, 183));

		LinearGradientBrush br(Point(rcCli.left, rcCli.top), Point(rcCli.left, rcCli.bottom), clrBkS, clrBkE);
		gp.FillRectangle(&br, 0, 0, rcCli.Width(), rcCli.Height());
	}

	Image* img;
	ImageFromIDResource((m_bnState==bn_Clicked) ? m_uImageClick : (m_bnState==bn_Focused ? m_uImageFocus : (m_bnState == bn_Disable ? m_uImageDisab : m_uImageNormal)), "png", img);
	if (img)
	{
		Rect rcImg;
		rcImg.X = (rcCli.Width()-img->GetWidth())/2;
		rcImg.Y = (rcCli.Height()-img->GetHeight())/2;
		rcImg.Width  = img->GetWidth();
		rcImg.Height = img->GetHeight();

		if (m_RFT != RotateNoneFlipNone)
		{
			img->RotateFlip(m_RFT);
		}

		gp.DrawImage(img, rcImg);


		SAFE_DELETE(img);
	}
}

void CBnItem::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	if (GetDlgCtrlID() == ID_BNITEM_LREBAR)
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));

	m_bLBnDown = TRUE;
	m_bnState = bn_Clicked;

	RedrawWindow();

	CWnd::OnLButtonDown(nFlags, point);
}

void CBnItem::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	if (m_bLBnDown)
	{
		::SendMessage(GetParent()->m_hWnd, GetDlgCtrlID(), 0, 0);
	}

	m_bLBnDown = FALSE;
	m_bnState = bn_Normal;

	CWnd::OnLButtonUp(nFlags, point);
}

void CBnItem::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetDlgCtrlID() == ID_BNITEM_LREBAR)
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));

	if (m_bLBnDown)
	{
		// 左键按下后鼠标移动处理
	}
	else
	{
		// 鼠标飘过窗口
		m_bnState = bn_Focused;
		
		RedrawWindow();

		SetTimer(TIMER_KILLFOCUS, 100, NULL);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CBnItem::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_KILLFOCUS:
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			//ScreenToClient(&ptCursor);

			CRect rcWnd;
			GetWindowRect(&rcWnd);
			if (!rcWnd.PtInRect(ptCursor))
			{
				m_bnState = bn_Normal;
				RedrawWindow();

				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

				KillTimer(nIDEvent);
			}
		}

		break;

	default:break;
	}
	

	CWnd::OnTimer(nIDEvent);
}
