TOP = .
include $(TOP)/build/defs.mk

.PHONY: all clean

SPECIAL_CASE := NO

######################################################
            ##### make SWATCA=t #####
######################################################
ifdef SWATCA
ifneq ($(strip $(SWATCA)), FOOB)
SPECIAL_CASE := YES
all clean:
	@echo
	@echo "*********************************************"
	@echo "**************** BASIC BUILD ****************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWBASIC=t $@
	@echo
	@echo "*********************************************"
	@echo "**************** MEMORY POOL ****************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWMP=t $@
	@echo
	@echo "*********************************************"
	@echo "**************** DIGIT MAP ****************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWDM=t $@
	@echo
	@echo "*********************************************"
	@echo "****************** LOTTERY ******************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWLTR=t $@
	@echo
	@echo "*********************************************"
	@echo "******************* POSIX *******************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWPOSIX=t $@
	@echo
	@echo "*********************************************"
	@echo "***************** ISUPCUST ******************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWISUPCUST=t $@
	@echo
	@echo "*********************************************"
	@echo "******************* PANI ********************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWPANI=t $@
	@echo
	@echo "*********************************************"
	@echo "******************* SDES ********************"
	@echo "*********************************************"
	@echo
	make SWATCA=FOOB SWSDES=t $@
endif
endif # SWATCA

ifeq ($(SPECIAL_CASE), NO)

######################################################
             ##### make SWBASIC=t #####
######################################################
ifdef SWBASIC
SUBDIRS = $(TOP)/sigcom
endif # SWBASIC

######################################################
              ##### make SWMP=t #####
######################################################
ifdef SWMP
SUBDIRS = $(TOP)/MemoryPool
endif # SWMP

######################################################
              ##### make SWDM=t #####
######################################################
ifdef SWDM
SUBDIRS = $(TOP)/DigitMap
endif # SWDM

######################################################
              ##### make SWLTR=t #####
######################################################
ifdef SWLTR
SUBDIRS = $(TOP)/Lottery
endif # SWLTR

######################################################
             ##### make SWPOSIX=t #####
######################################################
ifdef SWPOSIX
SUBDIRS = $(TOP)/Posix
endif # SWPOSIX

######################################################
             ##### make SWISUPCUST=t #####
######################################################
ifdef SWISUPCUST
SUBDIRS = $(TOP)/ISUP_CUST
endif # SWISUPCUST

######################################################
             ##### make SWPANI=t #####
######################################################
ifdef SWPANI
SUBDIRS = $(TOP)/PANI
endif # SWPANI

######################################################
             ##### make SWSDES=t #####
######################################################
ifdef SWSDES
SUBDIRS = $(TOP)/SDES
endif # SWSDES

SUBDIR_TARGETS = $(SUBDIRS:%=%.subdir)
BUILDTARGETS = $(SUBDIR_TARGETS) 

CC = $(GCC)
include $(TOP)/build/rules.mk

clean:
	for subdir in $(SUBDIRS); do \
		make -C $$subdir clean; \
	done
endif # SPECIAL_CASE = NO
