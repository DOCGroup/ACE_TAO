#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile
LIBNAME  = libIBM1047_ISO8859
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

ACE_SHLIBS = -lTAO -lACE

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(TAO_ROOT)/taoconfig.mk

CPP_SRCS += \
  Char_IBM1047_ISO8859_Translator \
  Char_IBM1047_ISO8859_Factory \
  Wchar_IBM1047_ISO8859_Translator \
  Wchar_IBM1047_ISO8859_Factory

IDL_SRC = \
        $(addsuffix S.cpp, $(IDL_FILES)) \
        $(addsuffix C.cpp, $(IDL_FILES))
FILES = $(CPP_SRCS)
DEFS  = $(addsuffix .h,$(FILES))
LSRC  = $(addsuffix .cpp,$(FILES))

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Char_IBM1047_ISO8859_Translator.o .obj/Char_IBM1047_ISO8859_Translator.so .shobj/Char_IBM1047_ISO8859_Translator.o .shobj/Char_IBM1047_ISO8859_Translator.so: Char_IBM1047_ISO8859_Translator.cpp Char_IBM1047_ISO8859_Translator.h
