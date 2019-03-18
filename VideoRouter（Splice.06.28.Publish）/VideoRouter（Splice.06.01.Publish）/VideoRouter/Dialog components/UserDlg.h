#pragma once
#include "MMListCtrl.h"
#include "..\UserManager.h"

// CUserDlg �Ի���

class CUserDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserDlg)

public:
	CUserDlg(CWnd* pParent = NULL);   // ��׼���캯��
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

	// �༭ʹ��
	void EnableEditUserInfo(BOOL bEnable);
	// ��ձ༭
	void ClearEditUserInfo();
	// Ȩ������
	void DescribeAuthority(emUserAuthority eUA);


// �Ի�������
	enum { IDD = IDD_DLG_USER_MANAGE };

private:
	// ��ʼ���û��б�
	void InitUserList();
	// ��ȡ��ǰѡ�е��б���
	int GetCursorItem();
	// ��ȡ�û���Ϣ������
	void GetUserInfo(int iItem);

	// ����û���Ϣ�ĺϷ���
	BOOL CheckUserInf();
	// �����ַ��޶�
	BOOL PasswordCharLmt(const CString &strPassword);
	// �����޸ĺ����Ϣ
	void SaveModifiedInfo();
	// �������û���Ϣ
	void SaveNewUserInfo();

protected:
	CUserManager m_userManager;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
