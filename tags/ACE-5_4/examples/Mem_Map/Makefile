#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the Mem_Map test directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

# move definition of DIRS below so we can exclude one of them from
# mingw build

#----------------------------------------------------------------------------
#	macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifneq  ($(mingw32), 1)
 DIRS  = IO-tests \
        file-reverse
else
 DIRS  = file-reverse
endif

include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
