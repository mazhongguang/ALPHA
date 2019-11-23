#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "int.h"

int main(void)
{
	unsigned char led_state = OFF;
	
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();/* 初始化按键 */
	exti_init();	/* initialization interrupt  */
	while (1)
	{
		led_state = !led_state;
		led_switch(LED0, led_state);
		delay(500);
	}
	return 0;
}
