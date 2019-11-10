#include "fsl_iomuxc.h"
/* 使能外设时钟 */
void clk_enable()
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
}

/* 初始化LED */
void led_init(void)
{
	/* 复用为GPIO1_IO03 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);

	 /* 设置GPIO1_IO03电气属性 */
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);
	// GPIO初始化
	GPIO1->GDIR = 0x8; /* 设置为输出 */
	GPIO1->DR = 0X0;   /* 打开LED灯 */
}

/* 短延时 */
void delay_short(volatile unsigned int n)
{
	while (n--)
		;
}

/* 延时，一次循环大概是1ms ,在主频397MHz*/
void delay(volatile unsigned int n)
{
	while (n--)
	{
		delay_short(0x7ff);
	}
}

/* 打开LED灯 */
void led_on(void)
{
	GPIO1->DR &= ~(1 << 3); /* bit3清零 */
}

/* 关闭LED灯 */
void led_off(void)
{
	GPIO1->DR |= (1 << 3);
}
int main(void)
{
	clk_enable(); /* 使能外设时钟 */

	// 初始化LED
	led_init(); /*初始化LED*/

	while (1)
	{
		led_on();
		delay(2000);

		led_off();
		delay(2000);
	}
	return 0;
}
