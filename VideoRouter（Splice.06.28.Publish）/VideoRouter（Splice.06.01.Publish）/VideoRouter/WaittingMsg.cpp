// LoadingMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "WaittingMsg.h"

// CWaitingMsg 对话框

IMPLEMENT_DYNAMIC(CWaittingMsg, CDialog)

CWaittingMsg::CWaittingMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaittingMsg::IDD, pParent)
{
	m_iRotate = 0;
}

CWaittingMsg::CWaittingMsg(CStringW strMsg, CWnd* pParent /*=NULL*/)
: CDialog(CWaittingMsg::IDD, pParent)
{
	m_strMsg = strMsg;
	m_iRotate = 0;
}

CWaittingMsg::~CWaittingMsg()
{
}

void CWaittingMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaittingMsg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWaitingMsg 消息处理程序

void CWaittingMsg::OnPaint()
{
	CPaintDC dc(this); 

	Graphics gp(dc.m_hDC);

	CRect rcCli;
	GetClientRect(&rcCli);

	Image* imgBk;
	if (ImageFromIDResource(IDB_PNG_TOP_BORDER, "png", imgBk))
	{
		gp.DrawImage(imgBk, Rect(rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height()));

		SAFE_DELETE(imgBk);
	}

	Rect rcImg;
	RectF rcText;
	Image* imgLoading;
	if (ImageFromIDResource(IDB_PNG_LOADING, "png", imgLoading))
	{
		rcImg.X = rcCli.left+ 10;//(rcCli.Width()-imgLoading->GetWidth())/2;
		rcImg.Y = (rcCli.Height()-imgLoading->GetHeight())/2;//rcCli.top+2;
		rcImg.Width = imgLoading->GetWidth();
		rcImg.Height = imgLoading->GetHeight();

		if (m_iRotate !=0 )
			imgLoading->RotateFlip(RotateFlipType(m_iRotate));

		gp.DrawImage(imgLoading, rcImg);

		SAFE_DELETE(imgLoading);
	}

	rcText.X = rcImg.GetRight()+5.0f;//(REAL)rcCli.left;
	rcText.Y = (REAL)rcCli.top;//(REAL)rcImg.GetBottom() + 2;
	rcText.Width = (REAL)rcCli.Width() - rcImg.GetRight();//(REAL)rcCli.Width();
	rcText.Height = (REAL)rcCli.Height();//(REAL)(rcCli.bottom - rcText.Y);

	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 14.0f, FontStyleBold, UnitPixel);
	//Gdiplus::Font Bfont(&fontFamily, (REAL)pMargins->PageScale.iFontHeight, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(255, 116, 101, 131));
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	CStringW strDot;
	for (int i=0; i<m_iRotate; i++)
		strDot.Append(L"..");

	gp.DrawString(m_strMsg+strDot, -1, &font, rcText, &format, &blackBr);

}

BOOL CWaittingMsg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CWaittingMsg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1000)
	{
		m_iRotate++;
		if (m_iRotate > 3)
			m_iRotate = 0;

		Invalidate();
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CWaittingMsg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1000, 200, NULL);

	return TRUE;
}

void CWaittingMsg::Reset()
{
	m_iRotate = 0;
}

void CWaittingMsg::SetMessage(CStringW strMsg)
{
	m_strMsg = strMsg;
}