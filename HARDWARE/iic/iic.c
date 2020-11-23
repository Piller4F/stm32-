#include "iic.h"
#include "delay.h"

void IIC_Init(void) {
	RCC->APB2ENR|=1<<3;     //ʹ��GPIOBʱ��
	GPIOB->CRL&=0x00FFFFFF; //PB6,7�������50MHz
	GPIOB->CRL|=0x33000000;
	GPIOB->ODR|=3<<6;       //PB6,7�����
}

//������ʼ�ź�
void IIC_Start(void) {
	SDA_OUT();
	IIC_SCL=1;
	IIC_SDA=1;
	delay_us(4);     //SCLΪ��ʱ��SDA�ɸ߱��
	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=0;
}

//����IICֹͣ�ź�
void IIC_Stop(void) {
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=1;
	delay_us(4);
	IIC_SDA=1;       //����IIC���߽����ź�
}

//Ӧ���ź�
//1 ����ʧ��
//0 ���ܳɹ�
u8 IIC_Wait_Ack(void) {
	u8 ucErrTime=0;
	SDA_IN();        //SDA����Ϊ����
	IIC_SDA=1;delay_us(1);
	IIC_SCL=1;delay_us(1);
	while(READ_SDA) {
		ucErrTime++;
		if(ucErrTime>250) {
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;
	return 0;
}

//����ACKӦ��
void IIC_Ack(void) {
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
	
//������ACKӦ��
void IIC_NAck(void) {
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1 ��Ӧ��
//0 ��Ӧ��
void IIC_Send_Byte(u8 txd) {
	u8 t;
	SDA_OUT();
	IIC_SCL=0;
	for(t=0;t<8;++t) {
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1;
		delay_us(2);
		
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
	}
}

//��һ���ֽ�,ack=1ʱ����ack,ack=0ʱ������nack
u8 IIC_Read_Byte(unsigned char ack) {
	unsigned char i,receive=0;
	SDA_IN();
	for(i=0;i<8;++i) {
		IIC_SCL=0;
		delay_us(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA) receive++;
		delay_us(1); 
	}
	if(!ack) IIC_NAck();
	else IIC_Ack();
	return receive;
}



