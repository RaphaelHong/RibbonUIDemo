#include "stdafx.h"
#include "MVintergrationController.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommandClass cpp
CCommandClass::CCommandClass()
{
	m_pCmdSock = NULL;
	m_pCmdCom = NULL;
	m_pCmdUSB = NULL;
}

void CCommandClass::SetInterface(CEthernetPort *pSock, CSerialPort *pCom, CUSBHIDPort *pUSB)
{
	m_pCmdSock = pSock;
	m_pCmdCom = pCom;
	m_pCmdUSB = pUSB;
}

BOOL CCommandClass::ConvertSTM_Cmd(STM_Command* pCmd_STM,char *pFrame,int len)
{
	ASSERT(pCmd_STM);
	ASSERT(pFrame);

	if(len<11)
		return false;
	memcpy(pCmd_STM,pFrame,9);
	int bytes = (pCmd_STM->length==0x8001)?1:pCmd_STM->length;
	memcpy(pCmd_STM->data,pFrame+9,bytes);
	memcpy(&(pCmd_STM->check),pFrame+9+bytes,2);
	if(pCmd_STM->isCorrect())
		return true;
	else
		return false;
}

void CCommandClass::ConvertPC_Cmd(PC_Command* pCmd_PC,char *pFrame,unsigned int &len)
{
	ASSERT(pCmd_PC);
	ASSERT(pFrame);

	pCmd_PC->CalcCheck();
	len = pCmd_PC->length+11;
	memcpy(pFrame,pCmd_PC,len-2);
	memcpy(pFrame+len-2,&(pCmd_PC->check),2);
}


UINT CCommandClass::BinCommand_RW(UCHAR cmd, UINT cmd_para, UCHAR *data, UINT len, UCHAR port, int usbRW)
{
	// ȷ���Ƿ����ͨ������
	if (theApp.m_nConnType == NON_CONNECT) return CMD_TIMEOUT;

	PC_Command Cmd_PC;
	STM_Command Cmd_STM;
	char sendbuf[2100]={0};
	char recvbuf[2100]={0};
	unsigned int length;

	memset(&Cmd_PC,0,sizeof(PC_Command));
	Cmd_PC.head1 = '<';  //֡ͷ1
	Cmd_PC.head2 = 0xC3; //֡ͷ2
	Cmd_PC.inst  = cmd;
	Cmd_PC.para  = cmd_para;
	Cmd_PC.length = len;
	memcpy(&Cmd_PC.data,data,len);
	Cmd_PC.tail = '>';   //֡β
	ConvertPC_Cmd(&Cmd_PC,sendbuf,length); //������֡��װ

	//**************************  ���η�װ���Ѵμ��豸���������Ϣ��װ����  **************************** 
	memset(&Cmd_PC,0,sizeof(PC_Command));
	Cmd_PC.head1 = '<';  //֡ͷ1
	Cmd_PC.head2 = 0xC3; //֡ͷ2
	Cmd_PC.inst  = 0xfe;
	Cmd_PC.para  = port;
	Cmd_PC.length = length; //
	memcpy(&Cmd_PC.data,sendbuf,length);
	Cmd_PC.tail = '>';   //֡β
	ConvertPC_Cmd(&Cmd_PC,sendbuf,length); //������֡��װ


	int Length = 0;
	//����ָ��
	if(m_pCmdCom->m_bOpen)
		Length = m_pCmdCom->SeialPort_Mviewer_RW(sendbuf,length,recvbuf);
	else if(m_pCmdSock->b_Connect) 
		Length = m_pCmdSock->Ethernet_Mviewer_RW(sendbuf,length,recvbuf);  //�ܵ�֡����
	else if (m_pCmdUSB->m_bOpen)
		Length = m_pCmdUSB->USBHIDPort_Mviewer_RW(sendbuf,length, recvbuf, 1000, usbRW);
	else
		return CMD_TIMEOUT;  //δ֪���� 

	if(Length&CMD_TIMEOUT) //������ʱ
	{
		return Length;
	}

	unsigned char ret = 0;
	ret = ConvertSTM_Cmd(&Cmd_STM,recvbuf,Length); //����У��
	if(ret == 0)//У��ʧ��
	{
		return CMD_RTERCODE;  //У��ʧ�� 
	}

	if( (Cmd_STM.length == 0x8001)&&(Cmd_STM.data[0] == 0xf4) )
	{
		return CMD_RTER; //ִ��ʧ�� 
	}
	else 
	{
		memcpy(recvbuf,&Cmd_STM.data,Cmd_STM.length);  //ִ����ɵ����ݣ���ŵ�data�� 
		Length = UCHAR(recvbuf[7]) + (UCHAR(recvbuf[8])&0x0f)*256 +11; //�����ݳ���  
		ret = ConvertSTM_Cmd(&Cmd_STM,recvbuf,Length); //����У��

		if(ret == 0)//У��ʧ��
		{
			return CMD_RTERCODE;//����У����� 
		}

		if( (Cmd_STM.length == 0x8001)&&(Cmd_STM.data[0] == 0xf4) )
		{
			return CMD_RTER; //ִ��ʧ�� 
		}
		else if( (Cmd_STM.length == 0x01)&&(Cmd_STM.data[0] == 0x04) )
		{
			memcpy(data,&Cmd_STM.data,1);  //ִ����ɵ����ݣ���ŵ�data�� 
			return CMD_RTOK;//ִ�гɹ�
		}
		else
		{
			memcpy(data,&Cmd_STM.data,Length-11);  //ִ����ɵ����ݣ���ŵ�data�� 
			return Cmd_STM.length;//ִ�гɹ�
		}
	}
}

UINT CCommandClass::NormalASCII_RW(char *cmd, UINT *cmd_para, UINT len, UINT port, int usbRW)
{
	//ȷ���Ƿ����ͨ������
	if (theApp.m_nConnType == NON_CONNECT)	return CMD_TIMEOUT;

	char wbuf[2048] = {0};
	char rbuf[2048] = {0};
	UINT rbuf_len = 0;

	//ָ��֡��װ 
	//����ͷ
	if(port == 0xff)
		sprintf_s(wbuf,"<%s",cmd);
	else
		sprintf_s(wbuf,"<%d,%s", port, cmd);
	//�������
	for(unsigned int i=0;i<len;i++)
	{	sprintf_s(wbuf,"%s,%d", wbuf, cmd_para[i]); }
	//����β
	sprintf_s(wbuf,"%s>", wbuf);

	//����ָ��
	if(m_pCmdCom->m_bOpen)
		rbuf_len = m_pCmdCom->SeialPort_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if(m_pCmdSock->b_Connect) 
		rbuf_len = m_pCmdSock->Ethernet_Mviewer_RW(wbuf,strlen(wbuf),rbuf);
	else if (m_pCmdUSB->m_bOpen)
		rbuf_len = m_pCmdUSB->USBHIDPort_Mviewer_RW(wbuf,strlen(wbuf), rbuf, 1000, usbRW);
	else
		return CMD_TIMEOUT;

	theApp.PrintLog(wbuf);
	theApp.PrintLog(rbuf);

	if(rbuf_len & CMD_TIMEOUT) //������ʱ
		return rbuf_len;

	int start_index,end_index;
	CString str,str_tmp;
	str.Format("%s", rbuf);
	if (str.Find('<') == -1 || str.Find('>') == -1)
	{
		return CMD_RTER;
	}
	else if(str.Find("RTOK") != -1) //���� ִ����ɱ�־ "RTOK"
	{
		return CMD_RTOK;
	}
	else if(str.Find("RTER") != -1) //���� ִ��ʧ��/�����־ "RTER"
	{
		start_index = str.Find("RTER")+4;  //��4��cmd�����ַ� 1�������ַ� 
		end_index   = str.Find('>',start_index); //��ȡ��������λ��

		if(end_index > start_index)//��ʾ���ڴ������
		{
			str_tmp = str.Mid(start_index+1,end_index-start_index-1);	
			cmd_para[0] = atoi(str_tmp);
			return CMD_RTERCODE;
		}
		else
		{
			return CMD_RTER;
		}
	}
	else  //���ؾ�������� 
	{
		rbuf_len = 0;
		start_index = str.Find(cmd)+1+4;  //��4��cmd�����ַ� 1�������ַ� 
		end_index   = str.Find(',',start_index);

		while(end_index != -1)//��ȡ','֮�����ֵ ʮ����
		{
			str_tmp = str.Mid(start_index,end_index-start_index);	
			cmd_para[rbuf_len++] = atoi(str_tmp);

			start_index = end_index+1;
			end_index   = str.Find(',',start_index);
		}

		end_index   = str.Find('>',start_index);
		if(end_index!=-1) //��ȡ','��'>'֮�����ֵ ʮ���� 
		{
			str_tmp = str.Mid(start_index,end_index-start_index);	
			cmd_para[rbuf_len++] = atoi(str_tmp);
		}
		return rbuf_len;
	}
}

//--------------------------------- ϵͳ��֤ ------------------------------------//
// ��ȡ����ID
int CCommandClass::CmdRMVR(int nPort)
{
	UINT nParam[SAFE_BUFF_LEN]={0};
	UINT nRet = NormalASCII_RW("RMVR", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

// ��ȡ�豸ID ��忨ID
int CCommandClass::CmdRDID(int nPort)
{
	UINT nParam[SAFE_BUFF_LEN]={0};
	UINT nRet = NormalASCII_RW("RDID", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return (int)nParam[0];
}

// ��ȡ�豸������ ��忨������
int CCommandClass::CmdRDFN(int nPort)
{
	UINT nParam[SAFE_BUFF_LEN]={0};
	UINT nRet = NormalASCII_RW("RDFN", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	return nParam[0];
}

// ��ȡ�豸���а忨���忨ͨ�Ŷ˿ں�
int CCommandClass::CmdRDPT(int &nBoards, int *nPortID, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN]={0};
	UINT nRet = NormalASCII_RW("RDPT", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	nBoards = nParam[0];
	for (int i=0; i < nBoards; i++)
		nPortID[i] = nParam[i+1];

	return CMD_RTOK;
}

//--------------------------------- �ֱ��� ---------------------------------//
// ��ȡ����ֱ���
int CCommandClass::CmdRINT(TIMING_STRUCT *pTsR, int nEdidPort/*EDID��*/, int nTimingIndex/*���ηֱ���*/, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	int nLen = 0;
	nParam[nLen++] = nEdidPort;
	if (nTimingIndex != 0xff) nParam[nLen++] = nTimingIndex;
	UINT nRet = NormalASCII_RW("RINT", nParam, nLen, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	if (nParam[4] <= 0) return CMD_FAILURE;

	int nTick = 2;
	if (nTimingIndex == 0xff) nTick = 0; //��������
	//�������еĲ�������ȡֵ
	pTsR->m_Hz        = nParam[nTick++];
	pTsR->m_HTotal    = nParam[nTick++];
	pTsR->m_HActive   = nParam[nTick++];
	pTsR->m_HFPorch   = nParam[nTick++];
	pTsR->m_HSyncWid  = nParam[nTick++];
	pTsR->m_HPol      = nParam[nTick++];
	pTsR->m_VTotal    = nParam[nTick++];
	pTsR->m_VActive   = nParam[nTick++];
	pTsR->m_VFPorch   = nParam[nTick++];
	pTsR->m_VSyncWid  = nParam[nTick++];
	pTsR->m_VPol      = nParam[nTick++];
	pTsR->m_PixelRate = nParam[nTick++];

	pTsR->m_HBlank    = pTsR->m_HTotal-pTsR->m_HActive;
	pTsR->m_VBlank    = pTsR->m_VTotal-pTsR->m_VActive;

	return CMD_RTOK;
}

// ��������ֱ���
int CCommandClass::CmdWINT(TIMING_STRUCT *pTsW, int nEdidport, int nTimingIndex, int nPort)  
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	pTsW->m_HTotal = pTsW->m_HActive+pTsW->m_HBlank;
	pTsW->m_VTotal = pTsW->m_VActive+pTsW->m_VBlank;

	int nTick = 0;
	nParam[nTick++] = nEdidport;
	//������д��ʱ����Ҫ�ӷֱ�����ţ�����ͳһ��0xff��ʾĬ�Ϸֱ���
	if (nTimingIndex != 0xff) nParam[nTick++] = nTimingIndex;
	nParam[nTick++] = pTsW->m_Hz;
	nParam[nTick++] = pTsW->m_HTotal;
	nParam[nTick++] = pTsW->m_HActive;
	nParam[nTick++] = pTsW->m_HFPorch;
	nParam[nTick++] = pTsW->m_HSyncWid;
	nParam[nTick++] = pTsW->m_HPol;
	nParam[nTick++] = pTsW->m_VTotal;
	nParam[nTick++] = pTsW->m_VActive;
	nParam[nTick++] = pTsW->m_VFPorch;
	nParam[nTick++] = pTsW->m_VSyncWid;
	nParam[nTick++] = pTsW->m_VPol;
	nParam[nTick++] = pTsW->m_PixelRate;

	UINT nRet = NormalASCII_RW("WINT", nParam, nTick, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

//��ȡ��ʱ�źŵķֱ���
int CCommandClass::CmdGETR(UINT &nHz, UINT &nHtotal, UINT &nHactive, UINT &nVtatol, UINT &nVactive, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("GETR", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i = 0;
	nHz      = nParam[i++];
	nHtotal  = nParam[i++];
	nHactive = nParam[i++];
	nVtatol  = nParam[i++];
	nVactive = nParam[i++];

	return CMD_RTOK;
}

//��ȡ����ֱ���
int CCommandClass::CmdROUT(TIMING_STRUCT *pTsR, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("ROUT", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	int i=1;

	pTsR->m_Hz        = nParam[i++];
	pTsR->m_HTotal    = nParam[i++];
	pTsR->m_HActive   = nParam[i++];
	pTsR->m_HFPorch   = nParam[i++];
	pTsR->m_HSyncWid  = nParam[i++];
	pTsR->m_HPol      = nParam[i++];
	pTsR->m_VTotal    = nParam[i++];
	pTsR->m_VActive   = nParam[i++];
	pTsR->m_VFPorch   = nParam[i++];
	pTsR->m_VSyncWid  = nParam[i++];
	pTsR->m_PixelRate = nParam[i++];

	pTsR->m_HBlank    = pTsR->m_HTotal - pTsR->m_HActive;
	pTsR->m_VBlank    = pTsR->m_VTotal - pTsR->m_VActive;

	return CMD_RTOK;
}

//��������ֱ���
int CCommandClass::CmdWOUT(TIMING_STRUCT *pTsW, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	pTsW->m_HTotal = pTsW->m_HActive+pTsW->m_HBlank;
	pTsW->m_VTotal = pTsW->m_VActive+pTsW->m_VBlank;

	int nTick = 0;
	nParam[nTick++] = 1; //Different from input
	nParam[nTick++] = pTsW->m_Hz;
	nParam[nTick++] = pTsW->m_HTotal;
	nParam[nTick++] = pTsW->m_HActive;
	nParam[nTick++] = pTsW->m_HFPorch;
	nParam[nTick++] = pTsW->m_HSyncWid;
	nParam[nTick++] = pTsW->m_HPol;
	nParam[nTick++] = pTsW->m_VTotal;
	nParam[nTick++] = pTsW->m_VActive;
	nParam[nTick++] = pTsW->m_VFPorch;
	nParam[nTick++] = pTsW->m_VSyncWid;
	nParam[nTick++] = pTsW->m_VPol;
	nParam[nTick++] = pTsW->m_PixelRate;

	UINT nRet = NormalASCII_RW("WOUT", nParam, nTick, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

//--------------------------------- �������� ---------------------------------//
// ��ȡ�������
int CCommandClass::CmdRNET(pNetParams netparamR, int nPort/* =0xff */)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RNET", nParam, 0 , nPort);

	if (nRet == CMD_RTER || nRet== CMD_RTERCODE || nRet == CMD_TIMEOUT) return CMD_FAILURE;

	if (nRet != 18) return CMD_FAILURE; //���������������Ӧ����18������

	memcpy(netparamR, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// �����������
int CCommandClass::CmdWNET(pNetParams netparamW, int nPort/* =0xff */)  
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nLen = sizeof(NetParams)/sizeof(UINT);

	memcpy(nParam, netparamW, sizeof(NetParams));

	UINT nRet = NormalASCII_RW("WNET", nParam, nLen);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//--------------------------------- ɫ��У�� ---------------------------------//
// ɫ�����
int CCommandClass::CmdTCLR(pColorTestParams clrtestParam, int nPort)  
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nLen = sizeof(ColorTestParams)/sizeof(UINT);

	memcpy(nParam, clrtestParam, sizeof(ColorTestParams));

	UINT nRet = NormalASCII_RW("TCLR", nParam, nLen, nPort);

	if(nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ȡ��ɫ�����
int CCommandClass::CmdCCLR(int nPort)  
{
	UINT nRet = NormalASCII_RW("CCLR", NULL, 0, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// д��ɫ������
int CCommandClass::CmdWCRA_ASC(UINT *pColorData, int nDataLen, int nColortype, int nPort) 
{
	UINT nParam[SAFE_BUFF_LEN*4] = {0};
	nParam[0] = nColortype;
	memcpy(&nParam[1], pColorData, nDataLen*sizeof(UINT));

	UINT nRet = NormalASCII_RW("WCRA", nParam, nDataLen+1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}
int CCommandClass::CmdWCRA_BIN(UCHAR *pColorData, int nColortype, int nLen, int nPort)
{
	return BinCommand_RW(BINEXE_COLOR_WRITE|BINCODE_FCN, nColortype, pColorData, nLen, nPort);
}

// ��ȡɫ������
int CCommandClass::CmdRCRA_ASC(UINT *pColorData, int nColortype, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN*4] = {0};
	nParam[0] = nColortype;
	UINT nRet = NormalASCII_RW("RCRA", nParam, 1, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pColorData, &nParam[1], (nRet-1)*sizeof(UINT));

	return CMD_RTOK;
}
int CCommandClass::CmdRCRA_BIN(UCHAR *pColorData, int nColortype, int nPort)
{
	return BinCommand_RW(BINEXE_COLOR_READ|BINCODE_FCN, 0x07, pColorData, 0, nPort);
}

// ��λɫ��
int CCommandClass::CmdRSTC(UINT nColorsel, int nPort) 
{
	UINT nParam = nColorsel;

	UINT nRet = NormalASCII_RW("RSTC", &nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// �����ںϴ�Ӱ��
int CCommandClass::CmdCRBD(UINT nBlendOpt, int nPort)
{
	UINT nParam = nBlendOpt;

	UINT nRet = NormalASCII_RW("CRBD", &nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//--------------------------------- �ںϴ����� ---------------------------------//
// �����ںϲ���
int CCommandClass::CmdWBDP(pBlendParams pBPW, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	memcpy(nParam, pBPW, sizeof(BlendParams));

	UINT nRet = NormalASCII_RW("WBDP", nParam, 5, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ�ںϲ���
int CCommandClass::CmdRBDP(UINT *pBlendParam, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RBDP", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pBlendParam, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// ����/�ر��ںϹ���
int CCommandClass::CmdWBDE(UINT nEnableW, int nPort)
{
	UINT nParam = nEnableW;

	UINT nRet = NormalASCII_RW("WBDE", &nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ�ںϹ����Ƿ���
int CCommandClass::CmdRBDE(UINT &nEnableR, int nPort)
{
	UINT nParam = 0;

	UINT nRet = NormalASCII_RW("RBDE", &nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	nEnableR = nParam;

	return CMD_RTOK;
}

//�����ںϴ����
int CCommandClass::CmdWBDW(UINT *pWidthW, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	memcpy(nParam, pWidthW, sizeof(nParam));

	UINT nRet = NormalASCII_RW("WBDW", nParam, 4, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ�ںϴ����
int CCommandClass::CmdRBDW(UINT *pWidthR, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RBDW", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pWidthR, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// ���ô��׷�ֵ
int CCommandClass::CmdWBDT(UINT nWhiteLvlW, int nPort)   
{
	UINT nParam = nWhiteLvlW;

	UINT nRet = NormalASCII_RW("WBDT", &nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ���׷�ֵ
int CCommandClass::CmdRBDT(UINT &nWhiteLvlR, int nPort)
{
	UINT nParam = 0;

	UINT nRet = NormalASCII_RW("RBDT", &nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	nWhiteLvlR = nParam;

	return CMD_RTOK;
}

// ��ȡ�ں�ֵ
int CCommandClass::CmdRBPD(UINT *pDatatoRead, int nBlendside, int nPureWhite, int nBlendWidth, int nPort)
{
	unsigned char data[SAFE_BUFF_LEN*5];
	unsigned short *pshort_buf = (unsigned short*)data;

	int nStart = 0;
	int nTotol = nBlendWidth;
	int nRest = nTotol > 512 ? 512 : nTotol;
	int para;

	while(nRest > 0)
	{
		para = nBlendside|(nPureWhite<<8)| (nStart<<16);  //blend_side   white_sel   start_addr
		data[0] = (nRest*2)&0xff;    data[1] = ((nRest*2)>>8)&0xff;  //read data len
		int return_back = BinCommand_RW(BINEXE_BLENDPARA_READ|BINCODE_FCN, para, data,2, nPort);

		if(return_back&0x10000) //ͨ��ʧ�� 
		{	
			return CMD_FAILURE;		
		}

		for(int i=0;i<nRest;i++)
		{
			pDatatoRead[nStart+i] = pshort_buf[i];
		}

		nStart += nRest;
		nRest = (nTotol-nStart > 512) ? 512 : (nTotol-nStart);
	}

	return CMD_RTOK;
}

// д���ں�ֵ
int CCommandClass::CmdWBPD(UINT *pDatatoWrite/*���ݵ�ַ*/, int nBegin/*д����ʼλ��*/, int nEnd/*д�����λ��*/, int nBlendside, int nPureWhite, int nPort)
{
	unsigned char data[SAFE_BUFF_LEN*5];
	unsigned short *pshort_buf = (unsigned short*)data;

	int para;
	int nTick = 0;
	int nStartIndex = nBegin;

	int nDataLen; 
	if (nBegin == nEnd) nDataLen = 1; //������	
	else		nDataLen = nEnd - nBegin;

	int nRestData = (nDataLen > 256) ? 256 : nDataLen;

	while(nRestData > 0)
	{
		nTick = 0;
		for (int i=nStartIndex; i<nStartIndex+nRestData; i++)
		{
			pshort_buf[nTick++] = pDatatoWrite[i];
		}

		para = nBlendside|(nPureWhite<<8)| (nStartIndex<<16);  //blend_side   white_sel   start_addr
		int return_back = BinCommand_RW(BINEXE_BLENDPARA_WRITE|BINCODE_FCN, para, (unsigned char *)pshort_buf, nRestData*2, nPort, 1);
		if(return_back&0x10000) //ͨ��ʧ�� 
		{	
			return CMD_FAILURE;		
		}

		nStartIndex += nRestData;
		nRestData = ((nDataLen-nStartIndex) > 256) ? 256 : (nDataLen-nStartIndex);
	}

	return CMD_RTOK;
}


//--------------------------------- �ڵ�ƽ���� ---------------------------------//

// �����ڵ�ƽ���Ի���
int CCommandClass::CmdTCRY(UINT nBlackLvl, UINT nEnable, int nPort)
{
	UINT  nParam[SAFE_BUFF_LEN] = {0};

	nParam[0] = nBlackLvl;
	nParam[1] = nEnable;

	UINT nRet = NormalASCII_RW("TCRY", nParam, 2, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ���úڵ�ƽ
int CCommandClass::CmdWBLV(UINT *pBlackLvlDataW, UINT nSegment, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	// �ڵ�ƽֵ ���ﶼ��ƫ��ֵ
	for (int iBlendzone=0; iBlendzone<13; iBlendzone++)
		nParam[iBlendzone] = pBlackLvlDataW[iBlendzone];

	//�ڵ�ƽ��
	nParam[13] = nSegment;

	UINT nRet = NormalASCII_RW("WBLV", nParam, 14, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ�ڵ�ƽ
int CCommandClass::CmdRBLV(UINT *pBlackLvlDataR, UINT nSegment, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = nSegment;

	UINT nRet = NormalASCII_RW("RBLV", nParam, 1, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	for (int iBlendzone=0; iBlendzone<13; iBlendzone++)
		pBlackLvlDataR[iBlendzone] = nParam[iBlendzone];

	return CMD_RTOK;
}

// ����©������
int CCommandClass::CmdWLKT(UINT nLeakageSel, UINT nSide1Width, UINT nSide2Width, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = nLeakageSel;
	nParam[1] = nSide1Width;
	nParam[2] = nSide2Width;

	UINT nRet = NormalASCII_RW("WLKT", nParam, 3, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ©������
int CCommandClass::CmdRLKT(UINT *pLeakageWidth, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RLKT", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pLeakageWidth, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// ���úڵ�ƽ�ε�
int CCommandClass::CmdWLVT(UINT *pBlkLvlSegmentW, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	memcpy(nParam, pBlkLvlSegmentW, sizeof(nParam));

	UINT nRet = NormalASCII_RW("WLVT", nParam, 8, nPort);

	if (nRet !=  CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ�ڵ�ƽ�ε�
int CCommandClass::CmdRLVT(UINT *pBlkLvlSegmentR, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RLVT", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pBlkLvlSegmentR, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// �����رպڵ�ƽ����
int CCommandClass::CmdWBLE(UINT nEnable, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = nEnable;

	UINT nRet = NormalASCII_RW("WBLE", nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��ȡ�ڵ�ƽ�����Ƿ���
int CCommandClass::CmdRBLE(UINT &nEnable, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = 0;

	UINT nRet = NormalASCII_RW("RBLE", nParam, 1, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	nEnable = nParam[0];

	return CMD_RTOK;
}

// ��λ�ڵ�ƽ
int CCommandClass::CmdRSTL(int nPort)
{
	UINT nRet = NormalASCII_RW("RSTL", NULL, 0, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}



//--------------------------------- ����У�� ---------------------------------//
// ��ȡ����У�����Ƶ�λ��
int CCommandClass::CmdGW4P(UINT *p4PointPos, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("GW4P", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(p4PointPos, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// ��ȡ����У�����Ƶ�λ��
int CCommandClass::CmdGW9P(UINT *pCurvePos, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("GW9P", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pCurvePos, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// ��ȡ����У�����Ƶ�λ��
int CCommandClass::CmdGW5P(UINT *pDensityPos,int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("GW5P", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pDensityPos, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

// ���ñ����Warp��
int CCommandClass::CmdSWPT(UINT nTableID, int nPort)
{
	UINT nParam = nTableID;

	UINT nRet = NormalASCII_RW("SWPT", &nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��������У������
int CCommandClass::CmdSW4P(UINT *p4PointPos, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	//{table_id,x0,y0,x1,y1,x2,y2,x3,y3}
	memcpy(nParam, p4PointPos, 9*sizeof(UINT));

	UINT nRet = NormalASCII_RW("SW4P", nParam, 9, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;
	
	return CMD_RTOK;
}

// ��������У������
int CCommandClass::CmdSW9P(UINT *pCurvePos, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	//{table_id,x0,y0,x1...x8,y8}
	memcpy(nParam, pCurvePos, 19*sizeof(UINT));

	UINT nRet = NormalASCII_RW("SW9P", nParam, 19, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

// ��������У������
int CCommandClass::CmdSW5P(UINT *pDensityPos,int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	//{table_id, x0, x1, x2, x3, x4, y0, y1, y2, y3, y4}
	memcpy(nParam, pDensityPos, 11*sizeof(UINT));

	UINT nRet = NormalASCII_RW("SW5P", nParam, 11, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//------------------------------ ͼ������ۻ� ------------------------------//
// ��ȡ�ۻ�ֵ
int CCommandClass::CmdRFLI(UINT &nFilter, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RFLI", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	nFilter = nParam[0];

	return CMD_RTOK;
}
// ���öۻ�ֵ
int CCommandClass::CmdWFLI(UINT nFilter, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = nFilter;

	UINT nRet = NormalASCII_RW("WFLI", nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//--------------------------------- ��λ���� ---------------------------------//

int CCommandClass::CmdRSTF(int nPort)
{
	UINT nRet = NormalASCII_RW("RSTF", NULL, 0, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//--------------------------------- �̻����� ---------------------------------//
int CCommandClass::CmdSALL(int nPort)
{
	UINT nRet = NormalASCII_RW("SALL", NULL, 0, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//-------------------------------- MVX303 ����ģ�� ----------------------------//
int CCommandClass::CmdRCN1(UINT *pBuf, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};

	UINT nRet = NormalASCII_RW("RCN1", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pBuf, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

int CCommandClass::CmdCALL(UINT *pSceneBufR, int nSceneID, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN*2] = {0};
	nParam[0] = nSceneID;

	UINT nRet = NormalASCII_RW("CALL", nParam, 1, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	memcpy(pSceneBufR, nParam, nRet*sizeof(UINT));

	return CMD_RTOK;
}

int CCommandClass::CmdSAVE(UINT *pSceneBufW, int nLen, int nSceneID, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN*2] = {0};
	nParam[0] = nSceneID;

	memcpy(&nParam[1], pSceneBufW, nLen*sizeof(UINT));

	UINT nRet = NormalASCII_RW("SAVE", nParam, nLen+1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

int CCommandClass::CmdOPEN(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	int i=0;
	nParam[i++] = nWinID;
	nParam[i++] = nSrcID;
	nParam[i++] = nSrcType;
	nParam[i++] = rcWnd.left;
	nParam[i++] = rcWnd.top;
	nParam[i++] = rcWnd.right;
	nParam[i++] = rcWnd.bottom;

	UINT nRet = NormalASCII_RW("OPEN", nParam, i, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

int CCommandClass::CmdMove(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	int i=0;
	nParam[i++] = nWinID;
	nParam[i++] = nSrcID;
	nParam[i++] = nSrcType;
	nParam[i++] = rcWnd.left;
	nParam[i++] = rcWnd.top;
	nParam[i++] = rcWnd.right;
	nParam[i++] = rcWnd.bottom;

	UINT nRet = NormalASCII_RW("MOVE", nParam, i, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

int CCommandClass::CmdSize(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	int i=0;
	nParam[i++] = nWinID;
	nParam[i++] = nSrcID;
	nParam[i++] = nSrcType;
	nParam[i++] = rcWnd.left;
	nParam[i++] = rcWnd.top;
	nParam[i++] = rcWnd.right;
	nParam[i++] = rcWnd.bottom;

	UINT nRet = NormalASCII_RW("SIZE", nParam, i, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}

int CCommandClass::CmdSHUT(int nWinID, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = nWinID;

	UINT nRet = NormalASCII_RW("SHUT", nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}


//-------------------------------- HDMI_3D ģ�� ----------------------------//
//��������ֱ���
int CCommandClass::CmdWOUI(int nForceTiming, int nPort)
{
	return CMD_RTOK;
}
//��ȡ����ֱ���
int CCommandClass::CmdROUI(int &nForceTiming, int &nTimingIndex, int &nHactive, int &nVactive, int nPort)
{
	return CMD_RTOK;
}
//��������3Dģʽ
int CCommandClass::CmdWI3D(int n3Dmode, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam [0] = n3Dmode;

	UINT nRet = NormalASCII_RW("WI3D", nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}
//��ȡ����3Dģʽ
int CCommandClass::CmdRI3D(int &n3Dmode, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	UINT nRet = NormalASCII_RW("RI3D", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;
	
	n3Dmode = nParam[0];

	return CMD_RTOK;
}
//�������3D����ģʽ
int CCommandClass::CmdWO3D(int n3Dmode, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = n3Dmode;

	UINT nRet = NormalASCII_RW("WO3D", nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}
//��ȡ���3D����ģʽ
int CCommandClass::CmdRO3D(int &n3Dmode, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	UINT nRet = NormalASCII_RW("RO3D", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	n3Dmode = nParam[0];

	return CMD_RTOK;
}
//���ó�Ƶ����
int CCommandClass::CmdWOHZ(int nTimes, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	nParam[0] = nTimes;

	UINT nRet = NormalASCII_RW("WOHZ", nParam, 1, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}
//��ȡ��Ƶ����
int CCommandClass::CmdROHZ(int &nTimes, int nPort)
{
	UINT nParam[SAFE_BUFF_LEN] = {0};
	UINT nRet = NormalASCII_RW("ROHZ", nParam, 0, nPort);

	if ((nRet & CMD_RTER) != 0) return CMD_FAILURE;

	nTimes = nParam[0];

	return CMD_RTOK;
}
//�������л�
int CCommandClass::CmdWLRC(int nPort)
{
	UINT nRet = NormalASCII_RW("WLRC", NULL, 0, nPort);

	if (nRet != CMD_RTOK) return CMD_FAILURE;

	return CMD_RTOK;
}