#
# $Id$
#

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

PLATFORM_WXWINDOWS_CPPFLAGS = `wx-config --cxxflags`
PLATFORM_WXWINDOWS_LDFLAGS  = `wx-config --ldflags`
PLATFORM_WXWINDOWS_LIBS     = `wx-config --libs`

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs
LDFLAGS  += -L$(TAO_ROOT)/orbsvcs
LDLIBS	  = -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO

BIN_UNCHECKED = wxNamingViewer

SRC = wxNamingViewer.cpp \
      wxAddNameServerDlg.cpp \
      wxBindDialog.cpp\
      wxBindNewContext.cpp \
      wxNamingObject.cpp \
      wxNamingTree.cpp \
      wxNamingViewerFrame.cpp \
      wxSelectNSDialog.cpp \
      wxViewIORDialog.cpp

OBJS=$(SRC:.cpp=.o)

TAO_IDLFLAGS += -Ge 1


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Naming,$(findstring Naming,$(TAO_ORBSVCS)))
  ifeq ($(wxWindows),1)
    BIN = $(BIN_UNCHECKED)
    CPPFLAGS += $(PLATFORM_WXWINDOWS_CPPFLAGS)
    LDFLAGS  += $(PLATFORM_WXWINDOWS_LDFLAGS)
    LDLIBS   += $(PLATFORM_WXWINDOWS_LIBS)
  endif # wxWindows
endif # Naming

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

