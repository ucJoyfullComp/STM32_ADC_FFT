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
* Filename      : usart1.h
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#ifndef  __USART1_H__
#define  __USART1_H__

/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <stdio.h>



/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void	USART1_Init(u32 baudrate,u16 parity);
extern void UART1_SendByte(u16 dat);
extern void UART1Write(u8* data,u16 len);
int fputc(int ch, FILE *f);
void Uart5_Send_String(u8 *p);
void UART5_Config(u32 baud);
void UART5_IRQHandler(void);

#endif                                                          /* End of module include.                               */
