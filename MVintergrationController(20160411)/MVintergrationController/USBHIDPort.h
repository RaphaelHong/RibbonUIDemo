#if !defined(AFX_USBHIDPORT_H__F519AEB8_0C1A_4D84_AC90_A8C29F5BFB43__INCLUDED_)
#define AFX_USBHIDPORT_H__F519AEB8_0C1A_4D84_AC90_A8C29F5BFB43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USBHIDPort.h : header file
//
#include <afxmt.h>


#define WM_COMM_BREAK_DETECTED		(WM_USER+1)	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		(WM_USER+2)	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		(WM_USER+3)	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		(WM_USER+4)	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		(WM_USER+5)	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		(WM_USER+6)	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				(WM_USER+7)	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		(WM_USER+8)	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	(WM_USER+9)	// The last character in the output buffer was sent.  
/////////////////////////////////////////////////////////////////////////////
// CCUSBHIDPort window

class CUSBHIDPort : public CWnd
{
// Construction
public:
	CUSBHIDPort(CWnd *pParent = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCUSBHIDPort)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Flush();
	BOOL OpenPort();
	void ClosePort();
	void SetParent(CWnd *pParent);

	static UINT CommThread(LPVOID pParam);
	
	int ReadBuf(LPSTR lpszBlock, int nMaxLength,int &BytesInQueue);
	BOOL WriteBuf( LPSTR lpByte , DWORD dwBytesToWrite);
	int ReadBuf(LPSTR lpszBlock, int nMaxLength);
	
	unsigned int USBHIDPort_Mviewer_RW(char *wbuf,int length,char *rbuf,unsigned int time_out = 5000,unsigned char RW_Only=0);
	    
	BOOL OpenPort(unsigned short myVid, unsigned short myPid, unsigned short myPvn);

	virtual ~CUSBHIDPort();

	BOOL m_bOpen;
	CEvent m_hCommEvent;
	DCB	m_dcb;
	int m_nPort;

	unsigned short USB_HID_Vid;
	unsigned short USB_HID_Pid;
	unsigned short USB_HID_Pvn;

	// Generated message map functions
protected:
	//{{AFX_MSG(CUSBHIDPort)
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
