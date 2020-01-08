#include "epit.h"
#include "int.h"
#include "bsp_led.h"
/* initialization EPIT1
 * @frac:
 * @value: autoload value
 */
void epit1_init(unsigned int frac, unsigned int value)
{
	if(frac > 4095)
		frac = 4095;
	/* config EPTI1_CR register */
	EPIT1->CR = 0;

	EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);
	EPIT1->LR = value;		/* load register */
	EPIT1->CMPR = 0;

	/* initialization interrupt */
	GIC_EnableIRQ(EPIT1_IRQn);
	system_register_irqhandler(EPIT1_IRQn,epit1_irqhandler, NULL);

	/* enable EPIT1 */
	EPIT1->CR |= (1 << 0);
}

/* EPIT1 interrupt service function */
void epit1_irqhandler(unsigned int gicciar, void *param)
{
	static unsigned char state = 0;

	state = !state;
	if(EPIT1->SR & (1 << 0))	/* The interruption happened */
	{
		led_switch(LED0, state);
	}

	/* clear interrupt flag bit */
	EPIT1->SR |= (1 << 0);
}
