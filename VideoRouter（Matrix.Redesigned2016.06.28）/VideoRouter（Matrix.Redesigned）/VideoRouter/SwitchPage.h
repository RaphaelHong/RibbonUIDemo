#pragma once

#include "SwitchHeadBar.h"
#include "SwitchSourceBar.h"
#include "SwitchSelectBar.h"
#include "PortHelpText.h"

// �л����ĳߴ��ģ
class CSwitchScale 
{
public:
	CSwitchScale() {}
	CSwitchScale(const CSwitchScale & scale) {
		iSelectBoxSize = scale.iSelectBoxSize;
		iSourceBarWidth = scale.iSourceBarWidth;
		iHorzSpace = scale.iHorzSpace;
		iVertSpace = scale.iVertSpace;
		iFontHeight = scale.iFontHeight;
	}

	int iSelectBoxSize;
	int iSourceBarWidth;
	int iHorzSpace;
	int iVertSpace;
	int iFontHeight;
};
// ��ߴ� ��ť���ش�С24x24
class CLargeScale : public CSwitchScale
{
public:
	CLargeScale () 
	{
		iSelectBoxSize  = 24;
		iSourceBarWidth = 190;
		iHorzSpace      = 10;
		iVertSpace      = 10;
		iFontHeight     = 16;
	}
};
// �гߴ� ��ť���ش�С18x18
class CMediumScale : public CSwitchScale
{
public:
	CMediumScale ()
	{
		iSelectBoxSize  = 20;
		iSourceBarWidth = 190;
		iHorzSpace      = 8;
		iVertSpace      = 8;
		iFontHeight     = 12;
	}
};
// С�ߴ� ��ť���ش�С12x12
class CSmallScale : public CSwitchScale
{
public:
	CSmallScale ()
	{
		iSelectBoxSize  = 16;
		iSourceBarWidth = 190;
		iHorzSpace      = 7;
		iVertSpace      = 7;
		iFontHeight     = 10;
	}
};

/* �л�����ͼ�ߴ��ģ���ܹ��д�(24Pixel)����(18Pixel)��С(12Pixel)���ֹ�ģ��*/
enum emZoomLevel
{
	eZLSmall  = 1,
	eZLMedium = 2,
	eZLLarge  = 3,
};

// �������߾�����
class CBasicBarMargins
{
public:
	int iBarWidth;           // �����
	int iBarHeight;          // ���߶�

	int iItemWidth;          // ����
	int iItemHeight;         // ��߶�

	int iMaxVisiableItemsOnHorz;   // ����ˮƽ�������ɼ�����
	int iMaxVisiableItemsOnVert;   // ���д�ֱ�������ɼ�����
};

// ͷĿ���߾�����
class CHeadBarMargins : public CBasicBarMargins
{
public:
	int iColumnWidthFst;   // ��һ����� ����SourceBar�����ȣ�
	int iColumnWidthSec;   // �ڶ������
	int iColumnWidthThd; // ��������� ���ڶ�������������֮�͵���SelectBar��ȣ�
};

// �ź�Դ���߾�����
typedef CBasicBarMargins CSourceBarMargins;

// �л���ť���߾�����
typedef CBasicBarMargins CSelectBarMargins;

// ����ҳ�߾�����
class CPageMargins
{
public:
	int iPageWidth;        // ҳ����
	int iPageHeight;       // ҳ��߶�

	int iTopBorderHeight;  // �ϱ߿�߶�
	int iBotBorderHeight;  // �±߿�߶�

	//int iFontHeight;       // �����С

	CHeadBarMargins   HeadBarMargin;
	CSourceBarMargins SourceBarMargin;
	CSelectBarMargins SelectBarMargin;

	CSwitchScale PageScale;
};

//////////////////////////////////////////////////////////////////////////
// SwitchPage
// һ�������л������߼��ӿ���ҳ ����һ��ͷ�������������һ������źſ�������һ���л�ѡ��ť��

class CSwitchPage : public CWnd
{
	friend class CSwitchHeadBar;
	friend class CSwitchSourceBar;
	friend class CSwitchSelectBar;

	DECLARE_DYNAMIC(CSwitchPage)
public:
	CSwitchPage();
	virtual ~CSwitchPage();

	//static int m_sFirstVisibleInput;  // ��һ���ɼ��������
	//static int m_sFirstVisibleOutput; // ��һ���ɼ��������
	static emZoomLevel m_sZoomLvl;  // ��ǰҳ��ߴ��ģ
	
	// �����л���������
	void SetPageRect(const CRect rc);

	// ��ȡ��ǰ�߾�����
	const CPageMargins* GetMargins() { return &m_Margins; }
	void SetMargins();      /*���ñ߾�*/

	void RecalLayout();

private:	
	CRect m_rcPage;   /* �л������� */

	CPageMargins m_Margins; /*ҳ��߾�*/
	
	CSwitchHeadBar m_wndHeadBar;      // ͷ��
	CSwitchSourceBar m_wndSourceBar;  // �ź���
	CSwitchSelectBar m_wndSelectBar;  // �л���

	BOOL CreateHeadBar();//����ͷ����
	BOOL CreateSourceBar();//�����ź�Դ��
	BOOL CreateSelectBar();//����ѡ����
	inline void CalcHeadBarPos(CRect &rcPos);//����ͷ������λ��
	inline void CalcSourceBarPos(CRect &rcPos);//�����ź�Դ����λ��
	inline void CalcSelectBarPos(CRect &rcPos);//����ѡ������λ��

	CWnd* GetHeadBar();//��ȡͷ�����Ĵ���ָ��
	CWnd* GetSourceBar();//��ȡ�ź�Դ���Ĵ���ָ��
	CWnd* GetSelectBar();//��ȡѡ�����Ĵ���ָ��

private:
	
	CScrollBar m_Horzbar;// ˮƽ�����������Թ�������˿ڣ���ǰ��ͼ��С��������ʾ��������˿ڵ�ʱ�򣬴˹������ͻ��ڶ������֣���֮�������
	void SetHScrollBar();// ����������

	// �±߿��ϵķ�ҳ��ť
	CBnItem m_bnFarBottom;//��������ײ���ť
	CBnItem m_bnPageDown;//���·�ҳ��ť
	void SetBnItem();//������ť

	// ��ť��ʾ
	CToolTipCtrl m_tips;//��̬��ʾ��
	void SetTooltips();//������ʾ��

	// �˿���ʾ��
	CPortHelpText* m_pHelptext;//�ź�Դ��Ϣ��ʾ��ָ��
	void SetNeedHelpPort(const int nID, const int nType);//�����ź�Դ��Ϣ��ʾ��
	INT_PAIR GetNeedHelpPort();//��ȡ��ǰ��Ҫ��Ϣ��ʾ�Ķ˿�
	void ShowHelpText(BOOL bShow, CStringW strText=L"", CPoint ptText=CPoint(0,0));//������ʾ��ʾ��
	BOOL IsHelpTextVisible();//�ź�Դ��ʾ���Ƿ�ɼ�
	void RedrawHelpText();//ˢ���ź�Դ��ʾ��

public:

	// �Ի水ť��Click�¼�����
	void RefreshAll();  //ˢ������
	void FarBottom();   //�ź�����������׶�
	void PageDown();    //�ź������¹���һҳ
	void FarTop();      //�ź������������
	void PageUp();      //�ź������Ϲ���һҳ
	void FarLeft();     //������б�����������
	void PageLeft();    //������б��������һҳ
	void FarRight();    //������б���������ұ�
	void PageRight();   //������б����ҹ���һҳ

	void ManageWizardBn(); //�����й�����ť��ʹ�ܴ���

public:

	// ������������ļ�
	void PackFile(__in LPCTSTR lpFile);
	// ���������ļ�
	void ParseFile(__in LPCTSTR lpFile);
	// �����������ݿ�
	void ParseSceneBlock(__in UINT* pBlock, __in int nBlockLen);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnFarBottom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPageDown(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


