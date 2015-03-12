/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"

PUBLIC void *mock_Malloc(MemoryPool *pool, U32 size)
{
	S8 *m = (S8 *)malloc(size*sizeof(S8));
	return (void *)m;
}

PUBLIC void *mock_Calloc(MemoryPool *pool, U32 size)
{
	S8 *m = (S8 *)calloc(size, sizeof(S8));
	return (void *)m;
}

PUBLIC S16 mock_Free(MemoryPool *pool, void *p)
{
	free(p);
	return ROK;
}
