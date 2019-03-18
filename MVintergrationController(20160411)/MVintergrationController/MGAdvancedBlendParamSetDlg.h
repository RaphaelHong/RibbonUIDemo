#pragma once
#include "resource.h"

//******************************************************************************************************************
//                                            CAdvancedBlendParamView.h
//******************************************************************************************************************

struct stSuppressibleButton
{
	CRect rcBtn;
	BOOL  bMouseOver;
	BOOL  bClicked;
};

typedef enum
{
	Nonius_Null     =  0,
	Nonius_LeftPos  =  1,
	Nonius_RigtPos  =  2,
	Nonius_IndvPos  =  3,
	Nonius_LeftVal  =  4,
	Nonius_RigtVal  =  5,
	Nonius_IndvVal  =  6,
} emNoniusType;

// CAdvancedBlendParamView 视图

class CAdvancedBlendParamView : public CView
{
	DECLARE_DYNCREATE(CAdvancedBlendParamView)

protected:
	CAdvancedBlendParamView();           // 动态创建所使用的受保护的构造函数
	virtual ~CAdvancedBlendParamView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	stSuppressibleButton m_ZonSelBtn[4];  //四个自动隐藏按钮
	CRect m_rcChart;      //坐标位置
	void InitMemberVariables();
	void InitButtons();
	void DrawBlendZoneSelectionButton(CDC *pValidDC, BOOL bShow, int ZoneSel/*融合带选择*/);   //画上下左右四个融合带选择按钮
	void DrawTriangle(CDC *pDC, const LPPOINT points, HBRUSH hbr);     //画三角形
	void DrawAxis(CDC *pDC);  //画坐标轴
	void DrawDataLine(CDC *pDC); //画数据
	void DrawRangeNonius(CDC *pDC); //画范围游标控制符(包括左右两个游标)
	void DrawIndivNonius(CDC *pDC); //画独立游标控制符
	void ShowBlendZoneBtn(CPoint point); //显示和隐藏融合带选择按钮
	void NoniusMoveProc(CPoint point);   //鼠标控制游标滑动
	BOOL IsMouseOverNonius(CPoint point, emNoniusType &eNoniustype); //鼠标左键是否在游标上按下
	void ResetCtrlVariables();           //重置控制变量
	void AverageBlendData();     //均分左右两个游标之间的融合值
	void WriteBlendParaData(int nBegin, int nEnd);   //写入融合数据
	

	BOOL m_bLBtnDownInNonius; //鼠标左键在游标上按下
	emNoniusType m_eNoniusType; //游标类型
	emNoniusType m_eCurNonius; //当前受控制游标
	
public:
	UINT m_BlendParamData[1024]; //融合参数值
	int m_BlendWidth;              //当前融合带宽度
	int m_nChannel;                //当前通道
	int m_nPureWhite;
	double alpha;
	double p;
	double gamma;
	int m_nSide;

	int  m_nRightNonius;    //右游标对应的融合带像素位置
	int  m_nLeftNonius;     //左游标对应的融合带像素位置
	int  m_nIndivNonius;    //独立游标对应的融合带像素位置

	bool SetBlenddParaData(); //获取融合参数
	void SetNoniusState(int nNoniusType, int nNoniusVal);
	void OnVKdown(UINT vkType); //键盘方向键响应函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


//******************************************************************************************************************
//                                            CAdvancedBlendParamSetDlg.h
//******************************************************************************************************************

// CAdvancedBlendParamSetDlg 对话框

class CAdvancedBlendParamSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedBlendParamSetDlg)

public:
	CAdvancedBlendParamSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdvancedBlendParamSetDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADVANCED_BLENDPARAM };

private:
	CAdvancedBlendParamView *pView;
	void InitViewData();
	BOOL CreateView();
	void OnVirtrulKeyEnter(CWnd *pDataWnd);
	void SetSpinButtonBuddy();

public:
	int m_nPort;
	int m_nBlendWidth;
	int m_PureWhite;
	double blend_alpha;
	double blend_p;
	double blend_gamma;
	int m_nBldSide;

	int m_nUpperPos;
	int m_nUpperVal;
	int m_nLowerPos;
	int m_nLowerVal;
	int m_nIndivPos;
	int m_nIndivVal;
	UINT m_nBlendVal[1024];
	void UpdateDlgData(int nPosLeft, int nValLeft, int nPosRight, int nValRight, int nPosIndiv, int nValIndiv);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnDeltaposSpinNonius(NMHDR *pNMHDR, LRESULT *pResult);
};
