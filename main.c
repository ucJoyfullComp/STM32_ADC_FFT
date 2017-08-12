/**
  ******************************************************************************
  * @file    ADC/3ADCs_DMA/main.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "bsp.h"
#include "dsp_asm.h"
#include "dsp_g2.h"
#include "usart1.h"
#include "main.h"
#include "delay.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_3ADCs_DMA
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

/* Private function prototypes -----------------------------------------------*/
void  SwitchLED(u8 LEDNo);
void SysTick_Configuration(void);

void Delay(__IO uint32_t nTime);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	delay_init();
	UART5_Config(4800);
  BSP_Init();
  SysTick_Configuration();
	Uart5_Send_String("System Started...\r\n");
	Uart5_Send_String("Prepare data for FFT asm.\r\n");
	delay_ms(200);
//  printf("System Started...\r\n");
//  printf("Prepare data for FFT asm.\r\n");
  dsp_asm_init();
  dsp_asm_test();
	delay_ms(200);
	Uart5_Send_String("Prepare data for FFT g2.\r\n");
	Uart5_Send_String("go to while loop. \r\n");
//  printf("Prepare data for FFT g2.\r\n");
  dsp_g2_test();
//  printf("go to while loop. \r\n");
  
  while(1)
  {
    SwitchLED(1);
    Delay(1000);
    SwitchLED(2);
    Delay(1000);
    SwitchLED(3);
    Delay(1000);
    SwitchLED(4);
    Delay(1000);
  }
}

void  SwitchLED(u8 LEDNo)
{
    switch(LEDNo)
    {
      case 1:
      case 7:
      GPIO_SetBits(GPIOD, GPIO_Pin_2);
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      break;
      case 2:
      case 6:
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      GPIO_SetBits(GPIOD, GPIO_Pin_3);
      GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      break;
      case 3:
      case 5:
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      GPIO_SetBits(GPIOD, GPIO_Pin_4);
      GPIO_ResetBits(GPIOD, GPIO_Pin_7);
      break;
      case 4:
      GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      GPIO_ResetBits(GPIOD, GPIO_Pin_3);
      GPIO_ResetBits(GPIOD, GPIO_Pin_4);
      GPIO_SetBits(GPIOD, GPIO_Pin_7);
      break;
    }
}

void SysTick_Configuration(void)
{
  if(SysTick_Config(SystemCoreClock / 1000))
  {
    while(1);
  }
}
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
