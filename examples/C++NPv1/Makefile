#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the example programs in "C++ Network Programming,
#	Vol. 1, Resolving Complexity Using Patterns and ACE"
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------
MAKEFILE=Makefile

MKLIST = Makefile.Iterative_Logging_Server \
	Makefile.Logging_Client \
	Makefile.Process_Per_Connection_Logging_Server \
	Makefile.Reactive_Logging_Server \
	Makefile.Reactive_Logging_Server_Ex \
	Makefile.RT_Thread_Per_Connection_Logging_Server \
	Makefile.Thread_Per_Connection_Logging_Server

## Makefile.Reactive_Logging_Server_Ex.mkfile is a dummy target which will cause
## $(MAKE) -f Makefile.Svc_Utils to be invoked
%.mkfile: %
	@echo $(MAKE) -f $< $(MKFILE_TARGET)
	@$(MAKE) -f $< $(MKFILE_TARGET)

# This rule invokes make again with the list of .mkfile targets as a
# parameter.  For example, if the all target is being made, make is invoked
# as follows:
#
# make -f Makefile MKFILE_TARGET=all Makefile.Reactive_Logging_Server_Ex 

all clean depend realclean:
ifneq ($(MKLIST),)
	@echo $(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
	@$(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
endif

