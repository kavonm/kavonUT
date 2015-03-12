/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "lottery.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "lottery.x"

/* global variables */
MemoryPool *pool;

PUBLIC S16 Two_In_One(LotteryInput *lotteryInput, LotteryOutput **pLotteryOutput, U16 *numLotteryOutput)
{
    assert(lotteryInput && pLotteryOutput && numLotteryOutput);
    assert(lotteryInput->single >= 1);
    assert(lotteryInput->high >= 1);
    assert(lotteryInput->low >= 1);
    assert(lotteryInput->high > lotteryInput->low);

    FLOAT single = lotteryInput->single;
    FLOAT high   = lotteryInput->high;
    FLOAT low    = lotteryInput->low;
#ifdef DEBUG
    DP("#################################\n");
    DP("single = %f\n", single);
    DP("high = %f\n", high);
    DP("low = %f\n", low);
#endif

	U16   i, index, count;
	FLOAT tmp;
    FLOAT high_ratio, low_ratio;
    FLOAT profit_to_cost_ratio, max_profit_to_cost_ratio = 0;

    U16   cost, min_cost, max_cost;

    FLOAT low_ratio_profit, high_ratio_profit;

    U16   low_ratio_profit_tickets = 0, high_ratio_profit_tickets = 0; 
    U16   low_ratio_tickets, low_ratio_tickets_min, low_ratio_tickets_max; 
    U16   high_ratio_tickets = 1;

	LotteryOutput *lotteryOutput = NULL;

    /* 1. calc the ratios */
    high_ratio = single * high;  /* 1 : high_ratio */
    low_ratio  = single * low;   /* 1 : low_ratio */
#ifdef DEBUG 
    DP("#################################\n");
    DP("high_ratio = 1:%f\n", high_ratio);
    DP("low_ratio = 1:%f\n", low_ratio);
#endif

    high_ratio--;
    low_ratio--;

    /* 2. calc the ticket number */
    if (high_ratio * low_ratio < 1)
    {
        DP("no result!!!!!!!!\n");
        return RFAILED;
    }

	tmp = high_ratio_tickets / low_ratio;
    low_ratio_tickets_min = tmp;
	if (tmp - low_ratio_tickets_min > 0)
	{
		low_ratio_tickets_min++;
	}
    low_ratio_tickets_max = high_ratio * high_ratio_tickets;
#ifdef DEBUG
    DP("#################################\n");
    DP("high_ratio_tickets = %d\n", high_ratio_tickets);
    DP("low_ratio_tickets_min = %d\n", low_ratio_tickets_min);
    DP("low_ratio_tickets_max = %d\n", low_ratio_tickets_max);
#endif

    /* 3. calc the cost */
    min_cost = (high_ratio_tickets + low_ratio_tickets_min) * UNIT_PRICE;
    max_cost = (high_ratio_tickets + low_ratio_tickets_max) * UNIT_PRICE;
#ifdef DEBUG
    DP("#################################\n");
    DP("unit price = %d yuan\n", UNIT_PRICE);
    DP("min_cost = %d yuan\n", min_cost);
    DP("max_cost = %d yuan\n", max_cost);
#endif

    /* 4. calc and print profit list */
#ifdef DEBUG
    DP("#################################\n");
#endif
	count = low_ratio_tickets_max - low_ratio_tickets_min + 1;
	lotteryOutput = (LotteryOutput *)Calloc(pool, count * sizeof(LotteryOutput));
	low_ratio_tickets = low_ratio_tickets_min;
	for (i = 0, low_ratio_tickets = low_ratio_tickets_min; i < count; i++, low_ratio_tickets++)
    {
        cost = (high_ratio_tickets + low_ratio_tickets) * UNIT_PRICE;
#ifdef DEBUG
        DP("cost = %d yuan\n", cost);
#endif

	    /* 4.1 win low ratio - profit */
#if 0 /* Option 1 */
        low_ratio_profit = (low_ratio * low_ratio_tickets - high_ratio_tickets) * UNIT_PRICE;
#else /* Option 2 */
        low_ratio_profit = (low_ratio + 1) * low_ratio_tickets * UNIT_PRICE - cost;
#endif
#ifdef DEBUG
		DP("low_ratio_profit(tickets: %d, %d) = %f yuan (%f%%)\n", high_ratio_tickets, low_ratio_tickets, low_ratio_profit, (low_ratio_profit / cost) * 100);
#endif
		
	    /* 4.2 win high ratio - profit */
#if 0 /* Option 1 */
        high_ratio_profit = (high_ratio * high_ratio_tickets - low_ratio_tickets) * UNIT_PRICE;
#else /* Option 2 */
        high_ratio_profit = (high_ratio + 1) * high_ratio_tickets * UNIT_PRICE - cost;
#endif
#ifdef DEBUG
		DP("high_ratio_profit(tickets: %d, %d) = %f yuan (%f%%)\n\n", high_ratio_tickets, low_ratio_tickets, high_ratio_profit, (high_ratio_profit / cost) * 100);
#endif

		lotteryOutput[i].cost = cost;
		if (low_ratio_profit < high_ratio_profit)
		{
		    lotteryOutput[i].min_profit = low_ratio_profit;
		    lotteryOutput[i].low_win = TRUE;
		    lotteryOutput[i].max_profit = high_ratio_profit;
		}
		else
		{
		    lotteryOutput[i].min_profit = high_ratio_profit;
		    lotteryOutput[i].low_win = FALSE;
		    lotteryOutput[i].max_profit = low_ratio_profit;
		}

		lotteryOutput[i].high_ratio_tickets = high_ratio_tickets;
		lotteryOutput[i].low_ratio_tickets = low_ratio_tickets;
    }

    /* 5. make a better choice */
	for (i = 0; i < count; i++)
	{
        profit_to_cost_ratio = lotteryOutput[i].min_profit / lotteryOutput[i].cost;
		if (profit_to_cost_ratio > max_profit_to_cost_ratio)
		{
			index = i;
			max_profit_to_cost_ratio = profit_to_cost_ratio;
		}
	}

#ifdef DEBUG
    DP("#################################\n");
	DP("cost is %d\n", lotteryOutput[index].cost);
	DP("min_profit is %f\n", lotteryOutput[index].min_profit);
	DP("min_profit/cost is %f%%\n", (lotteryOutput[index].min_profit / lotteryOutput[index].cost) * 100);
	DP("max_profit is %f\n", lotteryOutput[index].max_profit);
	DP("max_profit/cost is %f%%\n", (lotteryOutput[index].max_profit / lotteryOutput[index].cost) * 100);
	DP("low win %d\n", lotteryOutput[index].low_win);
	DP("high ratio tickets are %d\n", lotteryOutput[index].high_ratio_tickets);
	DP("low ratio tickets are %d\n\n", lotteryOutput[index].low_ratio_tickets);
#endif

    *pLotteryOutput = lotteryOutput;
    *numLotteryOutput = count;

    return ROK;
}
