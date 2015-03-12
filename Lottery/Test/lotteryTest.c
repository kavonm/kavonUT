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
#include "lottery.h"
#include "lotteryTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "lottery.x"                
#include "mock_mem.x"                
}

/* TEST GROUP: lotteryTestGroup */
TEST_GROUP(lotteryTestGroup)
{
	S16 ret;
    U16 i, j, lotteryNum[2];
    FLOAT high_ratio, low_ratio;
    LotteryInput lotteryInput[2];
    LotteryOutput *lotteryOutput[2];

    void setup()
    {
        UT_PTR_SET(Calloc, mock_Calloc);
	}

    void teardown()
    {
	}
};

/* case 1 */
TEST(lotteryTestGroup, Case1)
{
	lotteryInput[0].single = 2.60;
	lotteryInput[0].high = 5.60;
	lotteryInput[0].low = 1.03;

	lotteryInput[1].single = 1.31;
	lotteryInput[1].high = 5.60;
	lotteryInput[1].low = 1.03;

    /* Test Two_In_One function */
    for (i = 0; i < 2; i++)
    {
        ret = Two_In_One(&lotteryInput[i], &lotteryOutput[i], &lotteryNum[i]);
        CHECK_EQUAL(ROK, ret);
    }

#ifdef DEBUG
    DP("#################################\n");
	DP("lotteryNum[0] is %d\n", lotteryNum[0]);
	DP("lotteryNum[1] is %d\n", lotteryNum[1]);
#endif

    for (i = 0; i < lotteryNum[0]; i++)
    {
        for (j = 0; j < lotteryNum[1]; j++)
        {
#ifdef DEBUG
	        DP("lotteryOutput[0][%d].min_profit is %f\n", i, lotteryOutput[0][i].min_profit);
	        DP("lotteryOutput[1][%d].min_profit is %f\n", j, lotteryOutput[1][j].min_profit);
	        DP("lotteryOutput[0][%d].cost is %d\n", i, lotteryOutput[0][i].cost);
	        DP("lotteryOutput[1][%d].cost is %d\n", j, lotteryOutput[1][j].cost);
#endif

            high_ratio = lotteryOutput[0][i].min_profit * lotteryOutput[1][j].min_profit;
            low_ratio  = lotteryOutput[0][i].cost * lotteryOutput[1][j].cost;

#ifdef DEBUG
	        DP("high_ratio is %f\n", high_ratio);
	        DP("low_ratio is %f\n\n", low_ratio);
#endif

            if (high_ratio > low_ratio)
            {
                DP("#################################\n");
                DP("Perfect!!!!!!!!!!!!!!\n");
	            DP("high_ratio is %f\n", high_ratio);
	            DP("low_ratio is %f\n", low_ratio);
            }
        }
    }
}
