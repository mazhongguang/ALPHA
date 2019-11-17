#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "beep.h"
#include "key.h"

int main(void)
{
	int i = 0;
	unsigned char led_state = OFF;
	unsigned char beep_state = OFF;
	int keyvalue = 0;	

	imx6u_clkinit();
	// clk_enable(); /* 使能外设时钟 */
	// 初始化LED
	led_init(); /*初始化LED*/
	Beep_Init();/* 初始化蜂鸣器 */
	key_init();/* 初始化按键 */
	while (1)
	{
		/* 按键控制蜂鸣器 */
		keyvalue = key_getvalue();
		if(keyvalue)		/* 如果为正，表示为有效值 */
		{
			switch (keyvalue)
			{
				case KEY0_VALUE:
					beep_state = !beep_state;
					beep_switch(beep_state);
					break;
			}
		}
		i++;
		if(i == 200)		/* 50*10=500ms */
		{
			i = 0;
			led_state = !led_state;
			led_switch(LED0, led_state);
		}
		delay(10);
	}
	return 0;
}
