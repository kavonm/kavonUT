TOP = ../../..
include $(TOP)/build/defs.mk

TEST_EXE = sdesTest

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIBLDFLAGS += -ldl
SDES_TEST_LIBS = $(TOP)/SDES/AllTest/$(TARGET_OBJ_DIR)/libsdes_alltest.a \
                 $(TOP)/SDES/Test/$(TARGET_OBJ_DIR)/libSDESTest.a \
                 $(TOP)/sigcom/src/mock/$(TARGET_OBJ_DIR)/libMock.a \
                 $(CPPUTEST_HOME)/lib/libCppUTest.a \
                 $(CPPUTEST_HOME)/lib/libCppUTestExt.a \
                 $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a \
                 $(TOP)/SDES/$(TARGET_OBJ_DIR)/libSDES.a \
                 $(OPENSSL_HOME)/libcrypto.a

BUILDTARGETS = $(DEPENDS) $(TEST_EXE)

CC = $(GPP)

include $(TOP)/build/rules.mk

$(TEST_EXE):
	$(CC) -o $(TEST_EXE) $(SDES_TEST_LIBS) $(LIBLDFLAGS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(TEST_EXE)
