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
MKLIST = Makefile.TypeCodeFactory Makefile.IFR_Service Makefile.tao_ifr

all debug profile optimize install deinstall clean realclean clobber depend:
ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
endif # ! Windows


#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
