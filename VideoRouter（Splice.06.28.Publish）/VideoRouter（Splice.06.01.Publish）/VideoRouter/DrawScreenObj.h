
#pragma once

// CDrawScreenObj 

class CDrawScreenObj : public CCmdTarget
{
	friend class CPageMouseManager;
	friend class CDrawWndPage;
	DECLARE_DYNAMIC(CDrawScreenObj)

public:
	CDrawScreenObj();
	CDrawScreenObj(CDrawWndPage* pOwner);
	virtual ~CDrawScreenObj();

	CDrawWndPage* m_pOwner;
	void SetOwner(CDrawWndPage* pOwner);

	//��Ա��������
public:	
	//���ô���Ļ��ID
	void SetID(const int nID);
	int GetID() const { return m_iScreenID; }

	//���ô���Ļ�����������
	void SetOutPort(const int iPort);
	int GetOutPort() const { return m_iOutPort; }

	//���ô���Ļ��ѡ��״̬
	void SetFocus(BOOL bFocus);
	BOOL IsFocused() const { return m_bFocused; }

	//�رպʹ򿪴���Ļ
	void SetShutDown(BOOL bShutDown);
	BOOL IsShutDown() const { return m_bShutDown; }

	//������Ļ����
	void SetName(char *szName);
	char* GetName() const { return (char*)m_szName; }

	//���ô���Ļ�Ļ�ͼ����
	void SetLogCoords(const RectF rcScreen);
	RectF GetLogCoords() const { return m_rcLog; }

	//������Ļ����������
	void SetPhyCoords(const Rect rc);
	Rect GetPhyCoords() const { return m_rcPhy; };

	// ������Ļλ��
	void Lock(BOOL bLock = TRUE);
	BOOL IsLocked() const { return m_bLock; }

	// ����
	void Zoom();

	void OnOutputChange();

private:
	//���ƴ���Ļ
	virtual void Draw(CDC *pPrarentDC);

	void DrawScreenBk(Graphics* pGraphics);
	void DrawGridLine(Graphics* pGraphics);

	//��Ļ�ϵ��Ҽ��˵�
	virtual void OnPopupMenu(CPoint point);

	//��Ļ�ϵ����������´���
	virtual void OnLBtnDown(CPoint point);

	//��Ļ�ϵ�������������
	virtual void OnLBtnUp(CPoint point);

	//��Ļ�ϵ�������˫������
	virtual void OnLBtnDblClick(CPoint point);

	//��Ļ�ϵ�����ƶ�����
	virtual void OnMouseMove(CPoint point);

	//��Ա����
private:	

	static int m_iHorz;  //��Ļ��ͨ������ �߼���Ļ��-ˮƽ
	static int m_iVert;  //��Ļ��ͨ������ �߼���Ļ��-��ֱ
	static BOOL m_bLockWall; //ȫ�ֵ���Ļǽ�������
	int  m_iScreenID; //����Ļ��ID
	int  m_iOutPort;  //����Ļ��Ӧ������˿ں�
	BOOL m_bFocused;  //����Ļ�Ƿ�ѡ��
	BOOL m_bShutDown; //����Ļ�Ƿ��ѹر�
	BOOL m_bLock;     //��������Ļλ��
	char m_szName[_MAX_WND_NAME];    //����Ļ���ƣ��������ã�Ĭ��Ϊ��Ļ��
	RectF m_rcLog;    //����Ļ����ͼ�е�����
	Rect  m_rcPhy;    //����Ļ�ڱ����е���������

protected:
	DECLARE_MESSAGE_MAP()
};


