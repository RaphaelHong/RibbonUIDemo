#pragma once

//****************************************************
//  0:TOPLEFT       5:TOP      1:TOPRIGHT
//  4:LEFT        12:CENTER      6:RIGHT
//  2:BOTTOMLEFT   7:BOTTOM    3:BOTTOMRIGHT
//                9:TOPLEAK
//  8:LEFTLEAK                10:RIGHTLEAK
//              11:BOTTOMLEAK
#define TOPLEFT     0x0001  //左上融合区
#define	TOPRIGHT    0x0002  //右上融合区
#define	BOTTOMLEFT  0x0004  //左下融合区
#define	BOTTOMRIGHT 0x0008  //右下融合区
#define	LEFT        0x0010  //左方融合区
#define	TOP         0x0020  //顶部融合区
#define	RIGHT       0x0040  //右方融合区
#define	BOTTOM      0x0080  //底部融合区
#define LEFTLEAK    0x0100  //左侧漏光带
#define TOPLEAK     0x0200  //顶部漏光带
#define RIGHTLEAK   0x0400  //右侧漏光带
#define BOTTOMLEAK  0x0800  //底部漏光带
#define	CENTER      0x1000  //中部非融合区

#define X_TICK_NUM  10      //横刻度数
#define Y_TICK_NUM  9       //纵刻度数

// CMGblackLvlSetDlg 对话框

class CMGblackLvlSetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGblackLvlSetDlg)

public:
	CMGblackLvlSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGblackLvlSetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_BLACKLVL };

private: //-- 控件有关的
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

	int m_nLeakageSel;       //漏光带选择
	int m_nLeakageWid[4][2]; //漏光带宽度 [漏光带][两端]
	int m_nBlackLvlTestPos;  //测试黑电平位置
	int m_nEnableTest;       //开启测试
	int m_nFixedTestPos;     //固定测试点
	int m_nEnableBlackLvl;   //开启黑电平

public:
	
	USHORT m_uBlendZone;      //该通道具有的融合区，16位二进制，每一位代表一个融合区
	void SetTotalBlendZone(); //设置该通道具有的融合区 根据融合带宽度值判断

	void EnableLeakageWidthControls(int nLeakageSel, BOOL bEnable = TRUE);
	void ShowBlendSelBtn();

	void Initialize();

private://--- 和绘图有关的
	CRect m_rcPaint;  //绘图区
	CRect m_rcChart;  //表格区
	double m_dXscale; //横轴比例尺
	double m_dYscale; //纵轴比例尺
	void GetPaintRect();
	void CreateCustomFont();

	int m_ntickX[X_TICK_NUM]; //横轴刻度 既是刻度值 又是标准黑电平值 [通道数][横刻度个数]
	int m_ntickY[Y_TICK_NUM]; //纵轴刻度 [纵刻度个数]
	int m_nData[13][9];       //黑电平偏移值 [通道][融合区][黑电平段]
	int m_nCurTickX;          //当前选中的横刻度 - 待调整的横刻度

	CFont m_markFont;  //标记刻度等所用的合适字体
	CFont m_titleFont; //标题字体

	BOOL m_bLBtnDownAtCtrlColomnar; //鼠标在控制柱形区内按下 不分左键还是右键
	int m_nCurSection;             //当前选择的黑电平段，总共7段
	int m_uCurBlendZone;           //当前选择的融合区

	BOOL m_bLBtnDownAtTickArea;
	BOOL m_bTickOrBlackLvlAjust;   //刻度调整还是黑电平调整
	BOOL m_bTickPosChanged;

	void DrawAxis(CDC *pValidDC);
	void DrawData(CDC *pValidDC);
	void DrawTitle(CDC *pValidDC);

	void GetTitleString(CString &strtitle);

	inline int GetSectionBeginVal(const int iSec); //获取某段的起始和终止黑电平值
	inline int GetSectionEndVal(const int iSec);

	BOOL IsBtnDownAtCtrlColomnar(CPoint point); //鼠标是否在柱形区内按下
	BOOL IsBtnDownAtTickArea(CPoint point); //鼠标是否在横刻度标上按下

	void SetBlackLevel(int nNewLvl/*下限点的值*/);  // 设置某一段黑电平的值 
	void OffsetBlackLevel(int nOffset);         //黑电平偏移 方向键调整或鼠标滚轮
	void SetTickPos(int nPos); //调整刻度位置

private:
	void WriteBlackLevel();
	void WriteBlackSection();
	void SetLeakageWidth(int nLeakageSel);
	void Reset();
	void ResetTickPos();
	void ResetBlackLevel();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
