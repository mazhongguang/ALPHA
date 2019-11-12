#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "beep.h"

int main(void)
{
	clk_enable(); /* 使能外设时钟 */

	// 初始化LED
	led_init(); /*初始化LED*/
	Beep_Init();
	while (1)
	{
		led_on();
		beep_switch(ON);
		delay(2000);

		led_off();
		beep_switch(OFF);
		delay(2000);
	}
	return 0;
}
