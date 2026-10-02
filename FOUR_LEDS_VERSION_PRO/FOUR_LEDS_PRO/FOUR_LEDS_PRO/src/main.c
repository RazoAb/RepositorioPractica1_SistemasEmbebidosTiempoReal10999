#include "app_tasks.h"
#include "system_state.h"


void app_main(void)
{

    static app_context_t context;

    system_state_init(&context);

    create_application_tasks(&context);
}
