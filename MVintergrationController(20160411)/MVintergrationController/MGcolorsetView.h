#pragma once
#include "ColorArray.h"
#include "MsgTipDlg.h"

typedef enum emColorAjustType
{
	eRoughType      =  0x00,  //粗调
	ePreciseType    =  0x01,  //微调
};

typedef enum emColorAjustMethod
{
	eLinearMode  =   0x00,   //线性模式
	eRegionMode  =   0x01,   //区域模式
};

typedef enum emColor
{
	eRed   = 0,
	eGreen = 1,
	eBlue  = 2,
	eWhite = 3
};

// CcolorsetView 视图

class CcolorsetView : public CView
{
	DECLARE_DYNCREATE(CcolorsetView)

protected:
	CcolorsetView();           // 动态创建所使用的受保护的构造函数
	virtual ~CcolorsetView();

public:
	void SetAjustMethod(const emColorAjustMethod eMethod) { m_AjustMethod = eMethod; }
	void ExchangeColor(const emColor color) { m_eColor = color; }
	void InitMembers();
	void SetTestType(const int nType) { m_nTestType = nType; }
	int GetTestType() const { return m_nTestType; }

	void SetColor(int *data, int nColorSel);
	void TestColor();
	void CancelTest();
	void WriteColor();
	void ResetColor();

	void OnVKeyUpDown(UINT nVKCode);   //键盘上下键 0:Up 1:Down

private:
	CRect m_rcChart;  //表格在界面绘制的位置

	CColorArray m_ColorArray[4];  // 0:Red 1:Green 2:Blue 3:Pure White

	emColorAjustType m_AjustType;     // 粗调/微调
	emColorAjustMethod m_AjustMethod; // 线性/分段
	emColor m_eColor;    //颜色选择 0:Red 1:Green 2:Blue 3:Pure White
	int m_nColorSection; //颜色段 0:0-31 1:32-63 2:64-95 …
	int m_nCtrlPointIndex; //控制点位置

	BOOL m_bLBtnDownAtCtrlPoint; //鼠标左键点击控制点
	int  m_nTestType;  //背景测试、信号图像测试

	double m_Xscale;  //颜色代表区域宽度与实际颜色的比例-横轴 标准值
	double m_Yscale;  //颜色代表区域高度与实际颜色的比例-纵轴 校准值

	void CalcChartPos(CRect parentRc);   //计算合适的表格位置

	void DrawTitle(CDC *pValidDC);        //表格标题
	void DrawChart(CDC *pValidDC);        //绘表格
	void DrawRoughChart(CDC *pValidDC);   //粗调表格
	void DrawPreciseChart(CDC *pValidDC); //微调表格

	void DrawCurves(CDC *pValidDC);       //画数据线
	void DrawRoughCurve(CDC *pValidDC);   //粗调数据线
	void DrawPreciseCurve(CDC *pValidDC); //微调数据线

	BOOL IsLBtnDownAtColorCtrlPoint(CPoint pt); //鼠标是否点击控制点
	int  GetColorSectionIndex(CPoint point);    //获取当前微调的颜色段

	void AjustColor(int nColorValue); //颜色校准

private:
	CMsgTipDlg *m_pMsgTip;
	void ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip, BOOL bDockAtCursor=TRUE, CPoint ptDockAt=CPoint(0,0)); //显示提示框
	void GetMsgTipInfo(CMsgTipArray &arMsgTip, int nCtrlPtIndex);          //设置提示信息内容
	int  IsMouseOverColorCtrlPoint(CPoint point);   //鼠标是否在控制点上悬停

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


