/* [Test group list] 
 * 1. validateDigitMapTestGroup
 *    1.1 ValidateDigitMap_LEGAL
 *    1.2 ValidateDigitMap_ILLEGAL
 *
 * 2. digitMapTestGroup
 *    2.1 DigitMap_MultiMatch
 *    2.2 DigitMap_NoAnyMatch
 *    2.3 DigitMap_NoFullMatch_ButPartialMatch
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
#include "digit_map.h"
#include "digitMapTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "digit_map.x"                
#include "mock_mem.x"                
}

/* TEST GROUP 1: validateDigitMapTestGroup */
TEST_GROUP(validateDigitMapTestGroup)
{
	S16 ret;
	U16 i;

    void setup()
    {
	}

    void teardown()
    {
	}
};

/* case 1: legal pattern cases */
TEST(validateDigitMapTestGroup, ValidateDigitMap_LEGAL)
{
	/* Prepare testing input */
    S8 pattern[NUM_LEGAL_PATTERN_UT][MAX_DIGITMAP_LEN] = {
		"[A-f]xaB[c1-7]",
		"1xXB[4-4A-b9][3aC-F]xxx",
		"a[ab]x"
	};

	for (i=0; i<NUM_LEGAL_PATTERN_UT; i++)
	{
        /* Test ValidateDigitMap */
	    ret = ValidateDigitMap(pattern[i], strlen(pattern[i]));
	    CHECK_EQUAL(ROK, ret);
	}
}

/* case 2: illegal pattern cases */
TEST(validateDigitMapTestGroup, ValidateDigitMap_ILLEGAL)
{
	/* Prepare testing input */
    S8 pattern[NUM_ILLEGAL_PATTERN_UT][MAX_DIGITMAP_LEN] = {
		"2mxxX",
		"B-1Xx",
		"8?xx",
		"4A[]xxx",
		"4A[1-8]]x",
		"4A[[1-8]x",
		"[5-8]x[-]",
		"cx[6-2]",
		"Xcf[2-9-]",
		"Xcf[-2-9]",
		"cx[8-A]",
		"[9-9]8D[x-F]",
		"[B--F]D"
	};

	for (i=0; i<NUM_ILLEGAL_PATTERN_UT; i++)
	{
        /* Test ValidateDigitMap */
	    ret = ValidateDigitMap(pattern[i], strlen(pattern[i]));
	    CHECK_EQUAL(RFAILED, ret);
	}
}

/* case 3: just for test */
TEST(validateDigitMapTestGroup, TestValidateDigitMap_LEGAL)
{
	/* Test directly */
	ret = TestValidateDigitMap();
	CHECK_EQUAL(ROK, ret);
}

/* TEST GROUP 2: digitMapTestGroup */
TEST_GROUP(digitMapTestGroup)
{
    U16       i, j;
    DigitMap *pDigitMap;
    S8        maxDigits;

    void setup()
    {
        pDigitMap = (DigitMap *)malloc(sizeof(DigitMap));
        memset(pDigitMap, 0, sizeof(DigitMap));

        UT_PTR_SET(Calloc, mock_Calloc);
    }

    void teardown()
    {
        free(pDigitMap);
        pDigitMap = NULL;
    }
};

/* case 2.1: Multiple matches */
TEST(digitMapTestGroup, DigitMap_MultiMatch)
{
	/* Prepare testing input */
    S8 rawDigitMapStr[NUM_DIGITMAP_UT][MAX_DIGITMAP_LEN] = {
    	"12XXX",
    	"4A[A-F][3-68]X126XXX",
    	"ABXX01XX[A-F]", 
    	"[13-6]XX89XX[5-9A-F]X",
    	"[4-9]XXXXXXX[B-D]XXXXXX"
    };  
    S8 cdPtyNum[] = "4AB89126C";

    /* Test InitDigitMap */
    InitDigitMap(pDigitMap, rawDigitMapStr, NUM_DIGITMAP_UT);

    CHECK_EQUAL(NUM_DIGITMAP_UT, pDigitMap->numberCandidates);
    for (i=0; i<pDigitMap->numberCandidates; i++)
    {
        CHECK_EQUAL(DMS_ACTIVE, pDigitMap->candidates[i].state);
        CHECK_EQUAL(0, pDigitMap->candidates[i].index);
        
        CHECK_EQUAL(PRSNT_NODEF, pDigitMap->candidates[i].pattern.pres);
        CHECK_EQUAL(strlen(rawDigitMapStr[i]), pDigitMap->candidates[i].pattern.len);
        STRCMP_EQUAL(rawDigitMapStr[i], pDigitMap->candidates[i].pattern.val);
    }

    /* Test DigitMapCheck */
    for (i=0; i<strlen(cdPtyNum); i++)
    {
        for (j=0; j<pDigitMap->numberCandidates; j++)
        {
            if (pDigitMap->candidates[j].state == DMS_ACTIVE ||
                pDigitMap->candidates[j].state == DMS_PARTIAL_MATCH ||
                pDigitMap->candidates[j].state == DMS_FULL_MATCH)
            {
                DigitMapCheck(pDigitMap, j, cdPtyNum[i]);
            }                                                  
        }
    }

	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[0].state);
	CHECK_EQUAL(DMS_FULL_MATCH,   pDigitMap->candidates[1].state);
	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[2].state);
	CHECK_EQUAL(DMS_FULL_MATCH,   pDigitMap->candidates[3].state);
	CHECK_EQUAL(DMS_FULL_MATCH,   pDigitMap->candidates[4].state);
	CHECK_EQUAL(9, pDigitMap->maxMatchLength);

	/* Test GetMaxDigitsAndResetDigitMap */
    maxDigits = GetMaxDigitsAndResetDigitMap(pDigitMap);

    CHECK_EQUAL(11, maxDigits);
}

/* case 2.2: No Any match */
TEST(digitMapTestGroup, DigitMap_NoAnyMatch)
{
	/* Prepare testing input */
    S8 rawDigitMapStr[NUM_DIGITMAP_UT][MAX_DIGITMAP_LEN] = {
        "12XXX",
        "5A[A-F][3-68]X126XXX",
        "ABXX01XX[A-F]",
        "[13-6]XX69XX[5-9A-F]X",
        "[5-9]XXXXXXX[B-D]XXXXXX"
    };  
    S8 cdPtyNum[] = "4AB89126C";

    /* Test InitDigitMap */
    InitDigitMap(pDigitMap, rawDigitMapStr, NUM_DIGITMAP_UT);

    CHECK_EQUAL(NUM_DIGITMAP_UT, pDigitMap->numberCandidates);
    for (i=0; i<pDigitMap->numberCandidates; i++)
    {
        CHECK_EQUAL(DMS_ACTIVE, pDigitMap->candidates[i].state);
        CHECK_EQUAL(0, pDigitMap->candidates[i].index);
        
        CHECK_EQUAL(PRSNT_NODEF, pDigitMap->candidates[i].pattern.pres);
        CHECK_EQUAL(strlen(rawDigitMapStr[i]), pDigitMap->candidates[i].pattern.len);
        STRCMP_EQUAL(rawDigitMapStr[i], pDigitMap->candidates[i].pattern.val);
    }

    /* Test DigitMapCheck */
    for (i=0; i<strlen(cdPtyNum); i++)
    {
        for (j=0; j<pDigitMap->numberCandidates; j++)
        {
            if (pDigitMap->candidates[j].state == DMS_ACTIVE ||
                pDigitMap->candidates[j].state == DMS_PARTIAL_MATCH ||
                pDigitMap->candidates[j].state == DMS_FULL_MATCH)
            {
                DigitMapCheck(pDigitMap, j, cdPtyNum[i]);
            }                                                  
        }
    }

	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[0].state);
	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[1].state);
	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[2].state);
	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[3].state);
	CHECK_EQUAL(DMS_FAILED_MATCH, pDigitMap->candidates[4].state);
	CHECK_EQUAL(0, pDigitMap->maxMatchLength);

	/* Test GetMaxDigitsAndResetDigitMap */
    maxDigits = GetMaxDigitsAndResetDigitMap(pDigitMap);

    CHECK_EQUAL(NOT_CHECK_INFO, maxDigits);
}

/* case 2.3: No full match but partial match */
TEST(digitMapTestGroup, DigitMap_NoFullMatch_ButPartialMatch)
{
	/* Prepare testing input */
    S8 rawDigitMapStr[NUM_DIGITMAP_UT][MAX_DIGITMAP_LEN] = {
    	"12XXX",
    	"4A[A-F][3-68]X126B90",
    	"ABXX01XX[A-F]", 
    	"4XX89XX[5-9A-F]0",
    	"[3-9]XXXXXXX0"
    };  
    S8 cdPtyNum[] = "4AB89126";

    /* Test InitDigitMap */
    InitDigitMap(pDigitMap, rawDigitMapStr, NUM_DIGITMAP_UT);

    CHECK_EQUAL(NUM_DIGITMAP_UT, pDigitMap->numberCandidates);
    for (i=0; i<pDigitMap->numberCandidates; i++)
    {
        CHECK_EQUAL(DMS_ACTIVE, pDigitMap->candidates[i].state);
        CHECK_EQUAL(0, pDigitMap->candidates[i].index);
        
        CHECK_EQUAL(PRSNT_NODEF, pDigitMap->candidates[i].pattern.pres);
        CHECK_EQUAL(strlen(rawDigitMapStr[i]), pDigitMap->candidates[i].pattern.len);
        STRCMP_EQUAL(rawDigitMapStr[i], pDigitMap->candidates[i].pattern.val);
    }

    /* Test DigitMapCheck */
    for (i=0; i<strlen(cdPtyNum); i++)
    {
        for (j=0; j<pDigitMap->numberCandidates; j++)
        {
            if (pDigitMap->candidates[j].state == DMS_ACTIVE ||
                pDigitMap->candidates[j].state == DMS_PARTIAL_MATCH ||
                pDigitMap->candidates[j].state == DMS_FULL_MATCH)
            {
                DigitMapCheck(pDigitMap, j, cdPtyNum[i]);
            }                                                  
        }
    }

	CHECK_EQUAL(DMS_FAILED_MATCH,  pDigitMap->candidates[0].state);
	CHECK_EQUAL(DMS_PARTIAL_MATCH, pDigitMap->candidates[1].state);
	CHECK_EQUAL(DMS_FAILED_MATCH,  pDigitMap->candidates[2].state);
	CHECK_EQUAL(DMS_PARTIAL_MATCH, pDigitMap->candidates[3].state);
	CHECK_EQUAL(DMS_PARTIAL_MATCH, pDigitMap->candidates[4].state);
	CHECK_EQUAL(0, pDigitMap->maxMatchLength);

	/* Test GetMaxDigitsAndResetDigitMap */
    maxDigits = GetMaxDigitsAndResetDigitMap(pDigitMap);

    CHECK_EQUAL(0, maxDigits);
}

/* case 2.4: test rainy cases */
TEST(digitMapTestGroup, DigitMap_Rainy_Cases)
{
	/* Prepare testing input */
    S8 rawDigitMapStr[2][MAX_DIGITMAP_LEN] = {
    	"123X",
    	"1XXXXXX"
    };  
    S8 cdPtyNum[] = "12345";

    /* Test InitDigitMap */
    InitDigitMap(pDigitMap, rawDigitMapStr, 2);

    CHECK_EQUAL(2, pDigitMap->numberCandidates);
    for (i=0; i<pDigitMap->numberCandidates; i++)
    {
        CHECK_EQUAL(DMS_ACTIVE, pDigitMap->candidates[i].state);
        CHECK_EQUAL(0, pDigitMap->candidates[i].index);
        
        CHECK_EQUAL(PRSNT_NODEF, pDigitMap->candidates[i].pattern.pres);
        CHECK_EQUAL(strlen(rawDigitMapStr[i]), pDigitMap->candidates[i].pattern.len);
        STRCMP_EQUAL(rawDigitMapStr[i], pDigitMap->candidates[i].pattern.val);
    }

    /* Test DigitMapCheck */
    for (i=0; i<strlen(cdPtyNum); i++)
    {
        for (j=0; j<pDigitMap->numberCandidates; j++)
        {
            if (pDigitMap->candidates[j].state == DMS_ACTIVE ||
                pDigitMap->candidates[j].state == DMS_PARTIAL_MATCH ||
                pDigitMap->candidates[j].state == DMS_FULL_MATCH)
            {
                DigitMapCheck(pDigitMap, j, cdPtyNum[i]);
            }                                                  
        }
    }

	CHECK_EQUAL(DMS_FULL_MATCH,  pDigitMap->candidates[0].state);
	CHECK_EQUAL(DMS_FULL_MATCH,  pDigitMap->candidates[1].state);
	CHECK_EQUAL(5, pDigitMap->maxMatchLength);
	CHECK_EQUAL(4, pDigitMap->candidates[0].index);
	CHECK_EQUAL(5, pDigitMap->candidates[1].index);

	/* Test GetMaxDigitsAndResetDigitMap */
    maxDigits = GetMaxDigitsAndResetDigitMap(pDigitMap);

    CHECK_EQUAL(7, maxDigits);
}
