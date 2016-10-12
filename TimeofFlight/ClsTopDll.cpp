#include "stdafx.h"
#include "ClsTopDll.h"

//-------------------------函数指针声明
lpfnOpenTopDll pOpenTopDll;
lpfnSendHWND pSendHWND;
lpfnCloseTopDll pCloseTopDll;
lpfnGetSysNum pGetSysNum;
lpfnGetCardNum pGetCardNum;
lpfnSysInit pSysInit;
lpfnHardwareSet pHardwareSet;
lpfnAcq pAcq;
lpfnStopAcq pStopAcq;
lpfnManualTrg pManualTrg;
lpfnStatusCheck pStatusCheck;
lpfnGetHead pGetHead;
lpfnPack pPack;
lpfnDumpWaveRealShow pDumpWaveRealShow;

lpfnWriteZero pWriteZero;
lpfnWriteGain pWriteGain;
lpfnReadZero pReadZero;
lpfnReadGain pReadGain;

lpfnIO_Select pIO_Select;
lpfnIO_In pIO_In;
lpfnIO_Out pIO_Out;
lpfnDisable_OutData pDisable_OutData;
lpfnEnable_OutData pEnable_OutData;

lpfnPowerOn pPowerOn;
lpfnPowerOff pPowerOff;
//-------------------------------------

TopDllEx::TopDllEx()
{

}

TopDllEx::~TopDllEx()
{	
	if (hInstTopdll)
	{
		FreeLibrary(hInstTopdll);
	}
}

TopDllEx::OpenTopDll()
{
	int SysNum;
	//hInstTopdll=LoadLibrary("pci10016dll.dll");
    hInstTopdll=LoadLibrary("TdecApi.dll");
	
	if (hInstTopdll)
	{
		pOpenTopDll=(lpfnOpenTopDll)GetProcAddress(hInstTopdll, "OpenTopdll");
		pSendHWND=(lpfnSendHWND)GetProcAddress(hInstTopdll, "SendHandle");
		pCloseTopDll=(lpfnCloseTopDll)GetProcAddress(hInstTopdll,"CloseTopdll");
		pGetSysNum=(lpfnGetSysNum)GetProcAddress(hInstTopdll,"GetSysNum_");
		pGetCardNum=(lpfnGetCardNum)GetProcAddress(hInstTopdll,"GetCardNum_");
		pSysInit=(lpfnSysInit)GetProcAddress(hInstTopdll,"SysInit_");
		pHardwareSet=(lpfnHardwareSet)GetProcAddress(hInstTopdll,"HardwareSetEx_");
		pAcq=(lpfnAcq)GetProcAddress(hInstTopdll,"Acq_");
		pStopAcq=(lpfnStopAcq)GetProcAddress(hInstTopdll,"StopAcq_");
		pManualTrg=(lpfnManualTrg)GetProcAddress(hInstTopdll,"MTrg_");
		pStatusCheck=(lpfnStatusCheck)GetProcAddress(hInstTopdll,"StatusCheck_");
		pGetHead=(lpfnGetHead)GetProcAddress(hInstTopdll,"GetHead_");
		pPack=(lpfnPack)GetProcAddress(hInstTopdll,"PackEx32_");
		pDumpWaveRealShow=(lpfnDumpWaveRealShow)GetProcAddress(hInstTopdll,"DumpWaveRealShow32_");
		
		pWriteZero=(lpfnWriteZero)GetProcAddress(hInstTopdll,"writezero_");
		pWriteGain=(lpfnWriteGain)GetProcAddress(hInstTopdll,"writegain_");
		pReadZero=(lpfnReadZero)GetProcAddress(hInstTopdll,"readzero_");
		pReadGain=(lpfnReadZero)GetProcAddress(hInstTopdll,"readgain_");
		
		pIO_Select=(lpfnIO_Select)GetProcAddress(hInstTopdll,"IO_Select_");
		pIO_In=(lpfnIO_In)GetProcAddress(hInstTopdll,"IO_In_");
		pIO_Out=(lpfnIO_Out)GetProcAddress(hInstTopdll,"IO_Out_");
		pDisable_OutData=(lpfnDisable_OutData)GetProcAddress(hInstTopdll,"Disable_OutData_");
		pEnable_OutData=(lpfnEnable_OutData)GetProcAddress(hInstTopdll,"Enable_OutData_");

		pPowerOn = (lpfnPowerOn)GetProcAddress(hInstTopdll,"PowerOn_");
		pPowerOff = (lpfnPowerOff)GetProcAddress(hInstTopdll,"PowerOff_");

		SysNum=pOpenTopDll();
	}
	else
	{
		AfxMessageBox("动态库调用失败!");
	}
	
	return SysNum;
}

void TopDllEx::SendHandle(HWND handle)
{
	pSendHWND(handle);
}

void TopDllEx::CloseTopDll()
{ 	

	pCloseTopDll();
}

int TopDllEx::GetSysNum()
{
	int SysNum;	
	SysNum=pGetSysNum();
	return SysNum;
}

int TopDllEx::GetCardNum(int SysIndex)
{
	int CardNum;	
	CardNum=pGetCardNum(SysIndex);
	return CardNum;
}

int TopDllEx::SysInit(int SysIndex,SysInfo *pSysInfo,int Biasf)
{
	int ReturnVal;
	ReturnVal=pSysInit(SysIndex,pSysInfo,Biasf);
	return ReturnVal;
}

int TopDllEx::HardwareSet(int SysIndex,
						  CntrlInfo *pCntrlInfo,
						  TrgInfo *pTrgInfo,
						  CntrlInfoEx *pCntEx,
						  int SetFlg)
{
	int ReturnVal;
	PackType = pCntEx->RealAcq[0];
	ReturnVal=pHardwareSet(SysIndex,pCntrlInfo,pTrgInfo, pCntEx, SetFlg);
	return ReturnVal;
}

int TopDllEx::Acq(int SysIndex,int CardIndex)
{
	int ReturnVal;
	ReturnVal=pAcq(SysIndex,CardIndex);
	return ReturnVal;
}

int TopDllEx::StopAcq(int SysIndex,int CardIndex)
{
	int ReturnVal;
	ReturnVal=pStopAcq(SysIndex,CardIndex);
	return ReturnVal;
}

int TopDllEx::ManualTrg(int SysIndex,int CardIndex)
{
	int ReturnVal;
	ReturnVal=pManualTrg(SysIndex,CardIndex);
	return ReturnVal;
}

int TopDllEx::StatusCheck(int SysIndex,
						  int CardIndex,
						  TCardStatus *pStatus)
{
	int ReturnVal;
	ReturnVal=pStatusCheck(SysIndex,CardIndex,pStatus);
	return ReturnVal;
}

int TopDllEx::GetHead(int SysIndex,
					  int ChIndex_g,
					  void *Hdptr)
{
	int ReturnVal;
	ReturnVal=pGetHead(SysIndex,ChIndex_g,Hdptr);
	return ReturnVal;
}

int TopDllEx::Pack(int SysIndex,
				   int ChIndex_g,
				   int *DataPtr,
				   int zmode,
				   int znum,
				   long DStart,
				   long DLength)
{
	int ReturnVal;
	int buf[32768];
	int c = (DLength+32767)/32768;
	for(int i = 0; i < c; i++)
	{
		ReturnVal=pPack(SysIndex,
					ChIndex_g,
					buf,
					zmode,
					znum,
					DStart+i*32768,
					32768,
					PackType);
		memcpy(DataPtr+i*32768, buf, sizeof(int)*min(32768, (DLength-i*32768)));
	}
	
	return ReturnVal;
}

int TopDllEx::DumpWaveRealShow(int SysIndex,
							   int ChIndex_g,
							   int *pWave)
{
	int ReturnVal;
	ReturnVal=pDumpWaveRealShow(SysIndex,ChIndex_g,pWave);
	return ReturnVal;
}

//------------------------------------------------------------系统校准函数
int TopDllEx::WriteZero(int SysIndex,
						int ChIndex_g,
						BYTE Data_H,
						BYTE Data_L)
{
	int ReturnVal;
	ReturnVal=pWriteZero(SysIndex,ChIndex_g,Data_H,Data_L);
	return ReturnVal;
}

int TopDllEx::WriteGain(int SysIndex,
						int ChIndex,
						BYTE Data_H,
						BYTE Data_L)
{
	int ReturnVal;
	ReturnVal=pWriteGain(SysIndex,ChIndex,Data_H,Data_L);
	return ReturnVal;
}

int TopDllEx::ReadZero(int SysIndex,int ChIndex_g,BYTE *ph,BYTE *pl)
{
	int ReturnVal;
	ReturnVal=pReadZero(SysIndex,ChIndex_g,ph,pl);
	return ReturnVal;
}

int TopDllEx::ReadGain(int SysIndex,int ChIndex_g,BYTE *ph,BYTE *pl)
{
	int ReturnVal;
	ReturnVal=pReadGain(SysIndex,ChIndex_g,ph,pl);
	return ReturnVal;
}

//-------------------------------------------------------------数字I/O函数
int TopDllEx::IO_Select(int SysIndex,int CardIndex,int funsel)
{
	int ReturnVal;
	ReturnVal=pIO_Select(SysIndex,CardIndex,funsel);
	return ReturnVal;
}

BYTE TopDllEx::IO_In(int SysIndex,int CardIndex,int ioidx)
{
	BYTE ReturnVal;
	ReturnVal=pIO_In(SysIndex,CardIndex,ioidx);
	return ReturnVal;
}

int TopDllEx::IO_Out(int SysIndex,int CardIndex,int ioidx,BYTE outb)
{
	int ReturnVal;
	ReturnVal=pIO_Out(SysIndex,CardIndex,ioidx,outb);
	return ReturnVal;
}

int TopDllEx::Disable_OutData(int SysIndex,int CardIndex,int ioidx)
{
	int ReturnVal;
	ReturnVal=pDisable_OutData(SysIndex,CardIndex,ioidx);
	return ReturnVal;
}

int TopDllEx::Enable_OutData(int SysIndex,int CardIndex,int ioidx)
{
	int ReturnVal;
	ReturnVal=pEnable_OutData(SysIndex,CardIndex,ioidx);
	return ReturnVal;
}

void TopDllEx::PowerOn(int sys, int card)
{
	if(pPowerOn)
		pPowerOn(sys, card);
}

void TopDllEx::PowerOff(int sys, int card)
{
	if(pPowerOff)
		pPowerOff(sys,card);
}
