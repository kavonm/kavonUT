#ifndef MOCK_MEM_X_
#define MOCK_MEM_X_

EXTERN void *mock_Malloc(MemoryPool *pool, U32 size);
EXTERN void *mock_Calloc(MemoryPool *pool, U32 size);
EXTERN S16 mock_Free(MemoryPool *pool, void *p);

#endif /*MOCK_MEM_X_*/
