#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile to compile the Demux application
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN 	= tao
LIB     = libtao.a
SHLIB   = libtao.so

FILES   = tao \
	  gen \
	  client \
	  stub \
	  skel \
	  impl \
	  perf \
	  objcreate

LSRC    = $(addsuffix .cpp,$(FILES)) 
LOBJ    = $(addsuffix .o,$(FILES))
SHOBJ   = $(addsuffix .so,$(FILES))

LDLIBS  = -ltao
LIBS    = -lACE

VLDLIBS = $(LDLIBS:%=%$(VAR))

#BUILD   = $(VLIB) $(VSHLIB) $(SHLIBA) $(VBIN)
BUILD   = $(VSHLIB) $(SHLIBA) $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include         $(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include         $(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include         $(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include         $(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include         $(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include         $(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include         $(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Include macros and targets
# 	Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

CPPFLAGS   += -DDEBUG

#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/tao.o .shobj/tao.so: tao.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/gen.o .shobj/gen.so: gen.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/client.o .shobj/client.so: client.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/stub.o .shobj/stub.so: stub.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/skel.o .shobj/skel.so: skel.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/impl.o .shobj/impl.so: impl.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/perf.o .shobj/perf.so: perf.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i
.obj/objcreate.o .shobj/objcreate.so: objcreate.cpp tao.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 $(WRAPPER_ROOT)/ace/Get_Opt.h \
 $(WRAPPER_ROOT)/ace/Get_Opt.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
