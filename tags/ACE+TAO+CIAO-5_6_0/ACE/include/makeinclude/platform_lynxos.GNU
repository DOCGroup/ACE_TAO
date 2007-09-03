# $Id$
#
# LynxOS with g++.  Defaults to LynxOS Version 4.0.0.  For Version 4.2.0,
# for example, add "VERSION=4.2.0" to your make invocation.

#### NOTE:  It's best to create a file that includes this one, instead
#### of symlinking it.  That way, you can easily override the default
#### configuration.  For example, to configure for a PowerPC target, I
#### use a include/makeinclude/platform_macros.GNU that contains the
#### following lines (without comment markers):
####
####     LYNXTARGET = ppc
####     include $(ACE_ROOT)/include/makeinclude/platform_lynxos.GNU
####
#### Similarly, the default VERSION and HOST_OS settings (see below)
#### can be overridden for your particular platform.

ifeq ($(shell uname -s),LynxOS)
  export VERSION=$(shell uname -r)

  # LynxOS's mkdir needs -f option to prevent it returning error
  # if directory already exists.
  ACE_MKDIR=mkdir -pf
else
  CROSS-COMPILE = 1

  #### The following may need to be customized for your host or target type.
  #### Or, you can source the appropriate (for your shell) Lynx SETUP file
  #### before running "make".

  ifeq (,$(VERSION))
    export VERSION=4.0.0
  endif # VERSION

  ifeq (,$(LYNXTARGET))
    export LYNXTARGET = x86
  endif # LYNXTARGET

  ifeq (,$(OBJSFORMAT))
    ifeq (x86,$(LYNXTARGET))
      OBJSFORMAT = coff
    else  # ! x86
      OBJSFORMAT = xcoff
    endif # ! x86
    export OBJSFORMAT
  endif # OBJSFORMAT

  ifeq (,$(HOST_OS))
    export HOST_OS = sunos
  endif # HOST_OS

  ifeq (,$(HOST_OS_REVISION))
    #### Added for 3.1.0
    export HOST_OS_REVISION = `uname -r`
  endif # HOST_OS_REVISION

  ifeq (,$(HOST_OS_TYPE))
    #### Added for 3.1.0
    export HOST_OS_TYPE = solaris
  endif # HOST_OS_TYPE

  ifeq (,$(ENV_PREFIX))
    export ENV_PREFIX = /usr/lynx/$(VERSION)/$(LYNXTARGET)
  endif # ENV_PREFIX

  ifeq (,$(findstring $(ENV_PREFIX),$(PATH)))
    export PATH:=$(ENV_PREFIX)/cdk/$(HOST_OS)-$(OBJSFORMAT)-$(LYNXTARGET)/bin:$(ENV_PREFIX)/cdk/$(HOST_OS)-$(OBJSFORMAT)-$(LYNXTARGET)/usr/bin:$(PATH)
  endif # PATH
endif # ! LynxOS

LYNXOS_MAJOR = $(shell echo $(VERSION) | awk -F. '{print $$1;}')
LYNXOS_MINOR = $(shell echo $(VERSION) | awk -F. '{print $$2;}')

PLATFORM_XT_CPPFLAGS=
PLATFORM_XT_LIBS=-lXm -lXt
PLATFORM_XT_LDFLAGS=

PLATFORM_X11_CPPFLAGS=
PLATFORM_X11_LIBS=-lXpm -lXmu -lXext -lX11 -lSM -lICE
PLATFORM_X11_LDFLAGS=

PLATFORM_TCL_CPPFLAGS=
PLATFORM_TCL_LIBS=-ltcl
PLATFORM_TCL_LDFLAGS=

PLATFORM_TK_CPPFLAGS=$(shell . /usr/lib/tkConfig.sh && echo -n $$TK_INC_DIR $$TK_DEFS)
PLATFORM_TK_LIBS=-ltk $(PLATFORM_X11_LIBS)
PLATFORM_TK_LDFLAGS=

shared_libs ?= 1
optimize ?= 1
debug ?= 1
pipes ?= 1
threads ?= 1
exceptions ?= 1

ifeq ($(threads),1)
  CFLAGS  += -mthreads
  SOFLAGS += -mthreads
endif

ifeq ($(shared_libs),1)
  CPPFLAGS += -DACE_HAS_SVR4_DYNAMIC_LINKING
  CFLAGS   += -mshared
  SOFLAGS  += -mshared
  LIBS     += -ldl
else
  LIBS     += -lnetinet -lnsl
endif

CC              = gcc
CXX             = g++
PIC             = -fPIC
CFLAGS          += -Wpointer-arith -Wall -fsigned-char $(PIC)
CFLAGS          += -DACE_LYNXOS_MAJOR=$(LYNXOS_MAJOR) -DACE_LYNXOS_MINOR=$(LYNXOS_MINOR)
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
OCFLAGS         += -O2
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
SOFLAGS         += $(CPPFLAGS) -shared
SOBUILD         = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
                  $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB          = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

CCFLAGS         += $(CFLAGS) $(TEMPLATES_FLAG)

ifeq ($(CXX_MAJOR_VERSION), 3)
  LDFLAGS       += -L/usr/lib/thread
endif

# To save much disk space, strip all executables.  Comment the
# following line out if you want to debug.  Or, add "POSTLINK="
# to your make invocation.
ifeq ($(static_libs),1)
  POSTLINK       = ; strip $@
endif
