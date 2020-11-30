#include "mpu6050.h"
#include "usart.h"

void MPU6050_Init(void) {
	I2C_Init();
	
	MPU6050_Write_Reg(PWR_MGMT_1, 0x00);	//解除休眠状态
	MPU6050_Write_Reg(SMPLRT_DIV, 0x07);    //陀螺仪采样率，典型值：0x07(125Hz)
	MPU6050_Write_Reg(CONFIG, 0x06);        //低通滤波频率，典型值：0x06(5Hz)
	MPU6050_Write_Reg(GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
}

void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData) { //合成16位数据
	I2C_Start();      //发送起始信号
	//写设备地址
	I2C_Write_Byte(DEV_ADDR);
	if(I2C_Read_ACK()) goto stop;
	//写寄存器地址
	I2C_Write_Byte(regAddr);
	if (I2C_Read_ACK())goto stop;
	//写数据到寄存器
	I2C_Write_Byte(regData);
	if (I2C_Read_ACK())goto stop;
stop:
	I2C_Stop();
}

uint8_t MPU6050_Read_Reg(uint8_t regAddr) {
	uint8_t regData;
	//发送起始信号
	I2C_Start();
	//发送设备地址
	I2C_Write_Byte(DEV_ADDR);
	if (I2C_Read_ACK()) goto stop;
	//发送寄存器地址
	I2C_Write_Byte(regAddr);
	if (I2C_Read_ACK()) goto stop;
	//重复起始信号
	I2C_Start();
	//发送读模式
	I2C_Write_Byte(DEV_ADDR | 0x01);
	if (I2C_Read_ACK()) goto stop;
	//读寄存器数据
	regData=I2C_Read_Byte();
	I2C_Write_ACK(1);   //写一个非应答信号
stop:
	I2C_Stop();
	return regData;
}

int16_t MPU6050_Get_Data(uint8_t regAddr) {
	uint8_t Data_H,Data_L;
	uint16_t data;
	Data_H = MPU6050_Read_Reg(regAddr);
	Data_L = MPU6050_Read_Reg(regAddr + 1);
	data = (Data_H << 8) | Data_L;  // 合成数据
	return data;
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf) {
	u8 i;
	I2C_Start();                 //起始信号
	I2C_Write_Byte(DEV_ADDR);    //写
	if (I2C_Read_ACK()) return 1;//等待应答
	I2C_Write_Byte(addr);        //发送寄存器地址
	if (I2C_Read_ACK()) return 1;//等待应答
	for(i=0;i<len;++i) {
		I2C_Write_Byte(buf[i]);
		if(I2C_Read_ACK()) {
			I2C_Stop();
			return 1;
		}
	}
	I2C_Stop();
	return 0;
}

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf) {
	u8 i;
	I2C_Start();
	I2C_Write_Byte(DEV_ADDR|0);  //写
	if (I2C_Read_ACK()) return 1;//等待应答
	I2C_Write_Byte(addr);        //发送寄存器地址
	if (I2C_Read_ACK()) return 1;//等待应答
	I2C_Start();
	I2C_Write_Byte(DEV_ADDR|1);  //读
	if (I2C_Read_ACK()) return 1;//等待应答
	for(i=0;i<len;++i) {
		buf[i]=I2C_Read_Byte();
		if(i==len-1) I2C_Write_ACK(1);
		else         I2C_Write_ACK(0);
	}
	I2C_Stop();
	return 0;
}
