#include "mpu6050.h"
#include "usart.h"

void MPU6050_Init(void) {
	I2C_Init();
	
	MPU6050_Write_Reg(PWR_MGMT_1, 0x00);	//�������״̬
	MPU6050_Write_Reg(SMPLRT_DIV, 0x07);    //�����ǲ����ʣ�����ֵ��0x07(125Hz)
	MPU6050_Write_Reg(CONFIG, 0x06);        //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
	MPU6050_Write_Reg(GYRO_CONFIG, 0x18);   //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);  //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
}

void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData) { //�ϳ�16λ����
	I2C_Start();      //������ʼ�ź�
	//д�豸��ַ
	I2C_Write_Byte(DEV_ADDR);
	if(I2C_Read_ACK()) goto stop;
	//д�Ĵ�����ַ
	I2C_Write_Byte(regAddr);
	if (I2C_Read_ACK())goto stop;
	//д���ݵ��Ĵ���
	I2C_Write_Byte(regData);
	if (I2C_Read_ACK())goto stop;
stop:
	I2C_Stop();
}

uint8_t MPU6050_Read_Reg(uint8_t regAddr) {
	uint8_t regData;
	//������ʼ�ź�
	I2C_Start();
	//�����豸��ַ
	I2C_Write_Byte(DEV_ADDR);
	if (I2C_Read_ACK()) goto stop;
	//���ͼĴ�����ַ
	I2C_Write_Byte(regAddr);
	if (I2C_Read_ACK()) goto stop;
	//�ظ���ʼ�ź�
	I2C_Start();
	//���Ͷ�ģʽ
	I2C_Write_Byte(DEV_ADDR | 0x01);
	if (I2C_Read_ACK()) goto stop;
	//���Ĵ�������
	regData=I2C_Read_Byte();
	I2C_Write_ACK(1);   //дһ����Ӧ���ź�
stop:
	I2C_Stop();
	return regData;
}

int16_t MPU6050_Get_Data(uint8_t regAddr) {
	uint8_t Data_H,Data_L;
	uint16_t data;
	Data_H = MPU6050_Read_Reg(regAddr);
	Data_L = MPU6050_Read_Reg(regAddr + 1);
	data = (Data_H << 8) | Data_L;  // �ϳ�����
	return data;
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    I2C_Start(); 
	I2C_Write_Byte((addr<<1)|0);//����������ַ+д����	
	if(I2C_Read_ACK())	//�ȴ�Ӧ��
	{
		I2C_Stop();		 
		return 1;		
	}
    I2C_Write_Byte(reg);	//д�Ĵ�����ַ
    I2C_Read_ACK();		//�ȴ�Ӧ��
	for(i=0;i<len;i++)
	{
		I2C_Write_Byte(buf[i]);	//��������
		if(I2C_Read_ACK())		//�ȴ�ACK
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
	I2C_Write_Byte((addr<<1)|0);//����������ַ+д����	
	if(I2C_Read_ACK())	//�ȴ�Ӧ��
	{
		I2C_Stop();		 
		return 1;		
	}
    I2C_Write_Byte(reg);	//д�Ĵ�����ַ
    I2C_Read_ACK();		//�ȴ�Ӧ��
    I2C_Start();
	I2C_Write_Byte((addr<<1)|1);//����������ַ+������	
    I2C_Read_ACK();		//�ȴ�Ӧ�� 
	while(len)
	{
		if(len==1){
			*buf=I2C_Read_Byte();//������,����nACK 
			I2C_Write_ACK(1);
		}
		else {
			*buf=I2C_Read_Byte();		//������,����ACK  
			I2C_Write_ACK(0);
		}
		len--;
		buf++; 
	}    
    I2C_Stop();	//����һ��ֹͣ���� 
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
