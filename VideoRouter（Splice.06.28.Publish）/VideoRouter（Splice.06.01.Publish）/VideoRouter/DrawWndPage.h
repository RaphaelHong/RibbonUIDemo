#pragma once
#include "DrawWndObj.h"
#include "DrawScreenObj.h"

// class CAnchorPoint 锚点

class CResizeAnchor
{
	friend class CDrawScreenObj;
	friend class CDrawWndPage;
public:
	CResizeAnchor();
	CResizeAnchor(SPLICELAYOUT &layout);
	~CResizeAnchor();

private:
	void ResetAnchor(SPLICELAYOUT &layout);

	// 根据对齐线设置窗口大小
	long GetLeftAnchor(const long l) const;
	long GetTopAnchor(const long t) const;
	long GetRightAnchor(const long r) const;
	long GetBottomAnchor(const long b) const;

	CRect GetResizeRect(const CRect rcOrg) const;

private:
	CArray <CPoint, CPoint&> m_arAnchor;
	SPLICELAYOUT m_layout;
};

// class CPageMouseManager 鼠标管理器

class CPageMouseManager
{
	friend class CDrawWndObj;
	friend class CDrawScreenObj;
	friend class CDrawWndPage;

public:
	CPageMouseManager(void);
	CPageMouseManager(CDrawWndPage* pOwnerPage);
	~CPageMouseManager(void);

	void SetOwner(CDrawWndPage* pOwner);

public:
	void OnContextMenu(CPoint point);
	void OnLBtnDown(CPoint point);
	void OnLBtnUp(CPoint point);
	void OnLBtnDbClick(CPoint point);
	void OnMouseMove(CPoint point);
	//重置鼠标管理器
	void LBtnUpReturn();

	//信号源和输出端口拖放操作
	void OnDropSource(const int iSrcPort, const CPoint point);
	void OnDropOutput(const int iOutPort, const CPoint point);

	inline CDrawWndObj* GetWndUnderCursor(CPoint point, BOOL bDistinct) const;
	inline CDrawScreenObj* GetScnUnderCursor(CPoint point) const;

private:
	CDrawWndPage* m_pOwnerPage;   //所属页
	CDrawWndObj* m_pWndLBtnDown;  //鼠标左键按下时的窗口
	CDrawWndObj* m_pWndLBtnUp;    //鼠标左键弹起时的窗口

	CDrawScreenObj* m_pScnLBtnDown; //鼠标左键按下时的屏幕
	CDrawScreenObj* m_pScnLBtnUp;   //鼠标左键弹起时的屏幕

	CPoint m_ptLBtnDown;   //鼠标左键按下的位置
	CPoint m_ptLBtnUp;     //鼠标右键按下的位置

	DWORD m_dwLBtnDown;    //鼠标左键按下的时刻

	BOOL m_bDynamicBuilt;  //动态创建窗口标志
};



// class CPageKeyboardManager 鼠标管理器
class CPageKeyboardManager
{
	friend class CDrawWndObj;
	friend class CDrawScreenObj;
	friend class CDrawWndPage;

public:
	CPageKeyboardManager();
	CPageKeyboardManager(CDrawWndPage* pOwner);
	~CPageKeyboardManager();

public:
	void OnArrowKeyUp();
	void OnArrowKeyDown();
	void OnArrowKeyRight();
	void OnArrowKeyLeft();

private:
	CDrawWndPage* m_pOwner;
};


// class CDrawWndPage 背景管理器

class CDrawWndPage : public CCmdTarget
{
	friend class CDrawWndObj;
	friend class CDrawScreenObj;
	friend class CPageMouseManager;
	friend class CPageKeyboardManager;

	//DECLARE_DYNAMIC(CDrawWndPage)

public:
	CDrawWndPage(void);
	~CDrawWndPage(void);

	void SetOwner(CWnd *pOwner);
	inline void GetViewSite(CRect &rc) const;
	BOOL IsObjVisible(const CRect &rcObj) const;

	static BOOL m_sLockAll; //是否锁定所有
	static BOOL m_sShowGridLines; //是否显示网格线
	static CRect GetBkgSizeAccordingtoScrInf(const pSCREENMAP pscrInf, const int iScrNum, BOOL bInflate); //bInflate=TRUE 计算时先将每个屏的右下角坐标+1，否则-1

//////////////////////////////////////////////////
//窗体管理
public:
	//窗口数量
	inline int GetWndCount() const;
	CDrawWndObj* GetWndObj(int nIndex) const;

	void RemoveWndAt(int nIndex);
	void RemoveAllWnd();
	void RemoveWnd(CDrawWndObj* pObj);

	inline COLORREF GetColor(const int iSrcPort) const;
	int PrepareWndID() const;
	void AddWnd(CDrawWndObj* pObj);
	void ArrangeAdd(CDrawWndObj* pObj);
	void NewWnd(CPoint point); 
	CDrawWndObj* DynamicBuilt(const CPoint ptStart, const CPoint ptEnd);
	
	void ActiveWnd(CDrawWndObj* pObj);
	CDrawWndObj* GetActivedWnd() const;

	void MovetoTop(CDrawWndObj* pObj);
	void MovetoBottom(CDrawWndObj* pObj);
	void MoveForward(CDrawWndObj* pObj);
	void MoveBackward(CDrawWndObj* pObj);

	void MaximizeWnd(CDrawWndObj* pObj);
	void AlignWnd(CDrawWndObj* pObj);
	void RestoreWnd(CDrawWndObj* pObj);
	BOOL IsMaximized(CDrawWndObj* pObj);
	BOOL IsAligned(CDrawWndObj* pObj);

	void FillWndtoAnchoredScreen(CDrawWndObj* pObj);
	void FillWndtoAcrossedScreen(CDrawWndObj* pObj);

	void UpdateZorder();

	void LockAll();

	// 解析场景数据块
	void ParseSceneBlocks(__in UINT* pBlocks, __in int nLen);
	// 将场景数据打包
	void PackSceneBlocks(__out UINT* pBlocks, __out int nLen);
	// 将场景打包到文件
	void PackFile(__in LPCTSTR lpFile);
	// 解析场景文件
	void ParseFile(__in LPCTSTR lpFile);

//////////////////////////////////////////////////
// 屏幕管理
public:
	//屏幕数量
	int GetScreenCount() const;
	CDrawScreenObj* GetScreenObj(int nIndex) const;
	//关闭屏幕
	void RemoveScreen(CDrawScreenObj* pObj);
	void RemoveScreenAt(int nIndex);
	//清空所有屏幕
	void RemoveAllScreen();
	//添加屏幕
	void AddScreen(CDrawScreenObj* pObj);
	void ArrangeAdd(CDrawScreenObj* pObj);
	//获取备用屏幕ID号
	int PrepareScreenID() const;
	//重新计算各屏幕的坐标
	void InitScreenPos();
	//设置焦点屏幕
	void FocusScreen(CDrawScreenObj* pObj);
	CDrawScreenObj* GetFocusScreen() const;

	CDrawScreenObj* FindByID(const int nScreenID) const;

	//获取由某输出端口输出的屏幕
	inline CDrawScreenObj* GetScreenByOutPort(const int iOutPort) const;

	//解析底层返回的屏幕数据
	void ResetScreens(pSCREENMAP pScrMap, const int iCount, BOOL bReverse=FALSE); /*bReverse逆向设置=FALSE需要向底层更新屏幕位置*/
	void ResetScreens();
	////////////////////////////////////////////////////////
	// 绘制
public:

	//调用窗口绘制
	void DrawContent(CDC *pParentDC) const;
	void DrawGrid(Graphics *pGraphics) const;
	void Repaint(BOOL bDeCreate) const;

	//设置本体位置
	void SetPosition(const CRect rcPos);
	CRect GetPosition() const { return m_rcPage; }

	//设置布置信息
	void SetLayoutInfo(SPLICELAYOUT layout);
	void UpdateAligningLine(const int iHorz, const int iVert);

	//清理内存
	void Clear();

public:
	void OnSourceChanged(const int &iInputCh);
	void OnOutPortChanged(const SCREENMAP &scrmap);

private:
	CArray <CDrawWndObj*, CDrawWndObj*> m_arWndObj; //窗口动态数组
	CArray <CDrawScreenObj*, CDrawScreenObj*> m_arScreenObj; //屏幕动态数组

	SPLICELAYOUT m_Layout; //背景布置信息
	CRect m_rcPage;  //此页的视图坐标
	CWnd *m_pOwner;  //此页所有者的窗口指针 

};
