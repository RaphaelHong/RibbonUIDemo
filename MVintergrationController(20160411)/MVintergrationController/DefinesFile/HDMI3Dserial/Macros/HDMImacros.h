#pragma once

#include "DefinesFile/HDMI3Dserial/Timings/HDMItiming.h"

typedef enum emHDMI3DDeviceID
{
	INVALID_HDMI_ID   = 0x0,  //无效的ID
	DEV_ID_HDMI       = 0x1d01,
};

typedef enum emHDMIDFunCode
{
	INVALID_HDMI_FUNC   = 0x0, //无效的功能码
	DEV_FUNC_HDMI       = 0x28fe0301,
};

#define TOTAL_HDMI3D_TYPE 1

DEVICE_STRUCT stHDMI3DDevTypes[TOTAL_HDMI3D_TYPE] = {
	{ _T("HDMI_3D"), _T("HDMI_3D"), DEV_ID_HDMI, DEV_ID_HDMI, DEV_FUNC_HDMI, strHDMI3D_TIMING, stHDMI3D_detailed_timing},
};