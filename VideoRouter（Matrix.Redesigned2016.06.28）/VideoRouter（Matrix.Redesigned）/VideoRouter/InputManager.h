#pragma once

//******************************************
// CInputManager
// ����ƴ��������˿ڹ�����
//

class CInputManager : public CObject
{
	friend class CGlobalManager;
	friend class CInitialManager;
	friend class COutputManager;
public:
	CInputManager();
	virtual ~CInputManager();

	virtual void DemoInit();
	virtual BOOL RealInit();

	// �˿ڳ�ʼ��
	BOOL InitPorts();
	// ��ʼ�����˿ڵ��ź�����
	BOOL InitSignals();

	// �ź�ˢ��
	BOOL RefreshAllSignals();
	BOOL RefreshThisSignal(const int nIndex);

	// ��λ
	virtual BOOL Reset(const int iKey, BOOL byID=TRUE);
	virtual BOOL ResetAll();

	//��ǰѡ�е��ź�Դ�˿ں�
	static int m_sCurPort;

private:
	CArray <BASEPORT, BASEPORT&> m_arrPort;

public:
	//����ID ������������
	int FindID(const int nID) const;
	//�����߼�ͨ�Ŷ˿ںţ���������ֵ
	int FindCmdPort(const int nCmdPort) const;
	int FindCmdPort(const int nCmdPort, const int nSubPort) const;

	//----------------------------- ���鶯̬���� -------------------//
	//��ȡ�˿���
	virtual int GetCount() const;

	//������ж˿�
	virtual void RemoveAll();

	//ɾ��ָ���˿�
	virtual void RemoveAt(const int iKey, BOOL byID=TRUE); //byID=TRUE iKey��ʾһ���˿�ID��byID=FALSE iKey��ʾԪ������

	//��Ӷ˿�
	virtual void Add(BASEPORT &basePort);

	//----------------------------- �˿�ID���� ----------------------//
	//��ȡ�˿�ID������Ԫ�����
	virtual int GetID(const int nIndex) const;
	virtual void SetID(const int nIndex, const int nID);

	//----------------------------- �˿����ƹ��� --------------------//
	//���ö˿�����
	virtual void SetName(const int iKey, char* szName, BOOL byID=TRUE);
	//��ȡ�˿�����
	virtual char* GetName(const int iKey, BOOL byID=TRUE) const;
	virtual void Rename(const int nIndex, const CPoint ptDlgPos);

	//----------------------------- �˿����͹��� --------------------//
	//���ö˿�����
	virtual void SetSignalType(const int iKey, emVideoType eVT, BOOL byID=TRUE);
	//��ȡ�˿�����
	virtual emVideoType GetSignalType(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- �ֱ��ʹ��� -----------------------//
	//��������ֱ���
	virtual void SetTiming(const int iKey, TIMING &st, BOOL byID=TRUE);
	//��ȡ����ֱ���
	virtual TIMING GetTiming(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- ͨ�Ŷ˿ںŹ��� -------------------//
	//����ͨ�Ŷ˿ں�
	virtual void SetCmdPort(const int iKey, const int nPort, BOOL byID=TRUE);
	//��ȡͨ�Ŷ˿ں�
	virtual int  GetCmdPort(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- �߼��Ӷ˿ڹ��� -------------------//
	virtual void SetSubID(const int index, const int iSubID);
	virtual int  GetSubID(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- EDID���� -------------------//
	virtual BOOL UpdateEDID(const int iKey, BOOL byID=TRUE) const;

	//------------------------- ���õ�ǰ�����ź� ----------------//
	virtual void SetCurrencyResolution(const int iKey, const int iHactive, const int iVactive, const int iHz, BOOL byID=TRUE);
	virtual BOOL GetCurrencyResolution(const int iKey, int &iHactive, int &iVactive, int &iHz, BOOL byID=TRUE) const; 


	//------------ �߼��ź�����ʵ�źŵ�ת����ϵ���� ------------//
	// ˵����
	//          һ��أ�һ������˿�ʵ����ֻ�ܽ�һ·�����źţ��ڶ˿ڹ�������Ҳֻռ��һ���߼��ź�ID��
	//     ����������£���Щ����˿�(��4k�źŴ���˿�)���źű�������·���߸���·�ź�������ÿһ·����ռ��һ���߼��ź�ID��
	//     ��ˣ��ڶ˿ڹ�������һ������˿ھͿ���ռ���������߸�����߼��ź�ID���ڳ������������Ҫ��һЩת��������
	//
	// ��ʵ��ţ������źŵ���ţ��ų��������źź�ľ����
	// �߼���ţ��߼��źŵ���ţ������������ź����ڵ����

	// ��ȡ��ʵ��� ���ų��������ź�֮�����ţ��������ź�Ҳռ����һ��ID���ʶ���ʵ��Ų�����IDһһ��Ӧ��
	virtual int GetRealIndex(const int nID) const;
	// ��ȡ��ʵ�źŸ���
	virtual int GetRealCount() const;
	// ��һ���߼����ת������ʵ���
	virtual int LogicIndex2RealIndex(const int nLogicIndex) const;
	// ��һ����ʵ���ת�����߼����
	virtual int RealIndex2LogicIndex(const int nRealIndex) const;
};


extern CInputManager *pInputManager;