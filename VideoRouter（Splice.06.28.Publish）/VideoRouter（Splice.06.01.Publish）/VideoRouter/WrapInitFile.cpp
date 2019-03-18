#include "StdAfx.h"
#include "WrapInitFile.h"

#define         MAX_SECTION         256		//Section最大长度
#define         MAX_KEY             256			//KeyValues最大长度
#define         MAX_ALLSECTIONS     65535		//所有Section的最大长度
#define         MAX_ALLKEYS         65535		//所有KeyValue的最大长度


CWrapInitFile::CWrapInitFile(void)
{
}

CWrapInitFile::~CWrapInitFile(void)
{
}

BOOL CWrapInitFile::SetInitFilePath(CString strPath)
{
	m_strPath = strPath;

	//检查文件是否存在
	DWORD  dwFlag = GetFileAttributes((LPCTSTR)m_strPath);

	//文件或者路径不存在，创建
	if( 0xFFFFFFFF == dwFlag )
	{
		CFile fl;
		if(!fl.Open(m_strPath,CFile::modeCreate|CFile::modeReadWrite))
			return false;
		else
			fl.Close();
	}
	return TRUE;
}

BOOL CWrapInitFile::SectionExist(CString strSection)
{
	TCHAR chSection[MAX_SECTION];
	DWORD dwRetValue;

	dwRetValue = GetPrivateProfileString(
		(LPCTSTR)strSection,
		NULL,
		_T(""),
		chSection,
		sizeof(chSection)/sizeof(TCHAR),
		(LPCTSTR)m_strPath);

	return (dwRetValue>0);
}

CString CWrapInitFile::GetKeyValueString(CString strSection,
							  CString strKey)
{
	TCHAR         chKey[MAX_KEY];
	DWORD         dwRetValue;
	CString strKeyValue=_T("");

	dwRetValue = GetPrivateProfileString(
		(LPCTSTR)strSection,
		(LPCTSTR)strKey,
		_T(""),
		chKey,
		sizeof(chKey)/sizeof(TCHAR),
		(LPCTSTR)m_strPath);      

	strKeyValue = chKey;

	return strKeyValue;
}

CString CWrapInitFile::GetKeyValueStringDef(CString strSec, CString strKey, CString strDefault)
{
	TCHAR         chKey[MAX_KEY];
	DWORD         dwRetValue;
	CString strKeyValue=_T("");

	dwRetValue = GetPrivateProfileString(
		(LPCTSTR)strSec,
		(LPCTSTR)strKey,
		strDefault,
		chKey,
		sizeof(chKey)/sizeof(TCHAR),
		(LPCTSTR)m_strPath);      

	strKeyValue = chKey;

	return strKeyValue;
}

INT CWrapInitFile::GetKeyValueInt(CString strSection,
							  CString strKey)
{
	TCHAR         chKey[MAX_KEY];
	DWORD         dwRetValue;
	INT			  iRetValue=0;
//	CString strKeyValue=_T("");

	dwRetValue = GetPrivateProfileString(
		(LPCTSTR)strSection,
		(LPCTSTR)strKey,
		_T(""),
		chKey,
		sizeof(chKey)/sizeof(TCHAR),
		(LPCTSTR)m_strPath);      


	sscanf_s(chKey,"%d",&iRetValue);
	return iRetValue;
}

INT CWrapInitFile::GetKeyValueIntDef(CString strSec, CString strKey, int nDefault)
{
	TCHAR         chKey[MAX_KEY];
	DWORD         dwRetValue;
	INT			  iRetValue=0;

	CString strDefault;
	strDefault.Format(_T("%d"), nDefault);

	dwRetValue = GetPrivateProfileString(
		(LPCTSTR)strSec,
		(LPCTSTR)strKey,
		strDefault,
		chKey,
		sizeof(chKey)/sizeof(TCHAR),
		(LPCTSTR)m_strPath);      


	sscanf_s(chKey,"%d",&iRetValue);
	return iRetValue;
}

void CWrapInitFile::SetKeyValueString(CString strSection,
						   CString strKey,
						   CString strKeyValue)
{
	WritePrivateProfileString(
		(LPCTSTR)strSection,
		(LPCTSTR)strKey,
		(LPCTSTR)strKeyValue,
		(LPCTSTR)m_strPath);
}

void CWrapInitFile::SetKeyValueInt(CString strSection,CString strKey,INT iKeyValue)
{
	TCHAR   chVal[MAX_KEY]; 

	sprintf_s( chVal, "%d", iKeyValue);   

	WritePrivateProfileString(
		(LPCTSTR)strSection,
		(LPCTSTR)strKey,
		(LPCTSTR)chVal,
		(LPCTSTR)m_strPath);
}

void CWrapInitFile::DeleteKey(CString strSection, CString strKey)
{
	WritePrivateProfileString(
		(LPCTSTR)strSection,
		(LPCTSTR)strKey,
		NULL,          //       这里写NULL,则删除Key
		(LPCTSTR)m_strPath);
}

void CWrapInitFile::DeleteSection(CString strSection)
{
	WritePrivateProfileString(
		(LPCTSTR)strSection,
		NULL,         
		NULL,          //       这里都写NULL,则删除Section
		(LPCTSTR)m_strPath);
}

int CWrapInitFile::GetAllSections(CStringArray& strArrSection)
{
	int dwRetValue, i, j, iPos=0;
	TCHAR chAllSections[MAX_ALLSECTIONS];
	TCHAR chTempSection[MAX_SECTION];

	ZeroMemory(chAllSections, MAX_ALLSECTIONS);
	ZeroMemory(chTempSection, MAX_SECTION);

	dwRetValue = GetPrivateProfileSectionNames(
		chAllSections,
		MAX_ALLSECTIONS,
		m_strPath);

	//       因为Section在数组中的存放形式为“Section1”，0，“Section2”，0，0。
	//       所以如果检测到连续两个0，则break
	for(i=0; i<MAX_ALLSECTIONS; i++)
	{
		if( chAllSections[i] == NULL )
		{
			if( chAllSections[i] == chAllSections[i+1] )
				break;
		}
	}

	i++; //         保证数据读完
	strArrSection.RemoveAll(); //         清空数组

	for(j=0; j<i; j++)
	{
		chTempSection[iPos++] = chAllSections[j];
		if( chAllSections[j] == NULL )
		{ 
			strArrSection.Add(chTempSection);
			ZeroMemory(chTempSection, MAX_SECTION);
			iPos = 0;
		}
	}

	return strArrSection.GetSize();
}

int CWrapInitFile::GetAllKeysAndValues(CString  strSection,
								  CStringArray& strArrKey,
								  CStringArray& strArrKeyValue)
{
	int dwRetValue, i, j, iPos=0;
	TCHAR chAllKeysAndValues[MAX_ALLKEYS];
	TCHAR chTempkeyAndValue[MAX_KEY];
	CString strTempKey;

	ZeroMemory(chAllKeysAndValues, MAX_ALLKEYS);
	ZeroMemory(chTempkeyAndValue, MAX_KEY);

	dwRetValue = GetPrivateProfileSection(
		strSection,
		chAllKeysAndValues,
		MAX_ALLKEYS,
		m_strPath);

	//       因为Section在数组中的存放形式为“Key1=KeyValue1”，0，“Key2=KeyValue2”，0
	//       所以如果检测到连续两个0，则break
	for(i=0; i<MAX_ALLSECTIONS; i++)
	{
		if( chAllKeysAndValues[i] == NULL )
		{
			if( chAllKeysAndValues[i] == chAllKeysAndValues[i+1] )
				break;
		}
	}

	i++;
	strArrKey.RemoveAll();
	strArrKeyValue.RemoveAll();

	for(j=0; j<i; j++)
	{
		chTempkeyAndValue[iPos++] = chAllKeysAndValues[j];
		if( chAllKeysAndValues[j] == NULL )
		{ 
			strTempKey = chTempkeyAndValue;
			strArrKey.Add( strTempKey.Left(strTempKey.Find('=')) );
			strArrKeyValue.Add( strTempKey.Mid(strTempKey.Find('=')+1) );
			ZeroMemory(chTempkeyAndValue, MAX_KEY);
			iPos = 0;
		}
	}

	return strArrKey.GetSize();
}

void CWrapInitFile::DeleteAllSections()
{
	int nSecNum;
	CStringArray strArrSection;
	nSecNum = GetAllSections(strArrSection);
	for(int i=0; i<nSecNum; i++)
	{
		WritePrivateProfileString(
			(LPCTSTR)strArrSection[i],
			NULL,
			NULL,
			(LPCTSTR)m_strPath);      
	}
}