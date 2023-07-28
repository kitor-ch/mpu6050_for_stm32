#include "delay.h"

static uint16_t  g_fac_us = 0;      /* us延时倍乘数 */
/**
 * @brief       初始化延迟函数
 * @param       sysclk: 系统时钟频率, 即CPU频率(HCLK)
 * @retval      无
 */
void delay_init(uint16_t sysclk)
{

    SysTick->CTRL = 0;                                          /* 清Systick状态，以便下一步重设，如果这里开了中断会关闭其中断 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);   /* SYSTICK使用内核时钟源8分频,因systick的计数器最大值只有2^24 */
    g_fac_us = sysclk / 8;                                      /* 不论是否使用OS,g_fac_us都需要使用,作为1us的基础时基 */

}
/**
 * @brief       延时nus
 * @param       nus: 要延时的us数.
 * @note        注意: nus的值,不要大于1864135us(最大值即2^24 / g_fac_us @g_fac_us = 9)
 * @retval      无
 */
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * g_fac_us; /* 时间加载 */
    SysTick->VAL = 0x00;            /* 清空计数器 */
    SysTick->CTRL |= 1 << 0 ;       /* 开始倒数 */

    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); /* CTRL.ENABLE位必须为1, 并等待时间到达 */

    SysTick->CTRL &= ~(1 << 0) ;    /* 关闭SYSTICK */
    SysTick->VAL = 0X00;            /* 清空计数器 */
}

/**
 * @brief       延时nms
 * @param       nms: 要延时的ms数 (0< nms <= 65535)
 * @retval      无
 */
void delay_ms(uint16_t nms)
{
    uint32_t repeat = nms / 1000;   /*  这里用1000,是考虑到可能有超频应用,
                                     *  比如128Mhz的时候, delay_us最大只能延时1048576us左右了
                                     */
    uint32_t remain = nms % 1000;

    while (repeat)
    {
        delay_us(1000 * 1000);      /* 利用delay_us 实现 1000ms 延时 */
        repeat--;
    }

    if (remain)
    {
        delay_us(remain * 1000);    /* 利用delay_us, 把尾数延时(remain ms)给做了 */
    }
}
