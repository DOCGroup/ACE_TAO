#----------------------------------------------------------------------------
#       $Id$
#
#       Top-level Makefile for the Interface Repository
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Include macros
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#----------------------------------------------------------------------------
#       Define local rules and targets
#---------------------------------------------------------------------------
ifneq ($(CROSS-COMPILE),)
  all debug profile optimize install deinstall:  
	@echo The IFR Service requires a TAO_IDL library which isn\'t be built for CROSS-COMPILE platforms.

  clean realclean:
	@echo Nothing to clean in IFR_Service on CROSS-COMPILE platforms.
else  # ! CROSS-COMPILE
ifeq ($(findstring xlC,$(CXX)),xlC)
  all debug profile optimize install deinstall:  
	@echo The IFR Service requires the using keyword which isn\'t supported by this compiler.

  clean realclean:
	@echo Nothing to clean in IFR_Service.
else
MKLIST = Makefile.IFR_Service Makefile.tao_ifr

all debug profile optimize install deinstall clean realclean clobber depend idl_stubs:
ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
endif # ! Windows
endif # ! aix_build
endif # ! CROSS-COMPILE

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
