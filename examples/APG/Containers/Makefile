# $Id$
#
# Copyright 2003 Addison-Wesley Inc. All Rights Reserved.

BIN =	DLList \
	Stacks \
	Queues \
	Array \
	Sets \
	Map_Manager \
	Map_Manager_Specialization \
	Hash_Map \
	Hash_Map_Hash \
	RB_Tree \
	RB_Tree_Functors \
	Allocator

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif
