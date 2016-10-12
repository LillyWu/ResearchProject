// ACQXCORRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ACQXCORR.h"
#include "ACQXCORRDlg.h"
#include "stdio.h"
#include "iostream.h"
#include "time.h"
#include "math.h"
#include "stdlib.h"
#include "windows.h"
#include "cstring"
#include "SignalProcess.h"
  
#pragma comment(lib, "TdecApi.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACQXCORRDlg dialog

CACQXCORRDlg::CACQXCORRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACQXCORRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACQXCORRDlg)
	m_times = 0;
	m_time = 0;
	m_filename = _T("");
	m_Amp = 0.0;
	m_Pulse = 0.0;
	m_SignalFileName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

		// �������
	 m_SampleRateIdex=0;
	 m_SampleRate=0;
	 m_SampleNum=0;
	 m_gain=0;
	 m_trigSources=0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CACQXCORRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACQXCORRDlg)
	DDX_Control(pDX, IDC_FILTER, m_filter);
	DDX_Control(pDX, IDC_SAVECH8, m_savech8);
	DDX_Control(pDX, IDC_SAVECH7, m_savech7);
	DDX_Control(pDX, IDC_SAVECH6, m_savech6);
	DDX_Control(pDX, IDC_SAVECH5, m_savech5);
	DDX_Control(pDX, IDC_SAVECH4, m_savech4);
	DDX_Control(pDX, IDC_SAVECH3, m_savech3);
	DDX_Control(pDX, IDC_SAVECH2, m_savech2);
	DDX_Control(pDX, IDC_SAVECH1, m_savech1);
	DDX_Control(pDX, IDC_CH1, m_CH1);
	DDX_Control(pDX, IDC_CH2, m_CH2);
	DDX_Control(pDX, IDC_CH3, m_CH3);
	DDX_Control(pDX, IDC_CH4, m_CH4);
	DDX_Control(pDX, IDC_CH5, m_CH5);
	DDX_Control(pDX, IDC_CH6, m_CH6);
	DDX_Control(pDX, IDC_CH7, m_CH7);
	DDX_Control(pDX, IDC_CH8, m_CH8);
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Text(pDX, IDC_TIMES, m_times);
	DDX_Text(pDX, IDC_TIME, m_time);
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	DDX_Text(pDX, IDC_AMP, m_Amp);
	DDX_Text(pDX, IDC_PULSE, m_Pulse);
	DDX_Text(pDX, IDC_SIGNALFILENAME, m_SignalFileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACQXCORRDlg, CDialog)
	//{{AFX_MSG_MAP(CACQXCORRDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_INIT, OnInit)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACQXCORRDlg message handlers

BOOL CACQXCORRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	int iReturnValue;

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

// ���ñ����С
	m_Font.CreatePointFont(200, "Arial", NULL);  
    m_title.SetFont(&m_Font,true);  
    m_title.SetWindowText("PCI-10024���ݲɼ������ݲɼ�"); 
// ���öԻ������	
	SetWindowText("�źŲɼ����������");

	//���״̬��
    m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, CRect(0,0,500,0), this, 0); 
    int strPartDim[3]= {300, 500, -1}; //�ָ�����
    m_StatusBar.SetParts(3, strPartDim);

    //����״̬���ı�
    m_StatusBar.SetText("����һ", 0, 0);
    m_StatusBar.SetText("������", 1, 0);
    m_StatusBar.SetText("������", 2, 0);

// �źŷ�������������  ���� �����ظ����� �ļ���




	//�����ն�̬���ӿ�
	m_hInstTopdll = LoadLibrary("TWaveApi.dll");
	m_pOpenTopDll = (s_lpfnOpenTopDll)GetProcAddress(m_hInstTopdll, "OpenTopdll");
    m_pCloseTopDll = (s_lpfnCloseTopDll)GetProcAddress(m_hInstTopdll, "CloseTopdll");
	m_pSysInit_ = (lpfnSysInit_)GetProcAddress(m_hInstTopdll, "SysInit_");
	m_pSendHandle_ = (lpfnSendHandle_)GetProcAddress(m_hInstTopdll, "SendHandle_");
	m_pSetHardware_ = (lpfnSetHardware_)GetProcAddress(m_hInstTopdll, "SetHardware_");
	m_pStartDA_ = (lpfnStartDA_)GetProcAddress(m_hInstTopdll, "StartDA_");
	m_pStopDA_ = (lpfnStopDA_)GetProcAddress(m_hInstTopdll, "StopDA_");
	m_pMTrg_ = (lpfnMTrg_)GetProcAddress(m_hInstTopdll, "MTrg_");
	m_pCheckStatus_ = (lpfnCheckStatus_)GetProcAddress(m_hInstTopdll, "CheckStatus_");
	m_pSetFuncOut_ = (lpfnSetFuncOut_)GetProcAddress(m_hInstTopdll, "SetFuncOut_");
	m_pIOOut_ = (lpfnIOOut_)GetProcAddress(m_hInstTopdll, "IOOut_");
	m_pIOIn_ = (lpfnIOIn_)GetProcAddress(m_hInstTopdll, "IOIn_");
	m_pGetGain_ = (lpfnGetGain_)GetProcAddress(m_hInstTopdll, "GetGain_");
	m_pGetZero_ = (lpfnGetZero_)GetProcAddress(m_hInstTopdll, "GetZero_");
	m_pSetGain_ = (lpfnSetGain_)GetProcAddress(m_hInstTopdll, "SetGain_");
	m_pSetZero_ = (lpfnSetZero_)GetProcAddress(m_hInstTopdll, "SetZero_");

	int CardNum = m_pOpenTopDll();

	CString s1;
	s1.Format("��ǰϵͳ��TWave20612��Ŀ: %d",CardNum);
    m_StatusBar.SetText(s1, 0, 0);

	if(CardNum > 0)
	{
		iReturnValue = m_pSysInit_();
	}
	else
	{
		MessageBox("�򿪶�̬��ʧ�ܣ�");
		return FALSE;
	}
	m_bStop = FALSE;

	m_Pulse = 1000;

	m_Amp = 0.7;


	m_SignalFileName  = "lfm0_0_1_3k_0.1.bin";

	m_TriggerFileName  = "square0_0.02_0.2.bin";


	// ����combobox��ֵ  ����Ƶ��  �����ѹ����   ������ʽ


	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("100kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("80kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("50kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("25kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("12.5kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("10kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("5kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("2.5kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("2kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("1kHz");
	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->AddString("500Hz");

	((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->SetCurSel(2);


	((CComboBox*)GetDlgItem(IDC_GAIN))->AddString("10v");
	((CComboBox*)GetDlgItem(IDC_GAIN))->AddString("500mv");
	((CComboBox*)GetDlgItem(IDC_GAIN))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_TRIGGER))->AddString("INTERNAL");
	((CComboBox*)GetDlgItem(IDC_TRIGGER))->AddString("MANUAL");
	((CComboBox*)GetDlgItem(IDC_TRIGGER))->AddString("EXTERN");  // �ⴥ��  ����ͬ��
	((CComboBox*)GetDlgItem(IDC_TRIGGER))->SetCurSel(2);
	
	//�Ƿ�FFT��ʾѡ��
	((CButton *)GetDlgItem(IDC_FFTDIS))->SetCheck(TRUE);

	// FFTͨ��ѡ�� ��ʼ��
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH1");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH2");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH3");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH4");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH5");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH6");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH7");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->AddString("CH8");
	((CComboBox*)GetDlgItem(IDC_FFTCH))->SetCurSel(1);


	// �Ƿ񱣴��ļ�ѡ��
	((CButton *)GetDlgItem(IDC_SAVEFILE))->SetCheck(FALSE);


// �ļ�����
    txtcount = 1;
	
	TopHandle = new TopDllEx();

	SysNumber=0;
    SysNum=0;
	ReturnVal=0;

	//�򿪶�̬��	
	char buffer2[20];
	ZeroMemory(buffer,128);
	ZeroMemory(buffer2,20);

	SysNum = TopHandle->OpenTopDll();
	itoa(SysNum,buffer2,10);
	strcpy(buffer,"ϵͳ��ĿΪ��");
    strcat (buffer,buffer2);

    //ϵͳ�Լ�
	if (SysNum < 1)
	{
		MessageBox("�򿪶�̬��ʧ�ܣ�");
		return false	;
	}
    for(int i=0;i<SysNum;i++)
	{
		 SysBlock[i].CardNum=TopHandle->GetCardNum(i);
		 	//ϵͳ��Ϣ��ʼ��
	     for (unsigned int j=0;j<SysBlock[i].CardNum;j++)
		 {
			 SysBlock[i].pUserBuffer[j]=NULL;	//(unsigned long *)pDataBuf; //pTempUserBuffer;  //������ڴ�ָ��	
			 SysBlock[i].UserBufferLength[j]=0;	//128*1024*sizeof(int);  //256*1024;	 //������ڴ�ָ�볤��(�ֽ�)

		 }    
		 ReturnVal=TopHandle->SysInit(i,&SysBlock[i],1);
		 	
		 if(ReturnVal==0xff)
			AfxMessageBox("�豸����ʧ��!");
		 SysNumber +=  SysBlock[i].CardNum;
	}  
	

	s1.Format("��ɼ�����: %d",SysNumber);
    m_StatusBar.SetText(s1, 1, 0);

	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CACQXCORRDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CACQXCORRDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CACQXCORRDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CACQXCORRDlg::OnInit() 
{
	// TODO: Add your control notification handler code here

	UpdateData(true); 
// �źŷ���������


//	m_Pulse = GetDlgItemInt(IDC_PULSE);

//	m_Amp = GetDlgItemInt(IDC_AMP);


	pBuf1 = NULL;
	pBuf2 = NULL;


	m_times=0;
	m_SampleRateIdex=((CComboBox*)GetDlgItem(IDC_SAMPLERATE))->GetCurSel();
	m_gain=((CComboBox*)GetDlgItem(IDC_GAIN))->GetCurSel();
	m_trigSources=((CComboBox*)GetDlgItem(IDC_TRIGGER))->GetCurSel();
	m_time = GetDlgItemInt(IDC_TIME);
	m_times = GetDlgItemInt(IDC_TIMES);

	// ���Ƶ����ʾͨ������
	m_SaveFileIdex = ((CButton *)GetDlgItem(IDC_SAVEFILE))->GetCheck();
	m_FftDisplyIdex = ((CButton *)GetDlgItem(IDC_FFTDIS))->GetCheck();

	m_fft_index = ((CComboBox*)GetDlgItem(IDC_FFTCH))->GetCurSel();


	
	if(m_SampleRateIdex==0)
	{
		m_SampleNum=m_time*100;
	    m_SampleRate=100000; // 100k
	}
	else if(m_SampleRateIdex==1)
	{
		m_SampleNum=m_time*80;
		m_SampleRate=80000;// 80k200
	}
	else if(m_SampleRateIdex==2)
	{
	    m_SampleNum=m_time*50;
		m_SampleRate=50000;// 50k
	}
	else if(m_SampleRateIdex==3)
	{
		m_SampleNum=m_time*25;
		m_SampleRate=25000;// 25k
	}
	else if(m_SampleRateIdex==4)
	{
		m_SampleNum=m_time*12.5;
		m_SampleRate=12500;// 12.5k
	}
	else if(m_SampleRateIdex==5)
	{
		m_SampleNum=m_time*10;
		m_SampleRate=10000; // 10k
	}
	else if(m_SampleRateIdex==6)
	{
	  	m_SampleNum=m_time*5;
		m_SampleRate=5000;// 5k
	}
	else if(m_SampleRateIdex==7)
	{
	    m_SampleNum=m_time*2.5;
		m_SampleRate=2500; // 2.5k
	}
	else if(m_SampleRateIdex==8)
	{
	    m_SampleNum=m_time*2;
		m_SampleRate=2000; // 2k
	}
	else if(m_SampleRateIdex==9)
	{
        m_SampleNum=m_time*1;
		m_SampleRate=1000; // 1k
	}
	else if(m_SampleRateIdex==9)
	{
        m_SampleNum=m_time*0.5;
		m_SampleRate=500; // 500
	}

	int length;
	memset(filename,0,40);
	length = m_filename.GetLength() + 1;
	strncpy(filename,m_filename,length);


	UpdateData(false);
		
}



bool CACQXCORRDlg::DoEvents()
{ 
  MSG msg; 
     
  while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  { 
     if(msg.message==WM_QUIT)
     { 
     	return FALSE; 
     }
     TranslateMessage(&msg); 
     DispatchMessage(&msg); 
  } 
  return TRUE; 
 } 

void CACQXCORRDlg::StartSignal() 
{
	// TODO: Add your control notification handler code here

	int ii;
	int iReturnValue;
	UINT iFileLength[2];
	double *pBuf1 = new double[65536*2];
	double *pBuf2 = new double[65536*2];


	ZeroMemory(pBuf1,65536*2*sizeof(double));
	ZeroMemory(pBuf2,65536*2*sizeof(double));

	ZeroMemory(iFileLength,2*sizeof(UINT));

	UpdateData(true);
	m_bStop = FALSE;

	//1 ͨ����ȡ�����ļ�
	m_Card1File.Open(m_SignalFileName,CFile::modeRead);
	if(m_Card1File.GetLength() / 8 > 131072)
	{
		MessageBox("�����ļ����ȳ�����󳤶�128kB!");
		return;
	}
	else
	{
		iFileLength[0] = (int)(m_Card1File.GetLength());
		m_Card1File.ReadHuge(pBuf1,iFileLength[0]);
	}
	m_Card1File.Close();
	//2 ͨ����ȡ���������ź������ļ�
	m_Card2File.Open(m_TriggerFileName,CFile::modeRead);
	if(m_Card2File.GetLength() / 8 > 131072)
	{
		MessageBox("�����ļ����ȳ�����󳤶�128kB!");
		return;
	}
	else
	{
		iFileLength[1] = (int)(m_Card2File.GetLength());
		m_Card2File.ReadHuge(pBuf2,iFileLength[1]);
	}
	m_Card2File.Close();




	for(ii = 0;ii<ChNum;ii++)
	{
		if(ii == 0) // 1 Ϊ�������ģʽ Ϊ�����ֶ�ͬ������ģʽ ������ʱ��
		{
			m_pwvinfo[ii].iWaveType = 0xff;   //�Զ������� ���Ⲩ����
			m_pwvinfo[ii].iWaveLen = iFileLength[ii] / 8 + 128;  //�65536
			m_pwvinfo[ii].fWaveFreq = 100000;   //ת����  ÿ��ת���������� С��20M
			m_pwvinfo[ii].fWaveAmp = 1;   // ���η��� ���Ⲩ�����ֻ��Ϊ��1 2 5 10
			m_pwvinfo[ii].fWaveBase = 0; //  ƫ��
			m_pwvinfo[ii].fScale = 0.9;  // ����ռ�ձ� 0-1֮��
			m_pwvinfo[ii].iAMFM_FreqBS = 2;  //�ز�Ƶ�ʱ��������ڲ���Ϊ������Ƶ����Ч��������ڵ�����


			m_pwkmod[ii].TrgMode = (BYTE)2;      //������ʽ��0: �����ֶ�����;   1:�����ֶ�ͬ��;  2:�����ⴥ��; 3:�����ֶ�����; 4�����ֶ�ͬ��; 5:�����ⴥ����
			m_pwkmod[ii].MulTrg = (BYTE)1;       // 0: ���δ���; 1:��EnSingle=0ʱΪ��δ���
			m_pwkmod[ii].ClkSource = (BYTE)0;    // ʱ��Դ0 ������ʱ��  1 ������ʱ�� 2 ������ʱ��  ԭ��Ϊ0 �����޸�Ϊ2
			m_pwkmod[ii].EnSingle = (BYTE)0;  // ���ģʽ 0 ����ģʽ 1 ����ģʽ
			m_pwkmod[ii].CHConnect = (BYTE)0;  //ͨ��ģʽ��0 ��ͨ������ 1 CH0 CH1�����Ҵ�CH0���
			m_pwkmod[ii].IntEn = (BYTE)0;   //�ж�ģʽ�� 0 ��ʹ���ж� 1 ʹ���ж�
		}
		if(ii == 1)  //2Ϊ ����ͨ�� Ϊ����ͬ������ģʽ ������ʱ��
		{
			m_pwvinfo[ii].iWaveType = 0xff;   //�Զ������� ���Ⲩ����
			m_pwvinfo[ii].iWaveLen = iFileLength[ii] / 8 + 128;  //�65536
			m_pwvinfo[ii].fWaveFreq = 100000;   //ת����  ÿ��ת���������� С��20M
			m_pwvinfo[ii].fWaveAmp = 5;   // ���η��� ���Ⲩ�����ֻ��Ϊ��1 2 5 10
			m_pwvinfo[ii].fWaveBase = 0; //  ƫ��
			m_pwvinfo[ii].fScale = 0.9;  // ����ռ�ձ� 0-1֮��
			m_pwvinfo[ii].iAMFM_FreqBS = 2;  //�ز�Ƶ�ʱ��������ڲ���Ϊ������Ƶ����Ч��������ڵ�����


			m_pwkmod[ii].TrgMode = (BYTE)3;      // ������ʽ��0: �����ֶ�����;   1:�����ֶ�ͬ��;  2:�����ⴥ��; 3:�����ֶ�����; 4�����ֶ�ͬ��; 5:�����ⴥ���������ֶ�ͬ��
			m_pwkmod[ii].MulTrg = (BYTE)1;       // 0: ���δ���; 1:��EnSingle=0ʱΪ��δ���
			m_pwkmod[ii].ClkSource = (BYTE)0;    // ʱ��Դ0 ������ʱ��  1 ������ʱ�� 2 ������ʱ��
			m_pwkmod[ii].EnSingle = (BYTE)0;  // ���ģʽ 0 ����ģʽ 1 ����ģʽ
			m_pwkmod[ii].CHConnect = (BYTE)0;  //ͨ��ģʽ��0 ��ͨ������ 1 CH0 CH1�����Ҵ�CH0���
			m_pwkmod[ii].IntEn = (BYTE)0;   //�ж�ģʽ�� 0 ��ʹ���ж� 1 ʹ���ж�
		}
	}




	//���ý�Ҫ��������Ⲩ����
	for(ii=0;ii<m_pwvinfo[0].iWaveLen*2;ii++)
	{
		*(pBuf1+ii) = m_Amp*(*(pBuf1+ii));
		*(pBuf2+ii) = 5*(*(pBuf2+ii));
	}


//	for(ii = 0;ii<ChNum;ii++)
//	{
		iReturnValue = m_pSetHardware_(0,0, &m_pwkmod[0], &m_pwvinfo[0], pBuf1);
		iReturnValue = m_pSetHardware_(0,1, &m_pwkmod[1], &m_pwvinfo[1], pBuf2);

//	}  // ����ֵΪ0 ��ʾ�ɹ�



    //iReturnValue = m_pStartDA_(2,0xff);   // �����豸 0 ��ʾ�ɹ�


	iReturnValue = m_pStartDA_(0,1);
	iReturnValue = m_pStartDA_(0,0);

	CString s1;

	s1.Format("�źŷ���������״̬: %d",iReturnValue);
	m_StatusBar.SetText(s1, 2, 0);




 // ʵ�������������������ź�




}

void CACQXCORRDlg::Start()
{
    //UpdateData(true);    // �¼ӵ�Ϊ�����ݿ��ڲɼ���ͬʱ������ѡ��FFT��ʾ�����ݱ���
    int ReturnVall;
    double r[CHANNEL];
    int *Buf=new int[m_SampleNum*SysBlock[0].CardNum*SysBlock[0].ChperTB];

	 BufData1=new double[m_SampleNum];	 	memset(BufData1,0,m_SampleNum*sizeof(double));
     BufData2=new double[m_SampleNum];	 	memset(BufData2,0,m_SampleNum*sizeof(double));
     BufData3=new double[m_SampleNum];	 	memset(BufData3,0,m_SampleNum*sizeof(double));
     BufData4=new double[m_SampleNum];	 	memset(BufData4,0,m_SampleNum*sizeof(double));
	 BufData5=new double[m_SampleNum];	 	memset(BufData5,0,m_SampleNum*sizeof(double));
	 BufData6=new double[m_SampleNum];	 	memset(BufData6,0,m_SampleNum*sizeof(double));
	 BufData7=new double[m_SampleNum];	 	memset(BufData7,0,m_SampleNum*sizeof(double));
	 BufData8=new double[m_SampleNum];	 	memset(BufData8,0,m_SampleNum*sizeof(double));

	 FftResult = new double[4*m_SampleNum];   memset(FftResult,0,4*m_SampleNum*sizeof(double));

    

			//����Ӳ��
	ZeroMemory(CtrEx, sizeof(CtrEx));
	for (int i=0;i<SysNum;i++)
	{
		for (unsigned  int j=0;j<SysBlock[i].CardNum;j++)
		{
			for (unsigned int k=0;k<SysBlock[i].ChperTB;k++)
			{
				//���ƽṹ
				CtrlBlock[i].rg_idx[j*SysBlock[i].ChperTB+k]=m_gain;    // ����  
				CtrlBlock[i].cpl_idx[j*SysBlock[i].ChperTB+k]=DC;
				CtrlBlock[i].filter_idx[j*SysBlock[i].ChperTB+k]=0;
				CtrlBlock[i].Kv[j*SysBlock[i].ChperTB+k]=1.0;
				CtrlBlock[i].Basev[j*SysBlock[i].ChperTB+k]=0.0;
			    strcpy(CtrlBlock[i].Unitstr[j*SysBlock[i].ChperTB+k],"V");
                CtrlBlock[i].CHACQEn[j*SysBlock[i].ChperTB+k]=1; 
								
				//�����ṹ(ͨ��)
				TrgBlock[i].CH_trg[j*SysBlock[i].ChperTB+k].chtrgmode=FALL; //�����ش��� RISE �½��ش���FALL
				TrgBlock[i].CH_trg[j*SysBlock[i].ChperTB+k].chtrglvl_0=200; // ��ʾ������ƽ1�ĵ�ƽֵ����ȡֵ��Χ0~255�����赱ǰ����Ϊ��1V��0��ǰ���̵���Сֵ-1V,��255���ʾ��ǰ���̵����ֵ1V��
				TrgBlock[i].CH_trg[j*SysBlock[i].ChperTB+k].chtrglvl_1=0;  // ������ƽ2��ֵ
				TrgBlock[i].CH_trg[j*SysBlock[i].ChperTB+k].chtrglogic=0;//��ʾͨ�������߼���0��"�߼���"��1��"�߼���"����
				TrgBlock[i].CH_trg[j*SysBlock[i].ChperTB+k].chtrgen=1;//��ʾͨ�������ź��Ƿ�����������ߣ�0����ֹ��1��������
			}
			//���ƽṹ
			CtrlBlock[i].ADMode[j]=Single_ACQ; // ���βɼ�
			CtrlBlock[i].clkmode[j]=1;     //������ÿһԪ�ر�ʾ��Ӧʱ���Ĳ���ʱ��Դ��0:���ء�1:���ߡ�2:��ʱ��ֱ�����롢3:��ʱ�Ӷ���Ƶ���� ��4 :����ʱ��	,����Externcl��Ϊ��Ƶֵ���Զ������ʱ�ӵ�Ƶ��		
			CtrlBlock[i].smpclk_idx[j]=m_SampleRateIdex;     // ʱ���Ĳ���ʱ������   ����Ƶ��  = ��������Ƶֵ��/ (N+1)
			CtrlBlock[i].smplength[j]=m_SampleNum;   // ��Ӧʱ���Ĳ�������
			CtrlBlock[i].dlylength[j]=0;
			CtrlBlock[i].externclk[j]=0;   // ��ʱ��Ƶ�� ����Ƶ�ʵ�����ʱ��Ƶ��
            CtrlBlock[i].TestBlock[j].TestPart=NoCH;  // ������
            CtrlBlock[i].TestBlock[j].DA8BitL=0; // ���ⲿ�豸�����ɼ�����
			CtrEx[i].Aux_IOMode[j] = 1;    //�๦�ܿ�Ϊ�ⴥ���ź�����
		
			
			//����������(ʱ��)
			TrgBlock[i].TB_trg[j].trgsource=Local_trg;   //ʱ������Ϊ���ش���
             if(m_trigSources==0)
			{
			   TrgBlock[i].TB_trg[j].trgmode=INTERNAL;  // �ڴ���
			}
			else if(m_trigSources==1)
			{
			   TrgBlock[i].TB_trg[j].trgmode=MANUAL;   // �ֶ�����
			}
			else
			{
			   TrgBlock[i].TB_trg[j].trgmode=EXTERN;  // �ⲿ����
			}    
			TrgBlock[i].TB_trg[j].trgout=0;  // �����ź��Ƿ�����ߣ�0:��ֹ 1:������
			TrgBlock[i].TB_trg[j].bustrglogic=0;  // ���ߴ����߼���0:"�߼���"��ʽ�������ߡ�1:"�߼���"��ʽ�������ߡ�
		}

	    ZeroMemory(CtrlBlock[i].rsvd,24);	
		//����Ӳ��

		TopHandle->HardwareSet(i,&CtrlBlock[i],&TrgBlock[i],&CtrEx[i], 1);
	}
			int iReturnValue;

						CString s1;


	//�������п��ɼ�
	for (i=0;i<SysNum;i++)
	{
		TopHandle->Acq(i,0);  //�������п��ɼ�
	}

			//int iReturnValue;
		LARGE_INTEGER  litmp; 
		LONGLONG       QPart1,QPart2;
		double         dfMinus, dfFreq, dfTim; 

			QueryPerformanceFrequency(&litmp); 
			dfFreq = (double)litmp.QuadPart; // ��ü�������ʱ��Ƶ��
			QueryPerformanceCounter(&litmp); 
			QPart1 = litmp.QuadPart; // ��ó�ʼֵ

		//	iReturnValue = m_pMTrg_(0,1);		       
			
			do
			{
				QueryPerformanceCounter(&litmp); 
				QPart2 = litmp.QuadPart;// �����ֵֹ 
				dfMinus = (double)(QPart2-QPart1); 
				dfTim = dfMinus / dfFreq; // ��ö�Ӧ��ʱ��ֵ����λΪ��
			}while(dfTim < m_Pulse/1000.0);
			DoEvents();
//			s1.Format("�źŷ�����������");
//	m_StatusBar.SetText(s1, 2, 0);
			iReturnValue = m_pMTrg_(0,1);	

        //ϵͳ״̬���

   	for (i=0;i<SysNum;i++)
	{
		for (unsigned int m=0;m<SysBlock[i].CardNum;m++)
		{
			AcqSts=-1;


			for(;;)
			{ 

				AcqSts=TopHandle->StatusCheck(i,m,&CardStatus);

				    if(AcqSts==0)
				{
			     	break;
				}
			}
		}
	}


			s1.Format("���ݲɼ����źű�����");
	m_StatusBar.SetText(s1, 2, 0);

   for(unsigned int tt=0;tt<SysBlock[0].CardNum*SysBlock[0].ChperTB;tt++) // ����*ÿ�ſ����߼�ͨ����
	{
     	r[tt]=SysBlock[0].RangeTab[CtrlBlock[0].rg_idx[tt]];   //����pack  ÿ��ͨ��������
	}
	//tt1 = timeGetTime(); 	

	for (unsigned int ii=0;ii<SysBlock[0].CardNum*SysBlock[0].ChperTB;ii++)
	{
		 ReturnVall=TopHandle->Pack(0,ii,Buf+ii*m_SampleNum,0,0,0,m_SampleNum);
		//ReturnVall=TopHandle->Pack(0,ii,Buf,0,0,0,m_SampleNum);
		 // �����ļ�
	}


	if(1 == m_SaveFileIdex)
	{
		char index[20];
		itoa(txtcount,index,10);
		char name[40];
		strcpy(name,filename);
		strcat(name,index);
		strcat(name,".txt");

		FILE *fp;

		if((fp = fopen(name,"wb")) == NULL)
		{
			AfxMessageBox("�ļ���ʧ��!");
		}

		//�¼ӵ�  д�ļ�ͷ  ʱ�� �������� ����Ƶ�� ���� ͨ�� ����ͨ��
		CTime t = CTime::GetCurrentTime();
		CString nowtime = t.Format("����ʱ��: %Y��%m��%d�� %X");
		fputs(nowtime,fp);
		CString s;
		s.Format("����������%d",m_SampleNum);
		fputs("    ",fp);
		fputs(s,fp);
		s.Format("����Ƶ��(Hz)��%d",m_SampleRate);
		fputs("    ",fp);
		fputs(s,fp);
		float gain;
		gain = -9.5 * m_gain + 10;

		s.Format("����(v)��%f",gain);
		fputs("    ",fp);
		fputs(s,fp);
		fputs("\n",fp);
		//AfxMessageBox(nowtime);
		int k = 0;
		for(k = 0 ; k <SysBlock[0].CardNum*SysBlock[0].ChperTB;k++)
		{
			TCHAR temp[30]={0};

			if((m_savech1.GetCheck() == 1) && k == 0 )
			{
				fputs("CH1",fp);
			   // fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech2.GetCheck() == 1) && k == 1 )
			{
				fputs("CH2",fp);
			 //   fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech3.GetCheck() == 1) && k == 2 )
			{
				fputs("CH3",fp);
			 //   fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech4.GetCheck() == 1) && k == 3 )
			{
				fputs("CH4",fp);
			  //  fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech5.GetCheck() == 1) && k == 4 )
			{
				fputs("CH5",fp);
			   // fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech6.GetCheck() == 1) && k == 5 )
			{
				fputs("CH6",fp);
			    //fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech7.GetCheck() == 1) && k == 6 )
			{
				fputs("CH7",fp);
			   // fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
			if((m_savech8.GetCheck() == 1) && k == 7 )
			{
				fputs("CH8",fp);
			  //  fputs("\n",fp);
				fwrite(Buf + k*m_SampleNum,sizeof(int),m_SampleNum,fp);

			}
		}

		fclose(fp);
		txtcount ++;
	}
// �¼ӵģ��Ը�ͨ�������˲� �ں��ڿ���ѡ�����˲������ǰ�ͨ�����˲�
// n Ϊ �˲�������  band���˲������ͣ���ͨ ��ͨ ��ͨ ����  fl\fh   fs:����Ƶ��  wn:���������� 5:������  data���˲������� result���˲������  dn: ���ݳ���
//int   firwin_e(int n,int band,int fl,int fh,int fs,int wn,int *data,int *result,int dn);
    if(m_filter.GetCheck())
	{
		for ( ii=7;ii<SysBlock[0].CardNum*SysBlock[0].ChperTB;ii++)
		{
			 ReturnVall = firwin_e(32,3,200,5000,m_SampleRate,5,Buf + ii *m_SampleNum,Buf + ii *m_SampleNum,m_SampleNum);
			//ReturnVall=TopHandle->Pack(0,ii,Buf,0,0,0,m_SampleNum);
			 // �����ļ�
		}
	}


//  ���޸ģ�ѡ����ʾ��ת��   �Ƿ���Բ�ת��ֱ����ʾ�أ�
       
			for(i=0;i<m_SampleNum;i++)
				BufData1[i]= Buf[i]*(r[0]/2147483648);


			for(i=0;i<m_SampleNum;i++)
				BufData2[i]= Buf[i+m_SampleNum]*(r[1]/2147483648);

			for(i=0;i<m_SampleNum;i++)
				 BufData3[i]= Buf[i+2*m_SampleNum]*(r[2]/2147483648);

			for(i=0;i<m_SampleNum;i++)
				 BufData4[i]= Buf[i+3*m_SampleNum]*(r[3]/2147483648);


			for(i=0;i<m_SampleNum;i++)
				 BufData5[i]= Buf[i+4*m_SampleNum]*(r[4]/2147483648);

			for(i=0;i<m_SampleNum;i++)
				  BufData6[i]= Buf[i+5*m_SampleNum]*(r[5]/2147483648);


			for(i=0;i<m_SampleNum;i++)
				 BufData7[i]= Buf[i+6*m_SampleNum]*(r[6]/2147483648);

			for(i=0;i<m_SampleNum;i++)
				 BufData8[i]= Buf[i+7*m_SampleNum]*(r[7]/2147483648);


	delete[] Buf; 	Buf=NULL;

	if(1 == m_FftDisplyIdex)
	{
		double *Fft = new double[m_SampleNum];


		if(m_fft_index == 0)
		{
			memcpy(Fft,BufData1,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 1)
		{
			memcpy(Fft,BufData2,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 2)
		{
			memcpy(Fft,BufData3,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 3)
		{
			memcpy(Fft,BufData4,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 4)
		{
			memcpy(Fft,BufData5,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 5)
		{
			memcpy(Fft,BufData6,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 6)
		{
			memcpy(Fft,BufData7,m_SampleNum*sizeof(double));
		}
		else if(m_fft_index == 7)
		{
			memcpy(Fft,BufData8,m_SampleNum*sizeof(double));
		}


		result = 1;

    int L;
	L =  (int)(log(m_SampleNum)/log(2))+1;
	int nn = 1;
	nn <<= L;
	fftlen =  nn ;
		result = fft_my(Fft,FftResult,m_SampleNum);

		delete[] Fft;
		Fft = NULL;

	}
// int fft_f(double *data,complex <double> *a,int L);


	ShowLen=m_SampleNum;
//	UpdateData(false);  //�¼ӵ� Ϊʵ��ʵʱFFT��ʾ
}


void CACQXCORRDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < SysBlock[0].CardNum; i++)
			TopHandle->PowerOn(0, i);


	check=true;
		StartSignal();
	 while(check)
	 {
		 long tt1,tt2,nn;


		tt1 = GetTickCount();

			Start();
		tt2 = GetTickCount(); 
        nn=tt2-tt1;		 


		CString s1;
			s1.Format("ִ��ʱ��: %d",nn);
	m_StatusBar.SetText(s1, 2, 0);

        if(m_CH1.GetCheck())
		{
            Draw1();
		}

		if(m_CH2.GetCheck())
		{
			Draw2();
		}

		if(m_CH3.GetCheck())
		{
            Draw3();
		}

		if(m_CH4.GetCheck())
		{
            Draw4();
		}

		if(m_CH5.GetCheck())
		{
            Draw5();
		}

        if(m_CH6.GetCheck())
		{
            Draw6();
		}

		if(m_CH7.GetCheck())
		{
            Draw7();
		}

        if(m_CH8.GetCheck())
		{
            Draw8();
		}		

		if(1 == m_FftDisplyIdex)
		{
			DrawFft();
		}


		m_times++;
		UpdateData(false);
		DoEvents();

	delete[] BufData1; BufData1 = NULL;
	delete[] BufData2; BufData2 = NULL;
	delete[] BufData3; BufData3 = NULL;
	delete[] BufData4; BufData4 = NULL;
	delete[] BufData5; BufData5 = NULL;
	delete[] BufData6; BufData6 = NULL;
	delete[] BufData7; BufData7 = NULL;
	delete[] BufData8; BufData8 = NULL;
	delete[] FftResult;   FftResult =NULL;
	}
	
	
}

void CACQXCORRDlg::OnStop() 
{
	// TODO: Add your control notification handler code here


	txtcount = 1;
	check = false;
	TopHandle->StopAcq(0,0xff);
//	m_bStop = TRUE;
	m_pStopDA_(0,0);     // ֹͣ��� 
    m_pStopDA_(0,1);     // ֹͣ��� 


	if(pBuf1 != NULL)
	{
		delete[] pBuf1;
		pBuf1 = NULL;
	}
	if(pBuf2 != NULL)
	{
		delete[] pBuf2;
		pBuf2 = NULL;
	}
	
}



void CACQXCORRDlg::OnQuit() 
{
	// TODO: Add your control notification handler code here

/*	for(int i = 0; i < SysBlock[0].CardNum; i++)
		TopHandle->PowerOff(0, i);*/
	m_pCloseTopDll();
	
	if (TopHandle)
	{
		TopHandle->CloseTopDll() ;
		delete TopHandle;
		TopHandle=NULL;
	}	
	else 
	{
	}

	if(BufData1 != NULL)
	{
			delete[] BufData1;
			BufData1 = NULL;
	}
	if(BufData2 != NULL)
	{
			delete[] BufData2;
			BufData2 = NULL;
	}
	if(BufData3 != NULL)
	{
			delete[] BufData3;
			BufData3 = NULL;
	}
	if(BufData4 != NULL)
	{
			delete[] BufData4;
			BufData4 = NULL;
	}
	if(BufData5 != NULL)
	{
			delete[] BufData5;
			BufData5 = NULL;
	}
	if(BufData6 != NULL)
	{
			delete[] BufData6;
			BufData6 = NULL;
	}
	if(BufData7 != NULL)
	{
			delete[] BufData7;
			BufData7 = NULL;
	}
	if(BufData8 != NULL)
	{
			delete[] BufData8;
			BufData8 = NULL;
	}
	if(FftResult != NULL )
	{
			delete[] FftResult;
			FftResult = NULL;
	}

	if(IDYES==AfxMessageBox(("���Ҫ�˳���?"),MB_YESNO))
    {
        CDialog::OnOK();
    }

}

void CACQXCORRDlg::DrawWave1(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL1); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;

	int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw1()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL1);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave1(&memDC,BufData1);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave2(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL2); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;
	int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw2()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL2);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave2(&memDC,BufData2);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave3(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL3); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;
		int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw3()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL3);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave3(&memDC,BufData3);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave4(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL4); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;
	int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw4()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL4);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave4(&memDC,BufData4);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave5(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL5); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;
	int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw5()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL5);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave5(&memDC,BufData5);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave6(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL6); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;
	int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw6()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL6);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave6(&memDC,BufData6);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave7(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL7); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;
	int mm = 0;
	mm = m_SampleNum/500.0;


	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw7()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL7);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave7(&memDC,BufData7);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWave8(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL8); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
	
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	double max=0;

	int mm = 0;
	mm = m_SampleNum/500.0;

	for(int i=0;i<ShowLen;i+= mm)
	{
		double temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=mm;i<ShowLen;i+= mm)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::Draw8()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_SIGNAL8);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWave8(&memDC,BufData8);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::DrawWaveFft(CDC *pDC,double *m_dTimeDomain)
{
	CRect Rect;  // ����һ����������Ķ���
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_FFTDISPLAY); // ��ô��ھ��
	
	pWnd->GetClientRect(&Rect); // �õ����ھ��������
	pDC->Rectangle(&Rect);  // ��һ������
    
	// ��X��Y����
	pDC->MoveTo(Rect.left+5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2+4);
	pDC->MoveTo(Rect.right-5,(Rect.top+Rect.bottom)/2);
	pDC->LineTo(Rect.right-15,(Rect.top+Rect.bottom)/2-4);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+20,Rect.bottom-20);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+16,Rect.top+30);
	pDC->MoveTo(Rect.left+20,Rect.top+20);
	pDC->LineTo(Rect.left+24,Rect.top+30);

	// ��X��Y �̶�
	int x = Rect.right - Rect.left - 25;  // ������߿ճ�20 �ұ߿ճ�5
	int y = Rect.Height();
	// ��ʾ�Ŀ̶�ֵ  ����ߴ�fft 
	

	int size ;
//	size = int (m_SampleRate/1000 *ShowLen/fftlen); 

// ���޸ĵ�  Ҫ����ʾ��1-10k
	size = 10;

	char ch[4];	

	// X��̶�
	for(int kk = 0; kk <= size;kk++)
	{
		pDC->MoveTo(x/size *kk +20,y/2);
		pDC->LineTo(x/size * kk +20,y/2-2);

		itoa(kk,ch,10);
		pDC->TextOut(x/size * kk-2 +20, y/2+4,ch);
	}

		pDC->TextOut(Rect.right-35, y/2+25,"(KHz)");
		pDC->TextOut(Rect.left, Rect.top+40,"����");

// ���޸ĵģ�����Ƶ����ʾ��Χȷ����ʾ����
		ShowLen =int (10000.0/m_SampleRate * fftlen);
		



    	// �������ʶ���
	CPen* pPenRed = new CPen;	
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	int max=0;

	for(int i=0;i<ShowLen;i++)
	{
		int temp=fabs(m_dTimeDomain[i]);
		if(max<temp)
		   max=temp;
	}
	// ��ʼ�� 0 ��
	pDC->MoveTo(Rect.left+20,(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[0]+(Rect.top+Rect.bottom)/2.0));
    
	for(i=1;i<ShowLen;i++)
	{
		pDC->LineTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
		pDC->MoveTo((int)((Rect.right-(Rect.left+20.0))/ShowLen*i+Rect.left+20.0),(int)(-((Rect.top+Rect.bottom)-2*(Rect.top+20))/2.0/max*m_dTimeDomain[i]+(Rect.top+Rect.bottom)/2.0));
	}
	pDC->SelectObject(pOldPen);	
	delete pPenRed;
}

void CACQXCORRDlg::DrawFft()
{
	CRect rect;	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_FFTDISPLAY);
	pWnd->GetClientRect(&rect);
	// ָ��
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	//�ڴ��ͼ
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom); //λͼ�Ŀ�Ⱥ͸߶�
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);
	DrawWaveFft(&memDC,FftResult);

	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

void CACQXCORRDlg::OnBrowser() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,"bin","*.bin");
	if(dlg.DoModal() == IDOK)
	{
		m_SignalFileName = dlg.GetFileName();
	}
	UpdateData(false);
	
}
