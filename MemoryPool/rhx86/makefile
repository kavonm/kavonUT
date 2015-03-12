TOP = ../..
include $(TOP)/build/defs.mk

LOCAL_VPATH = ../ $(TOP)/sigcom/src
LOCAL_INCLUDES = 

SRCS = memory_pool.c 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)
COVS = $(SRCS:%.c=%.gcno) $(SRCS:%.c=%.gcda) $(SRCS:%=%.gcov)

LIB = libMemoryPool.a

BUILDTARGETS = $(DEPENDS) $(LIB)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(COVS) $(DEPENDS) $(OBJS) $(LIB)
