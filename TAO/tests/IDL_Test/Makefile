#------------------------------------------------------------
#	$Id$
#
#	Makefile for IDL_Test examples
#------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS  += -lTAO
LDFLAGS += -L$(ACE_ROOT)/bin

IDL_SRC =	anonymous		\
		array			\
		constants		\
		enum_in_struct		\
		generic_object		\
		included		\
		including		\
		interface		\
		keywords		\
		reopened_modules	\
		sequence		\
		typedef			\
		union			\

IDL_FILES = \
        $(addsuffix S, $(IDL_SRCS)) \
        $(addsuffix C, $(IDL_SRCS))

IDL_TEST_FILES = $(IDL_FILES) main

IDL_TEST_OBJS = $(addsuffix .o, $(IDL_TEST_FILES))
SRC = $(addsuffix .cpp, $(IDL_TEST_FILES))

BIN = main
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
CPPFLAGS += -I$(TAO_ROOT)

.PRECIOUS: $(foreach file, $(IDL_SRC), $(foreach ext, $(IDL_EXT), $(file)$(ext)))

main:	$(addprefix $(VDIR),$(IDL_TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean:	clean
	-$(RM) $(foreach file, $(IDL_SRC), $(foreach ext, $(IDL_EXT), $(file)$(ext)))


