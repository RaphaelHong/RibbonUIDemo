#pragma once
#include "..\ResolutionManager.h"
#include "..\DrawWndPage.h"
#include "NumberEdit.h"
#include "IconListBox.h"

// CSpliceSettingDlg 对话框

class CSpliceSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpliceSettingDlg)

public:
	CSpliceSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSpliceSettingDlg(pSPLICELAYOUT pLayout, CDrawWndPage* pPage, CWnd* pParent=NULL);
	virtual ~CSpliceSettingDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SPLICE_SET };

	enum emScrOption {
		eScrSimplySetup  = 0,
		eScrComplexSetup = 1,
		eScrCustomized   = 2,
	};

protected:

	int m_iTimingIndex;

	// 组合模式
	int m_iPhyHorz;
	int m_iPhyVert;
	int m_iLogHorz;
	int m_iLogVert;
	CSpinButtonCtrl m_spinPhyHorz;
	CSpinButtonCtrl m_spinPhyVert;
	CSpinButtonCtrl m_spinLogHorz;
	CSpinButtonCtrl m_spinLogVert;
	CNumberEdit m_EditPhyHorz;
	CNumberEdit m_EditPhyVert;
	CNumberEdit m_EditLogHorz;
	CNumberEdit m_EditLogVert;

	// 简易融合拼接
	int m_iSpaceH;
	int m_iSpaceV;
	CSpinButtonCtrl m_spinSpaceH;
	CSpinButtonCtrl m_spinSpaceV;
	CNumberEdit m_EditSpaceH;
	CNumberEdit m_EditSpaceV;

	// 复杂融合拼接
	int m_iBlendWidH;
	int m_iBlendWidV;
	CNumberEdit m_EditBlendWidH;
	CNumberEdit m_EditBlendWidV;
	CSpinButtonCtrl m_spinBlendWidH;
	CSpinButtonCtrl m_spinBlendWidV;

	// 自订拼接
	int m_iScrPosX;
	int m_iScrPosY;
	int m_iScrWidth;
	int m_iScrHeight;
	CSpinButtonCtrl m_spinScrPosX;
	CSpinButtonCtrl m_spinScrPosY;
	CSpinButtonCtrl m_spinScrWidth;
	CSpinButtonCtrl m_spinScrHeight;
	CNumberEdit m_EditScrPosX;
	CNumberEdit m_EditScrPosY;
	CNumberEdit m_EditScrWidth;
	CNumberEdit m_EditScrHeight;

	CButton m_RadioSimplySetup;
	CButton m_RadioComplexSetup;
	CButton m_RadioCustomize;	

	CIconListBox m_ListScr;

	CSliderCtrl m_sliderTiming;

	CScrollBar m_HScroll;
	CScrollBar m_VScroll;

	CToolTipCtrl m_tips;
	BOOL SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pRes) ;

private:
	CResolutionManager m_resManager;  //拼接分辨率管理器
	pSPLICELAYOUT m_pLayout; //拼接属性设置
	CDrawWndPage* m_pPage;   //拼接管理器指针

	// 滑动条滚动分辨率
	void OnScrollTiming(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// 显示当前分辨率信息
	void ShowSelectedTiming(int nTimingIndex);
	// 当选择的分辨率发生变化时，更新某些微调按钮的微调范围
	void UpdateSpinRange(const TIMING &st);

	// 添加新的分辨率
	void AddNewResolution(const TIMING &timing);
	// 修改分辨率
	void ModifyResolution(const TIMING &timing);
	// 删除分辨率
	void DeleteResolution(const TIMING &timing);

	// 重绘示例图
	void RepaintPic();

	// 最近的设置
	BOOL SaveState();
	BOOL LoadState();

protected:
	emScrOption m_eScrOpt;  //屏幕设置选择模式 0:eScrCustomized(自订模式) 1:eScrSimplySetup(简易模式)
	// 切换到自订按钮
	void OnRadioCustomize();
	// 切换到简单融合设置按钮
	void OnRadioSimplySetup();
	// 切换到复杂融合设置按钮
	void OnRadioComplexSetup();

	// 根据模式隐藏和显示一部分控件
	void EnableControls();

	CImageList m_imglist;
	void InitImageList();
	// 重置屏幕列表
	void ResetListBox();

	pSCREENMAP m_pScrInf; //记载所有屏幕位置信息的数组
	int m_iScreens; //总屏幕数
	// 自动设置各屏幕的信息
	void AutoScreenInfo();
	// 获取某个屏幕的信息
	void GetScreenInfo(int iScreen);
	// 设置某个屏幕的信息
	void SetScreenInfo(int iScreen);

	int* m_pHorzBlend; //所有水平方向的融合带 程序中会额外的多出头尾两个为0的融合带，方便设计
	int* m_pVertBlend; //所有垂直方向的融合带 程序中会额外的多出头尾两个为0的融合带，方便设计
	int  m_icurselBlendH;  //当前选中的水平融合带
	int  m_icurselBlendV;  //当前选中的垂直融合带
	// 获取融合带信息
	void GetBlendInfo(int icurselBlendH, int icurselBlendV);
	// 设置融合带信息
	void SetBlendInfo(int icurselBlendH, int icurselBlendV);
	// 根据融合带值计算各屏幕坐标
	void CalcScreenPosAccordingtoBlendInfo();
	void EnableBlendEdit(int icurselBlendH, int icurselBlendV);

	// 鼠标在示例图上的按下处理
	void OnLBtnDownProc(CPoint point); 

	int m_iCurselScreen; //列表框当前选择的项
	BOOL m_bDataSaved; //用户变更了屏幕的属性值，是否已经应用

	// 当用户更改了编辑框的值，提醒用户应用。
	void RemindtoApplySetting();

private:
	// 绘制相关
	CBitmap m_bmpMonitor;
	BITMAP m_bmpInfo;

	// 示例图上一个屏幕所占的大小
	struct ScreenItemMargin 
	{
		int nItemWidth;
		int nItemHeight;
		int nItemSpace;
	} m_ScrItemMargin;

	void SetScreenItemMargin();
	ScreenItemMargin GetScreenItemMargin() const { return m_ScrItemMargin; }
	INT_PAIR GetMaxVisibleScreens();

	void DrawPicture(CDC* pDC);

	// 滚动绘制
	int m_iHPos;
	int m_iVPos;
	int m_iMaxHScroll;
	int m_iMaxVScroll;
	void OnScrollScreen(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar, UINT nSBType);
	void ResetScrollRange();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnOK();
	afx_msg void OnBtnClickedAddTiming();
	afx_msg void OnBtnClickedDelTiming();
	afx_msg void OnBtnClickedModTiming();
	afx_msg void OnRangeEnChangeEditCombineMode(UINT nID);
	afx_msg void OnRangeEnChangeEditScrSpace(UINT nID);
	afx_msg void OnRangeEnChangeEditScrPosition(UINT nID);
	afx_msg void OnRangeEnChangeEditScrBlend(UINT nID);
	afx_msg void OnBnClickedBtnUpdateGridline();
	afx_msg void OnRangeRadioScreenOption(UINT nID);
	afx_msg void OnLbnChangeListScreen();
	afx_msg void OnBtnApplyScrSettings();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
