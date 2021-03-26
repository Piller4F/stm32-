#include "delay.h"
#include "usart2.h"
#include "stdio.h"
#include "string.h"

u16 USART2_RX_STA=0;
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];


void USART2_IRQHandler(void) {
	u8 res;
	if(USART2->SR&(1<<5)){//收到数据
		res=USART2->DR;
		if(USART2_RX_STA<USART2_MAX_RECV_LEN){
			TIM4->CNT=0;
			if(USART2_RX_STA==0) TIM4->CR1 |= 1;
			USART2_RX_BUF[USART2_RX_STA++]=res;
		}
		else USART2_RX_STA |= 1<<15;//强制标记数据接收完成
	
	}
}

void USART2_Init(u32 pcl,u32 bound) {
	RCC->APB2ENR|=1<<2;
	GPIOA->CRL &= 0xffff00ff;
	GPIOA->CRL |= 0xffff8Bff;//3接收 2是输出
	
	RCC->APB1ENR |= 1<<17;//串口时钟使能
	RCC->APB1RSTR |=1<<17;
	RCC->APB1RSTR &=~(1<<17);//停止复位
	
	USART2->BRR=(pcl*1000000)/bound;//设置波特率
	USART2->CR1|=0x200C;//usart使能 一个起始位8个数据位 各种使能
	
	USART2->CR1 |=1<<8;//pe中断使能
	USART2->CR1 |=1<<5;//接收缓冲区非空中断使能

	MY_NVIC_Init(2,3,USART2_IRQn,2);
	
	TIM4_Init(99,7199);//10ms中断
	USART2_RX_STA=0;
	TIM4->CR1 &= ~1;
	
}

void TIM4_Init(u16 arr, u16 psc){
	RCC->APB1ENR |=1<<2;
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->DIER|=1;
	TIM4->CR1|=1;
	MY_NVIC_Init(1,3,TIM4_IRQn,2);//抢占2，子优先级3，
}

void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//是更新中断
	{	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM4->SR&=~(1<<0);		//清除中断标志位		   
		TIM4->CR1 &= ~1;	    //关闭TIM4  
	}	    
}
