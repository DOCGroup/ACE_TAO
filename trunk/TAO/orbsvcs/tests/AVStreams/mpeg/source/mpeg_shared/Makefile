#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_shared
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= com fileio filters routine sendpt

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

##LIBS 	+= -lTAO -lACE

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

##LDFLAGS += -L$(TAO_ROOT)/tao
##CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)#-H

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

com.o: com.cpp ../include/common.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/OS.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/inc_user_config.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/config.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/streams.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Basic_Types.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Basic_Types.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/OS.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Trace.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Msg.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Record.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/ACE.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Version.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/ACE.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Priority.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Record.i fileio.h \
 routine.h com.h
fileio.o: fileio.cpp routine.h ../include/common.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/OS.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/inc_user_config.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/config.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/streams.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Basic_Types.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Basic_Types.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/OS.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Trace.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Msg.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Record.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/ACE.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Version.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/ACE.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Priority.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Record.i fileio.h
filters.o: filters.cpp filters.h
routine.o: routine.cpp routine.h ../include/common.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/OS.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/inc_user_config.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/config.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/streams.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Basic_Types.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Basic_Types.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/OS.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Trace.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Msg.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Record.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/ACE.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Version.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/ACE.i \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Priority.h \
 /home/cs/student/naga/ACE/build/Sun_C++//ace/Log_Record.i
sendpt.o: sendpt.cpp sendpt.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
