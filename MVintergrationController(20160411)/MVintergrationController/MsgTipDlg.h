#pragma once

typedef struct _MSG_TIP//消息提示数据项描述信息
{
	CString sMsgItemName;                  //信息标题

	CString sMsgItemInfo;               //信息内容

	COLORREF nInfoShowColor; //显示颜色(此项值目前暂时没有使用，原意是使每个栏目的文字颜色均可自定义；因为考虑到设定的颜色可能和背景色冲突，因此目前文字颜色使用背景色的反色)

}MSG_TIP;

typedef CArray<MSG_TIP,MSG_TIP&> CMsgTipArray;//消息提示内容

// CMsgTipDlg 对话框

class CMsgTipDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgTipDlg)
public:
	CMsgTipDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgTipDlg();

// 对话框数据
	enum { IDD = IDD_MSGTIPDLG };

public:
	CSize ShowMsgTip(CMsgTipArray &arMsgTip); //供外部函数传递需要显示的内容，并计算尺寸  
	void UpdateTrans();     //修改透明度等配置参数      
	void DrawMsgTip();      //绘制提示信息

protected:  
	CMsgTipArray m_arMsgTip;    //提示信息数组  
	CFont   m_MsgTipFont;       //用于消息提示的字体  
	int     m_nTipNameWidth;    //标题名最大宽度，决定竖线绘制位置  
	int     m_nRowHeight;       //行高，决定横线绘制位置  

private:  
	void DrawGrid(CDC *pDC,CRect &rc);                      //绘制网格  
	void DrawMsgTip(CDC *pDC,CPoint topPos,MSG_TIP &mt);    //绘制提示信息(某一条)  
	void DrawMsg(CDC *pDC);                                 //绘制信息

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
