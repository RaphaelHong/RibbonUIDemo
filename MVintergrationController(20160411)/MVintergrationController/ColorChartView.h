#pragma once
#include "MsgTipDlg.h"
#include "ColorArray.h"

#define BIG_CTRL_NODE  9
#define TINY_CTRL_NODE 32
#define MAX_BUTTON_NUM 7

// �ֵ� ΢��
typedef enum emColorAjustType
{
	eRoughType   =   0x00,  //�ֵ�
	ePreciseType =   0x01   //΢��
};


typedef enum emColorAjustMode
{
	eLinearMode  =   0x00,   //����ģʽ
	eRegionMode  =   0x01,   //����ģʽ
};

typedef enum emButtonID
{
	eBtn_Red     =   0x00,   //��ɫ��ť
	eBtn_Green   =   0x01,   //��ɫ��ť
	eBtn_Blue    =   0x02,   //��ɫ��ť
	eBtn_White   =   0x03,   //������ť
	eBtn_Reset   =   0x04,   //��λ��ť
	eBtn_Linear  =   0x05,   //���Է�ʽ��ť
	eBtn_Region  =   0x06,   //����ʽ��ť	
};

struct stMyButton
{
	int      nID;            //��ť������
	CRect    rcBtn;			 //��ť����
	COLORREF crBtnNormal;	 //��ť������ɫ
	COLORREF crBtnMouseOver; //��꾭����ťʱ����ɫ
	COLORREF crBtnClicked;   //��걻����ʱ����ɫ
	BOOL     bMouseOver;     //����Ƿ񾭹���ť
	BOOL     bClicked;       //��ť�Ƿ񱻰���
	BOOL     bRealesed;      //��ť�Ƿ���
	BOOL     bAlwaysHighLight;//һֱ���ڸ���״̬
	CString  strFirstName;    //��ť����
	BOOL     DrawMyButton(CDC* pDC);
	BOOL     IsMouseOverBtn(CPoint ptCursor);
	BOOL     IsButtonClicked(CPoint ptCursor);
};

// CColorChartView ��ͼ

class CColorChartView : public CView
{
	DECLARE_DYNCREATE(CColorChartView)

protected:
	CColorChartView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CColorChartView();

public:
	BOOL m_bTPenable;                 //������ɫ���Ի���
	int  m_nBlendOption;              //ȥ���ںϴ�Ӱ��
	int  m_nChannel;                  //��ǰͶӰ��
	void CmdCancelBlending();         //ȡ���ںϴ�Ӱ��
	void CmdWriteColor();             //ʵʱд��ɫ������
	void CmdTestColor(int nPort);              //������ɫ
	void CmdCancelColorTest();        //ȡ����ɫ����
	void CmdSaveColor();              //�̻�ɫ��У������

	void SetColor(int *data, int nColorSel, int nChannel); //���豸��ȡɫ��

	void OnVKeyUpDown(UINT nVKCode);   //�������¼� 0:Up 1:Down

private:
	
	CColorArray m_ColorArray[7][4];    //7��ͨ����0:Red 1:Green 2:Blue 3:White
	
	int m_nColorSection;    //΢������ɫ��
	int m_nCtrlPointIndex;  //��ǰɫ�ʿ��Ƶ�
 
	stMyButton m_myButton[MAX_BUTTON_NUM];
	CRect m_rcChart;         //���λ��

	emColorAjustType m_emColorAjustType;  //�ֵ�/΢��
	emColorAjustMode m_emColorAjustMode;  //����/����
	//BOOL m_bLinkage;                      //�Ƿ�����
	BOOL m_bLBtnDownInCtrlPoint;          //�������������Ƶ�

	void InitVariables();                 //��ʼ����Ա
	void InitButtons();                   //��ʼ����ť��Ա
	void DrawButtons(CDC *pValidDC);      //�����ư�ť
	void CalcButtonPos(CDC *pValidDC);    //������ʵİ�ťλ��
	void DrawTitles(CDC *pValidDC);       //������
	void CalcChartPos(CRect *pParentRect);//������ʵı��λ��
	void DrawChart(CDC *pValidDC);        //��������
	void DrawRoughChart(CDC *pValidDC);   //�������� - �ֵ�
	void DrawPreciseChart(CDC *pValidDC); //�������� - ΢��
	void DrawCurves(CDC *pValidDC);       //��ɫ������
	void DrawRoughCurve(CDC *pValidDC);   //��ɫ������ - �ֵ�
	void DrawPreciseCurve(CDC *pValidDC); //��ɫ������ - ΢��

	void IsMyButtonClicked(CPoint point); //����������ڰ�ť��
	void OnMyButtonClicked(emButtonID eID); //�Զ��尴ť���������´���

	void ResetColor();                       //��λ��ɫ
	int  GetColorSectionIndex(CPoint point); //��ȡ��ǰ΢������ɫ��

	void AjustColor(int nColorValue); //��ɫУ׼

	BOOL IsLBtnDownAtColorCtrlPoint(CPoint point);  //�������Ƿ���ɫ�ʿ��Ƶ��ϰ���
	int  IsMouseOverColorCtrlPoint(CPoint point);   //����Ƿ��ڿ��Ƶ�����ͣ

	int  GetSectionMinValue(int nSectionIndex);  //��ȡĳ����С��У׼ֵ


	CMsgTipDlg *m_pColorMsgTipDlg;                            //������Ϣ��
	void ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip); //��ʾ��ʾ��
	void GetMsgTipInfo(CMsgTipArray &arMsgTip, CPoint ptCursor, int nCtrlPtIndex);          //������ʾ��Ϣ����

	
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


