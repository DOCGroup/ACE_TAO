#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the SNMP+ACE examples directory
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

DIRS  = asnmp \
        examples \
        tests
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU


LDLIBS := -L$(ACE_ROOT)/ASNMP/asnmp -lasnmp $(LDLIBS:%=%$(VAR))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
