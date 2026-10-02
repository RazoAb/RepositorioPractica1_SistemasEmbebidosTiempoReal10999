#include "buttons.h"
#include "app_config.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void button_init(gpio_num_t pin)
{
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
}

bool button_is_pressed(gpio_num_t pin)
{
    return gpio_get_level(pin) == 0;
}

void button_task(void *pvParameters)
{
    button_task_params_t *params = (button_task_params_t *)pvParameters;
    bool previous = false;

    if ((params == NULL) || (params->event == NULL)) 
	{
        vTaskDelete(NULL);
        return;
    }

    button_init(params->pin);

    for (;;) 
	{
        bool current = button_is_pressed(params->pin);

        /* Detección de flanco de pulsación. */
        if (current && !previous) 
		{
            params->event->pending = true;
        }

        previous = current;
        vTaskDelay(pdMS_TO_TICKS(BUTTON_PERIOD_MS));
    }
}
