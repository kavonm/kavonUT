/* header include files (.h) */
#include "gen.h"
#include "memory_pool.h"
#include "isup_cust.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "isup_cust.x"

/* local externs */
EXTERN MemoryPool *pool;

S32 main()
{
	//read_ISUP_CUST();
	read_ISUP_CUST_ATCA();
    return ROK;
}
