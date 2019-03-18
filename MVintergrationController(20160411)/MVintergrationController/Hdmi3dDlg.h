#pragma once
#include "BaseBoard.h"
#include "HdmiTimingSetDlg.h"
#include "Hdmi3dOptionDlg.h"

// CHdmi3dDlg �Ի���

class CHdmi3dDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CHdmi3dDlg)

public:
	CHdmi3dDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHdmi3dDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HDMI3D };

public:
	CBaseBoard m_boardinfo;
	void SetBoardInfo(const CBaseBoard &boardinfo) {
		m_boardinfo = boardinfo;
	}

	void CreateChildWindows();
	void Initialize();
	BOOL SaveAll();

public:
	CHdmiTimingSetDlg m_dlgTiming;
	CHdmi3dOptionDlg m_dlgOption;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CXTPTabControl m_wndTab;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectChanged(NMHDR* pNMHDR, LRESULT* pResult);
};
