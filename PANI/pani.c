/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "pani.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "pani.x"

/* global variables */
MemoryPool *pool;

PUBLIC U8 ccAscToHex(S8 asc)   
{
    U8 hex;

    if (asc >= 'A' && asc <= 'F')
    {
        hex = (asc - 'A') + 10 ;  
    }
    else 
    {
        hex = asc - '0';
    }

    return hex;
}

PUBLIC S8 ccHexToAsc(U8 hex)   
{
    S8 asc;

    if (hex >= 10 && hex <= 15)
    {
        asc = (hex - 10) + 'A' ;  
    }
    else 
    {
        asc = hex + '0';
    }

    return asc;
}

PUBLIC S16 ccAsciiToBcd(S8 *srcP, U8 srcLen, U8 *dstP, U8 *dstLen)   
{
    U8  i;                                                                            
    U8  bcdCount = 0;
                                                                                     
    /* Go through the source buffer looking for ascii digits */                      
    for (i = 0; i < srcLen; i++, srcP++)
    {                                                                                
        if ((*srcP >= '0') && (*srcP <= '9')) /* Numerical: 0-9 */                   
        {                                                                            
            if (++bcdCount & 1)  /* odd digit count */                               
            {
               *dstP = (*srcP - '0');
            }
            else               /* even digit count */                                
            {
               *dstP++ |= (S8) ((*srcP - '0') << 4);
            }
        }                                                                            
        else if ((*srcP >= 'A') && (*srcP <= 'F')) /* Over decimal digit */
        {                                                                            
            if (++bcdCount & 1)  /* odd digit count */                               
            {
               *dstP = (*srcP - 'A' + 10);                            
            }
            else               /* even digit count */                                
            {
               *dstP++ |= (S8) ((*srcP - 'A' + 10) << 4);
            }
        }                                                                            
        else                                                                         
        {                                                                            
            DP("ccAsciiToBcd: Invalid character\n");  
            return RFAILED;                                                             
        }                                                                             
    }                                                                                 
                                                                                      
    if (!bcdCount)                                                                    
    {                                                                                 
        DP("ccAsciiToBcd: NO ascii digits found in src buffer\n");  
        return RFAILED;                                                             
    }

    /* return the number of bytes in the dest buffer */
    *dstLen = bcdCount / 2;
    if (bcdCount & 1)
    {
        *dstLen = *dstLen + 1;
    }
                                                                                      
    return ROK;                                                             
}

PUBLIC S16 ccBcdToAscii(S8 *srcP, U8 srcLen, U8 *dstP, U8 *dstLen, U8 oddEven)
{
    U8  i;
    U8  bcdCount = 0;
    S8  *d = dstP;
    U8  dec;

    if((srcLen == 0) && (oddEven == 1))
    {
        DP("ccBcdToAscii: srcLen=0 and oddEven=1 return RFAILED\n");
        return RFAILED;
    }

    /* determine the number of BCD digits to convert ascii */
    bcdCount = srcLen*2;

    if (oddEven == 1)  /* odd */
    {
        bcdCount--;
    } 

	*dstLen= bcdCount;

    /* now go through the bcd digits 1 at a time and convert */
    for (i = 1; i <= bcdCount; i++)
    {
       if (i & 1)  /* odd (low order) digit */
       {
          dec = (*srcP & 0x0f);
       }
       else        /* even (high order) digit */
       {
          dec = ((*srcP++ >> 4) & 0x0f);
       }

       /* convert bcd to ascii */
       if ((dec > 9) && (dec <= 15))
       {
           *dstP++ = 'A' + (dec - 10); /* convert A-F to ascii */
       }
       else
       {
           *dstP++ = '0' + dec;  /* convert 0-9 to ascii */
       }
    }

    return ROK;
}

/* TODO replace number with MACRO */
PUBLIC S16 ccParsePANIForGstnLoc(PsifSipANI *pAccessNetworkInfo, SiCgPtyNum *cgPtyNumS)
{
    if ((pAccessNetworkInfo == NULL) && (cgPtyNumS == NULL))
    {
    	DP("NULL Pointer\n");
    	return RFAILED;
    }

    if (pAccessNetworkInfo->pres == NOTPRSNT)
    {
    	DP("No P-Access-Network-Info hdr\n");
    	return RFAILED;
    }

    S32 ret;
    S8 *aniValue = pAccessNetworkInfo->aniValue; 
    U8  aniLen  = strlen(aniValue);
    S8 *gstnStr = NULL;
	S8  paramVal[32];
	U8  paramSize = 32;
	U8  ch;
	U8  i = 0;

    if (aniLen == 0)
    {
        DP("No Content in P-Access-Network-Info hdr\n");
        return RFAILED;
    }

    gstnStr = strstr(aniValue, "GSTN;");
    if (gstnStr == NULL)
	{
        DP("No access-type GSTN in P-Access-Network-Info hdr\n");
        return RFAILED;
	}

    DP("gstnStr is %s\n", gstnStr);

	gstnStr += strlen("GSTN;");

	CC_CLR_LEADING_LWS(gstnStr);

	if (gstnStr >= aniValue + aniLen)
	{
        DP("No parameter value\n");
        return RFAILED;
	}

    if (strncmp(gstnStr, "gstn-location", strlen("gstn-location")) != 0)
	{
        DP("No gstn-location access info\n");
        return RFAILED;
	}

	gstnStr += strlen("gstn-location");

	CC_CLR_LEADING_LWS(gstnStr);

	if (gstnStr >= aniValue + aniLen)
	{
        DP("No parameter value\n");
        return RFAILED;
	}

	if (*gstnStr != CC_ASCII_EQUAL)
	{
        DP("%c is not \"=\"\n", *gstnStr);
        return RFAILED;
	}
	gstnStr++;

	CC_CLR_LEADING_LWS(gstnStr);

	if (gstnStr >= aniValue + aniLen)
	{
        DP("No parameter value\n");
        return RFAILED;
	}

    DP("gstn-location is %s\n", gstnStr);

	if (*gstnStr == CC_ASCII_DOUBLE_QUOTES)
	{
	    gstnStr++;  	
		
		while ((i < paramSize - 1) && (*gstnStr != CC_ASCII_DOUBLE_QUOTES))
		{
			paramVal[i++] = *gstnStr++;
		}

		if (i == paramSize - 1)
		{
			DP("Miss right double quote or invalid format\n");
			i = 0;
		}
	    paramVal[i] = CC_ASCII_NULL;
	}
	else
	{
        strcpy(paramVal, gstnStr);
	}

    DP("paramVal is %s\n", paramVal);
	if ((paramVal[0] == CC_ASCII_NULL) || (strlen(paramVal) < 5))
	{
        DP("Miss gstn-location mandatory field\n");
        return RFAILED;
	}

    ch = (ccAscToHex(paramVal[0]) << 4) | (ccAscToHex(paramVal[1]));
	cgPtyNumS->natAddrInd.pres = PRSNT_NODEF;
	cgPtyNumS->natAddrInd.val  = ch & CC_NOA_MASK;
	cgPtyNumS->oddEven.pres    = PRSNT_NODEF;
	cgPtyNumS->oddEven.val     = (ch & CC_ODDEVEN_MASK) >> 7;

    ch = (ccAscToHex(paramVal[2]) << 4) | (ccAscToHex(paramVal[3]));
	cgPtyNumS->scrnInd.pres    = PRSNT_NODEF;
	cgPtyNumS->scrnInd.val     = ch & CC_SCRNIND_MASK;
	cgPtyNumS->presRest.pres   = PRSNT_NODEF;
	cgPtyNumS->presRest.val    = (ch & CC_APRI_MASK) >> 2;
	cgPtyNumS->numPlan.pres    = PRSNT_NODEF;
	cgPtyNumS->numPlan.val     = (ch & CC_NP_MASK) >> 4;
	cgPtyNumS->niInd.pres      = PRSNT_NODEF;
	cgPtyNumS->niInd.val       = (ch & CC_INNIND_MASK) >> 7;

	cgPtyNumS->addrSig.pres    = PRSNT_NODEF;
    ret = ccAsciiToBcd(&paramVal[4], strlen(&paramVal[4]), cgPtyNumS->addrSig.val, &cgPtyNumS->addrSig.len);
    if (ret != ROK)
    {
        DP("address signals are invalid\n");
        return RFAILED;
    }

	cgPtyNumS->eh.pres = PRSNT_NODEF;

    return ROK;
}

PUBLIC S16 ccParsePANIForOperSpecCgi(PsifSipANI *pAccessNetworkInfo, SiCgPtyNum *cgPtyNumS)
{
    if ((pAccessNetworkInfo == NULL) && (cgPtyNumS == NULL))
    {
    	DP("NULL Pointer\n");
    	return RFAILED;
    }

    if (pAccessNetworkInfo->pres == NOTPRSNT)
    {
    	DP("No P-Access-Network-Info hdr\n");
    	return RFAILED;
    }

    S32 ret;
    S8 *aniValue = pAccessNetworkInfo->aniValue; 
    U8  aniLen  = strlen(aniValue);
    S8 *gstnStr = NULL;
	S8  paramVal[32];
	U8  paramSize = 32;
	U8  ch;
	U8  i = 0;

    if (aniLen == 0)
    {
        DP("No Content in P-Access-Network-Info hdr\n");
        return RFAILED;
    }

    gstnStr = strstr(aniValue, "GSTN;");
    if (gstnStr == NULL)
	{
        DP("No access-type GSTN in P-Access-Network-Info hdr\n");
        return RFAILED;
	}

    DP("gstnStr is %s\n", gstnStr);

	gstnStr += strlen("GSTN;");

	CC_CLR_LEADING_LWS(gstnStr);

	if (gstnStr >= aniValue + aniLen)
	{
        DP("No parameter value\n");
        return RFAILED;
	}

    if (strncmp(gstnStr, "operator-specific-GI", strlen("operator-specific-GI")) != 0)
	{
        DP("No operator-specific-GI access info\n");
        return RFAILED;
	}

	gstnStr += strlen("operator-specific-GI");

	CC_CLR_LEADING_LWS(gstnStr);

	if (gstnStr >= aniValue + aniLen)
	{
        DP("No parameter value\n");
        return RFAILED;
	}

	if (*gstnStr != CC_ASCII_EQUAL)
	{
        DP("%c is not \"=\"\n", *gstnStr);
        return RFAILED;
	}
	gstnStr++;

	CC_CLR_LEADING_LWS(gstnStr);

	if (gstnStr >= aniValue + aniLen)
	{
        DP("No parameter value\n");
        return RFAILED;
	}

    DP("operator-specific-GI is %s\n", gstnStr);

	if (*gstnStr == CC_ASCII_DOUBLE_QUOTES)
	{
	    gstnStr++;  	
		
		while ((i < paramSize - 1) && (*gstnStr != CC_ASCII_DOUBLE_QUOTES))
		{
			paramVal[i++] = *gstnStr++;
		}

		if (i == paramSize - 1)
		{
			DP("Miss right double quote or invalid format\n");
			i = 0;
		}
	    paramVal[i] = CC_ASCII_NULL;
	}
	else
	{
        strcpy(paramVal, gstnStr);
	}

    DP("paramVal is %s\n", paramVal);
	if (paramVal[0] == CC_ASCII_NULL)
	{
        DP("Miss operator-specific-GI mandatory field\n");
        return RFAILED;
	}

	cgPtyNumS->addrSig.pres    = PRSNT_NODEF;
    ret = ccAsciiToBcd(paramVal, strlen(paramVal), cgPtyNumS->addrSig.val, &cgPtyNumS->addrSig.len);
    if (ret != ROK)
    {
        DP("address signals are invalid\n");
        return RFAILED;
    }

	cgPtyNumS->oddEven.pres    = PRSNT_NODEF;
	cgPtyNumS->oddEven.val     = strlen(paramVal) & 1;
	cgPtyNumS->niInd.pres      = PRSNT_NODEF;
	cgPtyNumS->niInd.val       = 0x01;

	cgPtyNumS->eh.pres = PRSNT_NODEF;

    return ROK;
}

PUBLIC S16 ccBuildPANI(SiCgPtyNum *cgPtyNumS, PsifSipANI *pAccessNetworkInfo)
{
    if ((cgPtyNumS == NULL) && (pAccessNetworkInfo == NULL))
    {
        DP("NULL Pointer\n");
        return RFAILED;
    }

    if (cgPtyNumS->eh.pres == NOTPRSNT)
    {
        DP("No Location Number\n");
        return RFAILED;
    }

    S8 gstnStr[SIP_ACCESS_NETWORKINFO_LEN];
	S8 paramVal[32];
	U8 addrLen;
	U8 ch[2] = {0};
	U8 i;

	if (cgPtyNumS->natAddrInd.pres = PRSNT_NODEF)
	{
		ch[0] = cgPtyNumS->natAddrInd.val;
	}

	if (cgPtyNumS->oddEven.pres = PRSNT_NODEF)
	{
		ch[0] |= cgPtyNumS->oddEven.val << 7;
	}

	if (cgPtyNumS->scrnInd.pres = PRSNT_NODEF)
	{
		ch[1] = cgPtyNumS->scrnInd.val;
	}

	if (cgPtyNumS->presRest.pres = PRSNT_NODEF)
	{
		ch[1] |= cgPtyNumS->presRest.val << 2;
	}

	if (cgPtyNumS->numPlan.pres = PRSNT_NODEF)
	{
		ch[1] |= cgPtyNumS->numPlan.val << 4;
	}

	if (cgPtyNumS->niInd.pres = PRSNT_NODEF)
	{
		ch[1] |= cgPtyNumS->niInd.val << 7;
	}

	for (i = 0; i < 2; i++)
	{
		paramVal[2*i] = ccHexToAsc((ch[i] & 0xf0) >> 4);
		paramVal[2*i + 1] = ccHexToAsc(ch[i] & 0x0f);
	}

	if (cgPtyNumS->addrSig.pres = PRSNT_NODEF)
	{
		pAccessNetworkInfo->pres = PRSNT_NODEF;
        ccBcdToAscii(cgPtyNumS->addrSig.val, cgPtyNumS->addrSig.len, &paramVal[4], &addrLen, cgPtyNumS->oddEven.val);
		paramVal[addrLen + 4] = CC_ASCII_NULL;

        sprintf(gstnStr, "GSTN; gstn-location=\"%s\"", paramVal);
		strcpy(pAccessNetworkInfo->aniValue, gstnStr);
	}

    return ROK;
}
