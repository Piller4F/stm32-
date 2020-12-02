#include "hc06.h"
#include "usart.h"

u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];    //接受缓冲

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度

u16 USART2_RX_STA=0;  	 
void USART2_IRQHandler(void) {
	u8 res;	    
	if(USART2->SR&(1<<5)) {//接收到数据	 
		res=USART2->DR; 			 
		if(USART2_RX_STA<USART2_MAX_RECV_LEN) {		//还可以接收数据
			TIM4->CNT=0;         					//计数器清空
			if(USART2_RX_STA==0) {	 	//使能定时器4的中断
				TIM4->CR1|=1<<0;
			}				
			USART2_RX_BUF[USART2_RX_STA++]=res;		//记录接收到的值	 
		}else {
			USART2_RX_STA|=1<<15;					//强制标记接收完成
		} 
	}  											 
}   

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	
void USART2_Init(u32 pclk2,u32 bound) {              //串口2初始化
	RCC->APB2ENR|=1<<2;         //使能GPIOA
	GPIOA->CRL&=0xFFFF00FF;     //配置IO
	GPIOA->CRL|=0x00008B00;        
	RCC->APB1ENR|=1<<17;        //使能串口时钟
	RCC->APB1RSTR|=1<<17;       //复位串口2
	RCC->APB1RSTR&=~(1<<17);    //停止复位
	USART2->BRR=(pclk2*1000000)/(bound);// 波特率设置	
	USART2->CR1|=0x200C;        //1停止位，无校验位
	USART2->CR1|=1<<8;          //PE中断使能
	USART2->CR1|=1<<5;       	//接收缓冲区非空中断使能	 
	MY_NVIC_Init(2,3,USART2_IRQn,2);//组2，最低优先级 
	TIM4_Init(99,7199);         //10ms的中断
	USART2_RX_STA=0;
	TIM4->CR1&=~(1<<0);         //关闭定时器4
	//TIM4->CR1|=1<<0;
}

void TIM4_Init(u16 arr,u16 psc) {
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值   
	TIM4->PSC=psc;  	//预分频器
 	TIM4->DIER|=1<<0;   //允许更新中断				
 	TIM4->CR1|=0x01;  	//使能定时器4	  	   
   	MY_NVIC_Init(1,3,TIM4_IRQn,2);//抢占2，子优先级3，组2	在2中优先级最低								 
}

void TIM4_IRQHandler(void) { 	
	if(TIM4->SR&0X01) { //更新中断	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		//usart2_send_char('o');
		TIM4->SR&=~(1<<0);		//清除中断标志位		   
		TIM4->CR1&=~(1<<0);
	}	    
}

//串口2发送1个字符 
//c:要发送的字符
void usart2_send_char(u8 c) {
	while((USART2->SR&0X40)==0);//等待上一次发送完毕   
	USART2->DR=c;   	
}

