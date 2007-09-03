# $Id$

# This file should allow ACE to be built on Linux, using the
# Portland Group PGI compilers
#
# This is just a copy of platform_linux.GNU, with a few things changed.

include $(ACE_ROOT)/include/makeinclude/platform_linux_common.GNU

static_libs_only ?= 1

CC  ?= pgcc
CXX ?= pgCC

CFLAGS  += --one_instantiation_per_object --prelink_objects -Wc,-tused
ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT $(PLATFORM_AIO_SUPPORT)
endif # threads

CCFLAGS += $(CFLAGS) $(IMPLICIT_TEMPLATES_FLAG)
DCFLAGS += -g
DLD     = $(CXX)
LD      = $(CXX)
LIBS    += -ldl

ifeq ($(threads),1)
  LIBS += -lpthread
  ifeq (-DACE_HAS_AIO_CALLS,$(PLATFORM_AIO_SUPPORT))
    LIBS += -lrt
  endif
endif

OCFLAGS += -O3

SOFLAGS += $(CPPFLAGS) -shared $(PIC) --one_instantiation_per_object --prelink_objects
SOBUILD = $(COMPILE.cc) $(PIC) --one_instantiation_per_object --prelink_objects -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif
