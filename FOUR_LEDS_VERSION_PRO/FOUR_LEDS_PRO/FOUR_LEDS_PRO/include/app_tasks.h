#ifndef APP_TASKS_H
#define APP_TASKS_H

#include "system_state.h"

void task_manager(void *pvParameters);
void create_application_tasks(app_context_t *context);

#endif
