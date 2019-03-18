#pragma once
#include "NumberEdit.h"

typedef enum __emAjustStyle
{
	eKeystone = 0,
	eCurve    = 1,
	eDensity  = 2
} emAjustStyle;

// CMGeometricAjustDlg �Ի���

class CMGeometricAjustDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGeometricAjustDlg)

public:
	CMGeometricAjustDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMGeometricAjustDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MG_GEOMETRIC };

private:
	// �ؼ�
	CBrush m_bkBrush; //������ˢ

	CXTPButton m_btnKeystone; //�ĵ�У����ť
	CXTPButton m_btnCurve;    //����У����ť
	CXTPButton m_btnDensity;  //����У����ť
	CXTPButton m_btnReset;    //��λ��ť

	// ���¿ؼ��±�0����X����Ŀؼ� �±�1����Y����Ŀؼ�
	CNumberEdit m_editTL[2]; //Topleft      
	CNumberEdit m_editTC[2]; //Topcenter
	CNumberEdit m_editTR[2]; //Topright
	CNumberEdit m_editLC[2]; //Leftcenter
	CNumberEdit m_editRC[2]; //Rightcenter
	CNumberEdit m_editBL[2]; //Bottomleft
	CNumberEdit m_editBC[2]; //Bottomcenter
	CNumberEdit m_editBR[2]; //Bottomright
	CNumberEdit m_editMC[2]; //Middlecenter
	CNumberEdit m_editExX;   //Extra X
	CNumberEdit m_editExY[2];//Extra Y

	CXTPSpinButtonCtrl m_spinTL[2];
	CXTPSpinButtonCtrl m_spinTC[2];
	CXTPSpinButtonCtrl m_spinTR[2];
	CXTPSpinButtonCtrl m_spinLC[2];
	CXTPSpinButtonCtrl m_spinRC[2];
	CXTPSpinButtonCtrl m_spinBL[2];
	CXTPSpinButtonCtrl m_spinBC[2];
	CXTPSpinButtonCtrl m_spinBR[2];
	CXTPSpinButtonCtrl m_spinMC[2];
	CXTPSpinButtonCtrl m_spinExX;
	CXTPSpinButtonCtrl m_spinExY[2];

	void ResizeIt(); // ���µ����ؼ���С��λ��
	
	void SetControlsTheme(XTPControlTheme theme); // ���ÿؼ����

	void SetToolTips();

	void SetBuddys(); //���༭����΢����ť����
	
	void ShowKeystoneCtrls(); // ��ʾ�ĵ�У���Ŀؼ�
	void ShowCurveCtrls();    // ��ʾ����У���Ŀؼ�
	void ShowDensityCtrls();  // ��ʾ����У���Ŀؼ�

private:
	// ��ͼ
	CRect m_rcPaint;     //��ͼ����С
	CRect m_rcChart;     //��������С
	int m_nDeflate;      //�����������ߴ磬ʹ�������ܱ��п�������
	void GetPaintRect(); //��ȡ��ͼ������λ��

	double m_xScale;     //ʵ�����������ڻ�ͼ���ϱ��ֵĴ�С ����
	double m_yScale;     //ʵ�����������ڻ�ͼ���ϱ��ֵĴ�С ����

	//����У����ʽ 0������ 1������ 2������
	emAjustStyle m_eAjustStyle;  

	void DrawBkgGrid(CDC *pValidDC);     //������
	void DrawKeystoneData(CDC *pValidDC);//������У��
	void DrawCurveData(CDC *pValidDC);   //������У��
	void DrawDensityData(CDC *pValidDC); //������У��
	void DrawResolution(CDC *pValidDC);  //��ʾ��ǰ�ֱ���

	//�ĵ�У�����Ʊ���
	//{
		BOOL m_bLBtnDownAt4Point;             //�������ڿ��Ƶ��ϰ���-�ĵ�У��
		BOOL IsLBtnDownAt4Point(CPoint point);//�ж��������Ƿ��ڿ��Ƶ��ϰ���
		CPoint m_ptCursor4point;              //��¼�ĵ�У����ǰ���Ƶ������λ��(x,y) x�кţ�y�к�
		CDoublePoint m_dpaCursorPos4point;    //��¼�ĵ�У����ǰ���Ƶ����������λ��(posX, posY)
		void On4pointAjust(CPoint point, BOOL bMouseActive=TRUE); //�����ĵ�У��, bMouseActive=TRUE��ʾ����괥����У��
		void Reset4pointArray();              //�ĵ�У����λ
		void Show4pointCoords();              //ˢ���ĵ�����༭���п��Ƶ��ֵ
	//}

	//����У�����Ʊ���
	//{
		BOOL m_bLBtnDownAtCurve;              //�������ڿ��Ƶ㰴��-����У��
		BOOL IsLBtnDownAtCurve(CPoint point); //�ж��������Ƿ��ڿ��Ƶ��ϰ���
		CPoint m_ptCursorCurve;               //��¼����У����ǰ���Ƶ������λ��(x,y) x�кţ�y�к�
		CDoublePoint m_dpaCursorPosCurve;     //��¼����У����ǰ���Ƶ����������λ��(posX, posY)
		void OnCurveAjust(CPoint point, BOOL bMouseActive=TRUE);  //��������У��, bMouseActive=TRUE��ʾ����괥����У��
		void ResetCurveArray();               //����У����λ
		void ShowCurveCoords();               //ˢ������У���༭���п��Ƶ��ֵ
	//}

	//����У�����Ʊ���
	//{
		BOOL m_bLBtnDownAtDensity;             //�������ڿ��Ƶ㰴��-����У��
		BOOL IsLBtnDownAtDensity(CPoint point);//�ж��������Ƿ��ڿ��Ƶ��ϰ���
		CPoint m_ptCursorDensity;              //��¼����У����ǰ���Ƶ������λ��(x,y) x�кţ�y�к�
		CDoublePoint m_dpaCursorPosDensity;    //��¼����У����ǰ���Ƶ����������λ��(posX, posY)
		void OnDensityAjust(CPoint point, BOOL bMouseActive=TRUE); //��������У��, bMouseActive=TRUE��ʾ����괥����У��
		void ResetDensityArray();              //����У����λ
		void ShowDensityCoords();              //ˢ������У���༭���п��Ƶ��ֵ
	//}

public:
	// У�����ݺͲ���
	CGridSetting m_gridsetting;                //��¼��ǰ�忨��У���йص�һЩ������Ϣ
	CDoublePointArray *m_pDpaOrigpoints;       //ԭʼ����������
	CDoublePointArray *m_pDpa4points;          //�ĵ�У���ĵ���������
	CDoublePointArray *m_pDpaCurvepoints;      //����У���ĵ���������
	CDoublePointArray *m_pDpaDensitypoints;    //����У���ĵ���������

	void InitDpaPoints();  //�����ڴ棬��ʼ��
	void DeleteDpaData();  //�����ڴ�

	void SetGraphicSize(); //���û�ͼ�ߴ��С������ʼ��GridSetting

	CDoublePoint OrignalPos(int x, int y);        //����ĳ�����ԭʼλ��

	void Warp_Auto_Kestone();                     //�ĵ�У�� - �����Ե
	void Warp_Auto_RowX(int row_y_index);         //�ĵ�У�� - �о���
	void Warp_Auto_RowY(int column_x_index);      //�ĵ�У�� - �о���

	void Fitting();                               //����У�� - ��������㷨
	void Warp_Auto_XY();                          //����У�� - ������Ϻ�������е�

	void Warp_Auto_Ranks_Translation();           //����У�� - ���ܶȵ���

public:
	inline int GetBoardPort();    //��ȡ�����忨��ͨ�Ŷ˿ں�
	inline void Packaging(TaskPackage &pack, int nMode); //�������װ

	void Initialize();   //�Ӱ忨��ȡ���ݲ���ʼ��
	BOOL SaveGeoDatas(); //���漸��У������
	void Init4PointPosition(UINT *p4PointPos);   //��ʼ���ĵ�У���ĸ����λ��
	void InitCurvePosition(UINT *pCurvePos);     //��ʼ������У���Ÿ����λ��
	void InitDensityPosition(UINT *pDensityPos); //��ʼ������У���Ÿ����λ��

private:
	BOOL VKEnterInEditCtrls(HWND hWnd, double dOffset, BOOL bEnterActive=TRUE); //ENTER�������΢����ť�������꣬ bEnterAcitve=TRUE��ʾENTERֱ������
	void OnVKArrowAjust(UINT nVKcode); //������ƶ����Ƶ�

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedAjustStyle(UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedReset();
	afx_msg void OnDeltaposSpinPointPos(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
};
