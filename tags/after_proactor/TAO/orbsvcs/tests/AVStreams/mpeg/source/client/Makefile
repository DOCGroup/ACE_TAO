#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
BIN = vcr

BUILD = $(BIN)

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= vcr

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))


LIBS 	+= -lTAO -lACE -lorbsvcs
MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif


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

LDFLAGS +=  -lmpeg_shared -lmpeg_mib -lmpeg_client -lXm  -lXext -lXt -lSM -lICE  -lX11  -lsocket -lnsl -ldl
CPPFLAGS += -i -DSH_MEM -DNDEBUG -I/project/doc/pkg/X11/include -L/project/doc/pkg/X11/lib

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/vcr.o .obj/vcr.so .shobj/vcr.o .shobj/vcr.so: vcr.cpp \
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
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_client/video.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_client/proto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_client/newproto.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_shared/routine.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_client/global.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_client/util.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/mpeg_client/dither.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
