#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the tao_ifr back end library
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO_IDL_FE_DLL
ACE_SHLIBS = -lTAO_IDL_FE_DLL -lACE

VPATH = .

MAKEFILE=Makefile.BE

LIBNAME = libTAO_IFR_BE_DLL
LIB = $(LIBNAME).a
SHLIB = $(LIBNAME).$(SOEXT)

FILES = be_global \
        be_produce \
        ifr_adding_visitor \
        ifr_adding_visitor_exception \
        ifr_adding_visitor_operation \
        ifr_adding_visitor_structure \
        ifr_adding_visitor_union \
        ifr_removing_visitor \
        ifr_visitor

LSRC = $(addsuffix .cpp,$(FILES))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

CPPFLAGS += -I../TAO_IDL/include -I../TAO_IDL/fe

realclean: clean

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

