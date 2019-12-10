#include "main.h"

#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "int.h"
#include "keyfilter.h"
#include "uart.h"
#include "stdio.h"
#include "lcd.h"
#include "lcdapi.h"

int main(void)
{
	static unsigned char state = 0;
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	delay_init();
	uart_init();/*初始化串口*/
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();
	lcd_init();/*	初始化LCD*/

	lcd_show_string(10, 10, 272, 32, 32,(char *)"MYLOVE IS QINGER!");
	lcd_show_string(10, 60, 272, 32, 32,(char *)"MYLOVE IS QINGER!");
	lcd_show_string(10, 110, 272, 32, 32,(char *)"MYLOVE IS QINGER!");
	lcd_show_string(10, 160, 272, 32, 32,(char *)"MYLOVE IS QINGER!");
	while (1)
	{
		state = !state;
		led_switch(LED0, state);
		delay_ms(1000);
	}
	return 0;
}
