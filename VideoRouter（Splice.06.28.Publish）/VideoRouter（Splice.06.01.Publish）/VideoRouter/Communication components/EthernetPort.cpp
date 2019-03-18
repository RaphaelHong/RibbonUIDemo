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

//创建TCP连接
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

	int TimeOut=200; //设置发送超时1秒
	if(::setsockopt(m_sockClient,SOL_SOCKET,SO_SNDTIMEO,(char *)&TimeOut,sizeof(TimeOut))==SOCKET_ERROR)
	{
		return false;
	}
	TimeOut=200;//设置接收超时1秒
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
		timeout.tv_sec = 0; //连接超时
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


//网络方式 读写  参数 
/*
返回值：
出错标志：0x10000  
0x10000 表示接收端无响应，通信出错 
0x10004 等待超时  
0x10009 表示串口或者网口等通信资源不可用

0x100ff  未知错误
0x00 表示执行完成，无有效信息返回
0x01~0xfff 表示执行完成，并存在有效数据 
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

	ret = send(m_sockClient,wbuf,length,0);
	if(ret == -1)
	{
		return CMD_RTER; //表示通信出错 
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
						
						if(rb[i] == '>')
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
					if( (total_len >= 11)&&(rbuf[total_len-1] == '>')&&((UCHAR(rbuf[7])+(UCHAR(rbuf[8])&0x0f)*256+11) == total_len) )  //检测二进制数据帧的结束符
					{
						return total_len;
					}
				}
			}
		} //处理获取到的数据

		if(time_loop++ >= NET_MAX_OUTTIME)
		{
			time_out = 1;
			return CMD_TIMEOUT; //表示超时 
		}
	}

	return 0x100ff; //数据返回位置错误 
}
