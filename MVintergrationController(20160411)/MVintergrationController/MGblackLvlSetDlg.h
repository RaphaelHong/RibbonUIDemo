#pragma once

//****************************************************
//  0:TOPLEFT       5:TOP      1:TOPRIGHT
//  4:LEFT        12:CENTER      6:RIGHT
//  2:BOTTOMLEFT   7:BOTTOM    3:BOTTOMRIGHT
//                9:TOPLEAK
//  8:LEFTLEAK                10:RIGHTLEAK
//              11:BOTTOMLEAK
#define TOPLEFT     0x0001  //�����ں���
#define	TOPRIGHT    0x0002  //�����ں���
#define	BOTTOMLEFT  0x0004  //�����ں���
#define	BOTTOMRIGHT 0x0008  //�����ں���
#define	LEFT        0x0010  //���ں���
#define	TOP         0x0020  //�����ں���
#define	RIGHT       0x0040  //�ҷ��ں���
#define	BOTTOM      0x0080  //�ײ��ں���
#define LEFTLEAK    0x0100  //���©���
#define TOPLEAK     0x0200  //����©���
#define RIGHTLEAK   0x0400  //�Ҳ�©���
#define BOTTOMLEAK  0x0800  //�ײ�©���
#define	CENTER      0x1000  //�в����ں���

#define X_TICK_NUM  10      //��̶���
#define Y_TICK_NUM  9       //�ݿ̶���

// CMGblackLvlSetDlg �Ի���

class CMGblackLvlSetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGblackLvlSetDlg)

public:
	CMGblackLvlSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMGblackLvlSetDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MG_BLACKLVL };

private: //-- �ؼ��йص�
	CXTPButton m_btnBlendL;
	CXTPButton m_btnBlendT;
	CXTPButton m_btnBlendR;
	CXTPButton m_btnBlendB;
	CXTPButton m_btnBlendTL;  //topleft blend zone
	CXTPButton m_btnBlendTR;  //topright blend zone
	CXTPButton m_btnBlendBL;  //bottomleft blend zone
	CXTPButton m_btnBlendBR;  //bottomright blend zone
	CXTPButton m_btnLeakageL;
	CXTPButton m_btnLeakageT;
	CXTPButton m_btnLeakageR;
	CXTPButton m_btnLeakageB;
	CXTPButton m_btnBlendNon;

	CXTPButton m_btnSetLeakWidth;
	CXTPButton m_btnResetBlackLvl;

	CXTPButton m_RadioLeakage[4];

	CXTPEdit m_EditLeakageTL; //top of left
	CXTPEdit m_EditLeakageBL; //bottom of left
	CXTPEdit m_EditLeakageLT; //left of top
	CXTPEdit m_EditLeakageRT; //right of top
	CXTPEdit m_EditLeakageTR; //top of right
	CXTPEdit m_EditLeakageBR; //bottom of right
	CXTPEdit m_EditLeakageLB; //left of bottom
	CXTPEdit m_EditLeakageRB; //right of bottom

	CXTPSpinButtonCtrl m_SpinLeakageTL; //top of left
	CXTPSpinButtonCtrl m_SpinLeakageBL; //bottom of left
	CXTPSpinButtonCtrl m_SpinLeakageLT; //left of top
	CXTPSpinButtonCtrl m_SpinLeakageRT; //right of top
	CXTPSpinButtonCtrl m_SpinLeakageTR; //top of right
	CXTPSpinButtonCtrl m_SpinLeakageBR; //bottom of right
	CXTPSpinButtonCtrl m_SpinLeakageLB; //left of bottom
	CXTPSpinButtonCtrl m_SpinLeakageRB; //right of bottom

	CXTPButton m_CheckEnableTest;
	CXTPEdit m_EditTestPos;
	CXTPSpinButtonCtrl m_SpinTestPos;

	CXTPButton m_CheckEnableBlackLvl;
	CXTPButton m_CheckFixedTestPos;

	void ResizeIt();
	void SetControlsTheme(XTPControlTheme theme);
	void SetBuddys();

	CBrush m_bkBrush;

	int m_nLeakageSel;       //©���ѡ��
	int m_nLeakageWid[4][2]; //©������ [©���][����]
	int m_nBlackLvlTestPos;  //���Ժڵ�ƽλ��
	int m_nEnableTest;       //��������
	int m_nFixedTestPos;     //�̶����Ե�
	int m_nEnableBlackLvl;   //�����ڵ�ƽ

public:
	
	USHORT m_uBlendZone;      //��ͨ�����е��ں�����16λ�����ƣ�ÿһλ����һ���ں���
	void SetTotalBlendZone(); //���ø�ͨ�����е��ں��� �����ںϴ����ֵ�ж�

	void EnableLeakageWidthControls(int nLeakageSel, BOOL bEnable = TRUE);
	void ShowBlendSelBtn();

	void Initialize();

private://--- �ͻ�ͼ�йص�
	CRect m_rcPaint;  //��ͼ��
	CRect m_rcChart;  //�����
	double m_dXscale; //���������
	double m_dYscale; //���������
	void GetPaintRect();
	void CreateCustomFont();

	int m_ntickX[X_TICK_NUM]; //����̶� ���ǿ̶�ֵ ���Ǳ�׼�ڵ�ƽֵ [ͨ����][��̶ȸ���]
	int m_ntickY[Y_TICK_NUM]; //����̶� [�ݿ̶ȸ���]
	int m_nData[13][9];       //�ڵ�ƽƫ��ֵ [ͨ��][�ں���][�ڵ�ƽ��]
	int m_nCurTickX;          //��ǰѡ�еĺ�̶� - �������ĺ�̶�

	CFont m_markFont;  //��ǿ̶ȵ����õĺ�������
	CFont m_titleFont; //��������

	BOOL m_bLBtnDownAtCtrlColomnar; //����ڿ����������ڰ��� ������������Ҽ�
	int m_nCurSection;             //��ǰѡ��ĺڵ�ƽ�Σ��ܹ�7��
	int m_uCurBlendZone;           //��ǰѡ����ں���

	BOOL m_bLBtnDownAtTickArea;
	BOOL m_bTickOrBlackLvlAjust;   //�̶ȵ������Ǻڵ�ƽ����
	BOOL m_bTickPosChanged;

	void DrawAxis(CDC *pValidDC);
	void DrawData(CDC *pValidDC);
	void DrawTitle(CDC *pValidDC);

	void GetTitleString(CString &strtitle);

	inline int GetSectionBeginVal(const int iSec); //��ȡĳ�ε���ʼ����ֹ�ڵ�ƽֵ
	inline int GetSectionEndVal(const int iSec);

	BOOL IsBtnDownAtCtrlColomnar(CPoint point); //����Ƿ����������ڰ���
	BOOL IsBtnDownAtTickArea(CPoint point); //����Ƿ��ں�̶ȱ��ϰ���

	void SetBlackLevel(int nNewLvl/*���޵��ֵ*/);  // ����ĳһ�κڵ�ƽ��ֵ 
	void OffsetBlackLevel(int nOffset);         //�ڵ�ƽƫ�� �����������������
	void SetTickPos(int nPos); //�����̶�λ��

private:
	void WriteBlackLevel();
	void WriteBlackSection();
	void SetLeakageWidth(int nLeakageSel);
	void Reset();
	void ResetTickPos();
	void ResetBlackLevel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBlendzone(UINT nID);
	afx_msg void OnBnRadioLeakageSel(UINT nID);
	afx_msg void OnDeltaposSpinLeakageWid(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSetLeakageWid();
	afx_msg void OnBnCheckEnableBlackLvlTest();
	afx_msg void OnBnClickedCheckTestposFixed();
	afx_msg void OnBnClickedCheckEnableBlacklvl();
	afx_msg void OnDeltaposSpinTestLvlpos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnResetBlacklvl();
};
