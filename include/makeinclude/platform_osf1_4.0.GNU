# $Id$

# This platform macros file is intended to work with
# Digital UNIX 4.0 (OSF/1 4.0), using the cxx compiler..

debug = 1
exceptions = 1
rtti = 1

CC              = cxx
CXX             = $(CC)
#### Note: -pthread uses POSIX threads.  If you want to use DCE threads
####       instead, change "-pthread" to "-threads".
CFLAGS          += -pthread

ifndef CXX_VERSION
  CXX_VERSION := $(shell $(CXX) -V)
endif # CXX_VERSION

CXX_1016 =
CXX_1136 =
CXX_1180 =

ifneq (,$(optimize))
  CXX_1016 = ,1016
endif # optimize

#### 6.1-029 and later support msg 1136.  We disable it by default.
ifneq (6.0-,$(findstring 6.0-,$(CXX_VERSION)))
ifeq (6.1-,$(findstring 6.1-,$(CXX_VERSION)))
  ifeq (6.1-029,$(findstring 6.1-029,$(CXX_VERSION)))
    CXX_1136 = ,1136
  endif # 6.1-029
else  # 6.2 or later
  CXX_1136 = ,1136
  #### 6.2-009 has 1180, but 6.2-007 doesn't.
  CXX_1180 = ,1180
endif # 6.2 or later
endif # ! 6.0

#### These flags can only be used with cxx Version 6.0 and later.
#### They are enabled by default; they can be disabled by settting
#### CXX_VER to CXX_5 either on the make command line, or by
#### or by setting the CXX_VER environment variable to CXX_5.
# WARNING_FLAGS +=
#    9: nested comment not allowed.  But there's one in /usr/include/pdsc.h!
#  174: expr_has_no_effect
#  193: zero used for undefined preprocessing identifier
#  236: controlling expression is constant
#  401: base_class_with_nonvirtual_dtor
#  610: nonoverriding_function_decl
#  835: unreferenced_function_param
#  839: no_corresponding_delete (until this is fixed in libTAO)
# 1016: expected type is incompatible with declared type of int (in bzero ())
# 1136: conversion to integral type of smaller size could lose data
# 1180: statement causes unreachable return
ifneq ($(CXX_VER),CXX_5)
  WARNING_FLAGS += -w0 -msg_display_number
  WARNING_FLAGS += \
    -msg_disable 9,174,193,236,401,610,835,839$(CXX_1016)$(CXX_1136)$(CXX_1180)
endif # 6.0 or later

CCFLAGS         += $(CFLAGS) -ptr ptrepository $(WARNING_FLAGS)
ifeq ($(rtti),)
  CCFLAGS += -nortti
endif # rtti
DCFLAGS         += -g -O0
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += $(CFLAGS) -ptr ptrepository -ltli -lrt
MATHLIB         = -lm
OCFLAGS         += -O4
PIC             =
ARFLAGS         = cruvZ
# When libraries are archived, a hash index is automatically created
# so there is no need for ranlib
RANLIB          = @true
#### The -hidden [...] -non_hidden business avoids multiply defined
#### symbols between the shared library and libcxxstd.a.  It's the
#### workaround recommended in PTR 43-4-204.
SOFLAGS         = -hidden /usr/lib/cmplrs/cxx/libcxxstd.a -non_hidden \
                  -shared -use_ld_input $(ACELIB)
SOBUILD         = $(RM) $@; $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $< && \
                  ln $(VSHDIR)$*.o $@

ifndef exceptions
  #### Disable the default exception handling of cxx >= 6.0.
  #### This is untested.  exceptions=1 is the default, so it's not used
  #### by default.
  ifeq (6.,$(findstring 6.,$(CXX_VERSION)))
    CCFLAGS += -nocleanup
  endif # 6.x
endif # ! exceptions
