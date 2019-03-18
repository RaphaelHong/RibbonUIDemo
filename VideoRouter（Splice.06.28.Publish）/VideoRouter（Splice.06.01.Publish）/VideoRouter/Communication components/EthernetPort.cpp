#include "StdAfx.h"
#include "EthernetPort.h"

CEthernetPort::CEthernetPort(void)
{
	b_Connect = false;
}

CEthernetPort::~CEthernetPort(void)
{
	CloseSocket();
}

//����TCP����
BOOL CEthernetPort::OpenSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested,&wsaData);
	if(err != 0)
		return false;

	if( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	m_sockClient = socket(AF_INET,SOCK_STREAM,0);
	if(m_sockClient==INVALID_SOCKET)
	{
		return false;
	}

	int TimeOut=200; //���÷��ͳ�ʱ1��
	if(::setsockopt(m_sockClient,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		return false;
	}
	TimeOut=200;//���ý��ճ�ʱ1��
	if(::setsockopt(m_sockClient,SOL_SOCKET,SO_RCVTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		return false;
	}
	unsigned long ul = 1;
	ioctlsocket(m_sockClient,FIONBIO,(unsigned long*)&ul);
	
	m_addrSrv.sin_addr.S_un.S_addr = inet_addr(m_ipaddr.GetBuffer());
	m_addrSrv.sin_family = AF_INET;
	m_addrSrv.sin_port = htons(0xf3f4);

	int iTimes=0;
	char error;
	int len = sizeof(char);
	if(connect(m_sockClient,(SOCKADDR*)&m_addrSrv,sizeof(SOCKADDR)) == -1)
	{
		struct timeval timeout ;
		fd_set wset;
		FD_ZERO(&wset);
		FD_SET(m_sockClient, &wset);
		timeout.tv_sec = 0; //���ӳ�ʱ
		timeout.tv_usec = 300000; 

		int ret = select(m_sockClient+1, NULL, &wset, NULL, &timeout);
		if ( ret> 0 )
		{
			getsockopt(m_sockClient, SOL_SOCKET, SO_ERROR, &error,&len);
			if(error==0)
			{
				b_Connect = true;
			}
		}
	}
	ul = 0;
	ioctlsocket(m_sockClient,FIONBIO,(unsigned long*)&ul);
	return b_Connect;
}

BOOL CEthernetPort::CloseSocket()
{
	closesocket(m_sockClient);
	WSACleanup();
	b_Connect = false;
	return true;
}


//���緽ʽ ��д  ���� 
/*
����ֵ��
�����־��0x10000  
0x10000 ��ʾ���ն�����Ӧ��ͨ�ų��� 
0x10004 �ȴ���ʱ  
0x10009 ��ʾ���ڻ������ڵ�ͨ����Դ������

0x100ff  δ֪����
0x00 ��ʾִ����ɣ�����Ч��Ϣ����
0x01~0xfff ��ʾִ����ɣ���������Ч���� 
*/
unsigned int CEthernetPort::Ethernet_Mviewer_RW(char *wbuf,int length,char *rbuf)
{
	int ret;
	int total_len = 0;
	CString str;
	char rb[2100]={0};
	int frame_type;

	int rd_len,time_loop,time_out = 0;
	rd_len  = 0;
	time_loop = 0;

	if(b_Connect == false)	
	{
		return CMD_TIMEOUT;	
	}
	
	if( (wbuf[0] == '<')&&(wbuf[1] == (char)0xC3) )
	{
		frame_type = 0x00;  //����������֡ 
	}
	else if( (wbuf[0] == '<')&&(wbuf[1] != 0xC3) )
	{
		frame_type = 0x02; //ͨ����ASCII������֡ 
	}
	else
	{
		frame_type = 0x01; //Basic ASCIIͨ��
	}

	ret = send(m_sockClient,wbuf,length,0);
	if(ret == -1)
	{
		return CMD_RTER; //��ʾͨ�ų��� 
	}

	struct timeval timeout ;
	fd_set r;
	FD_ZERO(&r);
	FD_SET(m_sockClient, &r);
	timeout.tv_sec = 0;  
	timeout.tv_usec = 2; 

	time_loop = 0;

	DWORD dwStart = 0;
	DWORD dwEnd = 0;

	while(1)
	{
		select(m_sockClient+1, &r, 0, 0, &timeout);
		rd_len = recv(m_sockClient,rb,2048,0);

		if(rd_len >0)
		{
			if(frame_type == 0x01)  //Basic ASCIIͨ��
			{
				if(rd_len>=2)
				{
					return rd_len;
				}
			}
			else if(frame_type == 0x02) //ͨ����ASCII������֡ 
			{
				for(int i=0;i<rd_len;i++)
				{
					if( (rb[i] == '<')&&(total_len == 0) ) //��ȡ��ʼλ�� 
					{
						rbuf[total_len++] = rb[i];
					}
					else if(total_len > 0)
					{
						rbuf[total_len++] = rb[i];
						
						if(rb[i] == '>')
						{
							return total_len;
						}
					}
				}
			}
			else  //����������֡ 
			{
				for(int i=0;i<rd_len;i++)
				{
					rbuf[total_len++] = rb[i];

					if(total_len == 2)
					{	
						if( (rbuf[0] == '<')&&(rbuf[1] == (char)0xC3) ) //��ȡ��ʼλ�� '<' 0xC3 
						{
							//rbuf[total_len++] = rb[i];
						}
						else
						{
							total_len = 0x00; //���¿�ʼ��� ��ʼ�� 
						}
					}
					//��ʼ������ȷ���������� '>'
					if( (total_len >= 11)&&(rbuf[total_len-1] == '>')&&((UCHAR(rbuf[7])+(UCHAR(rbuf[8])&0x0f)*256+11) == total_len) )  //������������֡�Ľ�����
					{
						return total_len;
					}
				}
			}
		} //�����ȡ��������

		if(time_loop++ >= NET_MAX_OUTTIME)
		{
			time_out = 1;
			return CMD_TIMEOUT; //��ʾ��ʱ 
		}
	}

	return 0x100ff; //���ݷ���λ�ô��� 
}
