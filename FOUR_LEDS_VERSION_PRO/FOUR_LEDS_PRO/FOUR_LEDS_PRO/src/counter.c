#include "counter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void counter_init(bcd_counter_t *counter, uint8_t initial_value)
{
    if (counter != NULL) 
	{
        counter->value = initial_value % 10U;
    }
}

void counter_step(bcd_counter_t *counter, count_direction_t direction)
{
    if (counter == NULL) 
	{
    // TO DO
        return;
    }

    if (direction == COUNT_UP) 
	{
        counter->value = (counter->value >= 9U) ? 0U : (counter->value + 1U);
    } 
	else 
	{
        counter->value = (counter->value == 0U) ? 9U : (counter->value - 1U);
    }
}

uint8_t counter_get_value(const bcd_counter_t *counter)
{
    return (counter != NULL) ? counter->value : 0U;
}

void counter_task(void *pvParameters)
{
    counter_config_t *config = (counter_config_t *)pvParameters;
    bcd_counter_t counter;
    TickType_t period_ticks;
    TickType_t start;

    if (config == NULL) 
	{
        vTaskDelete(NULL);
        return;
    }

   
    for (;;) 
	{
        period_ticks = pdMS_TO_TICKS(config->period_ms);
        if (period_ticks == 0)
        {
            period_ticks = 1;
        }

        start = xTaskGetTickCount();
        vTaskDelay(period_ticks);

        if ((xTaskGetTickCount() - start) < period_ticks)
        {
            continue;
        }


        counter_init(&counter, config->value);
        counter_step(&counter, config->direction);
        config->value = counter_get_value(&counter);
    }
}
