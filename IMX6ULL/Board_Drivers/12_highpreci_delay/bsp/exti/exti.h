#ifndef	__EXTI_H
#define __EXTI_H

#include "imx6u.h"

void gpio1_io18_irqhandler(unsigned int gicciar, void *param);
void exti_init(void);

#endif // !1_
