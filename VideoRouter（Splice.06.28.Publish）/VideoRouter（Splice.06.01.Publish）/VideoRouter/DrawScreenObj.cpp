// DrawScreenObj.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "DrawScreenObj.h"
#include "DrawWndPage.h"

int CDrawScreenObj::m_iHorz = 2;
int CDrawScreenObj::m_iVert = 2;
BOOL CDrawScreenObj::m_bLockWall = TRUE;

// CDrawScreenObj

IMPLEMENT_DYNAMIC(CDrawScreenObj, CCmdTarget)


CDrawScreenObj::CDrawScreenObj()
{
	m_pOwner = NULL;
}

CDrawScreenObj::CDrawScreenObj(CDrawWndPage* pOwner)
{
	m_pOwner = pOwner;
}

CDrawScreenObj::~CDrawScreenObj()
{
	m_pOwner = NULL;
}


BEGIN_MESSAGE_MAP(CDrawScreenObj, CCmdTarget)
END_MESSAGE_MAP()



// CDrawScreenObj 消息处理程序
void CDrawScreenObj::SetOwner(CDrawWndPage* pOwner)
{
	m_pOwner = pOwner;
}

void CDrawScreenObj::SetID(const int nID)
{
	m_iScreenID = nID;
}

void CDrawScreenObj::SetOutPort(const int iPort)
{
	m_iOutPort = iPort;

	if (iPort == INVALID_VALUE)
		m_bShutDown = TRUE;
	else
		m_bShutDown = FALSE;
}

void CDrawScreenObj::SetFocus(BOOL bFocus)
{
	m_bFocused = bFocus;
}

void CDrawScreenObj::SetShutDown(BOOL bShutDown)
{
	m_bShutDown = bShutDown;
}

void CDrawScreenObj::SetName(char *szName)
{
	memset(m_szName, 0, sizeof(m_szName));

	int nLen = strlen(szName);

	strncpy_s(m_szName, _MAX_WND_NAME, szName, nLen);
}

void CDrawScreenObj::SetLogCoords(const RectF rcScreen)
{
	m_rcLog = rcScreen;
}

void CDrawScreenObj::SetPhyCoords(const Rect rc)
{
	m_rcPhy = rc;
}

void CDrawScreenObj::Lock(BOOL bLock /* = TRUE */)
{
	m_bLock = bLock;
}

void CDrawScreenObj::Zoom()
{
	CRect rcPage = m_pOwner->GetPosition();

	CRect rcL, rcP;
	rcP = CRect(m_rcPhy.X, m_rcPhy.Y, m_rcPhy.GetRight(), m_rcPhy.GetBottom());
	rcL = rcP.MulDiv(CDrawWndObj::m_sxLogScalar, CDrawWndObj::m_sxPhyScalar);
	rcL.OffsetRect(rcPage.TopLeft());

	m_rcLog = RectF(REAL(rcL.left), REAL(rcL.top), REAL(rcL.Width()), REAL(rcL.Height()));
}

void CDrawScreenObj::OnOutputChange()
{
	SCREENMAP scrmap;
	scrmap.iScreenID = m_iScreenID;
	scrmap.iOutputCh = m_iOutPort;
	scrmap.rcScreenPos.left = m_rcPhy.X;
	scrmap.rcScreenPos.top  = m_rcPhy.Y;
	scrmap.rcScreenPos.right= m_rcPhy.GetRight()-1;
	scrmap.rcScreenPos.bottom = m_rcPhy.GetBottom()-1;

	GetCmdManager()->CmdSOPS(SYSBOARD_PORT, eBoardTypeCtrl, scrmap);
}

void CDrawScreenObj::Draw(CDC *pPrarentDC)
{
	if (!m_pOwner->IsObjVisible(CRect((int)m_rcLog.GetLeft(), (int)m_rcLog.GetTop(), (int)m_rcLog.GetRight(), (int)m_rcLog.GetBottom())))
		return;

	RectF rcObj = m_rcLog;

	Graphics gp(pPrarentDC->m_hDC);
	gp.SetSmoothingMode(SmoothingModeAntiAlias);

	//绘制开关状态
	SolidBrush brush(Color(200, 35, 35, 35));
	if (m_bShutDown)
	{
		gp.FillRectangle(&brush, rcObj);
	}

	//绘制选中框
	Pen redPen(Color(255, 0, 0, 0), 5);
	if (m_bFocused)
	{
		gp.DrawRectangle(&redPen, rcObj);
	}

	//绘制文本
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingNone);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	//标题
	RectF textLayout;
	WCHAR wszText[_MAX_SIGNAL_NAME];
	CString strText;

	brush.SetColor(Color(255, 0, 0, 0));
	//屏幕名称
	textLayout = RectF(rcObj.X+5, rcObj.Y+5, rcObj.Width-5, 20.0f);

	strText.Format(_T("%s"), m_szName);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_WND_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &brush);

	//输出端口号
	textLayout = RectF(rcObj.X+5, rcObj.Y+20, rcObj.Width-5, 20.f);

	if (m_iOutPort == INVALID_VALUE)
		strText.Format(_T("输出端口: 无"));
	else
		strText.Format(_T("输出端口: %d"), m_iOutPort);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_WND_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &brush);
}

void CDrawScreenObj::DrawScreenBk(Graphics* pGraphics)
{
	ASSERT(pGraphics);

	//************ 填充屏幕背景
	// A 渐变色背景
	Color colors[] = {
		Color(220, 220, 220, 255),
		Color(255, 128, 128, 255),
		Color(220, 220, 220, 255),
	};

	REAL positions[] = {
		0.0f,
		0.45f,
		1.0f
	};

	LinearGradientBrush GradientBrushBrown(PointF(m_rcLog.X, m_rcLog.Y), PointF(m_rcLog.X+m_rcLog.Height, m_rcLog.Y+m_rcLog.Height), Color(255, 40, 140, 120), Color(255, 76, 176, 156));
	GradientBrushBrown.SetInterpolationColors(colors, positions, 3);
	pGraphics->FillRectangle(&GradientBrushBrown, m_rcLog);
	
	// B 纯灰色背景
	//SolidBrush brBk(Color(180,128,128,128));
	//pGraphics->FillRectangle(&brBk, m_rcLog);

	//************ 屏幕详细
	//绘制开关状态
	SolidBrush brush(Color(200, 35, 35, 35));
	if (m_bShutDown)
	{
		pGraphics->FillRectangle(&brush, m_rcLog.X, m_rcLog.Y, m_rcLog.Width, m_rcLog.Height);
	}

	//绘制选中框
	Pen redPen(Color(255, 0, 0, 0), 5);
	if (m_bFocused)
	{
		pGraphics->DrawRectangle(&redPen, m_rcLog.X, m_rcLog.Y, m_rcLog.Width, m_rcLog.Height);
	}

	//绘制文本
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentNear);
	format.SetTrimming(StringTrimmingNone);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	//标题
	RectF textLayout;
	WCHAR wszText[_MAX_SIGNAL_NAME];
	CString strText;

	brush.SetColor(Color(255, 0, 0, 0));
	//屏幕名称
	textLayout = RectF((REAL)m_rcLog.GetLeft()+5, (REAL)m_rcLog.GetTop()+5, (REAL)m_rcLog.Width-5, 20.0f);
	if (textLayout.GetBottom() > m_rcLog.GetBottom())
		textLayout.Height = m_rcLog.GetBottom() - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		strText.Format(_T("%s"), m_szName);
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_WND_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &brush);
	}	

	//输出端口号
	textLayout = RectF((REAL)m_rcLog.GetLeft()+5, (REAL)m_rcLog.GetTop()+20, (REAL)m_rcLog.Width-5, 20.f);
	if (textLayout.GetBottom() > m_rcLog.GetBottom())
		textLayout.Height = m_rcLog.GetBottom() - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		if (m_iOutPort == INVALID_VALUE)
			strText.Format(_T("输出端口: 无"));
		else
			strText.Format(_T("输出端口: %d"), m_iOutPort);
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_WND_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &brush);
	}
}

void CDrawScreenObj::DrawGridLine(Graphics* pGraphics)
{
	//if (!CDrawWndPage::m_sShowGridLines)
	//	return;

	ASSERT(pGraphics);

	//************ 画屏幕边框和网格线
	Pen SolidPen(Color(255, 10, 28, 155), 1);
	Pen DotPen(Color(255, 10, 28, 155), 1);
	DotPen.SetDashStyle(DashStyleDot);
	pGraphics->DrawRectangle(&SolidPen, m_rcLog);

	//double dVSpa = m_rcPhy.Height()*1.0/m_iVert;
	//double dHSpa = m_rcPhy.Width()*1.0/m_iHorz;

	//double xScale = m_rcLog.Width*1.0/m_rcPhy.Width();
	//double yScale = m_rcLog.Height*1.0/m_rcPhy.Height();

	//for (int i=0; i<m_iVert; i++)
	//	pGraphics->DrawLine(&DotPen, PointF(REAL(m_rcLog.GetLeft()), REAL(m_rcLog.GetTop() + i*dVSpa*yScale)), PointF(REAL(m_rcLog.GetRight()), REAL(m_rcLog.GetTop() + i*dVSpa*yScale)));

	//for (int i=0; i<m_iHorz; i++)
	//	pGraphics->DrawLine(&DotPen, PointF(REAL(m_rcLog.GetLeft() + i*dHSpa*xScale), REAL(m_rcLog.GetTop())), PointF(REAL(m_rcLog.GetLeft() + i*dHSpa*xScale), REAL(m_rcLog.GetBottom())));
}

void CDrawScreenObj::OnPopupMenu(CPoint point)
{
	//弹出菜单

}

void CDrawScreenObj::OnLBtnDown(CPoint point)
{
	
}

void CDrawScreenObj::OnLBtnUp(CPoint point)
{

}

void CDrawScreenObj::OnLBtnDblClick(CPoint point)
{
	m_pOwner->FocusScreen(this);
}

void CDrawScreenObj::OnMouseMove(CPoint point)
{

}