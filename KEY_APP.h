#ifndef _KEY_APP_H
#define _KEY_APP_H

#include <pigpio.h>
#include "DEV_Config.h"
#include "dynamicArray.h"

#define OLED_CS		8
#define OLED_RST	25
#define OLED_DC		24

#define OLED_CS_0 	gpioWrite(OLED_CS, 0)
#define OLED_CS_1	gpioWrite(OLED_CS, 1)

#define OLED_RST_0	gpioWrite(OLED_RST, 0)
#define OLED_RST_1	gpioWrite(OLED_RST, 1)
#define OLED_RST_RD	gpioRead(OLED_RST)

#define OLED_DC_0	gpioWrite(OLED_DC, 0)
#define OLED_DC_1	gpioWrite(OLED_DC, 1)


//Keys

#define KEY_UP_PIN	6
#define KEY_DOWN_PIN	19
#define KEY_LEFT_PIN	5
#define KEY_RIGHT_PIN	26
#define KEY_PRESS_PIN	13
#define KEY1_PIN	21
#define KEY2_PIN	20
#define KEY3_PIN	16

#define KEY_UP_RD	gpioRead(KEY_UP_PIN)
#define KEY_DOWN_RD	gpioRead(KEY_DOWN_PIN)
#define KEY_LEFT_RD 	gpioRead(KEY_LEFT_PIN)
#define KEY_RIGHT_RD	gpioRead(KEY_RIGHT_PIN)
#define KEY_PRESS_RD	gpioRead(KEY_PRESS_PIN)
#define KEY1_RD		gpioRead(KEY1_PIN)
#define KEY2_RD		gpioRead(KEY2_PIN)
#define KEY3_RD		gpioRead(KEY3_PIN)




#define DisString_EN  GUI_DisString_EN
#define DrawRectangle  GUI_DrawRectangle
#define Font_KEY  Font12
#define Font_Offset  2

#define CLEAR  OLED_Clear


#define WORD_COLOR WHITE
#define BACK_COLOR FONT_BACKGROUND

#define WIDTH  132
#define HEIGHT  64


void KEY_Listen(void);
void GPIO_Init();
void Menu_Key_Listen(void);
void Music_Key_Listen(void);
void keyAppArrayinit(Array *stringList);
#endif

