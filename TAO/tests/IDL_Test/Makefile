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

LDLIBS  += -lTAO_PortableServer -lTAO
LDFLAGS += -L$(ACE_ROOT)/bin

IDL_ALWAYS =	anonymous		\
		array			\
		constants		\
		enum_in_struct		\
                full                    \
                fwd                     \
		generic_object		\
                gperf                   \
		included		\
		including		\
		interface		\
		keywords		\
		pragma			\
		sequence		\
		typedef			\
		union

IDL_MAYBE = 	reopened_modules \
		reopen_include1  \
		reopen_include2  \
                params           \
                nested_scope

IDL_FILES = $(IDL_ALWAYS) $(IDL_MAYBE)

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_ALWAYS)) \
        $(addsuffix C.cpp, $(IDL_ALWAYS))

TEST_FILES = main namespace_check

TEST_OBJS = $(addsuffix .o, $(TEST_FILES)) $(IDL_SRC:.cpp=.o)
SRC = $(IDL_SRC) \
	reopened_modulesC.cpp \
	reopened_modulesS.cpp \
	reopen_include1C.cpp \
	reopen_include1S.cpp \
	reopen_include2C.cpp \
	reopen_include2S.cpp \
        paramsC.cpp \
        paramsS.cpp \
        nested_scopeC.cpp \
        nested_scopeS.cpp \
	main.cpp \
        namespace_check.cpp

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

BUILD += 	namespace_check.cpp   \
		reopened_modulesC.cpp \
		reopen_include1C.cpp  \
		reopen_include2C.cpp  \
                paramsC.cpp           \
                nested_scopeC.cpp

BIN = main

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

CPPFLAGS += -I$(TAO_ROOT)

.PRECIOUS: $(foreach file, $(IDL_SRC), $(foreach ext, $(IDL_EXT), $(file)$(ext)))

namespace_check.cpp: reopened_modulesC.h reopen_include1C.h reopen_include2C.h \
                     paramsC.h nested_scopeC.h

reopened_modulesC.h: reopened_modules.idl $(TAO_IDL)
	$(TAO_IDL) -w $(TAO_IDLFLAGS) reopened_modules.idl

reopen_include1C.h: reopen_include1.idl $(TAO_IDL)
	$(TAO_IDL) -w $(TAO_IDLFLAGS) reopen_include1.idl

reopen_include2C.h: reopen_include2.idl $(TAO_IDL)
	$(TAO_IDL) -w $(TAO_IDLFLAGS) reopen_include2.idl

paramsC.h: params.idl $(TAO_IDL)
	$(TAO_IDL) -w $(TAO_IDLFLAGS) params.idl

nested_scopeC.h: nested_scope.idl $(TAO_IDL)
	$(TAO_IDL) -w $(TAO_IDLFLAGS) nested_scope.idl

main:	$(addprefix $(VDIR),$(TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean:	clean
	-$(RM) $(foreach file, $(IDL_FILES), $(foreach ext, $(IDL_EXT), $(file)$(ext)))


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
