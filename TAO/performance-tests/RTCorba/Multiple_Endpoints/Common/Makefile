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
TAO_IDLFLAGS += -Ge 1\
 -Wb,export_macro=RTCORBA_COMMON_Export \
 -Wb,export_include=rtcorba_common_export.h \


LIB = libRTCORBA_Common.a
SHLIB = libRTCORBA_Common.$(SOEXT)

IDL_FILES = test
IDL_SRC = testC testS

ACE_SHLIBS = -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO -lACE
LDLIBS = -lTAO_PortableServer -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO_IORInterceptor -lTAO
SRCS = $(IDL_SRC) test_i
LSRC = $(addsuffix .cpp,$(IDL_SRC) test_i)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS  += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs

ifeq ($(shared_libs),1)
ifneq ($(SHLIB),)
CPPFLAGS     += -DRTCORBA_COMMON_BUILD_DLL
endif
endif
ifeq ($(static_libs),1)
ifneq ($(LIB),)
CPPFLAGS     += -DTAO_AS_STATIC_LIBS -DACE_AS_STATIC_LIBS
endif
endif

.PRECIOUS: $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext))))

realclean: clean
	-$(RM) $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext)))

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
