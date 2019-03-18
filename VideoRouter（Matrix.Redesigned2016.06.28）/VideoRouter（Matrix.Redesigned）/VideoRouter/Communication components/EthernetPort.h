#pragma once

class CEthernetPort
{
public:
	CEthernetPort(void);
	~CEthernetPort(void);


public:
	CString m_ipaddr;
	SOCKET m_sockClient;
	SOCKADDR_IN m_addrSrv;
	BOOL b_Connect;

public:

	BOOL OpenSocket();
	BOOL CloseSocket();	
	unsigned int Ethernet_Mviewer_RW(char *wbuf,int length,char *rbuf);
};
