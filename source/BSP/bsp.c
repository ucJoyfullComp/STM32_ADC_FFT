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

#define  BSP_GLOBALS
#include "stm32f10x.h"
#include "bsp.h"
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
static volatile ErrorStatus HSEStartUpStatus = SUCCESS;
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void GPIO_Config(void);
void USART1_Config(void);

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

void  BSP_Init(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
    {
     /* Enable Prefetch Buffer */
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

     /*************************************************************************/
     /*   HSE=25MHz,HCLK=72MHz,PCLK2=72MHz,PCK1=36MHz                         */
     /*************************************************************************/

     /* Flash 2 wait state */
     FLASH_SetLatency(FLASH_Latency_2);
    
     /* HCLK = SYSCLK */
     RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
     /* PCLK2 = HCLK */
     RCC_PCLK2Config(RCC_HCLK_Div1); 
 
     /* PCLK1 = HCLK/2 */
     RCC_PCLK1Config(RCC_HCLK_Div2);
 
     
#if defined (STM32F10X_CL)
     /* Get HSE clock = 25MHz on PA8 pin(MC0)                                 */
     RCC_MCOConfig(RCC_MCO_HSE);
     /* Configure PLLS                                                        */
     /* PPL2 configuration: PLL2CLK=(HSE/5)*8 = 40MHz                         */
     RCC_PREDIV2Config(RCC_PREDIV2_Div5);
     RCC_PLL2Config(RCC_PLL2Mul_8);
     /* Enable PLL2                                                           */
     RCC_PLL2Cmd(ENABLE);
     /* wait till PLL2 is ready                                               */
     while(RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);

     /* PPL1 configuration:PLLCLK=(PLL2/5)*9=72MHz                            */
     RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2,RCC_PREDIV1_Div5);
     RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);
#else
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif

 
     /* Enable PLL */ 
     RCC_PLLCmd(ENABLE);
 
     /* Wait till PLL is ready */
     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
       {
       }
 
     /* Select PLL as system clock source */
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
 
     /* Wait till PLL is used as system clock source */
     while(RCC_GetSYSCLKSource() != 0x08)
      {
      }
    }

//	SystemInit();


   /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG 
         | RCC_APB2Periph_AFIO, ENABLE);

  // Enable USART1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);


 // STM3210E_LCD_Init();
   /* Clear the LCD */ 
 // LCD_Clear(White);
//   Lcd_Configuration();
  GPIO_Config();
//  SPI_Config();
  //InterruptConfig();
//  SysTick_Config();                                             /* Initialize the uC/OS-II tick interrupt                   */

  USART1_Config();
}


/*
*********************************************************************************************************
*                                     DISABLE ALL INTERRUPTS
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : None.
*
* Returns     : None.
*********************************************************************************************************
*/
void  BSP_IntDisAll (void)
{
  // CPU_IntDis();
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         uC/OS-II Timer Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*
* Arguments   : none
*
* Note(s)     : 1) The timer is setup for output compare mode BUT 'MUST' also 'freerun' so that the timer
*                  count goes from 0x00000000 to 0xFFFFFFFF to ALSO be able to read the free running count.
*                  The reason this is needed is because we use the free-running count in uC/OS-View.
*********************************************************************************************************
*/

//void  SysTick_Config(void)
//{
//    RCC_ClocksTypeDef  rcc_clocks;
//    INT32U         cnts;
//
//
//    RCC_GetClocksFreq(&rcc_clocks);
//
//    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;
//
//    SysTick_SetReload(cnts);
//    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
//    SysTick_CounterCmd(SysTick_Counter_Enable);
//    SysTick_ITConfig(ENABLE);
//}



void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE );
}

void USART1_Config(void)
{
  USART1_Init(115200,USART_Parity_Even);
}

//void InterruptConfig(void)
//{
// // NVIC_InitTypeDef NVIC_InitStructure;
//  //EXTI_InitTypeDef EXTI_InitStructure;
//
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//
//  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,0,0);
// // NVIC_SystemHandlerPriorityConfig(SystemHandler_PSV,3,3);
//
//   /* Enable the EXTI4 Interrupt */
//   /*
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQChannel;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure); */
//
//   //触摸屏的中断输入为PE4
//   /* Enable the EXTI Line4 Interrupt */
//   /*
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);  */
//}
//
//
///*
// T_CS PA4
// SPI1_SCK PA5
// SPI1_MISO PA6
//SPI1_MOSI PA7
//   T_BUSY PA8
//*/
//void SPI_Config(void) 
//{ 
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  SPI_InitTypeDef   SPI_InitStructure; 
//
//  //SPI1 Periph clock enable 
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE); 
//   //SPI2 Periph clock enable 
// // RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE ) ;
//
// //Configure SPI3 pins: SCK, MISO and MOSI 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12; 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
//  GPIO_Init(GPIOC,&GPIO_InitStructure); 
//
//  //Configure SPI3 pins: SCK, MISO and MOSI 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
//  GPIO_Init(GPIOC,&GPIO_InitStructure);	 
//
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
//  GPIO_Init(GPIOC,&GPIO_InitStructure);	
//
//  // SPI3 Config  
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
//  SPI_InitStructure.SPI_CRCPolynomial = 7; 
//  SPI_Init(SPI3,&SPI_InitStructure); 
//
//  // SPI1 enable  
//  SPI_Cmd(SPI3,ENABLE);  
//}
//
//
//unsigned char SPI_WriteByte(unsigned char data) 
//{ 
// unsigned char Data = 0; 
//
//   //Wait until the transmit buffer is empty 
//  while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET); 
//  // Send the byte  
//  SPI_I2S_SendData(SPI3,data); 
//
//   //Wait until a data is received 
//  while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET); 
//  // Get the received data 
//  Data = SPI_I2S_ReceiveData(SPI3); 
//
//  // Return the shifted data 
//  return Data; 
//}  
//void SpiDelay(unsigned int DelayCnt)
//{
// unsigned int i;
// for(i=0;i<DelayCnt;i++);
//}
//
//u16 TPReadX(void)
//{ 
//   u16 x=0;
//   TP_CS();
//   SpiDelay(10);
//   SPI_WriteByte(0x90);
//   SpiDelay(10);
//   x=SPI_WriteByte(0x00);
//   x<<=8;
//   x+=SPI_WriteByte(0x00);
//   SpiDelay(10);
//   TP_DCS(); 
//   x = x>>3;
//   return (x);
//}
//
//u16 TPReadY(void)
//{
// u16 y=0;
//  TP_CS();
//  SpiDelay(10);
//  SPI_WriteByte(0xD0);
//  SpiDelay(10);
//  y=SPI_WriteByte(0x00);
//  y<<=8;
//  y+=SPI_WriteByte(0x00);
//  SpiDelay(10);
//  TP_DCS();
//  y = y>>3; 
//  return (y);
//}

