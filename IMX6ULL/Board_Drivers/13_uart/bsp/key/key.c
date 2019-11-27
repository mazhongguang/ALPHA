#include "key.h"
#include "bsp_delay.h"
#include "gpio.h"
/* 初始化按键 */
void key_init(void)
{
	gpio_pin_config_t key_config;
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);	/* 复用为GPIO1_IO18 */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0XF080);

	/* GPIO初始化 */
	gpio_init(GPIO1, 18, &key_config);
	GPIO1->GDIR &= ~(1<<18);	/* 设置为输入 */
}

/* 读取按键值 */
int read_key(void)
{
	int ret = 0;
	ret = gpio_pinread(GPIO1, 18);
	return ret;
}

/* 按键值获取函数 */
int key_getvalue(void)
{
	int ret = 0;
	static unsigned char release = 1;		/* 为1表示按键释放 */

	if((release == 1) && gpio_pinread(GPIO1, 18) == 0)		/* 按下 */
		{
			delay(10);
			release = 0;
			if(gpio_pinread(GPIO1, 18) == 0)					/* 如果延时10ms后KEY0还是0,表示按键有效 */
				{
					ret = KEY0_VALUE;	
				}
		}
	else if(gpio_pinread(GPIO1, 18) ==1)					/* 未按下 */
		{
			ret = KEY_NONE;
			release = 1;
		}
	return ret;
}
