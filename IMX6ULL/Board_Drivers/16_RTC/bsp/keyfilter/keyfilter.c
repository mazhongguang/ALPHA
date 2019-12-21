#include "keyfilter.h"
#include "gpio.h"
#include "int.h"
#include "beep.h"

/* initialization keyfilter */
void keyfilter_init(void)
{
	gpio_pin_config_t key_config;
	IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);/* 复用为GPIO1_IO18 */
	IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);

	/* initialization GPIO */
	key_config.direction = kGPIO_DigitalInput;
	key_config.interruptMode = kGPIO_IntFallingEdge;
	gpio_init(GPIO1, 18, &key_config);

	GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
	system_register_irqhandler(GPIO1_Combined_16_31_IRQn,gpio1_16_31_irqhandler, NULL);
	gpio_enableint(GPIO1, 18);

	/* initialization timer */
	filtertimer_init(66000000/10); 	/* initialization timer cycle for 10ms */
}

/* initialization EPIT1 timer */
void filtertimer_init(unsigned int value)
{

	EPIT1->CR = 0;

	EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);
	EPIT1->LR = value;		/* load register */
	EPIT1->CMPR = 0;

	/* initialization interrupt */
	GIC_EnableIRQ(EPIT1_IRQn);
	system_register_irqhandler(EPIT1_IRQn,filtertimer_irqhandler, NULL);

}

/* close EPIT1 timer */
void filtertimer_stop(void)
{
	EPIT1->CR &= ~(1 << 0);
}

/* restart EPIT1 timer */
void filtertimer_restart(unsigned int value)
{
	EPIT1->CR &= ~(1 << 0);
	EPIT1->LR = value;
	EPIT1->CR |= 1 << 0;
}

/* the interrupt handler function for EPIT1 timer */
void filtertimer_irqhandler(unsigned int gicciar, void *param)
{
	static unsigned char state = OFF;

	if(EPIT1->SR & (1 << 0))
	{
		/* close timer */
		filtertimer_stop();
		if(gpio_pinread(GPIO1, 18) == 0)
		{
			state = !state;
			beep_switch(state);
		}
	}
	EPIT1->SR |= 1 << 0;	/* clear interrupt flag bit */
}
/* key interrupt service function */
void gpio1_16_31_irqhandler(unsigned int gicciar, void *param)
{
	/* open timer */
	filtertimer_restart(66000000/100);
	/* clear interrupt flag bit */
	gpio_clearintflags(GPIO1, 18);
}
