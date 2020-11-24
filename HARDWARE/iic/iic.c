#include "iic.h"

void delay(void) {
	uint16_t t=10;
	while(t--);
}

void I2C_Init(void) {
	RCC->APB2ENR|=1<<3;
	GPIOB->CRH&=0xFFFFFF00;
	GPIOB->CRH|=0x00000033;
	
	SCL=SDA=1;
}

void I2C_Start(void) {
	SDA_OUT();
	SDA=1;
	SCL=1;
	delay();
	SDA=0;
	delay();
	SCL=0;
	delay();
}

void I2C_Stop(void) {
	SDA_OUT();
	SDA=0;
	SCL=1;
	delay();
	SDA=1;
	delay();
}

void I2C_Write_Byte(uint8_t byte) {
	uint16_t i;
	SDA_OUT();

	SCL=0;
	for(i=0;i<8;++i) {
		if(byte&(0x80>>i)) SDA=1;
		else               SDA=0;
		delay();
		SCL=1;
		delay();
		SCL=0;
		delay();
	}
}

uint8_t I2C_Read_Byte(void) {

	uint8_t byte;
	uint16_t i;
	for(i=0;i<8;++i) {
		SDA_OUT();
		SDA=1;
		SCL=1;
		delay();
		byte<<=1;
		SDA_IN();
		byte|=RSDA;
		SCL=0;
		delay();
	}
	return byte;
}

uint8_t I2C_Read_ACK(void) {
	uint8_t ack;
	SDA_OUT();
	
	SDA=1;
	SCL=1;
	delay();
	SDA_IN();
	ack=RSDA;
	SCL=0;
	delay();
	return ack;
}

void I2C_Write_ACK(uint8_t ack) {
	SDA_OUT();
	SCL=0;
	if(ack) SDA=1;
	else    SDA=0;
	SCL=1;
	delay();
	SCL=0;
	delay();
}
