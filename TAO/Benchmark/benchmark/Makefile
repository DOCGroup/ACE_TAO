#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the CORBA_Benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LIBNAME  = libbenchmark
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

FILES 	= driver \
	  marshal_options \
	  marshal_results \
	  marshal_proxy

LSRC	= $(addsuffix .cpp,$(FILES))

LDLIBS = -lACE
LIBS = -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) $(VLIB)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
# Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

CPPFLAGS += -I$(TAO_ROOT)/Benchmark/

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/driver.o .shobj/driver.so: driver.cpp \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/driver.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i
.obj/marshal_options.o .shobj/marshal_options.so: marshal_options.cpp \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_options.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_options.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/marshal_results.o .shobj/marshal_results.so: marshal_results.cpp \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_results.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_results.i
.obj/marshal_proxy.o .shobj/marshal_proxy.so: marshal_proxy.cpp \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_proxy.h \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_options.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_options.i \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_results.h \
 $(ACE_ROOT)/ace/Profile_Timer.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 $(ACE_ROOT)/ace/Profile_Timer.i \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_results.i \
 $(ACE_ROOT)/TAO/Benchmark/benchmark/marshal_proxy.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
