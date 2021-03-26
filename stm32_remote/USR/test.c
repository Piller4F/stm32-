#include "sys.h"
#include "usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "delay.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "usart2.h"
#include "string.h"

void USART2_Send_Byte(u8 data) {
	USART2->DR=data;
	while((USART2->SR&(1<<6))==0);
}

int main() {
	float pitch,roll,yaw;         //欧拉角
	int i;
	char USART_TX_BUF[200];       //数据接受数组
	Stm32_Clock_Init(9);          //时钟初始化
	delay_init(72);
	MPU6050_Init();
	uart_init(72,38400);
	USART2_Init(36,38400);
	LED_Init();
	while(mpu_dmp_init());//直到为0初始化成功
	LED=0;
	while(1){
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)== 0) {//因为正确的情况只有一种，所以0是正确接收
			printf("pitch:%f roll=%f yaw=%f\r\n",pitch,roll,yaw);
			sprintf(USART_TX_BUF,"%f,%f,%f",pitch,roll,yaw);
			for(i=0;i<strlen(USART_TX_BUF);i++){
					USART2_Send_Byte(USART_TX_BUF[i]);
			}
			delay_ms(70);
		}	
	}
}
