#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "int.h"
#include "keyfilter.h"

int main(void)
{
	unsigned char state = OFF;
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	delay_init();
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();
	while (1)
	{
		state = !state;
		led_switch(LED0, state);
		delay_ms(1000);
	}
	return 0;
}
