// MVintergrationController.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MVintergrationControllerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int g_AjustThreadWorkMode = IDLE_MODE;

// CMVintergrationControllerApp

BEGIN_MESSAGE_MAP(CMVintergrationControllerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMVintergrationControllerApp construction

CMVintergrationControllerApp::CMVintergrationControllerApp()
{
	//g_TaskNum = 0;

	//�����Ի���ͨ������
	m_DlgSetting.bkBrush.CreateSolidBrush(RGB(100,150,200));//RGB(0x88,0x22,0x55)RGB(91,154,187)
	m_DlgSetting.clrText = RGB(220,220,30);

	//��ǰͨ�����ӷ�ʽ
	m_nConnType = NON_CONNECT;

	//�������ɵĹ���ҳ������
	m_CurrentProductType = Product_INVALID;

	//����У���߳̾��
	m_ThreadAjustHandle = NULL;

	//��ʼ����Դ��
	InitializeCriticalSection(m_Lock);
}


// The one and only CMVintergrationControllerApp object

CMVintergrationControllerApp theApp;


// CMVintergrationControllerApp initialization

BOOL CMVintergrationControllerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	SetLogFilePath();

	//����У���߳�
	StartThread(AJUST_THREAD);

	CMVintergrationControllerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
	}
	else if (nResponse == IDCANCEL)
	{
	
	}

	return FALSE;
}

int CMVintergrationControllerApp::ExitInstance()
{
	ExitThread(AJUST_THREAD);

	return CWinApp::ExitInstance();
}

//����������־�ļ�·��
void CMVintergrationControllerApp::SetLogFilePath()
{
	LPCTSTR szPath = AfxGetApp()->m_pszHelpFilePath;
	char path[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char ext[_MAX_EXT];
	char fname[_MAX_FNAME];
	strcpy_s(path, _MAX_PATH, szPath);
	_splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
	m_strLogFilePath.Format(_T("%s%s"), drive, dir);
	m_strLogFilePath += _T("CommandLog.txt");
}

//д��־
void CMVintergrationControllerApp::PrintLog(char* szInfo)
{
	CStdioFile 	logFile;
	if(logFile.Open(m_strLogFilePath,CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite))
	{
		CTime t = CTime::GetCurrentTime();
		char *info = new char[256];
		sprintf_s(info, 256, "[%d:%d:%d] %s\n", t.GetHour(), t.GetMinute(), t.GetSecond(),  szInfo);
		logFile.SeekToEnd();
		logFile.Write(info, strlen(info));
		logFile.Close();
		delete info;
	}
}

//ͨ�ò˵�����
void CMVintergrationControllerApp::MvTrackPopupMenu(CPoint ptTrack, UINT nResource, CWnd *pParent)
{
	CMenu menu;
	menu.LoadMenu(nResource);

	CMenu *pSubMenu = NULL;
	pSubMenu = menu.GetSubMenu(0);

	ASSERT(pSubMenu != NULL);

	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, ptTrack.x, ptTrack.y, pParent);
}

void CMVintergrationControllerApp::PushPackage(TaskPackage pack)
{
	//�����̫�࣬У���߳�æ����������Ҫ�����������
	int nCount = m_ListTaskPack.GetCount();
	if (nCount > 20)
	{
		TaskPackage temp;
		POSITION pos;
		for (int i=nCount-1; i>=0; i--)
		{
			pos = m_ListTaskPack.FindIndex(i);
			temp = m_ListTaskPack.GetAt(pos);
			if (temp == pack)
			{
				m_ListTaskPack.RemoveAt(pos);
			}
		}		
	}

	m_ListTaskPack.AddTail(pack);
    //g_TaskNum++;
}  //���������
TaskPackage CMVintergrationControllerApp::PopPackage()
{
	return m_ListTaskPack.RemoveHead();
}    //��������
BOOL CMVintergrationControllerApp::IsTaskListEmpty()
{ 
	return m_ListTaskPack.IsEmpty();
}           //����������Ƿ�Ϊ��
int CMVintergrationControllerApp::GetTaskCount() const 
{
	return m_ListTaskPack.GetCount(); 
}

void CMVintergrationControllerApp::WaitForTaskOver()
{
	if (m_ThreadAjustHandle == NULL) return;
	PrintLog("Waiting Thread Task Over ...\r\n");

	theApp.GlobleLock();
	//�ȴ��̰߳ѵ�ǰ����ִ�����
	while(g_AjustThreadWorkMode)
	{
		Sleep(100);
	}
	theApp.GlobleUnlock();
}


/*��ȡͨ�Žӿ�ָ��*/
void CMVintergrationControllerApp::SetCmdInterface(CEthernetPort *pSock, CSerialPort*pCom, CUSBHIDPort *pUSB)
{
	ASSERT_POINTER(pSock, CEthernetPort);
	ASSERT_POINTER(pCom, CSerialPort);
	ASSERT_POINTER(pUSB, CUSBHIDPort);

	m_Cmd.SetInterface(pSock, pCom, pUSB);
}


//*****************************************************************************************
//                               ����У���߳̿�����
//-----------------------------------------------------------------------------------------
// �����̹߳��̴�����
UINT ThreadProcess_WarpGenerator(LPVOID lpvoid);

// �����߳�
BOOL CMVintergrationControllerApp::StartThread(UINT nThreadID)
{
	if (nThreadID == AJUST_THREAD)
	{
		if (m_ThreadAjustHandle != NULL) return TRUE;

		g_AjustThreadWorkMode = IDLE_MODE;

		m_ThreadAjustHandle = AfxBeginThread(ThreadProcess_WarpGenerator, NULL );

		if (m_ThreadAjustHandle == NULL) return FALSE;

		SuspendThread(AJUST_THREAD);

		return TRUE;
	}

	return FALSE;
}

// �ָ��߳�����
void CMVintergrationControllerApp::ResumeThread(UINT nThreadID)
{
	switch(nThreadID)
	{
	case AJUST_THREAD:
		if (m_ThreadAjustHandle != NULL)	m_ThreadAjustHandle->ResumeThread();
		break;
	default: break;
	}

}

// �����߳�
void CMVintergrationControllerApp::SuspendThread(UINT nThreadID)
{
	switch(nThreadID)
	{
	case AJUST_THREAD:
		if (m_ThreadAjustHandle != NULL)	m_ThreadAjustHandle->SuspendThread();
		break;
	default: break;
	}
}

// �˳��߳�
void CMVintergrationControllerApp::ExitThread(UINT nThreadID)
{
	switch(nThreadID)
	{
	case AJUST_THREAD:
		if (m_ThreadAjustHandle != NULL)
		{
			ResumeThread(nThreadID);

			GlobleLock();
			g_AjustThreadWorkMode = EXIT_MODE;
			GlobleUnlock();

			Sleep(500);

			m_ThreadAjustHandle = NULL;
		}
		break;
	default: break;
	}
}


// �̵߳��ù��̺���, �벻ҪWhileѭ����ʹ��Return��� ��������������
UINT ThreadProcess_WarpGenerator(LPVOID lpvoid)
{
	//�߳��ڲ�����������ģʽ��־
	int RuningMode = IDLE_MODE;

	//�洢��һ��ִ�е������
	TaskPackage LastPack;
	memset(&LastPack, 0, sizeof(TaskPackage));

	//�߳����ݻ���
	CDoublePoint Array4point[17*17];
	CDoublePoint ArrayCurve[17*17];
	CDoublePoint ArrayDensity[17*17];

	while(TRUE)
	{
		// ��ȡ�߳�����ģʽ
		theApp.GlobleLock();
		RuningMode = g_AjustThreadWorkMode;
		theApp.GlobleUnlock();

		if (RuningMode == IDLE_MODE) 
		{	// ����ģʽ

			Sleep(10);
			continue;
		}
		else if (RuningMode == EXIT_MODE) 
		{	// �˳�ģʽ

			break; 
		}
		else if (RuningMode == WORK_MODE_AJUST) 
		{   // ����ģʽ			

			// ��Դ��
			theApp.GlobleLock(); 

			// û�����
			if (theApp.IsTaskListEmpty()) 
			{ 
				g_AjustThreadWorkMode = IDLE_MODE; //���ڿ���Ъ����
				theApp.GlobleUnlock(); //�����Դ��
				continue; 
			}

			// ׼��һ����װ��
			TaskPackage TempPack;

			// �Ӹ���ɸ�
			TempPack = theApp.PopPackage();
			int cx = TempPack.cx;
			int cy = TempPack.cy;
			
			// ��ȡ�ֳ������
			CDoublePointArray* p4Points        = (CDoublePointArray*)TempPack.pDpa4pointData;
			CDoublePointArray* pCurvePoints    = (CDoublePointArray*)TempPack.pDpaCurveData;
			CDoublePointArray* pDensityPoints  = (CDoublePointArray*)TempPack.pDpaDensityData;
			CDoublePointArray* pGridWarpPoints = (CDoublePointArray*)TempPack.pDpaComplexData;
			CDoublePointArray* pOrigPoints     = (CDoublePointArray*)TempPack.pDpaOrigData;

			// ���ٻ���
			CDoublePoint *InputGridOriginalPoint = new CDoublePoint[cx*cy];
			CDoublePoint *OutputGridPointMid     = new CDoublePoint[cx*cy];
			CDoublePoint *OutputGridPoint;
			if (TempPack.nAjustMode == GRIDWARP_AJUST) OutputGridPoint = new CDoublePoint[65*65];
			else OutputGridPoint = new CDoublePoint[cx*cy];

			// �Ƚ����̴߳������������ϴε������Ƿ���ͬ������ͬ����п���
			BOOL bDataChanged = FALSE;

			if (TempPack.nAjustMode == KEYSTONE_AJUST 
			 || TempPack.nAjustMode == CURVE_AJUST
			 || TempPack.nAjustMode == DENSITY_AJUST)
			{
				if (memcmp(Array4point, p4Points->m_pPoints, sizeof(CDoublePoint)*cx*cy) != 0
				 || memcmp(ArrayCurve, pCurvePoints->m_pPoints, sizeof(CDoublePoint)*cx*cy) != 0
				 || memcmp(ArrayDensity, pDensityPoints->m_pPoints, sizeof(CDoublePoint)*cx*cy) != 0)
				{
					bDataChanged = TRUE;
					memcpy(Array4point, p4Points->m_pPoints, sizeof(CDoublePoint)*cx*cy);
					memcpy(ArrayCurve,  pCurvePoints->m_pPoints, sizeof(CDoublePoint)*cx*cy);
					memcpy(ArrayDensity,pDensityPoints->m_pPoints, sizeof(CDoublePoint)*cx*cy);
					for (int y=0; y<cy; y++)
						for (int x=0; x<cx; x++)
						{
							InputGridOriginalPoint[x+y*cx].x = double(TempPack.cxResIn*x) / (cx-1);
							InputGridOriginalPoint[x+y*cx].y = double(TempPack.cyResIn*y) / (cy-1);
						}
				}
			}
			else if(TempPack.nAjustMode == GRIDWARP_AJUST)
			{
				if (!ConvertGridPointData(cx, cy, pGridWarpPoints->m_pPoints, OutputGridPoint))
				{
					// �ͷ���Դ
					delete InputGridOriginalPoint; InputGridOriginalPoint = NULL;
					delete OutputGridPointMid; OutputGridPointMid = NULL;
					delete OutputGridPoint; OutputGridPoint = NULL;

					theApp.GlobleUnlock();
					continue;
				}

				bDataChanged = TRUE;
			}			

			// �����Դ��
			theApp.GlobleUnlock();

			// �������������ϴε���ͬ����������һ��
			if (bDataChanged != TRUE)
			{
				// �ͷ���Դ
				delete InputGridOriginalPoint; InputGridOriginalPoint = NULL;
				delete OutputGridPointMid; OutputGridPointMid = NULL;
				delete OutputGridPoint; OutputGridPoint = NULL;

				continue;
			}

			// ��ʼ�����������
			int eleNum = (1200/16+1)*(2048/16+1)+128; //16x16��������������
			int index = 0x00;
			CDoublePoint *FinalGridPoint = new CDoublePoint[eleNum];
			for(int yn =0;yn<=1200;yn+=16)	{
				for(int xn=0;xn<=2048;xn+=16)
				{
					FinalGridPoint[index].x = xn;
					FinalGridPoint[index].y = yn;
					index++;
				}
			}

			if (TempPack.nAjustMode == KEYSTONE_AJUST 
			 || TempPack.nAjustMode == CURVE_AJUST
			 || TempPack.nAjustMode == DENSITY_AJUST)
			{
				/************����Warp��Keystone�е������*********/
				Calc_Keystone_InToOut_Table(cx,
											cy,
											InputGridOriginalPoint,
											Array4point,
											cx*cy,
											ArrayCurve,
											OutputGridPointMid);

				/*************�������е�����������**************/
				int nMaxTimes = 10; //����
				//��һ��
				UINT min_err_time = 0;
				Calc_Warp_Table(nMaxTimes,
								cx*cy,
								InputGridOriginalPoint,
								OutputGridPointMid,
								&min_err_time,
								cx*cy,
								ArrayDensity,
								OutputGridPoint);

				//�����ο�������
				for(int i=0;i<cx*cy;i++)
				{
					pOrigPoints->m_pPoints[i].x = OutputGridPoint[i].x;
					pOrigPoints->m_pPoints[i].y = OutputGridPoint[i].y;
				}

				//�ڶ���
				UINT min_err_time2 = 0;
				Calc_Warp_Table(nMaxTimes,
								cx*cy,
								OutputGridPoint,
								InputGridOriginalPoint,
								&min_err_time2,
								eleNum,
								FinalGridPoint,
								FinalGridPoint);
			}
			else if (TempPack.nAjustMode == GRIDWARP_AJUST)
			{
				if(TempPack.nGridSmooth == 0) //ֱ����������ȡ
				{
					Calc_Point_Table_dir(65,
						                 65,
										 TempPack.cxResIn,
										 TempPack.cyResIn,
						                 OutputGridPoint,
										 FinalGridPoint );
				}
				else  //����㷨��ȡ
				{
					CDoublePoint *InputGridOriginalPoint_Mul = new CDoublePoint[65*65];
					for( int y=0; y<65; y++) {
						for( int x=0; x<65; x++)
						{
							InputGridOriginalPoint_Mul[x+y*65] = CDoublePoint(TempPack.cxResIn/64.0*x, TempPack.cyResIn/64.0*y);			
						}
					}

					unsigned int min_err_time3=0;  
					Calc_Warp_Table(5,
						            65*65,
									OutputGridPoint,
									InputGridOriginalPoint_Mul,
						            &min_err_time3,
									eleNum,
									FinalGridPoint,
									FinalGridPoint );

					delete InputGridOriginalPoint_Mul;
				}
			}

			/*************����Warp���ݼ��ع���****************/
			UINT nRet = 0xffff;     //����ش���
			UCHAR Data[2100] = {0}; //���ݻ���
			UCHAR Warp_Table_ID = 0;//���صı�� ��0,1,2��
			UINT Warp_Mode = (TempPack.nAjustMode == GRIDWARP_AJUST ? 0x80 : 0x00);

			memcpy(Data,   &TempPack.cxResIn, 2);
			memcpy(Data+2, &TempPack.cyResIn, 2);

			theApp.GlobleLock();
			nRet = theApp.m_Cmd.BinCommand_RW(BINEXE_WARP_ENABLE|BINCODE_CMD,
                                              0x01|((Warp_Table_ID<<8)|(Warp_Mode<<16)),
                                              Data,
                                              4,
                                              TempPack.nBoardPort,
											  1);
			theApp.GlobleUnlock();

			if(( nRet & 0x10000)                    /*ͨ��ʧ��*/
			|| ((nRet == 0x01)&&(Data[0] == 0xff))  /*ָ����Ч*/
			|| ( nRet == 0x04) )                    /*������ķֱ����ѱ�*/
			{
				// �ͷ���Դ
				delete InputGridOriginalPoint; InputGridOriginalPoint = NULL;
				delete OutputGridPointMid; OutputGridPointMid = NULL;
				delete OutputGridPoint; OutputGridPoint = NULL;
				delete FinalGridPoint; FinalGridPoint = NULL;

				if (nRet == 0x04) AfxMessageBox(_T("�豸������ֱ����Ѹı䣬�����������豸�����ԣ�"), MB_SYSTEMMODAL|MB_OK|MB_ICONSTOP);

				continue;
			}

			/*************����wfd���� ��C786�ڲ�ʹ������********/
			int nDataIndex = 0;
			int loop = 0;
			int Y_Num = (TempPack.cyResIn+15)/16 + 1; //1200/16;
			int X_Num = (TempPack.cxResIn+15)/16 + 1; //2048/16;
			short output_x;
			short output_y;
			int connect_err = 0;

			for(int j=0;j<Y_Num;j++)
			{	
				if(connect_err == 1)
					break;

				for(int i=0;i<X_Num;i++)
				{
					output_x = (short)((FinalGridPoint[j*2064/16+i].x*16+0.5)*TempPack.cxScaleIn);
					output_y = (short)((FinalGridPoint[j*2064/16+i].y*16+0.5)*TempPack.cyScaleIn);

					Data[nDataIndex++] =  output_x&0xff;
					Data[nDataIndex++] = (output_x>>0x08)&0xff;
					Data[nDataIndex++] =  output_y&0xff;
					Data[nDataIndex++] = (output_y>>0x08)&0xff;

					if (i == X_Num-1)
					{
						Sleep(1);
					}

					if(nDataIndex == 2048)
					{
						nDataIndex = 0;

						theApp.GlobleLock();
						nRet = theApp.m_Cmd.BinCommand_RW(BINEXE_WARP_WRITE|BINCODE_SPI,2048*loop, Data, 2048, TempPack.nBoardPort, 1);
						theApp.GlobleUnlock();

						if(nRet&0x10000)//ͨ��ʧ�� 
						{
							connect_err = 1;
							break;
						}

						loop++;
					}
				}		
			}

			if( (nDataIndex != 0)&&(connect_err != 1) )//֮ǰ��ͨ��û�г���
			{
				theApp.GlobleLock();
				theApp.m_Cmd.BinCommand_RW(BINEXE_WARP_WRITE|BINCODE_SPI,2048*loop, Data, nDataIndex, TempPack.nBoardPort, 1); 
				theApp.GlobleUnlock();

				loop++;
			}

			/*************�ر�MG300 Warp Table download����********/
			theApp.GlobleLock();
			theApp.m_Cmd.BinCommand_RW( BINEXE_WARP_ENABLE|BINCODE_CMD,0x00|(Warp_Table_ID<<0x08), Data, 0x01, TempPack.nBoardPort, 1);
			theApp.GlobleUnlock();


			/*************����Warp keystone/curve/density ����*****/
			if (TempPack.nAjustMode == KEYSTONE_AJUST
			 || TempPack.nAjustMode == CURVE_AJUST
			 || TempPack.nAjustMode == DENSITY_AJUST)
			{
				unsigned short *pshort_buf;
				int i,x,y;
				pshort_buf = (unsigned short *)Data;

				for(i=0;i<4;i++)
				{
					x = (cx-1)*(i%2);
					y = (cy-1)*(i/2);
					pshort_buf[i*2+0] = (unsigned short)(Array4point[x+y*cx].x*16);     
					pshort_buf[i*2+1] = (unsigned short)(Array4point[x+y*cx].y*16); 
				}

				for(i=0;i<9;i++)
				{
					x = (cx-1)*(i%3)/2;
					y = (cy-1)*(i/3)/2;
					pshort_buf[8+i*2+0] = (unsigned short)(ArrayCurve[x+y*cx].x*16);     
					pshort_buf[8+i*2+1] = (unsigned short)(ArrayCurve[x+y*cx].y*16); 
				}

				for(i=0;i<5;i++)
				{
					x = (cx-1)*i/4;
					y = (cy-1)/2;
					pshort_buf[8+18+i] = (unsigned short)(ArrayDensity[x+y*cx].x*16); 

					x = (cx-1)/2;
					y = (cy-1)*i/4;
					pshort_buf[8+18+5+i] = (unsigned short)(ArrayDensity[x+y*cx].y*16); 
				}

				theApp.GlobleLock();
				theApp.m_Cmd.BinCommand_RW(BINEXE_WARP_SAVE|BINCODE_CMD, 0x00, Data, (8+18+10)*2, TempPack.nBoardPort, 1); 
				theApp.GlobleUnlock();
			}

			/*************���һ����������һ��***************/
			// �ͷ���Դ
			delete InputGridOriginalPoint; InputGridOriginalPoint = NULL;
			delete OutputGridPointMid; OutputGridPointMid = NULL;
			delete OutputGridPoint; OutputGridPoint = NULL;
			delete FinalGridPoint; FinalGridPoint = NULL;
		}

	}

	return 0x78;
}