#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_server
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= as las lvs vs Globals

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
CPPFLAGS += -DSH_MEM -DNDEBUG -DSunOS -DSTAT -I/project/doc/pkg/X11/include

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/as.o .obj/as.so .shobj/as.o .shobj/as.so: as.cpp ../include/common.h ../mpeg_shared/fileio.h \
 ../mpeg_shared/routine.h ../mpeg_shared/com.h proto.h
.obj/las.o .obj/las.so .shobj/las.o .shobj/las.so: las.cpp ../include/common.h proto.h
.obj/lvs.o .obj/lvs.so .shobj/lvs.o .shobj/lvs.so: lvs.cpp ../include/common.h ../mpeg_shared/routine.h proto.h
.obj/vs.o .obj/vs.so .shobj/vs.o .shobj/vs.so: vs.cpp ../include/common.h ../mpeg_shared/routine.h \
 ../mpeg_shared/fileio.h ../mpeg_shared/com.h ../mpeg_shared/sendpt.h \
 proto.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
