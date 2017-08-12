#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "sys.h" 

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void USART3_SendByByter(u8 Data);
void USART3_Configuration(void);

#endif
