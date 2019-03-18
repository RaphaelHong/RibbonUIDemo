// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// QuickSwitchView.h : CQuickSwitchView 类的接口
//
#pragma once

#include "VirtualSwitcher.h"
#include "Dialog components/MachinePanelDlg.h"

class CVideoRouterDoc;

class CQuickSwitchView : public CScrollView
{
public: // 仅从序列化创建
	CQuickSwitchView();
	DECLARE_DYNCREATE(CQuickSwitchView)

public:
	CVideoRouterDoc *GetDocument() const;

	// 属性
public:

	//滚动视图控制
	CPoint m_ptViewOrg;  //视图坐标原点
	CSize m_sizeTotal;   //滚动视图总大小，在此相当于虚拟矩阵坐标尺寸
	CSize m_sizePage;    //滚动一页，视图移动尺寸
	CSize m_sizeLine;    //滚动一行，视图移动尺寸

	void OnVKScroll(UINT nVKCode); //方向键、Home键、End键、Page键滚动视图

public:
	//内存DC
	CDC *m_pMemDC;    //内存DC
	CSize m_LastSize; //视图最后一次调整尺寸后的大小

	//鼠标动作处理
	BOOL m_bResizeFirstCol;           //是否调整第一列宽度
	BOOL m_bLBtnDown;                 //当前鼠标左键是否按下
	BOOL m_bShowHint;                 //控制视图是否绘制提示

	CPoint m_ptLBtnDown;              //鼠标左键按下位置
	CPoint m_ptRBtnUp;                //鼠标右键弹起位置
	CPoint m_ptCursor;                //鼠标当前位置

	CPoint m_ptCaptureMatrix;         //鼠标当前悬停网格号
	CPoint m_ptLastCapMatrix;         //鼠标上一次悬停网格号

	int m_nCurInputID;              //记录鼠标左键点击的输入口号
	int m_nCurSubInput;
	int m_nCurOutputID;             //记录鼠标左键点击的输出口号
	int m_nCurSubOutput;

	void InitVariables();                     //初始化所有成员变量
	void ResetVariables();                    //重置控制量
	void OnMouseHangOver(CPoint ptMouse);     //鼠标悬停事件
	BOOL IsResizeFirstColumn(CPoint ptMouse); //调整第一列宽度事件处理，并设置鼠标形态
	void ResizeFirstColumn(CPoint ptMouse);   //调整第一列宽度 

	void GetSelectedPortID(__in int nRowIndex, __in int nColIndex, __out int &nInputID, __out int &nSubInput, __out int &nOutputID, __out int &nSubOutput);
	int GetGridRowEx(__in CPoint ptSignalID);

	// 操作
public:
	CVirtualSwitcher *m_pSwitcher;      //虚拟矩阵网格管理对象

	int m_nOpacity; //图层不透明度

	void InitMatrix();
	void InitMatrixEx(const int nInCount, const UINT* pInLogicPort, const int nOutCount, const UINT* pOutLogicPort);
	//void DrawVirtualMatrix(CDC *pValidDC);
	void DrawHint(CDC *pValidDC);
	void CreateMemDC(CDC *pDevDC, CRect rcBmp);        //创建内存DC
	void DrawBk(CDC *pValidDC);           //不需要重画的部分
	void DrawShadowFrame(CDC *pValidDC);  //不需要重画的部分
	//将需要重绘的部分提出来
	void DrawSourceName(CDC *pValidDC);   //绘信号源名称与图标
	void DrawMarkedGrid(CDC *pValidDC);   //矩阵切换标记
	void DrawResizeBar(CDC *pValidDC);    //列宽调整标记

public:
	//连接后初始化
	BOOL InitDevice();

	//获取背板上所有插槽的排列情况
	int CmdGBMG(int iPort, BACKBOARDINFO &bbi);
	//读取每个插槽上板卡的信息
	int CmdRSMG(int iPort, int iSlotPort, UINT &iCardID, UINT *pLogicPorts, UINT &iLogicPortCount);
	//保存场景
	int CmdSAVE(int iPort, const UINT iSceneID, const UINT iOutportCount, UINT *pInputPorts);
	//加载EDID
	int CmdWEDI(int iPort, UINT *pEdidData, int iLen);

public:
	CMachinePanelDlg *m_pMachinePanel;
	BOOL CreateMachinePanel();

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CQuickSwitchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	//afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMenuVGAProperty();
	afx_msg void OnMenuOSDDesign();
	afx_msg void OnMenuLoadEDID();
	afx_msg void OnMenuRename();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMenuShowDevPanel();
	afx_msg void OnUpdateCmdShowDevPanel(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // VedioRouterView.cpp 中的调试版本
inline CVideoRouterDoc* CQuickSwitchView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif