/* [Test group list]                                
 * 1. memoryPoolTestGroup
 *    1.1 CreateMemoryPool
 *    1.2 ResetMemoryPool
 *
 * 2. memoryPoolAllocTestGroup
 *    2.1 Malloc_Small
 *    2.2 Malloc_NewBlock
 *    2.3 Malloc_Large
 *    2.4 Memalign
 *    2.5 Free
 *
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "CppUTestExt/MockSupportPlugin.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

extern "C"                            
{
/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "memoryPoolTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"                
}

/* TEST GROUP 1: MemoryPoolTestGroup */
TEST_GROUP(memoryPoolTestGroup)
{                                            
    MemoryPool *pool;

    void setup()
    {
        pool = NULL;
    }

    void teardown()
    {
        DestroyMemoryPool(pool);
        pool = NULL;
    }
};

/* case 1.1: CreateMemoryPool */
TEST(memoryPoolTestGroup, CreateMemoryPool)
{
    U32 psize = 1024;
    pool = CreateMemoryPool(psize);
    
    CHECK(pool != NULL);
    CHECK_EQUAL(0, (U64)pool % MEMORY_POOL_ALIGNMENT);
    CHECK_EQUAL(pool->d.last, (S8 *)pool + sizeof(MemoryPool));
    CHECK_EQUAL(psize, pool->d.end - (S8 *)pool);
    CHECK(NULL == pool->d.next);
    CHECK_EQUAL(0, pool->d.failed);
    CHECK_EQUAL(psize - sizeof(MemoryPool), pool->max);
    CHECK_EQUAL(pool, pool->current);
    CHECK(NULL == pool->large);
}

/* case 1.2: ResetMemoryPool */
TEST(memoryPoolTestGroup, ResetMemoryPool)
{
    S8              *mem1, *mem2, *mem3;
    U32              psize = 1024;
    U32              smallSize = 256;
    U32              largeSize = 1024;
    U32              alignSize = 512;
    U32              align = 32;
    S16              ret;
    MemoryPool      *p;

    pool = CreateMemoryPool(psize);
    mem1 = (S8 *)Malloc(pool, smallSize);
    mem2 = (S8 *)Memalign(pool, alignSize, align);
    mem3 = (S8 *)Malloc(pool, largeSize);

    ret = Free(pool, (void *)mem3);
    mem3 = NULL;
    CHECK_EQUAL(ROK, ret);

    ResetMemoryPool(pool);

    for (p = pool; p; p = p->d.next)
    {
        if (p == pool)
        {
            CHECK_EQUAL((S8 *)p + sizeof(MemoryPool), p->d.last);
        }
        else
        {
            CHECK_EQUAL((S8 *)p + sizeof(MemoryPoolData), p->d.last);
        }
        CHECK_EQUAL(0, p->d.failed);
    }

    CHECK_EQUAL(pool, pool->current);
    CHECK(NULL == pool->large);
}

/* TEST GROUP 2: MemoryPoolAllocTestGroup */
TEST_GROUP(memoryPoolAllocTestGroup)
{                                            
    U32         psize;
    MemoryPool *pool;

    void setup()
    {
        psize = 1024; 
        pool  = CreateMemoryPool(psize);
    }

    void teardown()
    {
        DestroyMemoryPool(pool);
        pool = NULL;
    }
};

/* case 2.1: Malloc a small memory from pool */
TEST(memoryPoolAllocTestGroup, Malloc_Small)
{
    S8   *mem;
    U32   size = 512;

    mem = (S8 *)Malloc(pool, size);

    CHECK_EQUAL(pool->d.last - size, mem);
    CHECK_EQUAL(psize, pool->d.end - (S8 *)pool);
    CHECK(NULL == pool->d.next);
    CHECK_EQUAL(0, pool->d.failed);
    CHECK_EQUAL(psize - sizeof(MemoryPool), pool->max);
    CHECK_EQUAL(pool, pool->current);
    CHECK(NULL == pool->large);
}

/* case 2.2: Malloc the memory from the new block */
TEST(memoryPoolAllocTestGroup, Malloc_NewBlock)
{
    S8         *mem1, *mem2, *mem3;
    MemoryPool *p;
    U32         size = 512;

    mem1 = (S8 *)Malloc(pool, size);
    mem2 = (S8 *)Malloc(pool, size);
    mem3 = (S8 *)Malloc(pool, size);

    CHECK_EQUAL(pool->d.last - size, mem1);
    CHECK_EQUAL(psize, pool->d.end - (S8 *)pool);
    CHECK(NULL != pool->d.next);
    CHECK_EQUAL(2, pool->d.failed);
    CHECK_EQUAL(psize - sizeof(MemoryPool), pool->max);
    CHECK_EQUAL(pool, pool->current);
    CHECK(NULL == pool->large);

    p = pool->d.next;
    CHECK_EQUAL(p->d.last - size, mem2);
    CHECK_EQUAL(psize, p->d.end - (S8 *)p);
    CHECK(NULL != p->d.next);
    CHECK_EQUAL(1, p->d.failed);

    p = p->d.next;
    CHECK_EQUAL(p->d.last - size, mem3);
    CHECK_EQUAL(psize, p->d.end - (S8 *)p);
    CHECK(NULL == p->d.next);
    CHECK_EQUAL(0, p->d.failed);
}

/* case 2.3: Malloc a large memory from pool */
TEST(memoryPoolAllocTestGroup, Malloc_Large)
{
    S8   *mem;
    U32   size = 1024;

    mem = (S8 *)Malloc(pool, size);

    CHECK_EQUAL(pool->d.last - sizeof(MemoryPoolLarge), (S8 *)pool->large);
    CHECK_EQUAL((S8 *)ALIGN_PTR(MEMORY_ALIGNMENT, (S8 *)pool + sizeof(MemoryPool)), (S8 *)pool->large);
    CHECK_EQUAL((S8 *)pool->large->alloc, mem);
    CHECK(NULL == pool->large->next);
    CHECK_EQUAL(psize, pool->d.end - (S8 *)pool);
    CHECK(NULL == pool->d.next);
    CHECK_EQUAL(0, pool->d.failed);
    CHECK_EQUAL(psize - sizeof(MemoryPool), pool->max);
    CHECK_EQUAL(pool, pool->current);
}

/* case 2.4: Malloc an alignment memory from pool */
TEST(memoryPoolAllocTestGroup, Memalign)
{
    S8   *mem;
    U32   size = 512;
    U32   align = 32;

    mem = (S8 *)Memalign(pool, size, align);

    CHECK_EQUAL(pool->d.last - sizeof(MemoryPoolLarge), (S8 *)pool->large);
    CHECK_EQUAL((S8 *)ALIGN_PTR(MEMORY_ALIGNMENT, (S8 *)pool + sizeof(MemoryPool)), (S8 *)pool->large);
    CHECK_EQUAL((S8 *)pool->large->alloc, mem);
    CHECK_EQUAL(0, (U64)mem % align);
    CHECK(NULL == pool->large->next);
    CHECK_EQUAL(psize, pool->d.end - (S8 *)pool);
    CHECK(NULL == pool->d.next);
    CHECK_EQUAL(0, pool->d.failed);
    CHECK_EQUAL(psize - sizeof(MemoryPool), pool->max);
    CHECK_EQUAL(pool, pool->current);
}

/* case 2.5: Free a specific memory from pool */
TEST(memoryPoolAllocTestGroup, Free)
{
    S8              *mem1, *mem2, *mem3;
    U32              smallSize = 256;
    U32              largeSize = 1024;
    U32              alignSize = 512;
    U32              align = 32;
    S16              ret;
    MemoryPoolLarge *large;

    mem1 = (S8 *)Malloc(pool, smallSize);
    mem2 = (S8 *)Memalign(pool, alignSize, align);
    mem3 = (S8 *)Malloc(pool, largeSize);

    large = pool->large;
    CHECK_EQUAL((S8 *)large->alloc, mem3);
    CHECK_EQUAL((S8 *)large->next->alloc, mem2);

    ret = Free(pool, (void *)mem3);
    mem3 = NULL;

    CHECK_EQUAL(ROK, ret);
    CHECK(NULL == large->alloc);
}
