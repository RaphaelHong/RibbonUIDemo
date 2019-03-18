#pragma once

//******************************************
// CInitialManager
// ��ʼ�����������
//

class CInitialManager : public CObject
{
	friend class CGlobalManager;
public:
	CInitialManager();
	virtual ~CInitialManager();

	// ��ʼ����������{ȫ�������������ͨ�����������������������������������}
	void InitManagers();

	// �ͷ��ڴ���Դ
	void ReleaseManagers();

	// ��ʼ���豸���� {����ͨ�ź���豸��ȡ���ݲ���ʼ��������}
	BOOL InitDeviceContents();
	// ��ʼ��ƴ������
	BOOL InitSpliceLayout();
	// ��ʼ��ƴ�ӳ���
	BOOL InitSpliceScene();

	/* ������������ �� �ڵ����������߳�֮ǰ(BeginTransaction����)���������������� */
	void SetTransactionType(const emTransaction eTransaction);

	/* ���������� */
	BOOL BeginTransaction(BOOL bShowWaitWnd);
	void EndTransacation();

	/* ���ó�ʼ����־������ÿ�����ӳ�ʼ����Ϻ���ΪTRUE��ÿ�ζϿ����Ӻ���ΪFALSE */
	void SetInitializedFlag(BOOL bInit);
	BOOL GetInitializedFlag() const;

private:
	emTransaction m_eTransaction;
	BOOL m_bInitialized; //������ͨ�����ӳ�ʼ������
};


extern CInitialManager *pInitManager;