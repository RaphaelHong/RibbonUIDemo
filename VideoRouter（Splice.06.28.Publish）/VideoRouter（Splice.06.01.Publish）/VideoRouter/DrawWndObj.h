
#pragma once
//#include "DrawObj.h"

// CDrawWndObj ����Ŀ��

class CDrawWndObj : public CCmdTarget
{
	friend class CPageMouseManager;
	friend class CDrawWndPage;

	DECLARE_DYNAMIC(CDrawWndObj)

public:
	CDrawWndObj(CDrawWndPage* pOwner);
	virtual ~CDrawWndObj();

	CDrawWndPage *m_pOwner;
	void SetParentPage(CDrawWndPage* pOwner);

	static int m_sxPhyScalar; //�������
	static int m_syPhyScalar; 
	static int m_sxLogScalar; //�߼�����
	static int m_syLogScalar;
	//���ñ���
	static void SetScalar(const int ixPhy, const int iyPhy, const int ixLog, const int iyLog);

	int Write(__in char* pBlock, __in const long lsize, __out long &lPos) const; //For Write File
	int Write(__in UINT* pBlock, __in const long lsize, __out long &lPos) const; //For Write Device
	int Read(__in char* pBlock, __in const long lsize, __out long &lPos); //For Read File
	int Read(__in UINT* pBlock, __in const long lsize, __out long &lPos); //For Read Device
	static int GetObjectSize();

	//��Ա
private:
	//{
	int m_iWndID;         //����ID
	int m_iSrcPort;       //�ź�Դ
	int m_iZorder;        //���ڵ��Ŵ���
	int m_iAlpha;         //����͸����
	char m_szName[_MAX_WND_NAME];    //���ڱ���

	CRect m_rcPhy;        //�����ڱ�����Ļ�ϵ�λ��  
	CRect m_rcLog;        //��������ͼ�ĵ��ϵ�λ��
	CRect m_rcLastPhy;    //������һ�ε���Ļλ��
	CRect m_rcLastLog;    //������һ�ε���ͼλ��
	CRect m_rcBtnExit;    //�رհ�ť��λ��
	CRect m_rcBtnRestore; //��ԭ��ť��λ��
	CRect m_rcBtnMaximize;//��󻯰�ť��λ��
	CRect m_rcBtnAlign;   //���밴ť��λ��
	CRect m_rcTitle;      //����λ��

	COLORREF m_clrBk;     //���屳����ɫ
	//}

	BOOL m_bLocked;       //�����Ƿ�����
	BOOL m_bActived;      //�����Ƿ��ڼ���״̬

	//����
public:
	//����ID
	void SetWndID( const int iID);
	//��ȡID
	int GetWndID() const { return m_iWndID; };

	//�����ź�Դ
	void SetSourcePort(const int iSrcPort);
	//��ȡ�ź�Դ
	int  GetSourcePort() const { return m_iSrcPort; };

	//���õ���˳��
	void SetZorder(const int iZorder);
	//��ȡ����˳��
	int  GetZorder() const { return m_iZorder; };

	//͸��ϵ��
	void SetAlpha(const int iAlpha);
	int  GetAlpha() const { return m_iAlpha; }

	//��������
	void SetName(const char* szName);
	//��ȡ����
	char* GetName() const { return (char*)m_szName; };

	//����
	void SetLock(BOOL bLock);
	//�Ƿ�����
	BOOL IsLocked() const { return m_bLocked; };

	//����
	void Active(BOOL bActive);
	//�Ƿ񼤻�
	BOOL IsActived() const { return m_bActived;};

	//���ô��ڵ���Ļ����
	void SetPhyPos(const CRect rcPhy);
	void SetPhyPos(const Rect rcPhy);
	//��ȡ���ڵ���Ļ����
	CRect GetPhyPos() const { return m_rcPhy; };

	//���ô��ڵ���ͼ����
	void SetLogPos(const CRect rcLog);
	//��ȡ���ڵ���ͼ����
	CRect GetLogPos() const { return m_rcLog; };
	//���¼��㴰�ڲ���
	void RecalcLayOut();

	void SetPosMark();

	//��������
	void Zoom();

	//֪ͨ�������ػ��˴���
	void OnGraphChanged();

	// ��������
	int Open();
	int Size();
	int Move();
	int MoveZ();
	int MoveS();
	static int Shut(const int nID);

private:
	//���ô�����ɫ
	void SetBkColor(COLORREF clrBk);
	//���ڻ���
	void Draw(CDC *pDC);
	void DrawMethod2(Graphics *pGraphics);

	//�Ҽ��˵�
	void OnPopupMenu(CPoint point);

	//���������´���
	void OnLBtnDown(CPoint point);

	//������������
	void OnLBtnUp(CPoint point);

	//������˫������
	void OnLBtnDblClick(CPoint point);

	//����ƶ�����
	void OnMouseMove(CPoint point);

	//�����׷��,bTest=TRUE����HITTEST��=FALSEֻ���������̬
	int OnHitTest(CPoint point, BOOL bTest);
	void LBtnUpReturn();

	// �ر�
	void OnClose();
	// ���
	void OnMaximize();
	// ����
	void OnAlignTo();
	// ��ԭ
	void OnRestore();

	CPoint m_ptLBtnDown;
	int m_HitTest;        //���׷��

protected:
	DECLARE_MESSAGE_MAP()
};


