
#pragma once
//#include "DrawObj.h"

// CDrawWndObj 命令目标

class CDrawWndObj : public CCmdTarget
{
	friend class CPageMouseManager;
	friend class CDrawWndPage;

	DECLARE_DYNAMIC(CDrawWndObj)

public:
	CDrawWndObj(CDrawWndPage* pOwner);
	virtual ~CDrawWndObj();

	CDrawWndPage *m_pOwner;
	void SetParentPage(CDrawWndPage* pOwner);

	static int m_sxPhyScalar; //物理标量
	static int m_syPhyScalar; 
	static int m_sxLogScalar; //逻辑标量
	static int m_syLogScalar;
	//设置标量
	static void SetScalar(const int ixPhy, const int iyPhy, const int ixLog, const int iyLog);

	int Write(__in char* pBlock, __in const long lsize, __out long &lPos) const; //For Write File
	int Write(__in UINT* pBlock, __in const long lsize, __out long &lPos) const; //For Write Device
	int Read(__in char* pBlock, __in const long lsize, __out long &lPos); //For Read File
	int Read(__in UINT* pBlock, __in const long lsize, __out long &lPos); //For Read Device
	static int GetObjectSize();

	//成员
private:
	//{
	int m_iWndID;         //窗口ID
	int m_iSrcPort;       //信号源
	int m_iZorder;        //窗口叠放次序
	int m_iAlpha;         //窗口透明度
	char m_szName[_MAX_WND_NAME];    //窗口标题

	CRect m_rcPhy;        //窗口在背景屏幕上的位置  
	CRect m_rcLog;        //窗口在视图文档上的位置
	CRect m_rcLastPhy;    //窗口上一次的屏幕位置
	CRect m_rcLastLog;    //窗口上一次的视图位置
	CRect m_rcBtnExit;    //关闭按钮的位置
	CRect m_rcBtnRestore; //还原按钮的位置
	CRect m_rcBtnMaximize;//最大化按钮的位置
	CRect m_rcBtnAlign;   //对齐按钮的位置
	CRect m_rcTitle;      //标题位置

	COLORREF m_clrBk;     //窗体背景颜色
	//}

	BOOL m_bLocked;       //窗口是否被锁定
	BOOL m_bActived;      //窗口是否处于激活状态

	//方法
public:
	//设置ID
	void SetWndID( const int iID);
	//获取ID
	int GetWndID() const { return m_iWndID; };

	//设置信号源
	void SetSourcePort(const int iSrcPort);
	//获取信号源
	int  GetSourcePort() const { return m_iSrcPort; };

	//设置叠放顺序
	void SetZorder(const int iZorder);
	//获取叠放顺序
	int  GetZorder() const { return m_iZorder; };

	//透明系数
	void SetAlpha(const int iAlpha);
	int  GetAlpha() const { return m_iAlpha; }

	//设置名称
	void SetName(const char* szName);
	//获取名称
	char* GetName() const { return (char*)m_szName; };

	//锁定
	void SetLock(BOOL bLock);
	//是否锁定
	BOOL IsLocked() const { return m_bLocked; };

	//激活
	void Active(BOOL bActive);
	//是否激活
	BOOL IsActived() const { return m_bActived;};

	//设置窗口的屏幕坐标
	void SetPhyPos(const CRect rcPhy);
	void SetPhyPos(const Rect rcPhy);
	//获取窗口的屏幕坐标
	CRect GetPhyPos() const { return m_rcPhy; };

	//设置窗口的视图坐标
	void SetLogPos(const CRect rcLog);
	//获取窗口的视图坐标
	CRect GetLogPos() const { return m_rcLog; };
	//重新计算窗口布置
	void RecalcLayOut();

	void SetPosMark();

	//窗口缩放
	void Zoom();

	//通知所有者重画此窗口
	void OnGraphChanged();

	// 调用命令
	int Open();
	int Size();
	int Move();
	int MoveZ();
	int MoveS();
	static int Shut(const int nID);

private:
	//设置窗体颜色
	void SetBkColor(COLORREF clrBk);
	//窗口绘制
	void Draw(CDC *pDC);
	void DrawMethod2(Graphics *pGraphics);

	//右键菜单
	void OnPopupMenu(CPoint point);

	//鼠标左键按下处理
	void OnLBtnDown(CPoint point);

	//鼠标左键弹起处理
	void OnLBtnUp(CPoint point);

	//鼠标左键双击处理
	void OnLBtnDblClick(CPoint point);

	//鼠标移动处理
	void OnMouseMove(CPoint point);

	//鼠标热追踪,bTest=TRUE设置HITTEST，=FALSE只设置鼠标形态
	int OnHitTest(CPoint point, BOOL bTest);
	void LBtnUpReturn();

	// 关闭
	void OnClose();
	// 最大化
	void OnMaximize();
	// 对齐
	void OnAlignTo();
	// 还原
	void OnRestore();

	CPoint m_ptLBtnDown;
	int m_HitTest;        //鼠标追踪

protected:
	DECLARE_MESSAGE_MAP()
};


