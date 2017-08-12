/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210B-EVAL Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  DSP_ASM
#include "stm32f10x.h"
#include "dsp_asm.h"
#include "stm32_dsp.h"
#include "table_fft.h"
#include <stdio.h>
#include <math.h>


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/
#define PI2 6.28318530717959
//#define NPT_64 64
#define NPT_256 256
//#define NPT_1024 1024

// N=64,Fs/N=50Hz,Max(Valid)=1600Hz
//#ifdef NPT_64
//#define NPT 64
//#define Fs  3200
//#endif

// N=256,Fs/N=25Hz,Max(Valid)=3200Hz
#ifdef NPT_256
#define NPT 256
#define Fs  6400
#endif

// N=1024,Fs/N=5Hz,Max(Valid)=2560Hz
//#ifdef NPT_1024
//#define NPT 1024
//#define Fs  5120
//#endif


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
extern  uint16_t  TableFFT[];
long lBUFIN[NPT];         /* Complex input vector */
long lBUFOUT[NPT];        /* Complex output vector */
float lBUFMAG[NPT];/* Magnitude vector */
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void dsp_asm_powerMag(void);

/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         Global Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

void  dsp_asm_init()
{
  u16 i=0;
  float fx;
  for(i=0;i<NPT;i++)
  {
    fx  = 32000 * sin(PI2*i*50.0f/Fs) + 16000 * sin(PI2*i*550.0f/Fs) + 9000 * sin(PI2*i*1150.0f/Fs);
    lBUFIN[i] = ((s16)fx)<<16;
  }
}

void  dsp_asm_test()
{
//  u16 i=0;
  // FFT
//#ifdef NPT_64
  //cr4_fft_64_stm32(lBUFOUT, lBUFIN, NPT);
//#endif

#ifdef NPT_256
  cr4_fft_256_stm32(lBUFOUT, lBUFIN, NPT);
#endif

//#ifdef NPT_1024
 // cr4_fft_1024_stm32(lBUFOUT, lBUFIN, NPT);
//#endif

  // 计算幅值
  dsp_asm_powerMag();
  
//  printf("No. Freq      Power\n");
//  for(i=0;i<NPT/2;i++)
//  {
//    printf("%4d,%4d,%10d,%10d,%10d\n",i,(u16)((float)i*Fs/NPT),lBUFMAG[i],(lBUFOUT[i]>>16),(lBUFOUT[i]&0xffff));
//  }
//  printf("*********END**********\r\n");
}

void dsp_asm_powerMag(void)//计算各次谐波幅值
{
	
	u8 temp[50]={0};//临时字符串存储
  s16 lX,lY;
  u32 i;
	
	Uart5_Send_String("----DSP完成FFT----\r\n\0");
  for(i=0;i<NPT/2;i++)
  {
    lX  = (lBUFOUT[i] << 16) >> 16;
    lY  = (lBUFOUT[i] >> 16);
	{
		float X	= NPT * ((float)lX) /32768;
		float Y	= NPT * ((float)lY) /32768;
    float Mag = sqrt(lX*lX + lY*lY)/NPT;
		
		if(i == 0){
			lBUFMAG[i] = (Mag * 0.5);
		}else{
			lBUFMAG[i] = (Mag * 1.0);
		}
		
		sprintf(temp,"%d次谐波分量:%.2f\r\n\0",i,lBUFMAG[i]);
		Uart5_Send_String(temp);
		delay_ms(100);
    }
  }
}

