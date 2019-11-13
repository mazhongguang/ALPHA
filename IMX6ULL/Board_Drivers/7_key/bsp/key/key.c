#include "key.h"
#include "bsp_delay.h"
/* 初始化按键 */
void key_init(void)
{
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18,0);	/* 复用为GPIO1_IO18 */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18,0XF080);

	/* GPIO初始化 */
	GPIO1->GDIR &= ~(1<<18);	/* 设置为输入 */
}

/* 读取按键值 */
int read_key(void)
{
	int ret = 0;
	ret = ((GPIO1->DR) >> 18) & 0X1;
	return ret;
}

/* 按键值获取函数 */
int key_getvalue(void)
{
	int ret = 0;
	static unsigned char release = 1;		/* 为1表示按键释放 */

	if((release == 1) && (read_key() == 0))		/* 按下 */
		{
			delay(10);
			release = 0;
			if(read_key() == 0)					/* 如果延时10ms后KEY0还是0,表示按键有效 */
				{
					ret = KEY0_VALUE;	
				}
		}
	else if(read_key() ==1)					/* 未按下 */
		{
			ret = KEY_NONE;
			release = 1;
		}
	return ret;
}
