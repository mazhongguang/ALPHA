#include	"main.h"
/* 使能外设时钟 */
void clk_enable()
{
	CCM_CCGR1 = 0XFFFFFFFF;
	CCM_CCGR2 = 0XFFFFFFFF;
	CCM_CCGR3 = 0XFFFFFFFF;
	CCM_CCGR4 = 0XFFFFFFFF;
	CCM_CCGR5 = 0XFFFFFFFF;
	CCM_CCGR6 = 0XFFFFFFFF;
}

/* 初始化LED */
void led_init(void)
{
	SW_MUX_GPIO1_IO03 = 0X5;		/* 复用为GPIO1_IO03 */
	SW_PAD_GPIO1_IO03 = 0X10B0;		/* 设置GPIO1_IO03电气属性 */

	// GPIO初始化
	GPIO1_GDIR = 0x8;					/* 设置为输出 */
	GPIO1_DR = 0X0;						/* 打开LED灯 */
}

/* 短延时 */
void delay_short(volatile unsigned int n)
{
	while (n--);	
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
	GPIO1_DR &= ~(1<<3);	/* bit3清零 */
}

/* 关闭LED灯 */
void led_off(void)
{
	GPIO1_DR |= (1<<3);
}
int main(void)
{
	clk_enable();						/* 使能外设时钟 */

	// 初始化LED
	led_init();							/*初始化LED*/
	
	while (1)
	{
		led_on();
		delay(500);

		led_off();
		delay(500);
	}
	return 0;
}
