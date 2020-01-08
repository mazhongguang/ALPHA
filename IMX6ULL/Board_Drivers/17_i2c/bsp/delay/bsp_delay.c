#include "bsp_delay.h"
#include "int.h"
#include "bsp_led.h"

/* initialization delay function */
void delay_init(void)
{
	GPT1->CR = 0;

	GPT1->CR = (1 << 15);				/* soft reset */
	while ((GPT1->CR >> 15) & 0x01);	/* waiting the reset successful */
	
	/* 
	 * set GPT1 clock soure is ipg_clk=66Mhz, restart mode
	 * default count regsiter from 0 start
	 */
	GPT1->CR |= (1 << 1) | (1 << 6);

	/* config Frequency divider */
	GPT1->PR = 65;					/* 66 divid ,frequency = 66000000/66=1Mhz */

	/* 1M的频率，计1个数就是1us,那么0xFFFFFFFF=4294967295us=71.5min */
	GPT1->OCR[0] = 0XFFFFFFFF;
#if 0
	/* config output compare channel 1 */
	GPT1->OCR[0] = 1000000/2;		/* set interrupt cycle 500ms */

	/* open GPT1 output compare channel 1 interrupt */
	GPT1->IR = (1 << 0);

	/* config GIC */
	GIC_EnableIRQ(GPT1_IRQn);

	system_register_irqhandler(GPT1_IRQn,gpt1_irqhandler, NULL);
#endif

	/* enable GPT1 */
	GPT1->CR |= (1 << 0);

}

#if 0 
/* GPT1 interrupt service function */
void gpt1_irqhandler(unsigned int gicciar, void * param)
{
	static unsigned char state = 0;

	if(GPT1->SR & (1 << 0))
	{
		state = !state;
		led_switch(LED0, state)	;
	}

	GPT1->SR |= (1 << 0);			/* clear interrupt flag bit */
}
#endif

/* us delay */
void delay_us(unsigned int usdelay)
{
	unsigned long oldcnt,newcnt;
	unsigned long tcntvalue = 0;

	oldcnt = GPT1->CNT;
	while (1)
	{
		newcnt = GPT1->CNT;
		if(newcnt != oldcnt)
		{
			if(newcnt > oldcnt)
			{
				tcntvalue += newcnt - oldcnt;
			}
			else
			{
				tcntvalue += 0xFFFFFFFF;
			}
			oldcnt = newcnt;
			if(tcntvalue >= usdelay)
				break;
		}
	} 
}

/* ms delay */
void delay_ms(unsigned int msdelay)
{
	int i = 0;
	for(i = 0; i < msdelay; i ++)
	{
		delay_us(1000);
	}
}
/* 短延时 */
	void delay_short(volatile unsigned int n)
	{
		while (n--)
			;
}

/* 延时，一次循环大概是1ms ,在主频397MHz*/
void delay(volatile unsigned int n)
{
	while (n--)
	{
		delay_short(0x7ff);
	}
}
