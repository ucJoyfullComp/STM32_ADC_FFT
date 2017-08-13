#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "dsp_g2.h"
#include "handlefft.h"


extern u8 Rcv_Busy;																				//用来说明串口能不能继续接收下一帧数据
u8 CMD_GET_DATAS[8]={0x01,0x03,0x00,0x48,0x00,0x0a,0x45,0xdb};
extern float ADC_DATA[64];
extern int Current_Num;
extern int Flag_Can_Do_FFT;
extern float harmonic_regression;
extern float harmonic_intercept;
float last_harmonic_regression = 0;
float last_harmonic_intercept = 0;

int main(void)
{
	//--------------------- 基础申明 --------------------
		u8 tempstring[50]={0};															//临时字符串存储
		u16 adcx;			
		u16 cx;		
		float temp = 0.0;

		delay_init();																				//延时函数初始化
		Adc_Init();																					//ADC 初始化
		LED_Init();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 		//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		uart_init(4800);	 																	//串口初始化为480
		UART5_Config(4800); 
		TIM3_Int_Init(499, 7199); 													//定时器3初始化为50ms中断一次
		TIM4_Int_Init(499,44);															//定时器4设置50hz的计数频率20/64ms中断一次
		
   
	//------------------------- 初始化完成 ---------------

//		Uart5_Send_String("Initial OK\r\n\0");
	while(1)
	{																											//调用快速傅里叶变换
		delay_ms(1000);
		if(Flag_Can_Do_FFT==1)
		{
//发送调试数据，切记：延时函数演示不能太小，会导致电脑蓝屏==================
//			int i;
//			for(i=0;i<64;i++){
//				sprintf(tempstring,"%.2f\r\n\0",ADC_DATA[i]);
//				Uart5_Send_String(tempstring);
//				delay_ms(500);
//			}
//==========================================================================		
//			delay_ms(500);
			dsp_g2_test();
																//暂存上一次FFT 处理过的数据
			last_harmonic_regression = harmonic_regression;
			last_harmonic_intercept = harmonic_intercept;
			handle_fft_result();
			Flag_Can_Do_FFT=0;
			Current_Num=0;
			TIM_Cmd(TIM4, ENABLE); 
		}
		 
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC)  == RESET);//等待上一个字节发送完毕
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
