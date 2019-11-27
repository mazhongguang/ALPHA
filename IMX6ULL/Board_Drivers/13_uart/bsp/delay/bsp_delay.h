#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include "imx6u.h"

void delay_short(volatile unsigned int n);
void delay(volatile unsigned int n);
void delay_init(void);
void gpt1_irqhandler(unsigned int gicciar, void * param);
void delay_ms(unsigned int msdelay);
void delay_us(unsigned int usdelay);

#endif // !BSP_DELAY_H
