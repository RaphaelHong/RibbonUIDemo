// MvxSplitDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MvxSplitDlg.h"
#include "MvxWndPropertyDlg.h"
#include "MvxBackgrdSettingDlg.h"
#include "MvxVideoPropertyDlg.h"

COLORREF WINDOWBACKGROUNDCOLORDEF[]=
{
	RGB(0x63,0x5d,0xbb),
	RGB(0xaf,0x5d,0x22),
	RGB(0x88,0x22,0x55),
	RGB(0xcc,0x99,0x33),
	RGB(0xcc,0x33,0x33),
	RGB(0x33,0xcc,0x33),
	RGB(0x33,0xcc,0xcc),
	RGB(0x33,0x33,0xcc),
	RGB(0x66,0x33,0xcc),
	RGB(0xcc,0x33,0x99),
};

int CMvxSplitDlg::stcMvxObjCount = 0;
// CMvxSplitDlg �Ի���

IMPLEMENT_DYNAMIC(CMvxSplitDlg, CXTPResizeDialog)

CMvxSplitDlg::CMvxSplitDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMvxSplitDlg::IDD, pParent)
{
	//�������
	m_nObjOder = stcMvxObjCount++;

	//���ܰ�ť��ʼ��
	for (int i=0; i<MAX_BTN; i++)	{	m_xBtn[i].nID = i;	}

	m_xBtn[0].strSurfaceName = _T("��������");
	m_xBtn[1].strSurfaceName = _T("��Ƶ����");
	m_xBtn[2].strSurfaceName = _T("��������");
	m_xBtn[3].strSurfaceName = _T("��������");
	m_xBtn[4].strSurfaceName = _T("��ݲ˵�");

	//Ĭ�ϱ����ֱ���
	m_nPhyHorz       = 1024;
	m_nPhyVert       = 768;
	memset(&m_BkgTiming, 0, sizeof(TIMING_STRUCT));

	//Ĭ������
	m_nHorzLines     = 3;
	m_nVertLines     = 2;

	//��������ʼ��
	m_bLBtnDown      = FALSE;
	m_bLBtnDownInWnd = FALSE;
	m_nFocusID       = ErrWinID;
	m_nNewWndID      = ErrWinID;
	m_bNewCreated    = FALSE;

	m_eResizeDir     = eResize_Invalid;
	m_eMoveDir       = eNoMove;
}

CMvxSplitDlg::~CMvxSplitDlg()
{
	stcMvxObjCount--;
}

void CMvxSplitDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMvxSplitDlg, CXTPResizeDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SAVE_SCENE_FILE, OnSaveSceneFile)
	ON_COMMAND(ID_READ_SCENE_FILE, OnReadSceneFile)
	ON_COMMAND_RANGE(ID_WRITE_DEV_SCENE1, ID_WRITE_DEV_SCENE8, OnWriteDevScene)
	ON_COMMAND_RANGE(ID_LOAD_DEV_SCENE1, ID_LOAD_DEV_SCENE8, OnLoadDevScene)
	ON_COMMAND_RANGE(ID_MVX_OPEN_WINDOW, ID_MVX_SAME_SIZE, OnWindowOption)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CMvxSplitDlg ��Ϣ�������

BOOL CMvxSplitDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	SetResize(IDC_PIC_MVX, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	CalcBtnRect();

	CalcAnchorPoint();

	m_bkBrush.CreateSolidBrush(RGB(50,180,240));

	return TRUE;
}

void CMvxSplitDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	PaintBottomPicture();

	PaintTopPicture();
}

//*************************************************************************************
//                 ���Ƶ��µ�ͼƬ --- ���ܰ�ť��
//*************************************************************************************
void CMvxSplitDlg::PaintBottomPicture()
{
	//��ȡ��ͼDC
	CDC *pDC = GetDlgItem(IDC_PIC_MVX)->GetDC();

	//�����ڴ�DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//�ÿ�λͼ��Ϊ����
	CBitmap MemBmp, *pOldBmp;
	MemBmp.CreateCompatibleBitmap(pDC, m_rcBtnPic.Width(), m_rcBtnPic.Height());
	pOldBmp = pMemDC->SelectObject(&MemBmp);

	//Ϊ������ɫ
	pMemDC->FillSolidRect(0 , 0, m_rcBtnPic.Width(), m_rcBtnPic.Height(), RGB(193,243,133));

	//�����ܰ�ť
	DrawCustomBtn(pMemDC);

	//���ڴ�DC��������ͼDC��
	pDC->BitBlt(0, 0, m_rcBtnPic.Width(), m_rcBtnPic.Height(), pMemDC, 0, 0, SRCCOPY);

	//��Դ�ͷ�
	pMemDC->SelectObject(pOldBmp);

	pMemDC->DeleteDC();
	delete pMemDC;
	pMemDC = NULL;

	ReleaseDC(pDC);
}

//*************************************************************************************
//                 ���ƶ��ϵ�ͼƬ --- ��������
//*************************************************************************************
void CMvxSplitDlg::PaintTopPicture()
{
	//��ȡ��ͼDC
	CDC *pDC = GetDlgItem(IDC_PIC_MVX_CHART)->GetDC();

	//�����ڴ�DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//ѡȡ��λͼ
	CBitmap MemBmp, *pOldBmp;
	MemBmp.CreateCompatibleBitmap(pDC, m_rcChart.Width(), m_rcChart.Height());
	pOldBmp = pMemDC->SelectObject(&MemBmp);

	//Ϊ������ɫ
	pMemDC->FillSolidRect(0, 0, m_rcChart.Width(), m_rcChart.Height(), RGB(50,30,60));//RGB(0xe6, 0xe8, 0xfa)

	//������
	DrawGridLine(pMemDC);

	//�ֱ���
	DrawResolution(pMemDC);

	//����
	ReflashWindow(pMemDC);

	//�����ڴ�DC����ͼDC
	pDC->BitBlt(0, 0, m_rcChart.Width(), m_rcChart.Height(), pMemDC, 0, 0, SRCCOPY);

	//�ͷ���Դ
	pMemDC->SelectObject(pOldBmp);

	pMemDC->DeleteDC();
	delete pMemDC;
	pMemDC = NULL;

	ReleaseDC(pDC);
}

void CMvxSplitDlg::DrawCustomBtn(CDC *pValidDC)
{
	CFont markFont, boldFont, *pOldFont;
	VERIFY(markFont.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_REGULAR,                // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	VERIFY(boldFont.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	pOldFont = pValidDC->SelectObject(&markFont);

	CPen bluepen, *pOldPen;
	bluepen.CreatePen(PS_SOLID, 2, RGB(50, 220, 240));
	pOldPen = pValidDC->SelectObject(&bluepen);

	CSize textExt;

	pValidDC->SetBkMode(TRANSPARENT);
	for (int i=0 ; i<MAX_BTN; i++)
	{
		pValidDC->FillSolidRect(m_xBtn[i].rcWnd, m_xBtn[i].bClicked ? RGB(50,150,50) : m_xBtn[i].bFocused ? RGB(120, 200, 150) : RGB(180, 250, 200));
		pValidDC->FrameRect(m_xBtn[i].rcWnd, NULL);

		if (m_xBtn[i].bFocused) pValidDC->SelectObject(&boldFont);
		else pValidDC->SelectObject(&markFont);

		textExt = pValidDC->GetTextExtent(m_xBtn[i].strSurfaceName);
		pValidDC->SetTextColor(RGB(20,50,40));

		pValidDC->TextOut(m_xBtn[i].rcWnd.left + Round((m_xBtn[i].rcWnd.Width()-textExt.cx)*0.5),
			              m_xBtn[i].rcWnd.top + Round((m_xBtn[i].rcWnd.Height()-textExt.cy)*0.5),
						  m_xBtn[i].strSurfaceName);
	}

	pValidDC->SelectObject(pOldPen);
	pValidDC->SelectObject(pOldFont);
}

void CMvxSplitDlg::DrawGridLine(CDC *pValidDC)
{
	CPen bluepen, dotpen, *pOldPen;
	bluepen.CreatePen(PS_SOLID, 1, RGB(255, 255, 100));
	dotpen.CreatePen(PS_DOT, 1, RGB(63, 245, 180));

	pOldPen = pValidDC->SelectObject(&bluepen);
	pValidDC->MoveTo(m_rcChart.left, m_rcChart.top);
	pValidDC->LineTo(m_rcChart.right-1,m_rcChart.top);
	pValidDC->LineTo(m_rcChart.right-1,m_rcChart.bottom-1);
	pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-1);
	pValidDC->LineTo(m_rcChart.left, m_rcChart.top);

	pValidDC->SelectObject(&dotpen);
	for (int i=1; i<m_nHorzLines; i++)
	{
		pValidDC->MoveTo(i*m_rcChart.Width()/m_nHorzLines, m_rcChart.top);
		pValidDC->LineTo(i*m_rcChart.Width()/m_nHorzLines, m_rcChart.bottom);
	}

	for (int i=1; i<m_nVertLines; i++)
	{
		pValidDC->MoveTo(m_rcChart.left,  i*m_rcChart.Height()/m_nVertLines);
		pValidDC->LineTo(m_rcChart.right, i*m_rcChart.Height()/m_nVertLines);
	}

	pValidDC->SelectObject(pOldPen);
}


void CMvxSplitDlg::DrawResolution(CDC *pValidDC)
{
	CFont markFont, *pOldFont;
	VERIFY(markFont.CreateFont(
		40,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		TRUE,                      // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	pOldFont = pValidDC->SelectObject(&markFont);

	CString strRes;
	strRes.Format(_T("%dx%d @ %dHz"), m_nPhyHorz, m_nPhyVert, m_BkgTiming.m_Hz);

	CSize txtExt;
	txtExt = pValidDC->GetTextExtent(strRes);

	CPoint ptMark;
	ptMark.x = m_rcChart.CenterPoint().x - int(txtExt.cx*0.5);
	ptMark.y = m_rcChart.CenterPoint().y - int(txtExt.cy*0.5);
	ptMark.Offset(-m_rcChart.left, -m_rcChart.top);

	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(255,255,200));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	ptMark.Offset(1,1);
	pValidDC->SetTextColor(RGB(178,145,240));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	pValidDC->SelectObject(pOldFont);
}

void CMvxSplitDlg::ReflashWindow(CDC *pDC)
{
	int nCount = m_ArrWinInfo.GetCount();

	for (int i=0; i<MAX_WIN; i++)
	{
		for (int j=0; j<nCount; j++)
		{
			if (m_ArrWinInfo[j].iZorder == i)
			{
				DrawWindow(pDC, j);
			}
		}
	}
}

void CMvxSplitDlg::DrawWindow(CDC *pValidDC, int nIndex)
{
	CRect rcWnd = m_ArrWinInfo[nIndex].rcLog;
	int nWndID  = m_ArrWinInfo[nIndex].WinId;

	//����һ���ʹ��ڴ�Сһ���ı���λͼ
	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pValidDC, rcWnd.Width(), rcWnd.Height());

	//����һ����ͼDC
	CDC *pDC = new CDC;
	pDC->CreateCompatibleDC(NULL);
	pDC->SelectObject(MemBmp);
	pDC->SetBkMode(TRANSPARENT);

	//���⻭ˢ
	CBrush BrushBlue(RGB(101,211,246));
	CBrush BrushGray(RGB(90, 170, 90));
	CBrush* pOldBrush;
	if(m_ArrWinInfo[nIndex].WinId == m_nFocusID)
		pOldBrush = pDC->SelectObject(&BrushBlue);
	else
		pOldBrush = pDC->SelectObject(&BrushGray);

	CRect rcTitle, rcButton;

	//����������
	rcTitle.left   = 0;
	rcTitle.top    = 0;
	rcTitle.right  = rcWnd.Width();
	rcTitle.bottom = 21;
	m_ArrWinInfo[nIndex].rcTitle = rcTitle;
	m_ArrWinInfo[nIndex].rcTitle.OffsetRect(rcWnd.left, rcWnd.top);

	pDC->PatBlt(0, 0, rcWnd.Width(), 21, PATCOPY);
	pDC->SelectObject(pOldBrush);

	//�����ڿ�ܡ�
	CPen YellowPen(PS_SOLID, 2, RGB(250, 240, 78));
	CPen *pOldPen = pDC->SelectObject(&YellowPen);

	pDC->MoveTo(0,0);
	pDC->LineTo(0,rcWnd.Height()-1);
	pDC->LineTo(rcWnd.Width()-1,rcWnd.Height()-1);
	pDC->LineTo(rcWnd.Width()-1,0);
	pDC->LineTo(0,0);

	pDC->SelectObject(pOldPen);

	//׼����ťλͼDC
	CBitmap *pBmp = new CBitmap;
	CBitmap *pOldBmp;
	CDC *pImageDC = new CDC;
	pImageDC->CreateCompatibleDC(pDC);

	//���رհ�ť��
	rcButton.left  = rcTitle.right-22;
	rcButton.top   = rcTitle.top+2;
	rcButton.right = rcTitle.right - 3;
	rcButton.bottom= rcTitle.top+21;
	m_ArrWinInfo[nIndex].rcBtnExit = rcButton;
	m_ArrWinInfo[nIndex].rcBtnExit.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_EXIT_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//���ָ���ť��
	rcButton.OffsetRect(-19, 0);
	m_ArrWinInfo[nIndex].rcBtnRestore = rcButton;
	m_ArrWinInfo[nIndex].rcBtnRestore.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_RESTORE_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//����󻯰�ť��
	rcButton.OffsetRect(-19, 0);
	m_ArrWinInfo[nIndex].rcBtnMax = rcButton;
	m_ArrWinInfo[nIndex].rcBtnMax.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_MAX_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//�����밴ť��
	rcButton.OffsetRect(-19, 0);
	m_ArrWinInfo[nIndex].rcBtnResize = rcButton;
	m_ArrWinInfo[nIndex].rcBtnResize.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_RESIZE_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//�������屳��ɫ��
	CBrush bkBrush(WINDOWBACKGROUNDCOLORDEF[nWndID]);
	pOldBrush = pDC->SelectObject(&bkBrush);

	pDC->PatBlt(1, rcTitle.bottom, rcWnd.Width()-2, rcWnd.Height()-rcTitle.Height()-1, PATCOPY);
	pDC->SelectObject(pOldBrush);

	//׼���ı�����
	int OutX,OutY;
	if(rcWnd.left<0)	OutX = 0-rcWnd.left;
	else	OutX = 2;

	if(rcWnd.top<0)		OutY = 0-rcWnd.top;
	else	OutY = 2;

	CFont font; 
	font.CreatePointFont(80,_T("Arial")); 
	pDC->SelectObject(&font);

	CString str;
	//���������֡�
	str.Format(_T("%s"), m_ArrWinInfo[nIndex].szTitle);
	pDC->TextOut(OutX+25, OutY, str);
	//�����ڱ�ʾ��
	str.Format(_T("���ڱ�ʾ: %d"), m_ArrWinInfo[nIndex].WinId);
	pDC->TextOut(OutX, OutY+35, str);
	//��������š�
	str.Format(_T("�������: %d"), m_ArrWinInfo[nIndex].iZorder);
	pDC->TextOut(OutX, OutY+55, str);
	//���ź�Դ��
	str.Format(_T("�ź�Դ: %d"), m_ArrWinInfo[nIndex].iSource);
	pDC->TextOut(OutX, OutY+75, str);
	//������λ�á�
	str.Format(_T("����λ��: (%d, %d)"), m_ArrWinInfo[nIndex].rcPhy.left, m_ArrWinInfo[nIndex].rcPhy.top);
	pDC->TextOut(OutX, OutY+95, str);
	//�����ڴ�С��
	str.Format(_T("���ڴ�С: (%d x %d)"), m_ArrWinInfo[nIndex].rcPhy.Width(), m_ArrWinInfo[nIndex].rcPhy.Height());
	pDC->TextOut(OutX, OutY+115, str);
	//��LOGO��
	font.DeleteObject();
	font.CreatePointFont(240,_T("Ms Sans Serif"));
	pDC->SelectObject(&font);
	str.Format(_T("%d"), m_ArrWinInfo[nIndex].WinId+1);
	pDC->SetTextColor(RGB(128,128,128));
	pDC->TextOut(OutX+1, OutY, str);
	pDC->SetTextColor(RGB(254,210,234));
	pDC->TextOut(OutX+3, OutY+2, str);
		

	pValidDC->BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), pDC, 0, 0, SRCCOPY);

	delete pBmp;
	
	ReleaseDC(pImageDC);
	delete pImageDC;

	ReleaseDC(pDC);
	delete pDC;
}

void CMvxSplitDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if(GetDlgItem(IDC_PIC_MVX)->GetSafeHwnd())
	{
		CalcBtnRect();

		//���㿪����λ��
		ZoomPaintRect();

		//���¼���������ڵ��߼������С
		ZoomWindows();
	}
}

// �����Ի水ť������λ��
void CMvxSplitDlg::CalcBtnRect()
{
	GetDlgItem(IDC_PIC_MVX)->GetWindowRect(&m_rcBtnPic);
	ScreenToClient(&m_rcBtnPic);
	
	CRect tmpRc = m_rcBtnPic;
	tmpRc.OffsetRect(-m_rcBtnPic.left, -m_rcBtnPic.top);
	
	int nStart = Round((tmpRc.Width()-MAX_BTN*BTN_WID)*0.5);
	//����ײ�������ť��λ��
	for (int i=0; i<MAX_BTN; i++)
	{
		m_xBtn[i].rcWnd.left   = nStart + i*BTN_WID;
		m_xBtn[i].rcWnd.top    = tmpRc.top;
		m_xBtn[i].rcWnd.right  = nStart + i*BTN_WID + BTN_WID;
		m_xBtn[i].rcWnd.bottom = tmpRc.bottom;
	}
}

//���ݶԻ���ĳߴ������ͼ��(������)�����ʵĴ�С
void CMvxSplitDlg::ZoomPaintRect()
{
	CRect rcClient;
	GetWindowRect(&rcClient);
	ScreenToClient(&rcClient);

	int nXScale, nYScale, nScale;
	nXScale = rcClient.Width()*20 / m_nPhyHorz;
	nYScale = (rcClient.Height()-m_rcBtnPic.Height()-10)*20 / m_nPhyVert;

	nScale = min(nXScale, nYScale);

	m_rcChart = CRect(Round((rcClient.Width() - m_nPhyHorz*nScale/20)*0.5),
                      Round((rcClient.Height()+m_rcBtnPic.Height()+10-m_nPhyVert*nScale/20)*0.5),
                      Round((rcClient.Width() + m_nPhyHorz*nScale/20)*0.5),
                      Round((rcClient.Height()+m_rcBtnPic.Height()+10+m_nPhyVert*nScale/20)*0.5));

	GetDlgItem(IDC_PIC_MVX_CHART)->MoveWindow(m_rcChart);
	//ScreenToClient(&m_rcChart);
	m_rcPaint = m_rcChart;
	m_rcChart.OffsetRect(-m_rcChart.left, -m_rcChart.top);

	m_nLogHorz = m_rcChart.Width();
	m_nLogVert = m_rcChart.Height();
}

//���ڵ�ǰ�����Ĵ�С�ı䣬��Ҫ���¼��㴰���ڻ����ϵĴ�С
void CMvxSplitDlg::ZoomWindows()
{
	int nCount = m_ArrWinInfo.GetCount();
	
	for (int i=0; i<nCount; i++)
	{
		m_ArrWinInfo[i].rcLog = m_ArrWinInfo[i].rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
	}
}


BOOL CMvxSplitDlg::OnEraseBkgnd(CDC* pDC)
{
	return CXTPResizeDialog::OnEraseBkgnd(pDC);
}

HBRUSH CMvxSplitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return (HBRUSH)m_bkBrush;
	}

	return hbr;
}

void CMvxSplitDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	m_bLBtnDown = TRUE;

	//����Ƿ����Ի水ť��
	int nID = IsMouseOverCtrls(point);
	if (nID != -1)	{
		InvalidateRect(m_rcBtnPic);
		return CXTPResizeDialog::OnLButtonDown(nFlags, point);
	}

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);
	m_ptLBtnDown = point;

	//��겻�ڻ�ͼ��
	if (!PtInRect(m_rcChart, point))
		return CXTPResizeDialog::OnLButtonDown(nFlags, point);

	//����Ƿ����ڴ�����
	m_bLBtnDownInWnd = IsMouseBtnDownInWnd(point);

	//�����δ������κ�һ�������ϣ����ж��Ƿ����¿����ڵĿ���
	if (m_bLBtnDownInWnd)
		HitTest(point, eLBtnDown);
	else
		m_nNewWndID = PrepareNewWinID();

	CXTPResizeDialog::OnLButtonDown(nFlags, point);
}

void CMvxSplitDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	//����Ƿ����Ի水ť��
	int nID = IsMouseOverCtrls(point);
	if (nID != -1)
	{
		m_xBtn[nID].bClicked = FALSE;
		InvalidateRect(m_rcBtnPic);

		//�Զ���ť��Ϣ��Ӧ����
		OnCustomBtnClicked(nID);

		//���ÿ��Ʊ���
		ResetVariables();

		return CXTPResizeDialog::OnLButtonUp(nFlags, point);
	}

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	if (m_bLBtnDownInWnd)
	{// �������ϵ���굯����Ϣ
		DefLBtnUpProc(point);

		int nIndex = GetWndIndex(m_nFocusID);
		if (nIndex != ErrIndex)
		{
		//Send Cmd
		if ((m_eResizeDir | eNoResize) == eNoResize)
			theApp.MVX_MoveTheWindow(m_nFocusID, 
                                 m_ArrWinInfo[nIndex].rcPhy, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_boardInfo.GetBoardPort());
		else
			theApp.MVX_ResizeWindow(m_nFocusID, 
                                 m_ArrWinInfo[nIndex].rcPhy, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_boardInfo.GetBoardPort());
		}
	}
	else 
	{// �¿����ڴ���
		if ( (m_nNewWndID != ErrWinID )
		  && (abs(point.x - m_ptLBtnDown.x) > 50 || abs(point.y - m_ptLBtnDown.y) > 50) )
		{
			CMvxWndPropertyDlg PropertyDlg(TRUE, this);
			if (PropertyDlg.DoModal() != IDOK)
				DeleteWindow(m_nFocusID);
		}
		else {// ��ֹ������굯���¼����µĹرմ���
			if (m_bNewCreated) DeleteWindow(m_nFocusID); 
		}
	}

	//���ÿ��Ʊ���
	ResetVariables();

	CXTPResizeDialog::OnLButtonUp(nFlags, point);
}

// �����м������
void CMvxSplitDlg::ResetVariables()
{
	m_nNewWndID = ErrWinID;
	m_bLBtnDown = FALSE;
	m_bLBtnDownInWnd = FALSE;

	m_bNewCreated = FALSE;

	m_eResizeDir = eResize_Invalid;
}

void CMvxSplitDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	//����Ƿ����Ի水ť��
	int nID = IsMouseOverCtrls(point);
	if (nID != -1)
	{
		InvalidateRect(m_rcBtnPic);
		return CXTPResizeDialog::OnMouseMove(nFlags, point);
	}

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	//���ڿ���
	if (m_bLBtnDown)
	{		
		if (m_bLBtnDownInWnd)
		{	//�ƶ����ڻ��ߵ��ڴ��ڴ�С
			OnMoveNResize(point);

			m_ptLBtnDown = point;
		}
		else
		{	//�¿�����
			if (m_nNewWndID != ErrWinID)
				DynamicCreate(point);
		}
	}
	else
		HitTest(point, eMouseMove);

	CXTPResizeDialog::OnMouseMove(nFlags, point);
}

// ��̬�����µĴ���
void CMvxSplitDlg::DynamicCreate(CPoint ptCursor)
{
	CRect rcLogical;
	rcLogical = CRect(min(ptCursor.x, m_ptLBtnDown.x),
		min(ptCursor.y, m_ptLBtnDown.y),
		max(ptCursor.x, m_ptLBtnDown.x),
		max(ptCursor.y, m_ptLBtnDown.y));

	CRect rcPhysical;
	rcPhysical = rcLogical.MulDiv(m_nPhyHorz, m_nLogHorz);

	CString strName;
	strName.Format(_T("SourceChannel[%d]"), m_boardInfo.GetBoardPort()+1);

	SetWndInfo(m_nNewWndID, m_boardInfo.GetBoardPort()+1, rcLogical, rcPhysical, strName);

	m_bNewCreated = TRUE;
}

// �������ڳߴ����λ��
void CMvxSplitDlg::OnMoveNResize(CPoint ptCursor)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	//�����������߼��ߴ��
	double HScale = double(m_nPhyHorz)/double(m_nLogHorz);
	double VScale = double(m_nPhyVert)/double(m_nLogVert);

	CRect rcLog = m_ArrWinInfo[nIndex].rcLog;
	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	int nMinSize = 20;

	// ���κβ���
	if (m_eResizeDir == eResize_Invalid) return;

	if ((m_eResizeDir & eResize_L) == eResize_L)
	{ //������߿�
		if ((ptCursor.x + nMinSize) < rcLog.right)
			rcPhy.left = Round(ptCursor.x*HScale);
		else
			rcPhy.left = rcPhy.right-Round(nMinSize*HScale);
	}
	if ((m_eResizeDir & eResize_T) == eResize_T)
	{ //�����ϱ߿�
		if ((ptCursor.y + nMinSize) < rcLog.bottom)
			rcPhy.top = Round(ptCursor.y*VScale);
		else
			rcPhy.top = rcPhy.bottom-Round(nMinSize*VScale);
	}
	if ((m_eResizeDir & eResize_R) == eResize_R)
	{ //�����ұ߿�
		if ((ptCursor.x - nMinSize) > rcLog.left)
			rcPhy.right = Round(ptCursor.x*HScale);
		else
			rcPhy.right = rcPhy.left + Round(nMinSize*HScale);
	}
	if ((m_eResizeDir & eResize_B) == eResize_B)
	{ //�����±߿�
		if ((ptCursor.y - nMinSize) > rcLog.top)
			rcPhy.bottom = Round(ptCursor.y*VScale);
		else
			rcPhy.bottom = rcPhy.top + Round(nMinSize*VScale);
	}
	if ((m_eResizeDir | eNoResize) == eNoResize)
	{ //�ƶ�����
		rcPhy.OffsetRect(Round((ptCursor.x-m_ptLBtnDown.x)*HScale), Round((ptCursor.y-m_ptLBtnDown.y)*VScale));
	}

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
}

// ������������Ϣ���� ��Ҫ�������ϵĹ��ܼ�
void CMvxSplitDlg::DefLBtnUpProc(CPoint ptCursor)
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == 0) return;

	CRect rcLog;
	for (int iZoder=MAX_WIN-1; iZoder>=0; iZoder--)
	{
		for (int i=0; i<nCount; i++)
		{
			rcLog = m_ArrWinInfo[i].rcLog;
			if ((m_ArrWinInfo[i].iZorder == iZoder) && (PtInRect(rcLog, ptCursor)))
			{
				if (PtInRect(m_ArrWinInfo[i].rcBtnExit, ptCursor))
				{ //����˳���ť
					MyCloseWindow();
				}
				else if (PtInRect(m_ArrWinInfo[i].rcBtnMax, ptCursor))
				{ //�����󻯰�ť
					MyMaximizeWindow();
				}
				else if (PtInRect(m_ArrWinInfo[i].rcBtnResize, ptCursor))
				{ //������밴ť
					MyAligningWindow();
				}
				else if (PtInRect(m_ArrWinInfo[i].rcBtnRestore, ptCursor))
				{ //�����ԭ��ť
					MyRestoreWindow();
				}

				return; //�ս�ѭ��
			}
		}
	}
}

// ����Ƿ�ͣ�����Զ��尴ť��
int CMvxSplitDlg::IsMouseOverCtrls(CPoint ptCursor)
{
	ResetBtnState();

	if (PtInRect(m_rcBtnPic, ptCursor))
	{
		CRect tmpRc;

		for (int i=0; i<MAX_BTN; i++)
		{
			tmpRc = m_xBtn[i].rcWnd;
			tmpRc.OffsetRect(m_rcBtnPic.left, m_rcBtnPic.top);

			if (PtInRect(tmpRc, ptCursor))
			{
				m_xBtn[i].bFocused = TRUE;
				m_xBtn[i].bClicked = m_bLBtnDown;

				return i;
			}
		}
	}

	// ���û�о�����ť�� �ָ���ť������״̬
	InvalidateRect(m_rcBtnPic);

	return -1;
}

void CMvxSplitDlg::ResetBtnState()
{
	for (int i=0; i<MAX_BTN; i++)
	{
		m_xBtn[i].bClicked = m_xBtn[i].bFocused = FALSE;
	}
}

// �������Ƿ�������
BOOL CMvxSplitDlg::IsMouseBtnDownInWnd(CPoint ptCursor)
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == 0) return FALSE;

	for (int iZoder=MAX_WIN-1; iZoder>=0; iZoder--) {
		for (int i=0; i<nCount; i++)   {
			if ((m_ArrWinInfo[i].iZorder == iZoder) && (PtInRect(m_ArrWinInfo[i].rcLog, ptCursor)))
			{
				m_nFocusID = m_ArrWinInfo[i].WinId;
				return TRUE;
			}
		}
	}

	return FALSE;
}

// ���׷��
void CMvxSplitDlg::HitTest(CPoint ptCursor, emMouseAct nMouseAct)
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == 0) return;
	
	CRect rcLog;
	CRect rcL, rcTL, rcT, rcTR, rcR, rcBR, rcB, rcBL;
	for (int iZoder=MAX_WIN-1; iZoder>=0; iZoder--)	{
		for (int i=0; i<nCount; i++)
		{
			rcLog = m_ArrWinInfo[i].rcLog;
			if ((m_ArrWinInfo[i].iZorder == iZoder) && PtInRect(rcLog, ptCursor))
			{				
				rcL  = CRect(rcLog.left,    rcLog.top+6,   rcLog.left+6,  rcLog.bottom-6);
				rcTL = CRect(rcLog.left,    rcLog.top,     rcLog.left+6,  rcLog.top+6);
				rcT  = CRect(rcLog.left+6,  rcLog.top,     rcLog.right-6, rcLog.top+6);
				rcTR = CRect(rcLog.right-6, rcLog.top,     rcLog.right,   rcLog.top+6);
				rcR  = CRect(rcLog.right-6, rcLog.top+6,   rcLog.right,   rcLog.bottom-6);
				rcBR = CRect(rcLog.right-6, rcLog.bottom-6,rcLog.right,   rcLog.bottom);
				rcB  = CRect(rcLog.left+6,  rcLog.bottom-6,rcLog.right-6, rcLog.bottom);
				rcBL = CRect(rcLog.left,    rcLog.bottom-6,rcLog.left+6,  rcLog.bottom);

				if (PtInRect(rcL, ptCursor)) {
					m_eResizeDir = eResize_L;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); }
				else if (PtInRect(rcTL, ptCursor)) {
					m_eResizeDir = eResize_TL;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); }
				else if (PtInRect(rcT, ptCursor)) {
					m_eResizeDir = eResize_T;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); }
				else if (PtInRect(rcTR, ptCursor)) {
					m_eResizeDir = eResize_TR;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); }
				else if (PtInRect(rcR, ptCursor)) {
					m_eResizeDir = eResize_R;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); }
				else if (PtInRect(rcBR, ptCursor)) {
					m_eResizeDir = eResize_BR;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); }
				else if (PtInRect(rcB, ptCursor)) {
					m_eResizeDir = eResize_B;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); }
				else if (PtInRect(rcBL, ptCursor)) {
					m_eResizeDir = eResize_BL;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); }
				else {
					m_eResizeDir = eNoResize;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); }

				//��������������
				if (nMouseAct != eLBtnDown) m_eResizeDir = eResize_Invalid;

				return; //�ս�ѭ��
			}
		}
	}
}

// ����������������
void CMvxSplitDlg::CalcAnchorPoint()
{
	m_ArrAnchorPt.RemoveAll();

	CPoint ptAnchor;

	for (int i=0; i<m_nHorzLines; i++)	{
		for (int j=0; j<m_nVertLines; j++)	{
			ptAnchor.x = i*m_nPhyHorz/m_nHorzLines;
			ptAnchor.y = j*m_nPhyVert/m_nVertLines;
			m_ArrAnchorPt.Add(ptAnchor);
		}
	}
}

//��ȡ�¿����ڵ�ID
int CMvxSplitDlg::PrepareNewWinID() const
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == MAX_WIN) return ErrWinID;

	bool bFound = false;

	//Ѱ��δʹ�õ�ID
	for (int i=0; i<MAX_WIN; i++) 
	{
		bFound = false;

		for (int j=0; j<nCount; j++)
		{
			if (m_ArrWinInfo[j].WinId == i)
			{
				//���д���ID����i�Ĵ���
				bFound = true;
				break;
			}
		}

		if (!bFound) return i;
	}

	return ErrWinID;
}

//��ȡ�����������е�λ��
int CMvxSplitDlg::GetWndIndex(const int nIDtoFind) const
{
	for (int i=0; i<m_ArrWinInfo.GetCount(); i++)
	{
		if (m_ArrWinInfo[i].WinId == nIDtoFind)
		{
			return i;
		}
	}

	return ErrIndex;
}

//����µĴ�����Ϣ������
void CMvxSplitDlg::AddWindow(int nNewID, int nSourceID, CRect rcLog, CRect rcPhy, CString strName)
{
	WindowInfo winfo;

	winfo.WinId     = nNewID;
	winfo.iSource   = nSourceID;
	winfo.iZorder   = m_ArrWinInfo.GetCount();
	winfo.rcLog     = rcLog;
	winfo.rcLogLast = rcLog;
	winfo.rcPhy     = rcPhy;
	winfo.rcPhyLast = rcPhy;
	strncpy_s(winfo.szTitle, strName.GetBuffer(), strName.GetLength());
	strName.ReleaseBuffer();

	m_ArrWinInfo.Add(winfo);

	m_nFocusID = nNewID;
}

//ɾ��������Ϣ
void CMvxSplitDlg::DeleteWindow(const int nWinID)
{
	int nIndex = GetWndIndex(nWinID);
	if (nIndex == ErrIndex) return;

	m_ArrWinInfo.RemoveAt(nIndex);

	if (m_ArrWinInfo.GetCount() == 0) m_nFocusID = ErrWinID;
	else
	{
		for (int i=MAX_WIN-1; i>=0; i--)	{
			for (int j=0; j<m_ArrWinInfo.GetCount(); j++)	{
				if (m_ArrWinInfo[j].WinId == i)
				{
					m_nFocusID = i;
					return;
				}
			}
		}
	}
}

//���ô�����Ϣ
void CMvxSplitDlg::SetWndInfo(int nWinID, int nSourceID, CRect rcLog, CRect rcPhy, CString strName)
{
	int nIndex = GetWndIndex(nWinID);
	if (nIndex == ErrIndex)
		AddWindow(nWinID, nSourceID, rcLog, rcPhy, strName);
	else
	{
		m_ArrWinInfo[nIndex].iSource   = nSourceID;
		m_ArrWinInfo[nIndex].rcLog     = rcLog;
		m_ArrWinInfo[nIndex].rcPhy     = rcPhy;
		m_ArrWinInfo[nIndex].rcPhyLast = rcPhy;
		strncpy_s(m_ArrWinInfo[nIndex].szTitle, strName.GetBuffer(), strName.GetLength());
		strName.ReleaseBuffer();
	}
}

//���ô��ڵ�Z˳��
void CMvxSplitDlg::SetWndZorder(int nWinID, emZorderAct eZoderAct)
{
	int nCount = m_ArrWinInfo.GetCount();
	int nIndex = GetWndIndex(nWinID);
	if (nIndex == ErrIndex) return;
	switch(eZoderAct)
	{
	case eStepFront:
		{//����Ļ���ƶ�
			if (m_ArrWinInfo[nIndex].iZorder == nCount-1) return;//�����������
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == m_ArrWinInfo[nIndex].iZorder+1)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	case eStepBack:
		{//����Ļ���ƶ�
			if (m_ArrWinInfo[nIndex].iZorder == 0) return;//����������ײ�
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == m_ArrWinInfo[nIndex].iZorder-1)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	case eStepTop:
		{//�ö���ʾ
			if (m_ArrWinInfo[nIndex].iZorder == nCount-1) return;//�����������
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == nCount-1)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	case eStepBottom:
		{//�õ���ʾ
			if (m_ArrWinInfo[nIndex].iZorder == 0) return;//����������ײ�
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == 0)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	default:break;
	}
}

//�����������ڵĵ���˳��
inline void CMvxSplitDlg::SwapWndZorder(WindowInfo &WinInfoA, WindowInfo &WinInfoB)
{
	int iTmpZorder;
	iTmpZorder = WinInfoA.iZorder;
	WinInfoA.iZorder = WinInfoB.iZorder;
	WinInfoB.iZorder = iTmpZorder;
}

// �Զ��尴ť����¼�����
void CMvxSplitDlg::OnCustomBtnClicked(UINT nID)
{
	if (nID == 0)
		OnBkgndProperty();
	else if (nID == 1)
		OnVedioProperty();
	else if (nID == 2)
		OnWndProperty();
	else if (nID == 3)
		OnSceneManage();
	else if (nID == 4)
		OnShotCut();

}

void CMvxSplitDlg::OnBkgndProperty()
{
	//�����������ԶԻ������ñ�������
	CMvxBackgrdSettingDlg BackgrdSetting(this);
	BackgrdSetting.m_nHLogScreen = m_nHorzLines;
	BackgrdSetting.m_nVLogScreen = m_nVertLines;
	BackgrdSetting.DoModal();
}

void CMvxSplitDlg::OnVedioProperty()
{
	//������Ƶ���ԶԻ�����Ҫ�������
	CMvxVideoPropertyDlg VideoProperty(this);
	VideoProperty.DoModal();
}

void CMvxSplitDlg::OnWndProperty()
{
	//����������Ϣ�Ի���
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CMvxWndPropertyDlg PropertyDlg(FALSE, this);

	PropertyDlg.DoModal();
}

void CMvxSplitDlg::OnSceneManage()
{
	//������������˵�
	CPoint point;
	GetCursorPos(&point);

	theApp.MvTrackPopupMenu(point, IDR_MENU_MVX_SCENE_MANAGE, this);
}

void CMvxSplitDlg::OnShotCut()
{
	//������ݲ˵�
	CPoint point;
	GetCursorPos(&point);

	theApp.MvTrackPopupMenu(point, IDR_MENU_MVX_WND_OPTION, this);
}

void CMvxSplitDlg::MyCloseWindow()
{
	//DeleteWindow ������ı�m_nFocusID��ֵ
	theApp.MVX_ShutdownWindow(m_nFocusID, m_boardInfo.GetBoardPort());

	//���ô��ڵ�Zorder˳���ö������ڵ���˳�����
	SetWndZorder(m_nFocusID, eStepTop);

	//ɾ���ô����������е�ֵ
	DeleteWindow(m_nFocusID);
}

void CMvxSplitDlg::MyMaximizeWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	rcPhy.left   = 0;
	rcPhy.top    = 0;
	rcPhy.right  = m_nPhyHorz;
	rcPhy.bottom = m_nPhyVert;

	//�µĳߴ����ϴ����õĴ�Сһ��
	if (rcPhy == m_ArrWinInfo[nIndex].rcPhyLast) return;

	m_ArrWinInfo[nIndex].rcPhyLast = m_ArrWinInfo[nIndex].rcPhy;

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	//Send Cmd
	theApp.MVX_ResizeWindow(m_nFocusID, 
                        rcPhy,
                        m_ArrWinInfo[nIndex].iSource,
                        m_ArrWinInfo[nIndex].iSource,
                        m_boardInfo.GetBoardPort());
}

void CMvxSplitDlg::MyAligningWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	int nLeft   = 0;
	int nTop    = 0;
	int nRight  = m_nPhyHorz;
	int nBottom = m_nPhyVert;

	for (int i=0; i<m_ArrAnchorPt.GetCount(); i++)
	{
		// �������
		if ((m_ArrAnchorPt[i].x <= rcPhy.left) && (m_ArrAnchorPt[i].x >= nLeft))
			nLeft = m_ArrAnchorPt[i].x;
		// �϶�����
		if ((m_ArrAnchorPt[i].y <= rcPhy.top) && (m_ArrAnchorPt[i].y >= nTop))
			nTop = m_ArrAnchorPt[i].y;
		// �Ҷ�����
		if ((m_ArrAnchorPt[i].x >= rcPhy.right) && (m_ArrAnchorPt[i].x <= nRight))
			nRight = m_ArrAnchorPt[i].x;
		// �¶�����
		if ((m_ArrAnchorPt[i].y >= rcPhy.bottom) && (m_ArrAnchorPt[i].y <= nBottom))
			nBottom = m_ArrAnchorPt[i].y;
	}

	rcPhy = CRect(nLeft, nTop, nRight, nBottom);

	//�µĳߴ����ϴ����õĴ�Сһ��
	if (m_ArrWinInfo[nIndex].rcPhyLast == rcPhy) return;

	m_ArrWinInfo[nIndex].rcPhyLast = m_ArrWinInfo[nIndex].rcPhy;	
	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	//Send Cmd
	theApp.MVX_ResizeWindow(m_nFocusID, 
                        rcPhy,
                        m_ArrWinInfo[nIndex].iSource,
                        m_ArrWinInfo[nIndex].iSource,
                        m_boardInfo.GetBoardPort());
}

void CMvxSplitDlg::MyRestoreWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhyLast;

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	//Send Cmd
	theApp.MVX_ResizeWindow(m_nFocusID, 
                        rcPhy,
                        m_ArrWinInfo[nIndex].iSource,
                        m_ArrWinInfo[nIndex].iSource,
                        m_boardInfo.GetBoardPort());
}

void CMvxSplitDlg::Initialize()
{
	UINT Params[128] = {0};
	int Port = m_boardInfo.GetBoardPort();
	//��ȡ�����ź�
	if (theApp.MVX_ReadBackgrdInfo(Params, Port) == CMD_FAILURE)
		return (void)MessageBox(_T("��ʼ��ʧ�ܣ�δ�ܶ�ȡ�����źţ�"), _T("MVX303��ʼ��"), MB_OK|MB_ICONSTOP);

	if (Params[0] != 0x01)
		return (void)MessageBox(_T("��ʼ��ʧ�ܣ��������źţ���ȷ������˿��Ƿ�����źţ�"), _T("MVX303��ʼ��"), MB_OK|MB_ICONWARNING);

	//�����źŵķֱ�����Ϣ
	m_BkgTiming.m_HTotal  = Params[1];
	m_BkgTiming.m_HActive = Params[2];
	m_BkgTiming.m_HFPorch = Params[3];
	m_BkgTiming.m_HSyncWid= Params[4];
	m_BkgTiming.m_HPol    = Params[5];
	m_BkgTiming.m_HBlank  = m_BkgTiming.m_HTotal - m_BkgTiming.m_HActive;

	m_BkgTiming.m_VTotal  = Params[6];
	m_BkgTiming.m_VActive = Params[7];
	m_BkgTiming.m_VFPorch = Params[8];
	m_BkgTiming.m_VSyncWid= Params[9];
	m_BkgTiming.m_VPol    = Params[10];
	m_BkgTiming.m_VBlank  = m_BkgTiming.m_VTotal - m_BkgTiming.m_VActive;

	m_BkgTiming.m_Hz      = Params[11];

	m_nPhyHorz            = m_BkgTiming.m_HActive;
	m_nPhyVert            = m_BkgTiming.m_VActive;

	//�߼�����Ļ���к���
	m_nHorzLines          = Params[16];
	m_nVertLines          = Params[17];

	//���¼��㿪������С
	CalcAnchorPoint();
	ZoomPaintRect();

	//��ȡ������Ϣ
	InitScenes();
}

void CMvxSplitDlg::InitScenes()
{
	//��ȡ��ǰ����
	LoadDevScene(0xff);

	//�������ڵ��߼���С
	ZoomWindows();

	//�ػ�
	InvalidateRect(m_rcChart);
}

void CMvxSplitDlg::LoadDevScene(int nSceneID)
{
	//��ȡ��������
	UINT Params[256] = {0};
	int Port = m_boardInfo.GetBoardPort();

	if (theApp.MVX_CallWindows(Params, nSceneID, Port) == CMD_FAILURE)
		return (void)MessageBox(_T("���س���ʧ�ܣ�δ�ܶ�ȡ�������ݣ�"), _T("��������"), MB_OK|MB_ICONSTOP);

	//��������Ϣ����
	m_ArrWinInfo.RemoveAll();

	int nNextPos = 0;    //��һ��������Ϣ����ʼλ��

	//������Ϣ
	WindowInfo WinInfo;
	memset(&WinInfo, 0, sizeof(WindowInfo));

	int nSrcID  = Port;  //�ź�Դ
	int nWinID;          //����ID  
	CRect rcPhy;         //��������ߴ�
	CRect rcLog;         //�����ڻ�ͼ���ĳߴ�
	CString strName;
	strName.Format(_T("SourceChannel[%d]"), nSrcID+1);

	//���³���������
	while(Params[nNextPos] != 0) //������Ч�Ա�־λ 0x01��ʾ��Ч 0xff��ʾ��Ч 0x00��ʾ����
	{
		if (Params[nNextPos] == 0xff) { nNextPos += 9; continue; }

		nWinID = Params[nNextPos+1];
		if (nWinID >= MAX_WIN)	{ nNextPos += 9; continue; }

		rcPhy.left   = Params[nNextPos+5];
		rcPhy.top    = Params[nNextPos+6];
		rcPhy.right  = Params[nNextPos+7];
		rcPhy.bottom = Params[nNextPos+8];

		rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

		SetWndInfo(nWinID, nSrcID, rcLog, rcPhy, strName);

		nNextPos += 9;
	}
}

void CMvxSplitDlg::WriteDevScene(int nSceneID)
{
	UINT nParam[256];
	memset(nParam, 0, sizeof(nParam));

	int nTick = 0;
	nParam[nTick++] = nSceneID;  // num ������ Ĭ�ϳ���һ ��Ӧ�ĳ�����Ϊ0

	int nCount = m_ArrWinInfo.GetCount();
	for (int i=0; i< nCount; i++)
	{
		nParam[nTick++] = 1;                           //win_valid ������Ч��
		nParam[nTick++] = m_ArrWinInfo[i].WinId;       //win_ID
		nParam[nTick++] = m_ArrWinInfo[i].iSource;     //win_sourceID
		nParam[nTick++] = m_ArrWinInfo[i].iZorder;     //win_lvl
		nParam[nTick++] = 0;                           //win_angle

		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.left;  //x0 ͼ������Ͻ�
		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.top;   //y0 ͼ������Ͻ�

		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.right;  //x1 ͼ������½�
		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.bottom; //y1 ͼ������½�
	}

	int nRet = theApp.MVX_SaveWindows(nParam, nTick, nSceneID, m_boardInfo.GetBoardPort());
	if (nRet == -1)
		MessageBox(_T("�Բ��𣬱��泡��ʧ�ܣ�"), _T("���泡��"), MB_OK|MB_ICONERROR);		
	else
		MessageBox(_T("���泡���ɹ���"), _T("���泡��"), MB_OK|MB_ICONINFORMATION);
}

void CMvxSplitDlg::OnSaveSceneFile()
{
	//ѡ��洢�ļ�·��
	CFileDialog FileDlg(FALSE, NULL, "Screen1.srf", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Screen Files (*.srf)|*.srf|All Files (*.*)|*.*||");
	if (FileDlg.DoModal() == IDOK)
	{
		//��ȡ·��
		CString strFilPath = FileDlg.GetPathName();

		//���������ļ���д������
		CStdioFile File;
		if (!File.Open(strFilPath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			return (void)MessageBox(_T("�����ļ�ʧ�ܣ�"),_T("��������"), MB_OK|MB_ICONSTOP);

		int nCount = m_ArrWinInfo.GetCount();

		for (int i=0; i<nCount; i++)
		{
			File.Write(&m_ArrWinInfo[i], sizeof(WindowInfo));
		}

		File.Close();

		MessageBox(_T("���泡���ļ��ɹ���"), _T("��������"), MB_OK|MB_ICONINFORMATION);
	}
}

void CMvxSplitDlg::OnReadSceneFile()
{
	//�򿪳��������ļ�
	CFileDialog FileDlg(TRUE, _T("srf"), "Screen1", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Screen Files (*.srf)|*.srf|All Files (*.*)|*.*||");
	if (FileDlg.DoModal() == IDOK)
	{
		//��ȡ�ļ�·��
		CString strFilePath = FileDlg.GetPathName();

		CStdioFile File;
		if (!File.Open(strFilePath, CFile::modeRead))
			return (void)MessageBox(_T("���ļ�ʧ�ܣ�"),_T("��������"), MB_OK|MB_ICONSTOP);

		//�������д�����Ϣ
		m_ArrWinInfo.RemoveAll();
		WindowInfo WinInfo;
		memset(&WinInfo, 0, sizeof(WindowInfo));

		//��ȡ�ļ�����
		char buff[512] = {0};
		while(File.Read(buff, sizeof(WindowInfo)))
		{
			memcpy(&WinInfo, buff, sizeof(WindowInfo));
			m_ArrWinInfo.Add(WinInfo);
			memset(&WinInfo, 0, sizeof(WindowInfo));
		}
		File.Close();

		//���ݵ�ǰ������С�������ڵ��߼��ߴ�
		ZoomWindows();

		//�ػ�
		InvalidateRect(m_rcChart);

		MessageBox(_T("��ȡ�����ɹ���"), _T("��������"), MB_OK|MB_ICONINFORMATION);
	}
}

void CMvxSplitDlg::OnWriteDevScene(UINT nID)
{
	int nSceneID = nID-ID_WRITE_DEV_SCENE1;

	WriteDevScene(nSceneID);
}

void CMvxSplitDlg::OnLoadDevScene(UINT nID)
{
	int nSceneID = nID-ID_LOAD_DEV_SCENE1;

	//��ȡ��������
	LoadDevScene(nSceneID);

	//���ݵ�ǰ��ͼ���Ĵ�С���µ��������ڵ��߼��ߴ�
	ZoomWindows();

	//�ػ�
	InvalidateRect(m_rcChart);
}

void CMvxSplitDlg::OnWindowOption(UINT nID)
{
	switch(nID)
	{
	case ID_MVX_OPEN_WINDOW:
		//�½��˵�
		OnNewWindow();
		break;
	case ID_MVX_CLOSE_WINDOW:
		//�رղ˵�
		OnCloseWindow();
		break;
	case ID_ZORDER_TOPMOST:
	case ID_ZORDER_BOTTOMMOST:
	case ID_ZORDER_MOVEFRONT:
	case ID_ZORDER_MOVEBACK:
		//����Zorder
		{
			emZorderAct ZorderAct;
			ZorderAct = emZorderAct(nID-ID_ZORDER_TOPMOST+1);

			SetWndZorder(m_nFocusID, ZorderAct);
		}
		break;
	case ID_MVX_SAME_WID:
		//�ȿ�˵�
		OnSameSize(0x01);
		break;
	case ID_MVX_SAME_HGT:
		//�ȸ߲˵�
		OnSameSize(0x10);
		break;
	case ID_MVX_SAME_SIZE:
		//�ȴ�С�˵�
		OnSameSize(0x11);
		break;
	default:break;
	}
}


void CMvxSplitDlg::OnNewWindow()
{
	//���벻���µĺϷ�ID
	int nNewID = PrepareNewWinID();
	if (nNewID == ErrWinID) return;

	CString strName;
	CRect rcPhy, rcLog;
	int nSource = m_boardInfo.GetBoardPort();
	strName.Format(_T("SourceChannel[%d]"), nSource+1);

	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex)
	{// ��ǰû�д��� ʹ��Ĭ�ϳߴ��½�����
		//���Ϊ������ȵ�1/3���߶�Ϊ�����߶ȵ�1/2
		rcPhy.left   = m_nPhyHorz/3;
		rcPhy.top    = m_nPhyVert/4;
		rcPhy.right  = m_nPhyHorz/3*2;
		rcPhy.bottom = m_nPhyVert/4*3;
	}
	else
	{// ʹ�ý��㴰�ڵĳߴ��½�����
		rcPhy = m_ArrWinInfo[nIndex].rcPhy;
		rcPhy.OffsetRect(m_nPhyHorz/2 - rcPhy.CenterPoint().x, m_nPhyVert/2 - rcPhy.CenterPoint().y);
	}

	rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	SetWndInfo(nNewID, nSource, rcLog, rcPhy, strName);

	InvalidateRect(m_rcChart);

	//ȷ���½�����
	CMvxWndPropertyDlg dlg(TRUE, this);
	if (dlg.DoModal() != IDOK)
		DeleteWindow(nNewID);
}

void CMvxSplitDlg::OnCloseWindow()
{
	MyCloseWindow();
}

//ʹ���д��ڵĳߴ��뵱ǰ���ڵ�һ��
void CMvxSplitDlg::OnSameSize(int ntype)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;
	CRect rcTmp;

	int nCount = m_ArrWinInfo.GetCount();
	for (int i=0; i<nCount; i++)
	{
		rcTmp = m_ArrWinInfo[i].rcPhy;

		//�����ͬ
		if (ntype & 0x01) rcTmp.right = rcTmp.left + rcPhy.Width();
		//�߶���ͬ
		if (ntype & 0x10) rcTmp.bottom = rcTmp.top + rcPhy.Height();

		m_ArrWinInfo[i].rcPhy = rcTmp;
		m_ArrWinInfo[i].rcPhyLast = rcTmp;
		m_ArrWinInfo[i].rcLog = rcTmp.MulDiv(m_nLogHorz, m_nPhyHorz);
		m_ArrWinInfo[i].rcLogLast = m_ArrWinInfo[i].rcLog;

		theApp.MVX_ResizeWindow(m_ArrWinInfo[i].WinId,
                            rcTmp,
                            m_ArrWinInfo[i].iSource,
                            m_ArrWinInfo[i].iSource,
                            m_boardInfo.GetBoardPort());
	}

	//�ػ�
	InvalidateRect(m_rcChart);
}

void CMvxSplitDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	//�Ҽ����ڵ����˵�
	if (IsMouseBtnDownInWnd(point))
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);

		theApp.MvTrackPopupMenu(ptCursor, IDR_MENU_MVX_WND_OPTION, this);
	}

	CXTPResizeDialog::OnRButtonUp(nFlags, point);
}

BOOL CMvxSplitDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam >= VK_LEFT) && (pMsg->wParam <= VK_DOWN))
		{
			//LShift + Arrow Key �Ŵ󴰿�
			if (::GetAsyncKeyState(VK_LSHIFT) & 0x8000)
				OnVKresizeWindow(pMsg->wParam, TRUE);
			//RShift + Arrow Key ��С����
			else if (::GetAsyncKeyState(VK_RSHIFT) & 0x8000)
				OnVKresizeWindow(pMsg->wParam, FALSE);
			//������ƶ�����
			else
				OnVKmoveWindow(pMsg->wParam);

			return TRUE;
		}
		else if (pMsg->wParam == VK_TAB)
		{
			//��ݼ��л�����
			if (::GetAsyncKeyState(VK_CONTROL) & 0x8000)
				OnTabChangedWindow();
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

//������ƶ�����
void CMvxSplitDlg::OnVKmoveWindow(UINT nVKcode)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	rcPhy.OffsetRect((nVKcode==VK_LEFT) ? -1 : ((nVKcode==VK_RIGHT) ? 1 : 0),
                     (nVKcode==VK_UP) ? -1 : ((nVKcode==VK_DOWN) ? 1 : 0));

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcPhyLast = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
	m_ArrWinInfo[nIndex].rcLogLast = m_ArrWinInfo[nIndex].rcLog;

	theApp.MVX_MoveTheWindow(m_nFocusID, 
                         rcPhy,
                         m_ArrWinInfo[nIndex].iSource,
                         m_ArrWinInfo[nIndex].iSource,
                         m_boardInfo.GetBoardPort());

	InvalidateRect(m_rcChart);
}

//��ݼ��������ڴ�С
void CMvxSplitDlg::OnVKresizeWindow(UINT nVKcode, BOOL bInflate)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	if (bInflate)  //�Ŵ�
		rcPhy.InflateRect(nVKcode==VK_LEFT ? 1 : 0,
                          nVKcode == VK_UP ? 1 : 0,
                          nVKcode == VK_RIGHT ? 1 : 0,
                          nVKcode == VK_DOWN ? 1 : 0);
	else  //��С
		rcPhy.InflateRect(nVKcode==VK_LEFT ? -1 : 0,
                          nVKcode == VK_UP ? -1 : 0,
                          nVKcode == VK_RIGHT ? -1 : 0,
                          nVKcode == VK_DOWN ? -1 : 0);

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcPhyLast = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
	m_ArrWinInfo[nIndex].rcLogLast = m_ArrWinInfo[nIndex].rcLog;

	theApp.MVX_ResizeWindow(m_nFocusID, 
                         rcPhy,
                         m_ArrWinInfo[nIndex].iSource,
                         m_ArrWinInfo[nIndex].iSource,
                         m_boardInfo.GetBoardPort());

	InvalidateRect(m_rcChart);
}

//Ctrl + Tab �л�����
void CMvxSplitDlg::OnTabChangedWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	int nTabedID = (m_nFocusID+1)%MAX_WIN;

	if (GetWndIndex(nTabedID) == ErrIndex)
		return;

	m_nFocusID = nTabedID;

	InvalidateRect(m_rcChart);
}

//��������
BOOL CMvxSplitDlg::SaveAll()
{
	WriteDevScene(0);

	Sleep(50);

	if (theApp.SaveAll(m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("���󣡹̻�����ʧ�ܣ�"), _T("�̻�����"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}