/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "digit_map.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "digit_map.x"

/* local externs */
EXTERN MemoryPool *pool;

S8 rawDigitMapStr[NUM_DIGITMAP][MAX_DIGITMAP_LEN] = {
    "12XXX", 
    "34XX[1-9]", 
    "ABXX01XX[A-F]", 
    "[13-6]XX89XX[5-9A-F]X"
};

S32 main()
{
    U16         i, j;
    S16         ret         = ROK;
    S8          *cdPtyNum    = "4AB89125";
    DigitMap    *pDigitMap;
	S8          maxDigits;

#if KAVON
	printf("kavon defined\n");
#else
	printf("kavon not defined\n");
#endif

#ifdef KAVON
	printf("kavon2 defined\n");
#else
	printf("kavon2 not defined\n");
#endif
    /* Create a memory pool */
    pool = CreateMemoryPool(MEMORY_POOL_SIZE);
    if (pool == NULL)
    {
		DP("Memory pool failed to be created.\n");
    }

#if 0 /* old code */
    pDigitMap = (DigitMap *)malloc(sizeof(DigitMap));
    memset(pDigitMap, 0, sizeof(DigitMap));
#else /* new code */
    pDigitMap = (DigitMap *)Calloc(pool, sizeof(DigitMap));
#endif

    /* Initialize pDigitMap */
    ret = InitDigitMap(pDigitMap, rawDigitMapStr, NUM_DIGITMAP);
    if (ROK == ret)
    {
#ifdef DEBUG
    	DP("InitDigitMap Success!\n");
#endif
    }
    else
    {
#ifdef DEBUG
        DP("InitDigitMap Failure!\n");
#endif
        /* Assume that candidates in pDigitMap not allocated memory or allocated failed */
#if 0 /* old code */
        free(pDigitMap);
#else /* new code */
		DestroyMemoryPool(pool);
#endif
        return ret;
    }

    /* Update state of candidates */
    for (i=0; i<strlen(cdPtyNum); i++)
    {
        for (j=0; j<pDigitMap->numberCandidates; j++)
        {
            if (pDigitMap->candidates[j].state == DMS_ACTIVE ||
            	pDigitMap->candidates[j].state == DMS_PARTIAL_MATCH ||
            	pDigitMap->candidates[j].state == DMS_FULL_MATCH)
            {
                ret = DigitMapCheck(pDigitMap, j, cdPtyNum[i]);
                if (ROK != ret)
                {
#ifdef DEBUG
                    DP("DigitMapCheck Failure!\n");
#endif
		            DestroyMemoryPool(pool);
                    return ret;
                }
            }
        }
    }

#ifdef DEBUG
    for (j=0; j<pDigitMap->numberCandidates; j++)
	{
		DP("pDigitMap->candidates[%d].state = %d\n", j, pDigitMap->candidates[j].state);
	}
#endif

	/* Get maxDigits */
	maxDigits = GetMaxDigitsAndResetDigitMap(pDigitMap);
#ifdef DEBUG
	DP("maxDigits = %d\n", maxDigits);
#endif

    /* Release allocated memory before exit */
#if 0 /* old code */
    free(pDigitMap->candidates);
    pDigitMap->candidates = NULL;
    free(pDigitMap);
    pDigitMap = NULL;
#else /* new code */
    DestroyMemoryPool(pool);
#endif

    return ret;
}
