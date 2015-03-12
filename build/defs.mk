CPPUTEST_HOME = $(TOP)/cpputest
OPENSSL_HOME = $(TOP)/openssl
AR = ar
VPATH = $(LOCAL_VPATH)

GLOBAL_INCLUDES = -I$(TOP)/sigcom/include
BIN_DIR = bin
MAIN_DIR = main
TARGET_OBJ_DIR = rhx86
INCLUDE_LIST = $(GLOBAL_INCLUDES) $(LOCAL_INCLUDES)

ifdef DEBUG
GLOBAL_DEFINES += -DDEBUG
endif

ifdef UT
GLOBAL_DEFINES += -DUTEST
INCLUDE_LIST += -I$(TOP)/sigcom/src/mock
endif

CFLAGS = $(INCLUDE_LIST) $(GLOBAL_DEFINES) 
ifdef COV
CFLAGS += -fprofile-arcs
CFLAGS += -ftest-coverage
LIBLDFLAGS += -fprofile-arcs
LIBLDFLAGS += -ftest-coverage
endif

GCC = /usr/bin/gcc -g
GPP = /usr/bin/g++ -g
