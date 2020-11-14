#include "remote.h"
#include "sys.h"

void Remote_Init(void) {
	RCC->APB1ENR|=1<<3;     //TIM5ʱ��ʹ��
	RCC->APB2ENR|=1<<2;     //GPIOAʹ��
	GPIOA->CRL&=0xFFFFFF0F; //PA1����ˢ0
	GPIOA->CRL|=0x00000080; //PA1����Ϊ����
	GPIOA->ODR|=1<<1;       //����Ϊ����
	TIM5->ARR=10000;        //���ü�������װ��ֵ
	TIM5->PSC=71;           //72��Ƶ
	TIM5->CCMR1|=1<<8;
	TIM5->CCMR1|=3<<12;
	TIM5->CCMR1|=0<<10;
	TIM5->CCER|=0<<5;
	TIM5->CCER|=1<<4;       
	TIM5->DIER|=1<<2;       //�����ж�  
	TIM5->DIER|=1<<0;       //��������ж�
	TIM5->CR1|=0x01;        //ʹ�ܶ�ʱ��5
	MY_NVIC_Init(1,3,TIM5_IRQn,2);
	
}

u8 RmtSta=0;
u16 Dval;        //�½��ز���ֵ
u32 RmtRec=0;    //������ܵ�������
u8  RmtCnt=0;    //ͳ�ƺ��ⰴ�µĴ���

void TIM5_IRQHandler(void) {
	u16 tsr;
	tsr=TIM5->SR;                    //��ȡ��ǰ��ʱ��״̬
	if(tsr&0x01) {                   //����������ж�
		if(RmtSta&0x80) {            //������ܵ���������ʶ��
			RmtSta&=~0x10;           //ȡ��������
			if((RmtSta&0x0F)==0x00)  //��һ������ж�
				RmtSta|=1<<6;        //��ʾ���ܵ�ȫ����Ϣ
			if((RmtSta&0x0F)<14)     //��������������14��ͼ��������������ͳ��
				RmtSta++;            
			else {                   
				RmtSta&=~(1<<7);     //���������ʶ��
				RmtSta&=0xF0;        //��ռ�����
			}
		}
	}
	if(tsr&(1<<2)) {                 //���ش����ж�
		if(RDATA) {                  //�����ش���
			TIM5->CCER|=1<<5;        //�ı�Ϊ�½��ش���
			TIM5->CNT=0;             //��ն�ʱ��ֵ
			RmtSta|=0x10;            //����������ѱ�����
		} else {
			Dval=TIM5->CCR2;         //��ȡ��ǰ��ֵ
			TIM5->CCER&=~(1<<5);     //�ı�Ϊ�����ش���
			if(RmtSta&0x10) {
				if(RmtSta&0x80) {    //��ʾ�Ѿ����ܵ���������
					if(Dval>300&&Dval<800) {
						RmtRec<<=1;  //��ʾ���ܵ�0
					} else if(Dval>1400&&Dval<1800) {
						RmtRec<<=1;
						RmtRec|=1;   //��ʾ���ܵ�1
					} else if(Dval>2200&&Dval<2600) {
						RmtCnt++;
						RmtSta&=0xF0;//��ռ�����
					} 
				} else if(Dval>4200&&Dval<4700) {  //���ܵ���������
					RmtSta|=1<<7;
					RmtCnt=0;
				}	 
			}
			RmtSta&=~(1<<4);         //���¿�ʼ�ж��Ƿ���ܵ��ϱ���
		}
		
	}
	TIM5->SR=0;                      //����жϱ�־λ
}

u8  Remote_Scan(void) {
	u8 sta=0;
	u8 t1,t2;
	if(RmtSta&(1<<6)) {             //�Ѿ����ܵ���һ��������������Ϣ
		t1=RmtRec>>24;              //�õ���ַ��
		t2=(RmtRec>>16)&0xff;       //�õ���ַ����
		if(t1==(u8)~t2) {           
			t1=RmtRec>>8;
			t2=RmtRec;
			if(t1==(u8)~t2) sta=t1; //�жϼ�ֵ
		} 
		if((sta==0)||(RmtSta&0x80)==0) { //ң���Ѿ�û�а�����
			RmtSta&=~(1<<6);
			RmtCnt=0;
		}
	}
	return sta;
}








