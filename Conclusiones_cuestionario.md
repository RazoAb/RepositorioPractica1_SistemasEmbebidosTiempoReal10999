1. ¿Por qué Counter1 y Counter2 pueden ejecutar la misma función counterTask() y comportarse diferente?
Porque se crean dos instancias de la misma función, pero cada una recibe mediante pvParameters una estructura de configuración diferente: counter1 o counter2.

2. ¿Qué información recibe cada tarea mediante pvParameters?
Cada tarea recibe un puntero a su estructura counter_config_t, que contiene el valor del contador, dirección de conteo, periodo y número de display.

3. ¿Qué representa un TaskHandle_t y por qué el Task Manager necesita conservarlo?
Representa el identificador o referencia de una tarea de FreeRTOS. El Task Manager necesita conservar los handles de Counter1 y Counter2 para poder suspenderlas y reanudarlas mediante vTaskSuspend() y vTaskResume().

4. ¿Qué diferencia existe entre BLOCKED y SUSPENDED en esta práctica?
En el código de esta práctica, las tareas de los contadores se ponen explícitamente en SUSPENDED mediante vTaskSuspend() cuando el sistema está en pausa. En cambio, durante su funcionamiento normal, vTaskDelay() hace que la tarea quede temporalmente BLOCKED durante el periodo establecido.

5. ¿Qué ocurre con vTaskDelay() cuando una tarea es suspendida?
La tarea deja de ejecutarse mientras permanece suspendida. Por lo tanto, aunque counter_task() tenga un vTaskDelay(), este no hace que la tarea continúe ejecutándose mientras está en estado SUSPENDED; primero debe ser reanudada con vTaskResume().

6. ¿Qué ventaja aporta enum class frente a constantes enteras para representar estados?
En este código no se utiliza enum class, sino typedef enum. Los enum permiten representar los estados mediante nombres como SYSTEM_PAUSED, SYSTEM_RUNNING, SPEED_SLOW y SPEED_FAST, haciendo el código más claro que utilizar directamente valores enteros.

7. ¿Qué responsabilidad tiene BcdCounter y cuál SevenSegmentDisplay?
bcd_counter_t representa el valor del contador, mientras que las funciones de counter.c se encargan de inicializarlo y modificarlo según la dirección de conteo.

SevenSegmentDisplay se encarga de la representación física del valor: configura los segmentos y habilita cada display. Además, display_refresh_task() actualiza alternadamente los dos displays.

8. ¿Por qué volatile no resuelve por sí solo los problemas de concurrencia?
Porque volatile solamente indica al compilador que una variable puede cambiar externamente y que sus accesos deben realizarse directamente. No proporciona mecanismos de sincronización ni garantiza operaciones atómicas. En este código, por ejemplo, las variables compartidas están declaradas como volatile, pero no se utilizan mutex, semáforos o queues.

9. ¿Qué cambiaría en el diseño cuando posteriormente se permitan queues o semáforos?
Se podría sustituir parte de la comunicación mediante variables compartidas y eventos pending por mecanismos explícitos de comunicación y sincronización de FreeRTOS. Por ejemplo, las tareas podrían recibir eventos mediante una queue, mientras que los semáforos podrían utilizarse para coordinar el acceso a recursos compartidos. Actualmente los eventos se manejan mediante estructuras con un campo pending.

10. Si ambas tareas tienen la misma prioridad, ¿cómo interviene el scheduler de FreeRTOS?
Las dos tareas Counter1 y Counter2 se crean con prioridad 2. Cuando ambas están listas para ejecutarse, el scheduler de FreeRTOS permite que compartan el tiempo de CPU mediante planificación entre tareas de igual prioridad. Además, en esta práctica ambas pasan periódicamente a estado BLOCKED mediante vTaskDelay(), permitiendo que otras tareas puedan ejecutarse.