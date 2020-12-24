ROOT=$(PROJECTDIR)
OBJDIR=$(ROOT)/obj
BINDIR=$(ROOT)/bin
LIBDIR=$(ROOT)/lib
INCDIR=$(ROOT)/include

BASECOMDIR=$(PROJECTDIR)/../basecom
DATABASEDIR=$(ROOT)/../database
COMUTILSDIR=$(ROOT)/../utils

# CROSS_COMPILE specify the prefix used for all executables used
# during compilation. Only gcc and related bin-utils executables
# are prefixed with $(CROSS_COMPILE).
# CROSS_COMPILE can be set on the command line
# make CROSS_COMPILE=ia64-linux-
# Alternatively CROSS_COMPILE can be set in the environment.
# Default value for CROSS_COMPILE is not to prefix executables

CROSS_COMPILE   ?=
PREFIX          ?=
INC_PREFIX      ?= $(PREFIX)
LIB_PREFIX      ?= $(PREFIX)
BIN_PREFIX     ?= $(PREFIX)
ABIFLAGS        ?=
MFLAGS			?=

# Make variables (CC, etc...)
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)g++
CC              = $(CROSS_COMPILE)g++
CPP             = $(CROSS_COMPILE)g++
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
RANLIB          = $(CROSS_COMPILE)ranlib


LDFLAGS = $(ABIFLAGS) $(MFLAGS) -L$(BASECOMDIR) -L$(LIB_PREFIX)/usr/lib/SecHome -L$(LIBDIR) 
ARFLAGS = -cru

DEFS=
INCS=-I. -I$(INCDIR) -I$(INC_PREFIX)/usr/include/SecHome \
		-I$(BASECOMDIR)/include \
		-I$(BASECOMDIR)/basedao/include \
		-I$(DATABASEDIR)/include \
		-I$(INC_PREFIX)/usr/include/SecHome/accountnew \
		-I$(INC_PREFIX)/usr/include/SecHome/ini \
		-I$(INC_PREFIX)/usr/include/SecHome/ngfwdb 
CFLAGS+= $(ABIFLAGS) -Wall $(MFLAGS) -c -g -O2 -DPROJECT_$(PROJECT) $(DEFS) $(INCS) -DPROJECT_$(PROJECT) -DDEVTYPE_$(DEVTYPE)

debug=1

ifeq "$(debug)" "1"
     CFLAGS+=-D__DEBUG__
endif

TIPC=0
ifeq "$(TIPC)" "1"
     CFLAGS+=-D__TIPC__
endif

VPATH=.:$(BINDIR):$(LIBDIR):$(OBJDIR)


.o:;

.c.o:$$<
	@echo -n Compiling $<  ...... 
	@$(CC) $(CFLAGS) $< -o $(OBJDIR)/$@
	@echo [OK]

.C.o:$$<
	@echo -n Compiling $<  ...... 
	@$(CC) $(CFLAGS) $< -o $(OBJDIR)/$@
	@echo [OK]

.cpp.o:$$<
	@echo -n Compiling $<  ......
	@$(CPP) $(CFLAGS) $< -o $(OBJDIR)/$@
	@echo [OK]

