#pragma once
#include "BaseBoard.h"

typedef enum emResizeDir
{
	eNoResize   =  0x0000,
	eResize_L   =  0x0001,
	eResize_TL  =  0x0011,
	eResize_T   =  0x0010,
	eResize_TR  =  0x0110,
	eResize_R   =  0x0100,
	eResize_BR  =  0x1100,
	eResize_B   =  0x1000,
	eResize_BL  =  0x1001,
	eResize_Invalid = 0xffff,
};

typedef enum emMoveDir
{
	eNoMove = 0,
	eMove_L = 1,
	eMove_T = 2,
	eMove_R = 3,
	eMove_B = 4,
};

typedef enum emMouseAct
{
	eLBtnDown  = 0,
	eLBtnUp    = 1,
	eMouseMove = 2,
};

typedef enum emZorderAct
{
	eStepTop    = 0x01,  //Zorderֵ��� �����ʾ
	eStepBottom = 0x02,  //ZorderֵΪ0 ��ײ���ʾ
	eStepFront  = 0x03,  //Zorderֵ��1
	eStepBack   = 0x04,  //Zorderֵ��1
}; // �������ڵ���˳��

typedef struct stWindowdInfo
{
	CRect		rcLog;        //�����ڻ�ͼ��������
	CRect		rcLogLast;    //��һ�ε��߼�����
	CRect	    rcPhy;        //����ʵ����������
	CRect	    rcPhyLast;    //��һ�ε���������
	CRect		rcBtnExit;	  //�رհ�ťλ��
	CRect		rcBtnRestore; //��ԭ��ťλ��
	CRect		rcBtnMax;	  //��󻯰�ťλ��
	CRect		rcBtnResize;  //���밴ťλ��
	CRect		rcTitle;      //������λ��
	int			iZorder;      //����Z����
	int			iSource;      //�ź�Դ
	char		szTitle[64];  //��������
	BOOL		IsSelected;   //�����Ƿ�ѡ��
	int			WinId;        //����ID	
} WindowInfo,*pWindowInfo;

// ��࿪��������
#define MAX_WIN  3

// ���ܰ�ť������
#define MAX_BTN  5
#define BTN_WID  80

#define ErrIndex 0xff
#define ErrWinID 0xff

// CMvxSplitDlg �Ի���

class CMvxSplitDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMvxSplitDlg)

public:
	CMvxSplitDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMvxSplitDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MVX_MAIN };

	//���������
	static int stcMvxObjCount;
	int m_nObjOder;

	//������ˢ
	CBrush m_bkBrush;

	//�ֱ�������
	CString *m_pStrTiming;
	//�ֱ��ʲ���
	TIMING_STRUCT *m_pDetailedTiming;

	//�忨��Ϣ
	CBaseBoard m_boardInfo;

	void SetBoardInfo(CBaseBoard boardInfo) {
		m_boardInfo = boardInfo;
	}

	void Initialize(); //��ʼ��
	void InitScenes(); //��ʼ������

	void ResetVariables(); //��λ���Ʊ���

	BOOL SaveAll();  //��������

public:
	//����ˮƽ��������
	int m_nPhyHorz;
	//������ֱ��������
	int m_nPhyVert;
	//����ˮƽ�߼�����
	int m_nLogHorz;
	//������ֱ�߼�����
	int m_nLogVert;

	//�����ֱ��ʵ�ˢ����
	TIMING_STRUCT m_BkgTiming;

	//���������
	int m_nHorzLines;
	int m_nVertLines;

	CArray <WindowInfo, WindowInfo&> m_ArrWinInfo;
	CArray <CPoint, CPoint&> m_ArrAnchorPt;
	void CalcAnchorPoint();

	int m_nFocusID;

	int PrepareNewWinID() const;
	int GetWndIndex(const int nIDtoFind) const;
	void AddWindow(int nNewID, int nSourceID, CRect rcLog, CRect rcPhy, CString strName);
	void DeleteWindow(const int nWinID);
	void SetWndInfo(int nWinID, int nSourceID, CRect rcLog, CRect rcPhy, CString strName);
	void SetWndZorder(int nWinID, emZorderAct eZorderAct);
	inline void SwapWndZorder(WindowInfo &WinInfoA, WindowInfo &WinInfoB);

private:
	CRect m_rcBtnPic;
	CRect m_rcPaint;
	CRect m_rcChart;
	CustBtn m_xBtn[MAX_BTN];

	void CalcBtnRect();
	void ZoomPaintRect();
	void ZoomWindows();

	void PaintBottomPicture();
	void PaintTopPicture();
	void DrawCustomBtn(CDC *pValidDC);
	void DrawGridLine(CDC *pValidDC);
	void DrawResolution(CDC *pValidDC);
	void DrawWindow(CDC *pValidDC, int nIndex);
	void ReflashWindow(CDC *pDC);

private:
	BOOL m_bLBtnDown;
	CPoint m_ptLBtnDown;
	int IsMouseOverCtrls(CPoint ptCursor);
	void ResetBtnState();

	BOOL m_bLBtnDownInWnd;
	BOOL IsMouseBtnDownInWnd(CPoint ptCursor);

	int  m_nNewWndID;
	BOOL m_bNewCreated;
	void DynamicCreate(CPoint ptCursor);
	void OnMoveNResize(CPoint ptCursor);
	void OnVKmoveWindow(UINT nVKcode);
	void OnVKresizeWindow(UINT nVKcode, BOOL bInflate);
	void OnTabChangedWindow();

	void DefLBtnUpProc(CPoint ptCursor);
	void MyCloseWindow();
	void MyMaximizeWindow();
	void MyAligningWindow();
	void MyRestoreWindow();

	emResizeDir m_eResizeDir;
	emMoveDir   m_eMoveDir;
	void HitTest(CPoint ptCursor, emMouseAct nMouseAct); /*nMouseAct 0:������� 1:������� 2: ����ƶ�*/

protected:
	void OnWndProperty();
	void OnBkgndProperty();
	void OnVedioProperty();
	void OnSceneManage();
	void OnDevUpdate();
	void OnShotCut();
	void OnCustomBtnClicked(UINT nID);
	void OnNewWindow();
	void OnCloseWindow();
	void OnSameSize(int ntype);  //ntype��0x01 �����ͬ��0x10 �߶���ͬ 0x11 ��߶���ͬ

	void LoadDevScene(int nSceneID);
	void WriteDevScene(int nSceneID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSaveSceneFile();
	afx_msg void OnReadSceneFile();
	afx_msg void OnWriteDevScene(UINT nID);
	afx_msg void OnLoadDevScene(UINT nID);
	afx_msg void OnWindowOption(UINT nID);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
