#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
LIBNAME  = libTAO_TS
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

VPATH=.

LDLIBS += -lTAO_AV -lTAO_CosProperty -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO -lACE
ACE_SHLIBS += -lTAO_AV -lTAO_CosProperty -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

CPP_SRCS = TimeStamp

FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

BIN = sender receiver

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
## Do not install a symbolic link for libTAO_TS in $ACE_ROOT/ace
INSTALL=
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

ifeq ($(rapi),1)
  LDLIBS += -lACE_QoS
  ACE_SHLIBS += -lACE_QoS
endif # rapi

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/orbsvcs
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs 

sender: $(addprefix $(VDIR),sender.o) $(SHLIB)
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

receiver: $(addprefix $(VDIR),receiver.o) $(SHLIB)
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


realclean: clean

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
