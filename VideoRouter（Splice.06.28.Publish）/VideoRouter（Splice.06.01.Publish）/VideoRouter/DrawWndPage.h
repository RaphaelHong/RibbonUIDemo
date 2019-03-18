#pragma once
#include "DrawWndObj.h"
#include "DrawScreenObj.h"

// class CAnchorPoint ê��

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

	// ���ݶ��������ô��ڴ�С
	long GetLeftAnchor(const long l) const;
	long GetTopAnchor(const long t) const;
	long GetRightAnchor(const long r) const;
	long GetBottomAnchor(const long b) const;

	CRect GetResizeRect(const CRect rcOrg) const;

private:
	CArray <CPoint, CPoint&> m_arAnchor;
	SPLICELAYOUT m_layout;
};

// class CPageMouseManager ��������

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
	//������������
	void LBtnUpReturn();

	//�ź�Դ������˿��ϷŲ���
	void OnDropSource(const int iSrcPort, const CPoint point);
	void OnDropOutput(const int iOutPort, const CPoint point);

	inline CDrawWndObj* GetWndUnderCursor(CPoint point, BOOL bDistinct) const;
	inline CDrawScreenObj* GetScnUnderCursor(CPoint point) const;

private:
	CDrawWndPage* m_pOwnerPage;   //����ҳ
	CDrawWndObj* m_pWndLBtnDown;  //����������ʱ�Ĵ���
	CDrawWndObj* m_pWndLBtnUp;    //����������ʱ�Ĵ���

	CDrawScreenObj* m_pScnLBtnDown; //����������ʱ����Ļ
	CDrawScreenObj* m_pScnLBtnUp;   //����������ʱ����Ļ

	CPoint m_ptLBtnDown;   //���������µ�λ��
	CPoint m_ptLBtnUp;     //����Ҽ����µ�λ��

	DWORD m_dwLBtnDown;    //���������µ�ʱ��

	BOOL m_bDynamicBuilt;  //��̬�������ڱ�־
};



// class CPageKeyboardManager ��������
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


// class CDrawWndPage ����������

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

	static BOOL m_sLockAll; //�Ƿ���������
	static BOOL m_sShowGridLines; //�Ƿ���ʾ������
	static CRect GetBkgSizeAccordingtoScrInf(const pSCREENMAP pscrInf, const int iScrNum, BOOL bInflate); //bInflate=TRUE ����ʱ�Ƚ�ÿ���������½�����+1������-1

//////////////////////////////////////////////////
//�������
public:
	//��������
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

	// �����������ݿ�
	void ParseSceneBlocks(__in UINT* pBlocks, __in int nLen);
	// ���������ݴ��
	void PackSceneBlocks(__out UINT* pBlocks, __out int nLen);
	// ������������ļ�
	void PackFile(__in LPCTSTR lpFile);
	// ���������ļ�
	void ParseFile(__in LPCTSTR lpFile);

//////////////////////////////////////////////////
// ��Ļ����
public:
	//��Ļ����
	int GetScreenCount() const;
	CDrawScreenObj* GetScreenObj(int nIndex) const;
	//�ر���Ļ
	void RemoveScreen(CDrawScreenObj* pObj);
	void RemoveScreenAt(int nIndex);
	//���������Ļ
	void RemoveAllScreen();
	//�����Ļ
	void AddScreen(CDrawScreenObj* pObj);
	void ArrangeAdd(CDrawScreenObj* pObj);
	//��ȡ������ĻID��
	int PrepareScreenID() const;
	//���¼������Ļ������
	void InitScreenPos();
	//���ý�����Ļ
	void FocusScreen(CDrawScreenObj* pObj);
	CDrawScreenObj* GetFocusScreen() const;

	CDrawScreenObj* FindByID(const int nScreenID) const;

	//��ȡ��ĳ����˿��������Ļ
	inline CDrawScreenObj* GetScreenByOutPort(const int iOutPort) const;

	//�����ײ㷵�ص���Ļ����
	void ResetScreens(pSCREENMAP pScrMap, const int iCount, BOOL bReverse=FALSE); /*bReverse��������=FALSE��Ҫ��ײ������Ļλ��*/
	void ResetScreens();
	////////////////////////////////////////////////////////
	// ����
public:

	//���ô��ڻ���
	void DrawContent(CDC *pParentDC) const;
	void DrawGrid(Graphics *pGraphics) const;
	void Repaint(BOOL bDeCreate) const;

	//���ñ���λ��
	void SetPosition(const CRect rcPos);
	CRect GetPosition() const { return m_rcPage; }

	//���ò�����Ϣ
	void SetLayoutInfo(SPLICELAYOUT layout);
	void UpdateAligningLine(const int iHorz, const int iVert);

	//�����ڴ�
	void Clear();

public:
	void OnSourceChanged(const int &iInputCh);
	void OnOutPortChanged(const SCREENMAP &scrmap);

private:
	CArray <CDrawWndObj*, CDrawWndObj*> m_arWndObj; //���ڶ�̬����
	CArray <CDrawScreenObj*, CDrawScreenObj*> m_arScreenObj; //��Ļ��̬����

	SPLICELAYOUT m_Layout; //����������Ϣ
	CRect m_rcPage;  //��ҳ����ͼ����
	CWnd *m_pOwner;  //��ҳ�����ߵĴ���ָ�� 

};
