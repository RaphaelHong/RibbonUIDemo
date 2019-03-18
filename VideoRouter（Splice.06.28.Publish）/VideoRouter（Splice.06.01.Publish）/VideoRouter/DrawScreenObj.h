
#pragma once

// CDrawScreenObj 

class CDrawScreenObj : public CCmdTarget
{
	friend class CPageMouseManager;
	friend class CDrawWndPage;
	DECLARE_DYNAMIC(CDrawScreenObj)

public:
	CDrawScreenObj();
	CDrawScreenObj(CDrawWndPage* pOwner);
	virtual ~CDrawScreenObj();

	CDrawWndPage* m_pOwner;
	void SetOwner(CDrawWndPage* pOwner);

	//成员变量控制
public:	
	//设置此屏幕的ID
	void SetID(const int nID);
	int GetID() const { return m_iScreenID; }

	//设置此屏幕的物理输出口
	void SetOutPort(const int iPort);
	int GetOutPort() const { return m_iOutPort; }

	//设置此屏幕的选中状态
	void SetFocus(BOOL bFocus);
	BOOL IsFocused() const { return m_bFocused; }

	//关闭和打开此屏幕
	void SetShutDown(BOOL bShutDown);
	BOOL IsShutDown() const { return m_bShutDown; }

	//设置屏幕名称
	void SetName(char *szName);
	char* GetName() const { return (char*)m_szName; }

	//设置此屏幕的绘图坐标
	void SetLogCoords(const RectF rcScreen);
	RectF GetLogCoords() const { return m_rcLog; }

	//设置屏幕的物理坐标
	void SetPhyCoords(const Rect rc);
	Rect GetPhyCoords() const { return m_rcPhy; };

	// 锁定屏幕位置
	void Lock(BOOL bLock = TRUE);
	BOOL IsLocked() const { return m_bLock; }

	// 缩放
	void Zoom();

	void OnOutputChange();

private:
	//绘制此屏幕
	virtual void Draw(CDC *pPrarentDC);

	void DrawScreenBk(Graphics* pGraphics);
	void DrawGridLine(Graphics* pGraphics);

	//屏幕上的右键菜单
	virtual void OnPopupMenu(CPoint point);

	//屏幕上的鼠标左键按下处理
	virtual void OnLBtnDown(CPoint point);

	//屏幕上的鼠标左键弹起处理
	virtual void OnLBtnUp(CPoint point);

	//屏幕上的鼠标左键双击处理
	virtual void OnLBtnDblClick(CPoint point);

	//屏幕上的鼠标移动处理
	virtual void OnMouseMove(CPoint point);

	//成员变量
private:	

	static int m_iHorz;  //屏幕的通用属性 逻辑屏幕数-水平
	static int m_iVert;  //屏幕的通用属性 逻辑屏幕数-垂直
	static BOOL m_bLockWall; //全局的屏幕墙锁定标记
	int  m_iScreenID; //此屏幕的ID
	int  m_iOutPort;  //此屏幕对应的输出端口号
	BOOL m_bFocused;  //此屏幕是否被选中
	BOOL m_bShutDown; //此屏幕是否已关闭
	BOOL m_bLock;     //锁定此屏幕位置
	char m_szName[_MAX_WND_NAME];    //此屏幕名称，若不设置，默认为屏幕号
	RectF m_rcLog;    //此屏幕在视图中的坐标
	Rect  m_rcPhy;    //此屏幕在背景中的物理坐标

protected:
	DECLARE_MESSAGE_MAP()
};


