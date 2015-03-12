TOP = ../../..
include $(TOP)/build/defs.mk

EXE = pani

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

DIGITMAP_LIBS = $(TOP)/PANI/$(MAIN_DIR)/$(TARGET_OBJ_DIR)/libPANIMain.a \
                $(TOP)/PANI/$(TARGET_OBJ_DIR)/libPANI.a \
                $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a

BUILDTARGETS = $(DEPENDS) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(EXE): $(DIGITMAP_LIBS)
	$(CC) -o $(EXE) $(LIBLDFLAGS) $(DIGITMAP_LIBS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
