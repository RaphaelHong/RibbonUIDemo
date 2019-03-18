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


//********************** 板卡 ID ****************
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


//****************** 板卡 功能码 ****************
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

//***************** 板卡 通信端口 ***************
int CBaseBoard::GetBoardPort() const
{
	return m_nBoardPort;
}
void CBaseBoard::SetBoardPort(int uPort)
{
	m_nBoardPort = uPort;
}


//****************** 板卡 名称 ******************
CString CBaseBoard::GetBoardName() const
{
	return m_strBoardName;
}

void CBaseBoard::SetExtendedName(int nExt)
{
	if (nExt <= 0) return; //无相同类型板卡

	if (m_strBoardName == UNKNOW_DEV_NAME) return; //未知板卡

	CString strExtName;

	strExtName.Format(_T("(%d)"), nExt);
	m_strBoardName += strExtName;
}


//************* 板卡(产品) 型号 *****************
void CBaseBoard::SetProductType()
{
	m_ProductType = Product_INVALID;
	m_strBoardName = UNKNOW_DEV_NAME;

	//是否是多屏保产品
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

	//是否是MG300产品
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

	//是否是MVX三屏分割模块
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

	//是否是HDMI_3D产品
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

// 匹配分辨率名称
CString *CBaseBoard::GetTimingName(const emProductType etype) const
{
	if (etype == Product_MV)
	{//MV三屏宝
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
	{//MVX303模块
		return stMVX303DevTypes[0].m_pStrTiming;
	}
	else if (etype == Product_MG)
	{//MG300模块
		return strMGtimings;
	}
	else if (etype == Product_HDMI_3D)
	{//HDMI_3D模块
		return strHDMI3D_TIMING;
	}

	return NULL;
}

// 匹配分辨率
TIMING_STRUCT *CBaseBoard::GetDetailTiming(const emProductType etype) const
{
	if (etype == Product_MV)
	{//MV三屏宝
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
	{//MG300模块
		return MG_Detailed_timing;
	}
	else if (etype == Product_HDMI_3D)
	{//HDMI_3D模块
		return stHDMI3D_detailed_timing;
	}

	return NULL;
}

// 获取分辨率数量
int CBaseBoard::GetTotalTimings(const emProductType etype) const
{
	if (etype == Product_MG)
	{	//MG300模块
		return TOTAL_MG_TIMING;
	}
	else if (etype == Product_MV)
	{	//MV三屏宝模块
	}
	else if (etype == Product_MVX)
	{	//MVX三屏分割模块
		return TOTAL_MVX303_HDMI_TIMINGS;
	}
	else if (etype == Product_HDMI_3D)
	{   //HDMI_3D模块
		return TOTAL_HDMI3D_TIMINGS;
	}

	return 0;
}