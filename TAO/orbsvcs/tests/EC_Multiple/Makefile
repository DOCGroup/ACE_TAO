# $Id$

SRC = $(BIN:%=%$(VAR).cpp)
LDLIBS= -lorbsvcs -lTAO

ifndef TAO_ROOT
 TAO_ROOT = $(ACE_ROOT)/TAO
endif

CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

BIN2 = EC_Multiple

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Event,$(findstring Event,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # Event

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)
  # This test causes g++ 2.7.2.3 (the compiler) to core dump.
  all clean realclean:
	@echo orbsvcs/tests/EC_Multiple is not built with older versions of g++
else # ! ACE_HAS_GNUG_PRE_2_8
  include $(ACE_ROOT)/include/makeinclude/macros.GNU
  include $(TAO_ROOT)/rules.tao.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
  include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
endif # ! ACE_HAS_GNUG_PRE_2_8

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

ifeq ($(LYNXTARGET),ppc)
#### EC_Multiple.cpp raises internal compiler error with LynxOS 3.0.0 g++,
#### but only when -O2 optimization is enabled.  Disable it . . .
.obj/EC_Multiple.o .obj/EC_Multiple.so .shobj/EC_Multiple.o .shobj/EC_Multiple.so: EC_Multiple.cpp
	@echo NOTE: compiling EC_Multiple.cpp with -O instead of -O2, see Makefile.
	$(subst $(OCFLAGS) ,-O ,$(COMPILE.cc)) -o $@ $<
endif # LYNXTARGET == ppc

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
