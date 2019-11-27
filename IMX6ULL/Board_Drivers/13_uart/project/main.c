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

int main(void)
{
	unsigned char a = 0; 
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	delay_init();
	uart_init();/*初始化串口*/
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();
	while (1)
	{
		puts("请输入 1 个字符：");
		a = getc();
		putc(a);	
		puts("\r\n");
		puts("\r\n");
		puts("你刚才输入的字符是：");
		putc(a);
		puts("\r\n");
		puts("\r\n");
		puts("\r\n");


	}
	return 0;
}
