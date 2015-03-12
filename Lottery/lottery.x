#ifndef LOTTERY_X_
#define LOTTERY_X_

typedef struct lotteryInput
{
    FLOAT  single;
	FLOAT  high;
    FLOAT  low;
} LotteryInput;

typedef struct lotteryOutput
{
    U16    cost;
	FLOAT  min_profit;
	FLOAT  max_profit;
	U16    low_win;

	U16    high_ratio_tickets;
	U16    low_ratio_tickets;
} LotteryOutput;

EXTERN S16 Two_In_One(LotteryInput *lotteryInput, LotteryOutput **pLotteryOutput, U16 *numLotteryOutput);

#endif /*LOTTERY_X_*/
