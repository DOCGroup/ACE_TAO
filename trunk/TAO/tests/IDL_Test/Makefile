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

IDL_ALWAYS =	anonymous		\
		array			\
		constants		\
		enum_in_struct		\
		generic_object		\
		included		\
		including		\
		interface		\
		keywords		\
		nested_scope		\
		params			\
		pragma			\
		sequence		\
		typedef			\
		union			\

IDL_MAYBE = 	reopened_modules

IDL_SRC = $(IDL_ALWAYS) $(IDL_MAYBE)

IDL_FILES = \
        $(addsuffix S, $(IDL_ALWAYS)) \
        $(addsuffix C, $(IDL_ALWAYS))

IDL_TEST_FILES = $(IDL_FILES) main namespace_check

IDL_TEST_OBJS = $(addsuffix .o, $(IDL_TEST_FILES))
SRC = $(addsuffix .cpp, $(IDL_TEST_FILES))

BIN = main

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

CPPFLAGS += -I$(TAO_ROOT)

.PRECIOUS: $(foreach file, $(IDL_SRC), $(foreach ext, $(IDL_EXT), $(file)$(ext))) .reopened_modules_stamp

.reopened_modules_stamp $(foreach ext, $(IDL_EXT), reopened_modules$(ext)): $(TAO_ROOT)/TAO_IDL/tao_idl
	$(TAO_ROOT)/TAO_IDL/tao_idl -w -Ge 1 reopened_modules.idl
	@touch $@

main:	$(addprefix $(VDIR),$(IDL_TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

$(addprefix $(VDIR),$(IDL_TEST_OBJS)): .reopened_modules_stamp

realclean:	clean
	-$(RM) $(foreach file, $(IDL_SRC), $(foreach ext, $(IDL_EXT), $(file)$(ext))) .reopened_modules_stamp


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
