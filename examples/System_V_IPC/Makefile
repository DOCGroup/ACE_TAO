#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the System V IPC directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

DIRS  = SV_Message_Queues \
	SV_Semaphores \
	SV_Shared_Memory

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU

ifdef VXWORKS
  all.nested: any
  debug.nested: any
  profile.nested: any
  install.nested: any
  deinstall.nested: any
  clean.nested: any
  realclean.nested: any
  clobber.nested: any
  depend.nested: any
  rcs_info.nested: any
  any clean realclean:
	@echo These examples won\'t run or build for VxWorks.
else
  include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
  include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
endif # VXWORKS
