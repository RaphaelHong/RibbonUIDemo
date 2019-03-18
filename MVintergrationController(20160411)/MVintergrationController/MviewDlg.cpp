// MviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MviewDlg.h"


typedef BOOL (WINAPI* pEnumDisplayDevices)(PVOID,DWORD,PVOID,DWORD);
typedef BOOL (WINAPI* pEnumDisplaySettings)(PVOID,DWORD,PVOID);

#define	  MAX_DEVICES   128

#ifndef DISPLAY_DEVICE_PRIMARY_DEVICE
typedef struct _DISPLAY_DEVICE {
	DWORD cb;
	TCHAR DeviceName[32];
	TCHAR DeviceString[128];
	DWORD StateFlags;
} DISPLAY_DEVICE, *PDISPLAY_DEVICE, *LPDISPLAY_DEVICE;
#endif

CString		g_GPUModel[MAX_DEVICES];
CString		g_GPUDriverVersion[MAX_DEVICES];

CString		dev_Names[MAX_DEVICES];

bool GetDeviceIdentification(CString &vendorID, CString &deviceID,CString &regiskey)
{
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(DISPLAY_DEVICE);

	int i = 0;
	CString id;

	// locate primary display device
	while (EnumDisplayDevices(NULL, i, &dd, 0))
	{
		if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
		{
			id = (CString)dd.DeviceID;
			regiskey=dd.DeviceKey;
			break;
		}

		i++;
	}

	if (id == "") return false;

	// get vendor ID
	vendorID = id.Mid(8,4);//id.substr(8, 4);

	// get device ID
	deviceID = id.Mid(17,4);//id.substr(17, 4);

	return true;
}

#define NUM_OF_WIDES				15
int	wide_timing_modes[NUM_OF_WIDES] ={100000};

unsigned char tmp_wide_timing_nvidiaXP[92*3] ={
	// hactive,vactive,pixel_Width,Hz,pixel rate
	// hactive,vactive,htotal,hfrontporch,hsyncwidth,vtotal,vfron,vsync,hpolarity,vpolarity,unknow,unknow,Vfreq,unknow,pixel_freq,00,00
	// 6092x768x60Hz
	0x36,0xD6, 0x99,0x99, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0xff,0x00,
	0xcc,0x17, 0x00,0x03, 0x20,0x00, 0x3c,0x00, 0xe0,0x79, 0x00,0x00,
	0xcc,0x17, 0x00,0x03, 0x18,0x1a, 0x20,0x00, 0x20,0x00, 0x26,0x03, 0x03,0x00, 0x06,0x00, 0x00,0x00, 0x00,0x00, 0x01,0x00, 0x00,0x00, 0x60,0xea, 0x00,0x00, 0xe0,0x79, 0x00,0x00,
	0xcc,0x17, 0x00,0x03, 0x18,0x1a, 0x20,0x00, 0x20,0x00, 0x26,0x03, 0x03,0x00, 0x06,0x00, 0x00,0x00, 0x00,0x00, 0x01,0x00, 0x00,0x00, 0x60,0xea, 0x00,0x00, 0x00,0x00, 0x00,0x00,
	0x00,0x00,0x00,0x00,

	//4200x1050@60Hz -1
	0x36,0xD6,0x99,0x99,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
	0xcc,0x17, 0x00,0x03, 0x20,0x00, 0x3c,0x00, 0xe0,0x79, 0x00,0x00,
	0xcc,0x17, 0x00,0x03, 0x18,0x1a, 0x20,0x00, 0x20,0x00, 0x26,0x03, 0x03,0x00, 0x06,0x00, 0x00,0x00, 0x00,0x00, 0x01,0x00, 0x00,0x00, 0x60,0xea, 0x00,0x00, 0xe0,0x79, 0x00,0x00,
	0xcc,0x17, 0x00,0x03, 0x18,0x1a, 0x20,0x00, 0x20,0x00, 0x26,0x03, 0x03,0x00, 0x06,0x00, 0x00,0x00, 0x00,0x00, 0x01,0x00, 0x00,0x00, 0x60,0xea, 0x00,0x00, 0x00,0x00, 0x00,0x00,
	0x00,0x00,0x00,0x00,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00

};


/*
// vista template for timings 4098x768 as example, 0x 276bytes per entry, 0x 4 entries per timing

// line 1  six types:                  00 01 00 04 99 99 36 d6
//                                  or 01 01 00 04 99 99 36 d6

//                                  or 10 01 00 04 99 99 36 d6, 0x must
//									or 10 01 00 01 99 99 36 d6, 0x must
//									or 11 01 00 04 99 99 36 d6, 0x must
//									or 11 01 00 01 99 99 36 d6  must
//
// line 2   hact(4) vact(4), 0x 15 00 00 00, 0x pixel_depth(4), 0x 00(12bytes)
// line 3   hact(4) vact(4), 0x 00(8bytes)
// line 4   hact(4) vact(4), 0x 00(8bytes)
// line 5   hact(4) vact(4)
// line 6   hact(4) hfront(2) hsync(2) htotal(2) hpol(2) vact(4) vfront(2) vsyncw(2) vtotal(2), 0x vpol(2) unknow(4) pixel_rate(4) unknow(4) vfreq (4) vfreq*1000 (4) unknow (4)
// line 7   timing name
// line 8   hact(4) hfront(2) hsync(2) htotal(2) hpol(2) vact(4) vfront(2) vsyncw(2) vtotal(2), 0x vpol(2) unknow(4) pixel_rate(4) unknow(4) vfreq (4) vfreq*1000 (4) unknow (4)
// line 9   timing name
*/
unsigned char tmp_wide_timing_nvidiaW7_Old[272*3] ={

	//defined timing 1 
	0x11, 0x01, 0x10, 0x00, 0x99, 0x99, 0x36, 0xd6,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 

	//defined timing 2
	0x40, 0x42, 0x0f, 0x00, 0x99, 0x99, 0x36, 0xd6, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 

	//defined timing 3
	0x00, 0x00, 0x00, 0x00, 0x99, 0x99, 0x36, 0xd6, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

unsigned char tmp_wide_timing_nvidiaW7_Vista_New[280*3] ={

/*
	// vista template for timings 4098x768 as example, 0x 276bytes per entry, 0x 4 entries per timing

	// line 1  six types: 0x13, 0x01, 0x20, 0x00, 0x99, 0x99, 0x36, 0xd6,
	// line 2   hact(4) vact(4), 0x 15 00 00 00, 0x pixel_depth(4), 0x 00(12bytes)
	// line 3   hact(4) vact(4), 0x 00(8bytes)
	// line 4   hact(4) vact(4), 0x 00(8bytes)
	// line 5   hact(4) vact(4)
	// line 6   hact(4) hfront(2) hsync(2) htotal(2) hpol(2) vact(4) vfront(2) vsyncw(2) vtotal(2), 0x vpol(2) unknow(4) pixel_rate(4) unknow(4) vfreq (4) vfreq*1000 (4) unknow (4)
	// line 7   timing name
	// line 8   hact(4) hfront(2) hsync(2) htotal(2) hpol(2) vact(4) vfront(2) vsyncw(2) vtotal(2), 0x vpol(2) unknow(4) pixel_rate(4) unknow(4) vfreq (4) vfreq*1000 (4) unknow (4)
	// line 9   timing name
*/
	//defined timing 1 
	0x13, 0x01, 0x20, 0x00, 0x99, 0x99, 0x36, 0xd6,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
	0x00, 0x00, 0x00, 0x00, 

	0x13, 0x01, 0x20, 0x00, 0x99, 0x99, 0x36, 0xd6,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
	0x00, 0x00, 0x00, 0x00, 

	0x13, 0x01, 0x20, 0x00, 0x99, 0x99, 0x36, 0xd6,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x02, 0x10, 0x00, 0x00, 0x80, 0x00, 0xf8, 0x00, 0x06, 0x15, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x0c, 0x00, 0x26, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa9, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x60, 0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x43, 0x55, 0x53, 0x54, 0x3a, 0x34, 0x30, 0x39, 0x38, 0x78, 0x37, 0x36, 0x38, 0x78, 0x36, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x48, 0x7a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
	0x00, 0x00, 0x00, 0x00
};

const unsigned char EPROM_TEMP[256] = {

	//MVD20X-DP
	0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x36,0xD6,0x99,0x99,0x89,0x17,0x45,0x13,
	0x12,0x13,0x01,0x04,0xA5,0x00,0x00,0x78,0x22,0xEE,0x95,0xA3,0x54,0x4C,0x99,0x26,
	0x0F,0x54,0x54,0xBF,0xEF,0x80,0x81,0x80,0x81,0x40,0x71,0x4F,0x81,0x00,0x95,0x00,
	0x95,0x0F,0x81,0x0F,0xB3,0x00,0x2C,0x4C,0x00,0xC0,0xC3,0x00,0x26,0x30,0x48,0x98,
	0x36,0x10,0x00,0x00,0xC3,0x00,0x00,0x18,0x90,0x65,0x00,0x40,0x41,0x00,0x98,0xC0,
	0x18,0x88,0xC8,0x01,0x00,0x00,0x4C,0x00,0x00,0x18,0x00,0x00,0x00,0xFC,0x00,0x4D,
	0x56,0x44,0x32,0x30,0x58,0x00,0x48,0x31,0x31,0x0A,0x20,0x00,0x90,0x65,0x00,0x80,
	0x82,0x00,0x4C,0x60,0x30,0x10,0x6C,0x10,0x00,0x00,0x86,0x00,0x00,0x18,0x00,0x23,

	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

int CMviewDlg::sMvObjCount = 0; //统计CMviewDlg类对象的个数
/////////////////////////////////////////////////////////////////////////////////////////
// CMviewDlg 对话框

IMPLEMENT_DYNAMIC(CMviewDlg, CXTPResizeDialog)

CMviewDlg::CMviewDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMviewDlg::IDD, pParent)
	, NVidia_Presented(false)
	, ATI_Presented(false)
{
	m_nObjOder = (++sMvObjCount);
}

CMviewDlg::~CMviewDlg()
{
	sMvObjCount--;
}

void CMviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_DEF_LAYOUT, m_cmbDefLayout);
	DDX_Control(pDX, IDC_BTN_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTN_SET_MV_TIMING, m_btnSetTiming);
	DDX_Control(pDX, IDC_CBOX_DEF_TIMING, m_cmbDefTiming);
	DDX_Control(pDX, IDC_CBOX_SEC_LAYOUT, m_cmbSecLayout);
	DDX_Control(pDX, IDC_CBOX_SEC_TIMING, m_cmbSecTiming);
	DDX_Control(pDX, IDC_CBOX_THD_LAYOUT, m_cmbThdLayout);
	DDX_Control(pDX, IDC_CBOX_THD_TIMING, m_cmbThdTiming);
	DDX_Control(pDX, IDC_CBOX_ADAPTER, m_cmbAdapter);
}


BEGIN_MESSAGE_MAP(CMviewDlg, CXTPResizeDialog)
	ON_BN_CLICKED(IDC_BTN_SET_MV_TIMING, &CMviewDlg::OnBnClickedSetMvTiming)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CMviewDlg::OnBnClickedBtnRefresh)
	ON_CBN_SELCHANGE(IDC_CBOX_DEF_LAYOUT, &CMviewDlg::OnCbnSelchangeCboxDefLayout)
	ON_CBN_SELCHANGE(IDC_CBOX_SEC_LAYOUT, &CMviewDlg::OnCbnSelchangeCboxSecLayout)
	ON_CBN_SELCHANGE(IDC_CBOX_THD_LAYOUT, &CMviewDlg::OnCbnSelchangeCboxThdLayout)
END_MESSAGE_MAP()


// CMviewDlg 消息处理程序

BOOL CMviewDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	//自动调整控件尺寸和位置
	ResizeIt();

	//特殊风格控件
	SetControlTheme(xtpControlThemeResource);

	//获取操作系统版本号
	GetOSVersion(m_OSMajor, m_OSMinor);

	//获取显卡类型
	GetGPUModel();

	//创建背景画刷
	m_bkBrush.CreateSolidBrush(constClrDlgBk[m_nObjOder%BKCOLORS]);

	//英语支持
#ifdef ENGLISHVER
	EnglishSupport();
#endif

	return TRUE; 
}

void CMviewDlg::ResizeIt()
{
	SetResize(IDC_BTN_REFRESH, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_CBOX_ADAPTER, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR1_MV, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR2_MV, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR3_MV, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR4_MV, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_TEXT_COMB_MODE, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPLEFT);
	SetResize(IDC_TEXT_TIMING_MV, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_CBOX_DEF_LAYOUT, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_CBOX_DEF_TIMING, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_CBOX_SEC_LAYOUT, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_CBOX_SEC_TIMING, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_CBOX_THD_LAYOUT, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_CBOX_THD_TIMING, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BTN_SET_MV_TIMING, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
}

void CMviewDlg::SetControlTheme(XTPControlTheme theme)
{

	m_cmbDefLayout.SetTheme(theme);
	m_cmbDefTiming.SetTheme(theme);

	m_cmbSecLayout.SetTheme(theme);
	m_cmbSecTiming.SetTheme(theme);

	m_cmbThdLayout.SetTheme(theme);
	m_cmbThdTiming.SetTheme(theme);

	m_btnRefresh.SetTheme(theme);
	m_btnSetTiming.SetTheme(theme);
	m_cmbAdapter.SetTheme(theme);
}

HBRUSH CMviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(80,80,20));

		return m_bkBrush;
	}

	return hbr;
}

void CMviewDlg::SetBoardInfo(CBaseBoard boardinfo)
{
	m_boardInfo = boardinfo;
}


void CMviewDlg::Initialize()
{
	if (theApp.m_nConnType == NON_CONNECT) return (void)AfxMessageBox(_T("Please connect first！"));

	//添加设备到设备选择框
	CString strDev;
	strDev.Format(_T("%d . %s%s%s"), 1, m_boardInfo.GetBoardName(), _T(" (MV9999)"), g_GPUModel[0]);
	m_cmbAdapter.ResetContent();
	m_cmbAdapter.AddString(strDev);
	m_cmbAdapter.SetCurSel(0);

	//获取多屏保的分辨率
	int i;
	for (i=0; i<3; i++)
	{
		theApp.GetInputTiming(&m_CurTiming[i], 3, i, m_boardInfo.GetBoardPort());
	}

	//获取分辨率名称
	if ((m_pStrTiming = m_boardInfo.GetTimingName(Product_MV)) == NULL)
	{
		MessageBox(_T("Unknow device！"), _T("Unrecognizable Timing"), MB_OK|MB_ICONSTOP);

		return;
	}

	//获取分辨率明细参数
	if ((m_pDetailed_timing = m_boardInfo.GetDetailTiming(Product_MV)) == NULL)
	{
		MessageBox(_T("Unknow device！"), _T("Unrecognizable Timing"), MB_OK|MB_ICONSTOP);

		return;
	}

	//清空下拉框
	ResetCombBox();

	//分屏模式
	SelectModes();
	
	//选择当前多屏保的组合模式和分辨率
	SelectTimings();
}

// 清空下拉框
void CMviewDlg::ResetCombBox()
{
	m_cmbDefLayout.ResetContent();
	m_cmbDefTiming.ResetContent();
	m_cmbSecLayout.ResetContent();
	m_cmbSecTiming.ResetContent();
	m_cmbThdLayout.ResetContent();
	m_cmbThdTiming.ResetContent();
}

// 自适应模式
void CMviewDlg::SelectModes()
{
	int nMaxScreen;  //最大分屏数
	int nCombinationSupport; //支持的分屏模式：1：仅支持水平模式 2：支持所有模式
	nMaxScreen = (m_boardInfo.GetBoardFuncID() & 0x0000ff00) >> 8;
	nCombinationSupport = (m_boardInfo.GetBoardFuncID() & 0x000000f0) >> 4;
	int i=0;
	CString strHoriz;
#ifdef ENGLISHVER
	strHoriz = _T("Horizontal");
#else
	strHoriz = _T("水平");
#endif
	while(stSplitMode[i].strmode != _T("END"))
	{
		if (stSplitMode[i].nScreenNum > nMaxScreen)
		{
			i++;
			continue;
		}

		if ( (nCombinationSupport == 1) && (stSplitMode[i].strmode.Find(strHoriz) == -1) )
		{
			i++;
			continue;
		}

		m_cmbDefLayout.AddString(stSplitMode[i].strmode);
		m_cmbSecLayout.AddString(stSplitMode[i].strmode);
		m_cmbThdLayout.AddString(stSplitMode[i].strmode);
		i++;
	}
}

// 自适应分辨率
void CMviewDlg::SelectTimings()
{
	CString Str_timing_sel1, Str_timing_sel2, Str_timing_sel3;

	int i =0;
	while(m_pStrTiming[i]!= _T("END"))
	{
		// Default timing
		if((m_pDetailed_timing[i].m_PixelRate == m_CurTiming[0].m_PixelRate)
			&&(m_pDetailed_timing[i].m_HActive == m_CurTiming[0].m_HActive)
			&&(m_pDetailed_timing[i].m_VActive == m_CurTiming[0].m_VActive)
			&&(m_pDetailed_timing[i].m_HBlank == m_CurTiming[0].m_HBlank)
			&&(m_pDetailed_timing[i].m_VBlank==m_CurTiming[0].m_VBlank))
		{
			Str_timing_sel1 = m_pStrTiming[i];
		}

		// Secondary timing
		if((m_pDetailed_timing[i].m_PixelRate == m_CurTiming[1].m_PixelRate)
			&&(m_pDetailed_timing[i].m_HActive == m_CurTiming[1].m_HActive)
			&&(m_pDetailed_timing[i].m_VActive == m_CurTiming[1].m_VActive)
			&&(m_pDetailed_timing[i].m_HBlank == m_CurTiming[1].m_HBlank)
			&&(m_pDetailed_timing[i].m_VBlank==m_CurTiming[1].m_VBlank))
		{
			Str_timing_sel2 = m_pStrTiming[i];
		}

		// Third timing
		if((m_pDetailed_timing[i].m_PixelRate == m_CurTiming[2].m_PixelRate)
			&&(m_pDetailed_timing[i].m_HActive == m_CurTiming[2].m_HActive)
			&&(m_pDetailed_timing[i].m_VActive == m_CurTiming[2].m_VActive)
			&&(m_pDetailed_timing[i].m_HBlank == m_CurTiming[2].m_HBlank)
			&&(m_pDetailed_timing[i].m_VBlank==m_CurTiming[2].m_VBlank))
		{
			Str_timing_sel3 = m_pStrTiming[i];
		}
		i++;
	}

	/***************************  select mode & timing in the mode ***************************************/
	i =0;
	while(stSplitMode[i].strmode!= _T("END"))
	{
		if(Str_timing_sel1.Find(stSplitMode[i].strmode)!=-1)
		{
			m_cmbDefLayout.SelectString(0,stSplitMode[i].strmode);
			AddTimings(&m_cmbDefTiming, stSplitMode[i].strmode);
		}	

		if(Str_timing_sel2.Find(stSplitMode[i].strmode)!=-1)
		{
			m_cmbSecLayout.SelectString(0,stSplitMode[i].strmode);
			AddTimings(&m_cmbSecTiming, stSplitMode[i].strmode);
		}

		if(Str_timing_sel3.Find(stSplitMode[i].strmode)!=-1)
		{
			m_cmbThdLayout.SelectString(0,stSplitMode[i].strmode);
			AddTimings(&m_cmbThdTiming, stSplitMode[i].strmode);
		}
		i++;
	}


	/*select timing in list*/
	m_cmbDefTiming.SelectString(0,Str_timing_sel1);
	m_cmbSecTiming.SelectString(0,Str_timing_sel2);
	m_cmbThdTiming.SelectString(0,Str_timing_sel3);
}

void CMviewDlg::OnBnClickedBtnRefresh()
{
	Initialize();
}

// 向分辨率下拉框中添加分辨率
void CMviewDlg::AddTimings(CXTPComboBox *pCbox,CString strMode)
{
	int i=0;
	while(m_pStrTiming[i] != _T("END"))
	{
		if (m_pStrTiming[i].Find(strMode) != -1)
		{
			pCbox->AddString(m_pStrTiming[i]);
		}

		i++;
	}
}


void CMviewDlg::OnBnClickedSetMvTiming()
{
	if (theApp.m_nConnType == NON_CONNECT) return (void)AfxMessageBox(_T("Please connect first！"));

	//先判断设备是否有变化
	if (!AssertDevChanged()) return (void)MessageBox(_T("Wrong device ID, please refresh the device list！"), _T("Wrong ID"), MB_OK|MB_ICONSTOP);
	
	CWaitCursor wait;

	//获取当前所选的分辨率
	CString strTmingName[3];  //分辨率名称
	int nTimingIndex[3];  //当前选择的分辨率在数组中的索引号
	TIMING_STRUCT ts[3];
	GetCurrentSelectedTiming(strTmingName, nTimingIndex, ts, 3);

	//判断是否有宽屏分辨率
	if (IsWideTiming(ts, 3))
	{
		SetWideScreenTimings(ts);
	}

	for (int i=0; i< 3; i++)
	{
		int nRet = theApp.SetInputTiming(&ts[i], 3, i, m_boardInfo.GetBoardPort());
		if(nRet == -1)
		{
			CString str;
#ifdef ENGLISHVER
			str.Format(_T("Write resolution [%d] failed!"), i+1);
#else
			str.Format(_T("写入分辨率 [%d] 失败!"), i+1);
#endif
			AfxMessageBox(str);
			return;
		}
	}



	/*************************************** 提示 重启计算机 *****************************************/		
#ifdef ENGLISHVER
	AfxMessageBox(_T("Update Finished, Reinsert the input signal cable to take effect!"), MB_OKCANCEL );
#else
	AfxMessageBox(_T("更新完成, 重新插拔输入信号线后生效, 如果你设置的是宽屏分辨率(水平或者垂直像素大于4096),请重新重启计算机！"), MB_OKCANCEL );
#endif
}

BOOL CMviewDlg::AssertDevChanged() const
{
	int nFuncID;
	nFuncID = theApp.GetBoardFuncCode(m_boardInfo.GetBoardPort());

	if (nFuncID == -1 || nFuncID != m_boardInfo.GetBoardFuncID())	return FALSE;

	return TRUE;
}

// 获取当前选择的TIMING
void CMviewDlg::GetCurrentSelectedTiming(CString *strName, int *nIndex, TIMING_STRUCT *pTiming, int iNum) const
{
	m_cmbDefTiming.GetWindowText(strName[0]);
	m_cmbSecTiming.GetWindowText(strName[1]);
	m_cmbThdTiming.GetWindowText(strName[2]);

	int i=0;

	for (;i<iNum; i++) nIndex[i] = -1;

	i=0;
	while(m_pStrTiming[i] != _T("END"))
	{
		for (int j=0; j<3; j++)
		{
			if (strName[j] == m_pStrTiming[i])
			{
				nIndex[j] = i;
				pTiming[j] = m_pDetailed_timing[i];
			}
		}

		i++;
	}
}

BOOL CMviewDlg::IsWideTiming(TIMING_STRUCT *pTiming, int iNum) const
{
	BOOL bExistWideTiming = FALSE;

	for (int i=0; i<iNum; i++)
	{
		if (pTiming[i].m_HActive >= 4096 || pTiming[i].m_VActive >= 4096)
		{
			bExistWideTiming = TRUE;
		}
	}

	return bExistWideTiming;
}

void CMviewDlg::SetWideScreenTimings(pTIMING_STRUCT pTiming)
{
	int i;
	int tmp_Hz,wide_timings,wide_timing_seled[NUM_OF_WIDES] = {0}; // for 18~28
	unsigned char *WideTimingRegVal;
	TIMING_STRUCT *tmp_ptiming;

	UCHAR ucSetEEPROMWrtie[3]  =  {0xA0,0,0};

	wide_timings =0;
	for(i=0;i<3;i++)
	{
		wide_timing_seled[i] = 0;
		if((pTiming[i].m_HActive >= 4096)||(pTiming[i].m_VActive >= 4096))
		{
			wide_timing_seled[i] = 1;
			wide_timings++;
		}
	}

	int wide_dst_addr = 0;

	if((wide_timings!=0))  //主要问题是 写宽屏这部分 
	{
		if(NVidia_Presented)
		{
			if(m_OSMajor==5) //WindoesXP 
			{
				WideTimingRegVal = new unsigned char [wide_timings*92+92*4];
				memset(WideTimingRegVal,0,wide_timings*92+92*4);

				for(int i=0;i<3;i++)
				{
					if(wide_timing_seled[i]!=0)
					{
						tmp_ptiming = &pTiming[i];
						tmp_Hz = (int)(tmp_ptiming->m_PixelRate*(double)10000/(tmp_ptiming->m_HActive+tmp_ptiming->m_HBlank)/(tmp_ptiming->m_VActive+tmp_ptiming->m_VBlank) + 0.5); 

						GenWideTimingNVidiaXP(tmp_ptiming->m_HActive,tmp_ptiming->m_VActive,
							tmp_ptiming->m_HActive+tmp_ptiming->m_HBlank,tmp_ptiming->m_HFPorch,tmp_ptiming->m_HSyncWid,
							tmp_ptiming->m_VActive+tmp_ptiming->m_VBlank,tmp_ptiming->m_VFPorch,tmp_ptiming->m_VSyncWid,
							tmp_ptiming->m_HPol? 0:1,tmp_ptiming->m_VPol? 0:1,32, tmp_Hz,tmp_ptiming->m_PixelRate);

						memcpy(&WideTimingRegVal[wide_dst_addr],&tmp_wide_timing_nvidiaXP[0],92);
						wide_dst_addr+=92;
					}
				}
				SetWideScreenTimingsXP(WideTimingRegVal,wide_timings*92+92*4);
				delete [] WideTimingRegVal;
			}
			else if(m_OSMajor >= 6) // WindowsVista or Win7 W8 等 
			{							
				int dat1,dat2,dat3,dat4;
				sscanf_s(g_GPUDriverVersion[0],"%d.%d.%d.%d",&dat1,&dat2,&dat3,&dat4);
				CString driverversion;
				driverversion.Format("%2d%2d%2d%4d",dat1,dat2,dat3,dat4);

				//DriverVer=9.18.13.4043
				if(driverversion >= " 918134043")//W7——New
				{
					//if(g_GPUDriverVersion[0] )
					WideTimingRegVal = new unsigned char [280*32];
					memset(WideTimingRegVal,0,280*32);

					for(int i=0;i<3;i++)
					{
						if(wide_timing_seled[i]!=0)
						{
							tmp_ptiming = &pTiming[i];
							tmp_Hz = (int)(tmp_ptiming->m_PixelRate*(double)10000/(tmp_ptiming->m_HActive+tmp_ptiming->m_HBlank)/(tmp_ptiming->m_VActive+tmp_ptiming->m_VBlank) + 0.5); 

							GenWideTimingNVidiaW7_New(tmp_ptiming->m_HActive,tmp_ptiming->m_VActive,
								tmp_ptiming->m_HActive+tmp_ptiming->m_HBlank,tmp_ptiming->m_HFPorch,tmp_ptiming->m_HSyncWid,
								tmp_ptiming->m_VActive+tmp_ptiming->m_VBlank,tmp_ptiming->m_VFPorch,tmp_ptiming->m_VSyncWid,
								tmp_ptiming->m_HPol?0:1,tmp_ptiming->m_VPol?0:1,32, tmp_Hz,tmp_ptiming->m_PixelRate);


							memcpy(&WideTimingRegVal[wide_dst_addr],&tmp_wide_timing_nvidiaW7_Vista_New[0],280*3);
							wide_dst_addr+=280*3;
						}
					}

					SetWideScreenTimingsW7_New(WideTimingRegVal,280*33);
					delete [] WideTimingRegVal;
				}
				else  //old
				{
					WideTimingRegVal = new unsigned char [272*3*11];
					memset(WideTimingRegVal,0,272*3*11);

					for(int i=0;i<3;i++)
					{
						if(wide_timing_seled[i]!=0)
						{
							tmp_ptiming = &pTiming[i];
							tmp_Hz = (int)(tmp_ptiming->m_PixelRate*(double)10000/(tmp_ptiming->m_HActive+tmp_ptiming->m_HBlank)/(tmp_ptiming->m_VActive+tmp_ptiming->m_VBlank) + 0.5); 

							GenWideTimingNVidiaW7_Old(tmp_ptiming->m_HActive,tmp_ptiming->m_VActive,
								tmp_ptiming->m_HActive+tmp_ptiming->m_HBlank,tmp_ptiming->m_HFPorch,tmp_ptiming->m_HSyncWid,
								tmp_ptiming->m_VActive+tmp_ptiming->m_VBlank,tmp_ptiming->m_VFPorch,tmp_ptiming->m_VSyncWid,
								tmp_ptiming->m_HPol?0:1,tmp_ptiming->m_VPol?0:1,32, tmp_Hz,tmp_ptiming->m_PixelRate);


							memcpy(&WideTimingRegVal[wide_dst_addr],&tmp_wide_timing_nvidiaW7_Old[0],272*3);
							wide_dst_addr+=272*3;
						}
					}
					SetWideScreenTimingsW7_Old(WideTimingRegVal,272*3*11);
					delete [] WideTimingRegVal;
				}
			}
			if(ATI_Presented)
			{
			}
		}
	}
}

/************************************************************
*	                 WideTiming 相关操作
*********************************************************** */

void  CMviewDlg::GenWideTimingNVidiaXP(int h_act, int v_act, int h_total, int h_front, int h_synt, int v_total, int v_front, int vsynt, int hpol, int vpol, int pix_wid, int vHz, int pic_rate)
{
	int i,j;
	unsigned char *pData = &tmp_wide_timing_nvidiaXP[12];
	for(j=0;j<2;j++)
	{
		pData = put_data(h_act,pData,2);
		pData = put_data(v_act,pData,2);
		pData = put_data(pix_wid,pData,2);
		pData = put_data(vHz,pData,2);
		pData = put_data(pic_rate,pData,4);
		for(i=0;i<2;i++)
		{
			pData = put_data(h_act,pData,2);
			pData = put_data(v_act,pData,2);
			pData = put_data(h_total,pData,2);
			pData = put_data(h_front,pData,2);
			pData = put_data(h_synt,pData,2);
			pData = put_data(v_total,pData,2);
			pData = put_data(v_front,pData,2);
			pData = put_data(vsynt,pData,2);
			pData = put_data(0,pData,4);
			pData = put_data(hpol,pData,1);
			pData = put_data(vpol,pData,1);
			pData = put_data(0,pData,2);
			pData = put_data(vHz*1000,pData,4);
			if(i==0)
				pData = put_data(pic_rate,pData,4);
			else
				pData = put_data(0x2001,pData,4);
		}
		pData = put_data(0x0000,pData,4);
		pData+=12;
	}
}

void  CMviewDlg::GenWideTimingNVidiaW7_Old(int h_act, int v_act, int h_total, int h_front, int h_synt, int v_total, int v_front, int vsynt, int hpol, int vpol, int pix_wid, int vHz, int pic_rate)
{
	int i,j;
	unsigned char *pData;

	for(j=0;j<3;j++)
	{	
		pData = &tmp_wide_timing_nvidiaW7_Old[272*j + 8];

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);
		pData += 4;
		pData = put_data(pix_wid,pData,4);
		pData += 12;

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);
		pData += 8;

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);
		pData += 8;

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);

		for(i=0;i<2;i++)
		{
			pData = put_data(h_act,pData,4);
			pData = put_data(h_front,pData,2);	
			pData = put_data(h_synt,pData,2);		
			pData = put_data(h_total,pData,2);
			pData = put_data(hpol,pData,2);

			pData = put_data(v_act,pData,4);
			pData = put_data(v_front,pData,2);
			pData = put_data(vsynt,pData,2);
			pData = put_data(v_total,pData,2);
			pData = put_data(vpol,pData,2);

			pData += 4;
			pData = put_data(pic_rate,pData,4);
			pData += 4;

			pData = put_data(vHz,pData,4);
			pData = put_data(vHz*1000,pData,4);
			pData += 4;

			//timing name
			pData = get_timingname(h_act,v_act,vHz,pData,48);
		}
	}
}

void  CMviewDlg::GenWideTimingNVidiaW7_New(int h_act, int v_act, int h_total, int h_front, int h_synt, int v_total, int v_front, int vsynt, int hpol, int vpol, int pix_wid, int vHz, int pic_rate)
{
	int i,j;
	unsigned char *pData;

	for (j=0; j<3; j++)
	{
		pData = &tmp_wide_timing_nvidiaW7_Vista_New[280*j+8];

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);
		pData += 4;
		pData = put_data(pix_wid,pData,4);
		pData += 12;

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);
		pData += 8;

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);
		pData += 8;

		pData = put_data(h_act,pData,4);
		pData = put_data(v_act,pData,4);

		for(i=0;i<2;i++)
		{
			pData = put_data(h_act,pData,4);
			pData = put_data(h_front,pData,2);	
			pData = put_data(h_synt,pData,2);		
			pData = put_data(h_total,pData,2);
			pData = put_data(hpol,pData,2);

			pData = put_data(v_act,pData,4);
			pData = put_data(v_front,pData,2);
			pData = put_data(vsynt,pData,2);
			pData = put_data(v_total,pData,2);
			pData = put_data(vpol,pData,2);

			pData += 4;
			pData = put_data(pic_rate,pData,4);
			pData += 4;

			pData = put_data(vHz,pData,4);
			pData = put_data(vHz*1000,pData,4);
			pData += 4;

			//timing name
			pData = get_timingname(h_act,v_act,vHz,pData,48+4);
		}
	}
}

unsigned char* CMviewDlg::put_data(int val, unsigned char* p, int len)
{
	int i;
	unsigned char c;
	for(i=0;i<len;i++)
	{
		c = (unsigned char)(val&0xff);
		p[i] = c;
		val>>=8;
	}
	return (p+len);
}

unsigned char* CMviewDlg::get_timingname(int h_act, int v_act,int vHz, unsigned char* pData, int len)
{		
	unsigned char tmp_data[10];
	int tmp_data_num;
	int i;
	unsigned char *return_pData;
	return_pData = pData;
	pData += 13;

	//hactive 
	tmp_data_num = 0;
	do
	{
		tmp_data[tmp_data_num++] = h_act%10;
		h_act = h_act/10;
	}while(h_act != 0);
	for(i=(tmp_data_num-1);i>=0;i--)
	{
		*pData = tmp_data[i] + '0';
		pData++;
	}
	*pData = 'x';
	pData++;

	//vactive 
	tmp_data_num = 0;
	do
	{
		tmp_data[tmp_data_num++] = v_act%10;
		v_act = v_act/10;
	}while(v_act != 0);
	for(i=(tmp_data_num-1);i>=0;i--)
	{
		*pData = tmp_data[i] + '0';
		pData++;
	}
	*pData = 'x';
	pData++;

	//Hz 
	tmp_data_num = 0;
	do
	{
		tmp_data[tmp_data_num++] = vHz%10;
		vHz = vHz/10;
	}while(vHz != 0);
	for(i=(tmp_data_num-1);i>=0;i--)
	{
		*pData = tmp_data[i] + '0';
		pData++;
	}
	*pData = '.';
	pData++;
	*pData = '0';
	pData++;
	*pData = '0';
	pData++;
	*pData = '0';
	pData++;
	*pData = 'H';
	pData++;
	*pData = 'z';
	pData++;

	return (return_pData+len);
}

void CMviewDlg::GetGPUModel(void)
{
	int iCount;
	CString strTmp;
	int num_Devices = 0;
	//	ATI_Presented = NVidia_Presented = false;
	pEnumDisplayDevices pStartEnumDisplayDevices;
	pEnumDisplaySettings pStartEnumDisplaySettings;

	pStartEnumDisplayDevices = (pEnumDisplayDevices)GetProcAddress(LoadLibrary("USER32"), "EnumDisplayDevicesA");
	pStartEnumDisplaySettings = (pEnumDisplaySettings)GetProcAddress(LoadLibrary("USER32"), "EnumDisplaySettingsA");

	if (pStartEnumDisplayDevices && pStartEnumDisplaySettings)
	{
		DISPLAY_DEVICE dd;
		DEVMODE dv;

		ZeroMemory(&dv, sizeof(dv));
		ZeroMemory(&dd, sizeof(dd));

		dv.dmSize = sizeof(dv);
		dd.cb = sizeof(dd);

		for (iCount=0; (*pStartEnumDisplayDevices)(NULL, iCount, &dd, 0); iCount++)
		{
			dev_Names[num_Devices].Format(_T(" on %s "),dd.DeviceString);
			strTmp = dev_Names[num_Devices];
			strTmp.MakeUpper();

			if(strTmp.Find(_T("NVIDIA"),0)!=-1)
			{
				g_GPUModel[num_Devices] = dev_Names[num_Devices];
				NVidia_Presented = true;

				// ****************************** 获取显卡驱动版本号  g_GPUDriverVersion[]*************************************				
				g_GPUDriverVersion[num_Devices] = "99.99.99.9999";  //默认最大version, 宽屏模式使用W7_ new mode 
				DWORD Type = 0;
				HKEY khandle;
				unsigned char NV_TargetDataBuf [1024] = {0};
				unsigned long  NV_TargetDataBuf_Num;

				CString subKey = (CString)(dd.DeviceKey);//_T("System\\CurrentControlSet\\Control\\Video\\{75EC5FAB-83D9-4D59-B123-F8B043CCB46B}\\0000");//{0CD19CB9-58C3-45A8-92CD-3894A0F6BA29}\\0000");

				subKey.TrimLeft("\\Registry\\Machine\\");
				long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey,0,KEY_ALL_ACCESS|KEY_WOW64_64KEY,(PHKEY)(&khandle));
				if (ERROR_SUCCESS==ret)
				{
					Type = REG_BINARY;
					ret = RegQueryValueEx(khandle, TEXT("DriverVersion"), 0, &Type, (PBYTE)&NV_TargetDataBuf, (LPDWORD)&NV_TargetDataBuf_Num);
					if (ERROR_SUCCESS == ret)
					{
						g_GPUDriverVersion[num_Devices] = NV_TargetDataBuf;
					}
				}

				num_Devices++;
			}
			else if(strTmp.Find(_T("ATI"),0)!=-1)
			{
				g_GPUModel[num_Devices] = dev_Names[num_Devices];
				ATI_Presented = true;
				num_Devices++;
			}
			else if(strTmp.Find(_T("AMD"),0)!=-1)
			{
				g_GPUModel[num_Devices] = dev_Names[num_Devices];
				ATI_Presented = true;
				num_Devices++;
			}
			else if(strTmp.Find(_T("INTEL"),0)!=-1)
			{
				g_GPUModel[num_Devices] = dev_Names[num_Devices];
				//ATI_Presented = true;
				num_Devices++;
			}
		}

		if(num_Devices==0)
		{
			g_GPUModel[num_Devices] = _T(" on Unkown Graphics");
		}
	}	
}

bool CMviewDlg::GetOSVersion(int &OSVerMajor,int &OSVerMinor)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	OSVerMajor = osvi.dwMajorVersion;
	OSVerMinor = osvi.dwMinorVersion;
	return TRUE;
}



void CMviewDlg::SetWideScreenTimingsXP(unsigned char *pData,int len)
{
	HKEY KeyID = 0;
	DWORD Result = 0;
	DWORD BufSize = 0;
	DWORD Type = 0;
	int   PruningMode = 0;
	unsigned char MonitorTimingBytes[4] = {0x01,0x00,0x00,0x00};
	unsigned char *custom_data = new unsigned char [len];
	int i,nGPUID;
	CString subKeyTmp;
	long cerr;
	CString regKey,vendorID,deviceID,strTmp;
	CString subKey = _T("SYSTEM\\CurrentControlSet\\Control\\Video\\");//{0CD19CB9-58C3-45A8-92CD-3894A0F6BA29}\\0000");

	if(NVidia_Presented)	
	{
		GetDeviceIdentification(vendorID,deviceID,regKey);
		nGPUID = regKey.GetLength() - regKey.Find('{',0);

		strTmp=regKey.Right(nGPUID);
		subKey+=strTmp;


		subKeyTmp = subKey+_T("\\MonitorTiming");
		cerr = RegDeleteKey(HKEY_LOCAL_MACHINE,subKeyTmp);

		subKeyTmp = subKey+_T("\\pruningmode");
		cerr = RegDeleteKey(HKEY_LOCAL_MACHINE,subKeyTmp);

		subKeyTmp = subKey+_T("\\CUST_MODE");
		cerr = RegDeleteKey(HKEY_LOCAL_MACHINE,subKeyTmp);

		for(i=0;i<len;i++)
			custom_data[i] = pData[i];

		if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,subKey,
			0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &KeyID, &Result) == ERROR_SUCCESS)
		{
			RegSetValueEx(KeyID, "MonitorTiming", 0, REG_BINARY, (BYTE*)&MonitorTimingBytes[0], 4);
			RegSetValueEx(KeyID, "pruningmode", 0, REG_DWORD, (BYTE*)&PruningMode, 4);
			RegSetValueEx(KeyID, "CUST_MODE", 0, REG_BINARY, custom_data,len);
			RegCloseKey(KeyID);
		}
		delete [] custom_data;
	}
	else if(ATI_Presented)
	{
		return;
	}
}


void CMviewDlg::SetWideScreenTimingsW7_New(unsigned char *pData,int len)
{
	int i,j;
	HKEY KeyID = 0;
	DWORD Result = 0;
	DWORD BufSize = 0;
	DWORD Type = 0;
	HKEY khandle;
	int   PruningMode = 0;
	unsigned char NV_TargetDataBuf [1024] = {0};
	unsigned long  NV_TargetDataBuf_Num;
	unsigned char MonitorTimingBytes[4] = {0x01,0x00,0x00,0x00};
	unsigned char *custom_data = new unsigned char [280*32];//[len+5440];//4896]; // Total 8704
	int nGPUID;
	CString subKeyTmp;
	CString regKey,vendorID,deviceID,strTmp;
	CString subKey = _T("SYSTEM\\CurrentControlSet\\Control\\Video\\");//{0CD19CB9-58C3-45A8-92CD-3894A0F6BA29}\\0000");

	for(i=0;i<(280*32);i++)
		custom_data[i] = 0;

	GetDeviceIdentification(vendorID,deviceID,regKey);
	nGPUID = regKey.GetLength() - regKey.Find('{',0);

	strTmp=regKey.Right(nGPUID);
	subKey+=strTmp;


	NV_TargetDataBuf_Num = 256;
	long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey,0,KEY_ALL_ACCESS|KEY_WOW64_64KEY,(PHKEY)(&khandle));
	if (ERROR_SUCCESS!=ret)
	{
		AfxMessageBox("RegQueryValueEx Error!",MB_OK);
	}
	Type = REG_BINARY;


	ret = RegQueryValueEx(khandle, TEXT("NV_TargetData"), 0, &Type, (PBYTE)&NV_TargetDataBuf, (LPDWORD)&NV_TargetDataBuf_Num);
	if (ERROR_SUCCESS!=ret)
	{
		AfxMessageBox("RegQueryValueEx Error!",MB_OK);
	}

	for(i=0;i<11;i++)
	{
		if(pData[i*280*3] != 0x00)
		{
			for(j=16;j<20;j++)
			{
				pData[i*280*3+j-16]       = NV_TargetDataBuf[j];
			}
			for(j=36;j<40;j++)
			{
				pData[i*280*3+280+j-36]   = NV_TargetDataBuf[j];
			}
			for(j=52;j<56;j++)
			{
				pData[i*280*3+280*2+j-52] = NV_TargetDataBuf[j];
			}
		}
	}

	for(i=0;i<(len-280);i++)
		custom_data[i] = pData[i];

	if(ret == ERROR_SUCCESS)
	{
		RegSetValueEx(khandle, "MonitorTiming", 0, REG_BINARY, (BYTE*)&MonitorTimingBytes[0], 4);
		RegSetValueEx(khandle, "pruningmode", 0, REG_DWORD, (BYTE*)&PruningMode, 4);
		RegSetValueEx(khandle, "CustomDisplay", 0, REG_BINARY, custom_data,(280*3*11-280));
		RegCloseKey(KeyID);
	}
	else
		AfxMessageBox("Error open reg");

	delete [] custom_data;
}

void CMviewDlg::SetWideScreenTimingsW7_Old(unsigned char *pData,int len)
{
	int i,j;
	HKEY KeyID = 0;
	DWORD Result = 0;
	DWORD BufSize = 0;
	DWORD Type = 0;
	HKEY khandle;
	int   PruningMode = 0;
	unsigned char NV_TargetDataBuf [1024] = {0};
	unsigned long  NV_TargetDataBuf_Num;
	unsigned char MonitorTimingBytes[4] = {0x01,0x00,0x00,0x00};
	unsigned char *custom_data = new unsigned char [272*3*11-272];//[len+5440];//4896]; // Total 8704
	int nGPUID;
	CString subKeyTmp;
	CString regKey,vendorID,deviceID,strTmp;
	CString subKey = _T("SYSTEM\\CurrentControlSet\\Control\\Video\\");//{0CD19CB9-58C3-45A8-92CD-3894A0F6BA29}\\0000");

	for(i=0;i<(272*3*11-272);i++)
		custom_data[i] = 0;

	GetDeviceIdentification(vendorID,deviceID,regKey);
	nGPUID = regKey.GetLength() - regKey.Find('{',0);

	strTmp=regKey.Right(nGPUID);
	subKey+=strTmp;


	NV_TargetDataBuf_Num = 256;
	long ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey,0,KEY_ALL_ACCESS|KEY_WOW64_64KEY,(PHKEY)(&khandle));
	if (ERROR_SUCCESS!=ret)
	{
		AfxMessageBox("RegQueryValueEx Error!",MB_OK);
	}
	Type = REG_BINARY;


	ret = RegQueryValueEx(khandle, TEXT("NV_TargetData"), 0, &Type, (PBYTE)&NV_TargetDataBuf, (LPDWORD)&NV_TargetDataBuf_Num);
	if (ERROR_SUCCESS!=ret)
	{
		AfxMessageBox("RegQueryValueEx Error!",MB_OK);
	}

	for(i=0;i<11;i++)
	{
		if(pData[i*272*3] != 0x00)
		{
			for(j=16;j<20;j++)
			{
				pData[i*272*3+j-16]       = NV_TargetDataBuf[j];
			}
			for(j=36;j<40;j++)
			{
				pData[i*272*3+272+j-36]   = NV_TargetDataBuf[j];
			}
			for(j=52;j<56;j++)
			{
				pData[i*272*3+272*2+j-52] = NV_TargetDataBuf[j];
			}
		}
	}

	for(i=0;i<(len-272);i++)
		custom_data[i] = pData[i];

	if(ret == ERROR_SUCCESS)
	{
		RegSetValueEx(khandle, "MonitorTiming", 0, REG_BINARY, (BYTE*)&MonitorTimingBytes[0], 4);
		RegSetValueEx(khandle, "pruningmode", 0, REG_DWORD, (BYTE*)&PruningMode, 4);
		RegSetValueEx(khandle, "CustomDisplay", 0, REG_BINARY, custom_data,(272*3*11-272));
		RegCloseKey(KeyID);
	}
	else
		AfxMessageBox("Error open reg");

	delete [] custom_data;
}

void CMviewDlg::OnCbnSelchangeCboxDefLayout()
{
	//默认分辨率的组合模式
	UpdateData();

	CString strMode;
	m_cmbDefLayout.GetWindowText(strMode);

	m_cmbDefTiming.ResetContent();
	AddTimings(&m_cmbDefTiming, strMode);
	m_cmbDefTiming.SetCurSel(0);
}

void CMviewDlg::OnCbnSelchangeCboxSecLayout()
{
	//备用分辨率1的组合模式
	UpdateData();

	CString strMode;
	m_cmbSecLayout.GetWindowText(strMode);

	m_cmbSecTiming.ResetContent();
	AddTimings(&m_cmbSecTiming, strMode);
	m_cmbSecTiming.SetCurSel(0);
}

void CMviewDlg::OnCbnSelchangeCboxThdLayout()
{
	//备用分辨率2的组合模式
	UpdateData();

	CString strMode;
	m_cmbThdLayout.GetWindowText(strMode);

	m_cmbThdTiming.ResetContent();
	AddTimings(&m_cmbThdTiming, strMode);
	m_cmbThdTiming.SetCurSel(0);
}


void CMviewDlg::OnOK()
{
	//防止Enter Esc 键退出
	//CXTPResizeDialog::OnOK();
}
void CMviewDlg::OnCancel()
{
	//防止Enter Esc 键退出
	//CXTPResizeDialog::OnCancel();
}

void CMviewDlg::EnglishSupport()
{
	GetDlgItem(IDC_TEXT_DEVLIST_MV)->SetWindowText(_T("Adpters List:"));
	GetDlgItem(IDC_TEXT_RESOLUTIONS_SETTING_MV)->SetWindowText(_T("Resolution Settings"));
	GetDlgItem(IDC_TEXT_PREFER_TIMING_MV)->SetWindowText(_T("Prefered Timing:"));
	GetDlgItem(IDC_TEXT_ALTERNATE_TIMING1_MV)->SetWindowText(_T("Alternate Timing 1:"));
	GetDlgItem(IDC_TEXT_ALTERNATE_TIMING2_MV)->SetWindowText(_T("Alternate Timing 2:"));
	GetDlgItem(IDC_TEXT_COMB_MODE)->SetWindowText(_T("Combination List"));
	GetDlgItem(IDC_TEXT_TIMING_MV)->SetWindowText(_T("Timing List"));
	GetDlgItem(IDC_BTN_REFRESH)->SetWindowText(_T("Refresh adapter"));
	GetDlgItem(IDC_BTN_SET_MV_TIMING)->SetWindowText(_T("Apply New"));
}