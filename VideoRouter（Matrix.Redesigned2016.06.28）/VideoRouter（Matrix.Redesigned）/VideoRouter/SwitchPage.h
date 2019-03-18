#pragma once

#include "SwitchHeadBar.h"
#include "SwitchSourceBar.h"
#include "SwitchSelectBar.h"
#include "PortHelpText.h"

// 切换器的尺寸规模
class CSwitchScale 
{
public:
	CSwitchScale() {}
	CSwitchScale(const CSwitchScale & scale) {
		iSelectBoxSize = scale.iSelectBoxSize;
		iSourceBarWidth = scale.iSourceBarWidth;
		iHorzSpace = scale.iHorzSpace;
		iVertSpace = scale.iVertSpace;
		iFontHeight = scale.iFontHeight;
	}

	int iSelectBoxSize;
	int iSourceBarWidth;
	int iHorzSpace;
	int iVertSpace;
	int iFontHeight;
};
// 大尺寸 按钮像素大小24x24
class CLargeScale : public CSwitchScale
{
public:
	CLargeScale () 
	{
		iSelectBoxSize  = 24;
		iSourceBarWidth = 190;
		iHorzSpace      = 10;
		iVertSpace      = 10;
		iFontHeight     = 16;
	}
};
// 中尺寸 按钮像素大小18x18
class CMediumScale : public CSwitchScale
{
public:
	CMediumScale ()
	{
		iSelectBoxSize  = 20;
		iSourceBarWidth = 190;
		iHorzSpace      = 8;
		iVertSpace      = 8;
		iFontHeight     = 12;
	}
};
// 小尺寸 按钮像素大小12x12
class CSmallScale : public CSwitchScale
{
public:
	CSmallScale ()
	{
		iSelectBoxSize  = 16;
		iSourceBarWidth = 190;
		iHorzSpace      = 7;
		iVertSpace      = 7;
		iFontHeight     = 10;
	}
};

/* 切换器视图尺寸规模，总共有大(24Pixel)、中(18Pixel)、小(12Pixel)三种规模。*/
enum emZoomLevel
{
	eZLSmall  = 1,
	eZLMedium = 2,
	eZLLarge  = 3,
};

// 基础栏边距属性
class CBasicBarMargins
{
public:
	int iBarWidth;           // 栏宽度
	int iBarHeight;          // 栏高度

	int iItemWidth;          // 项宽度
	int iItemHeight;         // 项高度

	int iMaxVisiableItemsOnHorz;   // 栏中水平方向最大可见项数
	int iMaxVisiableItemsOnVert;   // 栏中垂直方向最大可见项数
};

// 头目栏边距属性
class CHeadBarMargins : public CBasicBarMargins
{
public:
	int iColumnWidthFst;   // 第一栏宽度 （与SourceBar宽度相等）
	int iColumnWidthSec;   // 第二栏宽度
	int iColumnWidthThd; // 第三栏宽度 （第二栏与第三栏宽度之和等于SelectBar宽度）
};

// 信号源栏边距属性
typedef CBasicBarMargins CSourceBarMargins;

// 切换按钮栏边距属性
typedef CBasicBarMargins CSelectBarMargins;

// 控制页边距属性
class CPageMargins
{
public:
	int iPageWidth;        // 页面宽度
	int iPageHeight;       // 页面高度

	int iTopBorderHeight;  // 上边框高度
	int iBotBorderHeight;  // 下边框高度

	//int iFontHeight;       // 字体大小

	CHeadBarMargins   HeadBarMargin;
	CSourceBarMargins SourceBarMargin;
	CSelectBarMargins SelectBarMargin;

	CSwitchScale PageScale;
};

//////////////////////////////////////////////////////////////////////////
// SwitchPage
// 一个矩阵切换器的逻辑视控制页 包含一个头部输出控制栏，一个左侧信号控制栏和一个切换选择按钮栏

class CSwitchPage : public CWnd
{
	friend class CSwitchHeadBar;
	friend class CSwitchSourceBar;
	friend class CSwitchSelectBar;

	DECLARE_DYNAMIC(CSwitchPage)
public:
	CSwitchPage();
	virtual ~CSwitchPage();

	//static int m_sFirstVisibleInput;  // 第一个可见的输入口
	//static int m_sFirstVisibleOutput; // 第一个可见的输出口
	static emZoomLevel m_sZoomLvl;  // 当前页面尺寸规模
	
	// 设置切换器的坐标
	void SetPageRect(const CRect rc);

	// 获取当前边距属性
	const CPageMargins* GetMargins() { return &m_Margins; }
	void SetMargins();      /*设置边距*/

	void RecalLayout();

private:	
	CRect m_rcPage;   /* 切换器坐标 */

	CPageMargins m_Margins; /*页面边距*/
	
	CSwitchHeadBar m_wndHeadBar;      // 头栏
	CSwitchSourceBar m_wndSourceBar;  // 信号栏
	CSwitchSelectBar m_wndSelectBar;  // 切换栏

	BOOL CreateHeadBar();//创建头部栏
	BOOL CreateSourceBar();//创建信号源栏
	BOOL CreateSelectBar();//创建选择栏
	inline void CalcHeadBarPos(CRect &rcPos);//计算头部栏的位置
	inline void CalcSourceBarPos(CRect &rcPos);//计算信号源栏的位置
	inline void CalcSelectBarPos(CRect &rcPos);//计算选择栏的位置

	CWnd* GetHeadBar();//获取头部栏的窗口指针
	CWnd* GetSourceBar();//获取信号源栏的窗口指针
	CWnd* GetSelectBar();//获取选择栏的窗口指针

private:
	
	CScrollBar m_Horzbar;// 水平滚动条，用以滚动输出端口，当前视图大小不足以显示所有输出端口的时候，此滚动条就会在顶部出现，反之则会隐藏
	void SetHScrollBar();// 创建滚动条

	// 下边框上的翻页按钮
	CBnItem m_bnFarBottom;//滚动到最底部按钮
	CBnItem m_bnPageDown;//向下翻页按钮
	void SetBnItem();//创建按钮

	// 按钮提示
	CToolTipCtrl m_tips;//动态提示条
	void SetTooltips();//创建提示条

	// 端口提示框
	CPortHelpText* m_pHelptext;//信号源信息提示框指针
	void SetNeedHelpPort(const int nID, const int nType);//创建信号源信息提示框
	INT_PAIR GetNeedHelpPort();//获取当前需要信息提示的端口
	void ShowHelpText(BOOL bShow, CStringW strText=L"", CPoint ptText=CPoint(0,0));//控制显示提示框
	BOOL IsHelpTextVisible();//信号源提示框是否可见
	void RedrawHelpText();//刷新信号源提示框

public:

	// 自绘按钮的Click事件处理
	void RefreshAll();  //刷新所有
	void FarBottom();   //信号栏滚动到最底端
	void PageDown();    //信号栏向下滚动一页
	void FarTop();      //信号栏滚动到最顶端
	void PageUp();      //信号栏向上滚动一页
	void FarLeft();     //输出口列表滚动到最左边
	void PageLeft();    //输出口列表向左滚动一页
	void FarRight();    //输出口列表滚动到最右边
	void PageRight();   //输出口列表向右滚动一页

	void ManageWizardBn(); //对所有滚动按钮的使能处理

public:

	// 将场景打包到文件
	void PackFile(__in LPCTSTR lpFile);
	// 解析场景文件
	void ParseFile(__in LPCTSTR lpFile);
	// 解析场景数据块
	void ParseSceneBlock(__in UINT* pBlock, __in int nBlockLen);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnFarBottom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPageDown(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


