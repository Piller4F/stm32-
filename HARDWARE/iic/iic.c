#include "iic.h"
#include "delay.h"

void IIC_Init(void) {
	RCC->APB2ENR|=1<<3;     //使能GPIOB时钟
	GPIOB->CRL&=0x00FFFFFF; //PB6,7推挽输出50MHz
	GPIOB->CRL|=0x33000000;
	GPIOB->ODR|=3<<6;       //PB6,7输出高
}

//产生起始信号
void IIC_Start(void) {
	SDA_OUT();
	IIC_SCL=1;
	IIC_SDA=1;
	delay_us(4);     //SCL为高时，SDA由高变低
	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=0;
}

//产生IIC停止信号
void IIC_Stop(void) {
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(4);
	IIC_SCL=1;
	delay_us(4);
	IIC_SDA=1;       //发送IIC总线结束信号
}

//应答信号
//1 接受失败
//0 接受成功
u8 IIC_Wait_Ack(void) {
	u8 ucErrTime=0;
	SDA_IN();        //SDA设置为输入
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

//产生ACK应答
void IIC_Ack(void) {
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
	
//不产生ACK应答
void IIC_NAck(void) {
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

//IIC发送一个字节
//返回从机有无应答
//1 有应答
//0 无应答
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

//读一个字节,ack=1时发送ack,ack=0时，发送nack
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



