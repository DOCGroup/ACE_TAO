# -*- Makefile -*-

# This file should allow ACE to be built for Android 2.3.1 (API Level 9)
# or greater, by cross compiling on Linux.

# We always include config-android.h on Android platforms.
ACE_PLATFORM_CONFIG ?= config-android.h

# The standalone gcc compilers in NDK r6-r9 have issues with the visibility.
no_hidden_visibility ?= 1

# as of NDK r6 inlining is required
inline ?= 1

debug ?= 1
optimize ?= 1
threads ?= 1
insure ?= 0

LSB_RELEASE_ID := $(shell lsb_release -i 2> /dev/null || echo Distributor ID: Unknown)
LSB_RELEASE_RELEASE := $(shell lsb_release -r 2> /dev/null || echo Release: Unknown)

PLATFORM_XT_CPPFLAGS=
PLATFORM_XT_LIBS=-lXt
PLATFORM_XT_LDFLAGS=

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

PLATFORM_FOX_CPPFLAGS ?= -I/usr/include/fox
PLATFORM_FOX_LIBS     ?= -lFOX
PLATFORM_FOX_LDFLAGS  ?=

# NOTE: we only support wxWindows over GTK
PLATFORM_WX_CPPFLAGS= $(shell wx-config --cxxflags) $(PLATFORM_GTK_CPPFLAGS)
PLATFORM_WX_LIBS    = $(shell wx-config --libs)     $(PLATFORM_GTK_LIBS)
PLATFORM_WX_LDFLAGS = $(shell wx-config --ldflags)  $(PLATFORM_GTK_LDFLAGS)

PLATFORM_BOOST_CPPFLAGS ?=
PLATFORM_BOOST_LDLAGS ?=
PLATFORM_BOOST_UTF_LIBS ?= -lboost_unit_test_framework

ifeq ($(buildbits),64)
PLATFORM_TK_CPPFLAGS=$(shell . /usr/lib64/tkConfig.sh && echo -n $$TK_INCLUDE_SPEC $$TK_DEFS)
PLATFORM_TK_LIBS=$(shell . /usr/lib64/tkConfig.sh && echo -n $$TK_LIB_FLAG)
else
PLATFORM_TK_CPPFLAGS=$(shell . /usr/lib/tkConfig.sh && echo -n $$TK_INCLUDE_SPEC $$TK_DEFS)
PLATFORM_TK_LIBS=$(shell . /usr/lib/tkConfig.sh && echo -n $$TK_LIB_FLAG)
endif
PLATFORM_TK_LDFLAGS=

ifeq ($(buildbits),64)
PLATFORM_TCL_CPPFLAGS=$(shell . /usr/lib64/tclConfig.sh && echo -n $$TCL_INCLUDE_SPEC $$TCL_DEFS)
PLATFORM_TCL_LIBS=$(shell . /usr/lib64/tclConfig.sh && echo -n $$(eval echo $$TCL_LIB_FLAG))
else
PLATFORM_TCL_CPPFLAGS=$(shell . /usr/lib/tclConfig.sh && echo -n $$TCL_INCLUDE_SPEC $$TCL_DEFS)
PLATFORM_TCL_LIBS=$(shell . /usr/lib/tclConfig.sh && echo -n $$(eval echo $$TCL_LIB_FLAG))
endif
PLATFORM_TCL_LDFLAGS=

PLATFORM_QT_CPPFLAGS ?= -I$(QTDIR)/include
PLATFORM_QT_LIBS ?= -lqt-mt
PLATFORM_QT_LDFLAGS ?= -L$(QTDIR)/lib

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

GNU_LIBPTHREAD_VERSION := $(shell getconf GNU_LIBPTHREAD_VERSION 2> /dev/null || echo Unknown)
ifeq (NPTL, $(word 1,$(GNU_LIBPTHREAD_VERSION)))
  NPTL_VERS := $(subst ., ,$(word 2,$(GNU_LIBPTHREAD_VERSION)))
  ifneq (0, $(word 1,$(NPTL_VERS)))
    nptl ?= 1
  endif
endif
nptl ?= 0
ifeq ($(nptl),0)
  CPPFLAGS += -DACE_LACKS_LINUX_NPTL
endif

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

SYSARCH := $(shell uname -m)

# as of API level 16 executables can be linked as Position Independent Executables
# as of API level 21 PIE is mandatory
PIE		?= -pie
PIC      = -fPIC
AR      ?= ar
ARFLAGS ?= rsuv
RANLIB   = @true

# end of: include most of platform_linux_common.GNU

#No rwho on Android
rwho = 0

#Standalone toolchain required.
#Required environment variables are
#$NDK - install directory for android NDK, at least r5c
#$SDK - install directory for android SDK
#$SYSROOT - $NDK/platform/android-9
#$TOOLCHAIN/bin in path, $TOOLCHAIN is root of standalone toolchain
#
#Optionally ANDROID_ABI can be set to select specific architecture ABIs like armeabi-v7a or neon
#ANDROID_ABI ?= arm64-v8a
#
#Toolchain defaults to arm architecture
ANDROID_ARCH ?= arm

#SYSROOT ?= $(NDK)/platforms/android-9/arch-$(ANDROID_ARCH)

ifeq (,$(CROSS_COMPILE))
  ifeq ($(ANDROID_ARCH),arm)
    CROSS_COMPILE=arm-linux-androideabi-
  else
    ifeq ($(ANDROID_ARCH),x86)
      CROSS_COMPILE=i686-linux-android-
    else
      CROSS_COMPILE=unknown-android-arch-
    endif
  endif
endif

# fix this
pipes ?= 1

# set architecture specific build flags
ifeq (arm,$(findstring arm,$(ANDROID_ARCH)))
  # NDK says -Wl,--fix-cortex-a8 is required for working around a CPU bug 
  # in some Cortex-A8 implementations
  
  # Set ABI specific flags
  ifeq ($(ANDROID_ABI),armeabi-v7a)
    FLAGS_C_CC += -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
    LDFLAGS    += -march=armv7-a -Wl,--fix-cortex-a8
  endif

  ifeq ($(ANDROID_ABI),arm64-v8a)
    CROSS_COMPILE=aarch64-linux-android-
    FLAGS_C_CC += -march=armv8-a 
    LDFLAGS    += -march=armv8-a 
  endif 

  ifeq ($(ANDROID_ABI),neon)
    FLAGS_C_CC += -march=armv7-a -mfloat-abi=softfp -mfpu=neon
    LDFLAGS    += -march=armv7-a -Wl,--fix-cortex-a8
  endif

endif

FLAGS_C_CC += -W -Wall -Wpointer-arith -Wno-psabi
ifeq ($(threads),1)
    CPPFLAGS  += -D_REENTRANT $(PLATFORM_AIO_SUPPORT)
endif # threads

ifeq ($(buildbits),32)
  FLAGS_C_CC += -m32
  LDFLAGS    += -m32
endif
ifeq ($(buildbits),64)
  FLAGS_C_CC += -m64
endif

# DT_RUNPATH is preferred over DT_RPATH, but the linker will only use it when
# this extra flag option (enable-new-dtags) is present
LD_RPATH_FLAGS += -Wl,--enable-new-dtags

# Rely on _GNU_SOURCE to set these defaults defined in /usr/include/features.h
# instead of setting them directly here (older versions of gcc don't set it
# for you): _SVID_SOURCE _BSD_SOURCE _POSIX_SOURCE _POSIX_C_SOURCE=199506L, ...
CPPFLAGS += -D_GNU_SOURCE

DCFLAGS  += -ggdb
DCCFLAGS += -ggdb
DLD      = $(CXX)
LD       = $(CXX) $(PIE)
# make sure to add the required libs for RTTI and exceptions (libsupc++)
# and a shared STL lib (libgnustl_shared by default)
static_libs_only ?=
ANDROID_STL_DLL ?= gnustl_shared
LIBS     += -ldl -lsupc++
ifneq ($(static_libs_only),1)
  LIBS   += -l$(ANDROID_STL_DLL)
endif

ifeq ($(optimize),1)
  SOFLAGS += -Wl,-O3
endif

SOFLAGS += $(CPPFLAGS) -shared
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<; \
          $(SOLINK.cc) -o $@ $(LDFLAGS) $(VSHDIR)$*.o
PRELIB  = @true

# Test for template instantiation, add to SOFLAGS if SONAME set,
# add -E to LDFLAGS if using GNU ld
#
include $(ACE_ROOT)/include/makeinclude/platform_g++_common.GNU

OCFLAGS ?= -O3
OCCFLAGS ?= -O3

# Added line below to support "Executable Shared Object" files (as
# needed by the service configurator).
# Marius Kjeldahl <mariusk@sn.no, marius@funcom.com>
ifeq ($(threads),1)
    ESOBUILD = $(COMPILEESO.cc) $(PIC) -shared -o $(VSHDIR)$*.so $<
    ifndef PRELIB
       PRELIB = @true
    endif # ! PRELIB
endif
