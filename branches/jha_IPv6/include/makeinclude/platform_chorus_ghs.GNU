# $Id$
#
# Chorus with Green Hills

# Locations of Green Hills installation, MVME installation, etc.  The
# ifndef protection allows setting via environment variables.
ifndef GHS_DIR
  GHS_DIR=/project/doc/mvme/green68k/GreenHills/releases/chorus-m68k
endif # GHS_DIR

ifndef COOL_DIR
  COOL_DIR=/project/doc/mvme/miniCOOL/minicool.r4.3/classix-r3-flm-mvme177-gh
endif # COOL_DIR

ifndef MVME_DIR
  MVME_DIR=/project/doc/mvme/mvme177
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
ifndef exceptions
  exceptions = 1
endif # exceptions
optimize = 1
shared_libs =
static_libs = 1

PATH +=:$(GHS_DIR)

CC      = cxch68
CXX     = $(CC)
CFLAGS  = -groot=$(MVME_DIR)/ -alttools=$(GHS_DIR)/gnuch68 -68040 -fprecise \
          -I$(MVME_INCL)/posix \
          -I$(MVME_INCL)/stdc \
          -I$(MVME_INCL)/chorus \
          -I$(MVME_INCL)/CC
CCFLAGS += $(CFLAGS) --mkctors --multibyte_chars --no_rtti --wchar_t_keyword \
           -tnone
DCFLAGS += -g -G
DLD     = $(CXX)
LD      = $(CXX)
LDFLAGS = --no_auto_instantiation -u __main -u __premain -u _abort -r \
          $(COOL_DIR)/lib/libOrb.a  \
          $(MVME_DIR)/lib/CC/libC.a \
          $(MVME_DIR)/lib/classix/libcx.s.a
OCFLAGS += -OL -OM
AR      = $(GHS_DIR)/gnuch68/ar
ARFLAGS = rv
RANLIB  = @true

ifeq ($(debug),)
  CRTH = $(MVME_DIR)/lib/crt/crth.s.o
else
  CRTH = $(MVME_DIR)/lib/crt/crth_d.s.o
endif # debug

ifeq (1,$(exceptions))
  CCFLAGS += --exceptions
endif # exceptions

POSTLINK = ; $(CC) -groot=$(MVME_DIR)/ -alttools=$(GHS_DIR)/gnuch68 -68040 \
           -nostartfiles -nostdlib -e __start -o $@.final \
           $(CRTH) $(MVME_DIR)/lib/crt/crt0.o \
           $(MVME_DIR)/lib/crt/crti.o $@ \
           $(MVME_DIR)/lib/crt/crtn.o \
           -locatedprogram -r; \
           mv -f $@.final $@; chmod +x $@; $(RM) /tmp/gh_*.[os]
