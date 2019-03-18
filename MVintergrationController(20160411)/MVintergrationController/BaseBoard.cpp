#pragma once

#include "StdAfx.h"
#include "MVintergrationController.h"
#include "BaseBoard.h"
#include "DefinesFile/MVserial/Macros/MVmacros.h"
#include "DefinesFile/MGserial/Macros/MGmacros.h"
#include "DefinesFile/MVXserial/Macros/MVXmacros.h"
#include "DefinesFile/HDMI3Dserial/Macros/HDMImacros.h"

CBaseBoard::CBaseBoard(void)
{
	m_nBoardID     = 0x0;
	m_nBoardFuncID = 0x0;
	m_nBoardPort   = 0xff;
	m_strBoardName = UNKNOW_DEV_NAME;
	m_ProductType  = Product_INVALID;
}

CBaseBoard::~CBaseBoard(void)
{
}


//********************** �忨 ID ****************
int CBaseBoard::GetBoardID() const
{
	return m_nBoardID;
}
void CBaseBoard::SetBoardID(int uID)
{
	if (uID == -1) return;

	m_nBoardID = uID;

	SetProductType();
}


//****************** �忨 ������ ****************
int CBaseBoard::GetBoardFuncID() const
{
	return m_nBoardFuncID;
}
void CBaseBoard::SetBoardFuncID(int uFuncID)
{
	if (uFuncID == -1) return;

	m_nBoardFuncID = uFuncID;
}
int CBaseBoard::GetFunCodeByte(int nByteOder) const
{
	if (nByteOder < 0 || nByteOder > 3) return 0xff;

	return (m_nBoardFuncID>>(nByteOder*8))&0xff;
}

//***************** �忨 ͨ�Ŷ˿� ***************
int CBaseBoard::GetBoardPort() const
{
	return m_nBoardPort;
}
void CBaseBoard::SetBoardPort(int uPort)
{
	m_nBoardPort = uPort;
}


//****************** �忨 ���� ******************
CString CBaseBoard::GetBoardName() const
{
	return m_strBoardName;
}

void CBaseBoard::SetExtendedName(int nExt)
{
	if (nExt <= 0) return; //����ͬ���Ͱ忨

	if (m_strBoardName == UNKNOW_DEV_NAME) return; //δ֪�忨

	CString strExtName;

	strExtName.Format(_T("(%d)"), nExt);
	m_strBoardName += strExtName;
}


//************* �忨(��Ʒ) �ͺ� *****************
void CBaseBoard::SetProductType()
{
	m_ProductType = Product_INVALID;
	m_strBoardName = UNKNOW_DEV_NAME;

	//�Ƿ��Ƕ�������Ʒ
	if (m_ProductType == Product_INVALID)
	{
		for (int i=0; i<TOTAL_MVIEW_TYPES; i++)	{
			if (m_nBoardID == stMviewDevTypes[i].m_Dev_ID)		{
				m_ProductType = Product_MV;
				m_strBoardName = stMviewDevTypes[i].m_strDev_Dev_Name;
				break;
			}
		}
	}

	//�Ƿ���MG300��Ʒ
	if (m_ProductType == Product_INVALID)
	{
		for (int i=0; i<TOTAL_MGEO_TYPES; i++)	{
			if (m_nBoardID == stMGeoDevTypes[i].m_Dev_ID)	{
				m_ProductType = Product_MG;
				m_strBoardName = stMGeoDevTypes[i].m_strDev_Dev_Name;
				break;
			}
		}
	}

	//�Ƿ���MVX�����ָ�ģ��
	if (m_ProductType == Product_INVALID)
	{
		for (int i=0; i<TOTAL_MVX303_TYPE; i++)	{
			if (m_nBoardID == stMVX303DevTypes[i].m_Dev_ID)	{
				m_ProductType = Product_MVX;
				m_strBoardName = stMVX303DevTypes[i].m_strDev_Dev_Name;
				break;
			}
		}
	}

	//�Ƿ���HDMI_3D��Ʒ
	if (m_ProductType == Product_INVALID)
	{
		for (int i=0; i<TOTAL_HDMI3D_TYPE; i++) {
			if (m_nBoardID == stHDMI3DDevTypes[i].m_Dev_ID)
			{
				m_ProductType = Product_HDMI_3D;
				m_strBoardName = stHDMI3DDevTypes[i].m_strDev_Dev_Name;
				break;
			}
		}
	}
}

emProductType CBaseBoard::GetProductType() const
{
	return m_ProductType;
}

// ƥ��ֱ�������
CString *CBaseBoard::GetTimingName(const emProductType etype) const
{
	if (etype == Product_MV)
	{//MV������
		for (int i=0; i<TOTAL_MVIEW_TYPES; i++)
		{
			if (stMviewDevTypes[i].m_Dev_ID == m_nBoardID
				&& stMviewDevTypes[i].m_nFunc_Code == m_nBoardFuncID)
			{
				return stMviewDevTypes[i].m_pStrTiming;
			}
		}
	}
	else if (etype == Product_MVX)
	{//MVX303ģ��
		return stMVX303DevTypes[0].m_pStrTiming;
	}
	else if (etype == Product_MG)
	{//MG300ģ��
		return strMGtimings;
	}
	else if (etype == Product_HDMI_3D)
	{//HDMI_3Dģ��
		return strHDMI3D_TIMING;
	}

	return NULL;
}

// ƥ��ֱ���
TIMING_STRUCT *CBaseBoard::GetDetailTiming(const emProductType etype) const
{
	if (etype == Product_MV)
	{//MV������
		for (int i=0; i<TOTAL_MVIEW_TYPES; i++)
		{
			if (stMviewDevTypes[i].m_Dev_ID == m_nBoardID
				&&stMviewDevTypes[i].m_nFunc_Code == m_nBoardFuncID)
			{
				return stMviewDevTypes[i].m_pDetailed_Timing;
			}
		}
	}
	else if (etype == Product_MVX)
	{//MVX303
		return stMVX303DevTypes[0].m_pDetailed_Timing;
	}
	else if (etype == Product_MG)
	{//MG300ģ��
		return MG_Detailed_timing;
	}
	else if (etype == Product_HDMI_3D)
	{//HDMI_3Dģ��
		return stHDMI3D_detailed_timing;
	}

	return NULL;
}

// ��ȡ�ֱ�������
int CBaseBoard::GetTotalTimings(const emProductType etype) const
{
	if (etype == Product_MG)
	{	//MG300ģ��
		return TOTAL_MG_TIMING;
	}
	else if (etype == Product_MV)
	{	//MV������ģ��
	}
	else if (etype == Product_MVX)
	{	//MVX�����ָ�ģ��
		return TOTAL_MVX303_HDMI_TIMINGS;
	}
	else if (etype == Product_HDMI_3D)
	{   //HDMI_3Dģ��
		return TOTAL_HDMI3D_TIMINGS;
	}

	return 0;
}