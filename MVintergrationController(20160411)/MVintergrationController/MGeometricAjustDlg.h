#pragma once
#include "NumberEdit.h"

typedef enum __emAjustStyle
{
	eKeystone = 0,
	eCurve    = 1,
	eDensity  = 2
} emAjustStyle;

// CMGeometricAjustDlg 对话框

class CMGeometricAjustDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGeometricAjustDlg)

public:
	CMGeometricAjustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGeometricAjustDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_GEOMETRIC };

private:
	// 控件
	CBrush m_bkBrush; //背景画刷

	CXTPButton m_btnKeystone; //四点校正按钮
	CXTPButton m_btnCurve;    //曲面校正按钮
	CXTPButton m_btnDensity;  //疏密校正按钮
	CXTPButton m_btnReset;    //复位按钮

	// 以下控件下标0代表X坐标的控件 下标1代表Y坐标的控件
	CNumberEdit m_editTL[2]; //Topleft      
	CNumberEdit m_editTC[2]; //Topcenter
	CNumberEdit m_editTR[2]; //Topright
	CNumberEdit m_editLC[2]; //Leftcenter
	CNumberEdit m_editRC[2]; //Rightcenter
	CNumberEdit m_editBL[2]; //Bottomleft
	CNumberEdit m_editBC[2]; //Bottomcenter
	CNumberEdit m_editBR[2]; //Bottomright
	CNumberEdit m_editMC[2]; //Middlecenter
	CNumberEdit m_editExX;   //Extra X
	CNumberEdit m_editExY[2];//Extra Y

	CXTPSpinButtonCtrl m_spinTL[2];
	CXTPSpinButtonCtrl m_spinTC[2];
	CXTPSpinButtonCtrl m_spinTR[2];
	CXTPSpinButtonCtrl m_spinLC[2];
	CXTPSpinButtonCtrl m_spinRC[2];
	CXTPSpinButtonCtrl m_spinBL[2];
	CXTPSpinButtonCtrl m_spinBC[2];
	CXTPSpinButtonCtrl m_spinBR[2];
	CXTPSpinButtonCtrl m_spinMC[2];
	CXTPSpinButtonCtrl m_spinExX;
	CXTPSpinButtonCtrl m_spinExY[2];

	void ResizeIt(); // 重新调整控件大小和位置
	
	void SetControlsTheme(XTPControlTheme theme); // 设置控件风格

	void SetToolTips();

	void SetBuddys(); //将编辑框与微调按钮关联
	
	void ShowKeystoneCtrls(); // 显示四点校正的控件
	void ShowCurveCtrls();    // 显示曲面校正的控件
	void ShowDensityCtrls();  // 显示疏密校正的控件

private:
	// 绘图
	CRect m_rcPaint;     //绘图区大小
	CRect m_rcChart;     //工作区大小
	int m_nDeflate;      //工作区内缩尺寸，使工作区周边有空闲区域
	void GetPaintRect(); //获取绘图控制区位置

	double m_xScale;     //实际物理像素在绘图区上表现的大小 横轴
	double m_yScale;     //实际物理像素在绘图区上表现的大小 纵轴

	//几何校正方式 0：梯形 1：曲面 2：疏密
	emAjustStyle m_eAjustStyle;  

	void DrawBkgGrid(CDC *pValidDC);     //画背景
	void DrawKeystoneData(CDC *pValidDC);//画梯形校正
	void DrawCurveData(CDC *pValidDC);   //画曲面校正
	void DrawDensityData(CDC *pValidDC); //画疏密校正
	void DrawResolution(CDC *pValidDC);  //显示当前分辨率

	//四点校正控制变量
	//{
		BOOL m_bLBtnDownAt4Point;             //鼠标左键在控制点上按下-四点校正
		BOOL IsLBtnDownAt4Point(CPoint point);//判断鼠标左键是否在控制点上按下
		CPoint m_ptCursor4point;              //记录四点校正当前控制点的索引位置(x,y) x列号，y行号
		CDoublePoint m_dpaCursorPos4point;    //记录四点校正当前控制点的物理像素位置(posX, posY)
		void On4pointAjust(CPoint point, BOOL bMouseActive=TRUE); //进行四点校正, bMouseActive=TRUE表示由鼠标触发的校正
		void Reset4pointArray();              //四点校正复位
		void Show4pointCoords();              //刷新四点较正编辑框中控制点的值
	//}

	//曲面校正控制变量
	//{
		BOOL m_bLBtnDownAtCurve;              //鼠标左键在控制点按下-曲面校正
		BOOL IsLBtnDownAtCurve(CPoint point); //判断鼠标左键是否在控制点上按下
		CPoint m_ptCursorCurve;               //记录曲面校正当前控制点的索引位置(x,y) x列号，y行号
		CDoublePoint m_dpaCursorPosCurve;     //记录曲面校正当前控制点的物理像素位置(posX, posY)
		void OnCurveAjust(CPoint point, BOOL bMouseActive=TRUE);  //进行曲面校正, bMouseActive=TRUE表示由鼠标触发的校正
		void ResetCurveArray();               //曲面校正复位
		void ShowCurveCoords();               //刷新曲面校正编辑框中控制点的值
	//}

	//疏密校正控制变量
	//{
		BOOL m_bLBtnDownAtDensity;             //鼠标左键在控制点按下-疏密校正
		BOOL IsLBtnDownAtDensity(CPoint point);//判断鼠标左键是否在控制点上按下
		CPoint m_ptCursorDensity;              //记录疏密校正当前控制点的索引位置(x,y) x列号，y行号
		CDoublePoint m_dpaCursorPosDensity;    //记录疏密校正当前控制点的物理像素位置(posX, posY)
		void OnDensityAjust(CPoint point, BOOL bMouseActive=TRUE); //进行疏密校正, bMouseActive=TRUE表示由鼠标触发的校正
		void ResetDensityArray();              //疏密校正复位
		void ShowDensityCoords();              //刷新疏密校正编辑框中控制点的值
	//}

public:
	// 校正数据和操作
	CGridSetting m_gridsetting;                //记录当前板卡和校正有关的一些属性信息
	CDoublePointArray *m_pDpaOrigpoints;       //原始点坐标数组
	CDoublePointArray *m_pDpa4points;          //四点校正的点坐标数组
	CDoublePointArray *m_pDpaCurvepoints;      //曲面校正的点坐标数组
	CDoublePointArray *m_pDpaDensitypoints;    //疏密校正的点坐标数组

	void InitDpaPoints();  //分配内存，初始化
	void DeleteDpaData();  //回收内存

	void SetGraphicSize(); //设置绘图尺寸大小，并初始化GridSetting

	CDoublePoint OrignalPos(int x, int y);        //计算某个点的原始位置

	void Warp_Auto_Kestone();                     //四点校正 - 计算边缘
	void Warp_Auto_RowX(int row_y_index);         //四点校正 - 列均匀
	void Warp_Auto_RowY(int column_x_index);      //四点校正 - 行均匀

	void Fitting();                               //曲面校正 - 曲线拟合算法
	void Warp_Auto_XY();                          //曲面校正 - 曲线拟合后均匀所有点

	void Warp_Auto_Ranks_Translation();           //疏密校正 - 疏密度调整

public:
	inline int GetBoardPort();    //获取所属板卡的通信端口号
	inline void Packaging(TaskPackage &pack, int nMode); //任务包封装

	void Initialize();   //从板卡读取数据并初始化
	BOOL SaveGeoDatas(); //保存几何校正数据
	void Init4PointPosition(UINT *p4PointPos);   //初始化四点校正四个点的位置
	void InitCurvePosition(UINT *pCurvePos);     //初始化曲面校正九个点的位置
	void InitDensityPosition(UINT *pDensityPos); //初始化疏密校正九个点的位置

private:
	BOOL VKEnterInEditCtrls(HWND hWnd, double dOffset, BOOL bEnterActive=TRUE); //ENTER输入或者微调按钮设置坐标， bEnterAcitve=TRUE表示ENTER直接输入
	void OnVKArrowAjust(UINT nVKcode); //方向键移动控制点

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedAjustStyle(UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReset();
	afx_msg void OnDeltaposSpinPointPos(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
};
