#----------------------------------------------------------------------------
#	%W%	%G%
#
#	Makefile for the ACE Adapter Web Server (JAWS) client
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= http_client

FILES   = http_handler

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	= $(addprefix $(VSHDIR),$(SHOBJ))
 
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(LDLIBS) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

test_URL.o: test_URL.cpp URL_Properties.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/SString.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/ACE.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/OS.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/config.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/stdcpp.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/OS.i \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Trace.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Log_Msg.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Log_Record.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Log_Priority.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Log_Record.i \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/ACE.i \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/SString.i \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Array.h \
 /project/cumbia/nanbor/build-sunos5.5-sunc++4.2/ace/Array.i \
 URL_Properties.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
