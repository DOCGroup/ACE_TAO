#----------------------------------------------------------------------------
#
# $Id$
#
#       Makefile for all the ACE ``one-button' tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN =   mm

PSRC=$(addsuffix .cpp,$(BIN))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ex03.html : mm.cpp
	../colorize < mm.cpp > ex03.html
