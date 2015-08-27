/* [Test group list] 
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
#include "pani.h"
#include "paniTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "pani.x"                
#include "mock_mem.x"                
}

TEST_GROUP(paniTestGroup)
{
	S16 ret;
    PsifSipANI *pAccessNetworkInfo;
    SiCgPtyNum *cgPtyNumS;
    S8 *gstnStr;

    void setup()
    {
        gstnStr = NULL;
        pAccessNetworkInfo = (PsifSipANI *)malloc(sizeof(PsifSipANI));
        memset(pAccessNetworkInfo, 0, sizeof(PsifSipANI));

        cgPtyNumS = (SiCgPtyNum *)malloc(sizeof(SiCgPtyNum));
        memset(cgPtyNumS, 0, sizeof(SiCgPtyNum));
	}

    void teardown()
    {
        free(pAccessNetworkInfo);
        pAccessNetworkInfo = NULL;

        free(cgPtyNumS);
        cgPtyNumS = NULL;
	}
};

TEST(paniTestGroup, ccParsePANI1)
{
    gstnStr = "GSTN; gstn-location=\"923456789\"";
    pAccessNetworkInfo->pres = PRSNT_NODEF;
    strcpy(pAccessNetworkInfo->aniValue, gstnStr);
	ret = ccParsePANIForGstnLoc(pAccessNetworkInfo, cgPtyNumS);

	CHECK_EQUAL(ROK, ret);
	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->eh.pres);
	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->natAddrInd.pres);
	CHECK_EQUAL(0x12, cgPtyNumS->natAddrInd.val);

	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->oddEven.pres);
	CHECK_EQUAL(1, cgPtyNumS->oddEven.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->scrnInd.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->scrnInd.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->presRest.pres);
    CHECK_EQUAL(0x01, cgPtyNumS->presRest.val);
    
    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->numPlan.pres);
    CHECK_EQUAL(0x03, cgPtyNumS->numPlan.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->niInd.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->niInd.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->addrSig.pres);
    CHECK_EQUAL(0x03, cgPtyNumS->addrSig.len);
    CHECK_EQUAL(0x65, cgPtyNumS->addrSig.val[0]);
    CHECK_EQUAL(0x87, cgPtyNumS->addrSig.val[1]);
    CHECK_EQUAL(0x09, cgPtyNumS->addrSig.val[2]);
}

TEST(paniTestGroup, ccParsePANI2)
{
    gstnStr = "GSTN; gstn-location=\"12345678\"";
    pAccessNetworkInfo->pres = PRSNT_NODEF;
    strcpy(pAccessNetworkInfo->aniValue, gstnStr);
	ret = ccParsePANIForGstnLoc(pAccessNetworkInfo, cgPtyNumS);

	CHECK_EQUAL(ROK, ret);
	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->eh.pres);
	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->natAddrInd.pres);
	CHECK_EQUAL(0x12, cgPtyNumS->natAddrInd.val);

	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->oddEven.pres);
	CHECK_EQUAL(0x00, cgPtyNumS->oddEven.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->scrnInd.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->scrnInd.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->presRest.pres);
    CHECK_EQUAL(0x01, cgPtyNumS->presRest.val);
    
    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->numPlan.pres);
    CHECK_EQUAL(0x03, cgPtyNumS->numPlan.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->niInd.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->niInd.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->addrSig.pres);
    CHECK_EQUAL(0x02, cgPtyNumS->addrSig.len);
    CHECK_EQUAL(0x65, cgPtyNumS->addrSig.val[0]);
    CHECK_EQUAL(0x87, cgPtyNumS->addrSig.val[1]);
}

TEST(paniTestGroup, ccParsePANI3)
{
    gstnStr = "GSTN; operator-specific-GI=\"56789\"";
    pAccessNetworkInfo->pres = PRSNT_NODEF;
    strcpy(pAccessNetworkInfo->aniValue, gstnStr);
	ret = ccParsePANIForOperSpecCgi(pAccessNetworkInfo, cgPtyNumS);

	CHECK_EQUAL(ROK, ret);
	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->eh.pres);
	CHECK_EQUAL(NOTPRSNT, cgPtyNumS->natAddrInd.pres);
	CHECK_EQUAL(0x00, cgPtyNumS->natAddrInd.val);

	CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->oddEven.pres);
	CHECK_EQUAL(1, cgPtyNumS->oddEven.val);

    CHECK_EQUAL(NOTPRSNT, cgPtyNumS->scrnInd.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->scrnInd.val);

    CHECK_EQUAL(NOTPRSNT, cgPtyNumS->presRest.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->presRest.val);
    
    CHECK_EQUAL(NOTPRSNT, cgPtyNumS->numPlan.pres);
    CHECK_EQUAL(0x00, cgPtyNumS->numPlan.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->niInd.pres);
    CHECK_EQUAL(0x01, cgPtyNumS->niInd.val);

    CHECK_EQUAL(PRSNT_NODEF, cgPtyNumS->addrSig.pres);
    CHECK_EQUAL(0x03, cgPtyNumS->addrSig.len);
    CHECK_EQUAL(0x65, cgPtyNumS->addrSig.val[0]);
    CHECK_EQUAL(0x87, cgPtyNumS->addrSig.val[1]);
    CHECK_EQUAL(0x09, cgPtyNumS->addrSig.val[2]);
}

TEST(paniTestGroup, ccBuildPANI1)
{
    cgPtyNumS->eh.pres = PRSNT_NODEF;
	cgPtyNumS->natAddrInd.pres = PRSNT_NODEF;
	cgPtyNumS->natAddrInd.val = 0x12;

	cgPtyNumS->oddEven.pres = PRSNT_NODEF;
	cgPtyNumS->oddEven.val = 0x01;

    cgPtyNumS->scrnInd.pres = PRSNT_NODEF;
    cgPtyNumS->scrnInd.val = 0x00;

    cgPtyNumS->presRest.pres = PRSNT_NODEF;
    cgPtyNumS->presRest.val = 0x01;
    
    cgPtyNumS->numPlan.pres = PRSNT_NODEF;
    cgPtyNumS->numPlan.val = 0x03;

    cgPtyNumS->niInd.pres = PRSNT_NODEF;
    cgPtyNumS->niInd.val = 0x00;

    cgPtyNumS->addrSig.pres = PRSNT_NODEF;
    cgPtyNumS->addrSig.len = 0x03;
    cgPtyNumS->addrSig.val[0] = 0x65;
    cgPtyNumS->addrSig.val[1] = 0x87;
    cgPtyNumS->addrSig.val[2] = 0x09;

	ret = ccBuildPANI(cgPtyNumS, pAccessNetworkInfo);

	CHECK_EQUAL(ROK, ret);
    CHECK_EQUAL(PRSNT_NODEF, pAccessNetworkInfo->pres);
	DP("pAccessNetworkInfo->aniValue = %s\n", pAccessNetworkInfo->aniValue);
}

TEST(paniTestGroup, ccBuildPANI2)
{
    cgPtyNumS->eh.pres = PRSNT_NODEF;
	cgPtyNumS->natAddrInd.pres = PRSNT_NODEF;
	cgPtyNumS->natAddrInd.val = 0x12;

	cgPtyNumS->oddEven.pres = PRSNT_NODEF;
	cgPtyNumS->oddEven.val = 0x00;

    cgPtyNumS->scrnInd.pres = PRSNT_NODEF;
    cgPtyNumS->scrnInd.val = 0x00;

    cgPtyNumS->presRest.pres = PRSNT_NODEF;
    cgPtyNumS->presRest.val = 0x01;
    
    cgPtyNumS->numPlan.pres = PRSNT_NODEF;
    cgPtyNumS->numPlan.val = 0x03;

    cgPtyNumS->niInd.pres = PRSNT_NODEF;
    cgPtyNumS->niInd.val = 0x00;

    cgPtyNumS->addrSig.pres = PRSNT_NODEF;
    cgPtyNumS->addrSig.len = 0x02;
    cgPtyNumS->addrSig.val[0] = 0x65;
    cgPtyNumS->addrSig.val[1] = 0x87;

	ret = ccBuildPANI(cgPtyNumS, pAccessNetworkInfo);

	CHECK_EQUAL(ROK, ret);
    CHECK_EQUAL(PRSNT_NODEF, pAccessNetworkInfo->pres);
	DP("pAccessNetworkInfo->aniValue = %s\n", pAccessNetworkInfo->aniValue);
}
