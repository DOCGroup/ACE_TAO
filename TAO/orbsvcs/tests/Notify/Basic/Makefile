#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

BIN2 =  ConnectDisconnect \
	LifeCycleTest \
	IdAssignment

#### If the orbsvcs library wasn't built with all components, don't
#### try to build certain tests.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Notify,$(findstring Notify,$(TAO_ORBSVCS)))
  ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Naming
endif # Notify

PSRC=$(addsuffix .cpp,$(BIN)) Updates.cpp
LDLIBS = -lNotifyTests -lTAO_CosNotification -lTAO_Svc_Utils -lTAO_CosNaming -lTAO
CPPFLAGS += -I../lib -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs \
        $(foreach svc, $(TAO_ORBSVCS), -DTAO_ORBSVCS_HAS_$(svc))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

ifeq (Notify,$(findstring Notify,$(TAO_ORBSVCS)))

all: Updates

Updates: $(addprefix $(VDIR),Updates.o)
	$(LINK.cc) $(LDFLAGS) -o $@ $^ -lTAO_CosNotification -lTAO_CosNaming $(VLDLIBS) $(POSTLINK)
endif # Notify

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/LifeCycleTest.o .obj/LifeCycleTest.so .shobj/LifeCycleTest.o .shobj/LifeCycleTest.so: LifeCycleTest.cpp \
 $(ACE_ROOT)/ace/Arg_Shifter.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Get_Opt.i \
 LifeCycleTest.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
