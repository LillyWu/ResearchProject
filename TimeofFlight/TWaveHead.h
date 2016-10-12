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
//  =0: ֱ�� ; =1: ���Ҳ� ; =2: ����; =3: ���ǲ�; =4:TTL
//  =5: ������ݲ� ; =6: �½���ݲ�; =7: ָ����;
//  =8: ������; =9: ���ҵ�����; =10: ���ҵ�Ƶ��;
//  =11: ������; =12: ��˹���� ; =0xff: �Զ���
typedef struct
{
	int iWaveType; //�������� 
	int iWaveLen; //�������ݳ��ȣ������ڣ�0-65536��
	double fWaveFreq;//����Ƶ�� (������ڵ���0)
	double fWaveAmp;//���η��ȣ�0-10V��
	double fWaveBase;//����ƫ�ã�-10V-+10V��
	double fScale;//����ռ�ձȣ���������Ч,0-1��
	int iAMFM_FreqBS; //�ز�Ƶ�ʱ�����������/��Ƶ����Ч,������ڵ���2��

}WaveParamInfo;



typedef struct 
{
	BYTE TrgMode;   //0: �����ֶ�����;   1:�����ֶ�ͬ��;  2:�����ⴥ��; 3:�����ֶ�����; 4�����ֶ�ͬ��; 5:�����ⴥ��
	BYTE MulTrg;  //0: ���δ���; 1:��EnSingle=0ʱΪ��δ��� 
	BYTE ClkSource; //0: ������ʱ��;   1:������ʱ�� ; 2��������ʱ�ӣ�3:������ʱ��
	BYTE EnSingle;  //0: ����ģʽ; 1:����ģʽ	
	BYTE CHConnect;  //0: ��ͨ������ ��1������CH0��CH1,��CH0���
	BYTE IntEn;   //DMAģʽ��

}TWorkMode;

typedef struct 
{
	BYTE GoFlag;   //0: δ����;   1:������
	BYTE TrgFlag;  //0: δ����;   1:�Ѵ��� 
	BYTE EndFlag;  //0: δ����;   1:�ѽ���
	BYTE revd;
}TDAStatus;
 



#endif


//��������ָ������
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
typedef int (*lpfnSetFuncOut_)(int CardIdx,int Type);//Type =0: �����  ����1�� DA0 CLK��� ; =2:  DA1 CLK��� ; 
                                                     //     =3: DA0 �����ź���� ; = 4: DA1 �����ź���� ;
                                                     //     =5: DA0�����ź����    ;  ��6��DA1�����ź����  ��
									                 //	    =7: BASECLK���
typedef void (*lpfnIOOut_)(int CardIdx,int IoIdx,BYTE d);
typedef void (*lpfnIOIn_)(int CardIdx,int IoIdx,BYTE d);
typedef int (*lpfnGetGain_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);
typedef int (*lpfnGetZero_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);
typedef int (*lpfnSetGain_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);
typedef int (*lpfnSetZero_)(int CardIdx,BYTE CHIdx, BYTE RgIdx, BYTE * dh, BYTE * dl);

