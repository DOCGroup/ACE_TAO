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

LDLIBS = -lTAO

IDLFILES = testC testS
BIN = driver
LIBNAME=libTest_DLL
LIB = $(LIBNAME).a
SHLIB = $(LIBNAME).$(SOEXT)

LSRC= \
	TestC.cpp \
	TestS.cpp \
	Test_i.cpp \
	Loader.cpp
ACE_SHLIBS = -lTAO -lACE
LDLIBS  = -lTAO

DRIVER_OBJS = driver.o TestC.o
PSRC=driver.cpp
BIN=driver

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(TAO_ROOT)/taoconfig.mk
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

driver: $(addprefix $(VDIR),$(DRIVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

.PRECIOUS: $(foreach ext, $(IDL_EXT), test$(ext))

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Test$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
