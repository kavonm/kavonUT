/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "pani.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "pani.x"

/* local externs */
EXTERN MemoryPool *pool;

S32 main()
{
    PsifSipANI *pAccessNetworkInfo;
    SiCgPtyNum *cgPtyNumS;
    S8 *gstnStr = "GSTN; gstn-location=\"12345678\"";
	U8  i;

	pool = CreateMemoryPool(MEMORY_POOL_SIZE);
	if (pool == NULL)
	{
        DP("Memory pool failed to be created.\n");
	}
    pAccessNetworkInfo = (PsifSipANI *)Calloc(pool, sizeof(PsifSipANI));
    pAccessNetworkInfo->pres = PRSNT_NODEF;
    strcpy(pAccessNetworkInfo->aniValue, gstnStr);    

    cgPtyNumS = (SiCgPtyNum *)Calloc(pool, sizeof(cgPtyNumS));
    ccParsePANI(pAccessNetworkInfo, cgPtyNumS);
    if (cgPtyNumS->eh.pres == PRSNT_NODEF)
	{
		DP("Mapped location number is as follows:\n");

	    DP("cgPtyNumS->natAddrInd.pres = %d, ", cgPtyNumS->natAddrInd.pres);
	    DP("cgPtyNumS->natAddrInd.val  = 0x%x\n", cgPtyNumS->natAddrInd.val);

	    DP("cgPtyNumS->oddEven.pres = %d, ", cgPtyNumS->oddEven.pres);
	    DP("cgPtyNumS->oddEven.val  = 0x%x\n", cgPtyNumS->oddEven.val);

	    DP("cgPtyNumS->scrnInd.pres = %d, ", cgPtyNumS->scrnInd.pres);
	    DP("cgPtyNumS->scrnInd.val  = 0x%x\n", cgPtyNumS->scrnInd.val);

	    DP("cgPtyNumS->presRest.pres = %d, ", cgPtyNumS->presRest.pres);
	    DP("cgPtyNumS->presRest.val  = 0x%x\n", cgPtyNumS->presRest.val);

	    DP("cgPtyNumS->numPlan.pres = %d, ", cgPtyNumS->numPlan.pres);
	    DP("cgPtyNumS->numPlan.val  = 0x%x\n", cgPtyNumS->numPlan.val);

	    DP("cgPtyNumS->niInd.pres = %d, ", cgPtyNumS->niInd.pres);
	    DP("cgPtyNumS->niInd.val  = 0x%x\n", cgPtyNumS->niInd.val);

	    DP("cgPtyNumS->addrSig.pres = %d, ", cgPtyNumS->addrSig.pres);
	    DP("cgPtyNumS->addrSig.len  = %d\n", cgPtyNumS->addrSig.len);
		for (i = 0; i < cgPtyNumS->addrSig.len; i++)
		{
	        DP("cgPtyNumS->addrSig.val[%d] = 0x%x\n", i, cgPtyNumS->addrSig.val[i]);
		}
	}

	/* go ahead to rebuild a PANI hdr */
	memset(pAccessNetworkInfo, 0, sizeof(PsifSipANI));
	ccBuildPANI(cgPtyNumS, pAccessNetworkInfo);

	DP("############################################\n");
	DP("Mapped PANI hdr is as follows:\n");
	DP("pAccessNetworkInfo->pres = %d\n", pAccessNetworkInfo->pres);
	DP("pAccessNetworkInfo->aniValue = %s\n", pAccessNetworkInfo->aniValue);

    DestroyMemoryPool(pool);
    return ROK;
}
