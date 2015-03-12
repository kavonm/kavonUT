/* [Test group list] 
 * 1. TrylockTestGroup
 *    1.1 TrylockTest
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
#include "trylock.h"
#include "trylockTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "trylock.x"                
#include "mock_mem.x"
}

/* TEST GROUP 1: TrylockTestGroup */
TEST_GROUP(TrylockTestGroup)
{
    void setup()
    {
	}

    void teardown()
    {
	}
};

TEST(TrylockTestGroup, TrylockTest)
{
	/* Test Trylock */
    Trylock();
}
