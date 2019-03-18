#pragma once
#include "..\ResolutionManager.h"
#include "..\DrawWndPage.h"
#include "NumberEdit.h"
#include "IconListBox.h"

// CSpliceSettingDlg �Ի���

class CSpliceSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpliceSettingDlg)

public:
	CSpliceSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CSpliceSettingDlg(pSPLICELAYOUT pLayout, CDrawWndPage* pPage, CWnd* pParent=NULL);
	virtual ~CSpliceSettingDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SPLICE_SET };

	enum emScrOption {
		eScrSimplySetup  = 0,
		eScrComplexSetup = 1,
		eScrCustomized   = 2,
	};

protected:

	int m_iTimingIndex;

	// ���ģʽ
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

	// �����ں�ƴ��
	int m_iSpaceH;
	int m_iSpaceV;
	CSpinButtonCtrl m_spinSpaceH;
	CSpinButtonCtrl m_spinSpaceV;
	CNumberEdit m_EditSpaceH;
	CNumberEdit m_EditSpaceV;

	// �����ں�ƴ��
	int m_iBlendWidH;
	int m_iBlendWidV;
	CNumberEdit m_EditBlendWidH;
	CNumberEdit m_EditBlendWidV;
	CSpinButtonCtrl m_spinBlendWidH;
	CSpinButtonCtrl m_spinBlendWidV;

	// �Զ�ƴ��
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
	CResolutionManager m_resManager;  //ƴ�ӷֱ��ʹ�����
	pSPLICELAYOUT m_pLayout; //ƴ����������
	CDrawWndPage* m_pPage;   //ƴ�ӹ�����ָ��

	// �����������ֱ���
	void OnScrollTiming(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// ��ʾ��ǰ�ֱ�����Ϣ
	void ShowSelectedTiming(int nTimingIndex);
	// ��ѡ��ķֱ��ʷ����仯ʱ������ĳЩ΢����ť��΢����Χ
	void UpdateSpinRange(const TIMING &st);

	// ����µķֱ���
	void AddNewResolution(const TIMING &timing);
	// �޸ķֱ���
	void ModifyResolution(const TIMING &timing);
	// ɾ���ֱ���
	void DeleteResolution(const TIMING &timing);

	// �ػ�ʾ��ͼ
	void RepaintPic();

	// ���������
	BOOL SaveState();
	BOOL LoadState();

protected:
	emScrOption m_eScrOpt;  //��Ļ����ѡ��ģʽ 0:eScrCustomized(�Զ�ģʽ) 1:eScrSimplySetup(����ģʽ)
	// �л����Զ���ť
	void OnRadioCustomize();
	// �л������ں����ð�ť
	void OnRadioSimplySetup();
	// �л��������ں����ð�ť
	void OnRadioComplexSetup();

	// ����ģʽ���غ���ʾһ���ֿؼ�
	void EnableControls();

	CImageList m_imglist;
	void InitImageList();
	// ������Ļ�б�
	void ResetListBox();

	pSCREENMAP m_pScrInf; //����������Ļλ����Ϣ������
	int m_iScreens; //����Ļ��
	// �Զ����ø���Ļ����Ϣ
	void AutoScreenInfo();
	// ��ȡĳ����Ļ����Ϣ
	void GetScreenInfo(int iScreen);
	// ����ĳ����Ļ����Ϣ
	void SetScreenInfo(int iScreen);

	int* m_pHorzBlend; //����ˮƽ������ںϴ� �����л����Ķ��ͷβ����Ϊ0���ںϴ����������
	int* m_pVertBlend; //���д�ֱ������ںϴ� �����л����Ķ��ͷβ����Ϊ0���ںϴ����������
	int  m_icurselBlendH;  //��ǰѡ�е�ˮƽ�ںϴ�
	int  m_icurselBlendV;  //��ǰѡ�еĴ�ֱ�ںϴ�
	// ��ȡ�ںϴ���Ϣ
	void GetBlendInfo(int icurselBlendH, int icurselBlendV);
	// �����ںϴ���Ϣ
	void SetBlendInfo(int icurselBlendH, int icurselBlendV);
	// �����ںϴ�ֵ�������Ļ����
	void CalcScreenPosAccordingtoBlendInfo();
	void EnableBlendEdit(int icurselBlendH, int icurselBlendV);

	// �����ʾ��ͼ�ϵİ��´���
	void OnLBtnDownProc(CPoint point); 

	int m_iCurselScreen; //�б��ǰѡ�����
	BOOL m_bDataSaved; //�û��������Ļ������ֵ���Ƿ��Ѿ�Ӧ��

	// ���û������˱༭���ֵ�������û�Ӧ�á�
	void RemindtoApplySetting();

private:
	// �������
	CBitmap m_bmpMonitor;
	BITMAP m_bmpInfo;

	// ʾ��ͼ��һ����Ļ��ռ�Ĵ�С
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

	// ��������
	int m_iHPos;
	int m_iVPos;
	int m_iMaxHScroll;
	int m_iMaxVScroll;
	void OnScrollScreen(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar, UINT nSBType);
	void ResetScrollRange();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
