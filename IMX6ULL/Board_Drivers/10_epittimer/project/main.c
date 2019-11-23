#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "int.h"
#include "epit.h"

int main(void)
{
	
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();/* 初始化按键 */
	exti_init();	/* initialization interrupt  */
	epit1_init(0,33000000);	/* initialization epit */
	while (1)
	{
	}
	return 0;
}
