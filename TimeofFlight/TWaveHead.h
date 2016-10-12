///////////////////////////////////////////////////////////////////////////////////
//  This head file define the important information for User Program Application //
//                                                                               //
//                         Author :  YangXun        @ 2006.8.23.                 //
//                                                                               //
//                   copyright reserved  1999-2006    TDEC                       //
///////////////////////////////////////////////////////////////////////////////////
#ifndef _TDEC_Twave_H_
#define _TDEC_Twave_H_

#include <conio.h>
#include <stdlib.h>

#define U32 unsigned long
#define U16 unsigned short int
#define U8 BYTE


#define ChNum   2

#define TOP_DacOver       (WM_USER + 60)

//pObjWaveParamInfo->iWaveType:
//  =0: 直流 ; =1: 正弦波 ; =2: 方波; =3: 三角波; =4:TTL
//  =5: 上升锯齿波 ; =6: 下降锯齿波; =7: 指数波;
//  =8: 对数波; =9: 正弦调幅波; =10: 正弦调频波;
//  =11: 白噪声; =12: 高斯噪声 ; =0xff: 自定义
typedef struct
{
	int iWaveType; //波形类型 
	int iWaveLen; //波形数据长度（单周期，0-65536）
	double fWaveFreq;//波形频率 (必须大于等于0)
	double fWaveAmp;//波形幅度（0-10V）
	double fWaveBase;//波形偏置（-10V-+10V）
	double fScale;//方波占空比（仅方波有效,0-1）
	int iAMFM_FreqBS; //载波频率倍数（仅调幅/调频波有效,必须大于等于2）

}WaveParamInfo;



typedef struct 
{
	BYTE TrgMode;   //0: 本卡手动独立;   1:本卡手动同步;  2:本卡外触发; 3:总线手动独立; 4总线手动同步; 5:总线外触发
	BYTE MulTrg;  //0: 单次触发; 1:在EnSingle=0时为多次触发 
	BYTE ClkSource; //0: 本卡内时钟;   1:本卡外时钟 ; 2：总线内时钟；3:总线外时钟
	BYTE EnSingle;  //0: 连续模式; 1:单次模式	
	BYTE CHConnect;  //0: 两通道独立 ；1：连接CH0、CH1,由CH0输出
	BYTE IntEn;   //DMA模式，

}TWorkMode;

typedef struct 
{
	BYTE GoFlag;   //0: 未启动;   1:已启动
	BYTE TrgFlag;  //0: 未触发;   1:已触发 
	BYTE EndFlag;  //0: 未结束;   1:已结束
	BYTE revd;
}TDAStatus;
 



#endif


//声明函数指针类型
typedef int (*s_lpfnOpenTopDll)();     
typedef void (*s_lpfnCloseTopDll)();    


typedef int (*lpfnSysInit_)();   
typedef void (*lpfnSendHandle_)(HWND handle);     
            
typedef int (*lpfnSetHardware_)(int CardIdx,int DaIdx,TWorkMode * pWork,WaveParamInfo * pWave,double * WaveData);
typedef int (*lpfnStartDA_)(int CardIdx,int DaIdx);
typedef int (*lpfnStopDA_)(int CardIdx,int DaIdx);
typedef int (*lpfnMTrg_)(int CardIdx,int DaIdx);
typedef int (*lpfnCheckStatus_)(int CardIdx,TDAStatus * pStatus0,TDAStatus * pStatus1);
//typedef int (*lpfnSetFuncOut_)(int Type);
typedef int (*lpfnSetFuncOut_)(int CardIdx,int Type);//Type =0: 不输出  ；＝1： DA0 CLK输出 ; =2:  DA1 CLK输出 ; 
                                                     //     =3: DA0 结束信号输出 ; = 4: DA1 结束信号输出 ;
                                                     //     =5: DA0触发信号输出    ;  ＝6：DA1触发信号输出  ；
									                 //	    =7: BASECLK输出
typedef void (*lpfnIOOut_)(int CardIdx,int IoIdx,BYTE d);
typedef void (*lpfnIOIn_)(int CardIdx,int IoIdx,BYTE d);
typedef int (*lpfnGetGain_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);
typedef int (*lpfnGetZero_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);
typedef int (*lpfnSetGain_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);
typedef int (*lpfnSetZero_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);

