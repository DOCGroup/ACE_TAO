# $Id$
#
# Chorus with Green Hills

# Location of Green Hills installation.  The ifndef protection allows
# setting via an environment variable.
ifndef GHS_DIR
  GHS_DIR=/project/doc/green68k/GreenHills/releases/chorus-m68k
endif # GHS_DIR

# Location of mvme installation.  The ifndef protection allows
# setting via an environment variable.
ifndef MVME_INCL
  MVME_INCL=/project/doc/mvme177/appl/include
endif # MVME_INCL

CHORUS = 1

debug = 1
optimize = 1
shared_libs =
static_libs = 1

CC              = $(GHS_DIR)/build -driver ch68
CXX             = $(CC)
CFLAGS          = -I$(MVME_INCL)/stdc \
                  -I$(MVME_INCL)/posix \
                  -I$(MVME_INCL)/chorus \
                  -I$(MVME_INCL)/CC
CCFLAGS         += $(CFLAGS) --no_rtti --multibyte_chars --wchar_t_keyword \
                   -tnone
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LDFLAGS         = --no_auto_instantiation
OFLAGS          += -O -OM -OL
AR              = $(GHS_DIR)/ax
ARFLAGS         = rv
RANLIB          = /bin/true
