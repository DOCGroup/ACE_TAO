# $Id$
#
# Chorus with Green Hills

# Locations of Green Hills installation, MVME installation, etc.  The
# ifndef protection allows setting via environment variables.
ifndef GHS_DIR
  GHS_DIR=/project/doc/green68k/GreenHills/releases/chorus-m68k
endif # GHS_DIR

ifndef COOL_DIR
  COOL_DIR=/project/doc/miniCOOL/minicool.r4.3/classix-r3-flm-mvme177-gh
endif # COOL_DIR

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

CC      = $(GHS_DIR)/cxch68
CXX     = $(CC)
CFLAGS  = -groot=$(MVME_DIR) -alttools=$(GHS_DIR) -68040 -X445 -fprecise \
          -I$(MVME_INCL)/posix \
          -I$(MVME_INCL)/stdc \
          -I$(MVME_INCL)/chorus \
          -I$(MVME_INCL)/CC
CCFLAGS += $(CFLAGS) --mkctors --multibyte_chars --no_rtti --wchar_t_keyword \
           -tnone
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS = --no_auto_instantiation \
          -u __main -u __premain -u _abort -r \
          $(MVME_DIR)/lib/CC/ioassign.o $(MVME_DIR)/lib/CC/iostream.o \
          $(COOL_DIR)/lib/libOrb.a  \
          $(MVME_DIR)/lib/CC/libC.a \
          $(GHS_DIR)/ch68/libedg.a \
          $(MVME_DIR)/lib/classix/libcx.s.a
OFLAGS  += -O -OM -OL
AR      = $(GHS_DIR)/gnuch68/bin/ar
ARFLAGS = rv
RANLIB  = @true

POSTLINK = ; $(CC) -groot=$(MVME_DIR) -alttools=$(GHS_DIR) -68040 \
           -O -nostartfiles -nostdlib -e __start -o $@.final \
           $(MVME_DIR)/lib/crt/crth.s.o $(MVME_DIR)/lib/crt/crt0.o \
           $(MVME_DIR)/lib/crt/crti.o $@ \
           $(MVME_DIR)/lib/crt/crtn.o \
           -locatedprogram -r; \
           mv -f $@.final $@
