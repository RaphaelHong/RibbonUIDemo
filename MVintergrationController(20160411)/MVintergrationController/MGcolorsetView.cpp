// colorsetView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGeoMainDlg.h"
#include "MGcolorsetDlg.h"
#include "MGcolorsetView.h"

#define BIG_CTRL_NODE  9
#define TINY_CTRL_NODE 32

//g_cstBigCtrlNode[9] ��¼�ֵ���׼�̶�ֵ�Լ��ֵ����Ƶ��λ��
const int g_cstBigCtrlNode[BIG_CTRL_NODE] = { 
	0, 31, 63, 95, 127, 159, 191, 223, 255
};

//g_cstTinyCtrlNode[32] ��¼΢����׼�̶�ֵ�Լ�΢�����Ƶ��λ��
const int g_cstTinyCtrlNode[TINY_CTRL_NODE] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

// CcolorsetView

IMPLEMENT_DYNCREATE(CcolorsetView, CView)

CcolorsetView::CcolorsetView()
{
	InitMembers();
}

CcolorsetView::~CcolorsetView()
{
	if (m_pMsgTip)
	{
		m_pMsgTip->DestroyWindow();
		delete m_pMsgTip;
		m_pMsgTip = NULL;
	}
}

void CcolorsetView::InitMembers()
{
	m_pMsgTip = NULL;

	m_AjustType     = eRoughType;
	m_AjustMethod   = eLinearMode;
	m_eColor        = eRed;
	m_nColorSection = 5;
	m_nCtrlPointIndex = 0;

	m_bLBtnDownAtCtrlPoint = FALSE;
	m_nTestType = 0;
	
	for (int i=0; i<4; i++) m_ColorArray[i].ResetColor();
}

BEGIN_MESSAGE_MAP(CcolorsetView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CcolorsetView ��ͼ

void CcolorsetView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
	//��ȡ��ͼ����С
	CRect rcClient;
	GetClientRect(rcClient);

	//��������DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	//������λͼ
	CBitmap *pOldbmp = dcMem.SelectObject(&MemBitmap); 
	//��ָ����ɫ�������
	dcMem.FillSolidRect(0, 0, rcClient.right, rcClient.bottom, RGB(250,250,250)); 

	DrawChart(&dcMem);

	DrawTitle(&dcMem);

	DrawCurves(&dcMem);

	//pDC->TransparentBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, rcClient.Width(), rcClient.Height(), RGB(250,250,250));

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldbmp);
	ReleaseDC(&dcMem);
}

void CcolorsetView::DrawChart(CDC *pValidDC)
{
	if (m_AjustType == eRoughType)
		DrawRoughChart(pValidDC);
	else if (m_AjustType == ePreciseType)
		DrawPreciseChart(pValidDC);
}

void CcolorsetView::DrawRoughChart(CDC *pValidDC)
{
	CPen penBk,*pOldpen;
	penBk.CreatePen(PS_SOLID, 1, RGB(0x1a,0x1a,0x1a));
	pOldpen = pValidDC->SelectObject(&penBk);

	// ��ͼ�������������
	pValidDC->MoveTo(CPoint(m_rcChart.left, m_rcChart.top));
	pValidDC->LineTo(CPoint(m_rcChart.left, m_rcChart.bottom+5));
	pValidDC->MoveTo(CPoint(m_rcChart.left-5, m_rcChart.bottom));
	pValidDC->LineTo(CPoint(m_rcChart.right, m_rcChart.bottom));

	// �̶� ��0,31,63,95...��
	CString strMark;
	CFont markFont,*pOldFont;
	VERIFY(markFont.CreateFont(
		10,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename
	pOldFont = pValidDC->SelectObject(&markFont);
	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(0,0,0));
	CPen  gridpen;
	CRect gridrect;
	gridpen.CreatePen(PS_SOLID, 1, RGB(220,220,197));

	// ��������̶ȣ����ĸ��ӻ��ϲ�ͬ�ĵ�ɫ
	gridrect = CRect(m_rcChart.left+1, m_rcChart.top, m_rcChart.left+10, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
	//CPoint ptHorMark[BIG_CTRL_NODE], ptVerMark[BIG_CTRL_NODE];
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		// �̶��ַ�
		strMark.Format(_T("%d"), g_cstBigCtrlNode[i]);

		// �����̶�
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->TextOut(m_rcChart.left-25, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale)-3, strMark);
		//ptVerMark[i] = CPoint(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));

		// �����̶�
		pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom+8);
		pValidDC->TextOut(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale)-3, m_rcChart.bottom+10, strMark);
		//ptHorMark[i] = CPoint(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom+8);

		if(i < BIG_CTRL_NODE-1)
		{
			// �����ɫ��
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.top, m_rcChart.left+10+Round(g_cstBigCtrlNode[i+1]*m_Xscale), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.top, m_rcChart.left+10+Round(g_cstBigCtrlNode[i+1]*m_Xscale), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}

			// ����̶ȼ��С�̶�
			for (int j=1; j<5; j++)
			{
				// ����С�̶�
				pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
				pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
				// ����С�̶�
				pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale)+Round(32*m_Xscale*j/5), m_rcChart.bottom);
				pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale)+Round(32*m_Xscale*j/5), m_rcChart.bottom+5);
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[8]*m_Xscale), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));


	// �������еĵĸ���
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<BIG_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		}
		pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.top);
	}

	// �������������
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+10, m_rcChart.bottom-3, _T("��׼ֵ"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("У׼ֵ"));
	pValidDC->SelectObject(pOldFont);

}

void CcolorsetView::DrawPreciseChart(CDC *pValidDC)
{
	CPen penBk,*pOldpen;
	penBk.CreatePen(PS_SOLID, 1, RGB(0x1a,0x1a,0x1a));
	pOldpen = pValidDC->SelectObject(&penBk);

	// ��ͼ�������������
	pValidDC->MoveTo(CPoint(m_rcChart.left,   m_rcChart.top));
	pValidDC->LineTo(CPoint(m_rcChart.left,   m_rcChart.bottom+5));
	pValidDC->MoveTo(CPoint(m_rcChart.left-5, m_rcChart.bottom));
	pValidDC->LineTo(CPoint(m_rcChart.right,  m_rcChart.bottom));

	// �̶� (0,31,63,95...)
	CFont markFont,*pOldFont;
	VERIFY(markFont.CreateFont(
		10,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename
	pOldFont = pValidDC->SelectObject(&markFont);
	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(0,0,0));
	CPen  gridpen;
	CRect gridrect;
	gridpen.CreatePen(PS_SOLID, 1, RGB(220,220,197));

	// ��������̶ȣ����ĸ��ӻ��ϲ�ͬ�ĵ�ɫ
	gridrect = CRect(m_rcChart.left+1, m_rcChart.top, m_rcChart.left+10, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
	int nStdSectionClr[32];
	m_ColorArray[m_eColor].GetStdSectionColor(nStdSectionClr, m_nColorSection);


	CString strHorMark, strVerMark;
	int nVerMark = 0;
	int nMarkOffset = 0; //���̶��ַ���׼���̶�

	//������
	for(int i=0; i<TINY_CTRL_NODE; i++)
	{
		// �̶��ַ�
		strHorMark.Format(_T("%d"), nStdSectionClr[i]);

		if(nStdSectionClr[i] >=0 && nStdSectionClr[i]<10) nMarkOffset = 2;
		else if(nStdSectionClr[i]>=10 && nStdSectionClr[i]<100) nMarkOffset = 4;
		else nMarkOffset = 5;

		// ����̶�
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.bottom+8);
		if (i%2 == 0) pValidDC->TextOut(m_rcChart.left+10+Round(i*m_Xscale*8)-nMarkOffset, m_rcChart.bottom+9, strHorMark);
		else pValidDC->TextOut(m_rcChart.left+10+Round(i*m_Xscale*8)-nMarkOffset, m_rcChart.bottom+14, strHorMark);

		if(i < TINY_CTRL_NODE-1)
		{
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*m_Xscale*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*m_Xscale*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(31*m_Xscale*8), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));

	//������
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		// �̶��ַ�
		strVerMark.Format(_T("%d"), g_cstBigCtrlNode[i]);

		// �����̶�
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->TextOut(m_rcChart.left-25, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale)-3, strVerMark);

		if(i < BIG_CTRL_NODE-1)
		{
			// ����̶ȼ��С�̶�
			for (int j=1; j<5; j++)
			{
				// ����С�̶�
				pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
				pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
			}
		}
	}

	// �������еĵĸ���
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.top);
	}

	for (int i=0; i<BIG_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		}
	}

	// �������������
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+2, m_rcChart.bottom-3, _T("��׼ֵ"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("У׼ֵ"));
	pValidDC->SelectObject(pOldFont);
}

void CcolorsetView::DrawTitle(CDC *pValidDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CFont titleFont, *pOldFont;
	VERIFY(titleFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		TRUE,                      // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����")));              // lpszFacename
	pOldFont = pValidDC->SelectObject(&titleFont);
	CSize textSize;
	CString strTitle;
	COLORREF colorTitle;
	strTitle   = m_eColor==eWhite ? _T("��ɫУ׼��") : m_eColor==eRed ? _T("��ɫУ׼��") : m_eColor == eGreen ? _T("��ɫУ׼��") : _T("��ɫУ׼��") ;
	colorTitle = m_eColor==eWhite ? RGB(100, 50, 80) : m_eColor==eRed ? RGB(255, 40, 40) : m_eColor == eGreen ? RGB(10, 200, 40) : RGB(100, 100, 255) ;
	textSize = pValidDC->GetTextExtent(_T("��ɫУ׼��"));
	pValidDC->SetTextColor(colorTitle);
	pValidDC->TextOut(int((rcClient.Width()-textSize.cx)*0.5), rcClient.top+2, strTitle);

	pValidDC->SelectObject(pOldFont);
}

void CcolorsetView::DrawCurves(CDC *pValidDC)
{
	if (m_AjustType == eRoughType)
		DrawRoughCurve(pValidDC);
	else if (m_AjustType == ePreciseType)
		DrawPreciseCurve(pValidDC);
}

void CcolorsetView::DrawRoughCurve(CDC *pValidDC)
{
	CBrush br,brCtrl/*��ǰ���Ƶ����ɫ*/,*pOldBr;
	CPen pen,*pOldpen;
	COLORREF colorCoodr;
	colorCoodr = m_eColor==eWhite ? RGB(200, 200, 200) : m_eColor==eRed ? RGB(255,50,50) : m_eColor == eGreen ? RGB(50,255,50) : RGB(50,50,255) ;
	br.CreateSolidBrush(colorCoodr);
	brCtrl.CreateSolidBrush(RGB(40,40,30));
	pen.CreatePen(PS_SOLID, 1, colorCoodr);
	pOldBr  = pValidDC->SelectObject(&br);
	pOldpen = pValidDC->SelectObject(&pen);

	int nDotColor = 0;
	int nNextDotColor = 0;

	//����ɫ��
	for (int i=0; i<255; i++)
	{
		nDotColor     = m_ColorArray[m_eColor].DotColor(i);
		nNextDotColor = m_ColorArray[m_eColor].DotColor(i+1);
		pValidDC->MoveTo(m_rcChart.left + Round(i*m_Xscale)     + 10, m_rcChart.bottom - Round(nDotColor*m_Yscale));
		pValidDC->LineTo(m_rcChart.left + Round((i+1)*m_Xscale) + 10, m_rcChart.bottom - Round(nNextDotColor*m_Yscale));
	}

	//����Ƶ�
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		nDotColor = m_ColorArray[m_eColor].DotColor(i*32-1);

		//���Է�ʽֻ����β�������Ƶ�
		if(m_AjustMethod == eLinearMode)
		{
			if (i > 0 && i < BIG_CTRL_NODE-1)
			{
				continue;
			}
		}

		if (i == m_nCtrlPointIndex)
		{
			pValidDC->SelectObject(&brCtrl);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) - 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) - 7,
				m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) + 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) + 7));
		}
		else
		{
			pValidDC->SelectObject(&br);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) - 5 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) - 5,
				m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) + 5 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) + 5));
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

void CcolorsetView::DrawPreciseCurve(CDC *pValidDC)
{
	CBrush br, brCtrl/*���Ƶ����ɫ*/, *pOldBr;
	CPen pen, *pOldpen;
	COLORREF colorCoodr;
	colorCoodr = m_eColor==eWhite ? RGB(200, 200, 200) : m_eColor==eRed ? RGB(255,50,50) : m_eColor == eGreen ? RGB(50,255,50) : RGB(50,50,255);
	br.CreateSolidBrush(colorCoodr);
	brCtrl.CreateSolidBrush(RGB(40, 40, 30));
	pen.CreatePen(PS_SOLID, 1, colorCoodr);
	pOldBr = pValidDC->SelectObject(&br);
	pOldpen = pValidDC->SelectObject(&pen);

	int nDotColor = 0;
	int nNextDotColor = 0;
	//int nColorSel = m_eColor == WHITE ? 0 : m_eColor;
	//����ɫ��
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		nDotColor     = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i);
		nNextDotColor = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i+1);

		//�����������Ƶ�
		if (i<TINY_CTRL_NODE-1)
		{
			pValidDC->MoveTo(m_rcChart.left + Round(i*m_Xscale*8)     + 10, m_rcChart.bottom - Round(nDotColor*m_Yscale));
			pValidDC->LineTo(m_rcChart.left + Round((i+1)*m_Xscale*8) + 10, m_rcChart.bottom - Round(nNextDotColor*m_Yscale));
		}

		//���ƿ��Ƶ�
		if(i == m_nCtrlPointIndex) //��ǰ���Ƶ�
		{
			pValidDC->SelectObject(&brCtrl);
			pValidDC->Ellipse(CRect(m_rcChart.left + Round(i*m_Xscale*8) + 10 -7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-7,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+7));
		}
		else
		{
			pValidDC->SelectObject(&br);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*m_Xscale*8) + 10 -5,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-5,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +5,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+5));
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

// CcolorsetView ���

#ifdef _DEBUG
void CcolorsetView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CcolorsetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CcolorsetView ��Ϣ�������

BOOL CcolorsetView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CcolorsetView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);

	CalcChartPos(rcClient);
}

void CcolorsetView::CalcChartPos(CRect parentRc)
{
	int nChartWidth;
	nChartWidth = (parentRc.Width() >= parentRc.Height()) ? parentRc.Height() : parentRc.Width();
	nChartWidth -= 60;  //�������Χ�ڳ�50���صĿ���ռ�

	m_rcChart.left   = 30;  //long((parentRc.Width()-nChartWidth)*0.5);
	m_rcChart.right  = parentRc.Width()-35; //m_rcChart.left+nChartWidth;
	m_rcChart.top    = 30; //long((parentRc.Height()-nChartWidth)*0.5);
	m_rcChart.bottom = parentRc.Height()-30; //m_rcChart.top+nChartWidth;

	m_Xscale = double(m_rcChart.Width()-25) / 255.0;
	m_Yscale = double(m_rcChart.Height()-15) / 255.0;
}

void CcolorsetView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	m_bLBtnDownAtCtrlPoint = IsLBtnDownAtColorCtrlPoint(point);
	
	if (m_bLBtnDownAtCtrlPoint)
	{
		TestColor();
	}

	CView::OnLButtonDown(nFlags, point);
}

void CcolorsetView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_bLBtnDownAtCtrlPoint = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

void CcolorsetView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// ���ڱ�������ڲ���Ӧ
	if (PtInRect(m_rcChart, point))
	{
		if (m_AjustType == eRoughType)
		{
			m_nColorSection = GetColorSectionIndex(point);
		}

		m_AjustType = (m_AjustType == eRoughType) ? ePreciseType :eRoughType;

		//����ǰ���Ƶ�ָ���Ĭ��ֵ
		m_nCtrlPointIndex = 0;

		Invalidate();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CcolorsetView::OnMouseMove(UINT nFlags, CPoint point)
{
	//����ڿ��Ƶ���ʱ��ʾ��ʾ��Ϣ
	if (PtInRect(m_rcChart, point))
	{
		CMsgTipArray msg;
		int nIndex = IsMouseOverColorCtrlPoint(point);
		if (nIndex != -1) 
		{
			GetMsgTipInfo(msg, nIndex);
			ShowMsgTip(TRUE, msg);
		}
		else
		{
			ShowMsgTip(FALSE, msg);
		}
	}

	//ɫ��У������
	if (m_bLBtnDownAtCtrlPoint)
	{
		int nNewColorVal = Round(double(m_rcChart.bottom - point.y)/m_Yscale);
		AjustColor(nNewColorVal);
	}

	CView::OnMouseMove(nFlags, point);
}

void CcolorsetView::AjustColor(int nColorValue)
{
	//����ɫ�ʷ�Χ0-255
	int nCurselColor = nColorValue;
	if (nCurselColor < 0) nCurselColor = 0;
	if (nCurselColor > 255) nCurselColor = 255;
			

	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_AjustType == eRoughType)
	{
		//�ֵ�

		if (m_AjustMethod == eLinearMode)
		{
			//����ģʽ
			if(m_nCtrlPointIndex != 0 && m_nCtrlPointIndex != 8) 	return;

			int nNewStart = m_ColorArray[m_eColor].DotColor(0);
			int nNewEnd   = m_ColorArray[m_eColor].DotColor(255);

			if(m_nCtrlPointIndex == 0) nNewStart = nCurselColor;
			else nNewEnd =nCurselColor;

			for (int i=0; i<256; i++)
			{	
				m_ColorArray[m_eColor].DotColor(i) = nNewStart + Round(double(nNewEnd-nNewStart)*i/255);
			}
		}
		else
		{
			//����ģʽ
			int nNewStart, nNewEnd;

			//������
			//��һ�����Ƶ�û��������
			if (m_nCtrlPointIndex != 0)
			{
				nNewStart = m_ColorArray[m_eColor].DotColor((m_nCtrlPointIndex-1)*32-1);
				nNewEnd   = nCurselColor;

				for (int i=0; i<32; i++)	{
					m_ColorArray[m_eColor].DotColor((m_nCtrlPointIndex-1)*32 + i) = nNewStart + Round((double(nNewEnd-nNewStart))*i/31);
				}				
			}				

			//������
			//���һ�����Ƶ㣬û��������
			if(m_nCtrlPointIndex != BIG_CTRL_NODE-1)
			{
				nNewStart = nCurselColor;
				nNewEnd   = m_ColorArray[m_eColor].DotColor((m_nCtrlPointIndex+1)*32-1);

				for (int i=0; i<32; i++) {
					m_ColorArray[m_eColor].DotColor(m_nCtrlPointIndex*32 + i) = nNewStart + Round((double(nNewEnd-nNewStart))*i/31);
				}
			}
		}
	}
	else
	{
		//΢��
		int nNewColorValue = nCurselColor;

		m_ColorArray[m_eColor].DotColor(m_nColorSection*32+m_nCtrlPointIndex) = nNewColorValue;

	}

	Invalidate();

	WriteColor();  //д��RGB 256�����ݣ� ����ģʽ��д��256*3 RGB�������� 

	TestColor(); //д��������� ������Ϊ�������Ի����ź�Դ���� ����m_bTPenable ��ȷ��(�豸�����ɸ���m_bTPenable�������� )
}


BOOL CcolorsetView::IsLBtnDownAtColorCtrlPoint(CPoint pt)
{
	int nDotColor = 0;
	CRect rcCtrlPoint;

	if (m_AjustType == eRoughType)
	{
		//�ֵ�ģʽ 9�����Ƶ�
		for(int i=0; i<BIG_CTRL_NODE; i++)
		{
			nDotColor = m_ColorArray[m_eColor].DotColor(i*32-1);

			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*32*m_Xscale) - 7 + 10,
				                m_rcChart.bottom - Round(nDotColor*m_Yscale) - 7,
				                m_rcChart.left   + Round(i*32*m_Xscale) + 7 + 10,
				                m_rcChart.bottom - Round(nDotColor*m_Yscale) + 7);

			if (PtInRect(rcCtrlPoint, pt))
			{
				m_nCtrlPointIndex = i;
				Invalidate();

				return TRUE;
			}
		}
	}
	else
	{
		//΢��ģʽ 32�����Ƶ�
		//int nMin = GetSectionMinValue(m_nColorSection);

		for (int i=0; i<TINY_CTRL_NODE; i++)
		{
			nDotColor   = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i);
			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*m_Xscale*8) + 10 -7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-7,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+7);

			if (PtInRect(rcCtrlPoint, pt))
			{
				m_nCtrlPointIndex = i;
				Invalidate();

				return TRUE;
			}
		}	
	}

	return FALSE;
}

int CcolorsetView::GetColorSectionIndex(CPoint point)
{
	CRect rcColorSection;
	int nSection = 0;

	for (int i=0; i<8; i++)
	{
		rcColorSection = CRect(m_rcChart.left+10+int(i*32*m_Xscale),
			m_rcChart.top,
			m_rcChart.left+10+int((i+1)*32*m_Xscale),
			m_rcChart.bottom);

		if (PtInRect(rcColorSection, point))
		{
			nSection = i;
			break;
		}
	}

	return nSection;
}

void CcolorsetView::TestColor()
{
	// ��������ͼ��
	//if (m_nTestType == 0) return;

	int valueRGB[3]={0};  //��ǰ���Ե��RGBֵ
	int valueTst[3]={0};  //��ǰ���Ե��RGB����λ��
	int nDot = 0;

	//���㵱ǰ��ɫ��λ��
	if (m_AjustType == eRoughType)
		nDot = m_nCtrlPointIndex*32-1;                //�ֵ���
	else
		nDot = m_nColorSection*32+m_nCtrlPointIndex;  //΢����

	//��ȡ��ǰ���Ƶ����ɫ
	if (m_eColor == eWhite)
	{
		//���׸���ɫҪ���Ե�ǰɫ��

		int nColorValue;		
		for(int i=0; i<3; i++)
		{
			//RGBֵ
			if (nDot <= 0)
				nColorValue = Round( double(m_ColorArray[eWhite].DotColor(0)) * (double(m_ColorArray[i].DotColor(0)) / 1) );
			else
				nColorValue = Round( double(m_ColorArray[eWhite].DotColor(nDot)) * (double(m_ColorArray[i].DotColor(nDot)) / (double(nDot)) ) );

			nColorValue  = nColorValue > 255 ? 255 : nColorValue;
			valueRGB[i] = nColorValue;

			//���Ե�λ��
			valueTst[i] = (nDot > 0) ? nDot : 0;
		}
	}
	else
	{
		//��ɫ
		//RGBֵ
		valueRGB[m_eColor] = m_ColorArray[m_eColor].DotColor(nDot);
		//���Ե�λ��
		for (int nColor=0; nColor<3; nColor++)
		{
			if (nColor != m_eColor)	{
				valueTst[nColor] = 256; //256
			}else {
				valueTst[nColor] = nDot > 0 ? nDot : 0;
			}
		}
	}

	ColorTestParams ctp;

	ctp.nRvalue   = valueRGB[0];
	ctp.nGvalue   = valueRGB[1];
	ctp.nBvalue   = valueRGB[2];	
	ctp.nRtestpos  = valueTst[0];
	ctp.nGtestpos  = valueTst[1];
	ctp.nBtestpos  = valueTst[2];
	ctp.nTPenable  = (UINT)m_nTestType;

	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());

	theApp.MG_ColorTest(&ctp, pDlg->m_boardInfo.GetBoardPort());
}

void CcolorsetView::CancelTest()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());
	theApp.MG_CancelColorTest(pDlg->m_boardInfo.GetBoardPort());
}

void CcolorsetView::WriteColor()
{
	UCHAR nParam[800] = {0};
	int tick = 0;
	UINT nRet;

	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());
	int nPort = pOwner->m_boardInfo.GetBoardPort();

	if (m_eColor == eWhite)
	{
		//���� ��������ɫ��Ҫд
		int nColorValue;
		tick = 0;

		for (int ncolor=0; ncolor<3; ncolor++)
		{			
			nParam[tick++] = ncolor;
			//��һ���� ������Ϊ0 ǿ��Ϊ1
			nColorValue = Round ( double(m_ColorArray[eWhite].DotColor(0)) * (double(m_ColorArray[ncolor].DotColor(0))/1) );
			nParam[tick++] = nColorValue > 255 ? 255 : nColorValue;

			for (int i=1; i<256; i++)
			{
				nColorValue = Round( double(m_ColorArray[eWhite].DotColor(i)) * ( (double(m_ColorArray[ncolor].DotColor(i))) / (double(i)) ) );
				nParam[tick++] = nColorValue > 255 ? 255 : nColorValue;
			}
	
			//nRet = theApp.WriteColor(nParam, 256, ncolor, nPort);
		}

		nRet = theApp.MG_WriteColor(nParam, tick, 0x07, nPort);
	}
	else
	{
		//��ɫ
		tick = 0;
		for (int i=0; i<256; i++)	{
			nParam[tick++] = m_ColorArray[m_eColor].DotColor(i);
		}

		//nRet = theApp.WriteColor(nParam, 256, m_eColor, nPort);
		nRet = theApp.MG_WriteColor(nParam, tick, 0x01<<m_eColor, nPort);
	}
}

void CcolorsetView::ResetColor()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());

	unsigned int nRet,nParam[10];
	nParam[0] = m_eColor == eWhite ? 0x07 : m_eColor;
	nRet = theApp.MG_ResetColor(nParam[0], pDlg->m_boardInfo.GetBoardPort());

	if (nRet != 0x00)
	{
		MessageBox(_T("��λʧ�ܣ�ͨ���쳣����ָ��ش���"), _T("ɫ�λ"), MB_OK|MB_ICONSTOP);
	}
	else
	{
		if (m_AjustType == eRoughType)//�ֵ�
		{
			if (m_eColor == eWhite)
			{
				for (int i=0 ;i< 4; i++)
					m_ColorArray[i].ResetColor();
			}
			else
				m_ColorArray[m_eColor].ResetColor();
		}
		else  //ϸ��
		{
			if (m_eColor == eWhite)
			{
				for (int i=0; i<4; i++)
					m_ColorArray[i].ResetSecColor(m_nColorSection);
			}
			else
				m_ColorArray[m_eColor].ResetSecColor(m_nColorSection);
		}

		Invalidate();

		MessageBox(_T("��λ�ɹ���"), _T("ɫ�λ"), MB_OK|MB_ICONINFORMATION);
	}	
}

void CcolorsetView::OnVKeyUpDown(UINT nVKCode)
{
	//����ɫ��ֵ
	int nDotColor;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_AjustType == eRoughType)
	{
		nDotColor = m_ColorArray[m_eColor].DotColor(m_nCtrlPointIndex*32-1);
	}
	else
	{
		nDotColor = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 + m_nCtrlPointIndex);
	}

	if (nVKCode == VK_UP)
	{
		//���ˡ������
		nDotColor ++;
	}
	else if (nVKCode == VK_DOWN)
	{
		//���ˡ������
		nDotColor --;
	}

	AjustColor(nDotColor);

}

void CcolorsetView::SetColor(int *data, int nColorSel)
{	
	if (nColorSel > 3 || nColorSel < 0) return;

	if (nColorSel == 3)
	{//����
		for (int i=0; i<256; i++)	{
			m_ColorArray[nColorSel].DotColor(i) = Round((m_ColorArray[0].DotColor(i) + m_ColorArray[1].DotColor(i) + m_ColorArray[2].DotColor(i))/3.0);
		}
	}
	else
	{
		ASSERT(data);
		for (int i=0; i<256; i++)	{
			m_ColorArray[nColorSel].DotColor(i) = data[i];
		}
	}

}

void CcolorsetView::ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip, BOOL bDockAtCursor, CPoint ptDockAt)
{
	if(m_pMsgTip == NULL)  
	{  
		m_pMsgTip = new CMsgTipDlg();  
		m_pMsgTip->Create(IDD_MSGTIPDLG, this);  
	} 
	if(!bShow)  
	{  
		if(m_pMsgTip->IsWindowVisible()) m_pMsgTip->ShowWindow(SW_HIDE);  
	}  
	else  
	{  
		CSize size = m_pMsgTip->ShowMsgTip(arMsgTip);  
		if(size.cx == 0)  
			return;

		CPoint pt; 
		if (bDockAtCursor) GetCursorPos(&pt); //��ʾ���������λ��
		else 	pt = ptDockAt;  //ָ����ʾ��λ��

		CRect rc(pt,size);  
		rc.OffsetRect(15,20);  
		CRect mRc;  
		GetWindowRect(&mRc);  
		if(rc.bottom > mRc.bottom)  
			rc.OffsetRect(0,mRc.bottom-rc.bottom-5);  
		if(rc.right > mRc.right)  
		{
			rc.OffsetRect(-rc.Width()-30,0);
		}
		m_pMsgTip->SetWindowPos(&wndTop, rc.left, rc.top, rc.Width(),rc.Height(),SWP_SHOWWINDOW | SWP_NOACTIVATE);  
		m_pMsgTip->DrawMsgTip();  
		//m_pMsgTip->ShowWindow(SW_SHOW);
	}  
}

void CcolorsetView::GetMsgTipInfo(CMsgTipArray &arMsgTip, int nCtrlPtIndex)
{
	CString strInfo;
	MSG_TIP mt;
	COLORREF colorMsg;
	colorMsg =  m_eColor== eRed ? RGB(255,0,0) : m_eColor == eGreen ? RGB(0,255,0) : m_eColor == eBlue ? RGB(0,0,255) : RGB(100, 50, 80);

	//��ʾ��ı���
	mt.nInfoShowColor = -1;
	mt.sMsgItemName = _T("ɫ��У��");
	mt.sMsgItemInfo = _T("ɫ��У��");	
	arMsgTip.Add(mt);


	//��ʾ��Ŀһ ��ɫ
	strInfo = m_eColor == eRed ? _T("��ɫ") : m_eColor == eGreen ? _T("��ɫ") : m_eColor == eBlue ? _T("��ɫ") : _T("����");
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName = _T("��ɫ");
	mt.sMsgItemInfo = strInfo;
	arMsgTip.Add(mt);


	//��ʾ��Ŀ�� У׼ֵ
	int nDotColor, nStdDotColor;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if(m_AjustType == eRoughType)	{	nDotColor = m_ColorArray[m_eColor].DotColor(nCtrlPtIndex*32-1); }
	else	{	nDotColor = m_ColorArray[m_eColor].DotColor(m_nColorSection*32+nCtrlPtIndex);	}
	strInfo.Format(_T("%d"), nDotColor);
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName   = _T("У׼ֵ");
	mt.sMsgItemInfo   = strInfo;
	arMsgTip.Add(mt);


	//��ʾ��Ŀ�� ��׼ֵ
	if(m_AjustType == eRoughType)	{	nStdDotColor = (nCtrlPtIndex*32-1) > 0 ? (nCtrlPtIndex*32-1) : 0 ; }
	else	{	nStdDotColor = m_nColorSection*32+nCtrlPtIndex;	}
	strInfo.Format(_T("%d"), nStdDotColor);
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName   = _T("��׼ֵ");
	mt.sMsgItemInfo   = strInfo;
	arMsgTip.Add(mt);
}

int CcolorsetView::IsMouseOverColorCtrlPoint(CPoint point)
{
	int nDotColor = 0;
	CRect rcCtrlPoint;
	int nCtrlIndex = 0;

	if (m_AjustType == eRoughType)
	{
		//�ֵ�ģʽ 9�����Ƶ�
		for(int i=0; i<BIG_CTRL_NODE; i++)
		{
			nDotColor = m_ColorArray[m_eColor].DotColor(i*32-1);

			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*32*m_Xscale) - 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) - 7,
				m_rcChart.left   + Round(i*32*m_Xscale) + 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) + 7);

			if (PtInRect(rcCtrlPoint, point))
			{
				nCtrlIndex = i;

				return nCtrlIndex;
			}
		}
	}
	else
	{
		//΢��ģʽ 32�����Ƶ�
		for (int i=0; i<TINY_CTRL_NODE; i++)
		{
			nDotColor   = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i);
			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*m_Xscale*8) + 10 -7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-7,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+7);

			if (PtInRect(rcCtrlPoint, point))
			{
				nCtrlIndex = i;

				return nCtrlIndex;
			}
		}	
	}


	return -1;
}