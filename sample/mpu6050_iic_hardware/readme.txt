这个工程是使用invenSense官方驱动库，移植到stm32f103c8t6
ps：因为是用官方的库，功能比较齐全，所对应的内存占有就比较大
Program Size: Code=35476 RO-data=3600 RW-data=104 ZI-data=4352  
超过了32kb，c6t6不能驱动
而正点原子的比较简略，大概19kb左右，c6t6也能跑

mpu设置为开启fifo（先入先出模式）使能中断（当准备好数据，fifo中有可用数据时 int引脚电平变化，具体变化看配置）

在cube配置，
1，串口
2，iic硬件 设置为400khz（代码里默认是iic1）
3，一个外部中断，设置为上升沿触发，使能中断，中断处理函数在mpu6050的280行左右
在keil
1，将mpu6050文件夹中的lib内核文件替换成为移植mcu的
//在官方DMP6.12中的...\motion_driver_6.12\mpl libraries\arm\Keil目录内解压属于你内核的lib。我这个用的是M3内核。如果你用的是M4内核，则解压M4，并将解压后的libmplib.lib替换我这个库中的
2//宏定义中添加
,MPL_LOG_NDEBUG=1,MPU6050,EMPL,USE_DMP,EMPL_TARGET_STM32F4
（f4的原因是因为从f4移植到f3系列，对mpu6050库文件生效，对mcu的设置无效
在inv_mpu.c里有个励例子
#if defined EMPL_TARGET_STM32F4
#include "i2c.h"   
#include "main.h"
#include "log.h"

）
3//usart.c函数最后添加这个，可以使用printf函数
#include "stdio.h"

int fputc(int ch,FILE *f) 
{ 
    HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,1000); return ch; 
} 
4//main.c中添加
#include "mpu6050.h"

5//MAIN函数中while函数前添加
mpu6050_init();

6//主函数WHILE循环中添加下面代码
float Pitch,Roll,Yaw;
long Temp; 
MPU6050_Get_Euler_Temputer(&Pitch,&Roll,&Yaw,&Temp);
printf("Pitch : %.4f ",(float)Pitch ); 
printf("Roll : %.4f ",(float)Roll ); 
printf("Yaw : %.4f \r\n",(float)Yaw ); 


参照b站大神  

https://www.bilibili.com/video/BV17h411p7s3/?spm_id_from=333.1007.top_right_bar_window_custom_collection.content.click&vd_source=72ea95c8491bdc7b7c63e3aa829e1a8c

github仓库：https://github.com/nxcosa/STM32-HAL-MPU6050-DMP