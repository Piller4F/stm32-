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
	/* 打印 x, y, z 轴加速度 */
	printf("ACCEL_X: %d\t", MPU6050_Get_Data(ACCEL_XOUT_H)+X_ACCEL_OFFSET);
	printf("ACCEL_Y: %d\t", MPU6050_Get_Data(ACCEL_YOUT_H)+Y_ACCEL_OFFSET);
	printf("ACCEL_Z: %d\t", MPU6050_Get_Data(ACCEL_ZOUT_H)+Z_ACCEL_OFFSET);

	/* 打印温度 */
	printf("TEMP: %0.2f\t", MPU6050_Get_Data(TEMP_OUT_H) / 340.0 + 36.53);
	
	/* 打印 x, y, z 轴角速度 */
	printf("GYRO_X: %d\t", MPU6050_Get_Data(GYRO_XOUT_H)+X_GYRO_OFFSET);
	printf("GYRO_Y: %d\t", MPU6050_Get_Data(GYRO_YOUT_H)+Y_GYRO_OFFSET);
	printf("GYRO_Z: %d\t", MPU6050_Get_Data(GYRO_ZOUT_H)+Z_GYRO_OFFSET);
	
	printf("\r\n");
}

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//等待上一次发送完毕   
	USART1->DR=c;   	
}

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}

//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
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
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
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
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			//printf("pitch:%f roll:%f yaw:%f\r\n",pitch,roll,yaw);
		}
		
	}
}




