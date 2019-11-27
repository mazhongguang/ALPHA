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

int main(void)
{
	unsigned int a,b;
	unsigned int c = 100;
	int_init();		/* initialization interrupt */
	imx6u_clkinit();	/* initialization system clock */
	clk_enable(); /* 使能外设时钟 */
	delay_init();
	uart_init();/*初始化串口*/
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();

	printf("%d的十六进制为：%#x\r\n", c, c);
	while (1)
	{
		printf("请输入两个整数，使用空格隔开：");
		scanf("%d %d", &a, &b);
		printf("\r\n 数据%d+%d=%d\r\n", a, b, a+b);
	}
	return 0;
}
