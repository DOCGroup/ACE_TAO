# $Id$
#
# Copyright 2003 Addison-Wesley Inc. All Rights Reserved.

BIN = Get_Opt Get_Opt_Long ARGV_Example
LIB     = libHA_Status.a
SHLIB   = libHA_Status.$(SOEXT)

LSRC    = HA_Status.cpp

LIBS   += $(ACELIB)

BUILD   = $(VLIB) $(VSHLIB) $(VBIN)

SOURCES = HA_Status.cpp Get_Opt.cpp Get_Opt_Long.cpp ARGV_Example.cpp

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
