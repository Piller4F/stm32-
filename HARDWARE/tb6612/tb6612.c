#include "tb6612.h"
#include "sys.h"

//#define STBY PBout(8)
//#define AIN1 PBout(9)
//#define AIN2 PBout(10)
//#define BIN1 PBout(11)
//#define BIN2 PBout(12)

//����pa6 pa7
void Tim4_Pwm_Init(u16 arr,u16 psc) {
RCC->APB1ENR|=1<<2;//TIM4ʱ��ʹ��
	GPIOB->CRL&=0x00FFFFFF;//PB6 PB7 �� ���֮ǰ������
	GPIOB->CRL|=0xBB000000;//��������
	TIM4->ARR=arr;//�趨���������Զ���װֵ
	TIM4->PSC=psc;//Ԥ��Ƶ������
	
	TIM4->CCMR1|=6<<4;//TIM4 CH1 PWMģʽ1 ���ϼ��� 
	TIM4->CCMR1|=1<<3;//CH1 Ԥװ��ʹ��
	
	TIM4->CCMR1|=6<<12;//TIM4 CH2 
	TIM4->CCMR1|=1<<11;//CH2 Ԥװ��ʹ��
	
	TIM4->CCER|=1;//OC1���ʹ��
	TIM4->CCER|=1<<4;
	TIM4->CR1=0x0080;//ARPEʹ��
	TIM4->CR1|=1;//ʹ�ܼ�����
}

void Tb6612_Init(void) {
	RCC->APB2ENR|=1<<3;
	
	GPIOB->CRL&=0x00FFFFFF;
	GPIOB->CRL|=0x33000000;
	
	GPIOB->CRH&=0xFFF00000;
	GPIOB->CRH|=0x00033333;
	
	STBY=1;
	AIN1=1;
	AIN2=0;
	BIN1=1;
	BIN2=0;
}





