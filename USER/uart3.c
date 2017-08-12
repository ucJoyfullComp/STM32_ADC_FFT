#include "sys.h"
#include "usart.h"
#include"crc.h"
#include "led.h"
#include "delay.h"
#include "uart3.h"

void NVIC_Configuration()
{
 	NVIC_InitTypeDef NVIC_InitStructure;
	//???? 2-level interrupt 
	//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//??3???? 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);   

}
void GPIO_Configuration(void) //?????
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin = 	  GPIO_Pin_7| GPIO_Pin_12;	//Power Control				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/******************************************************************/
	/******************************************************************/
	GPIO_InitStructure.GPIO_Pin =     GPIO_Pin_10 ;	/* USART3 Tx (PB.10)*/							
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/******************************************************************/
	GPIO_InitStructure.GPIO_Pin =     GPIO_Pin_11 ;	/* USART3 Rx (PB.11)*/								
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/******************************************************************/
	/******************************************************************/	
}
void RCC_Configuration(void)
{
	//--------------------------- CLK INIT, HSE PLL ----------------------------
	ErrorStatus HSEStartUpStatus;
	//RCC reset
	RCC_DeInit();
	//?????? ??????
	RCC_HSEConfig(RCC_HSE_ON); 
	//?????????
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	//???? ?????
	while(HSEStartUpStatus == ERROR);
	//????????
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	//?????8M ?????72M
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);

	//----------------------------- CLOSE HSI ---------------------------
	//??????HSI
	RCC_HSICmd(DISABLE);


	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_USART3 , ENABLE); 								

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB , ENABLE);
}

void USART3_SendByByter(u8 Data)
{
	//????
	USART_GetFlagStatus(USART3, USART_FLAG_TC);	
	USART_SendData(USART3, Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
}


void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 4800;//°?ιθ???a9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//Χ?3€?a8??κy?Y??κ?
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//ς???ν??1??
    USART_InitStructure.USART_Parity = USART_Parity_No;//?T????D??ι??
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//?Yαχ????
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//κ?·???κ?   
    
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    /* Enable USART2 */
    USART_Cmd(USART3, ENABLE);
}



void USART3_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{ 
		c = USART3->DR;
	} 	
}
