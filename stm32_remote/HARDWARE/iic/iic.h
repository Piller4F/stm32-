#ifndef __I2C_H__
#define __I2C_H__

#include "sys.h"
#define SCL  PBout(8)
#define SDA  PBout(9)
#define RSDA PBin(9) 


#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}

void I2C_Init(void);                //I2C��ʼ��
void I2C_Start(void);               //I2CЭ����ʼ�ź�
void I2C_Stop(void);                //I2CЭ����ֹ�ź�
void I2C_Write_Byte(uint8_t byte);  //����8λ����
uint8_t I2C_Read_Byte(void);        //��ȡ8λ����
uint8_t I2C_Read_ACK(void);         //����Ӧ���ź�
void I2C_Write_ACK(uint8_t ack);    //����Ӧ���ź�

#endif



