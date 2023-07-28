这个是基于正点原子的例程，正点原子的模块为atk6050，使用的芯片也是mpu6050，故可以直接使用到mpu6050模块上
该工程的mpu6050配置，关闭中断，关闭fifo，具体配置查看代码的mpu6050_init()函数定义

该工程使用到1个iic软件接口，在atk_ms6050_iic.h中配置（注意引脚和port和时钟使能要改对）
1个串口，输出传感器的信息

移植时要注意的点
1，delay_init(x)的初始化，单位为hz，在my_mpu6050_init中修改
2，iic软件引脚的配置，注意不要与其他的引脚冲突
3，不能在主函数加delay函数，会导致mpu6050的fifo数据区溢出
4，串口printf配置
//usart.c函数最后添加这个，可以使用printf函数
#include "stdio.h"

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,1000);
	return ch;
}


5
在初始化代码区域（while（1）前）里添加

  my_mpu6050_init ();
在主函数的while（1）里添加

/* 获取ATK-MS6050 DMP处理后的数据 */
        ret  = atk_ms6050_dmp_get_data(&pit, &rol, &yaw);
        /* 获取ATK-MS6050加速度值 */
        ret += atk_ms6050_get_accelerometer(&acc_x, &acc_y, &acc_z);
        /* 获取ATK-MS6050陀螺仪值 */
        ret += atk_ms6050_get_gyroscope(&gyr_x, &gyr_y, &gyr_z);
        /* 获取ATK-MS6050温度值 */
        ret += atk_ms6050_get_temperature(&temp);
        if (ret == 0)
        {
            if (1)
            {
                /* 上传相关数据信息至串口调试助手 */
                printf("pit: %.2f, rol: %.2f, yaw: %.2f, ", pit, rol, yaw);
                printf("acc_x: %d, acc_y: %d, acc_z: %d, ", acc_x, acc_y, acc_z);
                printf("gyr_x: %d, gyr_y: %d, gyr_z: %d, ", gyr_x, gyr_y, gyr_z);
                printf("temp: %d\r\n", temp);
							
            }
           
 
        }