#
#	MatrixSSL example applications makefile
#	Builds optimized by default, see comments below for debug.
#	Copyright (c) PeerSec Networks, 2002-2007. All Rights Reserved
#

#CC 	= gcc
O 	= .o
SO  = .so
A   = .a
E	=

#LIBRARIES	= -lssl -lcrypto 
#STATICS		=  -lpthread

BRIDGE_OBJS = kios.o lcd.o nvram.o gps.o kb.o  https.o
BRIDGE_EXE = kios
#	Debug flags (Define MATRIXSSL_DEBUG in shell to build debug) 
#	bash:	export MATRIXSSL_DEBUG=1
#			unset MATRIXSSL_DEBUG
#	tcsh:	setenv MATRIXSSL_DEBUG 1
#			unsetenv MATRIXSSL_DEBUG
#
ifdef MATRIXSSL_DEBUG
DFLAGS  = -g -Wall -DDEBUG
else
#DFLAGS	= -Os
DFLAGS	= -O3
endif

ifdef MATRIXSSL_PROFILE
DFLAGS	+= -g -pg
endif

#
#   Override variables for compilation on x86-64 (AMD64, Nacona, etc)
#
#ifeq ($(shell uname -m),x86_64)
#DFLAGS	+= -march=k8 -fPIC -DFP_64BIT -DTFM_X86_64 -DRDTSC
#endif

#
#	Compile options
#
CFLAGS  = $(DFLAGS) -DLINUX
LDFLAGS = -lc -lcrypto -lssl -lpthread

#
#   Override variables for compilation on Mac OS X (Darwin)
#
#ifeq ($(shell uname),Darwin)
#CC  = cc
#SO  = .dylib
#CFLAGS += -DOSX -isystem -I/usr/include
#LDFLAGS =
#endif

#
#   Override variables for compilation on uClinux (example only)
#
#ifdef UCLINUX
#CC		= /opt/brcm/hndtools-mipsel-uclibc/bin/mipsel-uclibc-gcc
#STRIP	= /opt/brcm/hndtools-mipsel-uclibc/bin/mipsel-uclibc-strip
#endif

all: compile

compile: $(BRIDGE_OBJS) $(BRIDGE_EXE) $(SETUP_OBJS) $(SETUP_EXE) $(BLINK_EXE) $(GPIO_EXE) $(BLINK_OBJS) $(GPIO_OBJS) $(UPDATE_EXE)  $(UPDATE_OBJS)

#
# Manual dependencies
#
*.o: $(shell find Include/*.h -type f) $(shell find *.h -type f) Makefile

$(BRIDGE_EXE):$(BRIDGE_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(STATICS)
	

#	Clean up all generated files
#
clean:
	rm -f  $(BRIDGE_EXE) $(BRIDGE_OBJS) 
