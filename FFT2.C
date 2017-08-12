#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct compx
{ double real;
  double imag;
} compx ;

#define PI2 6.28318530717959
//#define NPT_64 64
//#define NPT_256 256
#define NPT_1024 1024

// N=64,Fs/N=50Hz,Max(Valid)=1600Hz
#ifdef NPT_64
#define NPT NPT_64
#define Fs  3200
#endif

// N=256,Fs/N=25Hz,Max(Valid)=3200Hz
#ifdef NPT_256
#define NPT NPT_256
#define Fs  6400
#endif

// N=1024,Fs/N=5Hz,Max(Valid)=2560Hz
#ifdef NPT_1024
#define NPT NPT_1024
#define Fs  5120
#endif

float  result[NPT];
struct  compx s[NPT];

const float pi=3.14159;


struct compx EE(struct compx b1,struct compx b2)
{
struct compx b3;
b3.real=b1.real*b2.real-b1.imag*b2.imag;
b3.imag=b1.real*b2.imag+b1.imag*b2.real;
return(b3);
}

void FFT(struct compx *xin,int N)
{
int f,m,LH,nm,i,k,j,L;
double p , ps ;
int le,B,ip;
float pi;
struct compx v,w,t;
LH=N/2; 
f=N;
for(m=1;(f=f/2)!=1;m++){;}  /*2^m=N*/

{
for(L=m;L>=1;L--)    /*这里和时域的也有差别*/
{ 
le=pow(2,L);
B=le/2; /*每一级碟形运算间隔的点数*/
pi=3.14159;
 for(j=0;j<=B-1;j++)
  {
   p=pow(2,m-L)*j;
   ps=2*pi/N*p;
   w.real=cos(ps);
   w.imag=-sin(ps);
   for(i=j;i<=N-1;i=i+le)
     {
      ip=i+B;  
      t=xin[i];
      xin[i].real=xin[i].real+xin[ip].real;
      xin[i].imag=xin[i].imag+xin[ip].imag;  
      xin[ip].real=xin[ip].real-t.real;
      xin[ip].imag=xin[ip].imag-t.imag;     
      xin[ip]=EE(xin[ip],w);
     }
  }
}
}
/*变址运算*/

nm=N-2;   
j=N/2;
for(i=1;i<=nm;i++)
{
if(i<j){t=xin[j];xin[j]=xin[i];xin[i]=t;}
k=LH;
while(j>=k){j=j-k;k=k/2;}
j=j+k;
}

}


/*****************main programe********************/



main()
{

int i;

long Freq1=50;	// 频率1
long Freq2=2500;	// 频率2
long lBUF=0;
for(i=0;i<NPT;i++)
{
//s[i].real=1+sin(PI2 * i * 10000.0f / 48000.0f);

s[i].real	= 1000 * sin(PI2 * i * ((double)Freq1/Fs));
s[i].real  += 2000 * sin(PI2 * i * ((double)Freq2/Fs));
s[i].real  += 3000 * sin(PI2 * i * ((double)2550/Fs));
s[i].imag=0;

//lBUF	= ((long)(s[i].real))<<16;
//printf("%08lX,",lBUF);
//if((i+1)%8==0) printf("\n");
}
//printf("********************\n");
//for(i=NPT/2;i<NPT;i++)
//	{
//	s[i].real=0;s[i].imag=0;
//	}

FFT(s,NPT);
//for(i=0;i<NPT;i++)
//	{
//		lBUF	= ((long)(s[i].real))<<16 |((long)s[i].imag);
//		printf("%11.4f,%11.4f,%08lX,%08lX,%08lX\n",s[i].real,s[i].imag,(long)(s[i].real),(long)(s[i].imag),lBUF);
//		if((i+1)%8==0) printf("\n");
//	}

//printf("********Begin********\r\n");
printf("No. Freq      Power           Real            Image  \n");
for(i=0;i<NPT/2;i++)
{
	result[i]=sqrt(pow(s[i].real,2)+pow(s[i].imag,2)) * 2 /NPT;
//printf("%.4f",s[i].real);
//printf("+%.4fj\n",s[i].imag);

	printf("%4d,%4d,%10.4f,%15.4f,%15.4f\n",i,(int)((float)i*Fs/NPT),result[i],s[i].real,s[i].imag);

//printf("%3d,\t%3d,\t%.4f,\t%.4f,\t%.4f\r\n",i,i*Fs/NPT,s[i].real,s[i].imag,sqrt(s[i].real*s[i].real + s[i].imag * s[i].imag));
//printf("%3d,\t%3d,\t%.4f,\t+%.4fj\n",i,i*Fs/NPT,s[i].real,s[i].imag);
//result[i]=sqrt(pow(s[i].real,2)+pow(s[i].imag,2));
}
//printf("********End********\r\n");

return;
}