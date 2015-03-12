/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "sdes.h"
#include "openssl/rand.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "sdes.x"

/* local externs */
EXTERN MemoryPool *pool;

S32 main()
{
    S32 ret;
    U32 length;
    U8  *keySalt;
    U8  *base64KeySalt;
    U8  index;

    /* Create a memory pool */
    pool = CreateMemoryPool(MEMORY_POOL_SIZE);
    if (pool == NULL)
    {
		DP("Memory pool failed to be created.\n");
        return RFAILED;
    }

    length = ((KEY_SALT_ORIG_LEN + 2) / 3) * 4 + 1;
    base64KeySalt = (U8 *)Calloc(pool, length);
    keySalt = (U8 *)Calloc(pool, KEY_SALT_ORIG_LEN);

    /* create master key and salt */
    DP("1. Start to create master key and salt...\n\n");
    ret = CreateCryptoKeySalt(base64KeySalt);
    if (ret != ROK)
    {
        DP("Master key and salt could not be created.\n");
        return ret;
    }
    DP("Base64 master key and salt[%d]:\n\"%s\"\n\n", strlen(base64KeySalt), base64KeySalt);

    /* base64 decoding */
    DP("2. Start to base64 decode master key and salt...\n\n");
    ret = Base64Decode(base64KeySalt, length - 1, keySalt);
    if (ret != ROK)
    {
        DP("Master key and salt could not be decoded.\n");
        return ret;
    }

    DP("Master key and salt:\n");
    for (index = 0; index < KEY_SALT_ORIG_LEN; index++)
    {
        DP("0x%02x ", keySalt[index]);
    }
    DP("\n\n");

    /* Destroy a memory pool */
    DestroyMemoryPool(pool);
    return ROK;
}
