#include "delay.h"
#include "usart.h"
#include "timer.h"

#define NPT 64
#define PI2 6.28318530717959

extern float result[NPT];
float harmonic_difference;
float harmonic_regression = 0;
float harmonic_intercept = 0;

//���Իع���б��
void linear_regression(){
	u8 temp[50]={0};
	int i, j;
	float a = 0,b = 0,lxx = 0,lxy = 0;
	float X = 0,Y = 0;
	float sum_x = 0,sum_y = 0;
	
	for (i = 0;i!=31;i++){
		sum_x += i;
		sum_y += result[i];

	}

	X = sum_x/32;
	Y = sum_y/32;
	
	for (j = 0;j != 31;j++){
		lxx += (i - X)*(i - X);
		lxy += (result[i] - Y)*(result[i] - Y);
	}
	
	b = lxy/lxx;
	a = Y - b*X;
	harmonic_regression = b;
	harmonic_intercept = a;
	
//	sprintf(temp,"linear regression equation: Y = %.2fX + %.2f\r\n\0",b,a);
//	Uart5_Send_String(temp);
}


//����FFT�����������Ҫ�����������
void handle_fft_result(){
	u8 temp[50]={0};	
	int i;
	//���������һ��г����ֵ
	harmonic_difference = result[0] - result[1];
	//	sprintf(temp,"harmonic_difference:%.2f\r\n\0",harmonic_difference);
	//	Uart5_Send_String(temp);
	delay_ms(500);
//	sprintf(temp,"%.2f-%.2f=%.2f",result[0],result[1],harmonic_difference);
//	Uart5_Send_String(temp);
	//����31����ɢ��������߶�
	linear_regression();
}


//ʶ��������Ҫ����Ͻ�����бȶԣ��õ�����ʶ����
void handle_recongize(){
	

}