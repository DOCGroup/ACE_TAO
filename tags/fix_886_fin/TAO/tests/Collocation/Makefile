#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Include macros
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

MKLIST = \
	Makefile.stub \
	Makefile.impl \
	Makefile.test

all debug profile optimize install deinstall clean realclean clobber depend idl_stubs:
ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
endif # ! Windows
