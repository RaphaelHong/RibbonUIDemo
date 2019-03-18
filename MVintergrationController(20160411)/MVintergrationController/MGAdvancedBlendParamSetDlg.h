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

// CAdvancedBlendParamView ��ͼ

class CAdvancedBlendParamView : public CView
{
	DECLARE_DYNCREATE(CAdvancedBlendParamView)

protected:
	CAdvancedBlendParamView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAdvancedBlendParamView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	stSuppressibleButton m_ZonSelBtn[4];  //�ĸ��Զ����ذ�ť
	CRect m_rcChart;      //����λ��
	void InitMemberVariables();
	void InitButtons();
	void DrawBlendZoneSelectionButton(CDC *pValidDC, BOOL bShow, int ZoneSel/*�ںϴ�ѡ��*/);   //�����������ĸ��ںϴ�ѡ��ť
	void DrawTriangle(CDC *pDC, const LPPOINT points, HBRUSH hbr);     //��������
	void DrawAxis(CDC *pDC);  //��������
	void DrawDataLine(CDC *pDC); //������
	void DrawRangeNonius(CDC *pDC); //����Χ�α���Ʒ�(�������������α�)
	void DrawIndivNonius(CDC *pDC); //�������α���Ʒ�
	void ShowBlendZoneBtn(CPoint point); //��ʾ�������ںϴ�ѡ��ť
	void NoniusMoveProc(CPoint point);   //�������α껬��
	BOOL IsMouseOverNonius(CPoint point, emNoniusType &eNoniustype); //�������Ƿ����α��ϰ���
	void ResetCtrlVariables();           //���ÿ��Ʊ���
	void AverageBlendData();     //�������������α�֮����ں�ֵ
	void WriteBlendParaData(int nBegin, int nEnd);   //д���ں�����
	

	BOOL m_bLBtnDownInNonius; //���������α��ϰ���
	emNoniusType m_eNoniusType; //�α�����
	emNoniusType m_eCurNonius; //��ǰ�ܿ����α�
	
public:
	UINT m_BlendParamData[1024]; //�ںϲ���ֵ
	int m_BlendWidth;              //��ǰ�ںϴ����
	int m_nChannel;                //��ǰͨ��
	int m_nPureWhite;
	double alpha;
	double p;
	double gamma;
	int m_nSide;

	int  m_nRightNonius;    //���α��Ӧ���ںϴ�����λ��
	int  m_nLeftNonius;     //���α��Ӧ���ںϴ�����λ��
	int  m_nIndivNonius;    //�����α��Ӧ���ںϴ�����λ��

	bool SetBlenddParaData(); //��ȡ�ںϲ���
	void SetNoniusState(int nNoniusType, int nNoniusVal);
	void OnVKdown(UINT vkType); //���̷������Ӧ����
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

// CAdvancedBlendParamSetDlg �Ի���

class CAdvancedBlendParamSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedBlendParamSetDlg)

public:
	CAdvancedBlendParamSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvancedBlendParamSetDlg();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnDeltaposSpinNonius(NMHDR *pNMHDR, LRESULT *pResult);
};
