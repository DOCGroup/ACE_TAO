# $Id$

# This platform macros file is intended to work with Rational C++ (2.4.1) on
# Digital UNIX 4.0 (OSF/1 4.0).

ifeq (,$(debug))
  debug = 1
endif

ifeq (,$(exceptions))
  exceptions = 1
endif
ifeq (,$(rtti))
   rtti = 1
endif

ifeq (,$(threads))
  threads = 1
endif

ifeq (,$(optimize))
  optimize = 1
endif

CC              = rcc
CXX             = RCC
CFLAGS          += -D_REENTRANT
CCFLAGS         += $(CFLAGS) -nosienna
DCFLAGS         += -g -O0
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lpthread -lc_r -ltli -lrt
OCFLAGS         += -O4
PIC             = -pic
ARFLAGS         = cruvZ
# When libraries are archived, a hash index is automatically created
# so there is no need for ranlib
RANLIB          = /usr/bin/true
SOFLAGS         += -shared  # -use_ld_input $(ACELIB)
SOBUILD         = $(RM) $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
                  ln $(VSHDIR)$*.o $@
