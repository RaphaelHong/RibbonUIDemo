#pragma once

#include "DefinesFile/MGserial/Timings/MGtimings.h"

// MG300 设备ID
typedef enum emMGDeviceID
{
	INVALIDE_MG_ID     = 0x0,
	DEV_ID_MG300_S     = 0x2301,
	DEV_ID_MG300_H     = 0x2302,
	DEV_ID_MG300_A     = 0x2303,
	DEV_ID_MG310_S     = 0x2311,
	DEV_ID_MG310_H     = 0x2312,
	DEV_ID_MG310_A     = 0x2313,
};

// MG300 设备功能码
typedef enum emMGFuncCode
{
	INVALID_MG_FUNC    = 0x0,
	DEV_FUNC_MG300_S   = 0x30fe0211,
	DEV_FUNC_MG300_H   = 0x30fe0212,
	DEV_FUNC_MG300_A   = 0x30fe0213,
	DEV_FUNC_MG310_S   = 0x31fe0211,
	DEV_FUNC_MG310_H   = 0x31fe0212,
	DEV_FUNC_MG310_A   = 0x31fe0213,
};


#define TOTAL_MGEO_TYPES 6

DEVICE_STRUCT stMGeoDevTypes[TOTAL_MGEO_TYPES] = {
	//EDID_Name           Dev_Name      EDID_PRD_ID     Dev_ID          Dev_Func_code      Timing_Name     Detailed_timing
	{_T("MG300_S"),     _T("MG300_S"), 	 0,          DEV_ID_MG300_S,  DEV_FUNC_MG300_S,   strMGtimings,  MG_Detailed_timing},
	{_T("MG300_H"),     _T("MG300_H"), 	 0,          DEV_ID_MG300_H,  DEV_FUNC_MG300_H,   strMGtimings,  MG_Detailed_timing},
	{_T("MG300_A"),     _T("MG300_A"), 	 0,          DEV_ID_MG300_A,  DEV_FUNC_MG300_A,   strMGtimings,  MG_Detailed_timing},
	{_T("MG310_S"),     _T("MG310_S"), 	 0,          DEV_ID_MG310_S,  DEV_FUNC_MG310_S,   strMGtimings,  MG_Detailed_timing},
	{_T("MG310_H"),     _T("MG310_H"), 	 0,          DEV_ID_MG310_H,  DEV_FUNC_MG310_H,   strMGtimings,  MG_Detailed_timing},
	{_T("MG310_A"),     _T("MG310_A"), 	 0,          DEV_ID_MG310_A,  DEV_FUNC_MG310_A,   strMGtimings,  MG_Detailed_timing},
};