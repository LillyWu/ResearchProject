// ACQXCORRDlg.h : header file
//

#if !defined(AFX_ACQXCORRDLG_H__C9E47A7F_555A_4CED_9C49_E3A66D579262__INCLUDED_)
#define AFX_ACQXCORRDLG_H__C9E47A7F_555A_4CED_9C49_E3A66D579262__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ClsTopDll.h"
#include "TWaveHead.h"

#define CHANNEL 8
#define Num 18
#define DataLeng 800000



/////////////////////////////////////////////////////////////////////////////
// CACQXCORRDlg dialog

class CACQXCORRDlg : public CDialog
{
// Construction
public:
	CACQXCORRDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CACQXCORRDlg)
	enum { IDD = IDD_ACQXCORR_DIALOG };
	CButton	m_filter;
	CButton	m_savech8;
	CButton	m_savech7;
	CButton	m_savech6;
	CButton	m_savech5;
	CButton	m_savech4;
	CButton	m_savech3;
	CButton	m_savech2;
	CButton	m_savech1;
	CButton	m_CH1;
	CButton	m_CH2;
	CButton	m_CH3;
	CButton	m_CH4;
	CButton	m_CH5;
	CButton	m_CH6;
	CButton	m_CH7;
	CButton	m_CH8;
	CStatic	m_title;
	int		m_times;
	int		m_time;
	CString	m_filename;
	double	m_Amp;
	double	m_Pulse;
	CString	m_SignalFileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACQXCORRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	CStatusBarCtrl m_StatusBar;



	// �źŷ���������
	BOOL m_bStop;

	TDAStatus m_pTDAStatus[ChNum];
	WaveParamInfo m_pwvinfo[ChNum];
	WaveParamInfo m_pwvinfo_trig[ChNum];

	TWorkMode m_pwkmod[ChNum];
	lpfnSetZero_ m_pSetZero_;
	lpfnSetGain_ m_pSetGain_;
	lpfnGetZero_ m_pGetZero_;
	lpfnGetGain_ m_pGetGain_;
	lpfnIOIn_ m_pIOIn_;
	lpfnIOOut_ m_pIOOut_;
	lpfnSetFuncOut_ m_pSetFuncOut_;
	lpfnCheckStatus_ m_pCheckStatus_;
	lpfnMTrg_ m_pMTrg_;
	lpfnStopDA_ m_pStopDA_;
	lpfnStartDA_ m_pStartDA_;
	lpfnSetHardware_ m_pSetHardware_;
	lpfnSendHandle_ m_pSendHandle_;
	lpfnSysInit_ m_pSysInit_;
	s_lpfnCloseTopDll m_pCloseTopDll;
	s_lpfnOpenTopDll m_pOpenTopDll;
	HINSTANCE m_hInstTopdll;

	CFile m_Card1File;
	CFile m_CardFilePulse;
	double *pBuf1;


	CFile m_Card2File;
	double *pBuf2;

	CString	m_TriggerFileName;

	void StartSignal();


	// �����Ա����  ���ݳ�Ա

	TopDllEx *TopHandle;                     //����һ��ȫ�ֱ���
	int SysNumber ;                          //��ʼ���ɼ�����
	int SysNum ;                             //��ʼ��ϵͳ��
	SysInfo SysBlock[MaxSysNum];             //����ϵͳ��Ϣ
	TrgInfo TrgBlock[MaxSysNum];             //���崥����Ϣ
	CntrlInfo CtrlBlock[MaxSysNum];          //���������Ϣ
	TCardStatus CardStatus;                  //���忨״̬
	CntrlInfoEx CtrEx[MaxSysNum];
	DataHeadInfo  HeadInfo;
		
	int ChannelNum ;  		
	int ShowLen;
	double *m_dTimeDomain;
	int AcqSts;
	int ReturnVal;
	bool check;
	CString mm;
	char buffer[128];


	int m_fft_index;
	int result;
	double *FftResult;
	int fftlen;



	int m_SampleRateIdex;
	int m_SampleRate;
	int m_SampleNum;
	int m_gain;
	int m_trigSources;
	int m_SaveFileIdex;
	int m_FftDisplyIdex;


	double *BufData1;
	double *BufData2;
	double *BufData3;
	double *BufData4;
	double *BufData5;
	double *BufData6;
	double *BufData7;
	double *BufData8;

	int txtcount;
	char filename[40];

	void Start();
	bool DoEvents();

	void DrawWave1(CDC *pDC,double *m_dTimeDomain);
	void Draw1();


	void DrawWave2(CDC *pDC,double *m_dTimeDomain);
	void Draw2();

	void DrawWave3(CDC *pDC,double *m_dTimeDomain);
	void Draw3();

	void DrawWave4(CDC *pDC,double *m_dTimeDomain);
	void Draw4();

	void DrawWave5(CDC *pDC,double *m_dTimeDomain);
	void Draw5();

	void DrawWave6(CDC *pDC,double *m_dTimeDomain);
	void Draw6();

	void DrawWave7(CDC *pDC,double *m_dTimeDomain);
	void Draw7();

	void DrawWave8(CDC *pDC,double *m_dTimeDomain);
	void Draw8();


	void DrawWaveFft(CDC *pDC,double *m_dTimeDomain);
	void DrawFft();




protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACQXCORRDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStop();
	afx_msg void OnStart();
	afx_msg void OnQuit();
	afx_msg void OnInit();
	afx_msg void OnBrowser();
	//}}AFX_MSG
private:
	CFont m_Font;
	CDC *pDC;             //��Ļ��ͼ�豸
    CDC memDC;            //�ڴ��ͼ�豸


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQXCORRDLG_H__C9E47A7F_555A_4CED_9C49_E3A66D579262__INCLUDED_)
