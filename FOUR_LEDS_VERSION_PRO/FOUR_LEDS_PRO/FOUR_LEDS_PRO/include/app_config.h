#ifndef APP_CONFIG_H
#define APP_CONFIG_H
 
#include <stdint.h>
#include "driver/gpio.h"
 
/* Segmentos compartidos por ambos displays (multiplexados). */
#define SEG_A GPIO_NUM_4 //D23
#define SEG_B GPIO_NUM_13 //D2
#define SEG_C GPIO_NUM_14 //D3
#define SEG_D GPIO_NUM_18 //D1
#define SEG_E GPIO_NUM_19 //D19
#define SEG_F GPIO_NUM_21 //D22
#define SEG_G GPIO_NUM_22 //D25
 
/* Habilitación de cada display. Ajustar al hardware real. */
#define DISPLAY_1_EN GPIO_NUM_23 //D14
#define DISPLAY_2_EN GPIO_NUM_27 //D32
 
#define BTN_START_PAUSE GPIO_NUM_17 //D27
#define BTN_DIRECTION   GPIO_NUM_5 //D4
#define BTN_SPEED       GPIO_NUM_25  //D21
#define BTN_MODE        GPIO_NUM_26 //D0
 
#define SLOW_PERIOD_MS   500U
#define FAST_PERIOD_MS   250U
#define BUTTON_PERIOD_MS 20U
#define MANAGER_PERIOD_MS 10U
 
#endif
 