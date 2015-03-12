TOP = ../..
include $(TOP)/build/defs.mk

SUBDIRS = ./isup_cust 

ifdef UT
SUBDIRS += ./isup_cust_Test
endif

SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)

BUILDTARGETS = $(SUBDIR_TARGETS) 

CC = $(GCC)

include $(TOP)/build/rules.mk

.PHONY: clean
clean:
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
