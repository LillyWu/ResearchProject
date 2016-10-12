// SignalProcess.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SignalProcess.h"
#define wl 31

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

/*-----------------------------------------------  
           FFT函数(基于时间)
    data:指向数据序列地指针
	a   :指向data的DFT序列的指针
	L   :2的L次方为FFT的点数
--------------------------------------------------*/
int fft(int *data,complex <double> *a,int L)
{
	complex <double> u;
	complex <double> w;
	complex <double> t;
	unsigned n=1,nv2,nm1,k,le,lei,ip;
	int i,j,m,number;
	double tmp;
	// n 为FFT点数
	n<<=L;
	// 初始化序列 
	for(number = 0; number<n; number++)
	{
		a[number] = complex <double> (data[number],0);
	}

	//雷德算法： 倒位序 倒位序二进制数的下面一个数是上面一个数在最高位加1并由高位向低位进位而得到。
	nv2=n>>1;  //N/2
	nm1=n-1;   // N-1
	j=0;
	for(i=0;i<nm1;i++)
	{
		if(i<j)
		{
			t=a[j];
			a[j]=a[i];
			a[i]=t;
		}
		k=nv2;
		while(k<=j)
		{
			j-=k;
			k>>=1;
		}
		// 下一个倒位序
		j+=k;
	}

	le=1;
	for(m=1;m<=L;m++)  // 一共L级蝶形运算
	{
		lei=le;  // 蝶形距离
		le<<=1;
		u=complex<double>(1,0); // W(0)
		tmp=PI/lei;
		w=complex<double>(cos(tmp),-sin(tmp));
		for(j=0;j<lei;j++)
		{
			for(i=j;i<n;i+=le)   //  A + B * 旋转因子 ， A - B * 旋转因子
			{
				ip=i+lei;
				t=a[ip]*u;   // B * 旋转因子
				a[ip]=a[i]-t; // A - B * 旋转因子
				a[i]+=t;  // A + B * 旋转因子
			}
			u*=w;
		}
	}

	return 0;
}
/*-----------------------------------------------  
           IFFT函数
    data:指向数据序列地指针
	a   :指向data的DFT序列的指针
	L   :2的L次方为FFT的点数
--------------------------------------------------*/

int ifft(complex <double> *a,int *data,int L)  // 基本思想：同FFT，只是旋转因子为e^(jw),均除以N 输入为X(k)
{
	complex <double> u;
	complex <double> w;
	complex <double> t;
	unsigned n=1,nv2,nm1,k,le,lei,ip;
	int i,j,m,number;
	double tmp;
	n<<=L;
	nv2=n>>1;
	nm1=n-1;
	j=0;
	for(i=0;i<nm1;i++)
	{
		if(i<j)
		{
			t=a[j];
			a[j]=a[i];
			a[i]=t;
		}
		k=nv2;
		while(k<=j)
		{
			j-=k;
			k>>=1;
		}
		j+=k;
	}
	le=1;
	for(m=1;m<=L;m++)
	{
		lei=le;
		le<<=1;
		u=complex<double>(1,0);
		tmp=PI/lei;
		w=complex<double>(cos(tmp),sin(tmp));
		for(j=0;j<lei;j++)
		{
			for(i=j;i<n;i+=le)
			{
				ip=i+lei;
				t=a[ip]*u;
				a[ip]=a[i]-t;
				a[i]+=t;
			}
			u*=w;
		}
	}
	for(number = 0; number<n; number++)
	{
		data[number] = ceil(a[number].real())/n;
		a[number] = a[number]/complex<double>(n,0);  // 除以N
	}
	return 0;
}

/*--------------------------------------------------------------
                     Hilbert变换函数
	data：指向信号序列的指针
	filterdata：指向包络序列的指针
	dn：信号序列的点数
----------------------------------------------------------------*/
int   hilbert(int * data , int *filterdata,int dn)
{
	int i = 0, j = 0, k = 0,l = 0,N = 0;
	complex<double> *zk;
	int *ldata;
	l = (int)(log(dn)/log(2))+1;
	N =(int) pow(2,l);
	zk = (complex<double>*)malloc(N*sizeof(complex<double>));
	ldata = (int *)malloc(N*sizeof(int));
	memcpy(ldata,data,dn*sizeof(int));
	for(i=dn;i<N;i++)
	{
		ldata[i] = 0;
	}
	fft(ldata,zk,l);
	for(i=0;i<N;i++)
	{
		if(i>=1 && i<=N/2-1)
		{
			zk[i] = complex<double>(2,0)*zk[i];
		}
		if(i>=N/2 && i<=N-1)
		{
			zk[i]= complex<double> (0,0);
		}
	}
	ifft(zk,ldata,l);
    for(i = 0 ;i<dn;i++)
	{
		filterdata[i] = (int)sqrt(pow(zk[i].imag(),2)+pow(zk[i].real(),2));
	}
	free(zk);
	free(ldata);
	return 0;
}

/////////////////////////////////////////////////////////////
int  conv(int *h,int *data,int *result,int hn,int dn)
{
	int l,i,j,k,N;
	complex<double> *hk,*datak;
	l = (int)(log(hn+dn-1)/log(2))+1;
	N =(int) pow(2,l);
	int *lh,*ldata;
	lh =(int*)malloc(N*sizeof(int));
	ldata =(int*)malloc(N*sizeof(int));
	memcpy(lh,h,hn*sizeof(int));
	memcpy(ldata,data,dn*sizeof(int));
	for(i=hn;i<N;i++)
	{
		lh[i] = 0;
	}
	for(j=dn;j<N;j++)
	{
		ldata[j] = 0;
	}
    hk = (complex <double> *) malloc(N*sizeof(complex<double>));
    datak = (complex <double> *) malloc(N*sizeof(complex<double>));
	fft(lh,hk,l);
	fft(ldata,datak,l);
	for(k=0;k<N;k++)
	{
		datak[k] = datak[k]*hk[k];
	}
	ifft(datak,result,l);

	free(lh);
	free(ldata);
	free(hk);
	free(datak);
	return 0;
}

////////////////////////////////////////////////////////////////// 数据类型不同  double类型
int fft_f(double *data,complex <double> *a,int L)
{
	complex <double> u;
	complex <double> w;
	complex <double> t;
	unsigned n=1,nv2,nm1,k,le,lei,ip;
	int i,j,m,number;
	double tmp;
	n<<=L;
	for(number = 0; number<n; number++)
	{
		a[number] = complex <double> (data[number],0);
	}
	nv2=n>>1;
	nm1=n-1;
	j=0;
	for(i=0;i<nm1;i++)
	{
		if(i<j)
		{
			t=a[j];
			a[j]=a[i];
			a[i]=t;
		}
		k=nv2;
		while(k<=j)
		{
			j-=k;
			k>>=1;
		}
		j+=k;
	}
	le=1;
	for(m=1;m<=L;m++)
	{
		lei=le;
		le<<=1;
		u=complex<double>(1,0);
		tmp=PI/lei;
		w=complex<double>(cos(tmp),-sin(tmp));
		for(j=0;j<lei;j++)
		{
			for(i=j;i<n;i+=le)
			{
				ip=i+lei;
				t=a[ip]*u;
				a[ip]=a[i]-t;
				a[i]+=t;
			}
			u*=w;
		}
	}

	return 0;
}
// 我修改的 使fft结果为其abs  
int fft_my(double *source,double *result,int N)  // N 为序列数据点数
{
	complex <double> u;
	complex <double> w;
	complex <double> t;

    int L;
	L =  (int)(log(N)/log(2))+1;


	unsigned n=1,nv2,nm1,k,le,lei,ip;
	int i,j,m,number;
	double tmp;
	n<<=L;  // FFT点数

	complex <double> *a;
//	a = (complex <double> *) malloc(n*sizeof(complex<double>));
	a = new complex <double>[n];

	double *data;
	data =(double*)malloc(n*sizeof(double));
	memcpy(data,source,N*sizeof(double));

	for(i=N;i<n;i++)
	{
		data[i] = 0;
	}


	for(number = 0; number<n; number++)
	{
		a[number] = complex <double> (data[number],0);
	}
	nv2=n>>1;
	nm1=n-1;
	j=0;
	for(i=0;i<nm1;i++)
	{
		if(i<j)
		{
			t=a[j];
			a[j]=a[i];
			a[i]=t;
		}
		k=nv2;
		while(k<=j)
		{
			j-=k;
			k>>=1;
		}
		j+=k;
	}
	le=1;
	for(m=1;m<=L;m++)
	{
		lei=le;
		le<<=1;
		u=complex<double>(1,0);
		tmp=PI/lei;
		w=complex<double>(cos(tmp),-sin(tmp));
		for(j=0;j<lei;j++)
		{
			for(i=j;i<n;i+=le)
			{
				ip=i+lei;
				t=a[ip]*u;
				a[ip]=a[i]-t;
				a[i]+=t;
			}
			u*=w;
		}
	}

	for(int num = 0; num<n; num++)
	{
		result[num] = sqrt(pow(a[num].real(),2)+pow(a[num].imag(),2));
	}
	//free(a);
	delete[] a;
	a = NULL;
	delete[] data;
	data = NULL;
	return 0;
}

/////////////////////////////////////////////////////////////////
// h:滤波器单位脉冲响应函数  data:滤波器输入  result：滤波器输出  hn:滤波器长度  dn:数据长度
int conv_f(double *h,int *data,int *result,int hn,int dn)
{
	int l,i,j,k,N;
	complex<double> *hk,*datak;
	l = (int)(log(hn+dn-1)/log(2))+1;
	N =(int) pow(2,l);
	int *ldata;
	double *lh;
	int *raw;
	lh =(double*)malloc(N*sizeof(double));
	ldata =(int*)malloc(N*sizeof(int));
    raw=(int*)malloc(N*sizeof(int));
	memcpy(lh,h,hn*sizeof(double));
	memcpy(ldata,data,dn*sizeof(int));
	for(i=hn;i<N;i++)
	{
		lh[i] = 0;
	}
	for(j=dn;j<N;j++)
	{
		ldata[j] = 0;
	}
    hk = (complex <double> *) malloc(N*sizeof(complex<double>));
    datak = (complex <double> *) malloc(N*sizeof(complex<double>));
	fft_f(lh,hk,l);
	fft(ldata,datak,l);
	for(k=0;k<N;k++)
	{
		datak[k] = datak[k]*hk[k];
	}
	ifft(datak,raw,l);
	for(k=0;k<dn;k++)
	{
		result[k] = raw[k+hn/2];
	}
	free(raw);
	free(lh);
	free(ldata);
	free(hk);
	free(datak);
	return 0;
}
/*-------------------------------------------------------------
                           求相关
-------------------------------------------------------------*/
double  corr(double *h, double *data,int *result,int hn,int dn)
{
	int l,i,k,N;
	complex<double> *hk,*datak;
	l = (int)(log(hn+dn-1)/log(2))+1;   
	N =(int) pow(2,l);               // 长度 N > (N1 + N2 -1) ,且为 2^(l)
	double *lh,*ldata;
	lh =(double*)malloc(N*sizeof(double));
	ldata =(double*)malloc(N*sizeof(double));
	memcpy(lh,h,hn*sizeof(double));         // 序列1
	memcpy(ldata,data,dn*sizeof(double));  // 序列2
//	ZeroMemory(lh[dn-1],8*(N-dn));
	for(i=hn;i<N;i++)  // 补零  此处要求 hn = dn 
	{
		lh[i] = 0;
		ldata[i] = 0;
	}
    hk = (complex <double> *) malloc(N*sizeof(complex<double>));
    datak = (complex <double> *) malloc(N*sizeof(complex<double>));
	fft_f(lh,hk,l);  // 对序列1做fft
	fft_f(ldata,datak,l); // 对序列2做fft
	for(k=0;k<N;k++)
	{
		datak[k].real(datak[k].real());
		datak[k].imag(-datak[k].imag());
		datak[k] = datak[k]*hk[k];  // 序列1的fft 的共轭 * 序列2的fft 
	}
	ifft(datak,result,l);  // 求IFFT 互相关结果保存在result中

	free(lh);
	lh = NULL;

	free(ldata);
	ldata = NULL;

	free(hk);
	hk = NULL;

	free(datak);  
	datak = NULL;

	return 0;
}
/////////////////////////////////////////////////
//type：整型变量，窗函数的类型，取值1至7，分别对应矩形窗、图基窗、三角窗、
//	    汉宁窗、海明窗、布拉克曼窗和凯塞窗
/////////////////////////////////////////////////
double window(int type,int n,int i,double beta)
{
	int k;
	double w=1.0;
	switch(type)
	{
	case 1:
		{
			w=1.0;
			break;
		}
	case 2:
		{
			k=(n-2)/10;
			if(i<=k) w=0.5*(1.0-cos(i*PI/(k+1)));
			if(i>n-k-2) w=0.5*(1.0-cos((n-i-1)*PI/(k+1)));
			break;
		}
	case 3:
		{
			w=1.0-fabs(1.0-2*i/(n-1.0));
			break;
		}
	case 4:
		{
			w=0.5*(1.0-cos(2*i*PI/(n-1.0)));
			//w=1.0-cos(2*i*PI/(n-1.0));
			break;
		}
	case 5:
		{
			w=0.54-0.46*cos(2*i*PI/(n-1.0));
			break;
		}
	case 6:
		{
			w=0.42-0.5*cos(2*i*PI/(n-1.0))+0.08*cos(4*i*PI/(n-1.0));
			break;
		}
	case 7:
		{
			w=kaiser(i,n,beta);
			break;
		}
	}
	return(w);
}

double kaiser(int i,int n,double beta)
{
	double a,w,a2,b1,b2,beta1;
	b1=bessel0(beta);
	a=2.0*i/(double)(n-1)-1.0;
	a2=a*a;
	beta1=beta*sqrt(1.0-a2);
	b2=bessel0(beta1);
	w=b2/b1;
	return(w);
}

double bessel0(double x)
{
	int i;
	double d,y,d2,sum;
	y=x/2.0;
	d=1.0;
	for(i=1;i<=25;i++)
	{
		d=d*y/i;
		d2=d*d;
		sum=sum+d2;
		if(d2<sum*(1.0e-8)) break;
	}
	return(sum);
}

///////////////////////////////////////////////////////////////////////////////////////////
// n 为 滤波器长度  band：滤波器类型：低通 高通 带通 带阻  fl\fh   fs:采样频率  wn:窗函数类型  data：滤波器输入 result：滤波器输出  dn: 数据长度
int  firwin_e(int n,int band,int fl,int fh,int fs,int wn, int *data,int *result,int dn)
{
	int i,n2,mid;
	double *h;
	double sum = 0;
	double s,wc1,wc2,beta,delay;
	beta=0.0;
	double fln = (double)fl / fs;
	double fhn = (double)fh / fs;

	h = (double *)malloc((n+1)*sizeof(double));
	beta = 6;
	if((n%2)==0)
	{
		n2=n/2-1;
		mid=1;
	}
	else
	{
		n2=n/2;
		mid=0;
	}
	delay=n/2.0;
	wc1=2.0*PI*fln;
	if(band>=3) wc2=2.0*PI*fhn;
	switch(band)
	{
	case 1://低通
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(wc1*s)/(PI*s))*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=wc1/PI;
			for(i=0;i<=n;i++)
			{
				sum=sum+*(h+i);
			}
			for(i=0;i<=n;i++)
			{
				*(h+i)=*(h+i)/fabs(sum);
			}
			break;
		}

	case 2: //高通
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(PI*s)-sin(wc1*s))/(PI*s);
				*(h+i)=*(h+i)*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=1.0-wc1/PI;
			break;
		}
	case 3: //带通
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(wc2*s)-sin(wc1*s))/(PI*s);
				*(h+i)=*(h+i)*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=(wc2-wc1)/PI;
			break;
		}
	case 4: //带阻
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(wc1*s)+sin(PI*s)-sin(wc2*s))/(PI*s);
				*(h+i)=*(h+i)*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=(wc1+PI-wc2)/PI;
			break;
		}
	}
	conv_f(h,data,result,n+1,dn);
	free(h);
	return 0;
}
///////////////////////////////////////////

/*---------------------------------------------------------------
              求坐标函数(三维）
lresult存放测阵的坐标
lresult:0,1,2存放第一个水听器的三维坐标
lresult:3,4,5存放第二个水听起的三维坐标
依此类推

ldata存放测阵的距离
ldata[18]
ldata[0],ldata[1],ldata[2]存放第一个水听器到2号，3号和6号的距离
ldata[3],ldata[4],ldata[5]存放第二个水听器到2号，3号和6号的距离
依此类推
----------------------------------------------------------------*/
double coordinate(double *data,double *result,int dn)
{
	int i;
	double theta;
	double A,B,C,D,E,F,G,H;
	double AA,BB,CC,DD,EE,FF,GG,HH;
	double *ldata,*lresult;
   	ldata =(double*)malloc(dn*sizeof(double));
	lresult =(double*)malloc(dn*sizeof(double));
	memcpy(ldata,data,dn*sizeof(double));
	for(i=0;i<3;i++)
	{  
    	lresult[i]=0;
	}  
	
    lresult[3]=-ldata[0];
    lresult[4]=0;
    lresult[5]=0;
	
    theta=acos((ldata[0]*ldata[0]+ldata[1]*ldata[1]-ldata[4]*ldata[4])/(2*ldata[0]*ldata[1]));
    lresult[6]=-ldata[1]*cos(theta); 
    lresult[7]=ldata[1]*sin(theta);
	lresult[8]=0;

    lresult[15]=(ldata[2]*ldata[2]-ldata[5]*ldata[5]+lresult[3]*lresult[3])/(2*lresult[3]);
    lresult[16]=(ldata[2]*ldata[2]-ldata[8]*ldata[8]+lresult[6]*lresult[6]+lresult[7]*lresult[7])/(2*lresult[7])-lresult[15]*lresult[6]/lresult[7];
    lresult[17]=-sqrt(ldata[2]*ldata[2]-lresult[15]*lresult[15]-lresult[16]*lresult[16]);
	
    A=(ldata[9]*ldata[9]-ldata[10]*ldata[10]-lresult[3]*lresult[3]+lresult[6]*lresult[6]+lresult[7]*lresult[7])/(2*lresult[7]);
	B=(lresult[3]-lresult[6])/lresult[7];
    C=ldata[11]*ldata[11]-ldata[9]*ldata[9]-lresult[15]*lresult[15]-lresult[16]*lresult[16]-lresult[17]*lresult[17]+lresult[3]*lresult[3]+lresult[4]*lresult[4];
    D=((lresult[3]-lresult[15])-B*(lresult[4]-lresult[16]))/lresult[17];
    E=(2*A*(lresult[4]-lresult[16])-C)/(2*lresult[17]);
    F=1+B*B+D*D;
    G=2*(lresult[7]*B-A*B+D*E-lresult[6]);
    H=lresult[6]*lresult[6]+A*A-2*A*lresult[7]+lresult[7]*lresult[7]+E*E-ldata[10]*ldata[10];
    lresult[9]=(-G-sqrt(G*G-4*F*H))/(2*F); 
    lresult[10]=A-lresult[9]*B;
    lresult[11]=D*lresult[9]+E;
	
    AA=(ldata[12]*ldata[12]-ldata[13]*ldata[13]-lresult[3]*lresult[3]+lresult[6]*lresult[6]+lresult[7]*lresult[7])/(2*lresult[7]);
    BB=(lresult[3]-lresult[6])/lresult[7];
    CC=ldata[14]*ldata[14]-ldata[12]*ldata[12]-lresult[15]*lresult[15]-lresult[16]*lresult[16]-lresult[17]*lresult[17]+lresult[3]*lresult[3]+lresult[4]*lresult[4];
    DD=((lresult[3]-lresult[15])-BB*(lresult[4]-lresult[16]))/lresult[17];
    EE=(2*AA*(lresult[4]-lresult[16])-CC)/(2*lresult[17]);
    FF=1+BB*BB+DD*DD;
    GG=2*(lresult[7]*BB-AA*BB+DD*EE-lresult[6]);
    HH=lresult[6]*lresult[6]+AA*AA-2*AA*lresult[7]+lresult[7]*lresult[7]+EE*EE-ldata[13]*ldata[13];
    lresult[12]=(-GG+sqrt(GG*GG-4*FF*HH))/(2*FF); 
	lresult[13]=AA+lresult[12]*BB;
    lresult[14]=DD*lresult[12]+EE;
	
	memcpy(result,lresult,dn*sizeof(double));
	free(ldata);
	free(lresult);
	return 0;
}

/*---------------------------------------------------------------
              求坐标函数(二维）
lresult存放测阵的坐标
lresult[18]
lresult:0,1存放第一个水听器的二维坐标
lresult:3,4存放第二个水听起的二维坐标
依此类推

ldata存放测阵的距离
ldata[18]
ldata[0],ldata[1]存放第一个水听器到2号，3号的距离
ldata[3],ldata[4]存放第二个水听器到2号，3号的距离
z方向坐标均为零
依此类推
----------------------------------------------------------------*/
double coordinate_2(double *data,double *result,int dn)
{
	double theta;
	double A,B,C,D,E;
	double AA,BB,CC,DD,EE;
	double *ldata,*lresult;
   	ldata =(double*)malloc(dn*sizeof(double));
	lresult =(double*)malloc(dn*sizeof(double));
	memcpy(ldata,data,dn*sizeof(double));

    lresult[0]=0;
	lresult[1]=0;
	lresult[2]=0;
 
    lresult[3]=-ldata[0];
    lresult[4]=0;
	lresult[5]=0;
	
    theta=acos((ldata[0]*ldata[0]+ldata[1]*ldata[1]-ldata[4]*ldata[4])/(2*ldata[0]*ldata[1]));
	if (theta < PI/2.0)
	{
		lresult[6]=ldata[1]*cos(theta); 
		lresult[7]=ldata[1]*sin(theta);
	}
	else
	{
		lresult[6]=ldata[1]*cos(PI-theta); 
		lresult[7]=ldata[1]*sin(PI-theta);
	}
	lresult[8]=0;
	
	A=(ldata[9]*ldata[9]-ldata[10]*ldata[10]-lresult[3]*lresult[3]+lresult[6]*lresult[6]+lresult[7]*lresult[7])/(2*lresult[7]);
	B=(lresult[6]-lresult[3])/lresult[7];
	C=1+B*B;
	D=-2*(A*B+lresult[3]);
	E=lresult[3]*lresult[3]+A*A-ldata[9]*ldata[9];
	lresult[9]=(-D+sqrt(D*D-4*C*E))/(2*C);
	lresult[10]=A-B*lresult[9];
	lresult[11]=0;

	AA=(ldata[12]*ldata[12]-ldata[13]*ldata[13]-lresult[3]*lresult[3]+lresult[6]*lresult[6]+lresult[7]*lresult[7])/(2*lresult[7]);
	BB=(lresult[6]-lresult[3])/lresult[7];
	CC=1+BB*BB;
	DD=-2*(AA*BB+lresult[3]);
	EE=lresult[3]*lresult[3]+AA*AA-ldata[12]*ldata[12];
	lresult[12]=(-DD+sqrt(DD*DD-4*CC*EE))/(2*CC);
	lresult[13]=AA-BB*lresult[12];
	lresult[14]=0;
	
	memcpy(result,lresult,dn*sizeof(double));
	free(ldata);
	free(lresult);
	return 0;
}


/*---------------------------------------------------------------
                        求相关的最大值
----------------------------------------------------------------*/

int maxvalue(int *data,int dn)
{
	int i,j,temp;
	int *ldata;
   	ldata =(int*)malloc(dn*sizeof(int));
	memcpy(ldata,data,dn*sizeof(int));
	temp=0;
	for(i=0;i<dn;i++)
	{
		if(temp<ldata[i])
		{
			temp=ldata[i];
			j=i;
		}
	}

    free (ldata);
	return j;
     
}

/*---------------------------------------------------------------
                        求相关的最大值(修正)
----------------------------------------------------------------*/

int maxvalue_max(int *data,int dn)
{
	int i,j,temp;
	int *ldata;
   	ldata =(int*)malloc(dn*sizeof(int));
	memcpy(ldata,data,dn*sizeof(int));
	temp=0;
	for(i=dn/2-1500;i<dn/2+1500;i++)
	{
		if(temp<ldata[i])
		{
			temp=ldata[i];
			j=i;
		}
	}

    free (ldata);
	return j;
     
}

/*---------------------------------------------------------------
                        实矩阵相乘
----------------------------------------------------------------*/
void multiply(double *a,double *b,int m,int n,int k,double *c)
  {
      int i,j,l,u;
      for (i=0; i<=m-1; i++)
		  for (j=0; j<=k-1; j++)
      {
			  u=i*k+j; c[u]=0.0;
             for (l=0; l<=n-1; l++)
                c[u]=c[u]+a[i*n+l]*b[l*k+j];
      }
    return;
  }


/*---------------------------------------------------------------
                        实对称正定矩阵的逆矩阵
----------------------------------------------------------------*/

int inv(double *a,int n)
  { 
	  int i,j,k,m;
      double w,g,*b;
      b=(double*)malloc(n*sizeof(double));
    for (k=0; k<=n-1; k++)
      { w=a[0];
        if (fabs(w)+1.0==1.0)
          { free(b); printf("fail\n"); return(-2);}
           m=n-k-1;
        for (i=1; i<=n-1; i++)
          { g=a[i*n]; b[i]=g/w;
            if (i<=m) b[i]=-b[i];
            for (j=1; j<=i; j++)
              a[(i-1)*n+j-1]=a[i*n+j]+g*b[j];
          }
              a[n*n-1]=1.0/w;
        for (i=1; i<=n-1; i++)
          a[(n-1)*n+i-1]=b[i];
      }
    for (i=0; i<=n-2; i++)
    for (j=i+1; j<=n-1; j++)
      a[i*n+j]=a[j*n+i];
      free(b);
      return(2);
  }

//////////////////////////////////////////////////////////////////
int  cpsd (int *x, double *psd, double *angle,int dn)
{
	int l,N,k;
	double maxk=-1;
	double w;
	int pos;
//	double angle ;
    int *xx;//*yy;
	complex<double> *xk;//*yk,*yhk;
	l = (int)(log(dn)/log(2))+1;
	N =(int) pow(2,l);
	xx = (int *)malloc(N*sizeof(int));
//	yy = (int *)malloc(N*sizeof(int));
    xk = (complex <double> *) malloc(N*sizeof(complex<double>));
//   yk = (complex <double> *) malloc(N*sizeof(complex<double>));
//   yhk = (complex <double> *) malloc(N*sizeof(complex<double>));
    memcpy(xx,x,dn*sizeof(int));
//	memcpy(yy,y,dn*sizeof(int));
	for(k=0;k<dn;k++)
	{
		w=1-1.93*cos(2*PI*k/dn)+1.29*cos(4*PI*k/dn)-0.388*cos(6*PI*k/dn)+0.0322*cos(8*PI*k/dn);
		xx[k]=xx[k]*w;
//		yy[k]=yy[k]*w;
	}
	for(k=dn;k<N;k++)
	{
		xx[k]=0;
//		yy[k]=0;
	}
	fft(xx,xk,l);
//	fft(yy,yk,l);
	for(k=0;k<N/2;k++)
	{
//		yhk[k] = complex<double> (yk[k].real(),-(yk[k].imag()));
//		xk[k] = xk[k] * yhk[k];
		psd[k] = sqrt(xk[k].real()*xk[k].real()+xk[k].imag()*xk[k].imag());
//      psd[k] = xk[k]*yhk[k];
//      psdm = sqrt(psd[k].real()*psd[k].real()+psd[k].imag()*psd[k].imag());
		if (psd[k]>maxk) 
		{
			maxk = psd[k];
			pos = k;
		}
	}
	if(xk[pos].imag()>0 && xk[pos].real()>0)
	{
       *(angle) = atan(xk[pos].imag()/xk[pos].real())/PI*180;
	}
	if(xk[pos].imag()<0 && xk[pos].real()>0)
	{
       //*(angle) = (atan(xk[pos].imag()/xk[pos].real())+2*PI)/PI*180;
		*(angle) = atan(xk[pos].imag()/xk[pos].real())/PI*180;
	}
	if(xk[pos].imag()>0 && xk[pos].real()<0)
	{
       *(angle) = (atan(xk[pos].imag()/xk[pos].real())+PI)/PI*180;
	}
	if(xk[pos].imag()<0 && xk[pos].real()<0)
	{
       //*(angle) = (atan(xk[pos].imag()/xk[pos].real())+PI)/PI*180;
		*(angle) = (atan(xk[pos].imag()/xk[pos].real())-PI)/PI*180;
	}
	free(xk);
//	free(yk);
//	free(yhk);
	free(xx);
//	free(yy);
	return 0;
}
///////////////////////////////////////////////////////////
int  filterdsg(int n,int band,int fl,int fh,int fs,double *hf)
{
	int i,n2,mid;
	double h[1024];
	complex <double> hk[1024];
	double sum = 0;
	double s,wc1,wc2,beta,delay;
	int wn = 5;
	beta=0.0;
	double fln = (double)fl / fs;
	double fhn = (double)fh / fs;
//    h = (double *)malloc(1024*sizeof(double));      
//	hk = (complex <double> *)malloc(1024*sizeof(double));
	beta = 6;
	if((n%2)==0)
	{
		n2=n/2-1;
		mid=1;
	}
	else
	{
		n2=n/2;
		mid=0;
	}
	delay=n/2.0;
	wc1=2.0*PI*fln;
	if(band>=3) wc2=2.0*PI*fhn;
	switch(band)
	{
	case 1://低通
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(wc1*s)/(PI*s))*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=wc1/PI;
			for(i=0;i<=n;i++)
			{
				sum=sum+*(h+i);
			}
			for(i=0;i<=n;i++)
			{
				*(h+i)=*(h+i)/fabs(sum);
			}
			break;
		}

	case 2: //高通
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(PI*s)-sin(wc1*s))/(PI*s);
				*(h+i)=*(h+i)*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=1.0-wc1/PI;
			break;
		}
	case 3: //带通
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(wc2*s)-sin(wc1*s))/(PI*s);
				*(h+i)=*(h+i)*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=(wc2-wc1)/PI;
			break;
		}
	case 4: //带阻
		{
			for (i=0;i<=n2;i++)
			{
				s=i-delay;
				*(h+i)=(sin(wc1*s)+sin(PI*s)-sin(wc2*s))/(PI*s);
				*(h+i)=*(h+i)*window(wn,n+1,i,beta);
				*(h+n-i)=*(h+i);
			}
			if(mid==1) *(h+n/2)=(wc1+PI-wc2)/PI;
			break;
		}
	}
	for(i=n+1;i<=1023;i++)
	{
		*(h+i) = 0;
	}
	fft_f(h,hk,10);
//	free(h);
	for(i=0;i<1024;i++)
	{
		*(hf+i)= 20*log10(sqrt(hk[i].real()*hk[i].real()+hk[i].imag()*hk[i].imag()));
	}
//	free(h);
//	free(hk);
	return 0;
}

int  freqspec (double *data, double *datak)
{
  complex <double> dk[2048] ;
  double w[2048];
  double dtemp[2048];
  int retV;
  int i;
  int n;
  int N = 2048;
  //dk=(complex<double> *)malloc(N*sizeof(complex<double>));
  //w1=1-1.93*cos(2*pi*n/N)+1.29*cos(4*pi*n/N)-0.388*cos(6*pi*n/N)+0.0322*cos(8*pi*n/N);
  for(n=0;n<=2047;n++)
  {
	w[n] = 1-1.93*cos(2*PI*n/N)+1.29*cos(4*PI*n/N)-0.388*cos(6*PI*n/N)+0.0322*cos(8*PI*n/N);
  }
  for(n=0;n<=2047;n++)
  {
	dtemp[n]=data[n]*w[n];
  }
  retV=fft_f(dtemp,dk,11);
  for(i=0;i<=2047;i++)
  {
	  *(datak+i) = sqrt(dk[i].real()*dk[i].real()+dk[i].imag()*dk[i].imag());
  }
  return 0;
}
//////////////////////////////////
int spec(int *data, double *datak,int l,int dn,int wn,double v,double ref)
{
  complex <double> *dk;
//  double *w;
  double *dtemp;
  int retV;
  int i;
  int k;
  int n;
  double max=0;
  n =(int) pow(2,l);
  dk=(complex <double> *)malloc(n*sizeof(complex <double>));
//  w=(double *)malloc(n*sizeof(double));
  dtemp=(double *)malloc(n*sizeof(double));
// for(k=0;k<=n-1;k++)
// {
//	w[k] = 1-1.93*cos(2*PI*k/n)+1.29*cos(4*PI*k/n)-0.388*cos(6*PI*k/n)+0.0322*cos(8*PI*k/n);
//  }
  if(n<=dn)
  {
	  for(k=0;k<=n-1;k++)
	  {
	     dtemp[k]=data[k]*window(wn,n,k,0.1);
	  }
  }
  else
  {
	 for(k=0;k<=dn-1;k++)
	 {
	     dtemp[k]=data[k]*window(wn,dn,k,0.1);
//		 dtemp[k]=data[k]*1.0;
	 }
	 for(k=dn;k<n;k++)
	 {
         dtemp[k]=0.0;
	 }
  }
  retV=fft_f(dtemp,dk,l);
  for(i=0;i<=n-1;i++)
  {
	  if (n<=dn)
	  {
	    *(datak+i) = sqrt(dk[i].real()*dk[i].real()+dk[i].imag()*dk[i].imag())*2/n*v;
	  }
	  else
	  {
	    *(datak+i) = sqrt(dk[i].real()*dk[i].real()+dk[i].imag()*dk[i].imag())*2/dn*v;
	  }
//	  if (*(datak+i)>max)
//	  {
//		  max=*(datak+i);
//	  }
  }
  for(i=0;i<=n-1;i++)
  {
		*(datak+i)=20*log10(*(datak+i)/ref);
  }
  free(dtemp);
  free(dk);
  return 0;
}
//////////////////////////////////
int  psd (double *data, double *psd)
{
  complex <double> dk[2048] ;
  double w[2048];
  double dtemp[2048];
  int retV;
  int i;
  int n;
  int N = 2048;
 
    for(n=0;n<=2047;n++)
  {
	w[n] = 1-1.93*cos(2*PI*n/N)+1.29*cos(4*PI*n/N)-0.388*cos(6*PI*n/N)+0.0322*cos(8*PI*n/N);
  }
  for(n=0;n<=2047;n++)
  {
	dtemp[n]=data[n]*w[n];
  }
  retV=fft_f(dtemp,dk,11);
  for(i=0;i<=2047;i++)
  {
	  *(psd+i) = 10*log10((dk[i].real()*dk[i].real()+dk[i].imag()*dk[i].imag())/N);
  }
  return 0;
}
//////////////////////////////////////////////////
  void hpir1(double x[],double y[],int n,double a[],int m,double dt[],double fit[],int flag)
  { 
	int i,j,k;
    double z,p,c,g,q,d1,d2,s[20],t[20],b[20];
    for (i=0; i<=m-1; i++) a[i]=0.0;
    if (m>n) m=n;
    if (m>20) m=20;
    z=0.0;
    for (i=0; i<=n-1; i++) z=z+x[i]/(1.0*n);
    b[0]=1.0; d1=1.0*n; p=0.0; c=0.0;
    for (i=0; i<=n-1; i++)
      { p=p+(x[i]-z); c=c+y[i];}
    c=c/d1; p=p/d1;
    a[0]=c*b[0];
    if (m>1)
      { t[1]=1.0; t[0]=-p;
        d2=0.0; c=0.0; g=0.0;
        for (i=0; i<=n-1; i++)
          { q=x[i]-z-p; d2=d2+q*q;
            c=c+y[i]*q;
            g=g+(x[i]-z)*q*q;
          }
        c=c/d2; p=g/d2; q=d2/d1;
        d1=d2;
        a[1]=c*t[1]; a[0]=c*t[0]+a[0];
      }
    for (j=2; j<=m-1; j++)
      { s[j]=t[j-1];
        s[j-1]=-p*t[j-1]+t[j-2];
        if (j>=3)
          for (k=j-2; k>=1; k--)
            s[k]=-p*t[k]+t[k-1]-q*b[k];
        s[0]=-p*t[0]-q*b[0];
        d2=0.0; c=0.0; g=0.0;
        for (i=0; i<=n-1; i++)
          { q=s[j];
            for (k=j-1; k>=0; k--)
              q=q*(x[i]-z)+s[k];
            d2=d2+q*q; c=c+y[i]*q;
            g=g+(x[i]-z)*q*q;
          }
        c=c/d2; p=g/d2; q=d2/d1;
        d1=d2;
        a[j]=c*s[j]; t[j]=s[j];
        for (k=j-1; k>=0; k--)
          { a[k]=c*s[k]+a[k];
            b[k]=t[k]; t[k]=s[k];
          }
      }
    dt[0]=0.0; dt[1]=0.0; dt[2]=0.0;
    for (i=0; i<=n-1; i++)
      { q=a[m-1];
        for (k=m-2; k>=0; k--)
		      q=a[k]+q*(x[i]-z);
        //fit[i]=q;
		if(flag==0)
		{
		   if(i<22)
		   {
			*(fit+i) = q;
		   }
		}
		if(flag==1)
		{
		   if(i<31)
		   {
			*(fit+i) = q;
		   }
		}
        p=q-y[i];
        if (fabs(p)>dt[2]) dt[2]=fabs(p);
        dt[0]=dt[0]+p*p;
        dt[1]=dt[1]+fabs(p);
      }
    return ;
  }
//////////////////////////////////////////////////
void findpeak(double *data, int *p,int dn)
{
	int acc=0,acc1=0;
	int i,j;
	double a0=0.0,a1=0.0;
	for(i=0;i<dn;i++)
	{
		a0=*(data+i);
		//先向前找
        for(j=1;j<11;j++)
		{
			if ((i+j)>=dn)
			{
				a1=*(data+i+j-dn);
			}
			else
			{
				a1=*(data+i+j);
			}
			if (a0>a1)
			{
				acc=acc+1;
			}
//			a0=a1;
		}
        a0=*(data+i);
		////再向后找
        for(j=1;j<11;j++)
		{
			if ((i-j)<0)
			{
				a1=*(data+i-j+dn);
			}
			else
			{
				a1=*(data+i-j);
			}
			if (a0>a1)
			{
				acc1=acc1+1;
			}
//			a0=a1;
		}
	  if ((acc==10) && (acc1==10))
	  {
         *(p+i)=1;
	  }
	  acc=0;
      acc1=0;
	}
}
//////////////////////////////////////////////////
void findvalley(double *data, int *p,int dn)
{
	int acc=0,acc1=0;
	int i,j;
	double a0=0.0,a1=0.0;
	for(i=0;i<dn;i++)
	{
		a0=*(data+i);
		//先向前找
        for(j=1;j<6;j++)
		{
			if ((i+j)>=dn)
			{
				a1=*(data+i+j-dn);
			}
			else
			{
				a1=*(data+i+j);
			}
			if (a0<a1)
			{
				acc=acc+1;
			}
//			a0=a1;
		}
        a0=*(data+i);
		////再向后找
        for(j=1;j<6;j++)
		{
			if ((i-j)<0)
			{
				a1=*(data+i-j+dn);
			}
			else
			{
				a1=*(data+i-j);
			}
			if (a0<a1)
			{
				acc1=acc1+1;
			}
//			a0=a1;
		}
	  if ((acc==5) && (acc1==5))
	  {
         *(p+i)=1;
	  }
	  acc=0;
      acc1=0;
	}
}
//////////////////////////////////////////////////
void findMaxValue(double *data,double *angle,int *p,int dn,int *max1,int *maxL1,int *maxL2,int *maxR1,int *maxR2)
{
	double max=0;
	int i;
    for (i=0;i<dn;i++)
	{
	   if (*(data+i)>max)
	   {
          max=*(data+i);
          *(max1)=i;
	   }
	}
    
	if(*(angle+dn-1) > *(angle))
	{
		if(*(max1) == 0)
		{
			*(maxR1) = -1;
			*(maxR2) = -1;
			for(i=*(max1)+1;i<dn;i++)
			{
				if(*(p+i) == 1)
				{
					*(maxL1) = i;
					break;
				}
			}
			if(i == dn )
			{
				*(maxL1) = -1;
				*(maxL2) = -1;
			}
			else
			{
			   if(*(maxL1)<=dn-2)
			   {
					for(i=*(maxL1)+1;i<dn;i++)
					{
						if(*(p+i) == 1)
						{
							*(maxL2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxL2) = -1;
			   }
			   if(i == dn)
			   {
				   *(maxL2) = -1;
			   }
			}
		}
		////////////////////////
		if(*(max1) == dn-1)
		{
			*(maxL1) = -1;
			*(maxL2) = -1;
			for(i=*(max1)-1;i>=0;i--)
			{
				if(*(p+i) == 1)
				{
					*(maxR1) = i;
					break;
				}
			}
			if(i < 0 )
			{
				*(maxR1) = -1;
				*(maxR2) = -1;
			}
			else
			{
			   if(*(maxR1)>=1)
			   {
					for(i=*(maxR1)-1;i>=0;i--)
					{
						if(*(p+i) == 1)
						{
							*(maxR2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxR2) = -1;
			   }
			   if(i < 0)
			   {
				   *(maxR2) = -1;
			   }
			}
		}
		///////////////////////
    	if(*(max1) != dn-1 && *(max1) != 0)
		{
			//先找左旁瓣
			for(i=*(max1)+1;i<dn;i++)
			{
				if(*(p+i) == 1)
				{
					*(maxL1) = i;
					break;
				}
			}
			if(i == dn )
			{
				*(maxL1) = -1;
				*(maxL2) = -1;
			}
			else
			{
			   if(*(maxL1)<=dn-2)
			   {
					for(i=*(maxL1)+1;i<dn;i++)
					{
						if(*(p+i) == 1)
						{
							*(maxL2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxL2) = -1;
			   }
			   if(i == dn)
			   {
				   *(maxL2) = -1;
			   }
			}
			//再找右旁瓣
			for(i=*(max1)-1;i>=0;i--)
			{
				if(*(p+i) == 1)
				{
					*(maxR1) = i;
					break;
				}
			}
			if(i < 0 )
			{
				*(maxR1) = -1;
				*(maxR2) = -1;
			}
			else
			{
			   if(*(maxR1)>=1)
			   {
					for(i=*(maxR1)-1;i>=0;i--)
					{
						if(*(p+i) == 1)
						{
							*(maxR2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxR2) = -1;
			   }
			   if(i < 0)
			   {
				   *(maxR2) = -1;
			   }
			}
		}

	}
	else//起始角大于终止角
	{
		if(*(max1) == 0)
		{
			*(maxL1) = -1;
			*(maxL2) = -1;
			for(i=*(max1)+1;i<dn;i++)
			{
				if(*(p+i) == 1)
				{
					*(maxR1) = i;
					break;
				}
			}
			if(i == dn )
			{
				*(maxR1) = -1;
				*(maxR2) = -1;
			}
			else
			{
			   if(*(maxR1)<=dn-2)
			   {
					for(i=*(maxR1)+1;i<dn;i++)
					{
						if(*(p+i) == 1)
						{
							*(maxR2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxR2) = -1;
			   }
			   if(i == dn)
			   {
				   *(maxR2) = -1;
			   }
			}
		}
		////////////////////////
		if(*(max1) == dn-1)
		{
			*(maxR1) = -1;
			*(maxR2) = -1;
			for(i=*(max1)-1;i>=0;i--)
			{
				if(*(p+i) == 1)
				{
					*(maxL1) = i;
					break;
				}
			}
			if(i < 0 )
			{
				*(maxL1) = -1;
				*(maxL2) = -1;
			}
			else
			{
			   if(*(maxL1)>=1)
			   {
					for(i=*(maxL1)-1;i>=0;i--)
					{
						if(*(p+i) == 1)
						{
							*(maxL2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxL2) = -1;
			   }
			   if(i < 0)
			   {
				   *(maxL2) = -1;
			   }
			}
		}
		///////////////////////
    	if(*(max1) != dn-1 && *(max1) != 0)
		{
			//先找右旁瓣
			for(i=*(max1)+1;i<dn;i++)
			{
				if(*(p+i) == 1)
				{
					*(maxR1) = i;
					break;
				}
			}
			if(i == dn )
			{
				*(maxR1) = -1;
				*(maxR2) = -1;
			}
			else
			{
			   if(*(maxR1)<=dn-2)
			   {
					for(i=*(maxR1)+1;i<dn;i++)
					{
						if(*(p+i) == 1)
						{
							*(maxR2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxR2) = -1;
			   }
			   if(i == dn)
			   {
				   *(maxR2) = -1;
			   }
			}
			//再找左旁瓣
			for(i=*(max1)-1;i>=0;i--)
			{
				if(*(p+i) == 1)
				{
					*(maxL1) = i;
					break;
				}
			}
			if(i < 0 )
			{
				*(maxL1) = -1;
				*(maxL2) = -1;
			}
			else
			{
			   if(*(maxL1)>=1)
			   {
					for(i=*(maxL1)-1;i>=0;i--)
					{
						if(*(p+i) == 1)
						{
							*(maxL2) = i;
							break;
						}
					}
			   }
			   else
			   {
				   *(maxL2) = -1;
			   }
			   if(i < 0)
			   {
				   *(maxL2) = -1;
			   }
			}
		}
	}

}
/////////////////////////////////////////////////
double interLagrange(double x0,double x1,double x2,double y0,double y1,double y2,double t)
{
	double lt;
	lt=y0*(t-x1)*(t-x2)/((x0-x1)*(x0-x2))+y1*(t-x0)*(t-x2)/((x1-x0)*(x1-x2))+y2*(t-x0)*(t-x1)/((x2-x1)*(x2-x0));
	return lt;
}
//////////////////////////////////////////////////
void BeamWidth(double *data,double *angle,int pos,double kk,int *v,int dn,double *width)
{
	double a0,a1,a2;
	int j=1;
	int p1=0,p2=0;
	double pos1=0,pos2=0;
	double pl=0,pr=0;
	int z=0;
//	int pp[481];
	int flag=0;
	double ep=0.1;
	double h=0.01;
    double d=1;
	double interpp=0.0;
	double x0,x1,x2,y0,y1,y2;
	int ang1=0,ang2=0;
	double left3db=0,right3db=0;
	int loopnumber =0 ;
//	FILE *fp;
//	fp=fopen("F:/result_beam.d","w");
//	for(i=0;i<481;i++)
//	{
//		pp[i]=*(v+i);
//	}
	a0=*(data+pos);
	ep = a0/100;
	while(1)
	{
		if (pos+j>=dn)
		{
			a1=*(data+pos+j-dn);
			z=pos+j-dn;
		}
		else
		{
			a1=*(data+pos+j);
            z=pos+j;
		}
		if (z+1>=dn)
		{
			a2=*(data+z+1-dn);
		}
		else
		{
			a2=*(data+z+1);
		}
		if((a1<=a0*kk) && (a1>a2))
		{
			p1=z;
			break;
		}
		j=j+1;
		if (*(v+z)==1)
		{
			flag=1;
			break;
		}
    }
	a0=*(data+pos);
	j=1;
	z=0;
	while(1)
	{
		if (pos-j<0)
		{
			a1=*(data+pos-j+dn);
			z=pos-j+dn;
		}
		else
		{
			a1=*(data+pos-j);
            z=pos-j;
		}
		if (z-1<0)
		{
			a2=*(data+z-1+dn);
		}
		else
		{
			a2=*(data+z-1);
		}
		if((a1<=a0*kk) && (a1>a2))
		{
			p2=z;
			break;
		}
		j=j+1;
		if (*(v+z)==1)
		{
			flag=1;
			break;
		}
    }
  if (flag !=1)
  {
	pos1=p1-(a0*kk-*(data+p1))/(*(data+p1-1)-*(data+p1));
	pos2=p2+(a0*kk-*(data+p2))/(*(data+p2+1)-*(data+p2));
//	fprintf(fp,"%f\n",pos1);
//	fprintf(fp,"%f\n",pos2);

////找右三分贝点///////////////////
	if (fabs(p1-1-pos1) > fabs(p1-pos1))
	{
		x0=p1-1;
		x1=p1;
		x2=p1+1;
		y0=*(data+p1-1);
		y1=*(data+p1);
        y2=*(data+p1+1);
	}
	if (fabs(p1-1-pos1) < fabs(p1-pos1))
	{
		x0=p1-2;
		x1=p1-1;
		x2=p1;
		y0=*(data+p1-2);
		y1=*(data+p1-1);
        y2=*(data+p1);
	}
    while(pos1<p1+1)
	{
		interpp=interLagrange(x0,x1,x2,y0,y1,y2,pos1);
		d=fabs(interpp-a0*kk);
		loopnumber = loopnumber+1;
		if (loopnumber <= 20000)
		{
		  if(d<ep)
		  {
		      ep=d;
			  pr=pos1;
		  }

		  pos1=pos1+h;
		}
		else
		{
			break;
		}
	}
//	fprintf(fp,"%d\n",loopnumber);
//	pos1=pos1-h;
////////////////////
////找左三分贝点//////////////////////
	d=1;
	ep=0.01*a0;
	loopnumber = 0;
	if (fabs(p2-pos2) > fabs(p2+1-pos2))
	{
		x0=p2;
		x1=p2+1;
		x2=p2+2;
		y0=*(data+p2);
		y1=*(data+p2+1);
        y2=*(data+p2+2);
	}
	if(fabs(p2-pos2) < fabs(p2+1-pos2))
	{
		x0=p2-1;
		x1=p2;
		x2=p2+1;
		y0=*(data+p2-1);
		y1=*(data+p2);
        y2=*(data+p2+1);
	}
    while(pos2>p2-1)
	{
		interpp=interLagrange(x0,x1,x2,y0,y1,y2,pos2);
		d=fabs(interpp-a0*kk);
		loopnumber = loopnumber+1;
		if (loopnumber <= 20000)
		{
		  if(d<ep)
		  {
		      ep=d;
			  pl=pos2;
		  }

		  pos2=pos2-h;
		}
		else
		{
			break;
		}
	}
  }
//  fprintf(fp,"%d\n",loopnumber);
//	pos2=pos2+h;
///////////////////////

/*	if(flag!=1)
	{
		if (pr-pl>0)
		{
			*(width)=pr-pl;
		}
		else if(pr-pl<0)
		{
			*(width)=dn+pr-pl;
		}
	}
	else
	{
		*(width)=0;
	}
    fprintf(fp,"%f\n",pos1);
    fprintf(fp,"%f\n",pos2);*/
    if(flag!=1)
	{	
		//左3分贝角
		ang1 = (int)(floor(pl));
		ang2 = (int)(ceil(pl));
		if(fabs(pl-ang1) > fabs(ang2-pl))
		{
			x0=ang1;
			x1=ang2;
			x2=ang2+1;
			y0=*(angle+ang1);
			y1=*(angle+ang2);
			y2=*(angle+ang2+1);
		}
		if(fabs(pl-ang1) <= fabs(ang2-pl))
		{
			x0=ang1-1;
			x1=ang1;
			x2=ang2;
			y0=*(angle+ang1-1);
			y1=*(angle+ang1);
			y2=*(angle+ang2);
		}
		left3db = interLagrange(x0,x1,x2,y0,y1,y2,pl);
		//右三分贝角
		ang1 = (int)(floor(pr));
		ang2 = (int)(ceil(pr));
		if(fabs(pr-ang1) > fabs(ang2-pr))
		{
			x0=ang1;
			x1=ang2;
			x2=ang2+1;
			y0=*(angle+ang1);
			y1=*(angle+ang2);
			y2=*(angle+ang2+1);
		}
		if(fabs(pr-ang1) <= fabs(ang2-pr))
		{
			x0=ang1-1;
			x1=ang1;
			x2=ang2;
			y0=*(angle+ang1-1);
			y1=*(angle+ang1);
			y2=*(angle+ang2);
		}
		right3db = interLagrange(x0,x1,x2,y0,y1,y2,pr);
		*(width)=fabs(right3db-left3db);
	}
	else
	{
		*(width)=0;
	}
//    fprintf(fp,"%f\n",left3db);
//    fprintf(fp,"%f\n",right3db);
//    fprintf(fp,"%f\n",pr);
//    fprintf(fp,"%f\n",pl);

//    fclose(fp);
	return;
}

//////////////////////////////////////////////////
//2008年5月22日将DI改为-4dB开角
int  diranaly (double *angle,double *data,int dn,double *mp,double *d3,double *d6,double *d10,double *spL1,double *spL2,double *spR1,double *spR2,double *slevelL1,double *slevelL2,double *slevelR1,double *slevelR2,double *DI)
{
	int n0,n1;
	int i=0,j=0;
//	FILE *fp,*fp1,*fp2;
	int datal=22;
	int zeroIndex=-1;
    double inte = 0;

	double *dirf,*theta;
	double x[wl],y[wl];
	double result[NN];
	double dt[3];
	double *ft,*smooth;//*interp,*interptheta;
	int *p,*v;
	int max1=-1,maxL1=-1,maxL2=-1,maxR1=-1,maxR2=-1;
	int pn=0;
	double sum=0.0,average=0.0,std=0.0;
	double da=0;
//	int interpl;
//	double h;
//	interpl=2*dn-1;
	n1 = wl-dn%datal;
	dirf = (double *)malloc(dn*sizeof(double));
	theta = (double *)malloc(dn*sizeof(double));
	ft = (double *)malloc(dn*sizeof(double));
    smooth = (double *)malloc((dn)*sizeof(double));
//    interp = (double *)malloc(interpl*sizeof(double));
//	interptheta = (double *)malloc(interpl*sizeof(double));
	p=(int *)malloc(dn*sizeof(int));
	v=(int *)malloc(dn*sizeof(int));
	//fp=fopen("rr2.d","w");
	for(i = 0;i<dn;i++)
	{
        *(dirf+i)=*(data+i);
		*(theta+i)=*(angle+i);
		*(p+i)=0;
		*(v+i)=0;
//		if(fabs(*(angle+i))<0.001)
//		{
//			zeroIndex = i;
//		}
	}
//	fp2=fopen("F:/result_dirdata.d","w");
//	for(i = 0;i<dn;i++)
//	{
//		fprintf(fp2,"%f\n",*(data+i));
//	}
//	fclose(fp2);
//	for(i=dn;i<dn+n1;i++)
//	{
//		*(dirf+i)=*(dirf+i-dn);
//        *(theta+i)=i*da;
//	}
	n0=dn/datal+1;
	for(i=0;i<n0-1;i++)
	{
     	memcpy(x,theta+i*datal,wl*sizeof(double));
	    memcpy(y,dirf+i*datal,wl*sizeof(double));
    	hpir1(x,y,wl,result,NN,dt,ft+i*datal,0);
	}
	memcpy(x,theta+(dn-wl),wl*sizeof(double));
    memcpy(y,dirf+(dn-wl),wl*sizeof(double));
    hpir1(x,y,wl,result,NN,dt,ft+(dn-wl),1);

    memcpy(smooth,ft,dn*sizeof(double));


/*	j=0;
	for(i=0;i<dn-1;i++)
	{
		h = *(theta+i+1) - *(theta+i);
		*(interptheta+j) = *(theta+i);
		*(interptheta+j+1) = *(theta+i)+h/2;
		*(interp+j) = *(smooth+i);
		*(interp+j+1) = 1/2.0*(*(smooth+i+1)+*(smooth+i));
		j=j+2;
	}
	*(interptheta+j)=*(theta+dn-1);
	*(interp+j)=*(smooth+dn-1);*/
//	fp=fopen("F:/result_smooth.d","w");
//	for(i = 0;i<dn;i++)
//	{
//		fprintf(fp,"%f\n",*(smooth+i));
//	}
//	fclose(fp);

	for(i=0;i<dn;i++)
	{
		sum=sum+*(smooth+i);
	}
	average=sum/dn;
	for(i=0;i<dn;i++)
	{
		std=std+fabs(average-*(smooth+i));
	}
	std=std/dn;

	
	findpeak(smooth,p,dn);
	findvalley(smooth,v,dn);
//	fp1 = fopen("F:/peak.d","w");
//	for(i=0;i<dn;i++)
//	{
//		fprintf(fp1,"%d\n",*(p+i));
//	}
//	fclose(fp1);
	for (i=0;i<dn;i++)
	{
		if(*(p+i)>0)
		{
           pn=pn+1;
		}
	}

	if(pn==0 || std<0.01*average)
	{
		//printf("无指向性");
		*(mp)=-1000;
		*(spL1)=-1000;
		*(spL2)=-1000;
		*(spR1)=-1000;
		*(spR2)=-1000;
		*(d3)=360;
		*(d10)=360;
		*(d6)=360;
		*(DI) = 360;
		*(slevelL1)=0;
        *(slevelL2)=0;
		*(slevelR1)=0;
        *(slevelR2)=0;
	}
	else
	{
		findMaxValue(smooth,theta,p,dn,&max1,&maxL1,&maxL2,&maxR1,&maxR2);
		BeamWidth(smooth,angle,max1,0.707,v,dn,d3);
		BeamWidth(smooth,angle,max1,0.5,v,dn,d6);
		BeamWidth(smooth,angle,max1,0.316,v,dn,d10);
		BeamWidth(smooth,angle,max1,0.63,v,dn,DI);

		if (max1 != -1)
		{
			*(mp)=*(angle+max1);
		}
		if (maxL1 != -1)
		{
			*(spL1)=*(angle+maxL1);
			*(slevelL1)=20*log(*(smooth+maxL1)/(*(smooth+max1)))/log(10);
		}
		else
		{
			*(spL1) = -1000;
			*(slevelL1) = -50;
		}
		if (maxL2 != -1)
		{
		    *(spL2)=*(angle+maxL2);
			*(slevelL2)=20*log(*(smooth+maxL2)/(*(smooth+max1)))/log(10);
		}
		else
		{
			*(spL2) = -1000;
			*(slevelL2) = -50;
		}

		if (maxR1 != -1)
		{
			*(spR1)=*(angle+maxR1);
			*(slevelR1)=20*log(*(smooth+maxR1)/(*(smooth+max1)))/log(10);
		}
		else
		{
			*(spR1) = -1000;
			*(slevelR1) = -50;
		}
		if (maxR2 != -1)
		{
		    *(spR2)=*(angle+maxR2);
			*(slevelR2)=20*log(*(smooth+maxR2)/(*(smooth+max1)))/log(10);
		}
		else
		{
			*(spR2) = -1000;
			*(slevelR2) = -50;
		}
	}

/*	if (zeroIndex != -1)
	{
		for(i=zeroIndex;i<dn;i++)
		{
			if (i==0)
			{
				da = fabs(*(angle+1)-*(angle));
			}
			else
			{
				da = fabs(*(angle+i)-*(angle+i-1));
			}
			inte = inte + (*(data+i))*(*(data+i))*sin(*(angle+i)/180*PI)*(da/180*PI);
		}
		inte = 2.0 / inte;
	}
	else
	{
		inte = 0;
	}
	*(DI) = inte;*/


/*	fp1=fopen("E:/HydroCalib2.0/debugfile/result_dir.d","w");
	fprintf(fp1,"%f\n",*(mp));
	fprintf(fp1,"%f\n",*(spL1));
	fprintf(fp1,"%f\n",*(spL2));
	fprintf(fp1,"%f\n",*(spR1));
	fprintf(fp1,"%f\n",*(spR2));

	fprintf(fp1,"%f\n",*(slevelL1));
	fprintf(fp1,"%f\n",*(slevelL2));
	fprintf(fp1,"%f\n",*(slevelR1));
	fprintf(fp1,"%f\n",*(slevelR2));
	fprintf(fp1,"%f\n",*(d3));
	fprintf(fp1,"%f\n",*(d6));
	fprintf(fp1,"%f\n",*(d10));
	fprintf(fp1,"%d\n",pn);
	fprintf(fp1,"%f\n",std);
	fprintf(fp1,"%f\n",average);*/

	free(dirf);
	free(theta);
	free(ft);
	free(smooth);
	free(p);
	free(v);
//	fclose(fp);
//	fclose(fp1);
    return 0;
}
///////////////////////////////////////////////////////////////////////
int  revanaly (double *freq,double *data,int n,double *maxFreq,double *maxRev,double *minFreq,double *minRev,double *bandwidth)
{
	double *newdata;
	double *newfreq;
	double L;
	int i,j;
	double h;
	double max=0,min=0;
	int pos1=0,pos2=0;
	int p1=0,p2=0;
	int found=0;
	L=2*n-1;
	newdata=(double *)malloc(L*sizeof(double));
	newfreq=(double *)malloc(L*sizeof(double));
	j=0;
	for(i=0;i<n-1;i++)
	{
		h = *(freq+i+1) - *(freq+i);
		*(newfreq+j) = *(freq+i);
		*(newfreq+j+1) = *(freq+i)+h/2;
		*(newdata+j) = *(data+i);
		*(newdata+j+1) = 1/2.0*(*(data+i+1)+*(data+i));
		j=j+2;
	}
	*(newfreq+j)=*(freq+n-1);
	*(newdata+j)=*(data+n-1);
   max=*(newdata);
   min=*(newdata);
   //找最大和最小值
   for(i=0;i<2*n-1;i++)
   {
		if (*(newdata+i)>max)
		{
			max=*(newdata+i);
			pos1=i;
		}
		if (*(newdata+i)<min)
		{
			min=*(newdata+i);
			pos2=i;
		}
   }
   *(maxRev)=max;
   *(maxFreq)=*(newfreq+pos1);
   *(minRev)=min;
   *(minFreq)=*(newfreq+pos2);
   //计算带宽
   //i=pos1;
   if(pos1==0)
   {
       i=pos1;
	   while((i<2*n-1) && (found==0))
	   {
		   if(*(newdata+i)<max-3)
		   {
			   *(bandwidth)=*(newfreq+i)-*(newfreq);
			   found=1;
		   }
		   i=i+1;
	   }
	   if(i==2*n-1)
	   {
			*(bandwidth)=*(newfreq+2*n-2)-*(newfreq);
		   *(bandwidth)=*(newfreq+2*n-2)-*(newfreq);
	   }
   }
   if(pos1==2*n-2)
   {
	   i=pos1;
	   while((i>=0) && (found==0))
	   {
		   if(*(newdata+i)<max-3)
		   {
			   *(bandwidth)=*(newfreq+2*n-2)-*(newfreq+i);
			   found=1;
		   }
		   i=i-1;
	   }
	   if(i==0)
	   {
			*(bandwidth)=*(newfreq+2*n-2)-*(newfreq);
	   }
   }
   if(pos1>0 && pos1<2*n-2)
   {
	  i=pos1;
	   //先向后找
	   while((i<2*n-1) && (found==0))
	   {
		   if(*(newdata+i)<max-3)
		   {
			   p1=i;
			   found=1;
		   }
		   i=i+1;
	   }
	   if(i==2*n-1)
	   {
			p1=2*n-2;
	   }
	   found=0;
	   //再向前找
	   i=pos1;
	   while((i>=0) && (found==0))
	   {
		   if(*(newdata+i)<max-3)
		   {
			   p2=i;
			   found=1;
		   }
		   i=i-1;
	   }
	   if(i==0)
	   {
			p2=0;
	   }
    *(bandwidth)=(p1-p2)*h/2;
   }
	return 0;
}
///////////////////////////////////////////////////////////////////////
int  impedanceanaly (double *freq,double *data,int n,double *maxFreq,double *maxRes)
{
	double *newdata;
	double *newfreq;
	double L;
	int i,j;
	double h;
	double max=0,min=0;
	int pos1=0,pos2=0;
	int p1=0,p2=0;
	int found=0;
	L=2*n-1;
	newdata=(double *)malloc(L*sizeof(double));
	newfreq=(double *)malloc(L*sizeof(double));
	j=0;
	for(i=0;i<n-1;i++)
	{
		h = *(freq+i+1) - *(freq+i);
		*(newfreq+j) = *(freq+i);
		*(newfreq+j+1) = *(freq+i)+h/2;
		*(newdata+j) = *(data+i);
		*(newdata+j+1) = 1/2.0*(*(data+i+1)+*(data+i));
		j=j+2;
	}
	*(newfreq+j)=*(freq+n-1);
	*(newdata+j)=*(data+n-1);
   max=*(newdata);
   min=*(newdata);
   //找最大和最小值
   for(i=0;i<2*n-1;i++)
   {
		if (*(newdata+i)>max)
		{
			max=*(newdata+i);
			pos1=i;
		}
		if (*(newdata+i)<min)
		{
			min=*(newdata+i);
			pos2=i;
		}
   }
   *(maxRes)=max;
   *(maxFreq)=*(newfreq+pos1);
 //  *(minRev)=min;
 //  *(minFreq)=*(newfreq+pos2);*/
 //  *(index) = pos1;
   return 0;
}

///////////////////////////////////////////////////////////////////////
int   filterenv(int fs,int *data,int *result,int dn)
{
	int i;
	int Nd;
	Nd=dn/10;
	int *datade;
	double h[121]={0,-0.00018574,-0.00035561,-0.00049399,-0.00058768,
                   -0.00062701,-0.00060674,-0.00052657,-0.00039138,-0.00021108,
                   0,0.0013782,0.0026494,0.0036957,0.0044157,0.0047323,0.0046006,
                   0.0040119,0.0029969,0.0016247,0,-0.0055804,-0.010789,-0.015142,
                   -0.018206,-0.019641,-0.019228,-0.01689,-0.012714,-0.0069488,
                   0,0.016379,0.03197,0.045322,0.055081,0.060107,0.059565,0.053014,
                   0.040472,0.022458,0,-0.040623,-0.080841,-0.11705,-0.1456,-0.16302,
                   -0.16623,-0.15274,-0.12088,-0.069872,0,0.10829,0.22977,0.35921,
                   0.49082,0.61845,0.73597,0.83758,0.91811,0.97329,1,0.97329,
                   0.91811,0.83758,0.73597,0.61845,0.49082,0.35921,0.22977,0.10829,
                   0,-0.069872,-0.12088,-0.15274,-0.16623,-0.16302,-0.1456,-0.11705,
                   -0.080841,-0.040623,0,0.022458,0.040472,0.053014,0.059565,0.060107,
                   0.055081,0.045322,0.03197,0.016379,0,-0.0069488,-0.012714,
                    -0.01689,-0.019228,-0.019641,-0.018206,-0.015142,-0.010789,
                   -0.0055804,0,0.0016247,0.0029969,0.0040119,0.0046006,0.0047323,
                   0.0044157,0.0036957,0.0026494,0.0013782,0,-0.00021108,-0.00039138,
                   -0.00052657,-0.00060674,-0.00062701,-0.00058768,-0.00049399,-0.00035561,
                    -0.00018574,0};
	int *resultde;
	int *tempdata;
//	FILE *fpr1;
//	fpr1=fopen("h.d","r");
	datade=(int *)malloc(Nd*sizeof(int));
	resultde=(int *)malloc(Nd*sizeof(int));
    tempdata=(int *)malloc(dn*sizeof(int));
	for(i=0;i<Nd;i++)
	{
		*(datade+i)=*(data+10*(i+1)-1);
	}

	firwin_e(90,1,100,100,fs,5,datade,resultde,Nd);
    for(i=0;i<dn;i++)
	{
		if( i%10==9)
		{
			*(tempdata+i)=*(resultde+i/10);
		}
		else
		{
			*(tempdata+i)=0;
		}
	}
	conv_f(h,tempdata,result,121,dn);
	free(datade);
	free(tempdata);
	free(resultde);
	return(0);
}
///////////////////////////////////////////////////////////////////////
int   interp(int *data,int *result,int dn)
{
	int i;
	int Nd;
	Nd=dn*5;
//	int *datade;
	double h[101];

//	int *resultde;
	int *tempdata;
	FILE *fpr1;
	fpr1=fopen("h5.bin","rb");
	fread(h,sizeof(double),101,fpr1);
//	datade=(int *)malloc(Nd*sizeof(int));
//	resultde=(int *)malloc(Nd*sizeof(int));
    tempdata=(int *)malloc(Nd*sizeof(int));

    for(i=0;i<Nd;i++)
	{
		if( i%5==0)
		{
			*(tempdata+i)=*(data+i/5);
		}
		else
		{
			*(tempdata+i)=0;
		}
	}
	conv_f(h,tempdata,result,101,Nd);
//	free(datade);
	free(tempdata);
//	free(resultde);
	return(0);
}

///////////////////////////////////////////////////////////////////////

int amestim (int *x, int dn,double *amp,double *snr)
{
	int l,N,k;
	double maxk=-1;
	double w;
	int pos=0;
    int *xx;
	double *spectrum;
	double sum_noise = 0;
	double sum_signal = 0;
	double sum_all = 0;
	complex<double> *xk;
	//FILE *fp;
	//fp = fopen("E:/HydroCalib2.0/debugfile/snr.txt","w");
	l = (int)(log(dn)/log(2))+1;
	N =(int) pow(2,l);
	xx = (int *)malloc(N*sizeof(int));
	spectrum=(double *)malloc(N*sizeof(double));
    xk = (complex <double> *) malloc(N*sizeof(complex<double>));
    memcpy(xx,x,dn*sizeof(int));
	for(k=0;k<dn;k++)
	{
//		w=0.54-0.46*cos(2*k*PI/(dn-1));
		w=1-1.93*cos(2*PI*k/dn)+1.29*cos(4*PI*k/dn)-0.388*cos(6*PI*k/dn)+0.0322*cos(8*PI*k/dn);
		xx[k]=xx[k]*w;
	}
	for(k=dn;k<N;k++)
	{
		xx[k]=0;
	}
	fft(xx,xk,l);
	for(k=0;k<N/2;k++)
	{
		spectrum[k] = sqrt(xk[k].real()*xk[k].real()+xk[k].imag()*xk[k].imag());
		if (spectrum[k]>maxk) 
		{
			maxk = spectrum[k];
			pos = k;
		}
	}
    *(amp)=maxk*2/dn;
	if(pos >= 18 )
	{
		for(k = pos-18;k<=pos+18;k++)
		{
			sum_signal = sum_signal + spectrum[k]*spectrum[k];
		}
	}
	else
	{
		for(k = 0;k<=pos+18;k++)
		{
			sum_signal = sum_signal + spectrum[k]*spectrum[k];
		}
	}

	
	for(k=0;k<N/2;k++)
	{
    	sum_all = sum_all + spectrum[k]*spectrum[k];
	}

	sum_noise = sum_all-sum_signal;

//	fprintf(fp,"%f,%f,%d",sum_noise,sum_signal,pos);
	if(sum_noise > 0)
	{
		if (sum_signal>0)
		{
			*(snr) = 10*log10(sum_signal / sum_noise);
		}
		else
		{
			*(snr) = 10*log10(maxk*maxk / sum_noise);
		}
	}
	else
	{
		*(snr) = 0;
	}

	free(xk);
	free(xx);
	free(spectrum);
//	fclose(fp);
	return 0;
}
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////
void findMaxValuevec(double *data,int *p,int dn,int *max1,int *max2)
{
	double max=0;
	int i;
    for (i=0;i<dn;i++)
	{
		if (*(p+i)==1)
		{
		   if (*(data+i)>max)
		   {
             max=*(data+i);
             *(max1)=i;
		   }
		}
	}
    max=0;
    for (i=0;i<dn;i++)
	{
		if ((*(p+i)==1) && (i!=*(max1)))
		{
		   if (*(data+i)>max)
		   {
             max=*(data+i);
             *(max2)=i;
		   }
		}
	}

}

//////////////////////////////////////////////////
void findMinValuevec(double *data,int *v,int dn,int *min1,int *min2)
{
	double min=10000;
	int i;
    for (i=0;i<dn;i++)
	{
		if (*(v+i)==1)
		{
		   if (*(data+i)<min)
		   {
             min=*(data+i);
             *(min1)=i;
		   }
		}
	}
    min=10000;
    for (i=0;i<dn;i++)
	{
		if ((*(v+i)==1) && (i!=*(min1)))
		{
		   if (*(data+i)<min)
		   {
             min=*(data+i);
             *(min2)=i;
		   }
		}
	}

}
//////////////////////////////////////////////
int  Vecanaly (double *angle,double *data,int dn,double *kd,double *delta ,double *kmax,double *thetamax,double *kmin,double *thetamin)
{
	int n0,n1;
	int i;
//	FILE *fp;
	int datal=52;
	double *dirf,*theta;
	double x[61],y[61];
	double result[NN];
	double dt[3];
	double *ft,*smooth;
	int *p,*v;
	int max1=-1,max2=-1,max3=-1;
	int min1=-1,min2=-1;
	double sum=0.0,average=0.0,std=0.0;
    double g45,g90,g90m,g0,g180;
    double nearest=10;
	int p45=0;

	n1 = 61-dn%datal;
	dirf = (double *)malloc(dn*sizeof(double));
	theta = (double *)malloc(dn*sizeof(double));
	ft = (double *)malloc(dn*sizeof(double));
    smooth = (double *)malloc((dn)*sizeof(double));
	p=(int *)malloc((dn)*sizeof(int));
	v=(int *)malloc((dn)*sizeof(int));
	//fp=fopen("rr2.d","w");
	for(i = 0;i<dn;i++)
	{
        *(dirf+i)=*(data+i);
		*(theta+i)=*(angle+i);
		*(p+i)=0;
		*(v+i)=0;
	}
//	for(i=dn;i<dn+n1;i++)
//	{
//		*(dirf+i)=*(dirf+i-dn);
//        *(theta+i)=i*da;
//	}
	n0=dn/datal+1;
	for(i=0;i<n0-1;i++)
	{
     	memcpy(x,theta+i*datal,61*sizeof(double));
	    memcpy(y,dirf+i*datal,61*sizeof(double));
    	hpir1(x,y,61,result,NN,dt,ft+i*datal,0);
	}
	memcpy(x,theta+(dn-61),61*sizeof(double));
    memcpy(y,dirf+(dn-61),61*sizeof(double));
    hpir1(x,y,61,result,NN,dt,ft+(dn-61),1);

    memcpy(smooth,ft,dn*sizeof(double));
//	fp=fopen("E:/HydroCalib2.0/debugfile/result_vector.d","w");
//	for(i = 0;i<dn;i++)
//	{
//		fprintf(fp,"%f\n",*(smooth+i));
//	}
//	fclose(fp);

    findpeak(smooth,p,dn);
	findvalley(smooth,v,dn);
	findMaxValuevec(smooth,p,dn,&max1,&max2);
	findMinValuevec(smooth,v,dn,&min1,&min2);

	g0=*(data+max1);
	g180=*(data+max2);
	g90=*(data+min1);
	g90m=*(data+min2);
    *(kd) = 20*log10(g0/g90);
/*	if ((max1-(45/da))>=0)
	{
	   g45 = *(data+int(max1-(45/da)));
	}
	else
	{
	   g45 = *(data+int(max1+(45/da)));
	}*/
    for(i=0;i<dn;i++)
	{
	   if(fabs(*(angle+max1)-*(angle+i)-45) < nearest)
	   {
		   p45 = i;
		   nearest = fabs(*(angle+max1)-*(angle+i)-45);
	   }
	}
	if (nearest>0.5)
	{
	   for(i=0;i<dn;i++)
	   {
	      if(fabs(*(angle+max1)-*(angle+i)+45) < nearest)
		  {
		     p45 = i;
		     nearest = fabs(*(angle+max1)-*(angle+i)+45);
		  }
	   }
	}

	g45 = *(data+p45);
	*(delta) = 20*log10(1.414*g45/g0);
	*(kmax)= 20*log10(g0/g180);
	*(thetamax)=fabs(*(angle+max1)-*(angle+max2));
	*(kmin) = 20*log10(g90/g90m);
	*(thetamin) = fabs(*(angle+min1)-*(angle+min2));   

	free(dirf);
	free(theta);
	free(ft);
	free(smooth);
	free(p);
	free(v);
//	fclose(fp);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////
int   filternoise(int fs,int fl,int fh,float *data,float *result,int dn)
{
	int *datain;
	int *dataout;
	datain=(int *)malloc(dn*sizeof(int));
	dataout=(int *)malloc(dn*sizeof(int));
	int i;
	for(i=0;i<dn;i++)
	{
		*(datain+i)=int(*(data+i)*32767);
	}
//firwin_e(int n,int band,int fl,int fh,int fs,int wn, int *data,int *result,int dn)
    firwin_e(90,3,fl,fh,fs,5,datain,dataout,dn);
	for(i=0;i<dn;i++)
	{
		*(result+i)=*(dataout+i)/32767.0;
	}
	free(datain);
	free(dataout);
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
int  interdir(double *data,double *angle,int dn,double theta,double *dir)
{
	int i,pp;
	double ff;
    double x0,x1,x2,y0,y1,y2;
	double max=0;
//	FILE *fp;
//	fp=fopen("E:/HydroCalib2.0/debugfile/test.dat","w");
	for(i=0;i<dn-1;i++)
	{
	  if (*(data+i)>max)
	  {
		  max=*(data+i);
	  }
	}
	if (theta< (*(angle)) || theta>(*(angle+dn-1)))
	{
		ff=-50.0;
		*(dir)=ff;
//		fprintf(fp,"%f\n",*(data));
//		fprintf(fp,"%f\n",theta);

//		fclose(fp);
		return 0;
	}
	for(i=0;i<dn-1;i++)
	{

		if (*(angle+i)==theta)
		{
			ff=*(data+i);
			*(dir)=20*log10(ff/max);
//			fprintf(fp,"%f\n",*(data));
//		    fprintf(fp,"%f\n",theta);

//	    	fclose(fp);

			return 0;
		}
		if(theta>*(angle+i) && theta<*(angle+i+1))
		{
			pp=i;
			break;
		}
	}
	if(fabs(theta-*(angle+pp)) > fabs(theta-*(angle+pp+1)))
	{
		if (pp+2<dn)
		{
		  x0=*(angle+pp);
		  x1=*(angle+pp+1);
		  y0=*(data+pp); 
		  y1=*(data+pp+1);
		  x2=*(angle+pp+2);
		  y2=*(data+pp+2);
		}
		else
		{
			x0=*(angle+pp-1);
		    x1=*(angle+pp);
    		y0=*(data+pp-1);
 		    y1=*(data+pp);
	        x2=*(angle+pp+1);
		    y2=*(data+pp+1); 
		}
	}
	if(fabs(theta-*(angle+pp))<fabs(theta-*(angle+pp+1)))
	{
		if (pp-1>=0)
		{
			x0=*(angle+pp-1);
		    x1=*(angle+pp);
    		y0=*(data+pp-1);
 		    y1=*(data+pp);
	        x2=*(angle+pp+1);
		    y2=*(data+pp+1); 
		}
		else
		{
		  x0=*(angle+pp);
		  x1=*(angle+pp+1);
		  y0=*(data+pp); 
		  y1=*(data+pp+1);
		  x2=*(angle+pp+2);
		  y2=*(data+pp+2);
		}

	}
	ff=interLagrange(x0,x1,x2,y0,y1,y2,theta);
	*(dir)=20*log10(ff/max);
//	fprintf(fp,"%f\n",*(data));
//	fprintf(fp,"%f\n",theta);
//	fclose(fp);

	return 0;
}

//////////////////////////////////////////////////
/*int __stdcall diranalyopt (double *angle,double *data,int dn,double da,int mp,double *d3)
{
	int n0,n1;
	int i=0,j=0;
	FILE *fp,*fp1,*fp2;
	int datal=22;
	
	double *dirf,*theta;
	double x[wl],y[wl];
	double result[NN];
	double dt[3];
	double *ft,*smooth;//*interp,*interptheta;
	int *p,*v;
	int max1=-1,max2=-1,max3=-1;
	double sum=0.0,average=0.0,std=0.0;
//	int interpl;
//	double h;
//	interpl=2*dn-1;
	n1 = wl-dn%datal;
	dirf = (double *)malloc(dn*sizeof(double));
	theta = (double *)malloc(dn*sizeof(double));
	ft = (double *)malloc(dn*sizeof(double));
    smooth = (double *)malloc((dn)*sizeof(double));
//    interp = (double *)malloc(interpl*sizeof(double));
//	interptheta = (double *)malloc(interpl*sizeof(double));
	p=(int *)malloc(dn*sizeof(int));
	v=(int *)malloc(dn*sizeof(int));
	//fp=fopen("rr2.d","w");
	for(i = 0;i<dn;i++)
	{
        *(dirf+i)=*(data+i);
		*(theta+i)=*(angle+i);
		*(p+i)=0;
		*(v+i)=0;
	}
	fp2=fopen("E:/HydroCalib2.0/debugfile/result4.d","w");
	for(i = 0;i<dn;i++)
	{
		fprintf(fp2,"%f\n",*(data+i));
	}
	fclose(fp2);
//	for(i=dn;i<dn+n1;i++)
//	{
//		*(dirf+i)=*(dirf+i-dn);
//        *(theta+i)=i*da;
//	}
	n0=dn/datal+1;
	for(i=0;i<n0-1;i++)
	{
     	memcpy(x,theta+i*datal,wl*sizeof(double));
	    memcpy(y,dirf+i*datal,wl*sizeof(double));
    	hpir1(x,y,wl,result,NN,dt,ft+i*datal,0);
	}
	memcpy(x,theta+(dn-wl),wl*sizeof(double));
    memcpy(y,dirf+(dn-wl),wl*sizeof(double));
    hpir1(x,y,wl,result,NN,dt,ft+(dn-wl),1);

    memcpy(smooth,ft,dn*sizeof(double));

	fp=fopen("E:/HydroCalib2.0/debugfile/result.d","w");
	for(i = 0;i<dn;i++)
	{
		fprintf(fp,"%f\n",*(smooth+i));
	}
	fclose(fp);



	    findpeak(smooth,p,dn);
		findvalley(smooth,v,dn);
//		findMaxValue(smooth,p,dn,&max1,&max2,&max3);
		BeamWidth(smooth,mp,0.707,v,dn,d3);

	fp1=fopen("result1.d","w");
	fprintf(fp1,"%d\n",*(mp));
	fprintf(fp1,"%d\n",*(sp1));
	fprintf(fp1,"%d\n",*(sp2));
	fprintf(fp1,"%f\n",*(slevel1));
	fprintf(fp1,"%f\n",*(slevel2));
	fprintf(fp1,"%f\n",*(d3));
	fprintf(fp1,"%f\n",*(d6));
	fprintf(fp1,"%f\n",*(d10));

	free(dirf);
	free(theta);
	free(ft);
	free(smooth);
	free(p);
	free(v);
//	fclose(fp);
	fclose(fp1);
    return 0;
}*/
///////////////////////////////////////////////////////////////////////

//////////////////////////////////
int noisespec(int *data, double *datak,int dn,int average,double v)
{
  complex <double> *dk;
  double *w;
  double *dtemp,*dtempk;
  int retV;
  int i,k,l,j;
  l = (int)(log(dn)/log(2));
//  N =(int) pow(2,l);
  double max=0;
  dk=(complex <double> *)malloc(dn*sizeof(complex <double>));
  w=(double *)malloc(dn*sizeof(double));
  dtemp=(double *)malloc(dn*sizeof(double));
  dtempk=(double *)malloc(dn*sizeof(double));
 
 // for(k=0;k<=n-1;k++)
 // {
//	w[k] = 1-1.93*cos(2*PI*k/n)+1.29*cos(4*PI*k/n)-0.388*cos(6*PI*k/n)+0.0322*cos(8*PI*k/n);
//  }
  for(k=0;k<dn;k++)
  {
      dtemp[k]=0;
	  dtempk[k]=0;
  }
  for (j=0;j<average;j++)
  {
	 for(k=0;k<=dn-1;k++)
	 {
	     dtemp[k]=data[k+j*dn]*window(5,dn,k,0.1);
	 }

     retV=fft_f(dtemp,dk,l);
     for(i=0;i<=dn-1;i++)
	 {
	     *(dtempk+i) = sqrt(dk[i].real()*dk[i].real()+dk[i].imag()*dk[i].imag())*2/dn*v+*(dtempk+i) ;
	 }
  }
  for(k=0;k<=dn-1;k++)
  {
      *(datak+k)=*(dtempk+k)/average;
  }

  return 0;
}
//////////////////////////////////
int  amp (int *x,double *amplitude,int dn,double ff,int samplerate)
{
	int i=0,j=0;
	int N = 0;
	int L = 15000;
	double realpart=0;
	double imagpart=0;
	double sum = 0;
	int signal[15000];
//	FILE *fp;
//	fp = fopen("d:\\record.dat","w");
	if(dn<=L)
	{
		for (i=0;i<dn;i++)
		{
			realpart = realpart + x[i]*cos(2*PI*i*ff/(samplerate*1.0));
			imagpart = imagpart - x[i]*sin(2*PI*i*ff/(samplerate*1.0));
		}
		*(amplitude) = sqrt(realpart*realpart+imagpart*imagpart)*2/dn;
	}
	else
	{
		N = dn/L;
		for(j=0;j<N;j++)
		{
			memcpy(signal,x+j*L,L*sizeof(int));
			realpart=0;
			imagpart=0;
			for (i=0;i<L;i++)
			{
				realpart = realpart + signal[i]*cos(2*PI*i*ff/(samplerate*1.0));
				imagpart = imagpart - signal[i]*sin(2*PI*i*ff/(samplerate*1.0));
			}
			sum = sum+sqrt(realpart*realpart+imagpart*imagpart)*2/L;
//			fprintf(fp,"%f\n",sum);
		}
		*(amplitude) = sum / (N*1.0);
	}
//	fprintf(fp,"%d\n",N);
//	fclose(fp);
	return 0;
}

int  arg (int *x,double *angle,int dn,double ff,int samplerate)
{
//	int l,N,k;
//	double maxk=-1;
//	double w;
//	int pos;
//	double angle ;
//  int *xx;
	int i=0;
//	complex<double> xk;
//	l = (int)(log(dn)/log(2))+1;
//	N =(int) pow(2,l);
//	xx = (int *)malloc(N*sizeof(int));
//	yy = (int *)malloc(N*sizeof(int));
//    xk = complex <double>(0,0) ;
	double realpart=0;
	double imagpart=0;
//   yk = (complex <double> *) malloc(N*sizeof(complex<double>));
//   yhk = (complex <double> *) malloc(N*sizeof(complex<double>));
//    memcpy(xx,x,dn*sizeof(int));
	for (i=0;i<dn;i++)
	{
		realpart = realpart + x[i]*cos(2*PI*i*ff/(samplerate*1.0));
		imagpart = imagpart - x[i]*sin(2*PI*i*ff/(samplerate*1.0));
	}
	if(imagpart>0 && realpart > 0)
	{
       *(angle) = atan(imagpart / realpart)/PI*180;
	}
	if(imagpart<0 && realpart > 0)
	{
		*(angle) = atan(imagpart / realpart)/PI*180;
	}
	if(imagpart>0 && realpart < 0)
	{
       *(angle) = (atan(imagpart / realpart)+PI)/PI*180;
	}
	if(imagpart<0 && realpart < 0)
	{
		*(angle) = (atan(imagpart / realpart)-PI)/PI*180;
	}
	return 0;
}