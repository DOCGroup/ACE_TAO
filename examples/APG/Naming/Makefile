# $Id$
#
# Copyright 2003 Addison-Wesley Inc. All Rights Reserved.

MAKEFILE=Makefile

MKLIST = Makefile.netlocal \
	 Makefile.netlocal_reader \
	 Makefile.nodelocal \
	 Makefile.nodelocal_shared \
	 Makefile.nodelocal_shared_reader

## Ensure that Makefiles in MKLIST are executed in sequence during a
## parallel build because we share some files between multiple libraries.
## Also, the AIX tempinc directory will get confused.
.NOTPARALLEL:

## Makefile.*.mkfile is a dummy target which will cause
## $(MAKE) -f Makefile.* to be invoked, then it cleans
## up tempinc when needed for AIX Visual Age C++.
%.mkfile: %
	@echo $(MAKE) -f $< $(MKFILE_TARGET)
	@$(MAKE) -f $< $(MKFILE_TARGET)
	-@$(RM) -rf tempinc

# This rule invokes make again with the list of .mkfile targets as a
# parameter.  For example, if the all target is being made, make is invoked
# as follows:
#
# make -f Makefile MKFILE_TARGET=all Makefile.singles

all clean depend realclean:
ifneq ($(MKLIST),)
	@echo $(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
	@$(MAKE) -f $(MAKEFILE) MKFILE_TARGET=$@ $(addsuffix .mkfile, $(MKLIST))
endif

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
