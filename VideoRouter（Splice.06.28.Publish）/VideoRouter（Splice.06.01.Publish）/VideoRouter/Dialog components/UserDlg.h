#pragma once
#include "MMListCtrl.h"
#include "..\UserManager.h"

// CUserDlg 对话框

class CUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserDlg)

public:
	CUserDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserDlg();

	CMMListCtrl m_UserList;
	CMFCButton m_btnAdd;
	CMFCButton m_btnMod;
	CMFCButton m_btnSav;
	CMFCButton m_btnDel;
	CComboBox m_cboxAuthority;

	CString m_strUserName;
	CString m_strUserPwd;
	CString m_strUserPwdRept;

	CImageList m_imglistUser;

	enum emOperate
	{
		opt_non = 0,
		opt_add = 1,
		opt_mod = 2,
		opt_sav = 3,
		opt_del = 4,
	} m_opt;

	// 编辑使能
	void EnableEditUserInfo(BOOL bEnable);
	// 清空编辑
	void ClearEditUserInfo();
	// 权限描述
	void DescribeAuthority(emUserAuthority eUA);


// 对话框数据
	enum { IDD = IDD_DLG_USER_MANAGE };

private:
	// 初始化用户列表
	void InitUserList();
	// 获取当前选中的列表项
	int GetCursorItem();
	// 获取用户信息到界面
	void GetUserInfo(int iItem);

	// 检查用户信息的合法性
	BOOL CheckUserInf();
	// 密码字符限定
	BOOL PasswordCharLmt(const CString &strPassword);
	// 保存修改后的信息
	void SaveModifiedInfo();
	// 保存新用户信息
	void SaveNewUserInfo();

protected:
	CUserManager m_userManager;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddUser();
	afx_msg void OnBtnModUser();
	afx_msg void OnBtnSavUser();
	afx_msg void OnBtnDelUser();
	afx_msg void OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCboxUserAuthority();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
