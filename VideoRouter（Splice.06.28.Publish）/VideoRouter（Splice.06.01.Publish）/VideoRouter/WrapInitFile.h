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


	//设置Section、Key以及KeyValue，若Section或者Key不存在则创建
	void SetKeyValueString(CString strSection,CString strKey,CString strKeyValue);

	//设置Section、Key以及KeyValue，若Section或者Key不存在则创建
	void SetKeyValueInt(CString strSection,CString strKey,INT iKeyValue);

	//获得所有的Section
	//返回Section数目
	int GetAllSections(CStringArray& strArrSection);

	//根据指定Section得到其下的所有Key和KeyValue
	//返回Key的数目
	int GetAllKeysAndValues(CString strSection,CStringArray& strArrKey,CStringArray& strArrKeyValue);

	//删除指定Section下的一个Key
	void DeleteKey(CString strSection,CString strKey);

	//删除指定的Section以及其下的所有Key
	void DeleteSection(CString strSection);

	//删除所有Section
	void DeleteAllSections();

private:
	//ini文件路径
	CString m_strPath;
};
