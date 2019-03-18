// MGblurDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGFilterDlg.h"
#include "MGeoMainDlg.h"

using namespace std;

#define PI	3.1415926
//高斯模糊函数
#define GAUSS_FUN(x, y) (exp(-(x*x)/(double)(2*y*y)))/(sqrt(2.0*PI)*y)
#define SQUARE(x) ((x)*(x))

//暂且定义sigma为10
const double sigma=5;

struct COLOR_DATA
{
	double dDistance;
	DWORD dwColor;
};

long g_lBmpSize;
HBITMAP g_hSrcBitmap;
int g_nBmpWidth;
int g_nBmpHeight;
HWND g_hMainWnd;
#define WM_GAUSS_MSG WM_USER+1

CEvent g_WaitEvent;


//高斯模糊，nRadius为平均取值的半径，半径越大，图像越模糊，处理时间也越长
bool GaussFilter( DWORD* pData, DWORD* pCopy, const int nWidth, const int nHeight, const int nRadius/*=1*/ )
{
	if ( nWidth<=0 || nHeight<=0 || nRadius<=0 )
		return false;
	for ( int ny=0; ny<nHeight; ++ny )
	{
		for ( int nx=0; nx<nWidth; ++nx )
		{
			vector<COLOR_DATA> cdList;
			cdList.reserve(200);
			COLOR_DATA cd;
			double dTotal=0;
			for ( int m=nx-nRadius; m<=nx+nRadius; ++m )
			{
				if ( m<0 || m>=nWidth ) continue;
				for ( int n=ny-nRadius; n<=ny+nRadius; ++n )
				{
					if ( n<0 || n>=nHeight ) continue;
					cd.dDistance = GAUSS_FUN(sqrt((double)(SQUARE(m-nx)+SQUARE(n-ny))), sigma);
					dTotal+=cd.dDistance;
					cd.dwColor=*(pData+n*nWidth+m);
					cdList.push_back(cd);
				}
			}
			if ( cdList.size()>0 )
			{//这里来计算整个邻域内所有像素点的加权平均值
				std::vector<COLOR_DATA>::const_iterator itor=cdList.begin();
				double r=0, g=0, b=0;
				for ( ; itor!=cdList.end(); ++itor ) 
				{
					double dRate=itor->dDistance/dTotal;//距离中心点越远，权值越小
					r+=GetRValue(itor->dwColor)*dRate;
					g+=GetGValue(itor->dwColor)*dRate;
					b+=GetBValue(itor->dwColor)*dRate;
				}
				*(pCopy+ny*nWidth+nx)=RGB((int)r, (int)g, (int)b);
			}
		}
	}
	return true;
}


// CMGblurDlg 对话框

IMPLEMENT_DYNAMIC(CMGFilterDlg, CXTPResizeDialog)

CMGFilterDlg::CMGFilterDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGFilterDlg::IDD, pParent)
{
	m_clrPaint = RGB(50,10,50);
	m_pThreadGaussFilter = NULL;
	m_nFilterRadius = 6;
}

CMGFilterDlg::~CMGFilterDlg()
{
}

void CMGFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BLUR_VALUE, m_SliderBlur);
	DDX_Control(pDX, IDC_EDIT_BLUR_RADIUS, m_EditRadius);
	DDX_Control(pDX, IDC_SPIN_BLUR_RADIUS, m_SpinRadius);
	DDX_Text(pDX, IDC_EDIT_BLUR_RADIUS, m_nFilterRadius);
	DDV_MinMaxInt(pDX, m_nFilterRadius, 0, 29);
}


BEGIN_MESSAGE_MAP(CMGFilterDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BLUR_RADIUS, OnDeltaposSpinBlurRadius)
END_MESSAGE_MAP()


// CMGblurDlg 消息处理程序

BOOL CMGFilterDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	ResizeIt();

	m_SpinRadius.SetBuddy(&m_EditRadius);

	m_bkBrush.CreateSolidBrush(constClrDlgBk[7]);

	m_SliderBlur.SetRange(0,29);
	m_SliderBlur.SetTicFreq(1);
	m_SliderBlur.SetPos(m_nFilterRadius);

	m_srcBitmap.LoadBitmap(IDB_BITMAP_FILTER);
	m_BluredBitmap.LoadBitmap(IDB_BITMAP_FILTER);

	g_hSrcBitmap = (HBITMAP)m_BluredBitmap.GetSafeHandle();

	return TRUE; 
}


void CMGFilterDlg::ResizeIt()
{
	SetResize(IDC_TEXT_MIN_BLUR, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_MAX_BLUR, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_BLUR, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_BLUREFFECT, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_SLIDER_BLUR_VALUE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_PIC_BLUR_EFFICTIVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_EDIT_BLUR_RADIUS, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_SPIN_BLUR_RADIUS, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
}

HBRUSH CMGFilterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);

		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}

	return hbr;
}

void CMGFilterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//获取绘图区大小
	CRect rcClient, rcPaint;
	GetDlgItem(IDC_PIC_BLUR_EFFICTIVE)->GetWindowRect(&rcClient);
	rcPaint = rcClient;
	rcPaint.InflateRect(-1,-1);
	
	//获取绘图DC
	CDC *pDC = GetDlgItem(IDC_PIC_BLUR_EFFICTIVE)->GetDC();

	//创建内存DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//创建内存位图
	CBitmap membmp;
	membmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	CBitmap *pOldbmp = pMemDC->SelectObject(&membmp);

	//用指定颜色清除背景区
	pMemDC->FillSolidRect(1, 1, rcPaint.Width(), rcPaint.Height(), m_clrPaint);

	//加载图片资源
	BITMAP bitmap;
	m_srcBitmap.GetBitmap(&bitmap);

	//画指向箭头
	CPen Yellowpen, *Oldpen;
	Yellowpen.CreatePen(PS_SOLID, 1, RGB(255,255,0));
	Oldpen = pMemDC->SelectObject(&Yellowpen);

	CBrush YellowBr;
	YellowBr.CreateSolidBrush(RGB(200, 210, 40));

	CRect rcArrow = rcClient;
	rcArrow.OffsetRect(-rcArrow.left, -rcArrow.top);
	CPoint ptCenter = rcArrow.CenterPoint();
	CRgn rgnArrow;
	CPoint ptArray[7];
	ptArray[0] = CPoint(ptCenter.x-20, ptCenter.y-6);
	ptArray[1] = CPoint(ptCenter.x+15, ptCenter.y-6);
	ptArray[2] = CPoint(ptCenter.x+15, ptCenter.y-15);
	ptArray[3] = CPoint(ptCenter.x+30, ptCenter.y);	
	ptArray[4] = CPoint(ptCenter.x+15, ptCenter.y+15);
	ptArray[5] = CPoint(ptCenter.x+15, ptCenter.y+6);
	ptArray[6] = CPoint(ptCenter.x-20, ptCenter.y+6);

	rgnArrow.CreatePolygonRgn(ptArray, 7, ALTERNATE);

	pMemDC->FillRgn(&rgnArrow, &YellowBr);

	//输出DC内容
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pMemDC, 0, 0, SRCCOPY);

	CDC *pBitmapDC = new CDC;
	pBitmapDC->CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = pBitmapDC->SelectObject(&m_srcBitmap);

	pDC->BitBlt(10, int((rcClient.Height()-bitmap.bmHeight)*0.5), bitmap.bmWidth, bitmap.bmHeight, pBitmapDC, 0, 0, SRCCOPY);

	pBitmapDC->SelectObject(&m_BluredBitmap);

	pDC->BitBlt(rcClient.Width()-10-bitmap.bmWidth, int((rcClient.Height()-bitmap.bmHeight)*0.5), bitmap.bmWidth, bitmap.bmHeight, pBitmapDC, 0, 0, SRCCOPY);

	pBitmapDC->SelectObject(pOldBitmap);

	pBitmapDC->DeleteDC();
	delete pBitmapDC;
	pBitmapDC = NULL;

	//还原DC
	pMemDC->SelectObject(Oldpen);
	pMemDC->SelectObject(pOldbmp);
	membmp.DeleteObject();

	//释放GDI资源
	pMemDC->DeleteDC();
	delete pMemDC;
	pMemDC = NULL;

	ReleaseDC(pDC);
}


DWORD CMGFilterDlg::GaussFilterProcess(CBitmap &srcBitmap)
{
	return 0;
	//位图数据缓存
	BITMAP bitmap;
	srcBitmap.GetBitmap(&bitmap);

	g_lBmpSize = bitmap.bmWidthBytes*bitmap.bmHeight;
	g_nBmpWidth = bitmap.bmWidth;
	g_nBmpHeight = bitmap.bmHeight;

	HLOCAL hMem=LocalAlloc(LHND, g_lBmpSize);
	DWORD* pBuffer=(DWORD*)LocalLock(hMem);

	HBITMAP hBitmap = (HBITMAP)srcBitmap.GetSafeHandle();

	//获取位图原始数据
	LONG lCopySize=::GetBitmapBits(hBitmap, g_lBmpSize, pBuffer);
	HLOCAL hMemCopy=LocalAlloc(LHND, g_lBmpSize);
	DWORD* pBufferCopy=(DWORD*)LocalLock(hMemCopy);

	//高斯滤波
	GaussFilter(pBuffer, pBufferCopy, g_nBmpWidth, g_nBmpHeight, m_nFilterRadius);

	//赋予新的位图数据
	::SetBitmapBits(hBitmap, g_lBmpSize, pBufferCopy);


	LocalUnlock(hMemCopy);
	LocalFree(hMemCopy);
	
	return 0;
}


void CMGFilterDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->IsKindOf(RUNTIME_CLASS(CSliderCtrl)))
	{
		int nPos = m_SliderBlur.GetPos();

		m_nFilterRadius = nPos;

		CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();
		int nPort = pDlg->m_boardInfo.GetBoardPort();
		theApp.MG_SetFilterValue(m_nFilterRadius, nPort);

		//m_BluredBitmap.DeleteObject();
		//DeleteObject(g_hSrcBitmap);

		//m_BluredBitmap.LoadBitmap(IDB_BITMAP_BLUR);

		//CCmdTarget::BeginWaitCursor();
		//GaussFilterProcess(m_BluredBitmap);
		//CCmdTarget::EndWaitCursor();

		UpdateData(FALSE);
		Invalidate();
	}

	CXTPResizeDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMGFilterDlg::OnDeltaposSpinBlurRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	int nVal = m_nFilterRadius - pNMUpDown->iDelta;

	if (nVal > 29) nVal = 29;
	if (nVal < 0) nVal = 0;

	m_nFilterRadius = nVal;
	m_SliderBlur.SetPos(m_nFilterRadius);

	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();
	int nPort = pDlg->m_boardInfo.GetBoardPort();
	theApp.MG_SetFilterValue(m_nFilterRadius, nPort);

	//m_BluredBitmap.DeleteObject();
	//DeleteObject(g_hSrcBitmap);

	//m_BluredBitmap.LoadBitmap(IDB_BITMAP_BLUR);

	//CCmdTarget::BeginWaitCursor();
	//GaussFilterProcess(m_BluredBitmap);
	//CCmdTarget::EndWaitCursor();

	UpdateData(FALSE);
	Invalidate();

	*pResult = 0;
}

void CMGFilterDlg::OnOK()
{
	//CXTPResizeDialog::OnOK();
}

void CMGFilterDlg::OnCancel()
{
	//CXTPResizeDialog::OnCancel();
}


void CMGFilterDlg::Initialize()
{
	UINT nFilter = 0;

	CMGeoMainDlg *pOwer =(CMGeoMainDlg*)GetOwner();
	int nPort = pOwer->m_boardInfo.GetBoardPort();

	if (theApp.MG_GetFilterValue(nFilter, nPort) == CMD_FAILURE)
	{
		return (void)MessageBox(_T("错误！读取钝化值失败！"), _T("图像钝化初始化"), MB_OK|MB_ICONSTOP);
	}

	if (nFilter > 29 || nFilter<0) nFilter = 0;

	m_nFilterRadius = nFilter;

	m_SliderBlur.SetPos(m_nFilterRadius);

	UpdateData(FALSE);
}