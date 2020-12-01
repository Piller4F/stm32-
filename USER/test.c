#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "iic.h"
#include "mpu6050.h"
#include "inv_mpu.h"

void Delay(uint32_t t)
{
	while (t--);
}

void MPU6050_Display(void)
{
	/* ��ӡ x, y, z ����ٶ� */
	printf("ACCEL_X: %d\t", MPU6050_Get_Data(ACCEL_XOUT_H)+X_ACCEL_OFFSET);
	printf("ACCEL_Y: %d\t", MPU6050_Get_Data(ACCEL_YOUT_H)+Y_ACCEL_OFFSET);
	printf("ACCEL_Z: %d\t", MPU6050_Get_Data(ACCEL_ZOUT_H)+Z_ACCEL_OFFSET);

	/* ��ӡ�¶� */
	printf("TEMP: %0.2f\t", MPU6050_Get_Data(TEMP_OUT_H) / 340.0 + 36.53);
	
	/* ��ӡ x, y, z ����ٶ� */
	printf("GYRO_X: %d\t", MPU6050_Get_Data(GYRO_XOUT_H)+X_GYRO_OFFSET);
	printf("GYRO_Y: %d\t", MPU6050_Get_Data(GYRO_YOUT_H)+Y_GYRO_OFFSET);
	printf("GYRO_Z: %d\t", MPU6050_Get_Data(GYRO_ZOUT_H)+Z_GYRO_OFFSET);
	
	printf("\r\n");
}

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//�ȴ���һ�η������   
	USART1->DR=c;   	
}

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}

//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
}   

int main() {
	float pitch,roll,yaw;
	short aacx,aacy,aacz;
	short gyrox,gyroy,gyroz;
	short temp;
	
	Stm32_Clock_Init(9);
	delay_init(72);
	uart_init(72,9600);
	MPU6050_Init();
	while(mpu_dmp_init());
	while(1) {
		//printf("OK");
		while(mpu_dmp_get_data(&pitch,&roll,&yaw)==0) {
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			//printf("pitch:%f roll:%f yaw:%f\r\n",pitch,roll,yaw);
		}
		
	}
}




