#ifndef _CLSTopDll_H_
#define _CLSTopDll_H_
#endif

#include "topuser.h"
#include "TopDllHead.h"

class TopDllEx
{
private:
	HINSTANCE hInstTopdll; //���ö�̬��������
	int ZipMode;		   //����ѹ����ʽ
	int Zip;		       //����ѹ������
	long lstart;	       //�������ݵ����
	long length;           //�������ݳ���

	int PackType;          //ʵʱ�ɼ�Ϊ1,��ʵʱΪ0
protected:

public:
	void PowerOff(int sys, int card);
	void PowerOn(int sys, int card);
	TopDllEx();
	~TopDllEx();

	int OpenTopDll();              //�򿪶�̬��(ϵͳʶ��)
	void SendHandle(HWND handle);  //���ݴ��ھ��(������Ϣ�Ĳ���)
	void CloseTopDll();            //�رն�̬��
	int GetSysNum();               //��ȡ��ϵͳ����Ŀ
                                   //(һ����ϵͳ���Ǳ�ʾһ�����͵Ĳɼ���)

	int GetCardNum(int SysIndex);                           //��ȡĳ����ϵͳ������Ŀ
	int SysInit(int SysIndex,SysInfo *pSysInfo,int Biasf);  //ϵͳ�ĳ�ʼ��
	
	int HardwareSet(int SysIndex,CntrlInfo *pCntrlInfo,     //����Ӳ��
		            TrgInfo *pTrgInfo,CntrlInfoEx *pCntEx,int SetFlg=0);
	
	int Acq(int SysIndex,int CardIndex);        //�����ɼ�
    int StopAcq(int SysIndex,int CardIndx);     //ֹͣ�ɼ�
	int ManualTrg(int SysIndex,int CardIndex);  //�ֶ����� 
	int StatusCheck(int SysIndex,int CardIndex,TCardStatus *pStatus);//���Ӳ��״̬
                                                                     //(һ�������ɼ������)
	int GetHead(int SysIndex,int ChIndex_g, void far* HdPtr);        //��ȡ����ͷ��Ϣ

	int Pack(int SysIndex,int ChIndex_g,int far* DataPtr,          //�������
		     int zmode, int znum, long DStart, long DLength);
	
	int DumpWaveRealShow(int SysIndex,int ChIndex_g,int *pWave);   //ʵʱ������ʾ
	
	//---------------------------------------------------------ϵͳУ׼��ʱ��ʹ��
	int WriteZero(int SysIndex,int ChIndex_g,BYTE data_H,BYTE data_L);  //д0����
	int WriteGain(int SysIndex,int ChIndex_g,BYTE data_H,BYTE data_L);  //д����
	int ReadZero(int SysIndex,int ChIndex_g, BYTE *ph,BYTE *pl);        //��0����
	int ReadGain(int SysIndex,int ChIndex_g, BYTE *ph,BYTE *pl);        //������
	
	//---------------------------------------------------------------����I/O���ܺ���
	int IO_Select(int SysIndex,int CardIndx,int funsel);         //����I/Oģʽ��ѡ��
	BYTE IO_In(int SysIndex,int CardIndex,int ioidx);            //����ֵI/O������
	int IO_Out(int SysIndex,int CardIndex,int ioidx,BYTE outb);  //д��ֵI/O������
	int Disable_OutData(int SysIndex,int CardIndex,int ioidx);   //��ֹ��ֵI/O�������
	int Enable_OutData(int SysIndex,int CardIndex,int ioidx);    //ʹ����ֵI/O�������
};



