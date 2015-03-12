TOP = ../../../..
include $(TOP)/build/defs.mk

EXE = trylock

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

ALARM_LIBS = $(TOP)/Posix/Trylock/$(MAIN_DIR)/$(TARGET_OBJ_DIR)/libTrylockMain.a \
             $(TOP)/Posix/Trylock/$(TARGET_OBJ_DIR)/libTrylock.a \
             $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a

BUILDTARGETS = $(DEPENDS) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(EXE): $(ALARM_LIBS)
	$(CC) -pthread -o $(EXE) $(LIBLDFLAGS) $(ALARM_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
