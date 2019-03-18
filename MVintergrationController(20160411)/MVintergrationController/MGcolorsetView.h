#pragma once
#include "ColorArray.h"
#include "MsgTipDlg.h"

typedef enum emColorAjustType
{
	eRoughType      =  0x00,  //�ֵ�
	ePreciseType    =  0x01,  //΢��
};

typedef enum emColorAjustMethod
{
	eLinearMode  =   0x00,   //����ģʽ
	eRegionMode  =   0x01,   //����ģʽ
};

typedef enum emColor
{
	eRed   = 0,
	eGreen = 1,
	eBlue  = 2,
	eWhite = 3
};

// CcolorsetView ��ͼ

class CcolorsetView : public CView
{
	DECLARE_DYNCREATE(CcolorsetView)

protected:
	CcolorsetView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CcolorsetView();

public:
	void SetAjustMethod(const emColorAjustMethod eMethod) { m_AjustMethod = eMethod; }
	void ExchangeColor(const emColor color) { m_eColor = color; }
	void InitMembers();
	void SetTestType(const int nType) { m_nTestType = nType; }
	int GetTestType() const { return m_nTestType; }

	void SetColor(int *data, int nColorSel);
	void TestColor();
	void CancelTest();
	void WriteColor();
	void ResetColor();

	void OnVKeyUpDown(UINT nVKCode);   //�������¼� 0:Up 1:Down

private:
	CRect m_rcChart;  //����ڽ�����Ƶ�λ��

	CColorArray m_ColorArray[4];  // 0:Red 1:Green 2:Blue 3:Pure White

	emColorAjustType m_AjustType;     // �ֵ�/΢��
	emColorAjustMethod m_AjustMethod; // ����/�ֶ�
	emColor m_eColor;    //��ɫѡ�� 0:Red 1:Green 2:Blue 3:Pure White
	int m_nColorSection; //��ɫ�� 0:0-31 1:32-63 2:64-95 ��
	int m_nCtrlPointIndex; //���Ƶ�λ��

	BOOL m_bLBtnDownAtCtrlPoint; //������������Ƶ�
	int  m_nTestType;  //�������ԡ��ź�ͼ�����

	double m_Xscale;  //��ɫ������������ʵ����ɫ�ı���-���� ��׼ֵ
	double m_Yscale;  //��ɫ��������߶���ʵ����ɫ�ı���-���� У׼ֵ

	void CalcChartPos(CRect parentRc);   //������ʵı��λ��

	void DrawTitle(CDC *pValidDC);        //������
	void DrawChart(CDC *pValidDC);        //����
	void DrawRoughChart(CDC *pValidDC);   //�ֵ����
	void DrawPreciseChart(CDC *pValidDC); //΢�����

	void DrawCurves(CDC *pValidDC);       //��������
	void DrawRoughCurve(CDC *pValidDC);   //�ֵ�������
	void DrawPreciseCurve(CDC *pValidDC); //΢��������

	BOOL IsLBtnDownAtColorCtrlPoint(CPoint pt); //����Ƿ������Ƶ�
	int  GetColorSectionIndex(CPoint point);    //��ȡ��ǰ΢������ɫ��

	void AjustColor(int nColorValue); //��ɫУ׼

private:
	CMsgTipDlg *m_pMsgTip;
	void ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip, BOOL bDockAtCursor=TRUE, CPoint ptDockAt=CPoint(0,0)); //��ʾ��ʾ��
	void GetMsgTipInfo(CMsgTipArray &arMsgTip, int nCtrlPtIndex);          //������ʾ��Ϣ����
	int  IsMouseOverColorCtrlPoint(CPoint point);   //����Ƿ��ڿ��Ƶ�����ͣ

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


