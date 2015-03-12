/* [Test group list] 
 * 1. isupCustTestGroup
 *    1.1 readIsupCustTest
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
#include "isup_cust.h"
#include "isupCustTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "isup_cust.x"                
#include "mock_mem.x"                
}

/* TEST GROUP 1: validateDigitMapTestGroup */
TEST_GROUP(isupCustTestGroup)
{
	S16 ret;

    void setup()
    {
	}

    void teardown()
    {
	}
};

TEST(isupCustTestGroup, readIsupCustTest)
{
	/* Test directly */
	ret = read_ISUP_CUST();

	CHECK_EQUAL(ROK, ret);
}

TEST(isupCustTestGroup, readIsupCustATCATest)
{
	/* Test directly */
	ret = read_ISUP_CUST_ATCA();

	CHECK_EQUAL(ROK, ret);
}
