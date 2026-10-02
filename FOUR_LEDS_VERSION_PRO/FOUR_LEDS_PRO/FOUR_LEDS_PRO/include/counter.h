#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>
#include "system_state.h"

typedef struct 
{
    uint8_t value;
} bcd_counter_t;

void counter_init(bcd_counter_t *counter, uint8_t initial_value);
void counter_step(bcd_counter_t *counter, count_direction_t direction);
uint8_t counter_get_value(const bcd_counter_t *counter);

void counter_task(void *pvParameters);

#endif
