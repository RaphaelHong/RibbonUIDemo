#pragma once
#include "NumberEdit.h"

typedef enum __emGridWarpStyle
{
	eStyle_Keystone,
	eStyle_InterKeystone,
	eStyle_InterRow,
	eStyle_InterCol,
	eStyle_FullRow,
	eStyle_FullCol,
	eStyle_SinglePoint
} emGridWarpStyle; //多点校正的几种方式

typedef struct tagRecord
{
	CDoublePoint dpaData[65*65];
} UndoRecord, RedoRecord;

#define MAX_RECORD 20

// CMGgridwarpDlg 对话框

class CMGgridwarpDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGgridwarpDlg)

public:
	CMGgridwarpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGgridwarpDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_GRIDWARP };

private:
	CXTPButton m_GboxFuncCtrl;
	CXTPButton m_GboxAjustCtrl;
	CXTPButton m_GboxBlanceCtrl;
	CXTPButton m_GboxCursorPosCtrl;
	CXTPButton m_GboxGridLinesCtrl;

	CXTPButton m_BtnLoadGeoData;
	CXTPButton m_BtnUndoGrid;
	CXTPButton m_BtnRedoGrid;
	CXTPButton m_BtnResetGrid;

	CXTPButton m_BtnSwitchAjustMode;
	CXTPButton m_CheckBlanceAll;
	CXTPButton m_CheckBlanceLeft;
	CXTPButton m_CheckBlanceUpper;
	CXTPButton m_CheckBlanceRight;
	CXTPButton m_CheckBlanceDown;

	CXTPButton m_CheckSmoothGrid;

	CXTPButton m_BtnSaveAndLoad;

	CNumberEdit m_EditCursorPosX;
	CNumberEdit m_EditCursorPosY;

	CXTPComboBox m_CBoxIncrement;

	CXTPButton m_BtnDoubleGridRow;
	CXTPButton m_BtnHalfGridRow;
	CXTPButton m_BtnDoubleGridCol;
	CXTPButton m_BtnHalfGridCol;

	CBrush m_BkgBrush;  //背景画刷

	//用于控制闪烁文本的变量
	COLORREF m_clrTipText; //文本当前的颜色
	int m_nColorCount;     //颜色递增器
	BOOL m_bDecAsc;        //递增、递减标志

	void ResizeIt();  //动态调整窗口和控件的大小跟位置
	void SetControlTheme(XTPControlTheme theme);
	void SetButtonIcons();
	void SetToolTips();

private:
	// 绘图
	CRect m_rcPaint; //绘图区大小
	CRect m_rcChart; //实际工作区
	int m_nDeflate;  //工作区内缩一定尺寸

	double m_xScale;  //GraphicsSize_X / HActive
	double m_yScale;  //GraphicsSize_Y / VActive

	void GetPaintRect();   //获取绘图区大小
	void SetGraphicSize(); //设置网格属性信息

	void DrawBkgGrid(CDC *pValidDC);           //画背景
	void DrawDataGrid(CDC *pValidDC);          //画数据
	void DrawKeystone(CDC *pValidDC);          //画梯形校正
	void DrawInterKeystone(CDC *pValidDC);     //画区域四点
	void DrawHighLightRow(CDC *pValidDC);      //高亮显示行
	void DrawHighLightCol(CDC *pValidDC);      //高亮显示列
	void DrawOneRow(CDC *pValidDC);            //画一行
	void DrawOneCol(CDC *pValidDC);            //画一列
	void DrawSinglePoint(CDC *pValidDC);       //单个控制点
	void DrawCrossLine(CDC *pValidDC);         //单点移动时显示所在行和列
	void DrawResolution(CDC *pValidDC);        //分辨率
	void DrawTipText(CDC *pValidDC);           //提示用户怎么操作
	void DrawSelectRectangle(CDC *dc, CPoint ptBegin, CPoint ptEnd); //区域四点选取的时候画鼠标选取框的轨迹
	inline void DrawLine(CDC *dc, CPoint ptLineto, int xInc=0, int yInc=0);
	inline void DrawEllipse(CDC *dc, CPoint ptCenter, int nRadius);

private:
	CGridSetting m_gridsetting;  //网格属性信息
	CDoublePointArray *m_pDpaGridWarpPoints; //点坐标
	emGridWarpStyle m_TrimMode; //校正方式

	void InitDpaData();	   //初始化Dpa数据
	void ResetDpaData();   //复位Dpa数据
	CDoublePoint OrignalPos(int x, int y) const;
	CPoint GetGridIndex(CPoint ptLogic) const;  //根据逻辑坐标获取当前控制点的网格索引号

	CPoint m_ptCursor; //当前控制点的网格索引号
	CDoublePoint m_dpaCursorPos; //当前控制点的物理坐标
	double m_CursorOffsetX; //当前控制点移动的水平分量
	double m_CursorOffsetY; //当前控制点移动的垂直分量

	BOOL m_bLBtnDownAtGridPoint; //鼠标左键是否点击在网格点上
	BOOL IsLBtnDownAtGridPoint(CPoint point); //鼠标左键是否点击在网格点上

	BOOL m_bLBtnDown;    //鼠标左键按下
	BOOL m_bMouseMoving; //鼠标正在移动

	//区域四点控制有关的变量
	CPoint m_ptBegin;             //鼠标左键按下时的位置
	CPoint m_ptEnd;               //鼠标当前的位置
	CRect m_KeyRect;              //区域四点校正中四个点的位置（行列号）
	BOOL m_bBeginSelectKeyRect;   //是否开始选取四点区域
	void SelectInterKeystoneRect(CPoint ptEnd); //选取四点区域

	BOOL m_bBlanceLeft;   //左边均匀
	BOOL m_bBlanceUpper;  //上方均匀
	BOOL m_bBlanceRight;  //右边均匀
	BOOL m_bBlanceDown;   //下方均匀

	void OnAjust(CDoublePoint point);  //校正计算	

	void Warp_Auto_Kestone(int left, int top, int right, int bottom);  //梯形校正
	void Warp_Auto_RowX(int nRow);  //行均匀
	void Warp_Auto_RowY(int nCol);  //列均匀

	void BlanceLeft();  //向左均匀
	void BlanceUpper(); //向上均匀
	void BlanceRight(); //向右均匀
	void BlanceDown();  //向下均匀

	void FullRowMove(); //移动一行
	void FullColMove(); //移动一列

	void Warp_Auto_InterKeystone();  //区域四点校正

public:
	void Initialize();         //从设备读取数据初始化
	void ShowCursorCoords();   //显示当前控制点的坐标
	void OnCustomizedCoords(); //手动输入坐标值
	void OnVkArrowSwitchCursorPoint(UINT nVKcode); //切换控制点
	void OnVkArrowMoveCursorPoint(UINT nVKcode);   //用方向键移动控制点
	inline int GetBoardPort();    //获取所属板卡的通信端口号
	inline void Packaging(TaskPackage &pack, int nMode); //任务包封装

	double m_dIncrement;
	int  m_nSmooth; //是否平滑处理

private:
	int SaveGridFile();  //将多点校正数据保存到本地文件  return: 0：失败 1：成功 2：取消
	int ReadGridFile();  //从本地文件读取多点数据        return: 0：失败 1：成功 2：取消
	bool WriteGridData(); //将数据保存到设备
	bool LoadGridData();  //从设备读取数据

private:
	CList <UndoRecord, UndoRecord&> m_UndoList;
	CList <RedoRecord, RedoRecord&> m_RedoList;

	void AddRecord();
	void Undo();
	void Redo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAjustStyleDropDown();
	afx_msg void OnBtnLoadAndSaveDropDown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCommandAjustStyle(UINT nCmdID);
	afx_msg void OnCommandLoadAndSave(UINT nCmdID);
	afx_msg void OnCheckBlanceAllDirections();
	afx_msg void OnCheckBlanceLeft();
	afx_msg void OnCheckBlanceUpper();
	afx_msg void OnCheckBlanceRight();
	afx_msg void OnCheckBlanceDown();
	afx_msg void OnCheckGridSmooth();
	afx_msg void OnKeystoneSelect();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelChangeIncrement();
	afx_msg void OnBtnClickedReset();
	afx_msg void OnBtnClickedUndo();
	afx_msg void OnBtnClickedRedo();
	afx_msg void OnBtnClickedLoadGeoData();
	afx_msg void OnBtnClickedDoubleCols();
	afx_msg void OnBtnClickedHalfCols();
	afx_msg void OnBtnClickedDoubleRows();
	afx_msg void OnBtnClickedHalfRows();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
