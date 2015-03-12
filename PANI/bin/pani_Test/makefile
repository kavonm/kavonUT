TOP = ../../..
include $(TOP)/build/defs.mk

TEST_EXE = paniTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

DIGITMAP_TEST_LIBS = $(TOP)/PANI/AllTest/$(TARGET_OBJ_DIR)/libpani_alltest.a \
                     $(TOP)/PANI/Test/$(TARGET_OBJ_DIR)/libPANITest.a \
                     $(TOP)/sigcom/src/mock/$(TARGET_OBJ_DIR)/libMock.a \
                     $(CPPUTEST_HOME)/lib/libCppUTest.a \
                     $(CPPUTEST_HOME)/lib/libCppUTestExt.a \
                     $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a \
                     $(TOP)/PANI/$(TARGET_OBJ_DIR)/libPANI.a

BUILDTARGETS = $(DEPENDS) $(TEST_EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(TEST_EXE):
	$(CC) -o $(TEST_EXE) $(LIBLDFLAGS) $(DIGITMAP_TEST_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
