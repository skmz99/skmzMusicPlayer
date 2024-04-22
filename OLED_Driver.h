#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


#define COLOR		uint16_t
#define POINT		uint16_t
#define LENGTH		uint16_t

#define OLED_X_MAXPIXEL	132
#define OLED_Y_MAXPIXEL	64

#define OLED_X 2
#define OLED_Y 0

#define OLED_WIDTH (OLED_X_MAXPIXEL - 2 * OLED_X)
#define OLED_HEIGHT OLED_Y_MAXPIXEL

extern char Buffer[];

typedef struct{
	LENGTH OLED_Dis_Column;
	LENGTH OLED_Dis_Page;
	POINT OLED_X_Adjust;
	POINT OLED_Y_Adjust;
}OLED_DIS;
extern OLED_DIS sOLED_DIS;

void OLED_Init();
void OLED_SetGramScanWay(void);

void OLED_SetCursor(POINT Xpoint, POINT Ypoint);
void OLED_SetColor(POINT Xpoint, POINT Ypoint, COLOR Color);
void OLED_Clear(COLOR Color);
void OLED_Display(void);
#endif
