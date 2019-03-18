#pragma once

struct HelpMargins
{
	Rect rcCli;           // ����
	Rect rcTextBk;        // �ı���������
	Rect rcTitle;         // ��������
	Rect rcSeparator;     // �ָ�������
	Rect rcPortInfoText;  // ������Ϣ�ı�
	Rect rcPortTypeText;  // �˿����������ı�
	Rect rcPortPosText;   // �˿�ģ��λ���ı�
	Rect rcPortModel;     // ģ��˿�λ��
};
// CPortHelpText �Ի���

class CPortHelpText : public CDialog
{
	DECLARE_DYNAMIC(CPortHelpText)

public:
	CPortHelpText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPortHelpText();

// �Ի�������
	enum { IDD = IDD_DLG_HELPTEXT };

	void SetPortInfo(const int nID, const int nType);
	INT_PAIR GetPortInfo();

private:
	int m_nPortID;  // �˿�ID��
	int m_nType;    // �˿����� 1:����˿� 2: ����˿�

	HelpMargins m_HelpMargin;
	void SetMargins();

	void DrawTitle(Graphics* pGraphics);
	void DrawSeparator(Graphics* pGraphics);
	void DrawPortInfo(Graphics* pGraphics);
	void DrawModel(Graphics* pGraphics);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
