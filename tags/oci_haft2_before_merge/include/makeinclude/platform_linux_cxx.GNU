# $Id$

# This platform macros file is intended to work with
# Linux, using the cxx compiler..

debug = 1
exceptions = 1
rtti = 1

CC              = cxx
CXX             = $(CC)
CFLAGS          +=

  #### RedHat 5.2 /usr/include/malloc.h has an extraneous semicolon.
  WARNING_FLAGS += -w0 -msg_disable 381
  ifneq (,$(VERBOSE_WARNING))
    WARNING_FLAGS += -msg_display_number -msg_display_tag
  endif # VERBOSE_WARNING

CCFLAGS         += $(CFLAGS) $(WARNING_FLAGS)
ifeq ($(rtti),0)
  CCFLAGS += -nortti
endif # rtti
DCFLAGS         += -g -O0
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -ldl -lpthread
ifeq (-DACE_HAS_AIO_CALLS,$(PLATFORM_AIO_SUPPORT))
  LIBS += -lrt
endif
MATHLIB         = -lm
OCFLAGS         += -O4
PIC             =
ARFLAGS         = rsuv
RANLIB          = @true
SOFLAGS         += -shared $(ACELIB)
SOBUILD         = $(RM) $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
                  ln $(VSHDIR)$*.o $@

ifeq (0,$(exceptions))
  #### Disable the default exception handling of cxx >= 6.0.
  #### This is untested.  exceptions=1 is the default, so it's not used
  #### by default.
  ifndef CXX_VERSION
    CXX_VERSION := $(shell $(CXX) -V)
  endif # ! CXX_VERSION

  ifeq (6.,$(findstring 6.,$(CXX_VERSION)))
    CCFLAGS += -nocleanup
  endif # 6.x
endif # ! exceptions
