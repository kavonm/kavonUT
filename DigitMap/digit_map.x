#ifndef DIGIT_MAP_X_
#define DIGIT_MAP_X_

typedef enum DigitMapState_Tag
{
    DMS_INACTIVE = 0,
    DMS_ACTIVE,
    DMS_FAILED_MATCH,
    DMS_PARTIAL_MATCH,
    DMS_FULL_MATCH
} DigitMapState;

typedef struct digitMapCandidate
{
    DigitMapState       state;
    TknStr              pattern;
    U8                  index;
	U8                  matchLength;
} DigitMapCandidate;

typedef struct digitMap
{
    DigitMapCandidate  *candidates;
    U16                 numberCandidates;
	U8                  maxMatchLength;
} DigitMap;

#if 0
EXTERN S16 ValidateDigitMap(S8 *rawDigitMapStr, U8 len);
#else
EXTERN S16 ValidateDigitMap_real(const S8 *file, U32 line, S8 *rawDigitMapStr, U8 len);
#endif
EXTERN S16 TestValidateDigitMap();
EXTERN S16 InitDigitMap(DigitMap *pDigitMap, S8 rawDigitMapStr[][MAX_DIGITMAP_LEN], U16 num);
EXTERN S16 CandStateChange(DigitMap *pDigitMap, U16 idx, S16 matchFlag);
EXTERN S16 DigitMapCheck(DigitMap *pDigitMap, U16 idx, S8 digit);
EXTERN S8  GetMaxDigitsAndResetDigitMap(DigitMap *pDigitMap);

#endif /*DIGIT_MAP_X_*/
