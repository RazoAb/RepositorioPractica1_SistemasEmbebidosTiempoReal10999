#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <stdbool.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef enum 
{
    COUNT_UP = 0,
    COUNT_DOWN
} count_direction_t;

typedef enum 
{
    COUPLING_OPPOSITE = 0,
    COUPLING_SAME
} coupling_mode_t;

typedef enum 
{
    SYSTEM_PAUSED = 0,
    SYSTEM_RUNNING
} run_state_t;

typedef enum 
{
    SPEED_SLOW = 0,
    SPEED_FAST
} speed_mode_t;

typedef enum 
{
    BUTTON_START_PAUSE = 0,
    BUTTON_DIRECTION,
    BUTTON_SPEED,
    BUTTON_MODE
} button_id_t;

typedef struct
{
    count_direction_t direction;
    coupling_mode_t   coupling;
    run_state_t       state;
    speed_mode_t      speed;
    button_id_t       last_button;

} system_config_t;

typedef struct 
{
    volatile uint8_t value;
    volatile count_direction_t direction;
    volatile uint32_t period_ms;
    uint8_t display_id;
} counter_config_t;

typedef struct 
{
    volatile bool pending;
    button_id_t id;
} button_event_t;

typedef struct 
{
    volatile run_state_t run_state;
    volatile count_direction_t master_direction;
    volatile coupling_mode_t coupling_mode;
    volatile speed_mode_t speed_mode;
} system_state_t;

typedef struct 
{
    system_state_t system;

    counter_config_t counter1;
    counter_config_t counter2;

    button_event_t start_pause_event;
    button_event_t direction_event;
    button_event_t speed_event;
    button_event_t mode_event;

    TaskHandle_t counter1_handle;
    TaskHandle_t counter2_handle;
    TaskHandle_t manager_handle;
} app_context_t;

void system_state_init(app_context_t *context);

const char *direction_to_string(count_direction_t direction);
const char *coupling_to_string(coupling_mode_t mode);
const char *run_state_to_string(run_state_t state);
const char *speed_to_string(speed_mode_t speed);

#endif
