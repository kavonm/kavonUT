TOP = ../..
include $(TOP)/build/defs.mk

SUBDIRS = ./$(TARGET_OBJ_DIR) ./$(MAIN_DIR)

ifdef UT
SUBDIRS += ./Test ./AllTest
endif

SUBDIRS += ./$(BIN_DIR)

SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)

BUILDTARGETS = $(SUBDIR_TARGETS) 

CC = $(GCC)

include $(TOP)/build/rules.mk

.PHONY: clean
clean:
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
