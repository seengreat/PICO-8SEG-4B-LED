#ifndef __SEG_H
#define __SEG_H

#include "pico/stdlib.h"

#define SEG_RCLK_PIN 4
#define SEG_CLK_PIN 10
#define SEG_MOSI_PIN 11

#define KILOBIT   0XFE //KiloBit
#define HUNDREDS  0XFD //hundreds
#define TENS      0XFB //tens
#define UNITS     0XF7 //units
#define Dot       0x80 //decimal separator

/*------------*/
/* LED Pinout */
/*------------*/
#define LED(x) gpio_put(PICO_DEFAULT_LED_PIN, x)

void SEG_Init(void);
void SEG_Write(uint8_t location,uint8_t code);
void SEG_Display(float data,uint8_t mode);
#endif