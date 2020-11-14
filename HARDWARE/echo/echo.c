 #include "echo.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

//#define TRIG PBout(13)
//#define ECHO PAin(0)

u16 TIM2_CAP_STA;
u16 TIM2_CAP_VAL;

void Echo_Init(void) {
	RCC->APB2ENR|=1<<3; 
	
	GPIOB->CRH&=0xFF0FFFFF;
	GPIOB->CRH|=0x00300000;
	
	Trig=0;
}

void Tim2_Cap_Init(u16 arr,u16 psc) {
	RCC->APB1ENR|=1<<0;
	RCC->APB2ENR|=1<<2;
	
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=0x00000004;
	
	TIM2->ARR=arr;
	TIM2->PSC=psc;
	
	TIM2->CCMR1|=1<<0;
	
	TIM2->CCER&=~(1<<1);
	TIM2->CCER|=1<<0;
	
	TIM2->DIER|=1<<1;
	TIM2->DIER|=1<<0;
	
	TIM2->CR1|=1<<0;
	
	MY_NVIC_Init(2,0,TIM2_IRQn,2);
}

u16 TIM2_CAP_STA=0;
u16 TIM2_CAP_VAL;

void TIM2_IRQHandler(void) {
	u16 t;
	t=TIM2->SR;
	if((TIM2_CAP_STA&0x8000)==0) {
		if(t&0x01) {
			if((TIM2_CAP_STA&0x3FFF)==0x3FFF) {
				TIM2_CAP_STA|=0x8000;
				TIM2_CAP_VAL=0xFFFF;
			} else TIM2_CAP_STA++;			
		}
		if(t&0x02) {
			if((TIM2_CAP_STA&0x4000)==0) {
				TIM2_CAP_STA=0x4000;
				TIM2_CAP_VAL=0;
				TIM2->CNT=0;
				TIM2->CCER|=1<<1;
			} else {
				TIM2_CAP_STA|=0x8000;
				TIM2_CAP_VAL=TIM2->CCR1;
				TIM2->CCER&=~(1<<1);   
			}
		}
	}
	TIM2->SR=0;
}









