#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_client
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= 24bit            \
       2x2              \
       ab               \
       audio            \
       decoders         \
       file             \
       fs2              \
       fs2fast          \
       fs4              \
       global           \
       gray             \
       hybrid           \
       hybriderr        \
       info             \
       jrevdct          \
       mb_ordered       \
       mono             \
       motionvector     \
       ordered          \
       ordered2         \
       para             \
       parseblock       \
       prog             \
       ui               \
       util             \
       util32           \
       vb               \
       vd               \
       video            \
       vp               \
       Command_Handler

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT) -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs

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

LDFLAGS +=  -L/project/doc/pkg/X11/lib/ -lXm -lXt -lXext -lSM -lICE -lX11 -L/project/doc/pkg/X11/lib -lsocket -lnsl -ldl
CPPFLAGS += -i -DSH_MEM -DNDEBUG  -I/project/doc/pkg/X11/include -L/project/doc/pkg/X11/lib

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/24bit.o .obj/24bit.so .shobj/24bit.o .shobj/24bit.so: 24bit.cpp video.h dither.h proto.h
.obj/2x2.o .obj/2x2.so .shobj/2x2.o .shobj/2x2.so: 2x2.cpp video.h dither.h proto.h
.obj/ab.o .obj/ab.so .shobj/ab.o .shobj/ab.so: ab.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/filters.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/fileio.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/com.h
.obj/audio.o .obj/audio.so .shobj/audio.o .shobj/audio.so: audio.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/decoders.o .obj/decoders.so .shobj/decoders.o .shobj/decoders.so: decoders.cpp decoders.h util.h video.h proto.h
.obj/file.o .obj/file.so .shobj/file.o .shobj/file.so: file.cpp video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/fs2.o .obj/fs2.so .shobj/fs2.o .shobj/fs2.so: fs2.cpp video.h dither.h fs2.h proto.h
.obj/fs2fast.o .obj/fs2fast.so .shobj/fs2fast.o .shobj/fs2fast.so: fs2fast.cpp video.h proto.h dither.h
.obj/fs4.o .obj/fs4.so .shobj/fs4.o .shobj/fs4.so: fs4.cpp fs4.h video.h proto.h dither.h
.obj/global.o .obj/global.so .shobj/global.o .shobj/global.so: global.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i
.obj/gray.o .obj/gray.so .shobj/gray.o .shobj/gray.so: gray.cpp video.h proto.h dither.h
.obj/hybrid.o .obj/hybrid.so .shobj/hybrid.o .shobj/hybrid.so: hybrid.cpp video.h proto.h dither.h
.obj/hybriderr.o .obj/hybriderr.so .shobj/hybriderr.o .shobj/hybriderr.so: hybriderr.cpp video.h proto.h dither.h
.obj/info.o .obj/info.so .shobj/info.o .shobj/info.so: info.cpp
.obj/jrevdct.o .obj/jrevdct.so .shobj/jrevdct.o .shobj/jrevdct.so: jrevdct.cpp video.h proto.h
.obj/mb_ordered.o .obj/mb_ordered.so .shobj/mb_ordered.o .shobj/mb_ordered.so: mb_ordered.cpp video.h proto.h dither.h
.obj/mono.o .obj/mono.so .shobj/mono.o .shobj/mono.so: mono.cpp video.h proto.h dither.h
.obj/motionvector.o .obj/motionvector.so .shobj/motionvector.o .shobj/motionvector.so: motionvector.cpp video.h proto.h util.h
.obj/ordered.o .obj/ordered.so .shobj/ordered.o .shobj/ordered.so: ordered.cpp video.h proto.h dither.h
.obj/ordered2.o .obj/ordered2.so .shobj/ordered2.o .shobj/ordered2.so: ordered2.cpp video.h proto.h dither.h
.obj/para.o .obj/para.so .shobj/para.o .shobj/para.so: para.cpp video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/parseblock.o .obj/parseblock.so .shobj/parseblock.o .shobj/parseblock.so: parseblock.cpp video.h proto.h decoders.h util.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i
.obj/prog.o .obj/prog.so .shobj/prog.o .shobj/prog.so: prog.cpp video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/ui.o .obj/ui.so .shobj/ui.o .shobj/ui.so: ui.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_mib/mibload.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 interface/fb.xbm interface/play.xbm interface/step.xbm \
 interface/stop.xbm interface/loop.xbm interface/ff.xbm \
 interface/MainWindow.mib interface/Info.mib video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/util.o .obj/util.so .shobj/util.o .shobj/util.so: util.cpp video.h proto.h util.h
.obj/util32.o .obj/util32.so .shobj/util32.o .shobj/util32.so: util32.cpp video.h proto.h ui.h
.obj/vb.o .obj/vb.so .shobj/vb.o .shobj/vb.so: vb.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/filters.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/fileio.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/com.h
.obj/vd.o .obj/vd.so .shobj/vd.o .shobj/vd.so: vd.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h dither.h video.h proto.h
.obj/video.o .obj/video.so .shobj/video.o .shobj/video.so: video.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 decoders.h util.h video.h proto.h global.h ui.h
.obj/vp.o .obj/vp.so .shobj/vp.o .shobj/vp.so: vp.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h dither.h video.h proto.h ui.h
.obj/Command_Handler.o .obj/Command_Handler.so .shobj/Command_Handler.o .shobj/Command_Handler.so: Command_Handler.cpp Command_Handler.h \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/ARGV.h \
 $(ACE_ROOT)/ace/ARGV.i \
 $(TAO_ROOT)/tao/TAO.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.h \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Acceptor.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Strategies.h \
 $(ACE_ROOT)/ace/Strategies_T.h \
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Strategies_T.i \
 $(ACE_ROOT)/ace/Strategies_T.cpp \
 $(ACE_ROOT)/ace/Service_Repository.h \
 $(ACE_ROOT)/ace/Service_Types.h \
 $(ACE_ROOT)/ace/Service_Types.i \
 $(ACE_ROOT)/ace/Service_Repository.i \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.h \
 $(ACE_ROOT)/ace/Message_Queue.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
 $(ACE_ROOT)/ace/Message_Queue_T.h \
 $(ACE_ROOT)/ace/Message_Queue_T.i \
 $(ACE_ROOT)/ace/Message_Queue_T.cpp \
 $(ACE_ROOT)/ace/Message_Queue.i \
 $(ACE_ROOT)/ace/WFMO_Reactor.i \
 $(ACE_ROOT)/ace/Strategies.i \
 $(ACE_ROOT)/ace/Connector.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.h \
 $(ACE_ROOT)/ace/Task.h \
 $(ACE_ROOT)/ace/Task.i \
 $(ACE_ROOT)/ace/Task_T.h \
 $(ACE_ROOT)/ace/Task_T.i \
 $(ACE_ROOT)/ace/Task_T.cpp \
 $(ACE_ROOT)/ace/Module.h \
 $(ACE_ROOT)/ace/Module.i \
 $(ACE_ROOT)/ace/Module.cpp \
 $(ACE_ROOT)/ace/Stream_Modules.h \
 $(ACE_ROOT)/ace/Stream_Modules.i \
 $(ACE_ROOT)/ace/Stream_Modules.cpp \
 $(ACE_ROOT)/ace/Dynamic.h \
 $(ACE_ROOT)/ace/Dynamic.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Svc_Handler.i \
 $(ACE_ROOT)/ace/Svc_Handler.cpp \
 $(ACE_ROOT)/ace/Connector.i \
 $(ACE_ROOT)/ace/Connector.cpp \
 $(ACE_ROOT)/ace/Acceptor.h \
 $(ACE_ROOT)/ace/Acceptor.i \
 $(ACE_ROOT)/ace/Acceptor.cpp \
 $(TAO_ROOT)/tao/Align.h \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/Union.h \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/tao/Exception.h \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/Request.h \
 $(TAO_ROOT)/tao/Request.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Marshal.h \
 $(TAO_ROOT)/tao/Marshal.i \
 $(TAO_ROOT)/tao/CDR.h \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/POA.h \
 $(TAO_ROOT)/tao/POAC.h \
 $(TAO_ROOT)/tao/POAC.i \
 $(TAO_ROOT)/tao/Servant_Base.h \
 $(TAO_ROOT)/tao/POAS.h \
 $(TAO_ROOT)/tao/POA_CORBA.h \
 $(TAO_ROOT)/tao/POAS.i \
 $(TAO_ROOT)/tao/Active_Object_Map.h \
 $(TAO_ROOT)/tao/POA.i \
 $(TAO_ROOT)/tao/poa_macros.h \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/Connect.h \
 $(TAO_ROOT)/tao/Connect.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp \
 $(TAO_ROOT)/tao/Operation_Table.h \
 $(TAO_ROOT)/tao/debug.h \
 $(TAO_ROOT)/tao/Client_Strategy_Factory.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/default_client.h \
 $(TAO_ROOT)/tao/default_client.i \
 $(TAO_ROOT)/tao/default_server.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.h \
 $(TAO_ROOT)/tao/ORB_Strategies_T.i \
 $(TAO_ROOT)/tao/ORB_Strategies_T.cpp \
 $(TAO_ROOT)/tao/default_server.i \
 $(TAO_ROOT)/tao/IIOP_Object.h \
 $(TAO_ROOT)/tao/IIOP_Object.i \
 $(TAO_ROOT)/tao/IIOP_ORB.h \
 $(TAO_ROOT)/tao/IIOP_ORB.i \
 $(TAO_ROOT)/tao/IIOP_Interpreter.h \
 $(TAO_ROOT)/tao/GIOP.h \
 $(TAO_ROOT)/tao/GIOP.i \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Invocation.i \
 $(TAO_ROOT)/tao/Server_Request.h \
 $(TAO_ROOT)/tao/Server_Request.i \
 $(TAO_ROOT)/tao/singletons.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Video_ControlC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/Audio_ControlC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/AVStreams_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosPropertyServiceS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AVStreamsS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Property/CosPropertyService_i.h \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/AV/Endpoint_Strategy.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Null_MediaCtrlS.i \
 $(ACE_ROOT)/ace/High_Res_Timer.h \
 $(ACE_ROOT)/ace/High_Res_Timer.i \
 ctr.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/sendpt.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/filters.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/fileio.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/com.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
