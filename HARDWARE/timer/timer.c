#include "timer.h"
#include "88led.h"
#include "car.h"
#include "echo.h"
#include "delay.h"
#include "usart.h"

extern int led[8];       //��
extern int line[8];      //��
extern int picture[8][8];//ͼ��

void TIM3_IRQHandler(void){ 
	static u8 j=0;
	u8 i;
	//	Trig=!Trig;
	//	printf("1");
	//	delay_us(20);
	//	Trig=0;
	if(TIM3->SR&0X0001)//����ж�
	{
		for(i=0;i<8;++i) {
			if(i<4) {
				if(i==j) PCout(line[i])=1;
				else     PCout(line[i])=0;
			} else {
				if(i==j) PBout(line[i])=1;
				else     PBout(line[i])=0;
			}	
		}
		Led_Light((u8)picture[(int)car.status][j]);	 
		j=(j+1)%8;
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  	//Ԥ��Ƶ������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2									 
}



