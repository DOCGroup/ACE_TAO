#
# $Id$
#

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

BIN     = leader_follower

FILES   = $(BIN)
LSRC    = $(addsuffix .cpp,$(FILES))
LOBJ    = $(addsuffix .o,$(FILES))
CPPFLAGS += -I..

BUILD   = $(VOBJS) $(VBIN)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

