#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#---------------
#   STL (start)
#---------------

include $(STL_ROOT)/config/local.cfg

CC_INCLUDES += -I$(STD)
CC_INCLUDES += -pta

CPPFLAGS += \
        -I$(TOOLKIT) \
        $(CC_INCLUDES) \
        $(CC_FLAGS) \
        $(CC_DEFINES) \
        $(CC_EH) \
        $(CC_MT)

ifndef exceptions
CPPFLAGS += -DOS_NO_EXCEPTION_SPECIFIER
else
CPPFLAGS += -DOS_STL_ASSERT
endif

LDFLAGS += -L$(STL_ROOT)/lib

#---------------
#   STL (end)
#---------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------
static_libs_only=1
BIN     = export_test import_test

MAKEFILE = Makefile
LIBNAME  = TTest
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

IDL_FILES = TTestC \
	    TTestS \

FILES   = $(IDL_FILES) \
	  TT_Info \
	  Service_Type_Exporter \
	  Offer_Exporter \
	  Offer_Importer 

DCFLAGS = -g
LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(STL_ROOT)/lib
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

LSRC    = $(addsuffix .cpp,$(FILES))
LDLIBS  = -lTTest -lorbsvcs -lospace -lTAO
VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU

INSTALL=

include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

.PRECIOUS: TTestS.cpp TTestS.h TTestC.cpp TTestC.h

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
