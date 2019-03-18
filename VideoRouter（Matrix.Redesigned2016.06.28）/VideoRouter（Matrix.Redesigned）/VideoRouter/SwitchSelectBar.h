#pragma once

typedef struct tagBoxItem
{
	tagBoxItem() {}
	tagBoxItem(const tagBoxItem &DesBox)
	{
		iColumn = DesBox.iColumn;
		iRow = DesBox.iRow;
	}

	tagBoxItem(int nCol, int nRow)
	{
		iColumn = nCol;
		iRow = nRow;
	}

	BOOL operator==(const tagBoxItem &DesBox)
	{
		return (iColumn == DesBox.iColumn && iRow == DesBox.iRow);
	}

	BOOL operator!=(const tagBoxItem &DesBox)
	{
		return (iColumn != DesBox.iColumn || iRow != DesBox.iRow);
	}

	int iColumn;
	int iRow;

} BOXITEM,* pBOXITEM;

//////////////////////////////////////////////////////////////////////////
// CSwitchSelectBar ÃüÁîÄ¿±ê
// ¾ØÕóÇÐ»»Æ÷µÄÇÐ»»Ñ¡Ôñ°´Å¥À¸

class CSwitchSelectBar : public CWnd
{
	friend class CSwitchSourceBar;
	friend class CSwitchHeadBar;

	DECLARE_DYNAMIC(CSwitchSelectBar)

public:
	CSwitchSelectBar();
	virtual ~CSwitchSelectBar();

	void SetBarRect(const CRect rc);
	CRect GetBarRect() const { return m_rcBar; }

	void SetFocusBox(const int iCol, const int iRow);
	void SetFocusCol(const int iColumn);
	void SetFocusRow(const int iRow);

private:
	CRect m_rcBar;
	BOXITEM m_FocusBox;

	void OnHitTest(CPoint point);

	BOXITEM m_ptLBClkBox;
	BOXITEM GetFocusBox(CPoint point);

protected:
	void DrawBoxMap(CDC* pValidDC);
	void DrawBoxMap2(CDC* pValidDC);
	void DrawBoxMap3(CDC* pValidDC);

protected:
	DECLARE_MESSAGE_MAP();

public:
	BOOL BarCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext  = NULL );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


