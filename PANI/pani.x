#ifndef PANI_X_
#define PANI_X_

typedef struct psifSipANI
{
	U8 pres;
	S8 aniValue[SIP_ACCESS_NETWORKINFO_LEN];
} PsifSipANI;

typedef struct cgPtyNum          /* Calling Party Number */
{
   ElmtHdr eh;                   /* element header */
   TknU8 natAddrInd;             /* nature of addresss indicator */
   TknU8 oddEven;                /* odd or even */
   TknU8 scrnInd;                /* screen indicator */
   TknU8 presRest;               /* Address presentation restricted ind. */
   TknU8 numPlan;                /* numbering plan */
   TknU8 niInd;                  /* number incomplete indicator */
   TknUStr addrSig;               /* Address Signal */
} SiCgPtyNum;                                                              

EXTERN U8  ccAscToHex(S8 asc);
EXTERN S8  ccHexToAsc(U8 hex);
EXTERN S16 ccAsciiToBcd(S8 *srcP, U8 srcLen, U8 *dstP, U8 *dstLen);
EXTERN S16 ccBcdToAscii(S8 *srcP, U8 srcLen, U8 *dstP, U8 *dstLen, U8 oddEven);
EXTERN S16 ccParsePANI(PsifSipANI *pAccessNetworkInfo, SiCgPtyNum *cgPtyNumS);
EXTERN S16 ccBuildPANI(SiCgPtyNum *cgPtyNumS, PsifSipANI *pAccessNetworkInfo);

#endif /*PANI_X_*/
