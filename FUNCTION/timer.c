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

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����						 
}


void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			RCv_Time_Out=1;//������ʱ
			 if(Rcv_Complete==0)//������������
				{
					//printf("���ݽ��ճɹ�\r\n");
					Rcv_Complete=1;//���ݽ��ս���
					RCv_Time_Out=0;//������ճ�ʱ��־
					Rcv_Busy=1;//����æ
					USART_RX_BUF[0]=Rcv_Len;//���������ܳ���
				}
			//printf("������ʱ\r\n");
		}
}



//------------------------------------------------------- �ж϶�ʱ��4 -------------------------------------

void TIM4_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����						 
}

void TIM4_IRQHandler(void)   //TIM4�ж�
{
		u8 tempstring[50]={0};//��ʱ�ַ����洢
		u16 adcx;			
		u16 cx;		
		float temp = 0.0;
		int i;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			
		//-------------------------------------------------
//		if(FLag_LED==0)
//		{
//			GPIO_SetBits(GPIOC,GPIO_Pin_5);	 //IO������ߵ�ƽ
//			FLag_LED=1;
//		}
//		else
//		{
//			GPIO_ResetBits(GPIOC,GPIO_Pin_5);	 //IO������ߵ�ƽ
//			FLag_LED=0;
//		}
		//--------------------------------------------------
			if(Current_Num<=64)
			{
				
				//��ȡһ������
				cx=Get_Adc(ADC_Channel_1);
				temp=(float)cx*(3.3/4096);
				ADC_DATA[Current_Num] = temp*1.0f;
				
				Current_Num++;
			}
			else
			{
				//���յ��˵�64������
				
				
				Current_Num=0;
				TIM_Cmd(TIM4, DISABLE);  //ʧ��TIMx����	
				Flag_Can_Do_FFT=1;				
			}
				
			
			cx = Get_Adc(ADC_Channel_1);//��ADC����ȡ��һ������
			temp=(float)cx*(3.3/4096);

			
		}
		
	}


