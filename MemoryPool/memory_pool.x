#ifndef MEMORY_POOL_X_
#define MEMORY_POOL_X_

typedef struct memoryPool MemoryPool;
typedef struct memoryPoolLarge MemoryPoolLarge;

typedef struct memoryPoolData
{
    S8               *last;
    S8               *end;
    MemoryPool       *next;
    U32               failed;
} MemoryPoolData;

struct memoryPoolLarge
{
    MemoryPoolLarge  *next;
    void             *alloc;
};

struct memoryPool
{
    MemoryPoolData    d;
    U32               max;
    MemoryPool       *current;
    MemoryPoolLarge  *large;
};

EXTERN MemoryPool *CreateMemoryPool(U32 size);

#ifdef UTEST
EXTERN void *(*Malloc)(MemoryPool *pool, U32 size);
#else
EXTERN void *Malloc(MemoryPool *pool, U32 size);
#endif

#ifdef UTEST
EXTERN void *(*Calloc)(MemoryPool *pool, U32 size);
#else
EXTERN void *Calloc(MemoryPool *pool, U32 size);
#endif
EXTERN void *Memalign(MemoryPool *pool, U32 size, U32 align); /* Taken as large memory */

#ifdef UTEST
EXTERN S16 (*Free)(MemoryPool *pool, void *p);
#else
EXTERN S16 Free(MemoryPool *pool, void *p); /* Free large memory */
#endif

EXTERN void ResetMemoryPool(MemoryPool *pool); 
EXTERN void DestroyMemoryPool(MemoryPool *pool); 

#endif /*MEMORY_POOL_X_*/
