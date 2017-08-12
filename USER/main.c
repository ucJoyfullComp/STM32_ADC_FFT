#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "dsp_g2.h"

extern u8 Rcv_Busy;																				//����˵�������ܲ��ܼ���������һ֡����
u8 CMD_GET_DATAS[8]={0x01,0x03,0x00,0x48,0x00,0x0a,0x45,0xdb};
extern float ADC_DATA[64];
extern int Current_Num;
extern int Flag_Can_Do_FFT;
/****************************************************************
	���ƣ�Uart5_Send_String()
	�������ַ���ָ��
	���ܣ�����5����һ�����飬�ַ�����\0��β
	���أ���
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
	//--------------------- �������� --------------------
		u8 tempstring[50]={0};															//��ʱ�ַ����洢
		u16 adcx;			
		u16 cx;		
		float temp = 0.0;

		delay_init();																				//��ʱ������ʼ��
		Adc_Init();																					//ADC ��ʼ��
		LED_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(4800);	 																	//���ڳ�ʼ��Ϊ480
		UART5_Config(4800); 
    TIM3_Int_Init(499, 7199); 													//��ʱ��3��ʼ��Ϊ50ms�ж�һ��
		TIM4_Int_Init(499,44);															//��ʱ��4����50hz�ļ���Ƶ��20/64ms�ж�һ��
		
   
	//------------------------- ��ʼ����� ---------------

		Uart5_Send_String("Initial OK\r\n\0");
	while(1)
	{																											//���ÿ��ٸ���Ҷ�任
		delay_ms(1000);
		if(Flag_Can_Do_FFT==1)
		{
//���͵������ݣ��мǣ���ʱ������ʾ����̫С���ᵼ�µ�������==================
//			int i;
//			for(i=0;i<64;i++){
//				sprintf(tempstring,"%.2f\r\n\0",ADC_DATA[i]);
//				Uart5_Send_String(tempstring);
//				delay_ms(500);
//			}
//==========================================================================		
//			delay_ms(500);
			dsp_g2_test();
			Flag_Can_Do_FFT=0;
			Current_Num=0;
			TIM_Cmd(TIM4, ENABLE); 
		}
		 
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);//�ȴ���һ���ֽڷ������
		//delay_ms(300);
	}
	
	/*���������Ĵ�ѭ��*/
    while(1)
    {
		//�������ݣ���ȡ��ǰ����
		zh_USART_SendString(CMD_GET_DATAS,8);
        if(Rcv_Busy == 1) //����æ
		{
			ModHandle();//������ܵ�������
		}
		delay_ms(1000);
    }
}
