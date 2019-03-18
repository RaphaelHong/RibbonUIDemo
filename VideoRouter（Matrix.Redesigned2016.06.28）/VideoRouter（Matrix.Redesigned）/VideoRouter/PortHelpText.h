#pragma once

struct HelpMargins
{
	Rect rcCli;           // 窗口
	Rect rcTextBk;        // 文本背景区域
	Rect rcTitle;         // 标题区域
	Rect rcSeparator;     // 分隔栏区域
	Rect rcPortInfoText;  // 输入信息文本
	Rect rcPortTypeText;  // 端口物理类型文本
	Rect rcPortPosText;   // 端口模拟位置文本
	Rect rcPortModel;     // 模拟端口位置
};
// CPortHelpText 对话框

class CPortHelpText : public CDialog
{
	DECLARE_DYNAMIC(CPortHelpText)

public:
	CPortHelpText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPortHelpText();

// 对话框数据
	enum { IDD = IDD_DLG_HELPTEXT };

	void SetPortInfo(const int nID, const int nType);
	INT_PAIR GetPortInfo();

private:
	int m_nPortID;  // 端口ID号
	int m_nType;    // 端口类型 1:输入端口 2: 输出端口

	HelpMargins m_HelpMargin;
	void SetMargins();

	void DrawTitle(Graphics* pGraphics);
	void DrawSeparator(Graphics* pGraphics);
	void DrawPortInfo(Graphics* pGraphics);
	void DrawModel(Graphics* pGraphics);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
};
