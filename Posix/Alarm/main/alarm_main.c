/*
 * alarm_main.c
 *
 * Simple synchronous alarm program. This is used as a
 * reference for progressive examples of asynchronous
 * alarm programs.
 */

/* header include files (.h) */
#include <gen.h>
#include "memory_pool.h"
#include "alarm.h"

/* header/extern include files (.x) */
#include <gen.x>
#include "memory_pool.x"
#include "alarm.x"

/* local externs */
EXTERN MemoryPool *pool;

S32 main()
{
    /* Create a memory pool */
    pool = CreateMemoryPool(MEMORY_POOL_SIZE);
    if (pool == NULL)
    {
		DP("Memory pool failed to be created.\n");
    }

#if 0
    /* synchronous alarm */
    SyncAlarm();

    /* fork alarm */
    ForkAlarm();

    /* thread alarm */
    ThreadAlarm();
	
    /* mutex alarm */
    MutexAlarm();
#endif

    /* cond alarm */
    CondAlarm();

    /* destroy memory pool */
    DestroyMemoryPool(pool);

    return ROK;
}
