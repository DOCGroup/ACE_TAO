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

# On non-Windows environment, we should at least define
# the export_include IDL flag.
TAO_IDLFLAGS +=\
 -Wb,export_macro=testSched_Export \
 -Wb,export_include=testSched_export.h

LIB = libtestSched_Common.a
SHLIB = libtestSched_Common.$(SOEXT)

IDL_FILES = testSched
IDL_SRC = testSchedC testSchedS

ACE_SHLIBS = -lTAO_PortableServer -lTAO -lACE
LDLIBS = -lTAO_PortableServer -lTAO
SRCS = $(IDL_SRC)
LSRC = $(addsuffix .cpp,$(IDL_SRC) )
CCFLAGS := $(strip $(subst -fno-implicit-templates,,$(CCFLAGS))) -g
CPPFLAGS += -DACE_HAS_GNU_REPO
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)/tao
CPPFLAGS += -D_DEBUG
BIN = client server


CLIENT_OBJS = $(addsuffix .o, client )
SERVER_OBJS = $(addsuffix .o, server Object1_i )

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS  += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext))))

realclean: clean
	-$(RM) $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext)))

LDFLAGS = -L$(ACE_ROOT)/ace -L. -g
LDLIBS = -lTAO_RTCosScheduling -ltestSched_Common -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_RTPortableServer -lTAO_PortableServer -lTAO_RTCORBA -lTAO

include $(TAO_ROOT)/taoconfig.mk

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
