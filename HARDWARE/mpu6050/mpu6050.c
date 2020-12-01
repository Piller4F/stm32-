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

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    I2C_Start(); 
	I2C_Write_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(I2C_Read_ACK())	//等待应答
	{
		I2C_Stop();		 
		return 1;		
	}
    I2C_Write_Byte(reg);	//写寄存器地址
    I2C_Read_ACK();		//等待应答
	for(i=0;i<len;i++)
	{
		I2C_Write_Byte(buf[i]);	//发送数据
		if(I2C_Read_ACK())		//等待ACK
		{
			I2C_Stop();	 
			return 1;		 
		}		
	}    
    I2C_Stop();	 
	return 0;	
}

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	I2C_Start(); 
	I2C_Write_Byte((addr<<1)|0);//发送器件地址+写命令	
	if(I2C_Read_ACK())	//等待应答
	{
		I2C_Stop();		 
		return 1;		
	}
    I2C_Write_Byte(reg);	//写寄存器地址
    I2C_Read_ACK();		//等待应答
    I2C_Start();
	I2C_Write_Byte((addr<<1)|1);//发送器件地址+读命令	
    I2C_Read_ACK();		//等待应答 
	while(len)
	{
		if(len==1){
			*buf=I2C_Read_Byte();//读数据,发送nACK 
			I2C_Write_ACK(1);
		}
		else {
			*buf=I2C_Read_Byte();		//读数据,发送ACK  
			I2C_Write_ACK(0);
		}
		len--;
		buf++; 
	}    
    I2C_Stop();	//产生一个停止条件 
	return 0;	
}

u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(DEV_ADDR,GYRO_XOUT_H,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(DEV_ADDR,ACCEL_XOUT_H,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
