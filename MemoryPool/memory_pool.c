/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"

PRIVATE void *AllocBlock(MemoryPool *pool, U32 size);
PRIVATE void *AllocLarge(MemoryPool *pool, U32 size);

/* Create a new memory pool */
PUBLIC MemoryPool *CreateMemoryPool(U32 size)
{
    MemoryPool *pool;

    pool = (MemoryPool *)memalign(MEMORY_POOL_ALIGNMENT, size);
    if (pool == NULL)
    {
        return NULL;
    }

    /* initialize memory pool data */
    pool->d.last = (S8 *)pool + sizeof(MemoryPool);
    pool->d.end  = (S8 *)pool + size;
    pool->d.next = NULL;
    pool->d.failed = 0;

    /* initialize the others of memory pool */
    size -= sizeof(MemoryPool);
    pool->max = (size < MAX_MEMORY_POOL_ALLOC) ? size : MAX_MEMORY_POOL_ALLOC;

    pool->current = pool;
    pool->large = NULL;

    return pool;
}

/* Reset the memory pool */
PUBLIC void ResetMemoryPool(MemoryPool *pool)
{
    MemoryPool       *p;
    MemoryPoolLarge  *large;

    /* 1st is an exception */
    for (p = pool; p; p = p->d.next)
    {
        p->d.last = (S8 *)p + sizeof(MemoryPoolData);
        p->d.failed = 0;
    }
    pool->d.last = (S8 *)pool + sizeof(MemoryPool);
    
    pool->current = pool;
    for (large = pool->large; large; large = large->next)
    {
        if (large->alloc)
        {
            free(large->alloc);
            large->alloc = NULL;
        }
    }
    pool->large = NULL;
}

/* Destroy the memory pool */
PUBLIC void DestroyMemoryPool(MemoryPool *pool)
{
    MemoryPool       *p, *n;
    MemoryPoolLarge  *large;

    for (large = pool->large; large; large = large->next)
    {
        if (large->alloc)
        {
            free(large->alloc);
        }
    }

	p = pool;
	while (p)
	{
        n = p->d.next;
        free(p);
        p = n;
	}
}

/* Allocate a new block from pool */
PRIVATE void *AllocBlock(MemoryPool *pool, U32 size)
{
    S8    *m;
    U32   psize;
    MemoryPool *new, *current, *p, *prev;

    psize = (U32)(pool->d.end - (S8 *)pool);
    m     = (S8 *)memalign(MEMORY_POOL_ALIGNMENT, psize);
    if (m == NULL)
    {
        return NULL;
    }

    new = (MemoryPool *)m;
    new->d.end    = m + psize;
    new->d.next   = NULL;
    new->d.failed = 0;
    
    m += sizeof(MemoryPoolData);
    /* Align the start address by default */
    m  = (S8 *)ALIGN_PTR(MEMORY_ALIGNMENT, m);
    new->d.last = m + size;

    current = pool->current;
    for (p = current; p; p = p->d.next)
    {
		prev = p;
        if (p->d.failed++ > 4)
        {
            current = p->d.next;
        }
    }

    prev->d.next = new;
    pool->current = current ? current : new;

    return (void *)m;
}

/* Allocate external large memory */
PRIVATE void *AllocLarge(MemoryPool *pool, U32 size)
{
	S8 *m;
	MemoryPoolLarge *large;

	m = (S8 *)malloc(size);
	if (m == NULL)
	{
        return NULL;
	}
    
	large = (MemoryPoolLarge *)Malloc(pool, sizeof(MemoryPoolLarge));
	if (large == NULL)
	{
        free(m);
        return NULL;
	}

    large->alloc = (void *)m;
	large->next  = pool->large;
	pool->large  = large;

	return (void *)m;
}

/* Mocking malloc function */
#ifdef UTEST
PUBLIC void *Malloc_impl(MemoryPool *pool, U32 size)
#else
PUBLIC void *Malloc(MemoryPool *pool, U32 size)
#endif
{
    S8         *m;
    MemoryPool *p;

    if (size <= pool->max)
    {
        p = pool->current;

        while (p)
        {
            /* Align the start address by default */
            m = (S8 *)ALIGN_PTR(MEMORY_ALIGNMENT, p->d.last);

            if (size <= (U32)(p->d.end - m))
            {
                /* case 1: from current pool */
                p->d.last = m + size; 
                return (void *)m;
            }

            p = p->d.next;
        }

        /* case 2: from a new block */
        return AllocBlock(pool, size);
    }

    /* case 3: from external large memory */
    return AllocLarge(pool, size);
}

#ifdef UTEST
PUBLIC void *(*Malloc)(MemoryPool *pool, U32 size) = Malloc_impl;
#endif

/* Mocking calloc function */
#ifdef UTEST
PUBLIC void *Calloc_impl(MemoryPool *pool, U32 size)
#else
PUBLIC void *Calloc(MemoryPool *pool, U32 size)
#endif
{
    S8 *m;

    m = (S8 *)Malloc(pool, size);
    if (m)
    {
        memset(m, 0, size);
    }

    return (void *)m;
}

#ifdef UTEST
PUBLIC void *(*Calloc)(MemoryPool *pool, U32 size) = Calloc_impl;
#endif

/* Mocking memalign function */
PUBLIC void *Memalign(MemoryPool *pool, U32 size, U32 align)
{
    S8 *m;
    MemoryPoolLarge *large;

    m = (S8 *)memalign(align, size);
    if (m == NULL)
    {
        return NULL;
    }

    large = (MemoryPoolLarge *)Malloc(pool, sizeof(MemoryPoolLarge));
    if (large == NULL)
    {
        free(m);
        return NULL;
    }

    large->alloc = (void *)m;
    large->next  = pool->large;
    pool->large  = large;

    return (void *)m;
}

/* Mocking free function */
#ifdef UTEST
PUBLIC S16 Free_impl(MemoryPool *pool, void *p)
#else
PUBLIC S16 Free(MemoryPool *pool, void *p)
#endif
{
    MemoryPoolLarge *large;

    for (large = pool->large; large; large = large->next)
    {
        if (large->alloc == p)
        {
            free(large->alloc);
            large->alloc = NULL;

            return ROK;
        }
    }

    return RFAILED;
}

#ifdef UTEST
PUBLIC S16 (*Free)(MemoryPool *pool, void *p) = Free_impl;
#endif
