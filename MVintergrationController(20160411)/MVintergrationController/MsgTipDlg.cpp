// MsgTipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MsgTipDlg.h"

#define COLOR_WHITE RGB(255,255,255)
#define COLOR_BUFF  RGB(253,245,230)
#define COLOR_MAGENTA RGB(255,0,255)

/////////////////////////////////////////////////////////////////////////////
// CMsgTipDlg dialog

IMPLEMENT_DYNAMIC(CMsgTipDlg, CDialog)

CMsgTipDlg::CMsgTipDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CMsgTipDlg::IDD, pParent)
{
}

CMsgTipDlg::~CMsgTipDlg()
{

}


void CMsgTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgTipDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgTipDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgTipDlg message handlers

BOOL CMsgTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_MsgTipFont.CreateFont(
		16,// nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Times New Roman");//"MS Sans Serif");//Arial
	m_nTipNameWidth = 0;
	m_nRowHeight = 0;	

	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	//WS_EX_LAYERED
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)
			//fun(this->GetSafeHwnd(),0,400,2); 
			fun(this->GetSafeHwnd(),0,220,2);
		FreeLibrary(hInst); 
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//供外部函数传递需要显示的内容，同时获得需要显示的内容的尺寸，以决定本界面的大小
CSize CMsgTipDlg::ShowMsgTip(CMsgTipArray &arMsgTip)
{
	CSize size(0,0);
	//////////////////////////////////////
	m_arMsgTip.Copy(arMsgTip);

	int nSize = m_arMsgTip.GetSize();
	if(nSize == 0)
		return size;

	CDC *pDC = GetDC();
	pDC->SelectObject(&m_MsgTipFont);
	int nMaxNameLen = 0;
	int nMaxInfoLen = 0;
	int nMaxTitleLen = 0;
	CSize sz;
	for(int i=0; i<nSize; i++)
	{
		MSG_TIP mt = m_arMsgTip.GetAt(i);
		if(mt.nInfoShowColor == -1)
		{
			::GetTextExtentPoint32(pDC->m_hDC, mt.sMsgItemName,mt.sMsgItemName.GetLength(),&sz);
			if(sz.cx > nMaxTitleLen)
				nMaxTitleLen = sz.cx;
			continue;
		}
		::GetTextExtentPoint32(pDC->m_hDC, mt.sMsgItemName,mt.sMsgItemName.GetLength(),&sz);
		if(sz.cx > nMaxNameLen)
			nMaxNameLen = sz.cx;
		if(mt.sMsgItemInfo.GetLength() == 0)
			continue;
		else
		{
			::GetTextExtentPoint32(pDC->m_hDC, mt.sMsgItemInfo,mt.sMsgItemInfo.GetLength(),&sz);
			if(sz.cx > nMaxInfoLen)
				nMaxInfoLen = sz.cx;
		}

	}
	m_nTipNameWidth = nMaxNameLen + 8;
	m_nRowHeight = sz.cy + 4;
	//
	int nTitleWidth = nMaxTitleLen;
	size.cx = m_nTipNameWidth + nMaxInfoLen+5;
	if(size.cx < nTitleWidth)
		size.cx = nTitleWidth;
	size.cx += 5;
	size.cy = m_nRowHeight * nSize;
	ReleaseDC(pDC);
	return size;
}

void CMsgTipDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawMsg(&dc);

	// Do not call CDialog::OnPaint() for painting messages
}

void CMsgTipDlg::DrawMsg(CDC *pDC)
{
	CRect rc;
	GetClientRect(&rc);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	CBitmap *pOldBmp = memDC.SelectObject(&bmp);
	memDC.FillSolidRect(rc,RGB(180,250,180));
	CFont *pOldFont = memDC.SelectObject(&m_MsgTipFont);

	DrawGrid(&memDC,rc);
	//
	int nSize = m_arMsgTip.GetSize();
	CPoint topPos(0,0);
	for(int i=0; i<nSize; i++)
	{
		MSG_TIP mt = m_arMsgTip.GetAt(i);
		DrawMsgTip(&memDC,topPos,mt);
		topPos.y += m_nRowHeight;
	}
	pDC->BitBlt(0, 0, rc.Width(),rc.Height(), &memDC, 0, 0, SRCCOPY) ;
	pDC->SelectObject(pOldBmp);
	pDC->SelectObject(pOldFont);
	bmp.DeleteObject();
	memDC.DeleteDC();
}

void CMsgTipDlg::DrawMsgTip(CDC *pDC, CPoint topPos, MSG_TIP &mt)
{
	int nBkMode = pDC->SetBkMode(TRANSPARENT);
	if(mt.nInfoShowColor == -1)
	{
		CRect rc;
		GetClientRect(&rc);
		CRect rowRc(topPos,CSize(rc.Width(),m_nRowHeight+2));
		pDC->FillSolidRect(rowRc,RGB(250,250,210));
		COLORREF nTC = pDC->SetTextColor(COLOR_MAGENTA);
		pDC->TextOut(topPos.x+4,topPos.y+4,mt.sMsgItemName);
		pDC->SetTextColor(nTC);
	}
	else
	{
		COLORREF nTC = pDC->SetTextColor(mt.nInfoShowColor);
		pDC->TextOut(topPos.x+4,topPos.y+4,mt.sMsgItemName);
		//nTC = pDC->SetTextColor(mt.nInfoShowColor));//mt.nInfoShowColor);//
		pDC->TextOut(topPos.x+m_nTipNameWidth+8,topPos.y+4,mt.sMsgItemInfo);
		pDC->SetTextColor(nTC);
	}
	pDC->SetBkMode(nBkMode);
}

void CMsgTipDlg::DrawGrid(CDC *pDC,CRect &rc)
{
	CPen pen(PS_SOLID,1,COLOR_BUFF);
	CPen *pOldPen = pDC->SelectObject(&pen);
	CPoint ptTop(m_nTipNameWidth,0);
	CPoint ptBottom(m_nTipNameWidth,rc.bottom);
	pDC->MoveTo(ptTop);
	pDC->LineTo(ptBottom);
	//
	int nSize = m_arMsgTip.GetSize();
	for(int i=0; i<nSize-1; i++)
	{
		CPoint ptLeft(0,(m_nRowHeight)*(i+1)+2);
		CPoint ptRight(rc.right,(m_nRowHeight)*(i+1)+2);
		pDC->MoveTo(ptLeft);
		pDC->LineTo(ptRight);
	}
	pDC->SelectObject(pOldPen);
}

void CMsgTipDlg::UpdateTrans()
{
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL;
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if(fun)
			//fun(this->GetSafeHwnd(),0,400,2); 
			fun(this->GetSafeHwnd(),0, 220,2);
		FreeLibrary(hInst); 
	}
	if(m_MsgTipFont.m_hObject != NULL)
	{
		m_MsgTipFont.DeleteObject();
		m_MsgTipFont.CreateFont(
			16,// nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_SEMIBOLD,                  // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			DEFAULT_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			"Times New Roman");//Arial	}
	}
}

void CMsgTipDlg::DrawMsgTip()
{
	CDC *pDC = GetDC();
	DrawMsg(pDC);
	ReleaseDC(pDC);
}

//鼠标不能移动到本界面，一旦移动到本界面，立即隐藏
BOOL CMsgTipDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_MOUSEMOVE)
	{
		ShowWindow(SW_HIDE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
