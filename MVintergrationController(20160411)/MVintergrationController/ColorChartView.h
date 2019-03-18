#pragma once
#include "MsgTipDlg.h"
#include "ColorArray.h"

#define BIG_CTRL_NODE  9
#define TINY_CTRL_NODE 32
#define MAX_BUTTON_NUM 7

// 粗调 微调
typedef enum emColorAjustType
{
	eRoughType   =   0x00,  //粗调
	ePreciseType =   0x01   //微调
};


typedef enum emColorAjustMode
{
	eLinearMode  =   0x00,   //线性模式
	eRegionMode  =   0x01,   //区域模式
};

typedef enum emButtonID
{
	eBtn_Red     =   0x00,   //红色按钮
	eBtn_Green   =   0x01,   //绿色按钮
	eBtn_Blue    =   0x02,   //蓝色按钮
	eBtn_White   =   0x03,   //联动按钮
	eBtn_Reset   =   0x04,   //复位按钮
	eBtn_Linear  =   0x05,   //线性方式按钮
	eBtn_Region  =   0x06,   //区域方式按钮	
};

struct stMyButton
{
	int      nID;            //按钮索引号
	CRect    rcBtn;			 //按钮坐标
	COLORREF crBtnNormal;	 //按钮正常颜色
	COLORREF crBtnMouseOver; //鼠标经过按钮时的颜色
	COLORREF crBtnClicked;   //鼠标被按下时的颜色
	BOOL     bMouseOver;     //鼠标是否经过按钮
	BOOL     bClicked;       //按钮是否被按下
	BOOL     bRealesed;      //按钮是否弹起
	BOOL     bAlwaysHighLight;//一直处于高亮状态
	CString  strFirstName;    //按钮标题
	BOOL     DrawMyButton(CDC* pDC);
	BOOL     IsMouseOverBtn(CPoint ptCursor);
	BOOL     IsButtonClicked(CPoint ptCursor);
};

// CColorChartView 视图

class CColorChartView : public CView
{
	DECLARE_DYNCREATE(CColorChartView)

protected:
	CColorChartView();           // 动态创建所使用的受保护的构造函数
	virtual ~CColorChartView();

public:
	BOOL m_bTPenable;                 //开启纯色测试环境
	int  m_nBlendOption;              //去除融合带影响
	int  m_nChannel;                  //当前投影机
	void CmdCancelBlending();         //取消融合带影响
	void CmdWriteColor();             //实时写入色彩数据
	void CmdTestColor(int nPort);              //测试颜色
	void CmdCancelColorTest();        //取消颜色测试
	void CmdSaveColor();              //固化色差校正数据

	void SetColor(int *data, int nColorSel, int nChannel); //从设备读取色彩

	void OnVKeyUpDown(UINT nVKCode);   //键盘上下键 0:Up 1:Down

private:
	
	CColorArray m_ColorArray[7][4];    //7个通道，0:Red 1:Green 2:Blue 3:White
	
	int m_nColorSection;    //微调的颜色段
	int m_nCtrlPointIndex;  //当前色彩控制点
 
	stMyButton m_myButton[MAX_BUTTON_NUM];
	CRect m_rcChart;         //表格位置

	emColorAjustType m_emColorAjustType;  //粗调/微调
	emColorAjustMode m_emColorAjustMode;  //线性/区域
	//BOOL m_bLinkage;                      //是否联动
	BOOL m_bLBtnDownInCtrlPoint;          //鼠标左键单击控制点

	void InitVariables();                 //初始化成员
	void InitButtons();                   //初始化按钮成员
	void DrawButtons(CDC *pValidDC);      //画控制按钮
	void CalcButtonPos(CDC *pValidDC);    //计算合适的按钮位置
	void DrawTitles(CDC *pValidDC);       //画标题
	void CalcChartPos(CRect *pParentRect);//计算合适的表格位置
	void DrawChart(CDC *pValidDC);        //画背板表格
	void DrawRoughChart(CDC *pValidDC);   //画背板表格 - 粗调
	void DrawPreciseChart(CDC *pValidDC); //画背板表格 - 微调
	void DrawCurves(CDC *pValidDC);       //画色彩曲线
	void DrawRoughCurve(CDC *pValidDC);   //画色彩曲线 - 粗调
	void DrawPreciseCurve(CDC *pValidDC); //画色彩曲线 - 微调

	void IsMyButtonClicked(CPoint point); //鼠标左键点击在按钮上
	void OnMyButtonClicked(emButtonID eID); //自定义按钮鼠标左键按下处理

	void ResetColor();                       //复位颜色
	int  GetColorSectionIndex(CPoint point); //获取当前微调的颜色段

	void AjustColor(int nColorValue); //颜色校准

	BOOL IsLBtnDownAtColorCtrlPoint(CPoint point);  //鼠标左键是否在色彩控制点上按下
	int  IsMouseOverColorCtrlPoint(CPoint point);   //鼠标是否在控制点上悬停

	int  GetSectionMinValue(int nSectionIndex);  //获取某段最小的校准值


	CMsgTipDlg *m_pColorMsgTipDlg;                            //浮动信息框
	void ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip); //显示提示框
	void GetMsgTipInfo(CMsgTipArray &arMsgTip, CPoint ptCursor, int nCtrlPtIndex);          //设置提示信息内容

	
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


