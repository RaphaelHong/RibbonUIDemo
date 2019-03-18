// USB_HID.cpp : implementation file
//

#include "stdafx.h"
#include "USBHIDPort.h"
#include <windows.h>
#include <Setupapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef struct _HIDD_ATTRIBUTES {
  ULONG  Size;
  USHORT  VendorID;
  USHORT  ProductID;
  USHORT  VersionNumber;
} HIDD_ATTRIBUTES, *PHIDD_ATTRIBUTES;

//
extern "C"
{
#include "setupapi.h"
void __stdcall HidD_GetHidGuid(OUT LPGUID  HidGuid);
BOOLEAN __stdcall HidD_GetAttributes(
    IN HANDLE  HidDeviceObject,
    OUT PHIDD_ATTRIBUTES  Attributes
    );
}

HANDLE m_hDevice=INVALID_HANDLE_VALUE;
GUID myHidGuid;
CString myDevicePathName;
USHORT myVid=0x0483;
USHORT myPid=0xf3f4;
USHORT myPvn=0x0200;
//volatile BOOL MyDevFound=FALSE;
HANDLE m_hReadHandle=INVALID_HANDLE_VALUE;
HANDLE m_hWriteHandle=INVALID_HANDLE_VALUE;
//HWND Display_Info::ViewHwnd=NULL;

CWinThread *m_pReadEp1Thread=NULL;
CWinThread *m_pReadEp2Thread=NULL;
CWinThread *m_pWriteEp1Thread=NULL;
CWinThread *m_pWriteEp2Thread=NULL;

CWinThread *m_pReadReportThread=NULL;
CWinThread *m_pWriteReportThread=NULL;

OVERLAPPED ReadOverlapped;
OVERLAPPED WriteOverlapped;
HANDLE m_hExitSys=NULL;
BYTE KeyStatus=0;
BYTE LedStatus=0;
DWORD sendCounter=0;
DWORD       MemberIndex,Length,i;
//用来注册设备通知的事件广播接口
//要使用该结构体，需要在stdafx.h中增加语句#define WINVER 0x0500,并且加上头文件"dbt.h"
//DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;


/////////////////////////////////////////////////////////////////////////////
// CUSB_HID

CUSBHIDPort::CUSBHIDPort(CWnd *pParent)
{
	m_dcb.DCBlength = sizeof(DCB);
	m_hComDev = 0;
	m_bOpen = FALSE;
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	m_osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	m_osWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	m_Thread = NULL;
	m_pOwner = pParent;
}

CUSBHIDPort::~CUSBHIDPort()
{
	if(m_bOpen) ClosePort();
	CloseHandle(m_osRead.hEvent);
	CloseHandle(m_osWrite.hEvent);
}


BEGIN_MESSAGE_MAP(CUSBHIDPort, CWnd)
	//{{AFX_MSG_MAP(CUSB_HID)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// 开启USB_HID 的连接   
//USHORT myVid=0x0483;
//USHORT myPid=0xf3f4;
//USHORT myPvn=0x0200;
//开启USB_HID 的连接 
//返回 TRUE -- 打开设备，并与所需设备相连接 
BOOL CUSBHIDPort::OpenPort(unsigned short myVid, unsigned short myPid, unsigned short myPvn)
{
    if(m_bOpen) //设备打开了，则关闭 
	{
		ClosePort();
	}

	CString m_strLog;
	BOOL MyDevFound = FALSE;

	HDEVINFO hDevInfoSet;
	BOOL Result;
//	SP_DEVINFO_DATA deviceInfoData;
	DWORD memberIndex;
	SP_DEVICE_INTERFACE_DATA DevInterfaceData;
	DWORD RequiredSize;
	PSP_DEVICE_INTERFACE_DETAIL_DATA  pDevInterfaceDetailData;
	HIDD_ATTRIBUTES DevAttributes;
	DevAttributes.Size=sizeof(DevAttributes);
	DevInterfaceData.cbSize=sizeof(DevInterfaceData);//before used set size
	HidD_GetHidGuid(&myHidGuid);//get HID's GUID
	//from GUID to get device information set
	hDevInfoSet=SetupDiGetClassDevs(&myHidGuid,
				NULL,
				NULL,
				DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);//interface GUID and List the being used device
	memberIndex=0;

	while (TRUE)
	{
		Result=SetupDiEnumDeviceInterfaces(hDevInfoSet,
				/*&deviceInfoData*/NULL,
				&myHidGuid,
				memberIndex,
				&DevInterfaceData);//Get memberIndex's device information
		if (!Result)//if get information failed,find complete
		{
			break;
		}
		memberIndex++;//point to the next device
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
			&DevInterfaceData,
			NULL,//set NULL,function will return needed buffer length
			NULL,
			&RequiredSize,
			NULL);
		pDevInterfaceDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA )new UCHAR[RequiredSize];

//		pDevInterfaceDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
		if (pDevInterfaceDetailData==NULL)
		{
			SetupDiDestroyDeviceInfoList(hDevInfoSet);
			return FALSE;
		}
		pDevInterfaceDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
			&DevInterfaceData,
			pDevInterfaceDetailData,
			RequiredSize,
			NULL,
			NULL);
		myDevicePathName=pDevInterfaceDetailData->DevicePath;
		delete pDevInterfaceDetailData;
		if (!Result)
		{
			continue;//if call failed,find next device
		}
		m_hDevice=CreateFile(myDevicePathName,
			NULL,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (m_hDevice==INVALID_HANDLE_VALUE)
		{
			continue;
		}
		else
		{
			Result=HidD_GetAttributes(m_hDevice,
				&DevAttributes);//get device's attribution and save in HIDD_ATTRIBUTES struct
			CloseHandle(m_hDevice);
			m_hDevice=INVALID_HANDLE_VALUE;
			if (!Result)
			{
				continue;
			}
			if ((DevAttributes.VendorID==myVid)&&(DevAttributes.VersionNumber==myPvn)&&(DevAttributes.ProductID==myPid))
			{
				MyDevFound = TRUE;

				m_hComDev=CreateFile(myDevicePathName,
					GENERIC_READ|GENERIC_WRITE,
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
					NULL);
				if (m_hReadHandle==INVALID_HANDLE_VALUE)//创建线程失败
				{
				}
				else
				{
				}
				/*
				m_hWriteHandle=CreateFile(myDevicePathName,
					GENERIC_WRITE,
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
					NULL);
				if (m_hWriteHandle==INVALID_HANDLE_VALUE)  //创建写线程失败
				{
				}
				else
				{
				}*/
				
				break;
			}
		}
	}
	SetupDiDestroyDeviceInfoList(hDevInfoSet);//destroy device set,or memory leak
	if (MyDevFound) //无有效设备 
	{
	    m_bOpen = true;
	}
	else
	{
	    m_bOpen = FALSE;
	}

	return MyDevFound;
}

BOOL CUSBHIDPort::OpenPort()
{
	return OpenPort(myVid, myPid, myPvn);
}

//关闭USB_HID的连接 
void CUSBHIDPort::ClosePort()
{	
	if(m_bOpen)
	{
		m_bOpen = FALSE;
		SetCommMask(m_hComDev , 0 ) ;
		if(m_Thread != NULL)
			TerminateThread(m_Thread,0);
		// drop DTR
		EscapeCommFunction(m_hComDev , CLRDTR) ;
		// purge any outstanding reads/writes and close device handle
		PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_hComDev);
		m_hComDev = 0;
	}	
	


}

//写入数据，最多64bytes  
BOOL CUSBHIDPort::WriteBuf(LPSTR lpByte , DWORD dwBytesToWrite)
{
	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD		dwBytesSent = 0;
	DWORD       dwErrorFlags;
	COMSTAT     ComStat;
    char USB_wbuf[255] ={0};

	if(m_bOpen == FALSE) return FALSE;
	Flush();
	
	int Max_Len = (dwBytesToWrite>64) ? 64 : dwBytesToWrite;
	
	for( int i=0;i<Max_Len;i++)
	{
		USB_wbuf[i+1] = lpByte[i];
	}
	USB_wbuf[0] = 0;
	
	fWriteStat = WriteFile(m_hComDev, USB_wbuf, 64+1,
	                       &dwBytesWritten, &m_osWrite ) ;

	if (!fWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			while(!GetOverlappedResult(m_hComDev, &m_osWrite, &dwBytesWritten, TRUE))
			{
				if(GetLastError() == ERROR_IO_INCOMPLETE)
				{
					dwBytesSent += dwBytesWritten;
					continue;
				}
				else
				{
					ClearCommError(m_hComDev, &dwErrorFlags, &ComStat) ;
					break;
				}
			}
			dwBytesSent += dwBytesWritten;
		}
		else 
		{
			ClearCommError(m_hComDev, &dwErrorFlags, &ComStat) ;
			return FALSE;
		}
	}
	return TRUE;
}
//读取数据，直接返回  最多读取64个数据 
int CUSBHIDPort::ReadBuf(LPSTR lpszBlock, int nMaxLength,int &BytesInQueue)
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength; 
    char USB_rbuf[255] ={0};

	if(m_bOpen == FALSE) return FALSE;
	// only try to read number of bytes in queue 
	ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = 2;//min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
	BytesInQueue = ComStat.cbInQue;
	if (dwLength > 0)
	{
		fReadStat = ReadFile(m_hComDev, USB_rbuf,64+1,
		                     &dwLength, &m_osRead) ;
		if (!fReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				GetOverlappedResult(m_hComDev, &m_osRead, &dwLength, FALSE);

				ClearCommError(m_hComDev , &dwErrorFlags, &ComStat ) ;
				/*
				while(!GetOverlappedResult(m_hComDev, &m_osRead, &dwLength, FALSE))
				{
					if(GetLastError() == ERROR_IO_INCOMPLETE)
						continue;
					else
					{
						ClearCommError(m_hComDev , &dwErrorFlags, &ComStat ) ;
						break;
					}
				}*/
			}
			else 
			{
				dwLength = 0;
				ClearCommError(m_hComDev , &dwErrorFlags, &ComStat ) ;
			}
		}
	}
	
	for( i=0;i<65;i++)
	{
		lpszBlock[i] = USB_rbuf[i+1];
	}
	
   return dwLength-1;
}


//读取数据，最多读取64个数据 
int CUSBHIDPort::ReadBuf(LPSTR lpszBlock, int nMaxLength)
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength; 
    char USB_rbuf[255] ={0};

	if(m_bOpen == FALSE) return FALSE;
	// only try to read number of bytes in queue 
	ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = 2;//min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
	//BytesInQueue = ComStat.cbInQue;
	if (dwLength > 0)
	{
		fReadStat = ReadFile(m_hComDev, USB_rbuf,64+1,
		                     &dwLength, &m_osRead) ;
		if (!fReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				while(!GetOverlappedResult(m_hComDev, &m_osRead, &dwLength, TRUE))
				{
					if(GetLastError() == ERROR_IO_INCOMPLETE)
						continue;
					else
					{
						ClearCommError(m_hComDev , &dwErrorFlags, &ComStat ) ;
						break;
					}
				}
			}
			else 
			{
				dwLength = 0;
				ClearCommError(m_hComDev , &dwErrorFlags, &ComStat ) ;
			}
		}
	}
	
	for( i=0;i<65;i++)
	{
		lpszBlock[i] = USB_rbuf[i+1];
	}
	
   return dwLength-1;
}


/************************************************************************************************************
********************* 通用ASCII，基本ASCII码，二进制通信协议 USB HID读写操作 ********************************/
//USB HID读写操作，
/* 
返回值：
出错标志：0x10000  
0x10004 等待超时  
0x10009 表示串口或者网口,USB HID等通信资源不可用
0x00 表示执行完成，无有效信息返回
0x01~0xfff 表示执行完成，并存在有效数据 
*/
unsigned int CUSBHIDPort::USBHIDPort_Mviewer_RW(char *wbuf,int length,char *rbuf,unsigned int time_out,unsigned char RW_Only)
{	
	int total_len,rd_len=0;
	unsigned int time_loop = 0;
	bool status = false;
	int frame_type;
	char rb[2100] = {0};
	int wlen;

	if(m_bOpen==false)
	{
		return 0x10009;
	}

	Flush(); //清缓存 

	if( (wbuf[0] == '<')&&(wbuf[1] == (char)0xC3) )
	{
		frame_type = 0x00;  //二进制数据帧 
	}
	else if( (wbuf[0] == '<')&&(wbuf[1] != (char)0xC3) )
	{
		frame_type = 0x02; //通用性ASCII码数据帧 
	}
	else
	{
		frame_type = 0x01; //Basic ASCII通信
	}
	
	wlen = 0;
	while(wlen< length)
	{
	    WriteBuf(&(wbuf[wlen]),64); //发送指令 
	    wlen += 64;
    }

	if(RW_Only == 1)
	{
		return 0x00;
	}

	Sleep(2);

	total_len = 0x00; //获取到有效数据的个数 

	while(1)
	{
		if(rd_len = ReadBuf(rb,64)) //读取到数据,一次最多读取64个
		{
			if(frame_type == 0x01)
			{
				if(rd_len>=2)
				{
					for(int i=0;i<rd_len;i++)
					{
						rbuf[i] = rb[i];
					}
					return rd_len;
				}
			}
			else if(frame_type == 0x02) //通用性ASCII码数据帧 
			{
				for(int i=0;i<rd_len;i++)
				{
					if( (rb[i] == '<')&&(total_len == 0) ) //获取起始位置 
					{
						rbuf[total_len++] = rb[i];
					}
					else if(total_len > 0)
					{
						rbuf[total_len++] = rb[i];
						
						if(rb[i] == '>') //<RDID>
						{
							return total_len;
						}
					}
				}
			}
			else  //二进制数据帧 
			{
				for(int i=0;i<rd_len;i++)
				{
					rbuf[total_len++] = rb[i];

					if(total_len == 2)
					{
						if( (rbuf[0] == '<')&&(rbuf[1] == (char)0xC3) ) //获取起始位置 '<' 0xC3 
						{
							//rbuf[total_len++] = rb[i];
						}
						else
						{
							rbuf[0] = rbuf[1];
							total_len = 0x01; //重新开始检测 起始点 
							//total_len = 0x00; //重新开始检测 起始点 
						}
					}
					//起始点检测正确，检测结束点 '>'
					if( (total_len >= 11)&&(rbuf[total_len-1] == '>')&&((rbuf[7]+(rbuf[8]&0x0f)*256+11) == total_len) )  //检测二进制数据帧的结束符
					{
						return total_len;
					}
				}
			}
		} //处理获取到的数据

		Sleep(1);
		if(time_loop++ >= time_out)
		{
			time_out = 1;
			return 0x10004; //表示超时 
		}
	}

	return total_len;
}

void CUSBHIDPort::SetParent(CWnd *pParent)
{
	m_pOwner = pParent;

}


void CUSBHIDPort::Flush()
{
	if(m_bOpen == FALSE) return;

	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
}