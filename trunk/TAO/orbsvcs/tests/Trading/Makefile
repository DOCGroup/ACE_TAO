#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

MAKEFILE = Makefile

BIN2 = export_test \
       import_test \
       colocated_test

LIBNAME  = libTTest
LIB2 = $(LIBNAME).a
SHLIB2 = $(LIBNAME).$(SOEXT)

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

IDL_FILES = TTestC \
            TTestS \

FILES   = $(IDL_FILES) \
          TT_Info \
          Service_Type_Exporter \
          Offer_Exporter \
          Offer_Importer \
          Simple_Dynamic

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT)

LSRC    = $(addsuffix .cpp,$(FILES))
PSRC    = $(addsuffix .cpp,$(BIN))
ACE_SHLIBS = -lTAO_CosTrading -lTAO_CosNaming -lTAO -lACE
LDLIBS  = -lTAO_CosTrading -lTAO_CosNaming -lTAO

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (Trader,$(findstring Trader,$(TAO_ORBSVCS)))
  ifeq ($(minimum_corba),0)
    BIN = $(BIN2)
    LIB = $(LIB2)
    SHLIB = $(SHLIB2)
  endif # minimum_corba
endif # Trader

TAO_IDLFLAGS += -Ge 1
#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)

  # Since older g++ cannot handle nested classes properly, this test
  # will not build on g++ prior to 2.8.0.
  all clean realclean:
	@echo Trading tests is not built with older versions of g++

else # ! ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

.PRECIOUS: TTestS.cpp TTestS.h TTestC.cpp TTestC.h

ifneq ($(BIN),)
  ifeq ($(SHLIB), $(findstring $(SHLIB), $(BUILD)))
    $(BIN): $(SHLIB)
  endif
  ifeq ($(LIB), $(findstring $(LIB), $(BUILD)))
    $(BIN): $(LIB)
  endif
endif # BIN

realclean: clean
	-$(RM) $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES)) TTestS_T.*

endif # ! ACE_HAS_GNUG_PRE_2_8

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
