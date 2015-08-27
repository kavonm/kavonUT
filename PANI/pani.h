#ifndef PANI_H_
#define PANI_H_

#define CC_ASCII_NULL              0
#define CC_ASCII_HORIZONTAL_TAB    9
#define CC_ASCII_LINE_FEED         10
#define CC_ASCII_CARRIAGE_RETURN   13
#define CC_ASCII_SPACE             32
#define CC_ASCII_DOUBLE_QUOTES     34
#define CC_ASCII_EQUAL             61

#define CC_NOA_MASK                0x7F
#define CC_ODDEVEN_MASK            0x80
#define CC_SCRNIND_MASK            0x03
#define CC_APRI_MASK               0x0C
#define CC_NP_MASK                 0x70
#define CC_INNIND_MASK             0x80

#define SIP_ACCESS_NETWORKINFO_LEN 256

#define CC_CLR_LEADING_LWS(PBEG)                      \
{                                                     \
	if (PBEG != NULL && PBEG[0] != CC_ASCII_NULL)     \
    {                                                 \
        while (*PBEG == CC_ASCII_HORIZONTAL_TAB ||    \
               *PBEG == CC_ASCII_LINE_FEED ||         \
               *PBEG == CC_ASCII_CARRIAGE_RETURN ||   \
               *PBEG == CC_ASCII_SPACE)               \
		{                                             \
            PBEG++;                                   \
		}                                             \
    }                                                 \
}

#endif /*PANI_H_*/
