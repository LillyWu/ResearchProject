#ifndef SignalProcess_H
#define SignalProcess_H

#define PI  3.141592653589793
#define NN 19
#include "stdlib.h"
#include "stdio.h"
#include <math.h>

#include <complex>

using namespace std;

int   hilbert(int *data , int *filterdata, int dn);
int   firwin_e(int n,int band,int fl,int fh,int fs,int wn,int *data,int *result,int dn);
int   conv(int *h,int *data,int *result,int hn,int dn);
int  filterdsg(int n,int band,int fl,int fh,int fs,double *hf);
int freqspec(double *data, double *datak);
int  spec(int *data, double *datak,int l,int dn,int wn,double v,double ref);
int  psd (double *x, double *psd);
int cpsd (int *x,double *psd,double *angle,int dn);
int  diranaly (double *angle,double *data,int dn,double *mp,double *d3,double *d6,double *d10,double *spL1,double *spL2,double *spR1,double *spR2,double *slevelL1,double *slevelL2,double *slevelR1,double *slevelR2,double *DI);
int  revanaly (double *freq,double *data,int n,double *maxfreq,double *maxRev,double *minfreq,double *minRev,double *bandwidth);
int  filterenv(int fs,int *data,int *result,int dn);
int  amestim (int *x, int dn,double *amp,double *snr);
int  Vecanaly (double *angle,double *data,int dn,double *kd,double *delta,double *kmax,double *thetamax,double *kmin,double *thetamin);
int   interp(int *data,int *result,int dn);
int   filternoise(int fs,int fl,int fh,float *data,float *result,int dn);
int  interdir(double *data,double *angle,int dn,double theta);
int  diranalyopt (double *angle,double *data,int dn,double da,int mp,double *d3);
int  noisespec(int *data, double *datak,int dn,int average,double v);
int  arg (int *x,double *angle,int dn,double ff,int samplerate);
int  amp (int *x,double *amplitude,int dn,double ff,int samplerate);
int  impedanceanaly (double *freq,double *data,int n,double *maxFreq,double *maxRes);

int fft(int *data,complex <double> *a,int L);
int ifft(complex <double> *a,int *data,int L);
double window(int type,int n,int i,double beta);
double kaiser(int i,int n,double beta);
double bessel0(double x);
int conv_f(double *h,int *data,int *result,int hn,int dn);
int fft_f(double *data,complex <double> *a,int L);
int fft_my(double *data,double *result,int L);
void hpir1(double x[],double y[],int n,double a[],int m,double dt[],double fit[],int flag);
void findpeak(double *data, double *p, int dn);
void findvalley(double *data, double *p,int dn);
void findMaxValue(double *data,double *angle,int *p,int dn,int *max1,int *maxL1,int *maxL2,int *maxR1,int *maxR2);
void BeamWidth(double *data,double *angle,int pos,double kk,int *v,int dn,int *width);

//----------------------------------------------------------
double  corr(double *h,double *data,int *result,int hn,int dn);   //
double coordinate(double *data,double *result,int dn);
double coordinate_2(double *data,double *result,int dn);
int maxvalue(int *data,int dn);
int maxvalue_max(int *data,int dn);  
int inv(double *a,int n);
void multiply(double *a,double *b,int m,int n,int k,double *c);//

/*  a-双精度实型指针，体积为m*n 存放矩阵a的元素
    b-双精度实型指针，体积为n*k 存放矩阵b的元素
	c-双精度实型指针，体积为m*k 存放矩阵c=a*b的元素*/

//----------------------------------------------------------

#endif