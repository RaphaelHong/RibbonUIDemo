// SerialPort.cpp : implementation file
//

#include "stdafx.h"
#include "SerialPort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSerialPort


CSerialPort::CSerialPort(CWnd *pParent)
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

CSerialPort::~CSerialPort()
{
	if(m_bOpen) ClosePort();
	CloseHandle(m_osRead.hEvent);
	CloseHandle(m_osWrite.hEvent);
}


BEGIN_MESSAGE_MAP(CSerialPort, CWnd)
	//{{AFX_MSG_MAP(CSerialPort)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSerialPort message handlers

BOOL CSerialPort::OpenPort(int nPort, int nBaud, int nDataBit, int nStopBit, int nParity)
{
	WCHAR szBuffer[80];

#ifdef _UNICODE
	wsprintf( (LPWSTR)szBuffer, _T("\\\\.\\COM%d "), nPort + 1);
#else
	wsprintf( (LPSTR)szBuffer, _T("\\\\.\\COM%d "), nPort + 1 ) ;	//Solve MFC bug that cannot open port > 10
#endif

	COMMTIMEOUTS CommTimeOuts;

	if(m_bOpen)
	{
		ClosePort();
	}

#ifdef _UNICODE
	m_hComDev =	CreateFile( (LPCWSTR)szBuffer, GENERIC_READ | GENERIC_WRITE,
		0,                    // exclusive access
		NULL,                 // no security attrs
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_OVERLAPPED, // overlapped I/O
		NULL);
#else
	m_hComDev =	CreateFile( (LPCSTR)szBuffer, GENERIC_READ | GENERIC_WRITE,
		0,                    // exclusive access
		NULL,                 // no security attrs
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_OVERLAPPED, // overlapped I/O
		NULL);
#endif
	
	if(m_hComDev == INVALID_HANDLE_VALUE) 
	{
		m_hComDev = 0;
		return FALSE;
	}
	m_bOpen = TRUE;
	m_nPort = nPort;

	SetCommMask(m_hComDev, EV_RXCHAR);
	SetupComm(m_hComDev, 4096, 4096);
	// purge any information in the buffer

	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 2 * CBR_9600 / nBaud;
	CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
	SetCommTimeouts(m_hComDev, &CommTimeOuts);

	BOOL bRet = SetupPort(nBaud, nDataBit, nStopBit, nParity);

	if(!bRet) return FALSE;
	EscapeCommFunction(m_hComDev , SETDTR ) ;
	return TRUE;
}

BOOL CSerialPort::SetupPort(int nBaud, int nDataBit, int nStopBit, int nParity)
{
	GetCommState(m_hComDev, &m_dcb);
	m_dcb.BaudRate = nBaud;
	m_dcb.ByteSize = nDataBit;
	m_dcb.StopBits = (BYTE)nStopBit;
	m_dcb.Parity = nParity;
	if(m_dcb.Parity != 0) m_dcb.fParity = TRUE;
	else	m_dcb.fParity = FALSE;
	m_dcb.fOutX = FALSE;
	m_dcb.fInX = FALSE;
	m_dcb.XonLim = 100 ;
    m_dcb.XoffLim = 100 ;     
	m_dcb.fBinary = TRUE ;
	m_dcb.fParity = TRUE ;
	return SetCommState(m_hComDev, &m_dcb);
}

void CSerialPort::ClosePort()
{
	CString str;
	
	if(m_bOpen)
	{
		m_bOpen = FALSE;
		SetCommMask(m_hComDev , 0 ) ;
		while(m_Thread != NULL)
		{
			Sleep(1);
			;
		}
		// drop DTR
		EscapeCommFunction(m_hComDev , CLRDTR) ;
		// purge any outstanding reads/writes and close device handle
		PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		CloseHandle(m_hComDev);
		m_hComDev = 0;
	}

}


BOOL CSerialPort::WriteBuf(LPSTR lpByte , DWORD dwBytesToWrite)
{
	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD		dwBytesSent = 0;
	DWORD       dwErrorFlags;
	COMSTAT     ComStat;

	if(m_bOpen == FALSE) return FALSE;
	Flush();
	fWriteStat = WriteFile(m_hComDev, lpByte, dwBytesToWrite,
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


int CSerialPort::ReadBuf(LPSTR lpszBlock, int nMaxLength)
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength; 

	if(m_bOpen == FALSE) return FALSE;
	// only try to read number of bytes in queue 
	ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

	if (dwLength > 0)
	{
		fReadStat = ReadFile(m_hComDev, lpszBlock,
		                    dwLength, &dwLength, &m_osRead) ;
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
   return dwLength;
}



int CSerialPort::ReadBuf(LPSTR lpszBlock, int nMaxLength,int &BytesInQueue)
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength; 

	if(m_bOpen == FALSE) return FALSE;
	// only try to read number of bytes in queue 
	ClearCommError(m_hComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
	BytesInQueue = ComStat.cbInQue;
	if (dwLength > 0)
	{
		fReadStat = ReadFile(m_hComDev, lpszBlock,
		                    dwLength, &dwLength, &m_osRead) ;
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
   return dwLength;
}

//UINT CSerialPort::CommThread(LPVOID pParam)
//{
//	// Cast the void pointer passed to the thread back to
//	// a pointer of CSerialPort class
//	CSerialPort *port = (CSerialPort*)pParam;
//
//	if (!SetCommMask(port->m_hComDev , EV_RXCHAR )) return FALSE;
//	
//	DWORD dwEvtMask;
//
//	while (port->m_bOpen)
//	{
//		dwEvtMask=0;
//		WaitCommEvent(port->m_hComDev, &dwEvtMask, NULL);
//		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
//		{
//			if(port->m_pOwner != NULL)
//				port->m_pOwner->PostMessage(WM_COMM_RXCHAR, (WPARAM)port);
//			port->m_hCommEvent.SetEvent();
//		}
//		else
//			Sleep(1);
//	}
//
//    // get rid of event handle
//	port->m_Thread = NULL;
//	return 1;
//}
//
////
//// start comm watching
////
//BOOL CSerialPort::StartMonitoring()
//{
//	if (!(m_Thread = AfxBeginThread(CommThread, this)))
//		return FALSE;
//	TRACE("Thread started\n");
//	return TRUE;	
//}
//
////
//// Restart the comm thread
////
//BOOL CSerialPort::RestartMonitoring()
//{
//	int cnt;
//	TRACE("Thread resumed\n");
//	while(1)
//	{
//		cnt = m_Thread->ResumeThread();
//		if(cnt<=0) break;
//	}
//	return TRUE;	
//}
//
//
////
//// Suspend the comm thread
////
//BOOL CSerialPort::StopMonitoring()
//{
//	TRACE("Thread suspended\n");
//	m_Thread->SuspendThread(); 
//	return TRUE;
//}

void CSerialPort::SetParent(CWnd *pParent)
{
	m_pOwner = pParent;

}

BOOL CSerialPort::OpenPort()
{
	return OpenPort(m_nPort, m_dcb.BaudRate, m_dcb.ByteSize, m_dcb.StopBits, m_dcb.Parity);
}

void CSerialPort::Flush()
{
	if(m_bOpen == FALSE) return;

	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
}

/***************************************************************************************************************************************
*********************************************** 基础ASCII 通信协议  *******************************************************************/
//I2C操作读  RI
unsigned char CSerialPort::ReadI2CDataFromMviewer(unsigned char dev, unsigned char offset, unsigned char port,int *err)
{
	char wbuf[32] = {0};
	char rb[64] = {0};
	int time_loop = 0;
	int time_out = 0;
	char rt[5]={'0','x',0,0,0},*pe;
	unsigned long ret;
	//*err =0;
	//ReadDataFromMviewer(0xff);	// Open the I2C read operation	
	Sleep(2);
	sprintf_s(wbuf,"RI %02x %02x %02x\r",dev,offset,port);
	WriteBuf(wbuf,(DWORD)strlen(wbuf));
	while(1)
	{
		if(ReadBuf(rb,64))
			break;
		Sleep(2);
		time_loop++;
		if(time_loop>=500)
		{
			time_out = 1;
			*err = 1; //超时 ，定义为1.
			return false;
		}
	}

	rt[2] = rb[0];
	rt[3] = rb[1];

	ret = strtoul(rt,&pe,0);
	return (unsigned char)ret;
}

//I2C的操作写 WI
BOOL CSerialPort::WriteI2CDataToMviewer(unsigned char dev, unsigned char offset, unsigned char data, unsigned char port)
{
	char wbuf[32] = {0};
	unsigned char rb[64] = {0};
	char srbuf[64] = {0};
	int time_loop = 0;
	int time_out = 0;
	int i;
	CString str;

	sprintf_s(wbuf,"WI %02x %02x %02x %02x\r",dev,offset,port,data);

	Flush();
	WriteBuf(wbuf,(DWORD)strlen(wbuf));
	Sleep(2);
	int dchar_Read = 0;
	int total_char_read = 0;
	int rb_ptr = 0;
	while(1)
	{
		dchar_Read = ReadBuf(srbuf,64);
		for(i=0;i<dchar_Read;i++)
		{	
			rb[rb_ptr] = srbuf[i];
			rb_ptr++;
			if(rb_ptr==64)
				rb_ptr = 0;
		}
		total_char_read+=dchar_Read;
		if(total_char_read>=2)
			break;
		Sleep(1);
		time_loop++;
		if(time_loop>=500)
		{
			time_out = 1;
			return false;
		}
	}	
	str = (CString)rb;

	if(str.Find(_T("WI"),0)!=-1)
		return TRUE;
	else return false;
}

/************************************************************************************************************
********************* 通用ASCII，基本ASCII码，二进制通信协议 串口读写操作 ********************************/
//串口读写操作，
/* 
返回值：
出错标志：0x10000  
0x10004 等待超时  
0x10009 表示串口或者网口等通信资源不可用
0x00 表示执行完成，无有效信息返回
0x01~0xfff 表示执行完成，并存在有效数据 
*/
unsigned int CSerialPort::SeialPort_Mviewer_RW(char *wbuf,int length,char *rbuf)
{	
	int total_len,rd_len,time_loop,time_out = 0;
	rd_len  = 0;
	time_loop = 0;
	bool status = false;
	int frame_type;
	char rb[2100] = {0};

	if(m_bOpen==false)
	{
		return CMD_TIMEOUT;
	}

	Flush();

	if( (wbuf[0] == '<')&&(wbuf[1] == 0xC3) )
	{
		frame_type = 0x00;  //二进制数据帧 
	}
	else if( (wbuf[0] == '<')&&(wbuf[1] != 0xC3) )
	{
		frame_type = 0x02; //通用性ASCII码数据帧 
	}
	else
	{
		frame_type = 0x01; //Basic ASCII通信
	}
	WriteBuf(wbuf,length); //发送指令 
	Sleep(2);

	total_len = 0x00; //获取到有效数据的个数 

	while(1)
	{
		if(rd_len = ReadBuf(rb,256))//读取到数据,一次最多读取256个
		{
			if(frame_type == 0x01)  //Basic ASCII通信
			{
				if(rd_len>=2)
				{
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
							total_len = 0x00; //重新开始检测 起始点 
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
		if(time_loop++ >= COM_MAX_OUTTIME)
		{
			time_out = 1;
			return CMD_TIMEOUT; //表示超时 
		}
	}

	return total_len;
}