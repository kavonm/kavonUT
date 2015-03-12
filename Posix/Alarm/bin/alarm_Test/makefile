TOP = ../../../..
include $(TOP)/build/defs.mk

TEST_EXE = alarmTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

DIGITMAP_TEST_LIBS = $(TOP)/Posix/Alarm/AllTest/$(TARGET_OBJ_DIR)/libalarm_alltest.a \
                     $(TOP)/Posix/Alarm/Test/$(TARGET_OBJ_DIR)/libAlarmTest.a \
                     $(TOP)/sigcom/src/mock/$(TARGET_OBJ_DIR)/libMock.a \
                     $(CPPUTEST_HOME)/lib/libCppUTest.a \
                     $(CPPUTEST_HOME)/lib/libCppUTestExt.a \
                     $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a \
                     $(TOP)/Posix/Alarm/$(TARGET_OBJ_DIR)/libAlarm.a

BUILDTARGETS = $(DEPENDS) $(TEST_EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(TEST_EXE):
	$(CC) -pthread $(LIBLDFLAGS) -o $(TEST_EXE) $(DIGITMAP_TEST_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
