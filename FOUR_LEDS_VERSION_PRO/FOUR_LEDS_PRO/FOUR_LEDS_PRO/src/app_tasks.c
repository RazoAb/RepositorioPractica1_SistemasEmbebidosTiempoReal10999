#include "app_tasks.h"
#include "app_config.h"
#include "buttons.h"
#include "counter.h"
#include "display.h"
#include "esp_log.h"

static const char *TAG = "TASK_MANAGER";

static count_direction_t opposite_direction(count_direction_t direction)
{
    return (direction == COUNT_UP) ? COUNT_DOWN : COUNT_UP;
}

static void apply_configuration(app_context_t *c)
{
    uint32_t period;

    c->counter1.direction = c->system.master_direction;

    if (c->system.coupling_mode == COUPLING_SAME)
    {
         c->counter2.direction = c->system.master_direction;
        if (c->system.master_direction == COUNT_UP)
        {
            if (c->counter1.value > c->counter2.value)
            {
                c->counter2.value = c->counter1.value;
            }
            else
            {
                c->counter1.value = c->counter2.value;
            }
        }
        else
        {
            if (c->counter1.value < c->counter2.value)
            {
                c->counter2.value = c->counter1.value;
            }
            else
            {
                c->counter1.value = c->counter2.value;
            }
        }
    }

    else
    {
        c->counter2.direction = opposite_direction(c->system.master_direction);
    }

    period = (c->system.speed_mode == SPEED_FAST) ? FAST_PERIOD_MS : SLOW_PERIOD_MS;

    c->counter1.period_ms = period; // TO DO
    c->counter2.period_ms = period; // TO DO

    printf("CONFIG: DIR=%s, SPEED=%s, MODE=%s\n, RUN_STATE=%s\n", direction_to_string(c->system.master_direction),
           speed_to_string(c->system.speed_mode), coupling_to_string(c->system.coupling_mode), run_state_to_string(c->system.run_state));
}

static void set_counters_running(app_context_t *c, bool running)
{
    if (running)
    {
        vTaskResume(c->counter1_handle); // TO DO
        vTaskResume(c->counter2_handle); // TO DO
    }
    else
    {
        vTaskSuspend(c->counter1_handle); // TO DO
        vTaskSuspend(c->counter2_handle); // TO DO
    }
}

void task_manager(void *pvParameters)
{
    app_context_t *c = (app_context_t *)pvParameters;

    if (c == NULL)
    {
        vTaskDelete(NULL);
        return;
    }

    apply_configuration(c);
    set_counters_running(c, false);

    for (;;)
    {
        if (c->start_pause_event.pending)
        {
            c->start_pause_event.pending = false;

            c->system.run_state = (c->system.run_state == SYSTEM_PAUSED) ? SYSTEM_RUNNING : SYSTEM_PAUSED; // TO DO

            set_counters_running(c, c->system.run_state == SYSTEM_RUNNING);

            // ESP_LOGI(TAG, "STATE=%s", run_state_to_string(c->system.run_state));
        }

        /*
         * Igual que en la práctica original:
         * dirección, velocidad y modo se ignoran estando en PAUSE.
         */
        if (c->system.run_state == SYSTEM_RUNNING)
        {

            if (c->direction_event.pending)
            {
                c->direction_event.pending = false;
                c->system.master_direction = (c->system.master_direction == COUNT_DOWN) ? COUNT_UP : COUNT_DOWN;
                apply_configuration(c);
                // ESP_LOGI(TAG, "DIR=%s", direction_to_string(c->system.master_direction));
            }

            if (c->speed_event.pending)
            {
                c->speed_event.pending = false;
                c->system.speed_mode = (c->system.speed_mode == SPEED_SLOW) ? SPEED_FAST : SPEED_SLOW;
                apply_configuration(c);
                // ESP_LOGI(TAG, "SPEED=%s", speed_to_string(c->system.speed_mode));
            }

            if (c->mode_event.pending)
            {
                c->mode_event.pending = false;
                c->system.coupling_mode = (c->system.coupling_mode == COUPLING_SAME) ? COUPLING_OPPOSITE : COUPLING_SAME;
                apply_configuration(c);
                // ESP_LOGI(TAG, "MODE=%s", coupling_to_string(c->system.coupling_mode));
            }
        }
        else
        {
            c->direction_event.pending = false;
            c->speed_event.pending = false;
            c->mode_event.pending = false;
        }

        vTaskDelay(pdMS_TO_TICKS(MANAGER_PERIOD_MS));
    }
}

void create_application_tasks(app_context_t *c)
{
    static button_task_params_t start_params;
    static button_task_params_t direction_params;
    static button_task_params_t speed_params;
    static button_task_params_t mode_params;

    start_params.pin = BTN_START_PAUSE;
    start_params.event = &c->start_pause_event;

    direction_params.pin = BTN_DIRECTION;
    direction_params.event = &c->direction_event;

    speed_params.pin = BTN_SPEED;
    speed_params.event = &c->speed_event;

    mode_params.pin = BTN_MODE;
    mode_params.event = &c->mode_event;

    /*
     * Es la MISMA función que counter_task() --> dos instancias diferentes.
     * pvParameters determina qué estructura controla cada tarea.
     */

    xTaskCreate(counter_task, "Counter1", 1024, &c->counter1, 2, &c->counter1_handle);
    xTaskCreate(counter_task, "Counter2", 1024, &c->counter2, 2, &c->counter2_handle);

    /*
     * Es la misma MISMA función que button_task() --> cuatro botones distintos.
     * pvParameters parametriza el comportamiento.
     */

    xTaskCreate(button_task, "BtnStart", 1024, &start_params, 2, NULL);

    xTaskCreate(button_task, "BtnDirection", 1024, &direction_params, 2, NULL);

    xTaskCreate(button_task, "BtnSpeed", 1024, &speed_params, 2, NULL);

    xTaskCreate(button_task, "BtnMode", 1024, &mode_params, 2, NULL);

    xTaskCreate(task_manager, "TaskManager", 1024, c, 3, &c->manager_handle);

    xTaskCreate(display_refresh_task, "DisplayRefresh", 2048, c, 2, NULL);
}
