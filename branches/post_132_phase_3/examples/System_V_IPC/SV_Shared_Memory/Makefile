#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for tests of the SV_Shared_Memory wrapper
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN     = SV_Shared_Memory_Test

LSRC    = $(addsuffix .cpp,$(BIN)) 

LDLIBS  = 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD   = $(VBIN)

INSTALL =

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/SV_Shared_Memory_Test.o .obj/SV_Shared_Memory_Test.so .shobj/SV_Shared_Memory_Test.o .shobj/SV_Shared_Memory_Test.so: SV_Shared_Memory_Test.cpp \
  $(ACE_ROOT)/ace/SV_Shared_Memory.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/SV_Shared_Memory.i SV_Shared_Memory_Test.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
