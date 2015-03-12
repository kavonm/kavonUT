TOP = ../../..
include $(TOP)/build/defs.mk

EXE = lottery

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LOTTERY_LIBS = $(TOP)/Lottery/$(MAIN_DIR)/$(TARGET_OBJ_DIR)/libLotteryMain.a \
               $(TOP)/Lottery/$(TARGET_OBJ_DIR)/libLottery.a \
               $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a

BUILDTARGETS = $(DEPENDS) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(EXE): $(LOTTERY_LIBS)
	$(CC) -o $(EXE) $(LIBLDFLAGS) $(LOTTERY_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
