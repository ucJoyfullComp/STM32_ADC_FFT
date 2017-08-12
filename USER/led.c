#include "led.h"

/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，delay(6000000)延时约1s
* 输    入         : i
* 输    出         : 无
*******************************************************************************/
void delay(u32 i)
{
	while(i--);
}

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init()	  //端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO

	SystemInit();	//系统时钟初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); /* 开启GPIO时钟 */

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=LED;	 //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	  //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(GPIOC,&GPIO_InitStructure); /* 初始化GPIO */	
}

/*******************************************************************************
* 函 数 名         : led_display
* 函数功能		   : LED显示函数  LED闪烁
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void led_display()	//LED闪烁
{
	GPIO_SetBits(GPIOC,LED);	 //IO口输出高电平
	delay(6000000);//延时约为1s
	GPIO_ResetBits(GPIOC,LED); //IO口输出低电平
	delay(6000000);
	GPIO_SetBits(GPIOC,LED);	 //IO口输出高电平
}

/*******************************************************************************
* 函 数 名         : led_huayang
* 函数功能		   : LED跑马灯函数  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void led_huayang()	 //led花样跑马灯
{
	u8 i;
	for(i=0;i<8;i++)   //从左到右
	{
		GPIO_Write(GPIOC,(u16)~(0x01<<i));//给IO口写输入数据
		delay(2000000);
	}
	for(i=0;i<8;i++)  //从右到左
	{
		GPIO_Write(GPIOC,(u16)~(0x80>>i));
		delay(2000000);
	}
	for(i=0;i<4;i++)  //从右到左
	{
		GPIO_Write(GPIOC,(u16)~(0x11<<i));
		delay(2000000);
	}
	for(i=0;i<4;i++)  //从右到左
	{
		GPIO_Write(GPIOC,(u16)~(0x88>>i));
		delay(2000000);
	}	
}
