/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "lottery.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "lottery.x"

#define NUM_INPUT 2

/* local externs */
EXTERN MemoryPool *pool;

S32 main()
{
    S16 ret = ROK;
    U16 m, n, i, j, kavon = 0, lotteryNum[2] = {0};
    FLOAT high_ratio, low_ratio;
    LotteryInput lotteryInput[2];
    LotteryOutput *lotteryOutput[2];

    /* Create a memory pool */
    pool = CreateMemoryPool(MEMORY_POOL_SIZE);
    if (pool == NULL)
    {
		DP("Memory pool failed to be created.\n");
    }

    FLOAT rawData[NUM_INPUT][2] = 
    {
        {1.80, 1.00}, 
        {4.00, 1.31}, 
    };

    for (m = 0; m < NUM_INPUT-1; m++)
    {
        for (n = NUM_INPUT-1; n < NUM_INPUT; n++)
        {
            DP("kavon = %d\n", kavon++);
	        lotteryInput[0].single = rawData[m][0];
	        lotteryInput[0].high = rawData[n][0];
	        lotteryInput[0].low = rawData[n][1];

	        lotteryInput[1].single = rawData[m][1];
	        lotteryInput[1].high = rawData[n][0];
	        lotteryInput[1].low = rawData[n][1];

            for (i = 0; i < 2; i++)
            {
                ret = Two_In_One(&lotteryInput[i], &lotteryOutput[i], &lotteryNum[i]);
                if (ret == RFAILED)
                {
                    DP("stop~~~~\n");
                    return RFAILED;
                }
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
#endif
                    high_ratio = lotteryOutput[0][i].min_profit * lotteryOutput[1][j].min_profit;

#ifdef DEBUG
	                DP("lotteryOutput[0][%d].cost is %d\n", i, lotteryOutput[0][i].cost);
	                DP("lotteryOutput[1][%d].cost is %d\n", j, lotteryOutput[1][j].cost);
#endif
                    low_ratio = lotteryOutput[0][i].cost * lotteryOutput[1][j].cost;

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
    }

    /* Release allocated memory before exit */
    DestroyMemoryPool(pool);

    return ret;
}
