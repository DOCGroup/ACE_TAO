#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDLIBS = -lTAO -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor
IDL_FILES = CCS
IDL_SRC = CCSC.cpp CCSS.cpp
IDL_HDR = CCSC.h CCSS.h

PROG_SRCS = \
	client.cpp \
	server.cpp \
	icp.cpp

SRC = $(IDL_SRC) $(PROG_SRCS)

SIMPLE_CLT_OBJS = \
		CCSC.o \
		CCSS.o \
		client.o 

SIMPLE_SVR_OBJS = \
		CCSC.o \
		CCSS.o \
		server.o \
		icp.o


BIN = server \
      client
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: CCSC.cpp CCSC.i CCSC.h
.PRECIOUS: CCSS.cpp CCSS.i CCSS.h
.PRECIOUS: CCSS_T.cpp CCSS_T.i CCSS_T.h

override TAO_IDLFLAGS += Ge 0
CCFLAGS += -fimplicit-templates -frtti -O0

$(IDL_SRC): CCS.idl
server:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/$(RM) -rf CCSC.* CCSS.* CCSS_T.* CCS.h

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.















