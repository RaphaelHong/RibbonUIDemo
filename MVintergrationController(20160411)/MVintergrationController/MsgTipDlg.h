#pragma once

typedef struct _MSG_TIP//��Ϣ��ʾ������������Ϣ
{
	CString sMsgItemName;                  //��Ϣ����

	CString sMsgItemInfo;               //��Ϣ����

	COLORREF nInfoShowColor; //��ʾ��ɫ(����ֵĿǰ��ʱû��ʹ�ã�ԭ����ʹÿ����Ŀ��������ɫ�����Զ��壻��Ϊ���ǵ��趨����ɫ���ܺͱ���ɫ��ͻ�����Ŀǰ������ɫʹ�ñ���ɫ�ķ�ɫ)

}MSG_TIP;

typedef CArray<MSG_TIP,MSG_TIP&> CMsgTipArray;//��Ϣ��ʾ����

// CMsgTipDlg �Ի���

class CMsgTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgTipDlg)
public:
	CMsgTipDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgTipDlg();

// �Ի�������
	enum { IDD = IDD_MSGTIPDLG };

public:
	CSize ShowMsgTip(CMsgTipArray &arMsgTip); //���ⲿ����������Ҫ��ʾ�����ݣ�������ߴ�  
	void UpdateTrans();     //�޸�͸���ȵ����ò���      
	void DrawMsgTip();      //������ʾ��Ϣ

protected:  
	CMsgTipArray m_arMsgTip;    //��ʾ��Ϣ����  
	CFont   m_MsgTipFont;       //������Ϣ��ʾ������  
	int     m_nTipNameWidth;    //����������ȣ��������߻���λ��  
	int     m_nRowHeight;       //�иߣ��������߻���λ��  

private:  
	void DrawGrid(CDC *pDC,CRect &rc);                      //��������  
	void DrawMsgTip(CDC *pDC,CPoint topPos,MSG_TIP &mt);    //������ʾ��Ϣ(ĳһ��)  
	void DrawMsg(CDC *pDC);                                 //������Ϣ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
