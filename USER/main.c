#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "dsp_g2.h"

extern u8 Rcv_Busy;//用来说明串口能不能继续接收下一帧数据
u8 CMD_GET_DATAS[8]={0x01,0x03,0x00,0x48,0x00,0x0a,0x45,0xdb};
extern float ADC_DATA[64];
extern int Current_Num;
extern int Flag_Can_Do_FFT;
/****************************************************************
	名称：Uart5_Send_String()
	参数：字符串指针
	功能：串口5发送一个数组，字符串以\0结尾
	返回：无
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

int main(void)
{
		u8 tempstring[50]={0};//临时字符串存储
		u16 adcx;			
		u16 cx;		
		float temp = 0.0;
	/*一些初始化工作*/
		delay_init();	//延时函数初始化
		Adc_Init();		//ADC 初始化
		LED_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(4800);	 //串口初始化为480
		UART5_Config(4800); 
    TIM3_Int_Init(499, 7199); //定时器3初始化为50ms中断一次
		TIM4_Int_Init(499,44);//定时器4设置50hz的计数频率20/64ms中断一次
   
		
	//-------------------------

		dsp_asm_init();
		// dsp_asm_test();
		Uart5_Send_String("initial OK\r\n\0");
		//delay_ms(500);
	
	while(1)
	{
		 //Uart5_Send_String("System Started...\r\n\0");
		 //delay_ms(500);
		// Uart5_Send_String("Prepare data for FFT asm.\r\n\0");
			//adcx=Get_Adc_Average(ADC_Channel_1,10);//从ADC里取出10个数据取平均值
		
			//cx = Get_Adc(ADC_Channel_1);//从ADC里面取出一个数据
			//temp=(float)adcx*(3.3/4096);
			//sprintf(tempstring,"输出电压幅值: %.2f\r\n\0",temp);
			//Uart5_Send_String(tempstring);
		
		//调用快速傅里叶变换
		 //delay_ms(500);
		 //dsp_asm_test();
		delay_ms(1000);
		if(Flag_Can_Do_FFT==1)
		{
			int i;
			for(i=0;i<=64;i++){
				sprintf(tempstring,"%.2f\r\n\0",ADC_DATA[i]);
				Uart5_Send_String(tempstring);
				delay_ms(500);
			}
			dsp_g2_test();
			Flag_Can_Do_FFT=0;
			Current_Num=0;
			TIM_Cmd(TIM4, ENABLE); 
		}
		 
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);//等待上一个字节发送完毕
		//delay_ms(300);
	}
	/*进入著名的大循环*/
    while(1)
    {
		//发送数据，获取当前数据
		zh_USART_SendString(CMD_GET_DATAS,8);
        if(Rcv_Busy == 1) //串口忙
		{
			ModHandle();//处理接受到的数据
		}
		delay_ms(1000);
    }
}
