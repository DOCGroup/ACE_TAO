#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libmpeg_shared
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

#TAO_IDLFLAGS = -Wb,export_macro=TAO_ORBSVCS_Export -Wb,export_include=orbsvcs_export.h

IDL_FILES = Video_ControlS Video_ControlC Audio_ControlS Audio_ControlC ReceiverS ReceiverC MMDevice_ExporterC MMDevice_ExporterS

# On non-Windows environment, we should at least define
# the export_include IDL flag.
override TAO_IDLFLAGS += \
	-I$(TAO_ROOT)/orbsvcs/orbsvcs

FILES = $(IDL_FILES)

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

FILES += com fileio filters routine sendpt 

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG) -I$(TAO_ROOT)/orbsvcs/orbsvcs

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT)

LIBS 	+= -lTAO -lACE

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifneq (AV,$(findstring AV,$(TAO_ORBSVCS)))
  BUILD =
  override INSTALL =
endif # ! AV

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
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)
CPPFLAGS += -DSH_MEM -DNDEBUG

.PRECIOUS: Video_ControlS.cpp Video_ControlC.cpp Audio_ControlS.cpp Audio_ControlC.cpp

idl_stubs: $(addsuffix .h, $(IDL_FILES))

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/com.o .obj/com.so .shobj/com.o .shobj/com.so: com.cpp \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i fileio.h \
 routine.h com.h
.obj/fileio.o .obj/fileio.so .shobj/fileio.o .shobj/fileio.so: fileio.cpp routine.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i fileio.h
.obj/filters.o .obj/filters.so .shobj/filters.o .shobj/filters.so: filters.cpp filters.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i
.obj/routine.o .obj/routine.so .shobj/routine.o .shobj/routine.so: routine.cpp routine.h \
 $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/include/common.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i
.obj/sendpt.o .obj/sendpt.so .shobj/sendpt.o .shobj/sendpt.so: sendpt.cpp sendpt.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
