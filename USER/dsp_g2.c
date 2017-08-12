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

#include "stm32f10x.h"
#include "dsp_g2.h"
#include "fft2.h"
#include <math.h>
#include <stdio.h>



/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/
#define NPT 64
#define PI2 6.28318530717959

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
double result[NPT];
struct compx s[NPT];
float OMAG[NPT];
extern float ADC_DATA[64];

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

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

/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

void dsp_g2_test()
{
	u8 temp[50]={0};//临时字符串存储
	u32 X=0;
	u32 Y=0;
	float Mag = 0;
  u16 i=0;
  u32 Fs=3200;
	Uart5_Send_String("----Begin print FFT----\r\n\0");
  for(i=0;i<NPT;i++)//产生测试序列
  {
    s[i].real = ADC_DATA[i];
    s[i].imag=0;
  }
  FFT(s,NPT);
  for(i=0;i<NPT/2;i++)
  {// X 的值为s[i].real 表示FFT 计算结果实部，Y同理
    result[i] = sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag)*2/NPT;
		Mag = sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag) / NPT;
		X = NPT * ((float)s[i].real) / 32768.0;
		Y = NPT * ((float)s[i].imag) / 32768.0;
		//Mag = sqrt(X * X + Y * Y) / NPT;
		//Uart5_Send_String("X/0");
		//Uart5_Send_String(X);
		//delay_ms(500);
		//Uart5_Send_String("Y/0");
		//Uart5_Send_String(Y);
		//delay_ms(500);
		//Uart5_Send_String("Mag/0");
		if(i == 0){
			OMAG[i] = Mag * 0.5;
		}else{
			OMAG[i] = Mag;
		}
		
		//delay_ms(100);
		sprintf(temp,"%d Times Mag:%.2f\r\n\0",i,OMAG[i]);
		Uart5_Send_String(temp);
		delay_ms(100);
    //if(result[i] > 10)
		//Uart5_Send_String("%4d,%4d,%ld\n",i,(u16)((double)i*Fs/NPT),(u32)result[i]);
		//delay_ms(500);
		//Uart5_Send_String(Mag);
  }
  
}
