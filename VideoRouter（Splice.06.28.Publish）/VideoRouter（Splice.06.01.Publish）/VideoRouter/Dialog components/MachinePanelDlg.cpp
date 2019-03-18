// MachinePanelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../VideoRouter.h"
#include "MachinePanelDlg.h"

//using namespace Gdiplus;
// CMachinePanelDlg 对话框

IMPLEMENT_DYNAMIC(CMachinePanelDlg, CDialog)

CMachinePanelDlg::CMachinePanelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMachinePanelDlg::IDD, pParent)
{

}

CMachinePanelDlg::~CMachinePanelDlg()
{
}

void CMachinePanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMachinePanelDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMachinePanelDlg 消息处理程序

LRESULT CMachinePanelDlg::OnNcHitTest(CPoint point)
{
	return CDialog::OnNcHitTest(point);
}

void CMachinePanelDlg::OnSize(UINT nType, int cx, int cy)
{
	if (cx < 600 || cy < 400)
		return;

	CDialog::OnSize(nType, cx, cy);

	CalcImagePosition();

	RedrawWindow();

}

BOOL CMachinePanelDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CMachinePanelDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	//if(nHitTest == HTTOP)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTBOTTOM)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTLEFT)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTRIGHT)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTTOPLEFT)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTTOPRIGHT)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTBOTTOMLEFT)
	//	SendMessage( WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, MAKELPARAM(point.x, point.y));
	//else if(nHitTest == HTBOTTOMRIGHT)
	//	SendMessage(WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, MAKELPARAM(point.x, point.y));


	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CMachinePanelDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// 获取Picture控件的DC
	//CDC *pDC = GetDlgItem(IDC_PIC_PANEL_VIEW)->GetDC();

	// 获取Picture尺寸
	CRect rcPaint;
	//GetDlgItem(IDC_PIC_PANEL_VIEW)->GetClientRect(&rcPaint);
	GetClientRect(&rcPaint);

	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(&dc);

	CBitmap bmp, *pOldBmp;
	bmp.CreateCompatibleBitmap(&dc, rcPaint.Width(), rcPaint.Height());
	pOldBmp = pMemDC->SelectObject(&bmp);

	Graphics gp(pMemDC->m_hDC);

	Image MachineFace(L"res\\MachineFace.bmp");

	RectF rcF((REAL)rcPaint.left, (REAL)rcPaint.top, (REAL)rcPaint.Width(), (REAL)rcPaint.Height());
	gp.DrawImage(&MachineFace, rcF);

	//Color colors[] = {
	//	Color(255, 80, 0, 80),
	//	Color(255, 168, 0, 168),
	//	Color(255, 80, 0, 80),
	//};

	Image EmptyInBoard(L"res\\EmptyBoard.bmp");
	Image EmptyBoardOut(L"res\\EmptyBoardOut.bmp");
	Image DviIn(L"res\\DVI_IN.bmp");
	Image DviOut(L"res\\DVI_OUT.bmp");
	Image HdmiIn(L"res\\HDMI_IN.bmp");
	Image HdmiOut(L"res\\HDMI_OUT.bmp");

	RectF rcTmp;

	for (int h=0; h<HORZ_BOARD_NUM; h++)
		for (int v=0; v<VERT_BOARD_NUM; v++)
		{
			rcTmp.X = (REAL)m_rcBoard[h][v].left;
			rcTmp.Y = (REAL)m_rcBoard[h][v].top;
			rcTmp.Width = (REAL)m_rcBoard[h][v].Width();
			rcTmp.Height = (REAL)m_rcBoard[h][v].Height();
			if (h==0)
				gp.DrawImage(&EmptyInBoard, rcTmp);
			else
				gp.DrawImage(&EmptyBoardOut, rcTmp);
		}

	for (int h=0; h<HORZ_BOARD_NUM; h++)
	{
		for (int v=0; v<VERT_BOARD_NUM; v++)
		{
			if (v%2 == 1)
				continue;

			for (int p=0; p<TERMINALS_ONE_BOARD; p++)
			{
				rcTmp.X = (REAL)m_rcTerm[h][v][p].left;
				rcTmp.Y = (REAL)m_rcTerm[h][v][p].top;
				rcTmp.Width = (REAL)m_rcTerm[h][v][p].Width();
				rcTmp.Height = (REAL)m_rcTerm[h][v][p].Height();

				if (rcTmp.Width < 10 || rcTmp.Height < 5)
					break;

				if ( h==0 && (v%(HORZ_BOARD_NUM)) == 0 )
					gp.DrawImage(&DviIn, rcTmp);
				else if (h == 0 && (v%(HORZ_BOARD_NUM)) ==1)
					gp.DrawImage(&HdmiIn, rcTmp);
				else if (h == 1 && (v%(HORZ_BOARD_NUM)) == 0)
					gp.DrawImage(&DviOut, rcTmp);
				else if (h == 1 && (v%(HORZ_BOARD_NUM)) == 1)
					gp.DrawImage(&HdmiOut, rcTmp);
			}
		}
	}


	//REAL positions[] = {
	//	0.0f,
	//	0.45f,
	//	1.0f
	//};

	//LinearGradientBrush GradientBrushBrown(Point(0,0), Point(0, rcPaint.bottom), Color(255, 40, 140, 120), Color(255, 76, 176, 156));
	//GradientBrushBrown.SetInterpolationColors(colors, positions, 3);
	//gp.FillRectangle(&GradientBrushBrown, rcPaint.left, rcPaint.top, rcPaint.Width(), rcPaint.Height());

	dc.BitBlt(0, 0, rcPaint.Width(), rcPaint.Height(), pMemDC, 0, 0, SRCCOPY);

	ReleaseDC(pMemDC);
	delete pMemDC;
	pMemDC = NULL;
}

void CMachinePanelDlg::CalcImagePosition()
{
	CRect rcCli;
	GetClientRect(&rcCli);

	// Machine Frame Margins;
	const int iMachineFrmWid  = rcCli.Width();   //机箱面板的宽度
	const int iMachineFrmHei  = rcCli.Height();  //机箱面板的高度
	const int iMachineFrmTop  = 10;              //机箱面板的上下边框高度
	const int iMachineFrmLeft = 10;              //机箱面板的左右边框宽度
	const int iMachineFrmHSp  = 15;              //机箱上板卡之间的水平间隙
	const int iMachineFrmVSp  = 5;              //机箱上板卡之间的垂直间隙

	// Board Frame Margins;
	const int iBoardFrmWid  = (rcCli.Width() - iMachineFrmLeft*2 - iMachineFrmHSp)/HORZ_BOARD_NUM;    //板卡的宽度
	const int iBoardFrmHei  = (rcCli.Height() - iMachineFrmTop*2 - iMachineFrmVSp*(VERT_BOARD_NUM-1))/VERT_BOARD_NUM;  //板卡的高度
	const int iBoardFrmTop  = 10;														//板卡的上下边框高度
	const int iBoardFrmBtm  = 3;
	const int iBoardFrmLeft = int(iBoardFrmWid*0.075);                                    //板卡的左右边框宽度
	const int iBoardFrmHSp  = int(iBoardFrmWid*0.02);                                     //板卡上端口之间的水平间隙

	// Terminal Frame Margins;
	const int iTermWid = int((iBoardFrmWid - iBoardFrmLeft*2 - iBoardFrmHSp*(TERMINALS_ONE_BOARD-1))/TERMINALS_ONE_BOARD); //端子的宽度
	const int iTermHei = int(iBoardFrmHei - iBoardFrmTop - iBoardFrmBtm);                    //端子的高度

 	for (int h=0; h<HORZ_BOARD_NUM; h++)
 	{
 		for (int v=0; v<VERT_BOARD_NUM; v++)
 		{
 			m_rcBoard[h][v].left   = rcCli.left + iMachineFrmLeft + iBoardFrmWid*h + iMachineFrmHSp*h;
 			m_rcBoard[h][v].top    = rcCli.top + iMachineFrmTop + iBoardFrmHei*v + iMachineFrmVSp*v;
 			m_rcBoard[h][v].right  = m_rcBoard[h][v].left + iBoardFrmWid;
 			m_rcBoard[h][v].bottom = m_rcBoard[h][v].top + iBoardFrmHei;
 
  			for (int p=0; p<TERMINALS_ONE_BOARD; p++)
  			{
  				m_rcTerm[h][v][p].left   = m_rcBoard[h][v].left + iBoardFrmLeft + iTermWid*p + iBoardFrmHSp*p;
  				m_rcTerm[h][v][p].top    = m_rcBoard[h][v].top + iBoardFrmTop;
  				m_rcTerm[h][v][p].right  = m_rcTerm[h][v][p].left + iTermWid;
  				m_rcTerm[h][v][p].bottom = m_rcTerm[h][v][p].top + iTermHei;				
  			}
 		}
 	}
}

BOOL CMachinePanelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	CalcImagePosition();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMachinePanelDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

// 	if (nState == )
// 	{
// 	}
}
BOOL CMachinePanelDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CMachinePanelDlg::OnClose()
{
	ShowWindow(SW_HIDE);

	//CDialog::OnClose();
}
