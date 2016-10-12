#ifndef _CLSTopDll_H_
#define _CLSTopDll_H_
#endif

#include "topuser.h"
#include "TopDllHead.h"

class TopDllEx
{
private:
	HINSTANCE hInstTopdll; //调用动态库句柄声明
	int ZipMode;		   //数据压缩方式
	int Zip;		       //数据压缩比率
	long lstart;	       //传递数据的起点
	long length;           //传递数据长度

	int PackType;          //实时采集为1,非实时为0
protected:

public:
	void PowerOff(int sys, int card);
	void PowerOn(int sys, int card);
	TopDllEx();
	~TopDllEx();

	int OpenTopDll();              //打开动态库(系统识别)
	void SendHandle(HWND handle);  //传递窗口句柄(用于消息的捕获)
	void CloseTopDll();            //关闭动态库
	int GetSysNum();               //获取子系统的数目
                                   //(一种子系统即是表示一种类型的采集卡)

	int GetCardNum(int SysIndex);                           //获取某种子系统卡的数目
	int SysInit(int SysIndex,SysInfo *pSysInfo,int Biasf);  //系统的初始化
	
	int HardwareSet(int SysIndex,CntrlInfo *pCntrlInfo,     //设置硬件
		            TrgInfo *pTrgInfo,CntrlInfoEx *pCntEx,int SetFlg=0);
	
	int Acq(int SysIndex,int CardIndex);        //启动采集
    int StopAcq(int SysIndex,int CardIndx);     //停止采集
	int ManualTrg(int SysIndex,int CardIndex);  //手动触发 
	int StatusCheck(int SysIndex,int CardIndex,TCardStatus *pStatus);//检测硬件状态
                                                                     //(一般启动采集后调用)
	int GetHead(int SysIndex,int ChIndex_g, void far* HdPtr);        //获取数据头信息

	int Pack(int SysIndex,int ChIndex_g,int far* DataPtr,          //打包数据
		     int zmode, int znum, long DStart, long DLength);
	
	int DumpWaveRealShow(int SysIndex,int ChIndex_g,int *pWave);   //实时波形显示
	
	//---------------------------------------------------------系统校准的时候使用
	int WriteZero(int SysIndex,int ChIndex_g,BYTE data_H,BYTE data_L);  //写0数据
	int WriteGain(int SysIndex,int ChIndex_g,BYTE data_H,BYTE data_L);  //写增益
	int ReadZero(int SysIndex,int ChIndex_g, BYTE *ph,BYTE *pl);        //读0数据
	int ReadGain(int SysIndex,int ChIndex_g, BYTE *ph,BYTE *pl);        //读增益
	
	//---------------------------------------------------------------数字I/O功能函数
	int IO_Select(int SysIndex,int CardIndx,int funsel);         //数字I/O模式的选择
	BYTE IO_In(int SysIndex,int CardIndex,int ioidx);            //读数值I/O的数据
	int IO_Out(int SysIndex,int CardIndex,int ioidx,BYTE outb);  //写数值I/O的数据
	int Disable_OutData(int SysIndex,int CardIndex,int ioidx);   //禁止数值I/O数据输出
	int Enable_OutData(int SysIndex,int CardIndex,int ioidx);    //使能数值I/O数据输出
};



