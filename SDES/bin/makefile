TOP = ../..
include $(TOP)/build/defs.mk

SUBDIRS = ./sdes 

ifdef UT
SUBDIRS += ./sdes_Test
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
