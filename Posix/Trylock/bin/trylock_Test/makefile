TOP = ../../../..
include $(TOP)/build/defs.mk

TEST_EXE = trylockTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

DIGITMAP_TEST_LIBS = $(TOP)/Posix/Trylock/AllTest/$(TARGET_OBJ_DIR)/libtrylock_alltest.a \
                     $(TOP)/Posix/Trylock/Test/$(TARGET_OBJ_DIR)/libTrylockTest.a \
                     $(TOP)/sigcom/src/mock/$(TARGET_OBJ_DIR)/libMock.a \
                     $(CPPUTEST_HOME)/lib/libCppUTest.a \
                     $(CPPUTEST_HOME)/lib/libCppUTestExt.a \
                     $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a \
                     $(TOP)/Posix/Trylock/$(TARGET_OBJ_DIR)/libTrylock.a

BUILDTARGETS = $(DEPENDS) $(TEST_EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(TEST_EXE):
	$(CC) -pthread $(LIBLDFLAGS) -o $(TEST_EXE) $(DIGITMAP_TEST_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
