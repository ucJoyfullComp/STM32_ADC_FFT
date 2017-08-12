#include "stm32f10x.h"
#include"crc.h"
/*ModBus ����CRC��Ĳ���Ϊ��

(1).Ԥ��16λ�Ĵ���ΪFFFFH���ƴ˼Ĵ���ΪCRC�Ĵ�����

(2).�ѵ�һ��8λ������CRC�Ĵ����ĵ�λ����򣬰ѽ������CRC�Ĵ�����

(3).�ѼĴ�������������һλ(����λ)����0����λ��������λ��

(4).������λΪ0���ظ���3��(�ٴ���λ)������λΪ1��CRC�Ĵ��������ʽA001��1010 0000 0000 0001���������

(5).�ظ�����3��4��ֱ������8�Σ���������8λ����ȫ�������˴�����

(6).�ظ�����2������5��������һ��8λ���ݵĴ�����

(7).���õ���CRC�Ĵ�����ΪCRC�롣(CRC�� = CRC_L +CRC_H)

*/

/*****************crcУ��**********************************************/

//crc���ɺ���������crc�洢��Ԥ�ȶ��ڵ�������

//���÷�ʽcrc16��ָ�����ݵ�ָ�룬��ҪУ������ݳ��ȣ�



u16 crc16(u8 *ptr,u8 len)

{

unsigned long wcrc=0XFFFF;//Ԥ��16λcrc�Ĵ�������ֵȫ��Ϊ1

int i=0,j=0;//�������

for(i=0;i<len;i++)//ѭ������ÿ������

	{

		wcrc^=*ptr++;//����λ������crc�Ĵ������.ָ���ַ���ӣ�ָ���¸�����

		for(j=0;j<8;j++)//ѭ���������ݵ�

			{

				if(wcrc&0X0001)//�ж����Ƴ����ǲ���1�������1�������ʽ�������

					{
						wcrc=wcrc>>1^0XA001;//�Ƚ���������һλ��������Ķ���ʽ�������
					}

				else//�������1����ֱ���Ƴ�

					{
						wcrc>>=1;//ֱ���Ƴ�
					}
			}
	}
return wcrc<<8|wcrc>>8;//�Ͱ�λ��ǰ���߰�λ�ں�
}