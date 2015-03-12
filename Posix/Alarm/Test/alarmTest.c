/* [Test group list] 
 * 1. AlarmTestGroup
 *    1.1 SyncAlarmTest
 *    1.2 ForkAlarmTest
 *    1.3 ThreadAlarmTest
 *    1.4 MutexAlarmTest
 *    1.5 CondAlarmTest
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
#include "alarm.h"
#include "alarmTest.h"

/* header/extern include files (.x) */
#include "gen.x"
#include "memory_pool.x"
#include "alarm.x"                
#include "mock_mem.x"
}

/* TEST GROUP 1: AlarmTestGroup */
TEST_GROUP(AlarmTestGroup)
{
    void setup()
    {
        UT_PTR_SET(Malloc, mock_Malloc);
        UT_PTR_SET(Free, mock_Free);
	}

    void teardown()
    {
	}
};

/* case 1: synchronous alarm */
TEST(AlarmTestGroup, SyncAlarmTest)
{
	/* Test Synchronous Alarm */
    SyncAlarm();
}

/* case 2: fork alarm */
TEST(AlarmTestGroup, ForkAlarmTest)
{
	/* Test Fork Alarm */
    ForkAlarm();
}

/* case 3: thread alarm */
TEST(AlarmTestGroup, ThreadAlarmTest)
{
	/* Test Thread Alarm */
    ThreadAlarm();
}

/* case 4: mutex alarm */
TEST(AlarmTestGroup, MutexAlarmTest)
{
	/* Test Mutex Alarm */
    MutexAlarm();
}

/* case 5: cond alarm */
TEST(AlarmTestGroup, CondAlarmTest)
{
	/* Test Cond Alarm */
    CondAlarm();
}
