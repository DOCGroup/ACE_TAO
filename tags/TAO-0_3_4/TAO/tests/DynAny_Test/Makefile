#------------------------------------------------------------
#	$Id$
#
#	Makefile for DynAny_Test examples
#------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS  += -lTAO
LDFLAGS += -L$(ACE_ROOT)/bin

IDL_SRC = da_testsC.cpp da_testsS.cpp

BASIC_TEST_FILES = 	da_testsC		\
			da_testsS		\
			data			\
			driver			\
			test_dynany		\
			test_dynarray		\
			test_dynenum		\
			test_dynsequence	\
			test_dynstruct		\
			test_dynunion		\
			test_wrapper	

BASIC_TEST_OBJS = $(addsuffix .o, $(BASIC_TEST_FILES))
SRC = $(addsuffix .cpp, $(BASIC_TEST_FILES))	

BIN = basic_test
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
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

TSS_ORB_FLAG = #-DTAO_HAS_TSS_ORBCORE
CPPFLAGS += -I$(TAO_ROOT)

.PRECIOUS: da_testsC.h da_testsC.cpp da_testsS.h da_testsS.cpp \
	da_testsC.i da_testsS.i

basic_test:	$(addprefix $(VDIR),$(BASIC_TEST_OBJS))
		$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

clean:		
                -/bin/rm -rf $(VDIR)/*.o $(BIN) obj.* core Templates.DB .make.state

realclean:	clean
		-/bin/rm -rf da_testsC.* da_testsS.* da_testsS_T.*


