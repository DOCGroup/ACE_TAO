#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the server-side ACE WEB services
#----------------------------------------------------------------------------

LIB     = libwebsvcs.a
SHLIB   = libwebsvcs.$(SOEXT)

FILES   =  URL_Addr
DEFS    = $(addsuffix .h,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))

LIBS   += $(ACELIB)

BUILD   = $(VLIB) $(VSHLIB)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

ifeq ($(SUPPRESS_DASH_G),1)
#### Build this target without -g on some platforms.
$(VDIR)Server_Logging_Handler.o $(VSHDIR)Server_Logging_Handler.$(SOEXT):
	$(COMPILE-NO_DASH_G.cc) -o $@ $<
endif # SUPPRESS_DASH_G

ifeq ($(shared_libs),1)
ifneq ($(SHLIB),)
CPPFLAGS     += -DACE_WEBSVCS_BUILD_DLL
endif
endif

ifeq ($(static_libs),1)
ifneq ($(LIB),)
CPPFLAGS     += -DACE_AS_STATIC_LIBS
endif
endif

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/URL_Addr.o .obj/URL_Addr.so .shobj/URL_Addr.o .shobj/URL_Addr.so: URL_Addr.cpp URL_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Sock_Connect.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
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
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Sock_Connect.i \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 websvcs_export.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Flag_Manip.h \
 $(ACE_ROOT)/ace/Flag_Manip.i \
 $(ACE_ROOT)/ace/Handle_Ops.h \
 $(ACE_ROOT)/ace/Handle_Ops.i \
 $(ACE_ROOT)/ace/Lib_Find.h \
 $(ACE_ROOT)/ace/Lib_Find.i \
 $(ACE_ROOT)/ace/Init_ACE.h \
 $(ACE_ROOT)/ace/Init_ACE.i \
 $(ACE_ROOT)/ace/ACE.i URL_Addr.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
