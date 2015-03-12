/*
 * trylock_main.c
 *
 */

/* header include files (.h) */
#include <gen.h>
#include "memory_pool.h"
#include "trylock.h"

/* header/extern include files (.x) */
#include <gen.x>
#include "memory_pool.x"
#include "trylock.x"

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

    /* calling TryLock() */
    Trylock();

    /* destroy memory pool */
    DestroyMemoryPool(pool); 

    return ROK;
}
