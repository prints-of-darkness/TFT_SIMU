
#define IMM2D_WIDTH  800
#define IMM2D_HEIGHT 480
#define IMM2D_SCALE 1

#define IMM2D_IMPLEMENTATION
#include "immediate2d.h"
#include "ssd1963_fsmc.h"

#include "fonts/Font_3_Tiny.h"
#include "fonts/Font_3_PicoPixel.h"
#include "fonts/Font_3_TomThumb_Extended.h"
#include "fonts/Font_5_Org.h"
#include "fonts/Font_8_Sinclair.h"
#include "fonts/Font_8_Sinclair_Inverted.h"
#include "fonts/Font_8_Tiny.h"
#include "fonts/Font_8_Myke.h"
#include "fonts/Font_8_Default.h"
#include "fonts/Font_8_Retro.h"
#include "fonts/Font_9_Mono.h"
#include "fonts/Font_9_Mono_Bold.h"
#include "fonts/Font_9_Mono_Bold_Oblique.h"
#include "fonts/Font_9_Mono_Oblique.h"
#include "fonts/Font_9_Sans.h"
#include "fonts/Font_9_Sans_Bold.h"
#include "fonts/Font_9_Sans_Bold_Oblique.h"
#include "fonts/Font_9_Sans_Oblique.h"
#include "fonts/Font_9_Serif.h"
#include "fonts/Font_9_Serif_Bold.h"
#include "fonts/Font_9_Serif_Bold_Italic.h"
#include "fonts/Font_9_Serif_Italic.h"
#include "fonts/Font_12_Mono.h"
#include "fonts/Font_12_Mono_Bold.h"
#include "fonts/Font_12_Mono_Bold_Oblique.h"
#include "fonts/Font_12_Mono_Oblique.h"
#include "fonts/Font_12_Sans.h"
#include "fonts/Font_12_Sans_Bold.h"
#include "fonts/Font_12_Sans_Bold_Oblique.h"
#include "fonts/Font_12_Sans_Oblique.h"
#include "fonts/Font_12_Serif.h"
#include "fonts/Font_12_Serif_Bold.h"
#include "fonts/Font_12_Serif_Bold_Italic.h"
#include "fonts/Font_12_Serif_Italic.h"
#include "fonts/Font_16_Arial.h"
#include "fonts/Font_16_Arial_Bold.h"
#include "fonts/Font_16_Arial_Italic.h"
#include "fonts/Font_16_Default.h"
#include "fonts/Font_16_FranklinGothic.h"
#include "fonts/Font_16_Hallfetica.h"
#include "fonts/Font_16_Nadianne.h"
#include "fonts/Font_16_Sinclair.h"
#include "fonts/Font_16_Sinclair_Inverted.h"
#include "fonts/Font_16_Swiss_Outline.h"
#include "fonts/Font_16_Matrix_Full.h"
#include "fonts/Font_16_Matrix_Full_Slash.h"
#include "fonts/Font_16_Matrix_Num.h"
#include "fonts/Font_16_Arial_Round.h"
#include "fonts/Font_16_OCR.h"
#include "fonts/Font_16_Segment_16_Full.h"
#include "fonts/Font_16_Grotesk.h"
#include "fonts/Font_16_Grotesk_Bold.h"
#include "fonts/Font_16_Retro.h"
#include "fonts/Font_18_Mono.h"
#include "fonts/Font_18_Mono_Bold.h"
#include "fonts/Font_18_Mono_Bold_Oblique.h"
#include "fonts/Font_18_Mono_Oblique.h"
#include "fonts/Font_18_Sans.h"
#include "fonts/Font_18_Sans_Bold.h"
#include "fonts/Font_18_Sans_Bold_Oblique.h"
#include "fonts/Font_18_Sans_Oblique.h"
#include "fonts/Font_18_Serif.h"
#include "fonts/Font_18_Serif_Bold.h"
#include "fonts/Font_18_Serif_Bold_Italic.h"
#include "fonts/Font_18_Serif_Italic.h"
#include "fonts/Font_24_Matrix_Num.h"
#include "fonts/Font_24_Inconsola.h"
#include "fonts/Font_24_Ubuntu.h"
#include "fonts/Font_24_Ubuntu_Bold.h"
#include "fonts/Font_24_Segment_16_Full.h"
#include "fonts/Font_24_Mono.h"
#include "fonts/Font_24_Mono_Bold.h"
#include "fonts/Font_24_Mono_Bold_Oblique.h"
#include "fonts/Font_24_Mono_Oblique.h"
#include "fonts/Font_24_Grotesk.h"
#include "fonts/Font_24_Grotesk_Bold.h"
#include "fonts/Font_24_Sans.h"
#include "fonts/Font_24_Sans_Bold.h"
#include "fonts/Font_24_Sans_Bold_Oblique.h"
#include "fonts/Font_24_Sans_Oblique.h"
#include "fonts/Font_24_Serif.h"
#include "fonts/Font_24_Serif_Bold.h"
#include "fonts/Font_24_Serif_Bold_Italic.h"
#include "fonts/Font_24_SerifItalic.h"
#include "fonts/Font_32_Dingbats.h"
#include "fonts/Font_32_Special.h"
#include "fonts/Font_32_Calibri_Bold.h"
#include "fonts/Font_32_Arial_Num_Plus.h"
#include "fonts/Font_32_Matrix_Num.h"
#include "fonts/Font_32_Segment_7_Full.h"
#include "fonts/Font_32_Segment_7_Num_Plus.h"
#include "fonts/Font_32_Segment_16_Full.h"
#include "fonts/Font_32_Segment_18_Full.h"
#include "fonts/Font_32_Grotesk.h"
#include "fonts/Font_32_Grotesk_Bold.h"
#include "fonts/Font_40_Segment_16_Full.h"
#include "fonts/Font_48_Battery.h"
#include "fonts/Font_48_Segment_16_Num.h"
#include "fonts/Font_64_Segment_16_Num.h"
#include "fonts/Font_64_Segment_7_Num.h"

void lib_tester() 
{
	LCD_Rect_Fill(0, 0, 800, 480, BLUE);
	LCD_Rect_Fill(1, 1, 798, 478, BLACK);
	Wait(1000);

	LCD_Rect_Fill(0, 0, 160, 128, BLACK);
	for (uint8_t x = 8; x <= 160; x += 8) {
		LCD_Line(0, 0, x, 128, 1, GREEN);
	}
	for (uint8_t y = 8; y <= 128; y += 8) {
		LCD_Line(0, 0, 160, y, 1, GREEN);
	}
	Wait(250);

	uint8_t h = 16;
	uint8_t w = 20;
	for (uint8_t i = 0; i < 8; i++) {
		LCD_Rect(80 - w / 2, 64 - h / 2, w, h, 2, YELLOW);
		h += 16;
		w += 20;
	}
	Wait(250);

	LCD_Rect_Fill(0, 0, 160, 128, BLUE);
	LCD_Rect_Fill(1, 1, 158, 126, BLACK);
	LCD_Font(5, 40, "This is\n just a Test\n TomThumb Ext\n", &Font_3_TomThumb_Extended, 1, YELLOW);
	LCD_Line(23, 20, 137, 20, 1, MAGENTA);
	LCD_Line(23, 21, 137, 21, 1, BLUE);
	LCD_Line(23, 21, 137, 21, 1, BLUE);
	LCD_Font(41, 10, "DISPLAY DRIVER", &Font_5_Org, 1, MAGENTA);
	LCD_Font(45, 35, "SERIF BOLD", &Font_9_Serif_Bold, 1, RED);
	LCD_Circle(40, 90, 30, 0, 1, RED);
	LCD_Circle(45, 90, 20, 1, 1, BLUE);
	LCD_Triangle_Fill(5, 5, 5, 20, 25, 25, BLUE);
	LCD_Triangle(5, 5, 5, 20, 25, 25, 1, RED);
	LCD_Rect(60, 45, 30, 20, 2, GREEN);
	LCD_Rect_Round(80, 70, 60, 25, 10, 3, WHITE);
	LCD_Rect_Round_Fill(80, 100, 60, 25, 10, WHITE);
	LCD_Ellipse(60, 100, 30, 20, 0, 2, YELLOW);
	LCD_Ellipse(125, 60, 25, 15, 1, 1, YELLOW);
	LCD_Font(0, 200, "1234567890", &Font_32_Segment_7_Num_Plus, 1, RED);
	LCD_Font(10, 220, "1234567890 Default Font", &Font_8_Default, 1, RED);
}

void run() {
	lib_tester();
	SaveImage(123); //capture TFT display & save file to desktop.
}