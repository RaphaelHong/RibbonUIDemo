// MGgridwarpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGgridwarpDlg.h"
#include "MGeoMainDlg.h"

const CString strGridWarpTipText[] = {
	_T("用鼠标左键按住网格中四个角上的点，然后拖动鼠标！"),
	_T("请点击鼠标右键，开始选取四点区域！"),
	_T("用鼠标左键点击一个网格点，点所在的[行]会高亮显示！"),
	_T("用鼠标左键点击一个网格点，点所在的[列]会高亮显示！"),
	_T("用鼠标左键点击一个网格点，点所在的[行]会高亮显示！"),
	_T("用鼠标左键点击一个网格点，点所在的[列]会高亮显示！"),
	_T("用鼠标左键点击一个网格点，然后拖动鼠标！")
};

// CMGgridwarpDlg 对话框

IMPLEMENT_DYNAMIC(CMGgridwarpDlg, CXTPResizeDialog)

CMGgridwarpDlg::CMGgridwarpDlg(CWnd* pParent /*=NULL*/)
: CXTPResizeDialog(CMGgridwarpDlg::IDD, pParent)
, m_nSmooth(0)
{
	//初始化指针
	m_pDpaGridWarpPoints = NULL;

	//默认校正方式
	m_TrimMode = eStyle_Keystone;

	//默认当前控制点号
	m_ptCursor = CPoint(0, 0);

	//工作区偏移量
	m_nDeflate = 5;

	//初始化控制变量
	m_bLBtnDownAtGridPoint = FALSE;
	m_bBlanceLeft          = FALSE;
	m_bBlanceUpper         = FALSE;
	m_bBlanceRight         = FALSE;
	m_bBlanceDown          = FALSE;

	m_bLBtnDown            = FALSE;
	m_bMouseMoving         = FALSE;
	m_bBeginSelectKeyRect  = FALSE;

	m_ptBegin.SetPoint(-1,-1);
	m_ptEnd.SetPoint(-1,-1);

	m_CursorOffsetX = 0.0;
	m_CursorOffsetY = 0.0;

	//提示信息颜色
	m_clrTipText = RGB(240, 150, 240);
	m_nColorCount = 100;
	m_bDecAsc = TRUE;
}

CMGgridwarpDlg::~CMGgridwarpDlg()
{
	if (m_pDpaGridWarpPoints != NULL)
	{
		delete m_pDpaGridWarpPoints;
		m_pDpaGridWarpPoints = NULL;
	}
}

void CMGgridwarpDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GBOX_FUNC_CTRL, m_GboxFuncCtrl);
	DDX_Control(pDX, IDC_GBOX_AJUST_CTRL, m_GboxAjustCtrl);
	DDX_Control(pDX, IDC_GBOX_BLANCE_CTRL, m_GboxBlanceCtrl);
	DDX_Control(pDX, IDC_GBOX_CTRL_PT_COORDS, m_GboxCursorPosCtrl);
	DDX_Control(pDX, IDC_GBOX_GRIDLINES_CTRL, m_GboxGridLinesCtrl);

	DDX_Control(pDX, IDC_BTN_LOAD_GEOMETRIC_DATA, m_BtnLoadGeoData);
	DDX_Control(pDX, IDC_BTN_UNDO_GRIDWARP, m_BtnUndoGrid);
	DDX_Control(pDX, IDC_BTN_REDO_GRIDWARP, m_BtnRedoGrid);
	DDX_Control(pDX, IDC_BTN_RESET_GRIDWARP, m_BtnResetGrid);

	DDX_Control(pDX, IDC_BTN_SWITCH_AJUST_MODE, m_BtnSwitchAjustMode);
	DDX_Control(pDX, IDC_CHECK_BLANCE_ALL_DIRECTIONS, m_CheckBlanceAll);
	DDX_Control(pDX, IDC_CHECK_BLANCE_LEFT, m_CheckBlanceLeft);
	DDX_Control(pDX, IDC_CHECK_BLANCE_UPPER, m_CheckBlanceUpper);
	DDX_Control(pDX, IDC_CHECK_BLANCE_RIGHT, m_CheckBlanceRight);
	DDX_Control(pDX, IDC_CHECK_BLANCE_DOWN, m_CheckBlanceDown);

	DDX_Control(pDX, IDC_CHECK_GRID_SMOOTH, m_CheckSmoothGrid);
	DDX_Control(pDX, IDC_BTN_LOAD_AND_SAVE, m_BtnSaveAndLoad);

	DDX_Control(pDX, IDC_EDIT_CTRL_PT_X, m_EditCursorPosX);
	DDX_Control(pDX, IDC_EDIT_CTRL_PT_Y, m_EditCursorPosY);
	DDX_Control(pDX, IDC_COMBO_MOVE_INCREMENT, m_CBoxIncrement);

	DDX_Control(pDX, IDC_BTN_DOUBLE_GRID_ROW, m_BtnDoubleGridRow);
	DDX_Control(pDX, IDC_BTN_HALF_GRID_ROW, m_BtnHalfGridRow);
	DDX_Control(pDX, IDC_BTN_DOUBLE_GRID_COL, m_BtnDoubleGridCol);
	DDX_Control(pDX, IDC_BTN_HALF_GRID_COL, m_BtnHalfGridCol);

	DDX_Check(pDX, IDC_CHECK_GRID_SMOOTH, m_nSmooth);
}


BEGIN_MESSAGE_MAP(CMGgridwarpDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_CBN_DROPDOWN(IDC_BTN_SWITCH_AJUST_MODE, OnBtnAjustStyleDropDown)
	ON_CBN_DROPDOWN(IDC_BTN_LOAD_AND_SAVE, OnBtnLoadAndSaveDropDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND_RANGE(ID_GW_STYLE_KEYSTONE, ID_GW_STYLE_SINGLEPOINT, OnCommandAjustStyle)
	ON_COMMAND_RANGE(ID_READ_LOCAL_GRIDFILE, ID_SAVETO_DEVICE_GRIDDATA, OnCommandLoadAndSave)
	ON_BN_CLICKED(IDC_CHECK_BLANCE_ALL_DIRECTIONS, OnCheckBlanceAllDirections)
	ON_BN_CLICKED(IDC_CHECK_BLANCE_LEFT, OnCheckBlanceLeft)
	ON_BN_CLICKED(IDC_CHECK_BLANCE_UPPER, OnCheckBlanceUpper)
	ON_BN_CLICKED(IDC_CHECK_BLANCE_RIGHT, OnCheckBlanceRight)
	ON_BN_CLICKED(IDC_CHECK_BLANCE_DOWN, OnCheckBlanceDown)
	ON_BN_CLICKED(IDC_CHECK_GRID_SMOOTH, OnCheckGridSmooth)
	ON_BN_CLICKED(IDC_BTN_RESET_GRIDWARP, OnBtnClickedReset)
	ON_BN_CLICKED(IDC_BTN_UNDO_GRIDWARP, OnBtnClickedUndo)
	ON_BN_CLICKED(IDC_BTN_REDO_GRIDWARP, OnBtnClickedRedo)
	ON_BN_CLICKED(IDC_BTN_LOAD_GEOMETRIC_DATA, OnBtnClickedLoadGeoData)
	ON_BN_CLICKED(IDC_BTN_DOUBLE_GRID_ROW, OnBtnClickedDoubleRows)
	ON_BN_CLICKED(IDC_BTN_HALF_GRID_ROW, OnBtnClickedHalfRows)
	ON_BN_CLICKED(IDC_BTN_DOUBLE_GRID_COL, OnBtnClickedDoubleCols)
	ON_BN_CLICKED(IDC_BTN_HALF_GRID_COL, OnBtnClickedHalfCols)
	ON_COMMAND(ID_KEYSTONE_SELECT, OnKeystoneSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_MOVE_INCREMENT, OnCbnSelChangeIncrement)
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMGgridwarpDlg 消息处理程序

BOOL CMGgridwarpDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	//创建背景画刷
	m_BkgBrush.CreateSolidBrush(constClrDlgBk[6]);

	//调节窗口和控件位置大小
	ResizeIt();

	//设置控件风格
	SetControlTheme(xtpControlThemeResource);

	//设置按钮图标
	SetButtonIcons();

	//设置提示信息
	SetToolTips();

	//获取绘图区域
	GetPaintRect();

	// 初始化点阵数据
	InitDpaData();

	// 默认的校正方式为梯形校正
	m_BtnSwitchAjustMode.SetWindowText(_T("梯形校正"));
	m_GboxBlanceCtrl.ShowWindow(SW_HIDE);
	m_CheckBlanceAll.ShowWindow(SW_HIDE);
	m_CheckBlanceLeft.ShowWindow(SW_HIDE);
	m_CheckBlanceUpper.ShowWindow(SW_HIDE);
	m_CheckBlanceRight.ShowWindow(SW_HIDE);
	m_CheckBlanceDown.ShowWindow(SW_HIDE);

	// 默认的移动增量为0.5个物理像素
	m_CBoxIncrement.SetCurSel(0);
	m_dIncrement = 0.5;

	SetTimer(1, 100, NULL);

	return TRUE;
}

void CMGgridwarpDlg::ResizeIt()
{
	//绘图控件
	SetResize(IDC_PIC_GRIDWARP_VIEW, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	//功能性按钮
	SetResize(IDC_GBOX_FUNC_CTRL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);

	//校正控制按钮
	SetResize(IDC_GBOX_AJUST_CTRL, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_GBOX_BLANCE_CTRL,XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_BTN_SWITCH_AJUST_MODE, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_CHECK_BLANCE_ALL_DIRECTIONS, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_CHECK_BLANCE_LEFT, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_CHECK_BLANCE_UPPER, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_CHECK_BLANCE_RIGHT, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_CHECK_BLANCE_DOWN, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);
	SetResize(IDC_CHECK_GRID_SMOOTH, XTP_ANCHOR_MIDDLELEFT, XTP_ANCHOR_MIDDLELEFT);

	//加载与保存
	SetResize(IDC_BTN_LOAD_AND_SAVE, XTP_ANCHOR_BOTTOMLEFT, XTP_ANCHOR_BOTTOMLEFT);

	//当前点坐标
	SetResize(IDC_GBOX_CTRL_PT_COORDS, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_TEXT_CTRL_PT_X, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_TEXT_CTRL_PT_Y, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_EDIT_CTRL_PT_X, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_EDIT_CTRL_PT_Y, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_TEXT_MOVE_INCREMENT, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_COMBO_MOVE_INCREMENT,XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);

	//行列控制
	SetResize(IDC_GBOX_GRIDLINES_CTRL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_TEXT_GRID_ROW, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_TEXT_GRID_COLUMN, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_DOUBLE_GRID_ROW, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_HALF_GRID_ROW, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_DOUBLE_GRID_COL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_BTN_HALF_GRID_COL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
}


void CMGgridwarpDlg::SetControlTheme(XTPControlTheme theme)
{
	m_GboxFuncCtrl.SetBorderStyle(xtpGroupBoxSingleLine);
	m_GboxAjustCtrl.SetBorderStyle(xtpGroupBoxSingleLine);

	m_BtnLoadGeoData.SetTheme(theme);
	m_BtnUndoGrid.SetTheme(theme);
	m_BtnRedoGrid.SetTheme(theme);
	m_BtnResetGrid.SetTheme(theme);

	m_BtnSwitchAjustMode.SetTheme(theme);
	m_BtnSwitchAjustMode.SetPushButtonStyle(xtpButtonDropDown);
	m_CheckBlanceAll.SetTheme(theme);
	m_CheckBlanceLeft.SetTheme(theme);
	m_CheckBlanceUpper.SetTheme(theme);
	m_CheckBlanceRight.SetTheme(theme);
	m_CheckBlanceDown.SetTheme(theme);
	m_CheckSmoothGrid.SetTheme(theme);

	m_BtnSaveAndLoad.SetTheme(theme);
	m_BtnSaveAndLoad.SetPushButtonStyle(xtpButtonDropDown);

	m_CBoxIncrement.SetTheme(theme);

	m_BtnDoubleGridRow.SetTheme(theme);
	m_BtnHalfGridRow.SetTheme(theme);
	m_BtnDoubleGridCol.SetTheme(theme);
	m_BtnHalfGridCol.SetTheme(theme);
}

void CMGgridwarpDlg::SetButtonIcons()
{
	m_BtnLoadGeoData.SetIcon(CSize(16,16), IDI_ICON_CONVERT, IDI_ICON_CONVERT);
	m_BtnUndoGrid.SetIcon(CSize(16,16), IDI_ICON_UNDO, IDI_ICON_UNDO);
	m_BtnRedoGrid.SetIcon(CSize(16,16), IDI_ICON_REDO, IDI_ICON_REDO);
	m_BtnResetGrid.SetIcon(CSize(16,16), IDI_ICON_RESET, IDI_ICON_RESET);

	m_BtnSwitchAjustMode.SetIcon(CSize(16,16), IDI_ICON_AJUST_TYPE, IDI_ICON_AJUST_TYPE);

	m_BtnSaveAndLoad.SetIcon(CSize(16,16), IDI_ICON_SAVE, IDI_ICON_SAVE);
	m_BtnDoubleGridRow.SetIcon(CSize(16,16), IDI_ICON_DOUBLE_ROW, IDI_ICON_DOUBLE_ROW);
	m_BtnHalfGridRow.SetIcon(CSize(16,16), IDI_ICON_HALF_ROW, IDI_ICON_HALF_ROW);
	m_BtnDoubleGridCol.SetIcon(CSize(16,16), IDI_ICON_DOUBLE_COL, IDI_ICON_DOUBLE_COL);
	m_BtnHalfGridCol.SetIcon(CSize(16,16), IDI_ICON_HALF_COL, IDI_ICON_HALF_COL);
}

void CMGgridwarpDlg::SetToolTips()
{
	m_BtnLoadGeoData.SetTooltip(_T("加载几何校正数据"));
	m_BtnUndoGrid.SetTooltip(_T("撤销"));
	m_BtnRedoGrid.SetTooltip(_T("重复"));
	m_BtnResetGrid.SetTooltip(_T("复位"));

	m_BtnSwitchAjustMode.SetTooltip(_T("点击切换校正方式"));

	m_CheckSmoothGrid.SetTooltip(_T("平滑图像，图像断层不明显"));

	m_BtnSaveAndLoad.SetTooltip(_T("加载和保存选项"));
	m_BtnDoubleGridRow.SetTooltip(_T("行倍增"));
	m_BtnDoubleGridCol.SetTooltip(_T("列倍增"));
	m_BtnHalfGridRow.SetTooltip(_T("行倍减"));
	m_BtnHalfGridCol.SetTooltip(_T("列倍减"));
}

void CMGgridwarpDlg::InitDpaData()
{
	if (m_pDpaGridWarpPoints == NULL)
	{
		m_pDpaGridWarpPoints = new CDoublePointArray(m_gridsetting.cx, m_gridsetting.cy);

		ResetDpaData();

		AddRecord();

		m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

		ShowCursorCoords();
	}
}

CDoublePoint CMGgridwarpDlg::OrignalPos(int x, int y) const
{
	ASSERT( x < m_gridsetting.cx && y < m_gridsetting.cy );
	double xVal = double(x * m_gridsetting.cxResIn) / double(m_gridsetting.cx-1);
	double yVal = double(y * m_gridsetting.cyResIn) / double(m_gridsetting.cy-1);
	return CDoublePoint(xVal,yVal);
}

void CMGgridwarpDlg::ResetDpaData()
{
	for (int y=0; y<m_gridsetting.cy; y++)	{
		for (int x=0; x<m_gridsetting.cx; x++)	{
			m_pDpaGridWarpPoints->X(x, y) = OrignalPos(x, y).x;
			m_pDpaGridWarpPoints->Y(x, y) = OrignalPos(x, y).y;
		}
	}
}


void CMGgridwarpDlg::GetPaintRect()
{
	GetDlgItem(IDC_PIC_GRIDWARP_VIEW)->GetWindowRect(&m_rcPaint);
	ScreenToClient(&m_rcPaint);

	m_rcChart = m_rcPaint;
	m_rcChart.OffsetRect(-m_rcChart.left, -m_rcChart.top);
	m_rcChart.InflateRect(-m_nDeflate, -m_nDeflate);

	SetGraphicSize();
}

void CMGgridwarpDlg::SetGraphicSize()
{
	// 设置校正相关的属性信息
	m_gridsetting.cxComplexCurveGraphSize = m_rcChart.Width();
	m_gridsetting.cyComplexCurveGraphSize = m_rcChart.Height();

	m_xScale = double(m_rcChart.Width()) / double(m_gridsetting.cxResIn);
	m_yScale = double(m_rcChart.Height()) / double(m_gridsetting.cyResIn);
}

void CMGgridwarpDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if (GetDlgItem(IDC_PIC_GRIDWARP_VIEW)->GetSafeHwnd()) GetPaintRect();
}

HBRUSH CMGgridwarpDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return HBRUSH(m_BkgBrush);
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(m_BkgBrush);
	}
	else if (nCtlColor == CTLCOLOR_BTN)
	{
		return HBRUSH(m_BkgBrush);
	}

	return hbr;
}

void CMGgridwarpDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// 获取绘图DC
	CDC *pDC = GetDlgItem(IDC_PIC_GRIDWARP_VIEW)->GetDC();

	// 创建内存DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	// 选择空位图
	CBitmap Bmp, *pOldBmp;
	Bmp.CreateCompatibleBitmap(pDC, m_rcPaint.Width(), m_rcPaint.Height());
	pOldBmp = pMemDC->SelectObject(&Bmp);

	// 上背景色
	pMemDC->FillSolidRect(0, 0, m_rcPaint.Width(), m_rcPaint.Height(), RGB(205,225,255));

	// 画背景网格
	DrawBkgGrid(pMemDC);

	// 画分辨率
	DrawResolution(pMemDC);

	// 画提示文本
	if (!m_bMouseMoving) DrawTipText(pMemDC);

	// 画数据
	DrawDataGrid(pMemDC);

	// 画鼠标轮廓线
	if (m_bBeginSelectKeyRect && m_bLBtnDown)
		DrawSelectRectangle(pMemDC, m_ptBegin, m_ptEnd);

	// 将内存DC拷贝到绘图DC中
	pDC->BitBlt(0, 0, m_rcPaint.Width(), m_rcPaint.Height(), pMemDC, 0, 0, SRCCOPY);

	// 释放资源
	pMemDC->SelectObject(pOldBmp);
	pMemDC->DeleteDC();
	delete pMemDC;

	ReleaseDC(pDC);
}

void CMGgridwarpDlg::DrawBkgGrid(CDC *pValidDC)
{
	//给工作区上背景色
	pValidDC->FillSolidRect(m_rcChart, RGB(50,30,60));

	CPen GoldPen, *pOldpen;
	GoldPen.CreatePen(PS_DOT, 1, RGB(34, 50, 170)); //RGB(194,200,40)
	pOldpen = pValidDC->SelectObject(&GoldPen);

	double dScaleX = double(m_rcChart.Width()) / 16;
	double dScaleY = double(m_rcChart.Height())/ 16;

	// 背景虚线网格
	for (int i=0; i<17; i++) {
		pValidDC->MoveTo(m_rcChart.left, m_rcChart.top+Round(i*dScaleY));
		pValidDC->LineTo(m_rcChart.right, m_rcChart.top+Round(i*dScaleY));

		pValidDC->MoveTo(m_rcChart.left+Round(dScaleX*i), m_rcChart.top);
		pValidDC->LineTo(m_rcChart.left+Round(dScaleX*i), m_rcChart.bottom);
	}

	pValidDC->SelectObject(pOldpen);
}

void CMGgridwarpDlg::DrawDataGrid(CDC *pValidDC)
{
	CPen Redpen, Greenpen, Graypen, *pOldpen;
	Redpen.CreatePen(PS_SOLID, 2, RGB(255,50,30));
	Greenpen.CreatePen(PS_SOLID, 2, RGB(50,255,30));
	Graypen.CreatePen(PS_SOLID, 1, RGB(147,186,138));//RGB(194,200,40)

	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//中间网格线 灰白
	pOldpen = pValidDC->SelectObject(&Graypen);
	//纵向连线
	for (int x=1; x<cx; x++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(x, 0)*m_xScale), m_nDeflate+Round(m_pDpaGridWarpPoints->Y(x, 0)*m_yScale));
		for (int y=1; y<cy; y++)
			DrawLine(pValidDC, CPoint(x, y));
	}
	//横向连线
	for (int y=1; y<cy; y++)	{
		pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(0, y)*m_xScale), m_nDeflate+Round(m_pDpaGridWarpPoints->Y(0, y)*m_yScale));
		for (int x=1; x<cx; x++)
			DrawLine(pValidDC, CPoint(x, y));
	}	

	//上轮廓线
	pValidDC->SelectObject(&Redpen);
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(0,0)*m_xScale), m_nDeflate+Round(m_pDpaGridWarpPoints->Y(0,0)*m_yScale));
	for (int x=1; x<cx; x++) DrawLine(pValidDC, CPoint(x, 0));

	//右轮廓线
	pValidDC->SelectObject(&Greenpen);
	for (int y=1; y<cy; y++) DrawLine(pValidDC, CPoint(cx-1, y));

	//下轮廓线
	pValidDC->SelectObject(&Redpen);
	for (int x=cx-2; x>=0; x--) DrawLine(pValidDC, CPoint(x, cy-1));

	//左轮廓线
	pValidDC->SelectObject(&Greenpen);
	for (int y=cy-2; y>=0; y--) DrawLine(pValidDC, CPoint(0, y));

	//根据当前校正方式画控制点和线
	if (m_TrimMode == eStyle_Keystone)           DrawKeystone(pValidDC);
	else if (m_TrimMode == eStyle_InterKeystone) DrawInterKeystone(pValidDC);
	else if (m_TrimMode == eStyle_InterRow
		|| m_TrimMode == eStyle_FullRow)         DrawOneRow(pValidDC);
	else if (m_TrimMode == eStyle_InterCol
		|| m_TrimMode == eStyle_FullCol)         DrawOneCol(pValidDC);
	else if (m_TrimMode == eStyle_SinglePoint)   DrawCrossLine(pValidDC);

	pValidDC->SelectObject(pOldpen);
}

void CMGgridwarpDlg::DrawKeystone(CDC *pValidDC)
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//默认四个控制点
	CBrush br, brCtrl, *pOldBr;
	br.CreateSolidBrush(RGB(255,255,100));
	brCtrl.CreateSolidBrush(RGB(100,100,255));
	int nRadius = 7; //控制圆点半径大小
	//左上角
	pOldBr = pValidDC->SelectObject((m_ptCursor == CPoint(0,0)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(0,0), nRadius);

	//右上角
	pValidDC->SelectObject((m_ptCursor == CPoint(cx-1, 0)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(cx-1, 0), nRadius);

	//左下角
	pValidDC->SelectObject((m_ptCursor == CPoint(0, cy-1)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(0, cy-1), nRadius);

	//右下角
	pValidDC->SelectObject((m_ptCursor == CPoint(cx-1, cy-1)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(cx-1, cy-1), nRadius);

	pValidDC->SelectObject(pOldBr);
}

void CMGgridwarpDlg::DrawInterKeystone(CDC *pValidDC)
{
	//选取的区域为空区域
	if (m_KeyRect.IsRectEmpty()) return;

	CPen /*bluePen,*/ whitePen, *pOldPen;
	//bluePen.CreatePen(PS_SOLID, 1, RGB(180,181,185)); //RGB(14,35,140)
	whitePen.CreatePen(PS_SOLID, 2, RGB(251,252,254));
	pOldPen = pValidDC->SelectObject(&whitePen);

	//画区域四点连线
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(m_KeyRect.left, m_KeyRect.top)*m_xScale),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(m_KeyRect.left, m_KeyRect.top)*m_yScale));
	for (int x=m_KeyRect.left+1; x<=m_KeyRect.right; x++)
		DrawLine(pValidDC, CPoint(x, m_KeyRect.top));

	for (int y=m_KeyRect.top+1; y<=m_KeyRect.bottom; y++)
		DrawLine(pValidDC, CPoint(m_KeyRect.right, y));

	for (int x=m_KeyRect.right-1; x>=m_KeyRect.left; x--)
		DrawLine(pValidDC, CPoint(x, m_KeyRect.bottom));

	for (int y=m_KeyRect.bottom-1; y>=m_KeyRect.top; y--)
		DrawLine(pValidDC, CPoint(m_KeyRect.left, y));


	//画区域四控制点
	CBrush br, brCtrl, *pOldBr;
	br.CreateSolidBrush(RGB(255,255,100));
	brCtrl.CreateSolidBrush(RGB(100,100,255));
	int nRadius = 7; //控制圆点半径大小
	pOldBr = pValidDC->SelectObject(&br);
	//左上角
	pOldBr = pValidDC->SelectObject(m_ptCursor == CPoint(m_KeyRect.left, m_KeyRect.top) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(m_KeyRect.left, m_KeyRect.top), nRadius);

	//右上角
	pValidDC->SelectObject((m_ptCursor == CPoint(m_KeyRect.right, m_KeyRect.top)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(m_KeyRect.right, m_KeyRect.top), nRadius);

	//左下角
	pValidDC->SelectObject((m_ptCursor == CPoint(m_KeyRect.left, m_KeyRect.bottom)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(m_KeyRect.left, m_KeyRect.bottom), nRadius);

	//右下角
	pValidDC->SelectObject((m_ptCursor == CPoint(m_KeyRect.right, m_KeyRect.bottom)) ? &brCtrl : &br);
	DrawEllipse(pValidDC, CPoint(m_KeyRect.right, m_KeyRect.bottom), nRadius);

	pValidDC->SelectObject(pOldBr);

}

void CMGgridwarpDlg::DrawOneRow(CDC *pValidDC)
{
	DrawHighLightRow(pValidDC);

	//画控制点
	DrawSinglePoint(pValidDC);
}

void CMGgridwarpDlg::DrawHighLightRow(CDC *pValidDC)
{
	CPen bluePen, whitePen, *pOldPen;
	bluePen.CreatePen(PS_SOLID, 1, RGB(180,181,185)); //RGB(14,35,140)
	whitePen.CreatePen(PS_SOLID, 1, RGB(251,252,254));
	pOldPen = pValidDC->SelectObject(&bluePen);

	//中间蓝色线
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(0, m_ptCursor.y)*m_xScale+1),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(0, m_ptCursor.y)*m_yScale));
	for (int x=1; x<m_gridsetting.cx; x++)	{
		DrawLine(pValidDC, CPoint(x, m_ptCursor.y));
	}

	//两条白色线将蓝线包围在中间
	pValidDC->SelectObject(&whitePen);
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(0, m_ptCursor.y)*m_xScale+1),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(0, m_ptCursor.y)*m_yScale)-1);
	for (int x=1; x<m_gridsetting.cx; x++)	{
		DrawLine(pValidDC, CPoint(x, m_ptCursor.y), 0, -1);
	}

	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(0, m_ptCursor.y)*m_xScale+1),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(0, m_ptCursor.y)*m_yScale)+1);
	for (int x=1; x<m_gridsetting.cx; x++)	{
		DrawLine(pValidDC, CPoint(x, m_ptCursor.y), 0, 1);
	}

	pValidDC->SelectObject(pOldPen);
}

void CMGgridwarpDlg::DrawOneCol(CDC *pValidDC)
{
	DrawHighLightCol(pValidDC);

	//画控制点
	DrawSinglePoint(pValidDC);
}

void CMGgridwarpDlg::DrawHighLightCol(CDC *pValidDC)
{
	CPen bluePen, whitePen, *pOldPen;
	bluePen.CreatePen(PS_SOLID, 1, RGB(180,181,185)); //RGB(14,35,140)
	whitePen.CreatePen(PS_SOLID, 1, RGB(251,252,254));
	pOldPen = pValidDC->SelectObject(&bluePen);

	//中间蓝色线
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(m_ptCursor.x, 0)*m_xScale),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(m_ptCursor.x, 0)*m_yScale)+1);
	for (int y=1; y<m_gridsetting.cy; y++)	{
		DrawLine(pValidDC, CPoint(m_ptCursor.x, y));
	}

	//两条白色线将蓝色线包围在中间
	pValidDC->SelectObject(&whitePen);
	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(m_ptCursor.x, 0)*m_xScale-1),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(m_ptCursor.x, 0)*m_yScale)+1);
	for (int y=1; y<m_gridsetting.cy; y++)	{
		DrawLine(pValidDC, CPoint(m_ptCursor.x, y), -1, 0);
	}

	pValidDC->MoveTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(m_ptCursor.x, 0)*m_xScale+1),
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(m_ptCursor.x, 0)*m_yScale)+1);
	for (int y=1; y<m_gridsetting.cy; y++)	{
		DrawLine(pValidDC, CPoint(m_ptCursor.x, y), 1, 0);
	}

	pValidDC->SelectObject(pOldPen);
}

void CMGgridwarpDlg::DrawSinglePoint(CDC *pValidDC)
{
	//画单个控制圆点
	CBrush brCtrl, *pOldBr;
	brCtrl.CreateSolidBrush(RGB(100,100,255));
	pOldBr = pValidDC->SelectObject(&brCtrl);
	int nRadius = 7;

	DrawEllipse(pValidDC, CPoint(m_ptCursor.x, m_ptCursor.y), nRadius);

	pValidDC->SelectObject(pOldBr);
}

void CMGgridwarpDlg::DrawCrossLine(CDC *pValidDC)
{
	DrawHighLightRow(pValidDC);
	DrawHighLightCol(pValidDC);
	DrawSinglePoint(pValidDC);
}

void CMGgridwarpDlg::DrawResolution(CDC *pValidDC)
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
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	strRes.Format(_T("%dx%d @ %dHz"), m_gridsetting.cxResIn, m_gridsetting.cyResIn, m_gridsetting.nHz);

	CSize txtExt;
	txtExt = pValidDC->GetTextExtent(strRes);

	CPoint ptMark;
	ptMark.x = m_rcPaint.CenterPoint().x - int(txtExt.cx*0.5);
	ptMark.y = m_rcPaint.CenterPoint().y - int(txtExt.cy*0.5);
	ptMark.Offset(-m_rcPaint.left, -m_rcPaint.top);

	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(255,255,200));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	ptMark.Offset(1,1);
	pValidDC->SetTextColor(RGB(178,145,240));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	pValidDC->SelectObject(pOldFont);
}

void CMGgridwarpDlg::DrawTipText(CDC *pValidDC)
{
	int nFontHeight;
	if (m_rcPaint.Width() > 800) nFontHeight = 40;
	else if (m_rcPaint.Width() <= 800 && m_rcPaint.Width() >= 600) nFontHeight = 30;
	else nFontHeight = 20;
	CFont markFont, *pOldFont;
	VERIFY(markFont.CreateFont(
		nFontHeight,               // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
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

	CSize txtExt;
	CString strTipText;
	CPoint ptMark;

	if (m_TrimMode==eStyle_InterKeystone && m_bBeginSelectKeyRect)
		strTipText = _T("按住左键，拖动鼠标开始选取四点区域！");
	else 
		strTipText = strGridWarpTipText[int(m_TrimMode)];

	txtExt = pValidDC->GetTextExtent(strTipText);
	ptMark.x = m_rcPaint.CenterPoint().x-int(txtExt.cx*0.5);
	ptMark.y = m_rcPaint.top+50;

	ptMark.Offset(-m_rcPaint.left, -m_rcPaint.top);
	pValidDC->SetTextColor(m_clrTipText);
	pValidDC->TextOut(ptMark.x, ptMark.y, strTipText);

	pValidDC->SelectObject(pOldFont);
}

void CMGgridwarpDlg::DrawSelectRectangle(CDC *dc, CPoint ptBegin, CPoint ptEnd)
{
	if (ptBegin == CPoint(-1,-1) || ptEnd == CPoint(-1,-1)) return;

	//画鼠标轨迹线
	CPen dotPen, *pOldpen;
	dotPen.CreatePen(PS_DOT, 1, RGB(255,225,0));
	pOldpen = dc->SelectObject(&dotPen);

	int nLeft   = min(ptBegin.x, ptEnd.x);
	int nTop    = min(ptBegin.y, ptEnd.y);
	int nRight  = max(ptBegin.x, ptEnd.x);
	int nBottom = max(ptBegin.y, ptEnd.y);

	dc->MoveTo(nLeft, nTop);
	dc->LineTo(nRight, nTop);
	dc->LineTo(nRight, nBottom);
	dc->LineTo(nLeft, nBottom);
	dc->LineTo(nLeft, nTop);

	dc->SelectObject(pOldpen);
}

inline void CMGgridwarpDlg::DrawLine(CDC *dc, CPoint ptLineto, int xInc, int yInc)
{
	dc->LineTo(m_nDeflate+Round(m_pDpaGridWarpPoints->X(ptLineto.x, ptLineto.y)*m_xScale)+xInc,
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(ptLineto.x, ptLineto.y)*m_yScale)+yInc);
}

inline void CMGgridwarpDlg::DrawEllipse(CDC *dc, CPoint ptCenter, int nRadius)
{
	dc->Ellipse(m_nDeflate+Round(m_pDpaGridWarpPoints->X(ptCenter.x, ptCenter.y)*m_xScale)-nRadius,
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(ptCenter.x, ptCenter.y)*m_yScale)-nRadius,
		m_nDeflate+Round(m_pDpaGridWarpPoints->X(ptCenter.x, ptCenter.y)*m_xScale)+nRadius,
		m_nDeflate+Round(m_pDpaGridWarpPoints->Y(ptCenter.x, ptCenter.y)*m_yScale)+nRadius);
}

void CMGgridwarpDlg::OnCheckBlanceAllDirections()
{
	int nChecked = m_CheckBlanceAll.GetCheck();

	//更新控件状态
	m_CheckBlanceLeft.SetCheck(nChecked);
	m_CheckBlanceUpper.SetCheck(nChecked);
	m_CheckBlanceRight.SetCheck(nChecked);
	m_CheckBlanceDown.SetCheck(nChecked);

	//更新控制变量
	m_bBlanceLeft  = nChecked;
	m_bBlanceUpper = nChecked;
	m_bBlanceRight = nChecked;
	m_bBlanceDown  = nChecked;

	// 梯形和区域四点不需要单独均匀处理
	//if (m_TrimMode != eStyle_Keystone
	//	&& m_TrimMode != eStyle_InterKeystone)
	//{
	//	if (m_bBlanceLeft)		BlanceLeft();

	//	if (m_bBlanceUpper)     BlanceUpper();

	//	if (m_CheckBlanceRight) BlanceRight();

	//	if (m_CheckBlanceDown)  BlanceDown();

	//	InvalidateRect(m_rcPaint);
	//}
}

void CMGgridwarpDlg::OnCheckBlanceLeft()
{
	m_bBlanceLeft = m_CheckBlanceLeft.GetCheck();

	if (m_bBlanceLeft)
	{
		if (m_bBlanceLeft && m_bBlanceUpper && m_bBlanceRight && m_bBlanceDown)
			m_CheckBlanceAll.SetCheck(1);
		else
			m_CheckBlanceAll.SetCheck(0);
	}
	else
		m_CheckBlanceAll.SetCheck(0);
}

void  CMGgridwarpDlg::OnCheckBlanceUpper()
{
	m_bBlanceUpper = m_CheckBlanceUpper.GetCheck();

	if (m_bBlanceUpper)
	{
		if (m_bBlanceLeft && m_bBlanceUpper && m_bBlanceRight && m_bBlanceDown)
			m_CheckBlanceAll.SetCheck(1);
		else
			m_CheckBlanceAll.SetCheck(0);
	}
	else
		m_CheckBlanceAll.SetCheck(0);
}

void CMGgridwarpDlg::OnCheckBlanceRight()
{
	m_bBlanceRight = m_CheckBlanceRight.GetCheck();

	if (m_CheckBlanceRight)
	{	
		if (m_bBlanceLeft && m_bBlanceUpper && m_bBlanceRight && m_bBlanceDown)
			m_CheckBlanceAll.SetCheck(1);
		else
			m_CheckBlanceAll.SetCheck(0);
	}
	else
		m_CheckBlanceAll.SetCheck(0);
}

void CMGgridwarpDlg::OnCheckBlanceDown()
{
	m_bBlanceDown = m_CheckBlanceDown.GetCheck();

	if (m_CheckBlanceDown)
	{
		if (m_bBlanceLeft && m_bBlanceUpper && m_bBlanceRight && m_bBlanceDown)
			m_CheckBlanceAll.SetCheck(1);
		else
			m_CheckBlanceAll.SetCheck(0);
	}
	else
		m_CheckBlanceAll.SetCheck(0);
}

void CMGgridwarpDlg::OnCheckGridSmooth()
{
	UpdateData(TRUE);
}

void CMGgridwarpDlg::OnCommandLoadAndSave(UINT nCmdID)
{
	switch(nCmdID)
	{
	case ID_READ_LOCAL_GRIDFILE:
		{
		//加载本地数据
		int nRet = ReadGridFile();
		if (nRet == 0)
			MessageBox(_T("读取文件失败！文件不存在或者数据格式不对！"),_T("加载本地数据"),MB_OK|MB_ICONERROR);
		else if (nRet == 1)
			MessageBox(_T("读取数据成功！"),_T("加载本地数据"),MB_OK|MB_ICONINFORMATION);
		}

		break;
	case ID_SAVETO_LOCAL_GRIDFILE:
		{
		//保存到本地文件
		int nRet = SaveGridFile();
		if (nRet == 0)
			MessageBox(_T("保存文件失败！文件名非法或者存储空间不足！"),_T("保存到本地文件"),MB_OK|MB_ICONERROR);
		else if (nRet == 1)
			MessageBox(_T("保存数据成功！"),_T("保存到本地文件"),MB_OK|MB_ICONINFORMATION);
		}

		break;
	case ID_LOAD_DEVICE_GRIDDATA:
		{
		//加载设备数据
		if (!LoadGridData())
			MessageBox(_T("加载数据失败！通信失败或者设备中没有保存的数据！"),_T("加载设备数据"),MB_OK|MB_ICONERROR);
		else
			MessageBox(_T("加载数据成功！"),_T("加载设备数据"),MB_OK|MB_ICONINFORMATION);
		}

		break;
	case ID_SAVETO_DEVICE_GRIDDATA:
		{
		//保存到设备
		if (!WriteGridData())
			MessageBox(_T("写入数据失败！通信失败或者其他指令错误！"),_T("写入设备数据"),MB_OK|MB_ICONERROR);
		else
			MessageBox(_T("写入数据成功！"),_T("写入设备数据"),MB_OK|MB_ICONINFORMATION);
		}

		break;
	default:break;
	}
}

void CMGgridwarpDlg::OnCommandAjustStyle(UINT nCmdID)
{
	//切换校正方式
	m_TrimMode = emGridWarpStyle(nCmdID-ID_GW_STYLE_KEYSTONE);

	//更新按钮的文本
	CString strBtnFace;
	if (m_TrimMode == eStyle_Keystone)           strBtnFace = _T("梯形校正");
	else if (m_TrimMode == eStyle_InterKeystone) strBtnFace = _T("区域四点");
	else if (m_TrimMode == eStyle_InterRow)      strBtnFace = _T("行内校正");
	else if (m_TrimMode == eStyle_InterCol)      strBtnFace = _T("列内校正");
	else if (m_TrimMode == eStyle_FullRow)       strBtnFace = _T("整行移动");
	else if (m_TrimMode == eStyle_FullCol)       strBtnFace = _T("整列移动");
	else if (m_TrimMode == eStyle_SinglePoint)   strBtnFace = _T("单点移动");

	m_BtnSwitchAjustMode.SetWindowText(strBtnFace);

	//隐藏或显示均匀控件
	m_GboxBlanceCtrl.ShowWindow(m_TrimMode==eStyle_Keystone ? SW_HIDE : SW_SHOW);
	m_CheckBlanceAll.ShowWindow(m_TrimMode==eStyle_Keystone ? SW_HIDE : SW_SHOW);
	m_CheckBlanceLeft.ShowWindow(((m_TrimMode!=eStyle_Keystone) && (m_TrimMode!=eStyle_InterCol) && (m_TrimMode!=eStyle_FullRow)) ? SW_SHOW : SW_HIDE);
	m_CheckBlanceUpper.ShowWindow(((m_TrimMode!=eStyle_Keystone) && (m_TrimMode!=eStyle_InterRow) && (m_TrimMode!=eStyle_FullCol)) ? SW_SHOW : SW_HIDE);
	m_CheckBlanceRight.ShowWindow(((m_TrimMode!=eStyle_Keystone) && (m_TrimMode!=eStyle_InterCol) && (m_TrimMode!=eStyle_FullRow)) ? SW_SHOW : SW_HIDE);
	m_CheckBlanceDown.ShowWindow(((m_TrimMode!=eStyle_Keystone) && (m_TrimMode!=eStyle_InterRow) && (m_TrimMode!=eStyle_FullCol)) ? SW_SHOW : SW_HIDE);

	//从区域四点切换到其他方式
	if (m_TrimMode != eStyle_InterKeystone) m_bBeginSelectKeyRect = FALSE;

	//切换到梯形校正
	if (m_TrimMode == eStyle_Keystone)
	{
		m_ptCursor = CPoint(0, 0);
		m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);
	}

	InvalidateRect(m_rcPaint);
}

void CMGgridwarpDlg::OnBtnAjustStyleDropDown()
{
	//弹出校正方式选择菜单
	CMenu menu;
	menu.LoadMenu(IDR_MENU_GRIDWARP_STYLE);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);


	DWORD dwStyle = m_BtnSwitchAjustMode.GetPushButtonStyle();
	ASSERT(m_BtnSwitchAjustMode.IsDropDownStyle());

	CXTPWindowRect rect(m_BtnSwitchAjustMode);

	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(), TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left, dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom, 0, m_hWnd, 0);

}

void CMGgridwarpDlg::OnBtnLoadAndSaveDropDown()
{
	//弹出加载与保存菜单
	CMenu menu;
	menu.LoadMenu(IDR_MENU_GRIDWARP_SAVE);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	DWORD dwStyle = m_BtnSaveAndLoad.GetPushButtonStyle();
	ASSERT(m_BtnSaveAndLoad.IsDropDownStyle());

	CXTPWindowRect rect(m_BtnSaveAndLoad);

	int nCmd = TrackPopupMenu(pPopup->GetSafeHmenu(), TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
		dwStyle == xtpButtonDropDownRight ? rect.right : rect.left, dwStyle == xtpButtonDropDownRight ? rect.top : rect.bottom, 0, m_hWnd, 0);
}

void CMGgridwarpDlg::OnBtnClickedReset()
{
	//复位
	theApp.GlobleLock();

	ResetDpaData();

	TaskPackage pack;
	Packaging(pack, GRIDWARP_AJUST);
	theApp.PushPackage(pack);

	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock();

	InvalidateRect(m_rcPaint);
}

void CMGgridwarpDlg::OnBtnClickedUndo()
{
	Undo();
}

void CMGgridwarpDlg::OnBtnClickedRedo()
{
	Redo();
}

void CMGgridwarpDlg::OnBtnClickedLoadGeoData()
{
	if (MessageBox(_T("开始转换几何校正数据到多点数据，可能需要等待几秒钟...\r\n点击【是】开始\r\n点击【否】取消"),_T("加载几何校正数据"), MB_YESNO|MB_ICONINFORMATION) == IDNO)
		return;

	CWaitCursor wait;

	//让线程先把没做完的活做完
	theApp.WaitForTaskOver();

	//先将多点校正的网格转换到17*17
	int cx=17;
	int cy=17;
	if (m_gridsetting.cx != cx || m_gridsetting.cy != cy)
	{
		m_gridsetting.cx = cx;
		m_gridsetting.cy = cy;

		delete m_pDpaGridWarpPoints;
		m_pDpaGridWarpPoints = new CDoublePointArray(cx, cy);
	}
	
	// 将几何校正的结果转换到多点校正
	CDoublePoint *InputGridOriginalPoint = new CDoublePoint[cx*cy];
	CDoublePoint *Array4Point = new CDoublePoint[cx*cy];
	CDoublePoint *ArrayCurvePoint = new CDoublePoint[cx*cy];
	CDoublePoint *ArrayDensityPoint = new CDoublePoint[cx*cy];
	CDoublePoint *OutputGridPointMid = new CDoublePoint[cx*cy];
	CDoublePoint *OutputGridPoint = new CDoublePoint[cx*cy];


	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	CDoublePointArray *p4Points ;
	CDoublePointArray *pCurvePoints ;
	CDoublePointArray *pDensityPoints ;
	pOwner->GetGeoDpaDatas(p4Points, pCurvePoints, pDensityPoints);

	// 转换几何校正数据
	for (int y=0; y<m_gridsetting.cy; y++)	{
		for (int x=0; x<m_gridsetting.cx; x++)	{
			InputGridOriginalPoint[x+y*cx].x = OrignalPos(x, y).x;
			InputGridOriginalPoint[x+y*cx].y = OrignalPos(x, y).y;
		}
	}

	memcpy(Array4Point, p4Points->m_pPoints, cx*cy*sizeof(CDoublePoint));
	memcpy(ArrayCurvePoint, pCurvePoints->m_pPoints, cx*cy*sizeof(CDoublePoint));
	memcpy(ArrayDensityPoint, pDensityPoints->m_pPoints, cx*cy*sizeof(CDoublePoint));

	Calc_Keystone_InToOut_Table(cy, cy, InputGridOriginalPoint, Array4Point,
		cx*cy, ArrayCurvePoint, OutputGridPointMid );

	unsigned int min_err_time = 0;
	Calc_Warp_Table(10, cx*cy, InputGridOriginalPoint, OutputGridPointMid,
		&min_err_time, cx*cy, ArrayDensityPoint, OutputGridPoint );

	for (int y=0; y<cy; y++)
	{
		for (int x=0; x<cx; x++)
		{
			m_pDpaGridWarpPoints->X(x, y) = OutputGridPoint[x+y*cx].x;
			m_pDpaGridWarpPoints->Y(x, y) = OutputGridPoint[x+y*cx].y;
		}

	}

	m_ptCursor = CPoint(0,0);
	m_dpaCursorPos = m_pDpaGridWarpPoints->Val(0, 0);

	ShowCursorCoords();
	InvalidateRect(m_rcPaint);

	delete Array4Point;            Array4Point = NULL;
	delete ArrayCurvePoint;        ArrayCurvePoint = NULL;
	delete ArrayDensityPoint;      ArrayDensityPoint = NULL;
	delete InputGridOriginalPoint; InputGridOriginalPoint = NULL;
	delete OutputGridPoint;        OutputGridPoint = NULL;
	delete OutputGridPointMid;     OutputGridPointMid = NULL;
}

//往两行中间插入一行
void CMGgridwarpDlg::OnBtnClickedDoubleRows()
{
	if (m_gridsetting.cy >= 65) return;

	//DeleteMultiPointFile();
	//ResetUndoRedo();

	CWaitCursor wait;

	theApp.WaitForTaskOver();

	int nRows = m_gridsetting.cy;
	int nCols = m_gridsetting.cx;
	int nNewRows = (nRows-1)*2+1;
	CDoublePointArray *pNewDpaData = new CDoublePointArray(nCols, nNewRows);
	CDoublePointArray *pOldDpaData;

	theApp.GlobleLock();

	pOldDpaData = m_pDpaGridWarpPoints;
	for (int x=0; x<nCols; x++)
	{
		for (int y=0; y<nRows; y++)
		{
			pNewDpaData->X(x, y*2) = pOldDpaData->X(x, y); //奇数行
			pNewDpaData->Y(x, y*2) = pOldDpaData->Y(x, y);

			if (y != nRows -1)
			{
				pNewDpaData->X(x, y*2+1) = (pOldDpaData->X(x, y)+pOldDpaData->X(x, y+1))*0.5; //偶数行
				pNewDpaData->Y(x, y*2+1) = (pOldDpaData->Y(x, y)+pOldDpaData->Y(x, y+1))*0.5;
			}
		}
	}

	delete m_pDpaGridWarpPoints;
	m_pDpaGridWarpPoints = new CDoublePointArray(nCols, nNewRows);
	memcpy(m_pDpaGridWarpPoints->m_pPoints, pNewDpaData->m_pPoints, nCols*nNewRows*sizeof(CDoublePoint));	

	m_gridsetting.cx = nCols;
	m_gridsetting.cy = nNewRows;

	theApp.GlobleUnlock();

	//使当前控制点位置在图像上不变
	m_ptCursor.y = m_ptCursor.y == nRows-1 ? nNewRows-1 : m_ptCursor.y*2;
	m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

	if (!m_KeyRect.IsRectEmpty())
	{
		m_KeyRect.top    = m_KeyRect.top*2;
		m_KeyRect.bottom = m_KeyRect.bottom*2;
	}

	InvalidateRect(m_rcPaint);

	delete pNewDpaData;
	pNewDpaData = NULL;
	pOldDpaData = NULL;
}

//删除一半的行
void CMGgridwarpDlg::OnBtnClickedHalfRows()
{
	if (m_gridsetting.cy <= 17) return;

	//DeleteMultiPointFile();
	//ResetUndoRedo();

	CWaitCursor wait;

	theApp.WaitForTaskOver();

	int nRows = m_gridsetting.cy;
	int nCols = m_gridsetting.cx;
	int nNewRows = int((nRows-1)*0.5+1);
	CDoublePointArray *pNewDpaData = new CDoublePointArray(nCols, nNewRows);
	CDoublePointArray *pOldDpaData;

	theApp.GlobleLock();

	pOldDpaData = m_pDpaGridWarpPoints;

	for (int x=0; x<nCols; x++)
	{
		for (int y=0; y<nNewRows; y++)
		{
			pNewDpaData->X(x, y) = pOldDpaData->X(x, 2*y);
			pNewDpaData->Y(x, y) = pOldDpaData->Y(x, 2*y);
		}
	}

	delete m_pDpaGridWarpPoints;
	m_pDpaGridWarpPoints = new CDoublePointArray(nCols, nNewRows);
	memcpy(m_pDpaGridWarpPoints->m_pPoints, pNewDpaData->m_pPoints, nCols*nNewRows*sizeof(CDoublePoint));


	m_gridsetting.cy = nNewRows;
	m_gridsetting.cx = nCols;

	theApp.GlobleUnlock();

	m_ptCursor.y   = (m_ptCursor.y == nRows-1) ? (nNewRows-1) : long(m_ptCursor.y*0.5);
	m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

	if (!m_KeyRect.IsRectEmpty())
	{
		m_KeyRect.top    = Round(m_KeyRect.top*0.5);
		m_KeyRect.bottom = Round(m_KeyRect.bottom*0.5);
	}

	InvalidateRect(m_rcPaint);

	delete pNewDpaData;
	pNewDpaData = NULL;
	pOldDpaData = NULL;
}

//往两列中间插入一列
void CMGgridwarpDlg::OnBtnClickedDoubleCols()
{
	if (m_gridsetting.cx >= 65) return;

	//DeleteMultiPointFile();
	//ResetUndoRedo();

	CWaitCursor wait;

	theApp.WaitForTaskOver();

	int nRows = m_gridsetting.cy;
	int nCols = m_gridsetting.cx;
	int nNewCols = (nCols-1)*2+1;
	CDoublePointArray *pNewDpaData = new CDoublePointArray(nNewCols, nRows);
	CDoublePointArray *pOldDpaData;

	theApp.GlobleLock();

	pOldDpaData = m_pDpaGridWarpPoints;
	for (int y=0; y<nRows; y++)
	{
		for (int x=0; x<nCols; x++)
		{
			pNewDpaData->X(x*2, y) = pOldDpaData->X(x, y); //奇数列
			pNewDpaData->Y(x*2, y) = pOldDpaData->Y(x, y);

			if (x != nCols-1)
			{
				pNewDpaData->X(x*2+1, y) = (pOldDpaData->X(x, y)+pOldDpaData->X(x+1, y))*0.5; //偶数列
				pNewDpaData->Y(x*2+1, y) = (pOldDpaData->Y(x, y)+pOldDpaData->Y(x+1, y))*0.5;
			}
		}
	}

	delete m_pDpaGridWarpPoints;
	m_pDpaGridWarpPoints = new CDoublePointArray(nNewCols, nRows);
	memcpy( m_pDpaGridWarpPoints->m_pPoints, pNewDpaData->m_pPoints, nNewCols*nRows*sizeof(CDoublePoint));	

	theApp.GlobleUnlock();

	m_gridsetting.cx = nNewCols;
	m_gridsetting.cy = nRows;	

	m_ptCursor.x = m_ptCursor.x == nCols-1 ? nNewCols-1 : m_ptCursor.x*2;
	m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

	if (!m_KeyRect.IsRectEmpty())
	{
		m_KeyRect.left  = m_KeyRect.left*2;
		m_KeyRect.right = m_KeyRect.right*2;
	}

	InvalidateRect(m_rcPaint);

	delete pNewDpaData;
	pNewDpaData = NULL;
	pOldDpaData = NULL;
}

//剔除一半的列
void CMGgridwarpDlg::OnBtnClickedHalfCols()
{
	if (m_gridsetting.cx <= 17) return;

	//DeleteMultiPointFile();
	//ResetUndoRedo();

	CWaitCursor wait;

	theApp.WaitForTaskOver();

	int nRows = m_gridsetting.cy;
	int nCols = m_gridsetting.cx;
	int nNewCols = int((nCols-1)*0.5+1);
	CDoublePointArray *pNewDpaData = new CDoublePointArray(nNewCols, nRows);
	CDoublePointArray *pOldDpaData;

	theApp.GlobleLock();


	pOldDpaData = m_pDpaGridWarpPoints;

	for (int y=0; y<nRows; y++)
	{
		for (int x=0; x<nNewCols; x++)
		{
			pNewDpaData->X(x, y) = pOldDpaData->X(2*x, y);
			pNewDpaData->Y(x, y) = pOldDpaData->Y(2*x, y);
		}
	}

	delete m_pDpaGridWarpPoints;
	m_pDpaGridWarpPoints = new CDoublePointArray(nNewCols, nRows);
	memcpy(m_pDpaGridWarpPoints->m_pPoints, pNewDpaData->m_pPoints, nNewCols*nRows*sizeof(CDoublePoint));

	m_gridsetting.cx = nNewCols;
	m_gridsetting.cy = nRows;

	theApp.GlobleUnlock();

	m_ptCursor.x = m_ptCursor.x == nCols-1 ? nNewCols-1 : long(m_ptCursor.x*0.5);
	m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

	if (!m_KeyRect.IsRectEmpty())
	{
		m_KeyRect.left  = Round(m_KeyRect.left*0.5);
		m_KeyRect.right = Round(m_KeyRect.right*0.5);
	}

	InvalidateRect(m_rcPaint);

	delete pNewDpaData;
	pNewDpaData = NULL;
	pOldDpaData = NULL;
}

void CMGgridwarpDlg::OnCbnSelChangeIncrement()
{
	int nSel = m_CBoxIncrement.GetCurSel();

	switch(nSel)
	{
	case 0:
		m_dIncrement = 0.5;
		break;
	case 1:
		m_dIncrement = 1.0;
		break;
	case 2:
		m_dIncrement = 2.0;
		break;
	case 3:
		m_dIncrement = 5.0;
		break;
	case 4:
		m_dIncrement = 10.0;
		break;
	case 5:
		m_dIncrement = 20.0;
		break;
	default:
		m_dIncrement = .0;
	}
}

CPoint CMGgridwarpDlg::GetGridIndex(CPoint ptLogic) const
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	CRect rcPoint;
	int nRadius = 7;

	for (int y=0; y<cy; y++) {
		for (int x=0; x<cx; x++)
		{
			rcPoint.left   = m_nDeflate+Round(m_pDpaGridWarpPoints->X(x, y)*m_xScale)-nRadius;
			rcPoint.top    = m_nDeflate+Round(m_pDpaGridWarpPoints->Y(x, y)*m_xScale)-nRadius;
			rcPoint.right  = m_nDeflate+Round(m_pDpaGridWarpPoints->X(x, y)*m_xScale)+nRadius;
			rcPoint.bottom = m_nDeflate+Round(m_pDpaGridWarpPoints->Y(x, y)*m_xScale)+nRadius;
			if (PtInRect(rcPoint, ptLogic))
			{
				return CPoint(x, y);
			}
		}
	}

	return CPoint(-1,-1);
}

void CMGgridwarpDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	m_ptBegin = point;
	m_bLBtnDown = TRUE;

	//开始选取四点区域
	if (m_bBeginSelectKeyRect)
	{
		return CXTPResizeDialog::OnLButtonDown(nFlags, point);
	}

	//探测点击点
	m_bLBtnDownAtGridPoint = IsLBtnDownAtGridPoint(point);

	//重绘
	if (m_bLBtnDownAtGridPoint)
	{
		ShowCursorCoords();
		InvalidateRect(m_rcPaint);
	}

	CXTPResizeDialog::OnLButtonDown(nFlags, point);
}

void CMGgridwarpDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	//选取四点区域结束
	if (m_bBeginSelectKeyRect && !m_KeyRect.IsRectEmpty())
	{
		if (MessageBox(_T("当前选定的区域是否符合要求？"), _T("区域确认"), MB_YESNO) == IDYES)
		{
			m_bBeginSelectKeyRect = FALSE;
		}
		else
			m_KeyRect.SetRectEmpty();
	}

	if (m_bLBtnDownAtGridPoint && m_bMouseMoving) AddRecord();

	//重置控制变量
	m_ptBegin.SetPoint(-1,-1);
	m_ptEnd.SetPoint(-1,-1);
	m_bLBtnDownAtGridPoint = FALSE;
	m_bMouseMoving = FALSE;
	m_bLBtnDown = FALSE;

	InvalidateRect(m_rcPaint);

	CXTPResizeDialog::OnLButtonUp(nFlags, point);
}

void CMGgridwarpDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (PtInRect(m_rcPaint, point) && m_TrimMode == eStyle_InterKeystone)
	{
		//弹出开始四点区域选取的菜单

		ClientToScreen(&point);
		theApp.MvTrackPopupMenu(point, IDR_MENU_GRIDWARP_SELECT, this);
	}

	CXTPResizeDialog::OnRButtonDown(nFlags, point);
}


void CMGgridwarpDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	if (m_bLBtnDown)
	{
		//移动鼠标选取四点区域
		if (m_bBeginSelectKeyRect)
		{
			m_ptEnd = point;
			SelectInterKeystoneRect(point);
		}
		else
		{
			//区域四点校正，但是没有选定区域
			if (m_TrimMode == eStyle_InterKeystone && m_KeyRect.IsRectEmpty())
				return CXTPResizeDialog::OnMouseMove(nFlags, point);

			if (m_bLBtnDownAtGridPoint)
			{
				//执行校正过程
				m_bMouseMoving = TRUE;
				OnAjust(CDoublePoint(point.x/m_xScale, point.y/m_yScale));
			}
		}
	}

	CXTPResizeDialog::OnMouseMove(nFlags, point);
}


BOOL CMGgridwarpDlg::IsLBtnDownAtGridPoint(CPoint point)
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	CRect rcGridPoint;
	int nRadius = 7;
	int nScaleX = 1;
	int nScaleY = 1;

	if (m_TrimMode == eStyle_InterKeystone)
	{	//区域四点校正探测四个角

		//还没选定四个点
		if (m_KeyRect.IsRectEmpty()) return FALSE;

		CPoint ptCorner[4];
		ptCorner[0] = m_KeyRect.TopLeft();
		ptCorner[1] = CPoint(m_KeyRect.right, m_KeyRect.top);
		ptCorner[2] = CPoint(m_KeyRect.left, m_KeyRect.bottom);
		ptCorner[3] = m_KeyRect.BottomRight();
		for (int i=0; i<4; i++)
		{
			rcGridPoint.left   = m_nDeflate+Round(m_pDpaGridWarpPoints->X(ptCorner[i].x, ptCorner[i].y)*m_xScale)-nRadius;
			rcGridPoint.top    = m_nDeflate+Round(m_pDpaGridWarpPoints->Y(ptCorner[i].x, ptCorner[i].y)*m_yScale)-nRadius;
			rcGridPoint.right  = m_nDeflate+Round(m_pDpaGridWarpPoints->X(ptCorner[i].x, ptCorner[i].y)*m_xScale)+nRadius;
			rcGridPoint.bottom = m_nDeflate+Round(m_pDpaGridWarpPoints->Y(ptCorner[i].x, ptCorner[i].y)*m_yScale)+nRadius;

			if (PtInRect(rcGridPoint, point))
			{
				m_ptCursor = ptCorner[i];
				m_dpaCursorPos = m_pDpaGridWarpPoints->Val(ptCorner[i].x, ptCorner[i].y);

				return TRUE;
			}
		}

		return FALSE;
	}
	else if (m_TrimMode == eKeystone) {
		//梯形校正探测四个角
		nScaleX = cx-1;
		nScaleY = cy-1;

		cx = 2;
		cy = 2;
	}

	for (int x=0; x<cx; x++)	{
		for (int y=0; y<cy; y++)	{
			rcGridPoint.left   = m_nDeflate+Round(m_pDpaGridWarpPoints->X(x*nScaleX, y*nScaleY)*m_xScale)-nRadius;
			rcGridPoint.top    = m_nDeflate+Round(m_pDpaGridWarpPoints->Y(x*nScaleX, y*nScaleY)*m_yScale)-nRadius;
			rcGridPoint.right  = m_nDeflate+Round(m_pDpaGridWarpPoints->X(x*nScaleX, y*nScaleY)*m_xScale)+nRadius;
			rcGridPoint.bottom = m_nDeflate+Round(m_pDpaGridWarpPoints->Y(x*nScaleX, y*nScaleY)*m_yScale)+nRadius;

			if (PtInRect(rcGridPoint, point))
			{
				m_ptCursor = CPoint(x*nScaleX, y*nScaleY);
				m_dpaCursorPos = m_pDpaGridWarpPoints->Val(x*nScaleX, y*nScaleY);

				return TRUE;
			}
		}
	}

	return FALSE;
}

void CMGgridwarpDlg::SelectInterKeystoneRect(CPoint ptEnd)
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	CRect rcKey = CRect(min(m_ptBegin.x, ptEnd.x), min(m_ptBegin.y, ptEnd.y), max(m_ptBegin.x, ptEnd.x), max(m_ptBegin.y, ptEnd.y));
	//将选取区域转换成物理坐标
	rcKey.left   = Round(rcKey.left/m_xScale);
	rcKey.right  = Round(rcKey.right/m_xScale);
	rcKey.top    = Round(rcKey.top/m_yScale);
	rcKey.bottom = Round(rcKey.bottom/m_yScale);

	//选取的点数组
	CArray <CPoint, CPoint&> ArrPtInKey;
	CPoint ptDot;
	for (int y=0; y<cy; y++) {
		for (int x=0; x<cx; x++)
		{
			ptDot.x = int(m_pDpaGridWarpPoints->X(x, y));
			ptDot.y = int(m_pDpaGridWarpPoints->Y(x, y));

			if (PtInRect(rcKey, ptDot))
				ArrPtInKey.Add(CPoint(x,y));
		}
	}

	//计算最终的四个角坐标
	int nCount = ArrPtInKey.GetCount();
	if (nCount < 4)
	{
		m_KeyRect.SetRectEmpty();
		//主要是为了画鼠标轨迹线才刷新界面
		InvalidateRect(m_rcPaint);
		return;
	}

	//确定最终区域
	int nxMin = ArrPtInKey[0].x;
	int nxMax = ArrPtInKey[0].x;
	int nyMin = ArrPtInKey[0].y;
	int nyMax = ArrPtInKey[0].y;
	for (int i=1; i<nCount; i++) {
		if (ArrPtInKey[i].x >= nxMax) nxMax = ArrPtInKey[i].x;
		if (ArrPtInKey[i].x <= nxMin) nxMin = ArrPtInKey[i].x;
		if (ArrPtInKey[i].y >= nyMax) nyMax = ArrPtInKey[i].y;
		if (ArrPtInKey[i].y <= nyMin) nyMin = ArrPtInKey[i].y;
	}

	m_KeyRect = CRect(nxMin, nyMin, nxMax, nyMax);

	//将当前控制点设置为区域四点的右下角
	if ((m_KeyRect.Width() != 0) && (m_KeyRect.Height() != 0))
	{
		m_ptCursor = m_KeyRect.BottomRight();
		m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

		ShowCursorCoords();
	}
	else
		m_KeyRect.SetRectEmpty();

	//重绘
	InvalidateRect(m_rcPaint);
}


void CMGgridwarpDlg::OnAjust(CDoublePoint point)
{
	//计算点偏移量
	//m_CursorOffsetX = point.x/m_xScale - m_dpaCursorPos.x;
	//m_CursorOffsetY = point.y/m_yScale - m_dpaCursorPos.y;
	m_CursorOffsetX = point.x - m_dpaCursorPos.x;
	m_CursorOffsetY = point.y - m_dpaCursorPos.y;

	//行内校正和整列移动只允许X方向， 列内校正和整行移动只允许Y方向
	if (m_TrimMode == eStyle_InterRow || m_TrimMode == eStyle_FullCol)
		m_CursorOffsetY = 0.0;
	if (m_TrimMode == eStyle_InterCol || m_TrimMode == eStyle_FullRow)
		m_CursorOffsetX = 0.0;

	//计算当前点的坐标值
	if (m_dpaCursorPos.x+m_CursorOffsetX < 0.0) m_CursorOffsetX = 0-m_dpaCursorPos.x;
	if (m_dpaCursorPos.x+m_CursorOffsetX > m_gridsetting.cxResIn) m_CursorOffsetX = m_gridsetting.cxResIn - m_dpaCursorPos.x;
	if (m_dpaCursorPos.y+m_CursorOffsetY < 0.0) m_CursorOffsetY = 0-m_dpaCursorPos.y;
	if (m_dpaCursorPos.y+m_CursorOffsetY > m_gridsetting.cyResIn) m_CursorOffsetY = m_gridsetting.cyResIn - m_dpaCursorPos.y;

	m_dpaCursorPos.x += m_CursorOffsetX;
	m_dpaCursorPos.y += m_CursorOffsetY;

	theApp.GlobleLock(); //资源锁		

	m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y) = m_dpaCursorPos;

	// 点坐标计算
	if (m_TrimMode == eStyle_Keystone)
	{ //梯形校正
		Warp_Auto_Kestone(0, 0, m_gridsetting.cx-1, m_gridsetting.cy-1);
	}
	else if (m_TrimMode == eStyle_InterKeystone)
	{ //区域四点
		Warp_Auto_InterKeystone();
	}
	else if (m_TrimMode == eStyle_InterRow)
	{ //行内校正
		if (m_bBlanceLeft)  BlanceLeft();
		if (m_bBlanceRight) BlanceRight();
	}
	else if (m_TrimMode == eStyle_InterCol)
	{ //列内校正
		if (m_bBlanceUpper) BlanceUpper();
		if (m_bBlanceDown)  BlanceDown();
	}
	else if (m_TrimMode == eStyle_FullRow)
	{ //整行移动
		FullRowMove();
	}
	else if (m_TrimMode == eStyle_FullCol)
	{ //整列移动
		FullColMove();
	}
	else if (m_TrimMode == eStyle_SinglePoint)
	{ //单点移动
		if (m_bBlanceLeft)  BlanceLeft();
		if (m_bBlanceUpper) BlanceUpper();
		if (m_bBlanceRight) BlanceRight();
		if (m_bBlanceDown)  BlanceDown();
	}

	TaskPackage pack;
	Packaging(pack, GRIDWARP_AJUST);
	theApp.PushPackage(pack);

	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock(); //解除资源锁

	//刷新视图
	ShowCursorCoords();
	InvalidateRect(m_rcPaint);
}

//四点Keystone,自动调整边缘各个点,四边拉直线。
void CMGgridwarpDlg::Warp_Auto_Kestone(int left, int top, int right, int bottom)
{
	if (left==right || top==bottom) return;

	theApp.GlobleLock();

	double xAvr, yAvr;
	CDoublePointArray* pPoints = m_pDpaGridWarpPoints;

	//第1行平均，上面两个控制点才需要处理第1行
	if (m_ptCursor.y == top)
	{   
		xAvr = (pPoints->Val(right,top).x - pPoints->Val(left,top).x)/(right-left);
		yAvr = (pPoints->Val(right,top).y - pPoints->Val(left,top).y)/(right-left);
		for(int x =left;x<=right;x++) 
		{
			pPoints->X(x,top) = pPoints->Val(left,top).x + xAvr*(x-left);
			pPoints->Y(x,top) = pPoints->Val(left,top).y + yAvr*(x-left);
		}
	}

	//最后一行平均，下面两个控制点才需要处理最后一行
	if (m_ptCursor.y == bottom)
	{
		xAvr = (pPoints->Val(right,bottom).x - pPoints->Val(left,bottom).x)/(right-left);
		yAvr = (pPoints->Val(right,bottom).y - pPoints->Val(left,bottom).y)/(right-left);
		for(int x =left;x<=right;x++) 
		{
			pPoints->X(x,bottom) = pPoints->Val(left,bottom).x + xAvr*(x-left);
			pPoints->Y(x,bottom) = pPoints->Val(left,bottom).y + yAvr*(x-left);
		}
	}


	//第1 列平均，左边两个控制点才需要处理第一列
	if (m_ptCursor.x == left)
	{
		xAvr = (pPoints->Val(left,bottom).x - pPoints->Val(left,top).x)/(bottom-top);
		yAvr = (pPoints->Val(left,bottom).y - pPoints->Val(left,top).y)/(bottom-top);
		for(int y =top;y<=bottom;y++) 
		{
			pPoints->X(left,y) = pPoints->Val(left,top).x + xAvr*(y-top);
			pPoints->Y(left,y) = pPoints->Val(left,top).y + yAvr*(y-top);
		}
	}

	//最后一列平均，右边两个控制点才需要处理最后一列
	if (m_ptCursor.x == right)
	{
		xAvr = (pPoints->Val(right,bottom).x - pPoints->Val(right,top).x)/(bottom-top);
		yAvr = (pPoints->Val(right,bottom).y - pPoints->Val(right,top).y)/(bottom-top);
		for(int y =top;y<=bottom;y++) 
		{
			pPoints->X(right,y) = pPoints->Val(right,top).x + xAvr*(y-top);
			pPoints->Y(right,y) = pPoints->Val(right,top).y + yAvr*(y-top);
		}
	}

	//平均中间每一行
	for(int y=top; y<=bottom; y++)
	{
		xAvr = (pPoints->Val(right,y).x - pPoints->Val(left,y).x)/(right-left);	
		for(int x=left; x<=right; x++)
		{
			pPoints->X(x,y) = pPoints->Val(left,y).x + xAvr*(x-left);
		}
	}

	//平均中间每一列
	for(int x=left; x<=right; x++)
	{
		yAvr = (pPoints->Val(x,bottom).y - pPoints->Val(x,top).y)/(bottom-top);
		for(int y=top; y<=bottom; y++)
		{
			pPoints->Y(x,y) = pPoints->Val(x,top).y + yAvr*(y-top);
		}
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::BlanceLeft()
{
	if (m_ptCursor.x == 0) return;

	//最大行列数
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	//当前行列数
	int nCurY = m_ptCursor.y;
	int nCurX = m_ptCursor.x;

	theApp.GlobleLock();

	double xAvr = (m_dpaCursorPos.x - m_pDpaGridWarpPoints->X(0, nCurY))/nCurX;
	for (int x=1; x<nCurX; x++)
	{
		m_pDpaGridWarpPoints->X(x, nCurY) = m_pDpaGridWarpPoints->X(0, nCurY) + xAvr*x;
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::BlanceUpper()
{
	if (m_ptCursor.y == 0) return;

	//最大行列数
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	//当前行列数
	int nCurY = m_ptCursor.y;
	int nCurX = m_ptCursor.x;

	theApp.GlobleLock();

	double yAvr = (m_dpaCursorPos.y - m_pDpaGridWarpPoints->Y(nCurX, 0))/nCurY;
	for (int y=1; y<nCurY; y++)
	{
		m_pDpaGridWarpPoints->Y(nCurX, y) = m_pDpaGridWarpPoints->Y(nCurX, 0) + yAvr*y;
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::BlanceRight()
{
	if (m_ptCursor.x == m_gridsetting.cx-1) return;

	//最大行列数
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	//当前行列数
	int nCurY = m_ptCursor.y;
	int nCurX = m_ptCursor.x;

	theApp.GlobleLock();

	double xAvr = (m_pDpaGridWarpPoints->X(cx-1, nCurY) - m_dpaCursorPos.x)/(cx-nCurX-1);
	for (int x=nCurX; x<cx-1; x++)
	{
		m_pDpaGridWarpPoints->X(x, nCurY) = m_dpaCursorPos.x + xAvr*(x-nCurX);
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::BlanceDown()
{
	if (m_ptCursor.y == m_gridsetting.cy-1) return;

	//最大行列数
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;
	//当前行列数
	int nCurY = m_ptCursor.y;
	int nCurX = m_ptCursor.x;

	theApp.GlobleLock();

	double yAvr = (m_pDpaGridWarpPoints->Y(nCurX, cy-1) - m_dpaCursorPos.y)/(cy-nCurY-1);
	for (int y=nCurY; y<cy-1; y++)
	{
		m_pDpaGridWarpPoints->Y(nCurX, y) = m_dpaCursorPos.y + yAvr*(y-nCurY);
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::FullRowMove()
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	int nCurX = m_ptCursor.x;
	int nCurY = m_ptCursor.y;

	theApp.GlobleLock();

	//先移动该行
	for (int x=0; x<cx; x++)
	{
		if (x == nCurX) continue;
		m_pDpaGridWarpPoints->Y(x, nCurY) += m_CursorOffsetY;
	}

	double yAvr = 0.0;
	//平均该行上方所有行的Y坐标
	if (m_bBlanceUpper && (nCurY != 0)) 
	{
		for (int x=0; x<cx; x++)
		{
			yAvr = m_pDpaGridWarpPoints->YDistance(x, 0, x, nCurY)/nCurY;
			for (int y=1; y<nCurY; y++)
			{
				m_pDpaGridWarpPoints->Y(x, y) = m_pDpaGridWarpPoints->Y(x, 0) + yAvr*y;
			}
		}
	}

	//平均该行下方所有行的Y坐标
	if (m_bBlanceDown && (nCurY != cy-1))
	{
		for (int x=0; x<cx; x++)
		{
			yAvr = m_pDpaGridWarpPoints->YDistance(x, nCurY, x, cy-1)/(cy-1-nCurY);
			for (int y=nCurY+1; y<cy-1; y++)
			{
				m_pDpaGridWarpPoints->Y(x, y) = m_pDpaGridWarpPoints->Y(x, nCurY) + yAvr*(y-nCurY);
			}
		}
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::FullColMove()
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	int nCurX = m_ptCursor.x;
	int nCurY = m_ptCursor.y;

	theApp.GlobleLock();

	//先移动该列
	for (int y=0; y<cy; y++)
	{
		if (y == nCurY) continue;
		m_pDpaGridWarpPoints->X(nCurX, y) += m_CursorOffsetX;
	}

	double xAvr = 0.0;
	//平均该列左边所有列的X坐标
	if (m_bBlanceLeft && (nCurX != 0))
	{
		for (int y=0; y<cy; y++)
		{
			xAvr = m_pDpaGridWarpPoints->XDistance(0, y, nCurX, y)/nCurX;
			for (int x=1; x<nCurX; x++)
			{
				m_pDpaGridWarpPoints->X(x, y) = m_pDpaGridWarpPoints->X(0, y) + xAvr*x;
			}
		}
	}

	//平均该列右边所有列的X坐标
	if (m_bBlanceRight && (nCurX != cx-1))
	{
		for (int y=0; y<cy; y++)
		{
			xAvr = m_pDpaGridWarpPoints->XDistance(nCurX, y, cx-1, y)/(cx-1-nCurX);
			for (int x=nCurX+1; x<cx-1; x++)
			{
				m_pDpaGridWarpPoints->X(x, y) = m_pDpaGridWarpPoints->X(nCurX, y) + xAvr*(x-nCurX);
			}
		}
	}

	theApp.GlobleUnlock();
}

void CMGgridwarpDlg::Warp_Auto_InterKeystone()
{
	if (m_KeyRect.IsRectEmpty()) return;

	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	//先做内部四点校正
	Warp_Auto_Kestone(m_KeyRect.left, m_KeyRect.top, m_KeyRect.right, m_KeyRect.bottom);

	//控制点左上角区域四点校正
	if (m_bBlanceLeft && m_bBlanceUpper)
	{
		if (m_ptCursor == m_KeyRect.TopLeft())
			Warp_Auto_Kestone(0, 0, m_ptCursor.x, m_ptCursor.y);
		else if (m_ptCursor == CPoint(m_KeyRect.right, m_KeyRect.top))
			Warp_Auto_Kestone(m_KeyRect.left, 0, m_KeyRect.right, m_KeyRect.top);
		else if (m_ptCursor == CPoint(m_KeyRect.left, m_KeyRect.bottom))
			Warp_Auto_Kestone(0, m_KeyRect.top, m_KeyRect.left, m_KeyRect.bottom);
		//else if (m_ptCursor == m_KeyRect.BottomRight())
		//右下角控制点的左上区域为内部四点区域
	}

	//控制点右上角区域四点校正
	if (m_bBlanceRight && m_bBlanceUpper)
	{
		if (m_ptCursor == m_KeyRect.TopLeft())
			Warp_Auto_Kestone(m_KeyRect.left, 0, m_KeyRect.right, m_KeyRect.top);
		else if (m_ptCursor == CPoint(m_KeyRect.right, m_KeyRect.top))
			Warp_Auto_Kestone(m_KeyRect.right, 0, cx-1, m_KeyRect.top);
		//else if (m_ptCursor == CPoint(m_KeyRect.left, m_KeyRect.bottom))
		//右下角控制点的右上区域为内部四点区域	
		else if (m_ptCursor == m_KeyRect.BottomRight())
			Warp_Auto_Kestone(m_KeyRect.right, m_KeyRect.top, cx-1, m_KeyRect.bottom);
	}

	//控制点左下角区域四点校正
	if (m_bBlanceLeft && m_bBlanceDown)
	{
		if (m_ptCursor == m_KeyRect.TopLeft())
			Warp_Auto_Kestone(0, m_KeyRect.top, m_KeyRect.left, m_KeyRect.bottom);
		//else if (m_ptCursor == CPoint(m_KeyRect.right, m_KeyRect.top))
		//右上角控制点的左下区域为内部四点区域
		else if (m_ptCursor == CPoint(m_KeyRect.left, m_KeyRect.bottom))
			Warp_Auto_Kestone(0, m_KeyRect.bottom, m_KeyRect.left, cy-1);
		else if (m_ptCursor == m_KeyRect.BottomRight())
			Warp_Auto_Kestone(m_KeyRect.left, m_KeyRect.bottom, m_KeyRect.right, cy-1);
	}

	//控制点右下角区域四点校正
	if (m_bBlanceRight && m_bBlanceDown)
	{
		//if (m_ptCursor == m_KeyRect.TopLeft())
		//左上角控制点的右下区域为内部四点区域
		if (m_ptCursor == CPoint(m_KeyRect.right, m_KeyRect.top))
			Warp_Auto_Kestone(m_KeyRect.right, m_KeyRect.top, cx-1, m_KeyRect.bottom);
		else if (m_ptCursor == CPoint(m_KeyRect.left, m_KeyRect.bottom))
			Warp_Auto_Kestone(m_KeyRect.left, m_KeyRect.bottom, m_KeyRect.right, cy-1);
		else if (m_ptCursor == m_KeyRect.BottomRight())
			Warp_Auto_Kestone(m_KeyRect.right, m_KeyRect.bottom, cx-1, cy-1);
	}
}

//开始选择内部四点区域
void CMGgridwarpDlg::OnKeystoneSelect()
{
	m_bBeginSelectKeyRect = TRUE;
}

BOOL CMGgridwarpDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
		{
			if (pMsg->hwnd == m_EditCursorPosX.m_hWnd
				|| pMsg->hwnd == m_EditCursorPosY.m_hWnd) 
			{
				//手动输入坐标值
				OnCustomizedCoords();				
			}

			return TRUE;
		}
		else if (pMsg->wParam == VK_UP
			|| pMsg->wParam == VK_DOWN
			|| pMsg->wParam == VK_RIGHT
			|| pMsg->wParam == VK_LEFT)
		{
			if (::GetAsyncKeyState(VK_SHIFT) & 0x8000)
				OnVkArrowSwitchCursorPoint(pMsg->wParam);
			else
				OnVkArrowMoveCursorPoint(pMsg->wParam);

			return TRUE;
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

//编辑框中输入当前控制点的物理坐标
void CMGgridwarpDlg::OnCustomizedCoords()
{
	UpdateData();

	CString strCoords;
	double  dxCoords, dyCoords;
	//横坐标
	m_EditCursorPosX.GetWindowText(strCoords);
	dxCoords = atof(strCoords);
	m_dpaCursorPos.x = dxCoords;
	//纵坐标
	m_EditCursorPosY.GetWindowText(strCoords);
	dyCoords = atof(strCoords);
	m_dpaCursorPos.y = dyCoords;

	OnAjust(CDoublePoint(dxCoords, dyCoords));
}

//Alt+方向键变换控制点
void CMGgridwarpDlg::OnVkArrowSwitchCursorPoint(UINT nVKcode)
{
	int cx = m_gridsetting.cx;
	int cy = m_gridsetting.cy;

	if (m_TrimMode == eStyle_Keystone)
	{   //梯形校正的四个控制点切换
		m_ptCursor.x = (nVKcode == VK_LEFT) ? 0 : (nVKcode==VK_RIGHT ? cx-1 : m_ptCursor.x);
		m_ptCursor.y = (nVKcode == VK_UP) ? 0 : (nVKcode==VK_DOWN ? cy-1 : m_ptCursor.y);
	}
	else if (m_TrimMode == eStyle_InterKeystone
		&& !m_KeyRect.IsRectEmpty())
	{   //区域四点校正的四个控制点切换
		m_ptCursor.x = (nVKcode == VK_LEFT) ? m_KeyRect.left : (nVKcode==VK_RIGHT ? m_KeyRect.right : m_ptCursor.x);
		m_ptCursor.y = (nVKcode == VK_UP) ? m_KeyRect.top : (nVKcode==VK_DOWN ? m_KeyRect.bottom : m_ptCursor.y);
	}
	else
	{   //所有网格点间切换
		m_ptCursor.x -= (nVKcode==VK_LEFT ? 1 : nVKcode==VK_RIGHT ? -1 : 0);
		m_ptCursor.y -= (nVKcode==VK_UP ? 1 : nVKcode==VK_DOWN ? -1 : 0);
		if (m_ptCursor.x < 0)    m_ptCursor.x = 0;
		if (m_ptCursor.x > cx-1) m_ptCursor.x = cx-1;
		if (m_ptCursor.y < 0)    m_ptCursor.y = 0;
		if (m_ptCursor.y > cy-1) m_ptCursor.y = cy-1;
	}

	m_dpaCursorPos = m_pDpaGridWarpPoints->Val(m_ptCursor.x, m_ptCursor.y);

	ShowCursorCoords();
	InvalidateRect(m_rcPaint);
}

//方向键移动控制点
void CMGgridwarpDlg::OnVkArrowMoveCursorPoint(UINT nVKcode)
{
	double dx = .0;
	double dy = .0;
	dx = (nVKcode==VK_LEFT) ? -m_dIncrement : (nVKcode==VK_RIGHT ? m_dIncrement : 0.0);
	dy = (nVKcode==VK_UP) ? -m_dIncrement : (nVKcode==VK_DOWN ? m_dIncrement : 0.0);

	OnAjust(CDoublePoint(m_dpaCursorPos.x+dx, m_dpaCursorPos.y+dy));
}

//刷新控制点坐标
void CMGgridwarpDlg::ShowCursorCoords()
{
	CString strCoord;

	strCoord.Format(_T("%.1f"), m_dpaCursorPos.x);
	m_EditCursorPosX.SetWindowText(strCoord);

	strCoord.Format(_T("%.1f"), m_dpaCursorPos.y);
	m_EditCursorPosY.SetWindowText(strCoord);
}

//获取板卡的端口号
inline int CMGgridwarpDlg::GetBoardPort()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)GetOwner();
	return pDlg->m_boardInfo.GetBoardPort();
}

//任务包封包
inline void CMGgridwarpDlg::Packaging(TaskPackage &pack, int nMode)
{
	pack.cx              = m_gridsetting.cx;
	pack.cy              = m_gridsetting.cy;
	pack.cxResIn         = m_gridsetting.cxResIn;
	pack.cyResIn         = m_gridsetting.cyResIn;
	pack.nAjustMode      = nMode;
	pack.nBoardPort      = GetBoardPort();
	pack.pDpaComplexData = m_pDpaGridWarpPoints;
	pack.nGridSmooth     = m_nSmooth;
	pack.pPaintWnd       = this;
}

//从设备读取数据并初始化
void CMGgridwarpDlg::Initialize()
{
	CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
	//MG卡的通信端口号
	int nPort = pOwer->m_boardInfo.GetBoardPort();

	//读取当前分辨率
	TIMING_STRUCT ts;
	//pOwer->GetCurrencyTiming(ts);

	if (theApp.GetCurrencyInputTiming((UINT&)ts.m_Hz, (UINT&)ts.m_HTotal, (UINT&)ts.m_HActive, (UINT&)ts.m_VTotal, (UINT&)ts.m_VActive, nPort) != CMD_FAILURE)
	{		
		if (ts.m_HActive <= 0 || ts.m_VActive <= 0 || ts.m_Hz <= 0)
		{
			MessageBox(_T("警告：当前板卡未接入信号，多点校正将采用默认分辨率(1024x768 @ 60Hz)！"),_T("多点校正初始化"), MB_OK|MB_ICONERROR);
			ts = pOwer->m_boardInfo.GetDetailTiming(Product_MG)[0];
		}
	}
	else
	{
		MessageBox(_T("错误！未能获取到当前板卡输出分辨率，多点校正将采用默认分辨率(1024x768 @ 60Hz)！"),_T("多点校正初始化"), MB_OK|MB_ICONERROR);
		ts = pOwer->m_boardInfo.GetDetailTiming(Product_MG)[0];
	}

	//网格属性之输入输出分辨率
	m_gridsetting.cxResIn  = ts.m_HActive;
	m_gridsetting.cyResIn  = ts.m_VActive;
	m_gridsetting.cxResOut = ts.m_HActive;
	m_gridsetting.cyResOut = ts.m_VActive;
	m_gridsetting.nHz      = ts.m_Hz;

	SetGraphicSize();

	//重新计算坐标数据
	ResetDpaData();

	//重绘
	InvalidateRect(m_rcPaint);

	//当前点的坐标
	ShowCursorCoords();
}

void CMGgridwarpDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		//闪烁的信息提示
		if (!m_bMouseMoving)
		{
			int red   = Round(120*m_nColorCount/100);
			int green = Round(120*m_nColorCount/100);
			int blue  = Round(50*m_nColorCount/100);
			m_clrTipText =  RGB(red, green, blue);
			
			if (m_bDecAsc) m_nColorCount+=10;
			else m_nColorCount-=10;

			if (m_nColorCount> 200) m_bDecAsc = FALSE;
			if (m_nColorCount< 100) m_bDecAsc = TRUE;

			InvalidateRect(m_rcPaint);
		}
		break;
	default:
		break;
	}

	CXTPResizeDialog::OnTimer(nIDEvent);
}

//保存当前点阵坐标到PC文件
int CMGgridwarpDlg::SaveGridFile()
{
	CFileDialog File(FALSE, NULL, "MultiGrid.grid", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Grid Files (*.grid)|*.grid|All Files (*.*)|*.*||");
	if (File.DoModal() == IDOK)
	{
		CString strFilePath = File.GetPathName();

		// 打开多点数据文件
		CStdioFile File;
		if (!File.Open(strFilePath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
		{
			return 0;
		}

		// 写入多点的数据
		CString strData;
		char eol[2];
		eol[0] = 10; eol[1] = 0;

		int cxGrid = m_gridsetting.cx;
		int cyGrid = m_gridsetting.cy;

		// 写入文件标记
		strData.Format(_T("MG300"));
		File.WriteString(strData);
		File.Write(eol, 1);
		strData.Format(_T("%d"), 0xF4C9);
		File.WriteString(strData);
		File.Write(eol, 1);
		// 输入分辨率
		strData.Format(_T("%d"), m_gridsetting.cxResIn);
		File.WriteString(strData);
		File.Write(eol, 1);
		strData.Format(_T("%d"), m_gridsetting.cyResIn);
		File.WriteString(strData);
		File.Write(eol, 1);

		// 输出分辨率
		strData.Format(_T("%d"), m_gridsetting.cxResOut);
		File.WriteString(strData);
		File.Write(eol, 1);
		strData.Format(_T("%d"), m_gridsetting.cyResOut);
		File.WriteString(strData);
		File.Write(eol, 1);

		// 网格数
		strData.Format(_T("%d"), m_gridsetting.cx);
		File.WriteString(strData);
		File.Write(eol, 1);
		strData.Format(_T("%d"), m_gridsetting.cy);
		File.WriteString(strData);
		File.Write(eol, 1);

		// 多点坐标数据
		strData.Format(_T("%s"), "----------MultiPoint----------");
		File.WriteString(strData);
		File.Write(eol, 1);
		CDoublePointArray *pPoints = m_pDpaGridWarpPoints;

		for (int j=0; j<cyGrid; j++)
		{
			for (int i=0; i<cxGrid; i++)
			{
				strData.Format(_T("%14.9f\t%14.9f"), pPoints->X(i, j), pPoints->Y(i, j));
				File.WriteString(strData);
				File.Write(eol, 1);
			}			
		}

		File.Close();

		return 1;
	}

	return 2;
}

//从.grid文件中读取点阵数据并初始化到界面和设备
int CMGgridwarpDlg::ReadGridFile()
{
	CFileDialog File(TRUE, _T("grid"), _T("MultiGrid"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("Grid files (*.grid)|*.grid|All files (*.*)|*.*||"));
	if (File.DoModal() == IDOK)// 打开多点数据文件
	{	
		CString strFilePath = File.GetPathName();

		CStdioFile File;
		if (!File.Open(strFilePath, CFile::modeRead | CFile::typeText))
			return 0;

		// 读取数据
		CString strData, strMsg;
		// 读取文件标记
		CString strFlag;
		int nFlag;
		File.ReadString(strData);
		strFlag = strData;
		File.ReadString(strData);
		nFlag = _ttoi(strData);
		if (strFlag != _T("MG300") || nFlag != 0xF4C9)
		{
			MessageBox(_T("文件格式错误！请打开正确的多点数据文件！"), _T("打开文件"), MB_OK|MB_ICONERROR);
			return 0;
		}

		// 输入分辨率
		File.ReadString(strData);
		int nResInH = _ttoi(strData);
		File.ReadString(strData);
		int nResInV = _ttoi(strData);

		// 输出分辨率
		File.ReadString(strData);
		int nResOutH = _ttoi(strData);
		File.ReadString(strData);
		int nResOutV = _ttoi(strData);

		if (nResInH != m_gridsetting.cxResIn
		 || nResInV != m_gridsetting.cyResIn
		 || nResOutH != m_gridsetting.cxResOut
		 || nResOutV != m_gridsetting.cyResOut)
		{
#ifdef ENGLISHVER
			strMsg.Format(_T("Mismatched input and output resolution!\nReading input resolution：%dx%d，Current input resolution：%dx%d;\nReading output resolution：%dx%d，Current output resolution：%dx%d"),
				nResInH, nResInV, m_gridsetting.cxResIn, m_gridsetting.cyResIn, 
				nResOutH, nResOutV, m_gridsetting.cxResOut, m_gridsetting.cyResOut);
#else
			strMsg.Format(_T("读取的输入和输出分辨率与当前输入和输出分辨率不匹配。\n读取的输入分辨率为：%dx%d，当前的输入分辨率为：%dx%d;\n读取的输出分辨率为：%dx%d，当前输出分辨率为：%dx%d"),
				nResInH, nResInV, m_gridsetting.cxResIn, m_gridsetting.cyResIn, 
				nResOutH, nResOutV, m_gridsetting.cxResOut, m_gridsetting.cyResOut);
#endif
			MessageBox(strMsg, _T("打开文件"), MB_OK|MB_ICONINFORMATION);
			return 0;
		}

		// 网格数
		File.ReadString(strData);
		int cxGrid = _ttoi(strData);
		File.ReadString(strData);
		int cyGrid = _ttoi(strData);

		if (cxGrid != m_gridsetting.cx
		 || cyGrid != m_gridsetting.cy)
		{
#ifdef ENGLISHVER
			strMsg.Format(_T("Mismatched grid lines!\nReading grid：%dx%d，Current grid：%dx%d"),
				cxGrid,cyGrid, m_gridsetting.cx, m_gridsetting.cy);
#else
			strMsg.Format(_T("读取的校正网格数与当前的校正网格数相异。\n读取的校正网格配置为：%dx%d，当前的校正网格配置为：%dx%d"),
				cxGrid,cyGrid, m_gridsetting.cx, m_gridsetting.cy);
#endif
			MessageBox(strMsg, _T("打开文件"), MB_OK|MB_ICONINFORMATION);

			return 0;
		}

		// 读多点坐标值
		char *next_token = NULL;
		File.ReadString(strData); //"----------MultiPoint----------"
		for (int j=0; j<cyGrid; j++)
		{
			for(int i=0; i<cxGrid; i++)
			{
				File.ReadString(strData);
				if (strData.IsEmpty())
				{
					MessageBox(_T("读取数据失败！"), _T("读取数据"), MB_OK|MB_ICONERROR);
					return 0;
				}
				LPCTSTR sz = _tcstok_s(strData.GetBuffer(255), " \t\n", &next_token);
				double f = _tcstod(sz, NULL);
				m_pDpaGridWarpPoints->X(i, j) = f;

				sz = _tcstok_s(NULL, " \t\n", &next_token);
				f = _tcstod(sz, NULL);
				m_pDpaGridWarpPoints->Y(i, j) = f;
			}			
		}

		//更新到设备
		theApp.GlobleLock();

		TaskPackage pack;
		Packaging(pack, GRIDWARP_AJUST);
		theApp.PushPackage(pack);

		g_AjustThreadWorkMode = WORK_MODE_AJUST;

		theApp.GlobleUnlock();

		InvalidateRect(m_rcPaint);

		return 1;
	}

	return 2;
}

//保存点阵坐标到设备
bool CMGgridwarpDlg::WriteGridData()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	int nPort = pOwner->m_boardInfo.GetBoardPort();

	unsigned char data[2200];
	memset(data, 0, sizeof(data));

	// 写入文件标记  保存软件写入的信息，包括什么软件写入  通道   分辨率  网格数等 
	// 使能 Grid data 写入 Flash中  16字节为一行  
	// "MPG30X"0xF4C9  第一行 表示MPG30X软件写入的数据，且标志符为0xf4C9   [15] - 表示此文件上次保存为第几通道

	int i=0;
	data[i++]='M';data[i++]='P'; data[i++]='G'; data[i++]='3'; data[i++]='0'; data[i++]='X'; data[i++]=0xC9; data[i++]=0xF4;  data[15]=0; 

	// 输入分辨率 HactivexVactive 共占4个字节 高位在后，低位在前 输出分辨率 
	i=16;
	data[i++]=m_gridsetting.cxResIn&0xff;   data[i++]=(m_gridsetting.cxResIn>>8)&0xff; 
	data[i++]=m_gridsetting.cyResIn&0xff;   data[i++]=(m_gridsetting.cyResIn>>8)&0xff; 

	data[i++]=m_gridsetting.cxResOut&0xff;  data[i++]=(m_gridsetting.cxResOut>>8)&0xff; 
	data[i++]=m_gridsetting.cyResOut&0xff;  data[i++]=(m_gridsetting.cyResOut>>8)&0xff; 

	//网格数描述 17x17  or 65x65 最大为255x255
	i=32;
	data[i++]=m_gridsetting.cx & 0xff;   data[i++]=m_gridsetting.cy & 0xff;   

	//开启flash写入操作
	int flash_index = 0;
	int return_back,loop;

	return_back = theApp.m_Cmd.BinCommand_RW(BINEXE_GRID_ENABLE|BINCODE_FCN, 0x01|(flash_index<<24),data, 48, nPort); 
	if(return_back&0x10000)//通信失败 
		return false;

	theApp.GlobleLock();  //全局资源锁

	//写入GRID数据, x16  
	CDoublePointArray *pPoints = m_pDpaGridWarpPoints;
	int cxGrid = m_gridsetting.cx;
	int cyGrid = m_gridsetting.cy;
	int grid_data_index=0;

	loop = 0;
	for (int j=0; j<cyGrid; j++)
	{
		for (int i=0; i<cxGrid; i++)
		{
			short output_x = (short)(pPoints->X(i, j)*16+0.5);
			short output_y =(short)( pPoints->Y(i, j)*16+0.5);

			data[grid_data_index++] =  output_x&0xff;
			data[grid_data_index++] = (output_x>>0x08)&0xff;
			data[grid_data_index++] =  output_y&0xff;
			data[grid_data_index++] = (output_y>>0x08)&0xff;

			if(grid_data_index == 2048)
			{
				grid_data_index = 0;

				return_back = theApp.m_Cmd.BinCommand_RW(BINEXE_GRID_WRITE|BINCODE_FCN, (2048*loop)|(flash_index<<24), data, 2048, nPort);  
				if(return_back&0x10000)//通信失败 
				{
					theApp.GlobleUnlock();// 离开临界区
					return false;
				}

				loop++;
			}
		}			
	}

	if( (grid_data_index != 0))//之前的通信没有出错
	{
		return_back = theApp.m_Cmd.BinCommand_RW(BINEXE_GRID_WRITE|BINCODE_FCN,(2048*loop)|(flash_index<<24),data, grid_data_index,nPort);  

		if(return_back&0x10000)//通信失败
		{
			theApp.GlobleUnlock(); // 离开临界区
			return false;
		}
	}

	theApp.GlobleUnlock(); //解锁

	return true;
}

//从设备读取点阵坐标
bool CMGgridwarpDlg::LoadGridData()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();
	int nPort = pOwner->m_boardInfo.GetBoardPort();

	unsigned char data[100];
	unsigned char data_flag[10];
	int return_back;

	memset(data, 0, sizeof(data));

	// 读取数据
	CString strFlag,strData, strMsg;
	int nFlag;

	// 读取文件标记  输入分辨率   输出分辨率
	data[0]=48;
	data[1]=0;
	return_back = theApp.m_Cmd.BinCommand_RW(BINEXE_GRID_READ|BINCODE_FCN, 0xf000|(0<<24), data, 2, nPort); 

	if(return_back&0x10000)//通信失败 
		return false;

	//标记
	memset(data_flag, 0, sizeof(data_flag));
	strncpy_s((char*)data_flag, _countof(data_flag), (char*)data, 6); 

	strFlag = (CString)data_flag;
	nFlag = *((unsigned short*)&data[6]);

	//输入分辨率
	int nResInH = *((unsigned short*)&data[16]);
	int nResInV = *((unsigned short*)&data[18]);

	//输出分辨率
	int nResOutH = *((unsigned short*)&data[20]);
	int nResOutV = *((unsigned short*)&data[22]);

	//对比当前的分辨率
	if (nResInH != m_gridsetting.cxResIn
	 || nResInV != m_gridsetting.cyResIn
	 || nResOutH != m_gridsetting.cxResOut
	 || nResOutV != m_gridsetting.cyResOut)
	{
#ifdef ENGLISHVER
		strMsg.Format(_T("Mismatched input and output resolution!\nReading input resolution：%dx%d，Current input resolution：%dx%d;\nReading output resolution：%dx%d，Current output resolution：%dx%d"),
			nResInH, nResInV, m_gridsetting.cxResIn, m_gridsetting.cyResIn, 
			nResOutH, nResOutV, m_gridsetting.cxResOut, m_gridsetting.cyResOut);
#else
		strMsg.Format(_T("读取的输入和输出分辨率与当前输入和输出分辨率不匹配。\n读取的输入分辨率为：%dx%d，当前的输入分辨率为：%dx%d;\n读取的输出分辨率为：%dx%d，当前输出分辨率为：%dx%d"),
			nResInH, nResInV, m_gridsetting.cxResIn, m_gridsetting.cyResIn, 
			nResOutH, nResOutV, m_gridsetting.cxResOut, m_gridsetting.cyResOut);
#endif
		AfxMessageBox(strMsg, MB_OK|MB_ICONSTOP);

		return false;
	}

	// 网格数
	int cxGrid = (unsigned char)data[32];
	int cyGrid = (unsigned char)data[33];

	//对比当前网格
	if (cxGrid != m_gridsetting.cx
	 || cyGrid != m_gridsetting.cy)
	{
#ifdef ENGLISHVER
		strMsg.Format(_T("Mismatched grid lines!\nReading grid：%dx%d，Current grid：%dx%d"),
			cxGrid,cyGrid, m_gridsetting.cx, m_gridsetting.cy);
#else
		strMsg.Format(_T("读取的校正网格数与当前的校正网格数相异。\n读取的校正网格配置为：%dx%d，当前的校正网格配置为：%dx%d"),
			cxGrid,cyGrid, m_gridsetting.cx, m_gridsetting.cy);
#endif
		AfxMessageBox(strMsg, MB_OK|MB_ICONSTOP);

		return false;
	}

	int data_num = cxGrid*cyGrid*4;  //一个像素点占4个bytes, (x,y)
	int loop = data_num/2048 + ((data_num%2048==0)?0:1);

	unsigned char *read_data = new unsigned char[loop*2048];

	for(int i=0;i<loop;i++)
	{
		read_data[i*2048] = 2048&0xff;
		read_data[i*2048+1] = (2048>>8)&0xff;
		return_back = theApp.m_Cmd.BinCommand_RW(BINEXE_GRID_READ|BINCODE_FCN, (i*2048)|(0<<24), (unsigned char*)(&read_data[i*2048]), 2048, nPort); 
		if(return_back&0x10000)//通信失败 
		{
			delete read_data;
			return false;
		}
	}

	theApp.GlobleLock();//全局资源锁

	for (int j=0; j<cyGrid; j++)	{
		for(int i=0; i<cxGrid; i++)
		{
			double f = *((short *)&read_data[j*cxGrid*4+i*4])/16.0;
			m_pDpaGridWarpPoints->X(i, j) = f;

			f = *((short *)&read_data[j*cxGrid*4+i*4+2])/16.0;
			m_pDpaGridWarpPoints->Y(i, j) = f;
		}			
	}

	delete read_data;

	TaskPackage pack;
	Packaging(pack, GRIDWARP_AJUST);
	theApp.PushPackage(pack);

	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock(); //解锁

	InvalidateRect(m_rcPaint);

	return true;
}

void CMGgridwarpDlg::AddRecord()
{
	if (m_UndoList.GetCount() >= MAX_RECORD)
		m_UndoList.RemoveTail();

	//添加记录
	UndoRecord record;
	memset(&record, 0, sizeof(UndoRecord));

	memcpy(record.dpaData, m_pDpaGridWarpPoints->m_pPoints, sizeof(CDoublePoint)*m_gridsetting.cx*m_gridsetting.cy);

	m_UndoList.AddHead(record);

	if (m_RedoList.GetCount() > 0) m_RedoList.RemoveAll();
}

void CMGgridwarpDlg::Undo()
{
	if (m_UndoList.IsEmpty()) return;

	//头结点存的当前场景
	UndoRecord record;
	memset(&record, 0, sizeof(UndoRecord));
	record = m_UndoList.RemoveHead();

	//添加Redo点
	m_RedoList.AddHead(record);

	if (memcmp(m_pDpaGridWarpPoints->m_pPoints, record.dpaData, sizeof(CDoublePoint)*m_gridsetting.cx*m_gridsetting.cy) == 0)
	{
		if (m_UndoList.IsEmpty()) return;

		record = m_UndoList.RemoveHead();
		m_RedoList.AddHead(record);
	}

	//更新设备和界面
	theApp.GlobleLock();

	memcpy(m_pDpaGridWarpPoints->m_pPoints, record.dpaData, sizeof(CDoublePoint)*m_gridsetting.cx*m_gridsetting.cy);

	TaskPackage pack;
	Packaging(pack, GRIDWARP_AJUST);
	theApp.PushPackage(pack);

	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock();

	ShowCursorCoords();
	InvalidateRect(m_rcPaint);
}

void CMGgridwarpDlg::Redo()
{
	if (m_RedoList.IsEmpty()) return;

	//获取最近Undo记录点
	RedoRecord record;
	memset(&record, 0, sizeof(RedoRecord));
	record = m_RedoList.RemoveHead();

	m_UndoList.AddHead(record);

	if (memcmp(m_pDpaGridWarpPoints->m_pPoints, record.dpaData, sizeof(CDoublePoint)*m_gridsetting.cx*m_gridsetting.cy) == 0)
	{
		if (m_RedoList.IsEmpty()) return;

		record = m_RedoList.RemoveHead();
		m_UndoList.AddHead(record);
	}

	//更新设备和界面
	theApp.GlobleLock();

	memcpy(m_pDpaGridWarpPoints->m_pPoints, record.dpaData, sizeof(CDoublePoint)*m_gridsetting.cx*m_gridsetting.cy);

	TaskPackage pack;
	Packaging(pack, GRIDWARP_AJUST);
	theApp.PushPackage(pack);

	g_AjustThreadWorkMode = WORK_MODE_AJUST;

	theApp.GlobleUnlock();

	ShowCursorCoords();
	InvalidateRect(m_rcPaint);
}