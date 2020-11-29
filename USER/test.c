#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "iic.h"
#include "mpu6050.h"

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

int main() {
	Stm32_Clock_Init(9);
	delay_init(72);
	uart_init(72,9600);
	MPU6050_Init();
	while(1) {
		MPU6050_Display();
		Delay(0xFFFFF);
	}
}




