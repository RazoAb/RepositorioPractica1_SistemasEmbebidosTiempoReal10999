#include "display.h"
#include "app_config.h"
#include "system_state.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const uint8_t DIGITS[10] = 
{
    0x3FU, 0x06U, 0x5BU, 0x4FU, 0x66U,
    0x6DU, 0x7DU, 0x07U, 0x7FU, 0x6FU
};

static const gpio_num_t SEGMENTS[7] = 
{
    SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G
};

static void disable_displays(void)
{
    gpio_set_level(DISPLAY_1_EN, 0);
    gpio_set_level(DISPLAY_2_EN, 0);
}

static void write_segments(uint8_t pattern)
{
    uint8_t i;

    for (i = 0U; i < 7U; ++i) 
	{
        gpio_set_level(SEGMENTS[i], (pattern >> i) & 0x01U);
    }
}

void display_init(void)
{
    uint8_t i;

    for (i = 0U; i < 7U; ++i) 
	{
        gpio_reset_pin(SEGMENTS[i]);
        gpio_set_direction(SEGMENTS[i], GPIO_MODE_OUTPUT);
    }

    gpio_reset_pin(DISPLAY_1_EN);
    gpio_set_direction(DISPLAY_1_EN, GPIO_MODE_OUTPUT);

    gpio_reset_pin(DISPLAY_2_EN);
    gpio_set_direction(DISPLAY_2_EN, GPIO_MODE_OUTPUT);

    disable_displays();
    write_segments(0U);
}

void display_show_digit(uint8_t display_id, uint8_t digit)
{
    if (digit > 9U) 
	{
		
        return;
    }

    disable_displays();
    write_segments(DIGITS[digit]);

    if (display_id == 1U) 
	{
        gpio_set_level(DISPLAY_1_EN, 1);
    } 
	else if (display_id == 2U) 
	{
        gpio_set_level(DISPLAY_2_EN, 1);
    }
}

void display_blank(uint8_t display_id)
{
    if (display_id == 1U) 
	{
        gpio_set_level(DISPLAY_1_EN, 0);
    } 
	else if (display_id == 2U) 
	{
        gpio_set_level(DISPLAY_2_EN, 0);
    }
}

void display_refresh_task(void *pvParameters)
{
    app_context_t *context = (app_context_t *)pvParameters;


    if (context == NULL) 
	{
        vTaskDelete(NULL);
        return;
    }

    display_init();
    for (;;) 
	{
        display_show_digit(1U, context->counter1.value);
        vTaskDelay(pdMS_TO_TICKS(5U));

        display_show_digit(2U, context->counter2.value);
        vTaskDelay(pdMS_TO_TICKS(5U));
    }
}
