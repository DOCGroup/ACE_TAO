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

LDFLAGS +=  -lmpeg_shared -lmpeg_mib -lmpeg_client -lXm  -lXext -lXt -lSM -lICE  -lX11  -lsocket -lnsl -ldl
CPPFLAGS += -i -I../mpeg_client/ -DSH_MEM -DNDEBUG -DSunOS -DSTAT -I/project/doc/pkg/X11/include -L../mpeg_client/ -L/project/doc/pkg/X11/lib

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/vcr.o .obj/vcr.so .shobj/vcr.o .shobj/vcr.so: vcr.cpp ../mpeg_client//video.h ../mpeg_client//proto.h \
 ../include/common.h ../mpeg_client//newproto.h \
 ../mpeg_client//../mpeg_shared/routine.h ../mpeg_client//global.h \
 ../mpeg_client//util.h ../mpeg_client//dither.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
