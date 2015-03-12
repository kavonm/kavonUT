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
#include "sdes.h"
#include "sdesTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "sdes.x"                
#include "mock_mem.x"                
}

TEST_GROUP(validateCryptoKeySaltTestGroup)
{
	S16 ret;
    U32 length;
    U8  *keySalt;
    U8  *base64KeySalt;

    void setup()
    {
        length = ((KEY_SALT_ORIG_LEN + 2) / 3) * 4 + 1;

        base64KeySalt = (U8 *)malloc(length);
        memset(base64KeySalt, 0, length);

        keySalt = (U8 *)malloc(KEY_SALT_ORIG_LEN);
        memset(keySalt, 0, KEY_SALT_ORIG_LEN);
	}

    void teardown()
    {
        free(base64KeySalt);
        base64KeySalt = NULL;

        free(keySalt);
        keySalt = NULL;
	}
};

TEST(validateCryptoKeySaltTestGroup, CreateCryptoKeySalt)
{
	ret = CreateCryptoKeySalt(base64KeySalt);
	CHECK_EQUAL(ROK, ret);
}

TEST(validateCryptoKeySaltTestGroup, Base64EncPlusDec)
{
	ret = CreateCryptoKeySalt(base64KeySalt);
	CHECK_EQUAL(ROK, ret);

	ret = Base64Decode(base64KeySalt, length - 1, keySalt);
	CHECK_EQUAL(ROK, ret);
}
