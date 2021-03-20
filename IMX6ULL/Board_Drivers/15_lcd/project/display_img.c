/*#include "img.h"*/
#include "lcdapi.h"
#include "display_img.h"
void display_img(unsigned short startx, unsigned short starty)
{
	unsigned int x,y;
	unsigned int color = 0;
	unsigned int index = 0;

	for(x = startx; x < (startx + 720); x++)
	{
		for(y = starty; y < (starty + 480); y++)
		{	
			/*color = (gImage_img[index] << 16) | (gImage_img[index + 1] << 8) | (gImage_img[index + 2] );*/
			lcd_drawpoint(x , y, color);
			index += 3;
		}
	}
}
