#pragma once
#include "MySliderCtrl.h"

// CSourcePropertyDlg 对话框

class CSourcePropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSourcePropertyDlg)

public:
	CSourcePropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSourcePropertyDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_SOURCE_PROPERTY };

	CMySliderCtrl m_sliderLightness;
	CMySliderCtrl m_sliderContrast;
	CMySliderCtrl m_sliderPhase;
	CMySliderCtrl m_sliderFreq;
	CMySliderCtrl m_sliderLeft;
	CMySliderCtrl m_sliderTop;
	CMySliderCtrl m_sliderWidth;
	CMySliderCtrl m_sliderHeight;
	CMySliderCtrl m_sliderR;
	CMySliderCtrl m_sliderG;
	CMySliderCtrl m_sliderB;

	CComboBox m_cboxColorSpace;

	int m_nLightness;
	int m_nContrast;
	int m_nPhase;
	int m_nFreq;
	int m_nLeft;
	int m_nTop;
	int m_nWidth;
	int m_nHeight;
	int m_nRed;
	int m_nGreen;
	int m_nBlue;

	int m_nMidPosLeft;
	int m_nMidPosTop;
	int m_nMidPosWidth;
	int m_nMidPosHeight;

	void InitSlider();

	//从设备获取数据初始化
	void Initialize();

	void UpdateSlider();

	// 除CVBS信号外，其它信号不允许设置亮度对比度相位频率等，只可以这是输入位置和大小
	void EnableControls();

public:
	int m_nSourcePort;
	emVideoType m_eSourceType;

	void OnSliderLBtnUp(CMySliderCtrl* pSlider);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnDefaultBrightnessContrast();
	afx_msg void OnBtnDetectBrightnessContrast();
	afx_msg void OnBtnApplyBrightnessContrast();
	afx_msg void OnBtnDefaultSample();
	afx_msg void OnBtnAutoSample();
	afx_msg void OnBtnDetectSample();
	afx_msg void OnBtnApplySample();
	afx_msg void OnBtnSetColorspace();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnMsgSliderLBtnUp(WPARAM wParam, LPARAM lParam);
};
