#pragma once
#include "NumberEdit.h"
#include "IconListBox.h"
#include "..\DrawWndPage.h"
// CWndProperty 对话框

class CWndProperty : public CDialog
{
	DECLARE_DYNAMIC(CWndProperty)

public:
	CWndProperty(CWnd* pParent = NULL);   // 标准构造函数
	CWndProperty(CDrawWndPage* pPage, SPLICELAYOUT layout, CWnd* pParent = NULL);
	virtual ~CWndProperty();

// 对话框数据
	enum { IDD = IDD_DLG_WND_PROPERTY };

	CImageList m_imagelist;
	CIconListBox m_ListWnd;

	int m_nPosX;
	int m_nPosY;
	int m_nSizeCX;
	int m_nSizeCY;
	

	CEdit m_EditName;
	CNumberEdit m_EditPosX;
	CNumberEdit m_EditPosY;
	CNumberEdit m_EditSizeCX;
	CNumberEdit m_EditSizeCY;
	

	CSpinButtonCtrl m_SBtnPosX;
	CSpinButtonCtrl m_SBtnPoxY;
	CSpinButtonCtrl m_SBtnSizeCX;
	CSpinButtonCtrl m_SBtnSizeCY;
	

	CDrawWndPage* m_pPage;
	SPLICELAYOUT m_Layout;

protected:
	void EnableEditName(BOOL bEnable=TRUE);
	void EnableEditPos(BOOL bEnable=TRUE);
	void InitListBox();
	void GetWndInfo(int nIndex);
	void SetWndInfo(int nIndex);

	BOOL DataCheck();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBtnApply();
public:
	afx_msg void OnLbnSelchangeListWindows();
	afx_msg void OnBnClickedBtnWndPropertyActive();
	afx_msg void OnBtnLock();
};
