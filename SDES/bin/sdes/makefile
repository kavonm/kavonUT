TOP = ../../..
include $(TOP)/build/defs.mk

EXE = sdes

LOCAL_VPATH = 
LOCAL_INCLUDES = 

SRCS = 
OBJS = $(SRCS:%.c=%.o)
DEPENDS = $(SRCS:%.c=%.d)

LIBLDFLAGS += -ldl
SDES_LIBS = $(TOP)/SDES/$(MAIN_DIR)/$(TARGET_OBJ_DIR)/libSDESMain.a \
            $(TOP)/SDES/$(TARGET_OBJ_DIR)/libSDES.a \
            $(TOP)/MemoryPool/$(TARGET_OBJ_DIR)/libMemoryPool.a \
            $(OPENSSL_HOME)/libcrypto.a

BUILDTARGETS = $(DEPENDS) $(EXE)

CC = $(GCC)

include $(TOP)/build/rules.mk

$(EXE): $(DIGITMAP_LIBS)
	$(CC) -o $(EXE) $(SDES_LIBS) $(LIBLDFLAGS)

.PHONY: clean
clean:
	-rm -f $(DEPENDS) $(OBJS) $(EXE)
