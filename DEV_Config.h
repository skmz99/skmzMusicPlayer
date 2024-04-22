#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <pigpio.h>

//Keys

#define KEY_UP_PIN 	6
#define KEY_DOWN_PIN	19
#define KEY_LEFT_PIN	5
#define KEY_RIGHT_PIN	26
#define KEY_PRESS_PIN	13
#define KEY1_PIN	21
#define KEY2_PIN	20
#define KEY3_PIN	16

#define KEY_UP_RD	gpioRead(KEY_UP_PIN)
#define KEY_DOWN_RD	gpioRead(KEY_DOWN_PIN)
#define KEY_LEFT_RD	gpioRead(KEY_LEFT_PIN)
#define KEY_RIGHT_RD	gpioRead(KEY_RIGHT_PIN)
#define KEY_PRESS_RD	gpioRead(KEY_PRESS_PIN)
#define KEY1_RD		gpioRead(KEY1_PIN)
#define KEY2_RD		gpioRead(KEY2_PIN)
#define KEY3_RD		gpioRead(KEY3_PIN)


#endif
