#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_client
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

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
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
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
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/decoders.o .obj/decoders.so .shobj/decoders.o .shobj/decoders.so: decoders.cpp decoders.h util.h video.h proto.h
.obj/file.o .obj/file.so .shobj/file.o .shobj/file.so: file.cpp video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
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
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
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
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/parseblock.o .obj/parseblock.so .shobj/parseblock.o .shobj/parseblock.so: parseblock.cpp video.h proto.h decoders.h util.h
.obj/prog.o .obj/prog.so .shobj/prog.o .shobj/prog.so: prog.cpp video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h
.obj/ui.o .obj/ui.so .shobj/ui.o .shobj/ui.so: ui.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_mib/mibload.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i interface/fb.xbm \
 interface/play.xbm interface/step.xbm interface/stop.xbm \
 interface/loop.xbm interface/ff.xbm interface/MainWindow.mib \
 interface/Info.mib video.h \
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
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
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
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h dither.h video.h proto.h
.obj/video.o .obj/video.so .shobj/video.o .shobj/video.so: video.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i decoders.h util.h \
 video.h proto.h global.h ui.h
.obj/vp.o .obj/vp.so .shobj/vp.o .shobj/vp.so: vp.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/Version.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 global.h dither.h video.h proto.h ui.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
