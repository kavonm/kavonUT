TOP = ../../..
include $(TOP)/build/defs.mk

TEST_EXE = lotteryTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LOTTERY_TEST_LIBS = $(TOP)/Lottery/AllTest/$(TARGET_OBJ_DIR)/liblottery_alltest.a \
                    $(TOP)/Lottery/Test/$(TARGET_OBJ_DIR)/libLotteryTest.a \
                    $(TOP)/sigcom/src/mock/$(TARGET_OBJ_DIR)/libMock.a \
                    $(CPPUTEST_HOME)/lib/libCppUTest.a \
                    $(CPPUTEST_HOME)/lib/libCppUTestExt.a \
                    $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a \
                    $(TOP)/Lottery/$(TARGET_OBJ_DIR)/libLottery.a

BUILDTARGETS = $(DEPENDS) $(TEST_EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(TEST_EXE):
	$(CC) -o $(TEST_EXE) $(LIBLDFLAGS) $(LOTTERY_TEST_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
