# $Id$

# This platform macros file is intended to work with
# Digital UNIX 4.0 (OSF/1 4.0), using the cxx compiler..

debug = 1
exceptions = 1
rtti = 1

ifeq (,$(threads))
  threads = 1
endif

CC              = cxx
CXX             = $(CC)
#### Note: -thread uses POSIX threads.  If you want to use DCE threads
####       instead, change "-pthread" to "-threads".
ifeq ($(threads),1)
	CFLAGS          += -pthread
else
	CFLAGS          += -DACE_MT_SAFE=0
endif

#### These flags can only be used with cxx Version 6.0 and later.
#### They are enabled by default; they can be disabled by settting
#### CXX_VER to CXX_5 either on the make command line, or by
#### or by setting the CXX_VER environment variable to CXX_5.
ifneq ($(CXX_VER),CXX_5)
  WARNING_FLAGS += -w0
  ifneq (,$(VERBOSE_WARNING))
    WARNING_FLAGS += -msg_display_number -msg_display_tag
  endif # VERBOSE_WARNING

  ifneq (,$(optimize))
    #### See ace/config-cxx-common.h for the meaning of this warning.
    #### It's not necessary on DU 5.0.
    WARNING_FLAGS += -msg_disable 1016
  endif # optimize

  ifndef ACE_DU_VERSION
    # In case this file wasn't included by platform_osf1_cxx.GNU:
    # 9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
    WARNING_FLAGS += -msg_disable 9
  endif # ACE_DU_VERSION
endif # 6.0 or later

CCFLAGS         += $(CFLAGS) $(WARNING_FLAGS)
ifeq ($(rtti),0)
  CCFLAGS += -nortti
endif # rtti
DCFLAGS         += -g -O0
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += $(CFLAGS) -ltli -lrt
MATHLIB         = -lm
OCFLAGS         += -O4
PIC             =
ARFLAGS         = cruvZ
# When libraries are archived, a hash index is automatically created
# so there is no need for ranlib
RANLIB          = @true
SOFLAGS         = -shared -use_ld_input $(ACELIB)
#### The -hidden [...] -non_hidden business avoids multiply defined
#### symbols between the shared library and libcxxstd.a.  It's the
#### workaround recommended in PTR 43-4-204.  It must appear at (or
#### very near) the end of the link line.
LIBS            := $(LIBS) \
                -hidden $(wildcard /usr/lib/cmplrs/cxx/libcxxstd.a) -non_hidden
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
