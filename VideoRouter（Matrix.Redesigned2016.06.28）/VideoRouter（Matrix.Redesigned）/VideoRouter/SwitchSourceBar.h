#pragma once
#include "BnItem.h"
#include "Dialog components/InplaceEdit.h"

//////////////////////////////////////////////////////////////////////////
// CDrawSourceItem
// �ź�Դ��

class CDrawSourceItem : public CCmdTarget
{
	friend class CSwitchSourceBar;

	DECLARE_DYNAMIC(CDrawSourceItem)

public:
	CDrawSourceItem();
	CDrawSourceItem(CWnd* pOwner);
	~CDrawSourceItem();

public:
	void SetItemIndex(const int iIndex);
	void SetItemRect(const Rect rect);
	void Active(BOOL bActive);

	Rect GetItemRect() const;
	CRect GetItemRect();

	BOOL IsActived() const { return m_bActive; }

	void Draw(Graphics* pGraphics);

private:
	int m_nSourceIndex; //���Ӧ���ź�Դ��� ��ǰ�ɼ���һ��������+��ʼ�����
	Rect m_rcItem;      //������
	BOOL m_bActive;     //�����Ƿ��Ǽ���״̬

	CWnd* m_pOwner;

protected:
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
// CSwitchSourceBar
// �����л��������ź�Դ������

class CSwitchSourceBar : public CWnd
{
	DECLARE_DYNAMIC(CSwitchSourceBar)

public:
	CSwitchSourceBar();
	virtual ~CSwitchSourceBar();

	// ��һ���ɼ�����ţ���Ҫ���ڹ������ƺ����Ӧ���ź�Դ��Ż�ȡ
	static int m_sFirstVisiableItem;

	void SetBarRect(const CRect rc);
	CRect GetBarRect() const { return m_rcBar; }

	void RecalcLayout();

	BOOL IsEditing() const;

private:
	CRect m_rcBar;  // �˴����ڸ������е�λ��

	CBnItem m_bnFresh;
	void SetBnItem();
	inline void ShowFreshBtn(BOOL bShow);
	inline void RepositionFreshBtn(CDrawSourceItem* pActiveItem);

	int m_FocusItem;
	void OnHitTest(CPoint point);
	
	void RedrawItem(int iItemIndex);

public:
	CArray <CDrawSourceItem*, CDrawSourceItem*> m_arrItem;
	void RemoveAll();
	void RemoveAt(int iItemIndex);
	void AddItem(CDrawSourceItem* pItem);
	void ActiveItem(int iItemIndex);
	CDrawSourceItem* GetSourceItem(int iItemIndex);
	CDrawSourceItem* GetActiveItem();

	void CreateSourceItem();
	void ResetSourceItem();
	void ScrollItem();

	void SetFocuseItem(int nItem);

private:
	CScrollBar m_Vertbar;
	void SetVScrollBar();

	CToolTipCtrl m_tips;
	void SetTooltips();

	int m_nItemOnEdit; //��ǰ���ڱ༭״̬����
	CInplaceEdit m_Edit;
	void BeginEdit();
	void EndEdit();
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL BarCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext  = NULL );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnEndEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCancelEdit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshThis(WPARAM wParam, LPARAM lParam);
};