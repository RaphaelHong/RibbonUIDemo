#if !defined(AFX_SERIALPORT_H__F519AEB8_0C1A_4D84_AC90_A8C29F5BFB43__INCLUDED_)
#define AFX_SERIALPORT_H__F519AEB8_0C1A_4D84_AC90_A8C29F5BFB43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialPort.h : header file
//
#include <afxmt.h>


//#define WM_COMM_BREAK_DETECTED		(WM_USER+1)	// A break was detected on input.
//#define WM_COMM_CTS_DETECTED		(WM_USER+2)	// The CTS (clear-to-send) signal changed state. 
//#define WM_COMM_DSR_DETECTED		(WM_USER+3)	// The DSR (data-set-ready) signal changed state. 
//#define WM_COMM_ERR_DETECTED		(WM_USER+4)	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
//#define WM_COMM_RING_DETECTED		(WM_USER+5)	// A ring indicator was detected. 
//#define WM_COMM_RLSD_DETECTED		(WM_USER+6)	// The RLSD (receive-line-signal-detect) signal changed state. 
//#define WM_COMM_RXCHAR				(WM_USER+7)	// A character was received and placed in the input buffer. 
//#define WM_COMM_RXFLAG_DETECTED		(WM_USER+8)	// The event character was received and placed in the input buffer.  
//#define WM_COMM_TXEMPTY_DETECTED	(WM_USER+9)	// The last character in the output buffer was sent.
/////////////////////////////////////////////////////////////////////////////
// CSerialPort window

class CSerialPort : public CWnd
{
// Construction
public:
	CSerialPort(CWnd *pParent = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialPort)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Flush();
	//unsigned char SeialPort_Mviewer_RW(char *wbuf,int length,char *rbuf);
	BOOL OpenPort();
	void SetParent(CWnd *pParent);
	// start/stop comm watching
	//BOOL		StartMonitoring();
	//BOOL		RestartMonitoring();
	//BOOL		StopMonitoring();

	//static UINT CommThread(LPVOID pParam);
//	int ReadBuf(LPSTR lpszBlock, int nMaxLength,int &BytesInQueue);
	int ReadBuf(LPSTR lpszBlock, int nMaxLength);
	int ReadBuf(LPSTR lpszBlock, int nMaxLength,int &BytesInQueue);
	BOOL WriteBuf( LPSTR lpByte , DWORD dwBytesToWrite);
	unsigned char ReadI2CDataFromMviewer(unsigned char dev, unsigned char offset, unsigned char port,int *err);
	BOOL WriteI2CDataToMviewer(unsigned char dev, unsigned char offset, unsigned char data, unsigned char port);
	unsigned char CSerialPort::Func_RW(unsigned char func_id,unsigned char length,unsigned char flag_rw,unsigned char *data,unsigned char port);
	void ClosePort();
	BOOL SetupPort(int nBaud, int nDataBit=8, int nStopBit=ONESTOPBIT, int nParity=NOPARITY);
	BOOL OpenPort(int nPort, int nBaud, int nDataBit = 8, int nStopBit = ONESTOPBIT, int nParity = NOPARITY);

	unsigned int SeialPort_Mviewer_RW(char *wbuf,int length,char *rbuf);

	virtual ~CSerialPort();

	BOOL m_bOpen;
	CEvent m_hCommEvent;
	DCB	m_dcb;
	int m_nPort;
	// Generated message map functions
protected:
	//{{AFX_MSG(CSerialPort)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

	OVERLAPPED m_osRead;
	OVERLAPPED m_osWrite;
	HANDLE m_hComDev;
	// thread
	CWinThread*			m_Thread;
	// owner window
	CWnd*				m_pOwner;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORT_H__F519AEB8_0C1A_4D84_AC90_A8C29F5BFB43__INCLUDED_)
