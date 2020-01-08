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
#include "rtc.h"
#include "ap3216c.h"
#include "i2c.h"

int main(void)
{
	static unsigned char state = 0;
	unsigned short ir, ps, als;
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	delay_init();
	uart_init();/*初始化串口*/
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();
	lcd_init();/*	初始化LCD*/
	rtc_init();
	ap3216c_init();

	while(1)
	{
		ap3216c_readdata(&ir, &ps, &als);
		printf("ir = %d,ps = %d,als = %d\r\n", ir, ps, als);
		state = !state;
		led_switch(LED0, state);
		delay_ms(500);
	}
	return 0;
}
