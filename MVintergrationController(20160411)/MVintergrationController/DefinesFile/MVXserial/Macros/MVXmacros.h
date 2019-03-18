#pragma once

#include "DefinesFile/MVXserial/Timings/MVXtiming.h"

typedef enum emMVX303DeviceID
{
	INVALID_MVX_ID      =  0x00, //无效的ID
	DEV_ID_MVX_OUT_LMT  =  0x1982, //开窗大小与输出分辨率大小一致
	DEV_ID_MVX_OUT_ULMT =  0x19c2, //开窗大小不限制
};

typedef enum emMVX303FunCode
{
	INVALID_MVX_FUNC    =  0, //无效的功能码
	DEV_FUNC_MVX_LMT    =  0x22fe0322, //开窗大小与输出分辨率大小一致
	DEV_FUNC_MVX_ULMT   =  0x22fe0362, //开窗大小不限制
};

#define TOTAL_MVX303_TYPE 2

DEVICE_STRUCT stMVX303DevTypes[TOTAL_MVX303_TYPE] = {
	{ _T("MVX303_HDMI"), _T("MVX303_HDMI"), DEV_ID_MVX_OUT_LMT, DEV_ID_MVX_OUT_LMT, DEV_FUNC_MVX_LMT, m_MVX303_HDMI_strTimings, MVX303_HDMI_detailed_timings},
	{ _T("MVX303_HDMI"), _T("MVX303_HDMI"), DEV_ID_MVX_OUT_ULMT, DEV_ID_MVX_OUT_ULMT, DEV_FUNC_MVX_ULMT, m_MVX303_HDMI_strTimings, MVX303_HDMI_detailed_timings},
};