/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "digit_map.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "digit_map.x"

/* global variables */
MemoryPool *pool;

#if 0
PUBLIC S16 ValidateDigitMap(S8 *rawDigitMapStr, U8 len)
#else
PUBLIC S16 ValidateDigitMap_real(const S8 *file, U32 line, S8 *rawDigitMapStr, U8 len)
#endif
{
    assert(rawDigitMapStr != NULL);
    assert(len >= 0);

    U8  i, j, start, end;
    S8  ch, ch1, ch2, ch3;

	DP("%s called from %s:%d\n", __FUNCTION__, file, line);

    if (len > 0 && len <= MAX_DIGITMAP_LEN)
    {
#ifdef DEBUG
        DP("original string: %s\n", rawDigitMapStr);
#endif
        for (i=0; i<len; i++)
        {
            ch = rawDigitMapStr[i];
            /* TYPE1 valid charaters: 0-9, a-f, A-F, x, X, [ */
            if (IS_VALID_CHAR1(ch))
            {
                /* case 1: a-f, A-F, x, X */
                if (IS_LOWER_A2F(ch) || IS_LOWER_X(ch))
                {
                    TO_UPPER(rawDigitMapStr[i]);
                }
                /* case 2: [ */
                else if (IS_LEFT_BRACKET(ch))
                {
                    start = i + 1;
                    /* 2.1 search ] */
                    for (j=start; ((j<len) && (!IS_RIGHT_BRACKET(rawDigitMapStr[j]))); j++)
                    {
                        /* inc j to find ] and BTW, upper a-f to A-F */
                        if (IS_LOWER_A2F(rawDigitMapStr[j]))
                        {
                            TO_UPPER(rawDigitMapStr[j]);
                        }
                    }
                    if (j == len)
                    {
#ifdef DEBUG
                        DP("no ] after [\n");
#endif
                        return RFAILED;
                    }
                    else /* ] found */
                    {
                        end = j; /* end points to ']' */
                        if (start == end)
                        {
#ifdef DEBUG
                            DP("empty inside []\n");
#endif
                            return RFAILED;
                        }
                    }

                    /* 2.2 validate the characters between [ and ] */
                    while (start < end)
                    {
                        ch1 = rawDigitMapStr[start];
                        /* TYPE2 valid charaters: 0-9, A-F */
                        if (IS_VALID_CHAR2(ch1))
                        {
                            ch2 = rawDigitMapStr[start+1];
                            ch3 = rawDigitMapStr[start+2];

                            if (IS_SHORT_DASH(ch2))
                            {
                                if (IS_DIGIT_TO_DIGIT(ch1, ch3) ||
                                    IS_A2F_TO_A2F(ch1, ch3))
                                {
                                    start += 2;
                                }
                                else
                                {
#ifdef DEBUG
                                    DP("format error: %c-%c\n", ch1, ch3);
#endif
                                    return RFAILED;
                                }
                            }
                            else if (IS_LOWER_A2F(ch1))
                            {
                                TO_UPPER(rawDigitMapStr[start]);
                            }

                            start++;
                        }
                        else
                        {
#ifdef DEBUG
                            DP("invalid charater inside []: %c\n", ch1);
#endif
                            return RFAILED;
                        }
                    }

                    i = end;
                }
                /* case 3: 0-9 */
                /* else if (IS_NUM(ch)) do nothing */
            }
            else
            {
#ifdef DEBUG
                DP("invalid charater: %c\n", ch);
#endif
                return RFAILED;
            }
        }
#ifdef DEBUG
        DP("converted string: %s\n", rawDigitMapStr);
#endif
    }
    else 
    {
#ifdef DEBUG
        DP("invalid length: %d\n", len);
#endif
        return RFAILED;
    }

    return ROK;
}

PUBLIC S16 TestValidateDigitMap()
{
	U16 i;
    S16 ret = ROK;
    S8  pattern[64] = "1xXB[4-4A-b9][3aC-F]xxx";

	ret = ValidateDigitMap(pattern, strlen(pattern));

    return ret;
}

PUBLIC S16 InitDigitMap(DigitMap *pDigitMap, S8 rawDigitMapStr[][MAX_DIGITMAP_LEN], U16 num)
{
    assert(pDigitMap != NULL);
    assert(rawDigitMapStr != NULL);
    assert(num > 0);

    U16 i;
    S16 ret = ROK;
    U8  len;

    /* Allocate memories for num candidates */
    pDigitMap->numberCandidates = num;
    pDigitMap->maxMatchLength = 0;
#if 0 /* old code */
    pDigitMap->candidates = (DigitMapCandidate *)malloc(pDigitMap->numberCandidates * sizeof(DigitMapCandidate));
    memset(pDigitMap->candidates, 0, pDigitMap->numberCandidates*sizeof(DigitMapCandidate));
#else /* new code */
    pDigitMap->candidates = (DigitMapCandidate *)Calloc(pool, pDigitMap->numberCandidates * sizeof(DigitMapCandidate));
#endif

    /* Initialize num candidates */
    for (i=0; i<pDigitMap->numberCandidates; i++)
    {
        /* Initialize candidate pattern */
        len = strlen(rawDigitMapStr[i]);
        assert(len > 0);
        strncpy(pDigitMap->candidates[i].pattern.val, rawDigitMapStr[i], len);
        pDigitMap->candidates[i].pattern.pres = PRSNT_NODEF;
        pDigitMap->candidates[i].pattern.len = len;

        /* Initialize candidate index */
        pDigitMap->candidates[i].index = 0;

        /* Initialize candidate match length */
        pDigitMap->candidates[i].matchLength = 0;

        /* Initialize candidate state */
        pDigitMap->candidates[i].state = DMS_ACTIVE;
    }

    return ret;
}

PUBLIC S16 CandStateChange(DigitMap *pDigitMap, U16 idx, S16 matchFlag)
{
    assert(pDigitMap != NULL);
    assert(pDigitMap->candidates[idx].state == DMS_ACTIVE || 
           pDigitMap->candidates[idx].state == DMS_PARTIAL_MATCH ||
           pDigitMap->candidates[idx].state == DMS_FULL_MATCH);
    assert(idx >= 0);

    S16 ret             = ROK;
    U8  i;
    DigitMapCandidate    *pCandidate;
    pCandidate          = &(pDigitMap->candidates[idx]);
    U8  scanIndex       = pCandidate->index;
    U8  xFlag           = TRUE;
    
    if (matchFlag == NO_MATCH)
    {
        pCandidate->state = DMS_FAILED_MATCH;
    }
    else /* matchFlag == MATCH */
    {
        pCandidate->matchLength++;
        for (i=scanIndex; i<pCandidate->pattern.len; i++)
        {
            if (IS_NOT_UPPER_X(pCandidate->pattern.val[i]))
            {
                xFlag = FALSE;
                break;
            }
        }

        /* also consider xFlag as TRUE too if the last char matched */
        if (xFlag)
        {
            pCandidate->state = DMS_FULL_MATCH;
            pDigitMap->maxMatchLength = pCandidate->matchLength;
        }
        else
        {
            pCandidate->state = DMS_PARTIAL_MATCH;
        }
    }

    return ret;
}

PUBLIC S16 DigitMapCheck(DigitMap *pDigitMap, U16 idx, S8 digit)
{
    assert(pDigitMap != NULL);
    assert(idx >= 0);
    /* Note: digit is in the range of "0-9" and "A-F" */
    assert(IS_NUM(digit) || IS_UPPER_A2F(digit));

    S16 ret             = ROK;
    S16 matchFlag       = NO_MATCH;
    DigitMapCandidate    *pCandidate;
    pCandidate          = &(pDigitMap->candidates[idx]);
    U8 index            = pCandidate->index;
    S8 ref              = pCandidate->pattern.val[index];
    U8 i, start, end;
    S8 char1, char2, char3;

    /* return directly if already the last char is reached
     * in case that pCandidate->index will be added to exceed */
    if (ref == '\0')
    {
#ifdef DEBUG
        DP("reached the end of candidate pattern\n");
#endif
        return ret;
    }

    /* case 1: 0-9 A-F */
    if (DO_EXACT_MATCH(ref))
    {
        matchFlag = (digit == ref) ? MATCH : NO_MATCH;	
    }
    /* case 2: X */
    else if (DO_ANY_MATCH(ref))
    {
        matchFlag = MATCH;
    }
    /* case 3: [...] - assume the format is correct */
    else if (DO_RANGE_MATCH(ref))
    {
        start = index + 1; /* start the 1st char after '[' */

        for (i=start; !IS_RIGHT_BRACKET(pCandidate->pattern.val[i]); i++)
        {
            /* do nothing but just inc i to find ']' */
        }
        end = i; /* end points to ']' */

        while ((start < end) && (matchFlag == NO_MATCH))
        {
            char1 = pCandidate->pattern.val[start];
            char2 = pCandidate->pattern.val[start+1];
            char3 = pCandidate->pattern.val[start+2];

            /* [digit-digit] */
            if (IS_SHORT_DASH(char2))
            {
                matchFlag = (IN_RANGE(digit, char1, char3)) ? MATCH : NO_MATCH;
                start += 2;
            }
            else /* single exact match */
            {
                matchFlag = (digit == char1) ? MATCH : NO_MATCH;
            }
            start++;
        }
        pCandidate->index = end;
    }
    /* else no exception */

    /* index points to the next character */
    pCandidate->index++;

    /* set states of candidates */
    ret = CandStateChange(pDigitMap, idx, matchFlag);
    if (RFAILED == ret)
    {
#ifdef DEBUG
        DP("CandStateChange Failure!\n");
#endif
    }

    return ret;
}

PUBLIC S8 GetMaxDigitsAndResetDigitMap(DigitMap *pDigitMap)
{
    assert(pDigitMap != NULL);

    U16                  i;
    U8                   j;
    S8                   ref;
    U8                   numFailedMatchCand = 0;
    U8                   numExactDigits;
    U8                   maxExactDigits = 0;
    U8                   numAnyDigits;
    U8                   matchLength;
    S8                   maxDigits = 0;
    DigitMapCandidate    *pCandidate;

    /* Get a better match for maxDigits */
    for (i=0; i<pDigitMap->numberCandidates; i++)
    {
        numExactDigits  = 0;
        numAnyDigits    = 0;
        pCandidate      = &(pDigitMap->candidates[i]);
        if (pCandidate->state == DMS_FULL_MATCH)
        {
            for (j=0; j<pCandidate->index; j++)
            {
                ref = pCandidate->pattern.val[j];
                if (IS_NUM(ref) || IS_UPPER_A2F(ref))
                {
                    numExactDigits++;
				}
                else if (IS_LEFT_BRACKET(ref))
                {
                    while (!IS_RIGHT_BRACKET(pCandidate->pattern.val[j]))
                    {
                        j++;
                    }
                    numExactDigits++;
                }
                else /* X */
                {
                    numAnyDigits++;
                }
            }

			DP("kavon numAnyDigits = %d\n", numAnyDigits);
			DP("kavon numExactDigits = %d\n", numExactDigits);
			DP("kavon pDigitMap->maxMatchLength = %d\n", pDigitMap->maxMatchLength);
            matchLength = numExactDigits + numAnyDigits;
            if (matchLength == pDigitMap->maxMatchLength)
            {
                if (numExactDigits >= maxExactDigits)
                {
                    maxExactDigits = numExactDigits;
                    numAnyDigits += pCandidate->pattern.len - pCandidate->index;
                    maxDigits = maxExactDigits + numAnyDigits;
                }
            }
        }
        else if (pCandidate->state == DMS_FAILED_MATCH)
        {
            numFailedMatchCand++;
        }
        /* else continue */

        /* BTW, reset candidates */
        pCandidate->state = DMS_ACTIVE;
        pCandidate->index = 0;
        pCandidate->matchLength = 0;
    }
    /* BTW, reset pDigitMap */
    pDigitMap->maxMatchLength  = 0;

    /* if all failed to match, no need to check more info */
    if (numFailedMatchCand == pDigitMap->numberCandidates)
    {
#ifdef DEBUG
        DP("no need to check more info\n");
#endif
        maxDigits = NOT_CHECK_INFO;
    }
	
    return maxDigits;
}
