// HelpText.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "PortHelpText.h"


// CPortHelpText 对话框

IMPLEMENT_DYNAMIC(CPortHelpText, CDialog)

CPortHelpText::CPortHelpText(CWnd* pParent /*=NULL*/)
	: CDialog(CPortHelpText::IDD, pParent)
{
	m_nPortID = -1;
	m_nType   = -1;
}

CPortHelpText::~CPortHelpText()
{
}

void CPortHelpText::SetPortInfo(const int nID, const int nType)
{
	m_nPortID = nID;
	m_nType   = nType;
}

INT_PAIR CPortHelpText::GetPortInfo()
{
	return INT_PAIR(m_nPortID, m_nType);
}

void CPortHelpText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPortHelpText, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CPortHelpText 消息处理程序
BOOL CPortHelpText::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetMargins();

	return TRUE;
}

void CPortHelpText::SetMargins()
{
	CRect rcCli;
	GetClientRect(&rcCli);

	m_HelpMargin.rcCli = Rect(rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	m_HelpMargin.rcTextBk = m_HelpMargin.rcCli;
	m_HelpMargin.rcTextBk.Inflate(-6, -6);

	m_HelpMargin.rcTitle = Rect(rcCli.left + 20, rcCli.top + 16, rcCli.Width() - 20*2, 20);

	m_HelpMargin.rcSeparator = m_HelpMargin.rcTitle;
	m_HelpMargin.rcSeparator.Y = m_HelpMargin.rcTitle.GetBottom()+1;
	m_HelpMargin.rcSeparator.Height = 5;

	m_HelpMargin.rcPortInfoText = m_HelpMargin.rcSeparator;
	m_HelpMargin.rcPortInfoText.Y = m_HelpMargin.rcSeparator.GetBottom()+1;
	m_HelpMargin.rcPortInfoText.Height = 25;

	m_HelpMargin.rcPortTypeText = m_HelpMargin.rcPortInfoText;
	m_HelpMargin.rcPortTypeText.Y = m_HelpMargin.rcPortInfoText.GetBottom()+1;

	m_HelpMargin.rcPortPosText = m_HelpMargin.rcPortTypeText;
	m_HelpMargin.rcPortPosText.Y = m_HelpMargin.rcPortTypeText.GetBottom()+1;

	m_HelpMargin.rcPortModel = m_HelpMargin.rcPortPosText;
	m_HelpMargin.rcPortModel.Y = m_HelpMargin.rcPortPosText.GetBottom()+1;
	m_HelpMargin.rcPortModel.Height = rcCli.bottom - 15 - m_HelpMargin.rcPortModel.Y; 
}

void CPortHelpText::OnPaint()
{
	CPaintDC dc(this);

	// 获取窗体大小
	CRect rcCli;
	GetClientRect(&rcCli);

	// 创建一个内存DC
	CDC* pDC = new CDC;
	pDC->CreateCompatibleDC(&dc);

	CBitmap bmp, *pOldbmp;
	bmp.CreateCompatibleBitmap(&dc, rcCli.Width(), rcCli.Height());
	pOldbmp = pDC->SelectObject(&bmp);

	// 填充背景色
	CRgn rgnCli;
	rgnCli.CreateRectRgn(rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	CRgn rgnRound;
	rgnRound.CreateRoundRectRgn(6, 6, rcCli.right-6, rcCli.bottom-6, 9, 9);

	CRgn rgnFrame;
	rgnFrame.CreateRectRgn(0, 0, 50, 50);
	rgnFrame.CombineRgn(&rgnCli, &rgnRound, RGN_XOR);

	CBrush brFrame(m_nType==1 ? RGB(181, 230, 29) : RGB(129, 250, 191));
	pDC->FillRgn(&rgnFrame, &brFrame);

	CBrush brTextBk(m_nType==1 ? RGB(239, 228, 176) : RGB(176, 178, 239));
	pDC->FillRgn(&rgnRound, &brTextBk);

	// 在内存DC上创建一个GDI+绘制对象
	Graphics gp(pDC->m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighQuality);

	// 端口信号名
	DrawTitle(&gp);

	// 分割线
	DrawSeparator(&gp);

	// 无效的端口，下面的绘制就省了
	if ((m_nPortID < 0) || (m_nType < 0))
	{
		dc.BitBlt(0, 0, rcCli.Width(),rcCli.Height(), pDC, 0, 0, SRCCOPY);

		pDC->SelectObject(pOldbmp);
		ReleaseDC(pDC);
		SAFE_DELETE(pDC);

		return;
	}

	// 端口属性
	DrawPortInfo(&gp);

	// 端口模拟位置图
	//DrawModel(&gp);

	dc.BitBlt(0, 0, rcCli.Width(),rcCli.Height(), pDC, 0, 0, SRCCOPY);

	pDC->SelectObject(pOldbmp);
	ReleaseDC(pDC);
	SAFE_DELETE(pDC);
}

void CPortHelpText::DrawTitle(Graphics* pGraphics)
{
	ASSERT(pGraphics);

	// 端口信号名
	FontFamily fontFamily(L"新宋体");
	//Gdiplus::Font font(&fontFamily, 14.0f, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, 14.0f, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(255, 136, 0, 73));
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	CString strText;

	if ((m_nType < 0) || (m_nPortID < 0))
	{
		strText = L"Error";
	}
	else
	{
		if (m_nType == 1)
		{
			int nSubID = GetInputManager()->GetSubID(m_nPortID);
			if (nSubID == 0)
				strText.Format(_T("%-2d.%s"), GetInputManager()->GetRealIndex(m_nPortID)+1, GetInputManager()->GetName(m_nPortID));
			else if (nSubID == 1 || nSubID == 2)
				strText.Format(_T("%-2d.%s #%d"), GetInputManager()->GetRealIndex(m_nPortID)+1, GetInputManager()->GetName(m_nPortID), nSubID);
			else
				strText.Format(_T("Error"));
			//strText.Format(_T("%d. %s"), m_nPortID+1, GetInputManager()->GetName(m_nPortID));
		}
		else if (m_nType == 2)
			strText.Format(_T("%d. %s"), m_nPortID+1, GetOutputManager()->GetName(m_nPortID));
	}

	WCHAR wszText[_MAX_SIGNAL_NAME];
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);

	RectF textlayout;
	textlayout.X = (REAL)m_HelpMargin.rcCli.X + 20;
	textlayout.Y = (REAL)m_HelpMargin.rcCli.Y + 16;
	textlayout.Width = m_HelpMargin.rcCli.Width - 20.0f*2;
	textlayout.Height = 20.0f; 

	pGraphics->DrawString(wszText, -1, &Bfont, textlayout, &format, &blackBr);

}

void CPortHelpText::DrawSeparator(Graphics* pGraphics)
{
	ASSERT(pGraphics);

	Gdiplus::Color color(255, 136, 0, 73);
	REAL position = 0.5f;
	LinearGradientBrush SeparatorBr(Point(0, 0), Point(0, 5), Color(255, 204, 0, 110), Color(255, 204, 0, 110));
	SeparatorBr.SetInterpolationColors(&color, &position, 1);

	pGraphics->FillRectangle(&SeparatorBr, m_HelpMargin.rcSeparator);
}

void CPortHelpText::DrawPortInfo(Graphics* pGraphics)
{
	ASSERT(pGraphics);

	FontFamily fontFamily(L"新宋体");
	Gdiplus::Font font(&fontFamily, 12.0f, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, 12.0f, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(255, 93, 102, 234));
	SolidBrush WarningBr(Color(255, 177, 17, 22));
	SolidBrush HighLightBr(Color(255, 63, 72, 204));
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	RectF textlayout;
	textlayout.X = (REAL)m_HelpMargin.rcPortInfoText.X;
	textlayout.Y = (REAL)m_HelpMargin.rcPortInfoText.Y;
	textlayout.Width  = (REAL)m_HelpMargin.rcPortInfoText.Width;
	textlayout.Height = (REAL)m_HelpMargin.rcPortInfoText.Height;

	RectF rcHighLight;
	rcHighLight.X = textlayout.X + 60;
	rcHighLight.Y = textlayout.Y + 2;
	rcHighLight.Width = textlayout.Width - 60;
	rcHighLight.Height = textlayout.Height - 4;

	SolidBrush yellow(Color(255, 255, 242, 0));
	Pen pen(Color(255, 136, 0, 73));

	// 输入信号属性
	CStringW strText;
	strText = L"输入信号: ";
	pGraphics->DrawString(strText, -1, &font, textlayout, &format, &blackBr);

	pGraphics->FillRectangle(&yellow, rcHighLight);
	pGraphics->DrawRectangle(&pen, rcHighLight);

	BOOL HasInput = TRUE;
	if (m_nType == 1)
	{
		int hact, vact, hz;
		hact = vact = hz = 0;
		GetInputManager()->GetCurrencyResolution(m_nPortID, hact, vact, hz);
		emVideoType eVideo = GetInputManager()->GetSignalType(m_nPortID);

		if ((hact==0) || (vact==0) || (hz==0) || eVideo == vtUndef)
		{	strText = L" 无信号";  HasInput = FALSE;  }
		else
		{	strText.Format(L" %dx%d @ %dHz (%s)", hact, vact, hz, GetVideoName(eVideo));  }
	}
	else if (m_nType == 2)
	{
		int nFeederPort = GetOutputManager()->GetFeederID(m_nPortID);
		if (nFeederPort > -1)
		{	strText.Format(L" %d %s", nFeederPort, GetInputManager()->GetName(nFeederPort)); }
		else
		{	strText.Format(L" 无输入");  HasInput = FALSE; }
	}

	pGraphics->DrawString(strText, -1, &Bfont, rcHighLight, &format, HasInput ? &HighLightBr : &WarningBr);

	// 端口特性
	textlayout.Offset(0, 25);
	strText = L"端口特性: ";
	pGraphics->DrawString(strText, -1, &font, textlayout, &format, &blackBr);

	rcHighLight.Offset(0, 25);
	pGraphics->FillRectangle(&yellow, rcHighLight);
	pGraphics->DrawRectangle(&pen, rcHighLight);

	strText = L" DVI 端口";
	pGraphics->DrawString(strText, -1, &Bfont, rcHighLight, &format, &HighLightBr);

	// 端口位置
	textlayout.Offset(0, 25);
	strText = L"端口位置: ";
	pGraphics->DrawString(strText, -1, &font, textlayout, &format, &blackBr);

}


void CPortHelpText::DrawModel(Graphics* pGraphics)
{
	ASSERT(pGraphics);

	// 端口位置模型
	Pen FramePen(Color(255, 195, 195, 195));
	SolidBrush FrameBr(Color(255, 255, 201, 14));

	pGraphics->FillRectangle(&FrameBr, m_HelpMargin.rcPortModel);
	pGraphics->DrawRectangle(&FramePen, m_HelpMargin.rcPortModel);

	Image* img4InDvi=NULL;
	Image* img4OutDvi=NULL;
	Image* imgFocusDvi=NULL;

	ImageFromIDResource(IDB_PNG_DVI_4INPUT, "PNG", img4InDvi);
	ImageFromIDResource(IDB_PNG_DVI_4OUTPUT, "PNG", img4OutDvi);
	ImageFromIDResource(IDB_PNG_DVI_FOCUS, "PNG", imgFocusDvi);

	if (img4InDvi == NULL || img4OutDvi == NULL || imgFocusDvi==NULL)
	{
		if (img4InDvi != NULL) SAFE_DELETE(img4InDvi);
		if (img4OutDvi != NULL) SAFE_DELETE(img4OutDvi);
		if (imgFocusDvi != NULL) SAFE_DELETE(imgFocusDvi);

		return;
	}

	img4InDvi->RotateFlip(Rotate90FlipNone);
	img4OutDvi->RotateFlip(Rotate90FlipNone);
	imgFocusDvi->RotateFlip(Rotate90FlipNone);

	// 铺展模型图片
	int nSpaX = (m_HelpMargin.rcPortModel.Width-img4InDvi->GetWidth()*2)/3;
	int nSpaY = (m_HelpMargin.rcPortModel.Height - img4InDvi->GetHeight()*6)/7;

	// 3U机箱，左边全是输入，右边除最下边一块控制卡外全是输出
	RectF rcImg;
	for (int i=0; i<6; i++)
	{
		rcImg.X = m_HelpMargin.rcPortModel.X + nSpaX*1.0f;
		rcImg.Y = m_HelpMargin.rcPortModel.Y + nSpaY*1.0f + (nSpaY+img4InDvi->GetHeight())*i;
		rcImg.Width = img4InDvi->GetWidth()*1.0f;
		rcImg.Height = img4InDvi->GetHeight()*1.0f;

		pGraphics->DrawImage(img4InDvi, rcImg);
	}

	for (int i=0; i<5; i++)
	{
		rcImg.X = m_HelpMargin.rcPortModel.X + nSpaX*2.0f + img4InDvi->GetWidth();
		rcImg.Y = m_HelpMargin.rcPortModel.Y + nSpaY*1.0f + (nSpaY+img4InDvi->GetHeight())*i;
		rcImg.Width  = img4InDvi->GetWidth()*1.0f;
		rcImg.Height = img4InDvi->GetHeight()*1.0f;

		pGraphics->DrawImage(img4OutDvi, rcImg);
	}

	if (m_nType == 1)
	{
		int nCmdPort = GetInputManager()->GetCmdPort(m_nPortID);
		rcImg.X = m_HelpMargin.rcPortModel.X + nSpaX*1.0f + 5.0f + (imgFocusDvi->GetWidth()-1)*((nCmdPort-1)%4);
		rcImg.Y = m_HelpMargin.rcPortModel.Y + nSpaY*1.0f + ((nCmdPort-1)/4) * (img4InDvi->GetHeight() + nSpaY)-2;
		rcImg.Width  = imgFocusDvi->GetWidth()*1.0f;
		rcImg.Height = imgFocusDvi->GetHeight()*1.0f;

		pGraphics->DrawImage(imgFocusDvi, rcImg);
	}
	else if (m_nType == 2)
	{
		int nCmdPort = GetOutputManager()->GetCmdPort(m_nPortID);
		rcImg.X = m_HelpMargin.rcPortModel.X + nSpaX*2.0f + img4InDvi->GetWidth() + 5.0f + (imgFocusDvi->GetWidth()-1)*(nCmdPort%4-1);
		rcImg.Y = m_HelpMargin.rcPortModel.Y + nSpaY*1.0f + (nCmdPort/4) * (img4InDvi->GetHeight() + nSpaY)-2;
		rcImg.Width  = imgFocusDvi->GetWidth()*1.0f;
		rcImg.Height = imgFocusDvi->GetHeight()*1.0f;

		pGraphics->DrawImage(imgFocusDvi, rcImg);
	}

	SAFE_DELETE(img4InDvi);
	SAFE_DELETE(img4OutDvi);
	SAFE_DELETE(imgFocusDvi);
}

BOOL CPortHelpText::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CPortHelpText::OnMouseMove(UINT nFlags, CPoint point)
{
	ShowWindow(SW_HIDE);
	return;

	return CDialog::OnMouseMove(nFlags, point);
}