#include "delay.h"
#include "usart2.h"
#include "stdio.h"
#include "string.h"

u16 USART2_RX_STA=0;
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];


void USART2_IRQHandler(void) {
	u8 res;
	if(USART2->SR&(1<<5)){//�յ�����
		res=USART2->DR;
		if(USART2_RX_STA<USART2_MAX_RECV_LEN){
			TIM4->CNT=0;
			if(USART2_RX_STA==0) TIM4->CR1 |= 1;
			USART2_RX_BUF[USART2_RX_STA++]=res;
		}
		else USART2_RX_STA |= 1<<15;//ǿ�Ʊ�����ݽ������
	
	}
}

void USART2_Init(u32 pcl,u32 bound) {
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL &= 0xffff00ff;
	GPIOA->CRL |= 0xffff8Bff;//3���� 2�����
	
	RCC->APB1ENR |= 1<<17;//����ʱ��ʹ��
	RCC->APB1RSTR |=1<<17;
	RCC->APB1RSTR &=~(1<<17);//ֹͣ��λ
	
	USART2->BRR=(pcl*1000000)/bound;//���ò�����
	USART2->CR1|=0x200C;//usartʹ�� һ����ʼλ8������λ ����ʹ��
	
	USART2->CR1 |=1<<8;//pe�ж�ʹ��
	USART2->CR1 |=1<<5;//���ջ������ǿ��ж�ʹ��

	MY_NVIC_Init(2,3,USART2_IRQn,2);
	
	TIM4_Init(99,7199);//10ms�ж�
	USART2_RX_STA=0;
	TIM4->CR1 &= ~1;
	
}

void TIM4_Init(u16 arr, u16 psc){
	RCC->APB1ENR |=1<<2;
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->DIER|=1;
	TIM4->CR1|=1;
	MY_NVIC_Init(1,3,TIM4_IRQn,2);//��ռ2�������ȼ�3��
}

void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//�Ǹ����ж�
	{	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM4->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM4->CR1 &= ~1;	    //�ر�TIM4  
	}	    
}
