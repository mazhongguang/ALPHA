#include "lcd.h"
#include "gpio.h"
#include "stdio.h"
#include "bsp_delay.h"

/*屏幕参数结构体变量*/
struct tftlcd_typedef tftlcd_dev;

/*LCD初始化*/
void lcd_init(void)
{
	unsigned short lcdid = 0;

	lcdid = lcd_read_panelid();
	printf("LCD ID = %#x\r\n", lcdid);

	/*屏幕IO初始*/
	lcdgpio_init();
	lcd_reset();	/*复位*/
	delay_ms(10);
	lcd_noreset();	/*停止复位*/

	/*根据不同的屏幕ID来设置屏幕参数*/
	if(lcdid == ATK4342)
	{
		tftlcd_dev.height = 272;
		tftlcd_dev.width = 480;
		tftlcd_dev.vspw = 0;
		tftlcd_dev.vbpd = 8;
		tftlcd_dev.vfpd = 8;
		tftlcd_dev.hspw = 0;
		tftlcd_dev.hbpd = 40;
		tftlcd_dev.hfpd = 5;
	}
	else if(lcdid == ATK4384)
	{
		tftlcd_dev.height = 480;
		tftlcd_dev.width = 800;
		tftlcd_dev.vspw = 3;
		tftlcd_dev.vbpd = 32;
		tftlcd_dev.vfpd = 13;
		tftlcd_dev.hspw = 48;
		tftlcd_dev.hbpd = 88;
		tftlcd_dev.hfpd = 40;
		lcdclk_init(42, 4, 8);	/*初始化LCD时钟31.5MHz*/
	}
	tftlcd_dev.pixsize = 4;		/*每个像素4个字节*/
	tftlcd_dev.framebuffer = LCD_FRAMEBUF_ADDR;
	tftlcd_dev.forecolor = LCD_RED;
	tftlcd_dev.backcolor = LCD_BLACK;
	
	/*配置LCDIF控制器接口*/
	LCDIF->CTRL = 0;
	LCDIF->CTRL |= (1 << 5) | (3 << 8) | (3 << 10) | (1 << 17) | (1 << 19);

	LCDIF->CTRL1 = 0;
	LCDIF->CTRL1 |= (7 << 16);

	LCDIF->TRANSFER_COUNT = 0;
	LCDIF->TRANSFER_COUNT |= (tftlcd_dev.height << 16) | (tftlcd_dev.width << 0);

	LCDIF->VDCTRL0 = 0;
	LCDIF->VDCTRL0 = (tftlcd_dev.vspw << 0) | (1 << 20) | (1 << 21) | (1 << 24) | (0 << 25) |
					(0 << 26) | (0 << 27) | (1 << 28) | (0 << 29);

	LCDIF->VDCTRL1 = tftlcd_dev.vspw + tftlcd_dev.vbpd + tftlcd_dev.height + tftlcd_dev.vfpd;
	LCDIF->VDCTRL2 = (tftlcd_dev.hspw + tftlcd_dev.hbpd + tftlcd_dev.width + tftlcd_dev.hfpd) |
					(tftlcd_dev.hspw << 18);
	LCDIF->VDCTRL3 = (tftlcd_dev.vspw + tftlcd_dev.vbpd) | ((tftlcd_dev.hspw + tftlcd_dev.hbpd) << 16);
	LCDIF->VDCTRL4 = (tftlcd_dev.width) | (1 << 18);

	LCDIF->CUR_BUF = (unsigned int)tftlcd_dev.framebuffer;
	LCDIF->NEXT_BUF = (unsigned int)tftlcd_dev.framebuffer;

	lcd_enable();

	lcd_clear(LCD_WHITE);
}
	
/*像素时钟初始化
 *@loopDiv 设置DIV_SELECT,范围27-54
 *@prediv 设置为1～8，代表1-8分频
 *@div 范围1～8
 *LCD CLK = 24 * loopDiv / prediv / div
 *
 */
void lcdclk_init(unsigned char loopDiv, unsigned char prediv, unsigned char div)
{
	/*不使用小数分频器*/
	CCM_ANALOG->PLL_VIDEO_NUM = 0;
	CCM_ANALOG->PLL_VIDEO_DENOM = 0;

	CCM_ANALOG->PLL_VIDEO = (1 << 13) | (2 << 19) | (loopDiv << 0);

	CCM_ANALOG->MISC2 &= ~(3 << 30);

	CCM->CSCDR2 &= ~(7 << 15);
	CCM->CSCDR2 |= (2 << 15); 

	CCM->CSCDR2 &= ~(7 << 12);
	CCM->CSCDR2 |= (prediv - 1) << 12;
	
	CCM->CBCMR &= ~(7 << 23);
	CCM->CBCMR |= (div - 1) << 23;

	CCM->CSCDR2 &= ~(7 << 9);
	CCM->CSCDR2 |= 0 << 9;
}
/*复位LCD控制器*/
void lcd_reset(void)
{
	LCDIF->CTRL |= 1 << 31;
}

/*停止复位*/
void lcd_noreset(void)
{
	LCDIF->CTRL &= ~(1 << 31);
}

/*使能LCD控制器*/
void lcd_enable(void)
{
	LCDIF->CTRL |= 1 << 0;
}

/*屏幕IO初始化*/
void lcdgpio_init(void)
{
	/*设置IO复用*/
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA00_LCDIF_DATA00, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA01_LCDIF_DATA01, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA02_LCDIF_DATA02, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA03_LCDIF_DATA03, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA04_LCDIF_DATA04, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA05_LCDIF_DATA05, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA06_LCDIF_DATA06, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_LCDIF_DATA07, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA08_LCDIF_DATA08, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA09_LCDIF_DATA09, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA10_LCDIF_DATA10, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA11_LCDIF_DATA11, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA12_LCDIF_DATA12, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA13_LCDIF_DATA13, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA14_LCDIF_DATA14, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_LCDIF_DATA15, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA16_LCDIF_DATA16, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA17_LCDIF_DATA17, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA18_LCDIF_DATA18, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA19_LCDIF_DATA19, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA20_LCDIF_DATA20, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA21_LCDIF_DATA21, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA22_LCDIF_DATA22, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_LCDIF_DATA23, 0);
	
	
	IOMUXC_SetPinMux(IOMUXC_LCD_CLK_LCDIF_CLK, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_ENABLE_LCDIF_ENABLE, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_HSYNC_LCDIF_HSYNC, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_LCDIF_VSYNC, 0);

	/* 设置电气属性 */
    /* 2、配置LCD IO属性
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 0 默认22K上拉
	 *bit [13]: 0 pull功能
	 *bit [12]: 0 pull/keeper使能
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 111 驱动能力为R0/7
	 *bit [0]: 1 高转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA00_LCDIF_DATA00,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA01_LCDIF_DATA01,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA02_LCDIF_DATA02,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA03_LCDIF_DATA03,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA04_LCDIF_DATA04,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA05_LCDIF_DATA05,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA06_LCDIF_DATA06,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_LCDIF_DATA07,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA08_LCDIF_DATA08,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA09_LCDIF_DATA09,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA10_LCDIF_DATA10,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA11_LCDIF_DATA11,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA12_LCDIF_DATA12,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA13_LCDIF_DATA13,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA14_LCDIF_DATA14,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_LCDIF_DATA15,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA16_LCDIF_DATA16,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA17_LCDIF_DATA17,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA18_LCDIF_DATA18,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA19_LCDIF_DATA19,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA20_LCDIF_DATA20,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA21_LCDIF_DATA21,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA22_LCDIF_DATA22,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_LCDIF_DATA23,0xB9);

	IOMUXC_SetPinConfig(IOMUXC_LCD_CLK_LCDIF_CLK,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_ENABLE_LCDIF_ENABLE,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_HSYNC_LCDIF_HSYNC,0xB9);
	IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_LCDIF_VSYNC,0xB9);

	/*点亮背光*/
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_GPIO1_IO08, 0x10b0);

	/*背光GPIO初始化*/
	gpio_pin_config_t bl_config;
	bl_config.direction = kGPIO_DigitalOutput;
	bl_config.outputLogic = 1;
	gpio_init(GPIO1, 8, &bl_config);
}
/*读取屏幕ID
 *@param	:	无
 *@return	:	屏幕ID 
 */
unsigned short lcd_read_panelid(void)
{
	unsigned char idx = 0;

	/*打开模拟开关，设置LCD_VSYNC为高电平*/
	gpio_pin_config_t lcdio_config;
	IOMUXC_SetPinMux(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_VSYNC_GPIO3_IO03, 0x10b0);

	/*GPIO初始化*/
	lcdio_config.direction = kGPIO_DigitalOutput;
	lcdio_config.outputLogic = 1;
	gpio_init(GPIO3, 3, &lcdio_config);

	/*read LCD ID*/
	
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA07_GPIO3_IO12, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA15_GPIO3_IO20, 0);
	IOMUXC_SetPinMux(IOMUXC_LCD_DATA23_GPIO3_IO28, 0);

	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA07_GPIO3_IO12, 0x10b0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA15_GPIO3_IO20, 0x10b0);
	IOMUXC_SetPinConfig(IOMUXC_LCD_DATA23_GPIO3_IO28, 0x10b0);
	
	lcdio_config.direction = kGPIO_DigitalInput;
	gpio_init(GPIO3, 12, &lcdio_config);
   	gpio_init(GPIO3, 20, &lcdio_config);
	gpio_init(GPIO3, 28, &lcdio_config);

	idx = (unsigned char)gpio_pinread(GPIO3, 28); 
	idx |= (unsigned char)gpio_pinread(GPIO3, 20) << 1;
	idx |= (unsigned char)gpio_pinread(GPIO3, 12) << 2;

	if(idx == 0)
		return ATK4342;
	else if(idx == 1)
		return ATK7084;
	else if(idx == 2)
		return ATK7016;
	else if(idx == 4)
		return ATK4384;
	else if(idx == 5)
		return ATK1018;
	else
		return 0;
}

/*画点函数*/
inline void lcd_drawpoint(unsigned short x, unsigned short y, unsigned int color)
{
	*(unsigned int *)((unsigned int)tftlcd_dev.framebuffer +
					tftlcd_dev.pixsize * (tftlcd_dev.width * y + x)) = color;
}

/*读点函数*/
inline unsigned int lcd_readpoint(unsigned short x, unsigned short y)
{
	return *(unsigned int *)((unsigned int)tftlcd_dev.framebuffer + 
							tftlcd_dev.pixsize * (tftlcd_dev.width * y + x));
}

/*清屏函数*/
void lcd_clear(unsigned int color)
{
	unsigned int num;
	unsigned int i = 0;

	unsigned int *startaddr = (unsigned int *)(tftlcd_dev.framebuffer);

	num = (unsigned int)(tftlcd_dev.width * tftlcd_dev.height);

	for(i = 0; i < num ; i++)
	{
		startaddr[i] = color;
	}
}

/*
 * @description		: 以指定的颜色填充一块矩形
 * @param - x0		: 矩形起始点坐标X轴
 * @param - y0		: 矩形起始点坐标Y轴
 * @param - x1		: 矩形终止点坐标X轴
 * @param - y1		: 矩形终止点坐标Y轴
 * @param - color	: 要填充的颜色
 * @return 			: 读取到的指定点的颜色值
 */
void lcd_fill(unsigned    short x0, unsigned short y0,
                 unsigned short x1, unsigned short y1, unsigned int color)
{
    unsigned short x, y;

	if(x0 < 0) x0 = 0;
	if(y0 < 0) y0 = 0;
	if(x1 >= tftlcd_dev.width) x1 = tftlcd_dev.width - 1;
	if(y1 >= tftlcd_dev.height) y1 = tftlcd_dev.height - 1;

    for(y = y0; y <= y1; y++)
    {
        for(x = x0; x <= x1; x++)
			lcd_drawpoint(x, y, color);
    }
}
