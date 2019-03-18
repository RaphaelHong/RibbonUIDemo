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
	eStepTop    = 0x01,  //Zorder值最大 最顶层显示
	eStepBottom = 0x02,  //Zorder值为0 最底层显示
	eStepFront  = 0x03,  //Zorder值加1
	eStepBack   = 0x04,  //Zorder值减1
}; // 调整窗口叠放顺序

typedef struct stWindowdInfo
{
	CRect		rcLog;        //窗口在绘图区的坐标
	CRect		rcLogLast;    //上一次的逻辑坐标
	CRect	    rcPhy;        //窗口实际物理坐标
	CRect	    rcPhyLast;    //上一次的物理坐标
	CRect		rcBtnExit;	  //关闭按钮位置
	CRect		rcBtnRestore; //还原按钮位置
	CRect		rcBtnMax;	  //最大化按钮位置
	CRect		rcBtnResize;  //对齐按钮位置
	CRect		rcTitle;      //标题栏位置
	int			iZorder;      //窗口Z次序
	int			iSource;      //信号源
	char		szTitle[64];  //窗口名称
	BOOL		IsSelected;   //窗口是否被选中
	int			WinId;        //窗口ID	
} WindowInfo,*pWindowInfo;

// 最多开三个窗口
#define MAX_WIN  3

// 功能按钮的数量
#define MAX_BTN  5
#define BTN_WID  80

#define ErrIndex 0xff
#define ErrWinID 0xff

// CMvxSplitDlg 对话框

class CMvxSplitDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMvxSplitDlg)

public:
	CMvxSplitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMvxSplitDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MVX_MAIN };

	//对象计数器
	static int stcMvxObjCount;
	int m_nObjOder;

	//背景画刷
	CBrush m_bkBrush;

	//分辨率名称
	CString *m_pStrTiming;
	//分辨率参数
	TIMING_STRUCT *m_pDetailedTiming;

	//板卡信息
	CBaseBoard m_boardInfo;

	void SetBoardInfo(CBaseBoard boardInfo) {
		m_boardInfo = boardInfo;
	}

	void Initialize(); //初始化
	void InitScenes(); //初始化场景

	void ResetVariables(); //复位控制变量

	BOOL SaveAll();  //保存设置

public:
	//背景水平物理像素
	int m_nPhyHorz;
	//背景垂直物理像素
	int m_nPhyVert;
	//背景水平逻辑像素
	int m_nLogHorz;
	//背景垂直逻辑像素
	int m_nLogVert;

	//背景分辨率的刷新率
	TIMING_STRUCT m_BkgTiming;

	//网格对齐线
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
	void HitTest(CPoint ptCursor, emMouseAct nMouseAct); /*nMouseAct 0:左键按下 1:左键弹起 2: 鼠标移动*/

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
	void OnSameSize(int ntype);  //ntype：0x01 宽度相同，0x10 高度相同 0x11 宽高都相同

	void LoadDevScene(int nSceneID);
	void WriteDevScene(int nSceneID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
