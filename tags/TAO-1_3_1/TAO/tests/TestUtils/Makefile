#----------------------------------------------------------------------------
#  $Id$
#
#  Makefile for TAO TestUtils.
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libTAO_TestUtils
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
CCFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao

ifdef VXWORKS
LSRC = TestCombinedThreads.cpp Thread_Manager.cpp ORB_Core.cpp

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

Thread_Manager.cpp: $(ACE_ROOT)/ace/Thread_Manager.cpp
	$(ACE_ROOT)/bin/vxworks_modify.pl -threadmanager $^ $@

ORB_Core.cpp: $(ACE_ROOT)/TAO/tao/ORB_Core.cpp
	$(ACE_ROOT)/bin/vxworks_modify.pl -orbcore $^ $@

realclean:
	-$(RM) Thread_Manager.cpp ORB_Core.cpp
else
all debug profile optimize install deinstall:
	@echo This library is not required for non-VxWorks platforms.
endif


#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/TestCombinedThreads.o .obj/TestCombinedThreads. TestCombinedThreads.o TestCombinedThreads.: TestCombinedThreads.cpp TestCombinedThreads.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
