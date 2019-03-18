#pragma once
#include "BaseBoard.h"
#include "MGtimingsetDlg.h"
#include "MGcolorsetDlg.h"
#include "MGblendsetDlg.h"
#include "MGblackLvlSetDlg.h"
#include "MGFilterDlg.h"
#include "MGeometricAjustDlg.h"
#include "MGgridwarpDlg.h"

// CMGeomoduleDlg 对话框

class CMGeoMainDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGeoMainDlg)

public:
	CMGeoMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGeoMainDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG };

	CMGtimingsetDlg m_TimingSetDlg;
	CMGcolorsetDlg m_ColorSetDlg;
	CMGblendsetDlg m_BlendSetDlg;
	CMGblackLvlSetDlg m_BlacklvlSetDlg;
	CMGFilterDlg m_FilterDlg;
	CMGeometricAjustDlg m_GeometricDlg;
	CMGgridwarpDlg m_GridWarpDlg;

	void CreateSubWindows();

public:
	void SetBoardInfo(CBaseBoard boardinfo);
	void Initialize(); //初始化窗口数据
	void GetCurrencyTiming(TIMING_STRUCT &ts); //获取当前分辨率
	int GetCurrencyBlendWidth(int nBlendSide); //获取当前融合带宽度
	void NotifyBlendWidthChanged(); //通知黑电平融合带宽度重新设置
	void GetGeoDpaDatas(CDoublePointArray *&p4Point, CDoublePointArray *&pCurve, CDoublePointArray *&pDensity);
	BOOL SaveAll();

public:
	CBaseBoard m_boardInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CXTPTabControl m_wndTabCtrl;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSelectChanged(NMHDR* pNMHDR, LRESULT* pResult);
};
