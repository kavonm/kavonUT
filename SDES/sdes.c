/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "sdes.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "sdes.x"

/* global variables */
MemoryPool *pool;

static U8 base64EncMap[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

static S8 base64DecMap[] = 
{                                        
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

PUBLIC S16 CreateCryptoKeySalt(U8 *base64KeySalt)
{
    S32 ret;
    U8  index;
    U8  keySalt[KEY_SALT_ORIG_LEN];

    /* generate master key and salt */
    ret = RAND_bytes(keySalt, KEY_SALT_ORIG_LEN);

    if (ret == FALSE)
    {
        DP("Master key and salt could not be created.\n");
        return RFAILED;
    }

    DP("Original master key and salt[%d]:\n", KEY_SALT_ORIG_LEN);
    for (index = 0; index < KEY_SALT_ORIG_LEN; index++)
    {
        DP("0x%02x ", keySalt[index]);
    }
    DP("\n\n");

    /* base64 encode */
    Base64Encode(keySalt, KEY_SALT_ORIG_LEN, base64KeySalt);
 
    return ROK;
}

PUBLIC S16 Base64Encode(U8 *inBuf, U32 inLen, U8 *outBuf)
{
    U8 in[3];
    U8 unitLen;

    while (inLen)
    {
        memset(in, 0, 3);
        if (inLen >= 3)
        {
            memcpy(in, inBuf, 3);
            unitLen = 3;
        }
        else
        {
            memcpy(in, inBuf, inLen);
            unitLen = inLen;
        }
        inBuf += unitLen;
        inLen -= unitLen;
        
        *outBuf++ = base64EncMap[in[0] >> 2];
        *outBuf++ = base64EncMap[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
        *outBuf++ = (unitLen > 1 ? base64EncMap[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : BASE64_PADDING);
        *outBuf++ = (unitLen > 2 ? base64EncMap[in[2] & 0x3f] : BASE64_PADDING);
    }

    return ROK;
}

PUBLIC S16 Base64Decode(U8 *inBuf, U32 inLen, U8 *outBuf)
{
    S8 in[4];
    U8 i, noBase64Padding = TRUE;

    while (inLen && noBase64Padding && *inBuf != BASE64_PADDING)
    {
        memset(in, 0, 4);
        for (i = 0; i < 4 && inLen; i++)
        {
            if (*inBuf == BASE64_PADDING)
            {
                noBase64Padding = FALSE;
            }

            /* validate input base64 string */
            in[i] = base64DecMap[*inBuf];
            if ((in[i] == -1) ||
                (*inBuf == BASE64_PADDING && i == 1) ||
                (*inBuf != BASE64_PADDING && !noBase64Padding))
            {
                DP("Invalid base64 string: %c\n", *inBuf);
                return RFAILED;
            }

            inLen--;
            inBuf++;
        }
        
        *outBuf++ = (in[0]<<2) | (in[1]>>4);
        *outBuf++ = (in[1]<<4) | (in[2]>>2);
        *outBuf++ = (in[2]<<6) | (in[3]);
    }

    return ROK;
}
