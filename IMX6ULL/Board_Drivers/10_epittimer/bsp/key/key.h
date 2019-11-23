#ifndef __KEY_H
#define __KEY_H

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"

/* 按键值 */
enum keyvalue
{
	KEY_NONE = 0,
	KEY0_VALUE = 1,
};

int key_getvalue(void);
int read_key(void);
void key_init(void);

#endif
