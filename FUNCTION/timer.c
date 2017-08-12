#include "timer.h"
#include "usart.h"
#include "adc.h"
extern u8 RCv_Time_Out;
extern u8 Rcv_Len;
extern u8 Rcv_Busy;
extern u8 Rcv_Complete;

float ADC_DATA[64];
int Current_Num=0;
int Flag_Can_Do_FFT=0;
int FLag_LED=0;

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设						 
}


void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			RCv_Time_Out=1;//发生超时
			 if(Rcv_Complete==0)//接收正常结束
				{
					//printf("数据接收成功\r\n");
					Rcv_Complete=1;//数据接收结束
					RCv_Time_Out=0;//清除接收超时标志
					Rcv_Busy=1;//串口忙
					USART_RX_BUF[0]=Rcv_Len;//存入数据总长度
				}
			//printf("发生超时\r\n");
		}
}



//------------------------------------------------------- 中断定时器4 -------------------------------------

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设						 
}

void TIM4_IRQHandler(void)   //TIM4中断
{
		u8 tempstring[50]={0};//临时字符串存储
		u16 adcx;			
		u16 cx;		
		float temp = 0.0;
		int i;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			
		//-------------------------------------------------
//		if(FLag_LED==0)
//		{
//			GPIO_SetBits(GPIOC,GPIO_Pin_5);	 //IO口输出高电平
//			FLag_LED=1;
//		}
//		else
//		{
//			GPIO_ResetBits(GPIOC,GPIO_Pin_5);	 //IO口输出高电平
//			FLag_LED=0;
//		}
		//--------------------------------------------------
			if(Current_Num<=64)
			{
				
				//读取一个数据
				cx=Get_Adc(ADC_Channel_1);
				temp=(float)cx*(3.3/4096);
				ADC_DATA[Current_Num] = temp*1.0f;
				
				Current_Num++;
			}
			else
			{
				//接收到了第64个数据
				
				
				Current_Num=0;
				TIM_Cmd(TIM4, DISABLE);  //失能TIMx外设	
				Flag_Can_Do_FFT=1;				
			}
				
			
			cx = Get_Adc(ADC_Channel_1);//从ADC里面取出一个数据
			temp=(float)cx*(3.3/4096);

			
		}
		
	}


