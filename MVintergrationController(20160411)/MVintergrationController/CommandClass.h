#pragma once

#include "SerialPort.h"
#include "EthernetPort.h"
#include "USBHIDPort.h"

#define SAFE_BUFF_LEN (256)

#pragma pack(push,1)
/******************************* ������ͨ�ŷ�װ �� ******************************/
class PC_Command
{

public:
	unsigned char	head1;		//֡ͷ1			1Byte
	unsigned char	head2;		//֡ͷ2			1Byte
	unsigned char	inst;		//ָ��			1Byte
	unsigned int	para;		//��ַ ����		4Byte
	unsigned short  length;		//����			2Byte
	unsigned char   data[2100];	//����			2048Byte
	unsigned char   check;		//У��			1Byte
	unsigned char   tail;		//֡β			1Byte

	void CalcCheck()
	{
		check=head1^head2^inst^(para&0xFF)^((para>>8)&0xFF)^((para>>16)&0xFF)^((para>>24)&0xFF)^(length&0xFF)^((length>>8)&0xFF)^tail;
		for(int i=0; i<length; i++)
			check=check^data[i];
	};
};

class STM_Command
{
public:
	unsigned char	head1;		//֡ͷ1			1Byte
	unsigned char	head2;		//֡ͷ2			1Byte
	unsigned char	inst;		//ָ��			1Byte
	unsigned int	para;		//��ַ ����		4Byte
	unsigned short  length;		//����			2Byte
	unsigned char   data[2100];	//����			2048Byte
	unsigned char   check;		//У��			1Byte
	unsigned char   tail;		//֡β			1Byte


	BOOL isCorrect()
	{
		unsigned char ck = 0;
		ck=head1^head2^inst^(para&0xFF)^((para>>8)&0xFF)^((para>>16)&0xFF)^((para>>24)&0xFF)^(length&0xFF)^((length>>8)&0xFF)^check^tail;

		for(int i=0; i<((length == 0x8001)?1:length); i++)
			ck=ck^data[i];
		if(0x00 == ck)
			return true;
		return false;
	}
};
#pragma pack(pop)

//******************** �����ƶ�д���� ***********
#define BINCODE_SPI             0x00 //SPI ���� 
#define BINCODE_CMD             0x03 //cmd ָ����� 
#define BINCODE_FLASH           0x02 //Flash ���� 
#define BINCODE_FCN             0x08 //���ܲ���

// ɫ�����ݼ�������
#define BINEXE_COLOR_WRITE      0x00
#define BINEXE_COLOR_READ       0x10

// �߼��ںϲ�������
#define BINEXE_BLENDPARA_WRITE  0x20
#define BINEXE_BLENDPARA_READ   0x30

// ����У�����ݼ���
#define BINEXE_WARP_ENABLE      0x80 //c786 warp execute end  or start
#define BINEXE_WARP_WRITE       0x90
#define BINEXE_WARP_SAVE        0x70  //���� Warp 4Point 9Point  5x5���ܶ� 

// ���У�����ݼ����뱣��
#define BINEXE_GRID_ENABLE      0xc0  //Grid д�� 
#define BINEXE_GRID_WRITE       0xd0  
#define BINEXE_GRID_READ        0xe0 


class CCommandClass
{
public:
	CCommandClass();

public:
	void SetInterface(CEthernetPort *pSock, CSerialPort *pCom, CUSBHIDPort *pUSB);

	//*********************** ���ư忨 *************************//
	//
	// �������
	int CmdRNET(pNetParams netparamW, int nPort=0xff); /*��ȡ�������*/
	int CmdWNET(pNetParams netparamR, int nPort=0xff); /*�����������*/

	//*********************** ���а忨ͨ������ *************************//
	//
	// ϵͳ��֤
	int CmdRMVR(int nPort=0xff);  /*��ȡ��������Ϣ*/
	int CmdRDID(int nPort=0xff);  /*��ȡ�豸ID*/
	int CmdRDFN(int nPort=0xff);  /*��ȡ�豸������*/
	int CmdRDPT(int &nBoards, int *nPortID, int nPort=0xff); /*��ȡ�豸���а忨�������忨�Ķ˿ںţ� ֻ���һ���*/

	// �ֱ���
	int CmdRINT(TIMING_STRUCT *pTsR, int nEdidPort/*EDID��*/, int nTimingIndex/*���ηֱ���*/, int nPort);  /*��ȡ����ֱ���*/
	int CmdWINT(TIMING_STRUCT *pTsW, int nEdidport, int nTimingIndex, int nPort);                          /*��������ֱ���*/
	int CmdGETR(UINT &nHz, UINT &nHtotal, UINT &nHactive, UINT &nVtatol, UINT &nVactive, int nPort); /*��ȡ��ʱ�źŵķֱ���*/
	int CmdROUT(TIMING_STRUCT *pTsR, int nPort); /*��ȡ����ֱ���*/
	int CmdWOUT(TIMING_STRUCT *pTsW, int nPort); /*��������ֱ���*/	

	//*********************** MG300 ģ�� *************************//
	//
	// ɫ��
	int CmdTCLR(pColorTestParams clrtestParam, int nPort);                  /*��ɫ����*/
	int CmdCCLR(int nPort);                                                 /*ȡ����ɫ����*/
	int CmdWCRA_ASC(UINT *pColorData, int nDataLen, int nColortype, int nPort); /*д��ɫ������*/
	int CmdRCRA_ASC(UINT *pColorData, int nColortype, int nPort);               /*��ȡɫ������*/
	int CmdWCRA_BIN(UCHAR *pColorData, int nColortype, int nLen, int nPort);
	int CmdRCRA_BIN(UCHAR *pColorData, int nColortype, int nPort);
	int CmdRSTC(UINT nColorsel, int nPort);                                 /*��λɫ��*/
	int CmdCRBD(UINT nBlendOpt, int nPort);                                 /*ȡ���ںϴ�Ӱ�� nBlendOpt:0-ȡ���ںϴ� 1-�����ںϴ� 2-�ָ��ںϴ�*/

	// �ںϴ�
	int CmdWBDP(pBlendParams pBPW, int nPort);  /*�����ںϲ���Alpha,P,Gamma����*/
	int CmdRBDP(UINT *pBlendParam, int nPort);  /*��ȡ�ںϲ���Alpha,P,Gamma*/
	int CmdWBDE(UINT nEnableW, int nPort);      /*����/�ر��ںϹ���*/
	int CmdRBDE(UINT &nEnableR, int nPort);     /*��ȡ�ںϹ����Ƿ���*/
	int CmdWBDW(UINT *pWidthW, int nPort);      /*�����ںϴ����*/
	int CmdRBDW(UINT *pWidthR, int nPort);      /*��ȡ�ںϴ����*/
	int CmdWBDT(UINT nWhiteLvlW, int nPort);    /*���ô��׷�ֵ*/
	int CmdRBDT(UINT &nWhiteLvlR, int nPort);   /*��ȡ���׷�ֵ*/
	int CmdRBPD(UINT *pDatatoRead/*���ݵ�ַ*/, int nBlendside, int nPureWhite, int nBlendWidth, int nPort); /*��ȡ�ں�ֵ*/
	int CmdWBPD(UINT *pDatatoWrite/*���ݵ�ַ*/, int nBegin/*д����ʼλ��*/, int nEnd/*д�����λ��*/, int nBlendside, int nPureWhite, int nPort); /*д���ں�ֵ*/

	// �ڵ�ƽ
	int CmdTCRY(UINT nBlackLvl/*���Ժڵ�ƽλ��*/, UINT nEnable/*�����ر�*/, int nPort);  /*�����ڵ�ƽ���Ի���*/
	int CmdWBLV(UINT *pBlackLvlDataW, UINT nSegment, int nPort);  /*д��ڵ�ƽ, ÿ���ں�����ǰ�ε�ƫ��ֵ*/
	int CmdRBLV(UINT *pBlackLvlDataR, UINT nSegment, int nPort);  /*��ȡ�ڵ�ƽ, ÿ���ں�����ǰ�ε�ƫ��ֵ*/
	int CmdWLKT(UINT nLeakageSel, UINT nSide1Width, UINT nSide2Width, int nPort);  /*����©������*/
	int CmdRLKT(UINT *pLeakageWidth, int nPort);    /*��ȡ©������*/
	int CmdWLVT(UINT *pBlkLvlSegmentW, int nPort);  /*���úڵ�ƽ�ε�ֵ*/
	int CmdRLVT(UINT *pBlkLvlSegmentR, int nPort);  /*��ȡ�ڵ�ƽ�ε�ֵ*/
	int CmdWBLE(UINT nEnable, int nPort);           /*�����رպڵ�ƽ����*/
	int CmdRBLE(UINT &nEnable, int nPort);          /*��ȡ�ڵ�ƽ�����Ƿ���*/
	int CmdRSTL(int nPort);                         /*��λ�ڵ�ƽ*/

	// ����У��
	int CmdGW4P(UINT *p4PointPos, int nPort);   /*��ȡ�ĵ�У�� ���Ƶ�λ��*/
	int CmdGW9P(UINT *pCurvePos,  int nPort);   /*��ȡ����У�� ���Ƶ�λ��*/
	int CmdGW5P(UINT *pDensityPos,int nPort);   /*��ȡ����У�� ���Ƶ�λ��*/
	int CmdSWPT(UINT nTableID,    int nPort);   /*����Warp��*/
	int CmdSW4P(UINT *p4PointPos, int nPort);   /*�����ĵ�У�� ���Ƶ�λ��*/
	int CmdSW9P(UINT *pCurvePos,  int nPort);   /*��������У�� ���Ƶ�λ��*/
	int CmdSW5P(UINT *pDensityPos,int nPort);   /*��������У�� ���Ƶ�λ��*/

	// ͼ����
	int CmdRFLI(UINT &nFilter, int nPort);  /*��ȡ�ۻ�ֵ*/
	int CmdWFLI(UINT nFilter, int nPort);   /*���öۻ�ֵ*/

	// ��λ����
	int CmdRSTF(int nPort);  /*�忨��λ*/

	// �̻�����
	int CmdSALL(int nPort);  /*�̻�����*/


	//*********************** MVX303 ��������ģ�� *************************//
	//
	int CmdRCN1(UINT *pBuf, int nPort);  //��ȡ������Ϣ
	int CmdCALL(UINT *pSceneBufR, int nSceneID, int nPort); //���ó���
	int CmdSAVE(UINT *pSceneBufW, int nLen, int nSceneID, int nPort); //���泡��
	int CmdOPEN(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort); //��������
	int CmdMove(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort); //�ƶ�����
	int CmdSize(int nWinID, CRect rcWnd, int nSrcID, int nSrcType, int nPort); //�����ߴ�
	int CmdSHUT(int nWinID, int nPort); //�رմ���

	//*********************** HDMI_3D ģ�� *************************//
	//
	int CmdWOUI(int nForceTiming, int nPort);     //��������ֱ���
	int CmdROUI(int &nForceTiming, int &nTimingIndex, int &nHactive, int &nVactive, int nPort); //��ȡ����ֱ���
	int CmdWI3D(int n3Dmode, int nPort);   //��������3Dģʽ
	int CmdRI3D(int &n3Dmode, int nPort);  //��ȡ����3Dģʽ
	int CmdWO3D(int n3Dmode, int nPort);   //�������3D����ģʽ
	int CmdRO3D(int &n3Dmode, int nPort);  //��ȡ���3D����ģʽ
	int CmdWOHZ(int nTimes, int nPort);    //���ó�Ƶ����
	int CmdROHZ(int &nTimes, int nPort);   //��ȡ��Ƶ����
	int CmdWLRC(int nPort);                //�������л�

public:
	CEthernetPort *m_pCmdSock;
	CSerialPort *m_pCmdCom;
	CUSBHIDPort *m_pCmdUSB;
	UINT NormalASCII_RW(char *cmd, UINT *cmd_para, UINT len=0, UINT port=0xff, int usbRW = 0/*0:USB�ɶ���д 1:USBֻд����*/);
	UINT BinCommand_RW(UCHAR type, UINT addr, UCHAR *data, UINT len, UCHAR port, int usbRW = 0); //������ָ��Ķ�д���� 
	BOOL ConvertSTM_Cmd(STM_Command* pCmd_STM,char *pFrame,int len);
	void ConvertPC_Cmd(PC_Command* pCmd_PC, char *pFrame, UINT &len);
};
