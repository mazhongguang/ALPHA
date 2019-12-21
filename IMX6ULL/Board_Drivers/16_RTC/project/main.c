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

int main(void)
{
	static unsigned char state = 0;
	struct rtc_datetime rtcdate;
	unsigned char key = 0;
	int i = 3, t = 0;
	char buf[160];
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

	memset(buf, 0, sizeof(buf));
	while(1)
	{
		if(t == 100)
		{
			t = 0;
			lcd_fill(100, 100, 420, 120, tftlcd_dev.backcolor);
			sprintf(buf, "will be running %ds ......",i);
			lcd_show_string(100, 100, 350, 16, 16, buf);
			i--;
			if(i < 0)
				break;
		}
		
		key = key_getvalue();
		if(key == KEY0_VALUE)
		{
			rtcdate.year = 2019;
			rtcdate.month = 12;
			rtcdate.day = 20;
			rtcdate.hour = 22;
			rtcdate.minute = 39;
			rtcdate.second = 0;
			
			rtc_setdatetime(&rtcdate);
			break;
		}
		delay_ms(10);
		t++;
	}
	tftlcd_dev.forecolor = LCD_RED;
	tftlcd_dev.backcolor = LCD_WHITE;
	lcd_fill(100, 100, 420, 120, tftlcd_dev.backcolor);
	lcd_show_string(100, 100 , 200, 16, 16, (char *)"Current Time:");
	/*tftlcd_dev.forecolor = LCD_BLACK;*/

	while (1)
	{
	rtc_getdatetime(&rtcdate);		
	sprintf(buf, "%4d-%2d-%2d   %2d:%2d:%2d",rtcdate.year,rtcdate.month,rtcdate.day,rtcdate.hour,rtcdate.minute,rtcdate.second);
	tftlcd_dev.backcolor = LCD_WHITE;
	lcd_fill(100, 130, 350, 150, tftlcd_dev.backcolor);
	lcd_show_string(100, 130, 336, 32, 32, (char *)buf);
   /* lcd_shownum(100, 50, rtcdate.year, 4, 32);*/
	/*lcd_shownum(184, 50, rtcdate.month, 2, 32);*/
	/*lcd_shownum(236, 50, rtcdate.day, 2, 32);*/
	/*lcd_shownum(100, 100, rtcdate.hour, 4, 32);*/
	/*lcd_shownum(184, 100, rtcdate.month, 2, 32);*/
	/*lcd_shownum(236, 100, rtcdate.second, 2, 32);	*/
	/*display_img(400,200);*/
		state = !state;
		led_switch(LED0, state);
		delay_ms(1000);
	}
	return 0;
}
