#----------------------------------------------------------------------------
# $Id$
#
#       Makefile for the Generic Servant example
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------
MAKEFILE=Makefile

MKLIST = Makefile.generic_servant \
         Makefile.client


## Ensure that Makefiles in MKLIST are executed in sequence during a
## parallel build because we share some files between multiple libraries.
.NOTPARALLEL:

## Makefile.server.mkfile is a dummy target which will cause
## $(MAKE) -f Makefile.server to be invoked, then it cleans
## up tempinc when needed for AIX Visual Age C++.
%.mkfile: %
	@echo $(MAKE) -f $< $(MKFILE_TARGET)
	@$(MAKE) -f $< $(MKFILE_TARGET)
	-@$(RM) -rf tempinc

# This rule invokes make again with the list of .mkfile targets as a
# parameter.  For example, if the all target is being made, make is invoked
# as follows:
# make -f Makefile MKFILE_TARGET=all Makefile.Reactive_Logging_Server_Ex

all clean depend realclean:
ifneq ($(MKLIST),)
	@echo $(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
	@$(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
endif

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
