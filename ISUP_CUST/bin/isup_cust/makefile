TOP = ../../..
include $(TOP)/build/defs.mk

EXE = isupCust

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

DIGITMAP_LIBS = $(TOP)/ISUP_CUST/$(MAIN_DIR)/$(TARGET_OBJ_DIR)/libIsupCustMain.a \
                $(TOP)/ISUP_CUST/$(TARGET_OBJ_DIR)/libIsupCust.a \
                $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a

BUILDTARGETS = $(DEPENDS) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(EXE): $(DIGITMAP_LIBS)
	$(CC) -o $(EXE) $(LIBLDFLAGS) $(DIGITMAP_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
