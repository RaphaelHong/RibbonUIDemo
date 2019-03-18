#pragma once
#include "BaseBoard.h"
#include "MGtimingsetDlg.h"
#include "MGcolorsetDlg.h"
#include "MGblendsetDlg.h"
#include "MGblackLvlSetDlg.h"
#include "MGFilterDlg.h"
#include "MGeometricAjustDlg.h"
#include "MGgridwarpDlg.h"

// CMGeomoduleDlg �Ի���

class CMGeoMainDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGeoMainDlg)

public:
	CMGeoMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMGeoMainDlg();

// �Ի�������
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
	void Initialize(); //��ʼ����������
	void GetCurrencyTiming(TIMING_STRUCT &ts); //��ȡ��ǰ�ֱ���
	int GetCurrencyBlendWidth(int nBlendSide); //��ȡ��ǰ�ںϴ����
	void NotifyBlendWidthChanged(); //֪ͨ�ڵ�ƽ�ںϴ������������
	void GetGeoDpaDatas(CDoublePointArray *&p4Point, CDoublePointArray *&pCurve, CDoublePointArray *&pDensity);
	BOOL SaveAll();

public:
	CBaseBoard m_boardInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
