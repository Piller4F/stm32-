#ifndef __I2C_H__
#define __I2C_H__

#include "sys.h"
#define SCL  PBout(8)
#define SDA  PBout(9)
#define RSDA PBin(9) 


#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

void I2C_Init(void);                //I2C初始化
void I2C_Start(void);               //I2C协议起始信号
void I2C_Stop(void);                //I2C协议终止信号
void I2C_Write_Byte(uint8_t byte);  //发送8位数据
uint8_t I2C_Read_Byte(void);        //读取8位数据
uint8_t I2C_Read_ACK(void);         //接受应答信号
void I2C_Write_ACK(uint8_t ack);    //发送应答信号

#endif



