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
	BYTE ADResolution[MaxTBNum];	//AD分辨率  0:16bit ;1: 14bit
	double UserFreq[MaxTBNum];      //用户自定义采样频率  
	
	BYTE RealAcq[MaxTBNum];         //采集卡传输模式 0:非实时;1:实时
	BYTE RealAcqRatio[MaxTBNum];    //实时采集的抽点传输比例
	
	DWORD Revd0[MaxTBNum];          //保留
	
	BYTE SectAcq[MaxTBNum];         //分段采集模式 0:不分段(分1段); 1:分段
	BYTE SectNum[MaxTBNum];         //分段数 0、1=分1段;
    unsigned long  SectLen[MaxTBNum];         //分段长度 (K样点);
	
	BYTE TimingAcq[MaxTBNum];       //定时采集模式 0:无定时; 1:板载实时时钟定时; 2:GPS定时
	
	DWORD Revd1[MaxTBNum];          //保留
    DWORD Revd2[MaxTBNum];          //保留
    
	BYTE RTTIMode[MaxTBNum];        //海量实时数据接口 0:关闭 ;1:打开
    BYTE DIOAcq[MaxTBNum];          //数字信号采集     0:关闭 ;1;打开,保存至4通道
	
	BYTE Aux_IOMode[MaxTBNum];      /*
																	Aux_IO工作模式设置 
																	=0：外部时钟信号输入；
																	=1：外部触发信号输入；
																	=2：本卡采集时钟输出；
																	=3：本卡触发信号输出；
																	=4：本卡软件控制位输出Aux_IOData；
																	=5：本卡10MHz信号输出；
																	=6：本机PXI同步10MHz输出；（PXI版）
																	=7:实时传输时钟
									*/
	BYTE Aux_IOData[MaxTBNum];      //当Aux_IOMode=4时有效, =0：输出低 ;>0 :输出高
	BYTE PowerSwitch[MaxTBNum];    //模拟开关 0 ：自动控制 ；1: 打开 ；2： 关闭
									
	BYTE  Revd3[7][MaxTBNum];
									
	DWORD Revd4[MaxTBNum];          //保留
									
			
	BYTE InputRes[MaxChannels];     //输入阻抗 0=1M欧; 1=50欧
	BYTE ICPEn[MaxChannels];        //接入ICP扩展模块  0:关闭 ;1:打开
									
	DWORD Revd5[MaxChannels];       //保留
	DWORD Revd6[MaxChannels];       //保留
									
} CntrlInfoEx;

#endif