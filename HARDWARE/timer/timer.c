#include "timer.h"
#include "88led.h"
#include "car.h"

extern int led[8];       //列
extern int line[8];      //行
extern int picture[8][8];//图案

void TIM3_IRQHandler(void){ 
	static u8 j=0;
	u8 i;
	if(TIM3->SR&0X0001)//溢出中断
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
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器设置
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}



