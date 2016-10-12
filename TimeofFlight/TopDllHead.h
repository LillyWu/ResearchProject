//--------------------------------------------------声明函数指针类型
typedef int (__stdcall*lpfnOpenTopDll)();     
typedef int (*lpfnSendHWND)(HWND);
typedef void (__stdcall*lpfnCloseTopDll)();    

typedef int (__stdcall*lpfnGetSysNum)();       
typedef int (*lpfnGetCardNum)(int);   

typedef int (*lpfnSysInit)(int,SysInfo*,int);                  
typedef int (*lpfnHardwareSet)(int,CntrlInfo*,TrgInfo*,CntrlInfoEx *, int);
typedef int (*lpfnAcq)(int,int);
typedef int (*lpfnStopAcq)(int,int);
typedef int (*lpfnManualTrg)(int,int);

typedef int (*lpfnStatusCheck)(int,int,TCardStatus*);
typedef int (*lpfnGetHead)(int,int,void far*);                 
typedef int (*lpfnPack)(int,int,int*,int,int,long,long, int);
 
typedef int  (*lpfnDumpWaveRealShow)(int,int,int*);      


typedef int (*lpfnWriteZero)(int,int,BYTE,BYTE);  
typedef int (*lpfnWriteGain)(int,int,BYTE,BYTE);  
typedef int (*lpfnReadZero)(int,int,BYTE*,BYTE*); 
typedef int (*lpfnReadGain)(int,int,BYTE*,BYTE*);

typedef int (*lpfnIO_Select)(int,int,int);         
typedef BYTE (*lpfnIO_In)(int,int,int);            
typedef int (*lpfnIO_Out)(int,int,int,BYTE);       
typedef int (*lpfnDisable_OutData)(int,int,int);
typedef int (*lpfnEnable_OutData)(int,int,int);

typedef void (*lpfnPowerOn)(int, int);
typedef void (*lpfnPowerOff)(int, int);