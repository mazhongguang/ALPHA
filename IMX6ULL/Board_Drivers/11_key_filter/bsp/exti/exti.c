#include "exti.h"
#include "gpio.h"
#include "int.h"
#include "bsp_delay.h"
#include "beep.h"
/* 初始化外部中断，也就是GPIO1_IO18 */
void exti_init(void)
{	
	gpio_pin_config_t key_config;
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);	/* 复用为GPIO1_IO18 */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

	/* GPIO初始化 */
	key_config.direction = kGPIO_DigitalInput;
	key_config.interruptMode = kGPIO_IntFallingEdge;
	gpio_init(GPIO1, 18, &key_config);

	GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
	system_register_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_io18_irqhandler, NULL);

	gpio_enableint(GPIO1, 18);
}
/* interrupt handling function */
void gpio1_io18_irqhandler(unsigned int gicciar, void *param)
{
	static unsigned char state = 0;

	delay(10);					/* 在实际开发中，禁止在中断服务函数中调用延时函数 */
	if(gpio_pinread(GPIO1, 18) == 0)
	{
		state = !state;
		beep_switch(state);
	}
	
	/* clear interrupt flag bit */
	gpio_clearintflags(GPIO1, 18);
}
