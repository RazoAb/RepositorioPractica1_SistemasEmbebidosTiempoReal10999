#ifndef BUTTONS_H
#define BUTTONS_H

#include "driver/gpio.h"
#include "system_state.h"

typedef struct 
{
    gpio_num_t pin;
    button_event_t *event;
} button_task_params_t;

void button_init(gpio_num_t pin);
bool button_is_pressed(gpio_num_t pin);
void button_task(void *pvParameters);

#endif
