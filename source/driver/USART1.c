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
* Filename      : usart1.c
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
#include "USART1.H"



/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


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



void	USART1_Init(u32 baudrate,u16 parity)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef	USART_ClockInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  USART_DeInit(USART1);
  
  #ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
  #else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
  #endif
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 9600 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the second edge 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate = baudrate;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  
  if( (parity != USART_Parity_Even) && (parity != USART_Parity_Odd) )
  {
    parity = USART_Parity_No;
  }
  USART_InitStructure.USART_Parity = parity ;
  
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_ClockInitStructure.USART_Clock	= USART_Clock_Disable;
  USART_ClockInitStructure.USART_CPOL		= USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA		= USART_CPHA_2Edge;
  USART_ClockInitStructure.USART_LastBit	= USART_LastBit_Disable;

  
  /* Configure the USART1 */
  USART_Init(USART1, &USART_InitStructure);
  USART_ClockInit(USART1,&USART_ClockInitStructure);

  /* Enable the USART Transmoit interrupt: this interrupt is generated when the 
   USART1 transmit data register is empty */  
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);

  /* Enable the USART Receive interrupt: this interrupt is generated when the 
   USART1 receive data register is not empty */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
  u8 c;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  { 	
    c=USART1->DR;
    UART1_SendByte(c); 	    
  } 
}

void UART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
   
}
void UART1Write(u8* data,u16 len)
{
	u16 i;
	for (i=0; i<len; i++){
		UART1_SendByte(data[i]);
	}
}

/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  /* Write a character to the USART */
  USART_SendData(USART1, (u8) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
  {
  }
  return ch;
}


/****************************************************************
	Ãû³Æ£NVIC_Configuration_Usart5º()
	²ÎÊý£ºÎÞ
	¹¦ÄÜ£º´®¿Ú5µÄÖÐ¶ÏÅäÖÃ
	·µ»Ø£ºÎÞ
****************************************************************/
void NVIC_Configuration_Usart5(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************
	Ãû³Æ£ºUART5_Config()
	²ÎÊý£º²¨ÌØÂÊ
	¹¦ÄÜ£ºÅäÖÃ´®¿Ú5
	·µ»Ø£ºÎÞ
****************************************************************/
void UART5_Config(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

    //ÅäÖÃPD2-->RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //ÅäÖÃPC12-->TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //²¨ÌØÂÊµÈÅäÖÃ
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART5, &USART_InitStructure);

    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);

    USART_Cmd(UART5, ENABLE);
    NVIC_Configuration_Usart5();
}
void UART5_IRQHandler(void)
{
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        USART_SendData(UART5, USART_ReceiveData(UART5));
		while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);//µÈ´ýÉÏÒ»¸ö×Ö½Ú·¢ËÍÍê±Ï
    }

}

/****************************************************************
	Ãû³Æ£ºUart5_Send_String()
	²ÎÊý£º×Ö·û´®Ö¸Õë
	¹¦ÄÜ£º´®¿Ú5·¢ËÍÒ»¸öÊý×é£¬×Ö·û´®ÒÔ\0½áÎ²
	·µ»Ø£ºÎÞ
****************************************************************/
void Uart5_Send_String(u8 *p)
{
	u8 CMD_TFT_END[3]={0xff,0xff,0xff};
	int i=0;
	while(*p!='\0')
	{
		USART_SendData(UART5,*p);
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET);
		p++;
	}
	for(i=0;i<3;i++)
	{
		USART_SendData(UART5,CMD_TFT_END[i]);
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET);
	}
	
}


