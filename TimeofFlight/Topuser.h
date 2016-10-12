///////////////////////////////////////////////////////////////////////////////////
//  This head file define the important information for User Program Application //
//                                                                               //
//                         Author :  Yangmin        @ 2002.7.18.                 //
//                                                                               //
//                   copyrite reserved @ 1999-2002    TDEC                       //
///////////////////////////////////////////////////////////////////////////////////
#ifndef _TopUser_H_
#define _TopUser_H_

//define Type of TOP ACQ Cards
#define Type_PCI_10016		 0
#define Type_PCI_20012		 1
#define Type_PCI_10616		 2
#define Type_PCI_50612		 3
#define Type_PCI_4012		 4

//define messages for TOP ACQ Card
#define TOP_AcqOver       (WM_USER + 32)
#define TOP_TrgEvent      (WM_USER + 33)
#define TOP_DataReady     (WM_USER + 34)
#define TOP_DataOverload  (WM_USER + 35)
#define TOP_SavedDataOver (WM_USER + 36)

//define API returned value (may not be used)
#define SelftestError    0xff
#define PointerError     0xff
#define rbfFileNotExist  0xfe
#define NoFreeMem        0xfd
#define ErrorVersion     0xfc
#define ParaMissmatch    0xfb
#define CallSuccess      0x00
#define DeviceFailed     -1


#define int16  short int

typedef enum { Single_ACQ=0, Multiple_ACQ=1, Continuous_ACQ=2 } SampMode;
typedef enum { Local_trg=0, Bus_trg=1 } CardTrgSource;
typedef enum { MANUAL=0, EXTERN=1, INTERNAL=2, NOTRG=4 } CardTrgmode;
typedef enum { RISE=0, FALL=1, OUTSIDE=2, BETWEEN=3 } CHTrgmode;

typedef enum { DC=0, AC=1, DGND=2, AGND=3, DAIN=4 } COUPLE;    //DAIN means selftest DA output

typedef enum { NoCH=0, CH1_2=1, CH3_4=2 } UpDown;

typedef enum { iddle=0, waittrg=1, trigged=2 } TACQStatus;

const MaxSysNum =5;
const MaxCardNum=16;
const MaxTBNum=16;
const MaxChannels=64;
const MaxChperCard=4;

typedef struct 
{
   char     Name[10];
   long     DataStart;
   long     DataEnd;
   double   DeltaX;
   double   OffsetX;
   char     UnitX[5];
   double   DeltaY;
   double   OffsetY;
   char     UnitY[5];
   char     resvd[20];
} DataHeadInfo;

typedef struct
{
	int         TestOk;
	int         inACQ;
	TACQStatus  ACQStatus;
	int         DataReadyNum;
} TCardStatus;

typedef struct 
{
    UpDown  TestPart;
    BYTE    DA8BitL;
} TestInfo;

typedef struct 
{
	CardTrgSource     trgsource;
	CardTrgmode       trgmode;          //defined same as TRGMODE
	BYTE              trgout;           //0:forbidden; 1:allowed
	BYTE              bustrglogic;      //0:or;1:and;
} TBtrginfo;

typedef struct 
{
	CHTrgmode chtrgmode; 
	BYTE      chtrglvl_0;
	BYTE      chtrglvl_1;
	BYTE      chtrgen;         // 0:forbidden; 1:allowed;
	BYTE      chtrglogic;      // 0:or;1:and;
} CHtrginfo;

///////////////////////////////////////////////////////////////////////////
//The following structure defined for API interface data exchange
///////////////////////////////////////////////////////////////////////////
typedef struct 
{
   DWORD   Type_Idx;
   char    Type_Str[20];

   DWORD   VID;
   DWORD   DID;

   DWORD   MaxCardNumber;
   DWORD   CardNum;
   DWORD   TBperCd;
   DWORD   ChperTB;
   DWORD   PhysicalCHNum[MaxCardNum];

   DWORD   MaxSmpLength;

   double  ClkRateTab[18];      //if ClkRateTab[i]<=0 :invalid  
   double  RangeTab[8];         //if RangeTab[i]<=0 :invalid 
   COUPLE  CoupleTab[5];

   double  BandTab[16];
   
   DWORD   Resolution;
   double  Base_Frq;

   DWORD   *pUserBuffer[MaxCardNum];        //provided by user
   DWORD   UserBufferLength[MaxCardNum];    //provided by user,Length in BYTEs
   DWORD   SectNum;
   DWORD   SectLength;

   DWORD   rsvd[4];            //reserved for future develop 
} SysInfo;

typedef struct 
{
   TBtrginfo  TB_trg[16];
   CHtrginfo  CH_trg[64];
} TrgInfo;

typedef struct 
{
	SampMode  ADMode[MaxTBNum];            //each element represents correspond TB 
	DWORD     clkmode[MaxTBNum];           //each element represents correspond TB: 0=local clk; 1=busclk; 2=externclk direct in; 3=externclk/2 in  
	DWORD     smpclk_idx[MaxTBNum];        //each element represents correspond TB: if<16:clk fetched from ClkRateTab; else:divided value of Base_Frq 
	DWORD     smplength[MaxTBNum];         //each element represents correspond TB 
	long      dlylength[MaxTBNum];         //each element represents correspond TB 
	double    externclk[MaxTBNum];         //each element represents correspond TB: cannot exceed  maxsmpclk 
    TestInfo  TestBlock[MaxTBNum];         //each element represents correspond TB 

	DWORD     rg_idx[MaxChannels];         //each element represents correspond AD 
	COUPLE    cpl_idx[MaxChannels];        //each element represents correspond AD 
    DWORD     filter_idx[MaxChannels];     //each element represents correspond AD 

	double    Kv[MaxChannels];             //each element represents correspond AD 
	double    Basev[MaxChannels];          //each element represents correspond AD 
	char      Unitstr[MaxChannels][10];    //each element represents correspond AD 
    DWORD     CHACQEn[MaxChannels];        //each element represents correspond AD 

	BYTE rsvd[24];

} CntrlInfo;

typedef struct 
{
	BYTE ADResolution[MaxTBNum];	//AD�ֱ���  0:16bit ;1: 14bit
	double UserFreq[MaxTBNum];      //�û��Զ������Ƶ��  
	
	BYTE RealAcq[MaxTBNum];         //�ɼ�������ģʽ 0:��ʵʱ;1:ʵʱ
	BYTE RealAcqRatio[MaxTBNum];    //ʵʱ�ɼ��ĳ�㴫�����
	
	DWORD Revd0[MaxTBNum];          //����
	
	BYTE SectAcq[MaxTBNum];         //�ֶβɼ�ģʽ 0:���ֶ�(��1��); 1:�ֶ�
	BYTE SectNum[MaxTBNum];         //�ֶ��� 0��1=��1��;
    unsigned long  SectLen[MaxTBNum];         //�ֶγ��� (K����);
	
	BYTE TimingAcq[MaxTBNum];       //��ʱ�ɼ�ģʽ 0:�޶�ʱ; 1:����ʵʱʱ�Ӷ�ʱ; 2:GPS��ʱ
	
	DWORD Revd1[MaxTBNum];          //����
    DWORD Revd2[MaxTBNum];          //����
    
	BYTE RTTIMode[MaxTBNum];        //����ʵʱ���ݽӿ� 0:�ر� ;1:��
    BYTE DIOAcq[MaxTBNum];          //�����źŲɼ�     0:�ر� ;1;��,������4ͨ��
	
	BYTE Aux_IOMode[MaxTBNum];      /*
																	Aux_IO����ģʽ���� 
																	=0���ⲿʱ���ź����룻
																	=1���ⲿ�����ź����룻
																	=2�������ɼ�ʱ�������
																	=3�����������ź������
																	=4�������������λ���Aux_IOData��
																	=5������10MHz�ź������
																	=6������PXIͬ��10MHz�������PXI�棩
																	=7:ʵʱ����ʱ��
									*/
	BYTE Aux_IOData[MaxTBNum];      //��Aux_IOMode=4ʱ��Ч, =0������� ;>0 :�����
	BYTE PowerSwitch[MaxTBNum];    //ģ�⿪�� 0 ���Զ����� ��1: �� ��2�� �ر�
									
	BYTE  Revd3[7][MaxTBNum];
									
	DWORD Revd4[MaxTBNum];          //����
									
			
	BYTE InputRes[MaxChannels];     //�����迹 0=1Mŷ; 1=50ŷ
	BYTE ICPEn[MaxChannels];        //����ICP��չģ��  0:�ر� ;1:��
									
	DWORD Revd5[MaxChannels];       //����
	DWORD Revd6[MaxChannels];       //����
									
} CntrlInfoEx;

#endif