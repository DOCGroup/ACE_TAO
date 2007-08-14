# $Id$

# This file should allow ACE to be built on Linux, using the Intel compiler.

exceptions ?= 1
debug ?= 1
optimize ?= 1
threads ?= 1
fast ?= 0
ipo ?= 0

PLATFORM_XT_CPPFLAGS=
PLATFORM_XT_LIBS=-lXt
LATFORM_XT_LDFLAGS=

PLATFORM_FL_CPPFLAGS=
PLATFORM_FL_LIBS=-lfltk -lfltk_forms -lfltk_gl
PLATFORM_FL_LDFLAGS=

PLATFORM_X11_CPPFLAGS=-I/usr/X11R6/include
PLATFORM_X11_LIBS=-lX11
PLATFORM_X11_LDFLAGS=-L/usr/X11R6/lib

PLATFORM_GL_CPPFLAGS=-I/usr/X11R6/include
PLATFORM_GL_LIBS    =-lGL
PLATFORM_GL_LDFLAGS =-L/usr/X11R6/lib

PLATFORM_GTK_CPPFLAGS=$(shell gtk-config --cflags)
PLATFORM_GTK_LIBS    =$(shell gtk-config --libs)
PLATFORM_GTK_LDFLAGS =

PLATFORM_FOX_CPPFLAGS=
PLATFORM_FOX_LIBS=-lFOX
PLATFORM_FOX_LDFLAGS=

# NOTE: we only support wxWindows over GTK
PLATFORM_WX_CPPFLAGS= $(shell wx-config --cxxflags) $(PLATFORM_GTK_CPPFLAGS)
PLATFORM_WX_LIBS    = $(shell wx-config --libs)     $(PLATFORM_GTK_LIBS)
PLATFORM_WX_LDFLAGS = $(shell wx-config --ldflags)  $(PLATFORM_GTK_LDFLAGS)

PLATFORM_BOOST_CPPFLAGS ?=
PLATFORM_BOOST_LDLAGS ?=
PLATFORM_BOOST_UTF_LIBS ?= -lboost_unit_test_framework

PLATFORM_TCL_CPPFLAGS=
PLATFORM_TCL_LIBS=-ltcl
PLATFORM_TCL_LDFLAGS=

PLATFORM_TK_CPPFLAGS=-I$(shell . /usr/lib/tkConfig.sh && echo -n $$TK_INC_DIR $$TK_DEFS)
PLATFORM_TK_LIBS=$(shell . /usr/lib/tkConfig.sh && echo -n $$TK_LIB_FLAG)
PLATFORM_TK_LDFLAGS=

PLATFORM_AIO_SUPPORT := \
       $(shell test "`ls -L /usr/lib/librt.so* /lib/librt.so*`" && echo -DACE_HAS_AIO_CALLS)

GNU_LIBPTHREAD_VERSION := $(shell getconf GNU_LIBPTHREAD_VERSION)
ifeq (NPTL, $(word 1,$(GNU_LIBPTHREAD_VERSION)))
  CPPFLAGS += -DACE_HAS_LINUX_NPTL
endif

SYSARCH := $(shell uname -m)
ssl ?= 0
ifeq ($(ssl),1)
  # Some Linux OpenSSL installations compile in Kerberos support.  Add
  # the Kerberos include path to preprocessor include path.
  #
  # We should probably also add the Kerberos libraries to
  # PLATFORM_SSL_LIBS but we can't be sure if they are needed without
  # a more sophisticated check.  This will only be a problem when
  # statically linking the OpenSSL library.  The majority of
  # installations use shared OpenSSL libraries so we should be okay,
  # at least until we migrate to Autoconf.
  PLATFORM_SSL_CPPFLAGS += -I/usr/kerberos/include
endif # ssl

insure ?= 0
ifeq ($(insure),1)
  CC  = insure
  CXX = insure
else
  CC  = icc
  CXX = icpc
endif

ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX) --version)
endif

ifeq (8.0,$(findstring 8.0,$(CXX_VERSION)))
  CFLAGS += -wd1476,1505
endif
ifeq (8.1,$(findstring 8.1,$(CXX_VERSION)))
  CFLAGS += -wd1476,1505,1572 -no-gcc
endif
ifeq (9.0,$(findstring 9.0,$(CXX_VERSION)))
  CFLAGS += -wd1684
endif
ifeq (9.1,$(findstring 9.1,$(CXX_VERSION)))
  CFLAGS += -wd1684
endif
ifeq (10.0,$(findstring 10.0,$(CXX_VERSION)))
  CFLAGS += -wd1684
  no_hidden_visibility ?= 1
endif

no_hidden_visibility ?= 1

ifeq ($(inline),0)
  CPPFLAGS += -fno-inline
endif

ifeq ($(fast),1)
  CPPFLAGS += -fast
endif

ifeq ($(ipo),1)
  CPPFLAGS += -ipo
endif

CFLAGS  += -w1
ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT $(PLATFORM_AIO_SUPPORT)
endif # threads

CCFLAGS += $(CFLAGS) $(IMPLICIT_TEMPLATES_FLAG) -ip
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

PLATFORM_QT_CPPFLAGS ?= -I$(QTDIR)/include
PLATFORM_QT_LIBS ?= -lqt-mt
PLATFORM_QT_LDFLAGS ?= -L$(QTDIR)/lib

OCFLAGS += -O3

# Disable floating point optimizer solves problem with
# min/max float values in the TAO_IDL compiler tests
# If these options are not passed we can get overflows
# when testing min/max
ifeq ($(SYSARCH),ia64)
  CPPFLAGS += -mp
else
  CPPFLAGS += -fp-model double
endif

ifeq ($(optimize),0)
  # Disable all optimizing in code
  CPPFLAGS += -O0
endif

PIC     = -fPIC
AR      = ar
ARFLAGS = rsuv
RANLIB  = @true
SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

ifeq ($(shared_libs), 1)
  ifneq ($static_libs_only), 1)
    LDFLAGS += -Wl,-E

    ifneq ($(no_hidden_visibility),1)
      CCFLAGS += -fvisibility=hidden
    else
      CPPFLAGS += -DACE_HAS_CUSTOM_EXPORT_MACROS=0
    endif # no_hidden_visibility
  endif
endif


# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif

sctp ?=
# support for OpenSS7 SCTP
ifeq ($(sctp),openss7)
  PLATFORM_SCTP_CPPFLAGS+= -DACE_HAS_OPENSS7_SCTP
  PLATFORM_SCTP_LDFLAGS?=
  PLATFORM_SCTP_LIBS?=
endif

# support for LKSCTP (Linux Kernel 2.5)
ifeq ($(sctp),lksctp)
  PLATFORM_SCTP_CPPFLAGS+= -DACE_HAS_LKSCTP
  PLATFORM_SCTP_LDFLAGS?= -L/usr/local/lib
  PLATFORM_SCTP_LIBS?= -lsctp
endif

