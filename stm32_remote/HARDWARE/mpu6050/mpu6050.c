#include "mpu6050.h"
#include "iic.h"

void MPU6050_Init() {
	I2C_Init();  // I2C 初始化     

    MPU6050_Write_Reg(PWR_MGMT_1, 0x00);    //解除休眠状态     
    MPU6050_Write_Reg(SMPLRT_DIV, 0x07);    //陀螺仪采样率，典型值：0x07(125Hz)     
    MPU6050_Write_Reg(CONFIG, 0x06);        //低通滤波频率，典型值：0x06(5Hz)     
    MPU6050_Write_Reg(GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)     
    MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz) 
}

void MPU6050_Write_Reg(uint8_t add,uint8_t data) {
	I2C_Start();//发送一个开始信号
	I2C_Write_Byte(DEV_ADDR|0);
	if(I2C_Read_ACK()){
		I2C_Stop();
		return ;
	}
	I2C_Write_Byte(add);
	if(I2C_Read_ACK()){
		I2C_Stop();
		return ;
	}
	I2C_Write_Byte(data);
	I2C_Stop();
}


uint8_t MPU6050_Read_Reg(uint8_t add) {
	uint8_t data;
	
	I2C_Start();//发送一个开始信号
	I2C_Write_Byte(DEV_ADDR|0);//芯片
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 0;
	}
	I2C_Write_Byte(add);//寄存器
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 0;
	}
	
	I2C_Start();//发送一个开始信号
	I2C_Write_Byte(DEV_ADDR|1);//芯片
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 0;
	}
	
	data=I2C_Read_Byte();//寄存器
	I2C_Write_ACK(1);//写一个不应答
	I2C_Stop();
	return data;
}


int16_t MPU6050_Get_Data(uint8_t add){
	int16_t data;
	uint8_t add_l,add_h;
	add_h=MPU6050_Read_Reg(add);
	add_l=MPU6050_Read_Reg(add+1);
	
	data=(add_h<<8)|add_l;
	return data;
}

u8 MPU_Write_Len(unsigned char slave_addr, unsigned char reg_addr,
      unsigned char length, unsigned char const *data) {
	int i;
	I2C_Start();//发送一个开始信号
	I2C_Write_Byte((slave_addr<<1)|0);
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 1;
	}
	I2C_Write_Byte(reg_addr);//寄存器
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 1;
	}
	for(i=0;i<length;i++) {
		I2C_Write_Byte(data[i]);
		if(I2C_Read_ACK()){
			I2C_Stop();
			return 1;
		}
	}
	I2C_Stop();
	return 0;
}
	  
u8 MPU_Read_Len(unsigned char slave_addr, unsigned char reg_addr,
      unsigned char length, unsigned char *data) {
	int i;
	I2C_Start();//发送一个开始信号
	I2C_Write_Byte((slave_addr<<1)|0);
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 1;
	}
	I2C_Write_Byte(reg_addr);//寄存器
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 1;
	}
	
	I2C_Start();//发送一个开始信号
	I2C_Write_Byte((slave_addr<<1)|1);//芯片
	if(I2C_Read_ACK()){
		I2C_Stop();
		return 1;
	}
	
	for(i=0;i<length;i++) {
		data[i]=I2C_Read_Byte();
		if(i<length-1) I2C_Write_ACK(0);
		else I2C_Write_ACK(1);	
	}
	I2C_Stop();
	return 0;
	  
}

void LED_Init(void){ 
	RCC->APB2ENR|=1<<4;//C
	GPIOC->CRH &= 0xff0fffff;
	GPIOC->CRH |= 0x00300000;
	
	GPIOC->ODR |= 1<<13;
}

