#include "beep.h"
#include "cc.h"
#include "gpio.h"
/* BEEP初始化 */
void Beep_Init(void)
{
	gpio_pin_config_t beep_config;
	IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);	//复用为GIPO5_IO01
	IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0X10B0);

	/* GPIO初始化 */
	beep_config.direction = kGPIO_DigitalOutput;
	beep_config.outputLogic = 1;
	gpio_init(GPIO5, 1, &beep_config);
}

/* 蜂鸣器控制函数 */
void beep_switch(int status)
{
	if(status == ON)
		gpio_pinwrite(GPIO5, 1, 0);
	else if (status == OFF)
		gpio_pinwrite(GPIO5, 1, 1);
}
