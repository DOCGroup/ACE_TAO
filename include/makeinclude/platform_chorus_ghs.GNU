# $Id$
#
# Chorus with Green Hills

# Locations of Green Hills installation, MVME installation, etc.  The
# ifndef protection allows setting via environment variables.
ifndef GHS_DIR
  GHS_DIR=/project/doc/green68k/GreenHills/releases/chorus-m68k
endif # GHS_DIR

ifndef MVME_DIR
  MVME_DIR=/project/doc/mvme177/appl
endif # MVME_DIR

ifndef MVME_INCL
  MVME_INCL = $(MVME_DIR)/include
endif # MVME_INCL

ifndef MVME_LIB
  MVME_LIB = $(MVME_DIR)/lib
endif # MVME_LIB

CHORUS = 1
CROSS-COMPILE = 1

debug = 1
optimize = 1
shared_libs =
static_libs = 1

CC      = $(GHS_DIR)/ccch68

CXX     = $(CC)
CFLAGS  = -groot= -alttools=$(GHS_DIR) -68040 -X445 -fprecise -D__cpluplus=1 \
          -I$(MVME_INCL)/posix \
          -I$(MVME_INCL)/stdc \
          -I$(MVME_INCL)/chorus \
          -I$(MVME_INCL)/CC
CCFLAGS += $(CFLAGS) --no_rtti --multibyte_chars --wchar_t_keyword -tnone
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS = --no_auto_instantiation \
          -L$(MVME_LIB)/classix -L$(MVME_LIB)/CC
OFLAGS  += -O -OM -OL
AR      = $(GHS_DIR)/gnuch68/bin/ar
ARFLAGS = rv
RANLIB  = /bin/true
