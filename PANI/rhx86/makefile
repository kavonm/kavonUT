TOP = ../..
include $(TOP)/build/defs.mk

LOCAL_VPATH = ../ $(TOP)/sigcom/src
LOCAL_INCLUDES = -I$(TOP)/MemoryPool

SRCS = \
	   pani.c \

OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)
COVS = $(SRCS:%.c=%.gcno) $(SRCS:%.c=%.gcda) $(SRCS:%=%.gcov)

LIB = libPANI.a

BUILDTARGETS = $(DEPENDS) $(LIB)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(LIB): $(OBJS)
	$(AR) -rsv $(LIB) $(OBJS)

.PHONY: clean
clean:
	-rm -f $(COVS) $(DEPENDS) $(OBJS) $(LIB)
