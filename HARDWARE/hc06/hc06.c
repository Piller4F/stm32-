#include "hc06.h"
#include "usart.h"

u8 USART2_RX_BUF[USART2_MAX_RECV_LEN];    //���ܻ���

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���

u16 USART2_RX_STA=0;  	 
void USART2_IRQHandler(void) {
	u8 res;	    
	if(USART2->SR&(1<<5)) {//���յ�����	 
		res=USART2->DR; 			 
		if(USART2_RX_STA<USART2_MAX_RECV_LEN) {		//�����Խ�������
			TIM4->CNT=0;         					//���������
			if(USART2_RX_STA==0) {	 	//ʹ�ܶ�ʱ��4���ж�
				TIM4->CR1|=1<<0;
			}				
			USART2_RX_BUF[USART2_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else {
			USART2_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  											 
}   

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	
void USART2_Init(u32 pclk2,u32 bound) {              //����2��ʼ��
	RCC->APB2ENR|=1<<2;         //ʹ��GPIOA
	GPIOA->CRL&=0xFFFF00FF;     //����IO
	GPIOA->CRL|=0x00008B00;        
	RCC->APB1ENR|=1<<17;        //ʹ�ܴ���ʱ��
	RCC->APB1RSTR|=1<<17;       //��λ����2
	RCC->APB1RSTR&=~(1<<17);    //ֹͣ��λ
	USART2->BRR=(pclk2*1000000)/(bound);// ����������	
	USART2->CR1|=0x200C;        //1ֹͣλ����У��λ
	USART2->CR1|=1<<8;          //PE�ж�ʹ��
	USART2->CR1|=1<<5;       	//���ջ������ǿ��ж�ʹ��	 
	MY_NVIC_Init(2,3,USART2_IRQn,2);//��2��������ȼ� 
	TIM4_Init(99,7199);         //10ms���ж�
	USART2_RX_STA=0;
	TIM4->CR1&=~(1<<0);         //�رն�ʱ��4
	//TIM4->CR1|=1<<0;
}

void TIM4_Init(u16 arr,u16 psc) {
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ   
	TIM4->PSC=psc;  	//Ԥ��Ƶ��
 	TIM4->DIER|=1<<0;   //��������ж�				
 	TIM4->CR1|=0x01;  	//ʹ�ܶ�ʱ��4	  	   
   	MY_NVIC_Init(1,3,TIM4_IRQn,2);//��ռ2�������ȼ�3����2	��2�����ȼ����								 
}

void TIM4_IRQHandler(void) { 	
	if(TIM4->SR&0X01) { //�����ж�	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		//usart2_send_char('o');
		TIM4->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM4->CR1&=~(1<<0);
	}	    
}

//����2����1���ַ� 
//c:Ҫ���͵��ַ�
void usart2_send_char(u8 c) {
	while((USART2->SR&0X40)==0);//�ȴ���һ�η������   
	USART2->DR=c;   	
}

