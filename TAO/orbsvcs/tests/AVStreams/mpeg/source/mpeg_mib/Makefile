#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_mib
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= mibButton \
       mibDrawArea \
       mibFrame \
       mibLabel \
       mibList \
       mibMenu \
       mibRadioBox \
       mibScale \
       mibScrollBar \
       mibTextBig \
       mibTextBox \
       mibToggle \
       mibload \

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

.obj/mibButton.o .obj/mibButton.so .shobj/mibButton.o .shobj/mibButton.so: mibButton.cpp mibload.h mibwidgets.h
.obj/mibDrawArea.o .obj/mibDrawArea.so .shobj/mibDrawArea.o .shobj/mibDrawArea.so: mibDrawArea.cpp mibload.h mibwidgets.h
.obj/mibFrame.o .obj/mibFrame.so .shobj/mibFrame.o .shobj/mibFrame.so: mibFrame.cpp mibload.h mibwidgets.h
.obj/mibLabel.o .obj/mibLabel.so .shobj/mibLabel.o .shobj/mibLabel.so: mibLabel.cpp mibload.h mibwidgets.h
.obj/mibList.o .obj/mibList.so .shobj/mibList.o .shobj/mibList.so: mibList.cpp mibload.h mibwidgets.h
.obj/mibMenu.o .obj/mibMenu.so .shobj/mibMenu.o .shobj/mibMenu.so: mibMenu.cpp mibload.h mibwidgets.h
.obj/mibRadioBox.o .obj/mibRadioBox.so .shobj/mibRadioBox.o .shobj/mibRadioBox.so: mibRadioBox.cpp mibload.h mibwidgets.h
.obj/mibScale.o .obj/mibScale.so .shobj/mibScale.o .shobj/mibScale.so: mibScale.cpp mibload.h mibwidgets.h
.obj/mibScrollBar.o .obj/mibScrollBar.so .shobj/mibScrollBar.o .shobj/mibScrollBar.so: mibScrollBar.cpp mibload.h mibwidgets.h
.obj/mibTextBig.o .obj/mibTextBig.so .shobj/mibTextBig.o .shobj/mibTextBig.so: mibTextBig.cpp mibload.h mibwidgets.h
.obj/mibTextBox.o .obj/mibTextBox.so .shobj/mibTextBox.o .shobj/mibTextBox.so: mibTextBox.cpp mibload.h mibwidgets.h
.obj/mibToggle.o .obj/mibToggle.so .shobj/mibToggle.o .shobj/mibToggle.so: mibToggle.cpp mibload.h mibwidgets.h
.obj/mibload.o .obj/mibload.so .shobj/mibload.o .shobj/mibload.so: mibload.cpp mibload.h mibwidgets.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
