#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

AV_SERVER_OBJS = server.o
AV_AUGSERVER_OBJS = Video_RepositoryC.o augmented_server.o \
		    Property_Exporter.o Machine_Properties.o \
		    Video_Repository.o
AV_VS_OBJS = vs.o
AV_AS_OBJS = as.o

# On non-Windows environment, we should at least define
# the export_include IDL flag.
override TAO_IDLFLAGS += \
	-I$(TAO_ROOT)/orbsvcs/orbsvcs

IDL_FILES = Video_Repository
IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))

FILES = augmented_server server vs as \
	Property_Exporter Machine_Properties

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES)) $(IDL_SRC)


LDLIBS 	+= -lmpeg_server -lmpeg_shared -lTAO_AV -lTAO_CosNaming -lTAO -lrpcsvc

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT) -I$(TAO_ROOT)/orbsvcs/orbsvcs -DTAO_ORBSVCS_HAS_Trader

BIN2 = augmented_server server vs as

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (AV,$(findstring AV,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # AV

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

CPPFLAGS += -DSH_MEM -DNDEBUG
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs

ifdef stat
CPPFLAGS += -DSTAT
endif

server:$(addprefix $(VDIR),$(AV_SERVER_OBJS))
	$(LINK.cc) $(LDFAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

augmented_server:$(addprefix $(VDIR),$(AV_AUGSERVER_OBJS))
	$(LINK.cc) $(LDFAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

vs:$(addprefix $(VDIR),$(AV_VS_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

as:$(addprefix $(VDIR),$(AV_AS_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

.PRECIOUS: $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext))))

realclean: clean
	-$(RM) $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext)))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
