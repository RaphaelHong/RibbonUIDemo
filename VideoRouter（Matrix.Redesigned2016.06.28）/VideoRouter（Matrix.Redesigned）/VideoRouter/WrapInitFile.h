#pragma once

class CWrapInitFile
{
public:
	CWrapInitFile(void);
	~CWrapInitFile(void);

	BOOL SetInitFilePath(CString strPath);

	BOOL SectionExist(CString strSection);

	CString GetKeyValueString(CString strSection,CString strKey);
	CString GetKeyValueStringDef(CString strSec, CString strKey, CString strDefault);
	
	INT GetKeyValueInt(CString strSection,CString strKey);
	INT GetKeyValueIntDef(CString strSec, CString strKey, int nDefault);


	//����Section��Key�Լ�KeyValue����Section����Key�������򴴽�
	void SetKeyValueString(CString strSection,CString strKey,CString strKeyValue);

	//����Section��Key�Լ�KeyValue����Section����Key�������򴴽�
	void SetKeyValueInt(CString strSection,CString strKey,INT iKeyValue);

	//������е�Section
	//����Section��Ŀ
	int GetAllSections(CStringArray& strArrSection);

	//����ָ��Section�õ����µ�����Key��KeyValue
	//����Key����Ŀ
	int GetAllKeysAndValues(CString strSection,CStringArray& strArrKey,CStringArray& strArrKeyValue);

	//ɾ��ָ��Section�µ�һ��Key
	void DeleteKey(CString strSection,CString strKey);

	//ɾ��ָ����Section�Լ����µ�����Key
	void DeleteSection(CString strSection);

	//ɾ������Section
	void DeleteAllSections();

private:
	//ini�ļ�·��
	CString m_strPath;
};
