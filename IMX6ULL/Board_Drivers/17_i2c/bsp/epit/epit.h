#ifndef __EPIT_H
#define __EPIT_H

#include "imx6u.h"

void epit1_irqhandler(unsigned int gicciar, void *param);

void epit1_init(unsigned int frac, unsigned int value);

#endif // !1___EPIT_H
