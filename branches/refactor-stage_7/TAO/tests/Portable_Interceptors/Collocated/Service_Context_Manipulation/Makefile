#----------------------------------------------------------------------------
#       GNU Makefile
#
# @file Makefile
#
# $Id$
#
#----------------------------------------------------------------------------
MAKEFILE        = Makefile
BIN_UNCHECKED   = collocated

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif
ifndef CIAO_ROOT
  CIAO_ROOT = $(TAO_ROOT)/CIAO
endif

TAO_IDL = $TAO_ROOT/TAO_IDL/tao_idl
IDL_FILES = test
IDL_SRC = $(foreach ext, C.cpp S.cpp, $(foreach file, $(IDL_FILES), $(file)$(ext)))

FILES = \
  testC \
  testS \
  test_i \
  Collocated_ORBInitializer \
  interceptors \
  Client_Task \
  Server_Task \
  Collocated_Test

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------
LDLIBS  = -lTAO_PortableServer -lTAO_IORInterceptor -lTAO_Valuetype -lTAO_ObjRefTemplate -lTAO -lACE
include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
## We don't need the ACELIB setting from wrapper_macros.GNU
ACELIB =
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(interceptors),1)
BIN    = $(BIN_UNCHECKED)
else
  all: require_warning
endif

OBJS   = $(addsuffix .o, $(notdir $(FILES)))
SRC    = $(addsuffix .cpp, $(FILES))

ifneq ($(BIN),)
all: idl_stubs
testC.cpp testS.cpp test_i.cpp Collocated_ORBInitializer.cpp interceptors.cpp Client_Task.cpp Server_Task.cpp Collocated_Test.cpp: idl_stubs
endif

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU

include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

CPPFLAGS     += -I$(ACE_ROOT) -I$(TAO_ROOT)/tao -I$(TAO_ROOT)
ifeq ($(static_libs),1)
  ifneq ($(LIB),)
    CPPFLAGS += -DTAO_AS_STATIC_LIBS -DTAO_AS_STATIC_LIBS -DACE_AS_STATIC_LIBS
  endif
endif


LDFLAGS      += -L../../../../../ace,../../../../tao,../../../../tao/PortableServer,../../../../tao/Valuetype,../../../../tao/ObjRefTemplate,../../../../tao/IORInterceptor -L../../../../../lib

TAO_IDLFLAGS += -Gv -Ge 1 -Sc -Wb,pre_include=ace/pre.h -Wb,post_include=ace/post.h
#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------
require_warning:
	@echo This project will not be built due to one of the following missing features:
	@echo interceptors

../../../../../lib:
	-@mkdir -p "../../../../../lib"


ADDITIONAL_IDL_TARGETS =
# IDL File Directory: .

ifneq ($(ADDITIONAL_IDL_TARGETS),)
idl_stubs: $(ADDITIONAL_IDL_TARGETS)
endif

.PRECIOUS: $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))

$(BIN): $(addprefix $(VDIR), $(OBJS))
	$(LINK.cc) $(LDFLAGS) $(CC_OUTPUT_FLAG) $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean 
	-$(RM) $(foreach ext, $(IDL_EXT), $(foreach file, $(IDL_FILES), $(file)$(ext)))
ifneq ($(GENERATED_DIRTY),)
	-$(RM) -r $(GENERATED_DIRTY)
endif

