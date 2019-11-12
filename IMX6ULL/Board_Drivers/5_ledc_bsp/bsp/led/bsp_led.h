#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "fsl_iomuxc.h"
#include "fsl_common.h"
#include "MCIMX6Y2.h"

#define LED0 	0

void led_init(void);
void led_on(void);
void led_off(void);
void led_swith(int led, int status);

#endif // !__BSP_LED_H
