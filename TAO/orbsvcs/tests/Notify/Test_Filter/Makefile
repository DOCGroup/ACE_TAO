#$Id$
#----------------------------------------------------------------------------
#       GNU Makefile
#----------------------------------------------------------------------------
MAKEFILE        = Makefile
DEPENDENCY_FILE = .depend.Makefile
LIB_UNCHECKED   = libTAO_RT_NotifyTests_Filter.a
SHLIB_UNCHECKED = libTAO_RT_NotifyTests_Filter.$(SOEXT)

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

IDL_FILES =
IDL_SRC = $(foreach ext, C.cpp S.cpp, $(foreach file, $(IDL_FILES), $(file)$(ext)))


FILES = \
  RT_Test_Filter \
  RT_Test_FilterFactory

CURRENT_COMPONENTS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(rt_corba),1)
ifeq (Notify, $(findstring Notify, $(CURRENT_COMPONENTS)))
ifeq (Naming, $(findstring Naming, $(CURRENT_COMPONENTS)))
LIB   = $(LIB_UNCHECKED)
SHLIB = $(SHLIB_UNCHECKED)
endif
endif
endif # ami
LSRC   = $(addsuffix .cpp, $(FILES))
ACELIB = -lACE

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk
include $(TAO_ROOT)/orbsvcs/tests/Notify/notify_tests.GNU

CPPFLAGS     += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao -I$(ACE_ROOT)
ifeq ($(shared_libs),1)
ifneq ($(SHLIB),)
CPPFLAGS     += -DTAO_NOTIFY_TEST_BUILD_DLL
endif
endif
ifeq ($(static_libs),1)
ifneq ($(LIB),)
CPPFLAGS     += -DTAO_AS_STATIC_LIBS -DACE_AS_STATIC_LIBS
endif
endif


LDFLAGS      += -L$(TAO_ROOT)/tao/PortableServer -L$(TAO_ROOT)/tao/DynamicAny -L$(TAO_ROOT)/tao/IORTable -L$(TAO_ROOT)/orbsvcs/orbsvcs/ETCL -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
TAO_IDLFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -Ge 1 -Sc

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

ADDITIONAL_IDL_TARGETS =

# IDL File Directory: .

idl_stubs: $(ADDITIONAL_IDL_TARGETS)

all: idl_stubs

.PRECIOUS: $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))
