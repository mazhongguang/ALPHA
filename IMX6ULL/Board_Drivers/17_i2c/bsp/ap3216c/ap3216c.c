#include "ap3216c.h"
#include "i2c.h"
#include "gpio.h"
#include "bsp_delay.h"
#include "stdio.h"

/*初始化AP3216C*/
void ap3216c_init(void)
{
	unsigned char value = 0;
	/*IO初始化*/
	IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);
	IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);
	IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70b0);
	IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0x70b0);

	/*I2C接口初始化*/
	i2c_init(I2C1);

	/*AP3216C传感器初始化*/
	ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONF, 0x4);	/*reset*/
	delay_ms(50);
	ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONF, 0x3);
	value = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONF);
	printf("ap3216c system config reg=%#x\r\n", value);
}

/*ap3216c read one byte*/
unsigned char ap3216c_readonebyte(unsigned char addr, unsigned char reg)
{
	unsigned char val = 0;
	struct i2c_transfer masterXfer;

	masterXfer.slaveAddress = addr;
	masterXfer.subaddress	= reg;
	masterXfer.direction	= kI2C_Read;
	masterXfer.subaddressSize = 1;
	masterXfer.data			= &val;
	masterXfer.dataSize		= 1;
	i2c_master_transfer(I2C1, &masterXfer);

	return val;
}

/*ap3216c write one byte 
 *return error
 */
unsigned char ap3216c_writeonebyte(unsigned char addr, unsigned char reg, unsigned char data)
{
	unsigned char writedata = data;
	unsigned char status = 0;
	struct i2c_transfer masterXfer;

	masterXfer.slaveAddress = addr;
	masterXfer.subaddress	= reg;
	masterXfer.direction	= kI2C_Write;
	masterXfer.subaddressSize = 1;
	masterXfer.data			= &writedata;
	masterXfer.dataSize		= 1;
	if(i2c_master_transfer(I2C1, &masterXfer))
	{
		status = 1;
	}

	return status;
}

/*read ap3216c data*/
void ap3216c_readdata(unsigned short *ir, unsigned short *ps, unsigned short *als)
{
	unsigned char buf[6];
	unsigned char i = 0;

	/*loop read data*/
	for(i = 0; i < 6; i++)
	{
		buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_IRDATALOW + i);
	}

	if(buf[0] & 0x80 )				/*ture is ir&ps data invalid*/
	{
		*ir = 0;
		*ps = 0;
	}
	else
	{
		*ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);
		*ps = (((unsigned short)buf[5] & 0x3F) << 4) | (buf[4] & 0xF);
	}

	*als = ((unsigned short)buf[3] << 8) | buf[2];
}

