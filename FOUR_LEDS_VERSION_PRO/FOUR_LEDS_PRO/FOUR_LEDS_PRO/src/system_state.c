#include "system_state.h"
#include "app_config.h"

void system_state_init(app_context_t *c)
{
    if (c == NULL)
    {
        return;
    }

    // TO DO: SOME ASSIGMENTS ARE MISSING.

    c->system.run_state = SYSTEM_PAUSED;
    c->system.coupling_mode = COUPLING_OPPOSITE;
    c->system.speed_mode = SPEED_SLOW;
    c->system.master_direction = COUNT_UP;
    
    c->counter1.display_id = 1U;
    c->counter1.value = 0U;
    c->counter1.direction = COUNT_UP;
    c->counter1.period_ms = SLOW_PERIOD_MS;

    c->counter2.value = 9U;
    c->counter2.direction = COUNT_DOWN;
    c->counter2.display_id = 2U;
    c->counter2.period_ms = SLOW_PERIOD_MS;

    c->start_pause_event.pending = false;
    c->start_pause_event.id = BUTTON_START_PAUSE;

    c->direction_event.pending = false;
    c->direction_event.id = BUTTON_DIRECTION;

    c->speed_event.pending = false;
    c->speed_event.id = BUTTON_SPEED;

    c->mode_event.pending = false;
    c->mode_event.id = BUTTON_MODE;

    c->counter1_handle = NULL;
    c->counter2_handle = NULL;
    c->manager_handle = NULL;
}

const char *direction_to_string(count_direction_t d)
{
    return (d == COUNT_UP) ? "UP" : "DOWN";
}

const char *coupling_to_string(coupling_mode_t m)
{
    return (m == COUPLING_SAME) ? "SAME" : "OPPOSITE";
}

const char *run_state_to_string(run_state_t s)
{
    return (s == SYSTEM_RUNNING) ? "RUNNING" : "PAUSED";
}

const char *speed_to_string(speed_mode_t s)
{
    return (s == SPEED_FAST) ? "FAST" : "SLOW";
}
