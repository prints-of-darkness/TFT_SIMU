
// https://github.com/anothermist/DISPLAYS/tree/master

#include "ssd1963_fsmc.h"

static uint16_t _x1;
static uint16_t _y1;
static uint16_t _x2;
static uint16_t _y2;

uint32_t _RGB(uint8_t r, uint8_t g, uint8_t b)
{   
    return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
}

uint16_t H24_RGB565(uint8_t reverse, uint32_t color)
{
	uint8_t b = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t r = color & 0xFF;
	if (reverse) return ((b / 8) << 11) | ((g / 4) << 5) | (r / 8);
	else return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}

void LCD_Pixel(uint16_t x, uint16_t y, uint32_t color)
{
	uint32_t c = 0xFF000000 | color;
	DrawPixel(x, y, c);
}

void LCD_Rect_Fill(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
	uint32_t c = 0xFF000000 | color;
	DrawRectangle(x, y, w, h, c, Transparent);
}

void LCD_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint32_t color)
{
	uint32_t c = 0xFF000000 | color;
	DrawLine(x1, y1, x2, y2, size, c);
}

void LCD_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint32_t color)
{
	LCD_Line(x1, y1, x2, y2, size, color);
	LCD_Line(x2, y2, x3, y3, size, color);
	LCD_Line(x3, y3, x1, y1, size, color);
}

#define ABS(x) ((x) > 0 ? (x) : -(x))

void LCD_Triangle_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;
	
	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1)
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)
	{
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else
	{
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		LCD_Line(x, y, x3, y3, 1, color);

		num += numadd;
		if (num >= den)
		{
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void LCD_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, uint32_t color)
{
	LCD_Line(x, y, x + w, y, size, color);
	LCD_Line(x, y + h, x + w, y + h, size, color);
	LCD_Line(x, y, x, y + h, size, color);
	LCD_Line(x + w, y, x + w, y + h, size, color);
}

void LCD_Ellipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint8_t fill, uint8_t size, uint32_t color)
{
	int16_t x, y;
	int32_t rx2 = rx * rx;
	int32_t ry2 = ry * ry;
	int32_t fx2 = 4 * rx2;
	int32_t fy2 = 4 * ry2;
	int32_t s;
	if (fill)
	{
		for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++)
		{
			LCD_Line(x0 - x, y0 - y, x0 + x + 1 - size, y0 - y, size, color);
			LCD_Line(x0 - x, y0 + y, x0 + x + 1 - size, y0 + y, size, color);
			if (s >= 0)
			{
				s += fx2 * (1 - y);
				y--;
			}
			s += ry2 * ((4 * x) + 6);
		}
		for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1-2 * rx); rx2 * y <= ry2 * x; y++)
		{
			LCD_Line(x0 - x, y0 - y, x0 + x + 1 - size, y0 - y, size, color);
			LCD_Line(x0 - x, y0 + y, x0 + x + 1 - size, y0 + y, size, color);
			if (s >= 0)
			{
				s += fy2 * (1 - x);
				x--;
			}
			s += rx2 * ((4 * y) + 6);
		}
	}
	else
	{
		for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++)
		{
			LCD_Rect_Fill(x0 + x, y0 + y, size, size, color);
			LCD_Rect_Fill(x0 - x, y0 + y, size, size, color);
			LCD_Rect_Fill(x0 + x, y0 - y, size, size, color);
			LCD_Rect_Fill(x0 - x, y0 - y, size, size, color);
			if (s >= 0)
			{
				s += fx2 * (1 - y);
				y--;
			}
			s += ry2 * ((4 * x) + 6);
		}
		for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++)
		{
			LCD_Rect_Fill(x0 + x, y0 + y, size, size, color);
			LCD_Rect_Fill(x0 - x, y0 + y, size, size, color);
			LCD_Rect_Fill(x0 + x, y0 - y, size, size, color);
			LCD_Rect_Fill(x0 - x, y0 - y, size, size, color);
			if (s >= 0)
			{
				s += fy2 * (1 - x);
				x--;
			}
			s += rx2 * ((4 * y) + 6);
		}
	}
}

void LCD_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint32_t color)
{
	int a_, b_, P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_)
	{
		if (fill == 1)
		{
			LCD_Rect_Fill(x - a_, y - b_, 2 * a_ + 1, 2 * b_ + 1, color);
			LCD_Rect_Fill(x - b_, y - a_, 2 * b_ + 1, 2 * a_ + 1, color);
		}
		else
		{
			LCD_Rect_Fill(a_ + x, b_ + y, size, size, color);
			LCD_Rect_Fill(b_ + x, a_ + y, size, size, color);
			LCD_Rect_Fill(x - a_, b_ + y, size, size, color);
			LCD_Rect_Fill(x - b_, a_ + y, size, size, color);
			LCD_Rect_Fill(b_ + x, y - a_, size, size, color);
			LCD_Rect_Fill(a_ + x, y - b_, size, size, color);
			LCD_Rect_Fill(x - a_, y - b_, size, size, color);
			LCD_Rect_Fill(x - b_, y - a_, size, size, color);
		}
		if (P < 0)
		{
			P = (P + 3) + (2 * a_);
			a_++;
		}
		else
		{
			P = (P + 5) + (2 * (a_ - b_));
			a_++;
			b_--;
		}
	}
}

void LCD_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint32_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			LCD_Rect_Fill(x0 + x, y0 + y, size, size, color);
			LCD_Rect_Fill(x0 + y, y0 + x, size, size, color);
		}
		if (cornername & 0x2) {
			LCD_Rect_Fill(x0 + x, y0 - y, size, size, color);
			LCD_Rect_Fill(x0 + y, y0 - x, size, size, color);
		}
		if (cornername & 0x8) {
			LCD_Rect_Fill(x0 - y, y0 + x, size, size, color);
			LCD_Rect_Fill(x0 - x, y0 + y, size, size, color);
		}
		if (cornername & 0x1) {
			LCD_Rect_Fill(x0 - y, y0 - x, size, size, color);
			LCD_Rect_Fill(x0 - x, y0 - y, size, size, color);
		}
	}
}

void LCD_Rect_Round(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint32_t color)
{
	LCD_Line(x + (r + 2), y, x + length + size - (r + 2), y, size, color);
	LCD_Line(x + (r + 2), y + width - 1, x + length + size - (r + 2), y + width - 1, size, color);
	LCD_Line(x, y + (r + 2), x, y + width - size - (r + 2), size, color);
	LCD_Line(x + length - 1, y + (r + 2), x + length - 1, y + width - size - (r + 2), size, color);

	LCD_Circle_Helper(x + (r + 2), y + (r + 2), (r + 2), 1, size, color);
	LCD_Circle_Helper(x + length - (r + 2) - 1, y + (r + 2), (r + 2), 2, size, color);
	LCD_Circle_Helper(x + length - (r + 2) - 1, y + width - (r + 2) - 1, (r + 2), 4, size, color);
	LCD_Circle_Helper(x + (r + 2), y + width - (r + 2) - 1, (r + 2), 8, size, color);
}

void LCD_Circle_Fill_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			LCD_Line(x0 + x, y0 - y, x0 + x, y0 - y + 2 * y + delta, 1, color);
			LCD_Line(x0 + y, y0 - x, x0 + y, y0 - x + 2 * x + delta, 1, color);
		}
		if (cornername & 0x2) {
			LCD_Line(x0 - x, y0 - y, x0 - x, y0 - y + 2 * y + delta, 1, color);
			LCD_Line(x0 - y, y0 - x, x0 - y, y0 - x + 2 * x + delta, 1, color);
		}
	}
}

void LCD_Rect_Round_Fill(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint32_t color)
{
	LCD_Rect_Fill(x + r, y, length - 2 * r, width, color);
	LCD_Circle_Fill_Helper(x + length - r - 1, y + r, r, 1, width - 2 * r - 1, color);
	LCD_Circle_Fill_Helper(x + r, y + r, r, 2, width - 2 * r - 1, color);
}

static void LCD_Char(int16_t x, int16_t y, const GFXglyph *glyph, const GFXfont *font, uint8_t size, uint32_t color)
{
	uint8_t  *bitmap = font -> bitmap;
	uint16_t bo = glyph -> bitmapOffset;
	uint8_t bits = 0, bit = 0;
	uint16_t set_pixels = 0;
	uint8_t  cur_x, cur_y;
	for(cur_y = 0; cur_y < glyph -> height; cur_y++)
	{
		for(cur_x = 0; cur_x < glyph -> width; cur_x++)
		{
			if(bit == 0)
			{
				bits = (*(const unsigned char *)(&bitmap[bo++]));
				bit  = 0x80;
			}
			if(bits & bit)
			{
				set_pixels++;
			}
			else if (set_pixels > 0)
			{
				LCD_Rect_Fill(x + (glyph -> xOffset + cur_x - set_pixels) * size, y + (glyph -> yOffset + cur_y) * size, size * set_pixels, size, color);
				set_pixels = 0;
			}
			bit >>= 1;
		}
		if (set_pixels > 0)
		{
			LCD_Rect_Fill(x + (glyph -> xOffset + cur_x-set_pixels) * size, y + (glyph -> yOffset + cur_y) * size, size * set_pixels, size, color);
			set_pixels = 0;
		}
	}
}

void LCD_Font(uint16_t x, uint16_t y, const char *text, const GFXfont *p_font, uint8_t size, uint32_t color)
{
	int16_t cursor_x = x;
	int16_t cursor_y = y;
	GFXfont font;
	memcpy((&font), (p_font), (sizeof(GFXfont)));
	for(uint16_t text_pos = 0; text_pos < strlen(text); text_pos++)
	{
		char c = text[text_pos];
		if(c == '\n')
		{
			cursor_x = x;
			cursor_y += font.yAdvance * size;
		}
		else if(c >= font.first && c <= font.last && c != '\r')
		{
			GFXglyph glyph;
			memcpy((&glyph), (&font.glyph[c - font.first]), (sizeof(GFXglyph)));
			LCD_Char(cursor_x, cursor_y, &glyph, &font, size, color);
			cursor_x += glyph.xAdvance * size;
		}
	}
}
