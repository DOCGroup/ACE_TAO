#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the entire ACE release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libCLASSIX.a

FILES    = \
        CLASSIX_OS            	\
        CLASSIX_Select_Reactor	\
        CLASSIX_Addr          	\
        CLASSIX_Port_Core     	\
        CLASSIX_Port          	\
        CLASSIX_Port_Default  	\
        CLASSIX_Group         	\
        CLASSIX_Group_Stamp   	\
        CLASSIX_Group_Dynamic 	\
        CLASSIX_SAP           	\
        CLASSIX_Peer_SAP      	\
        CLASSIX_Stream        	\
        CLASSIX_Dgram_Mcast   	\
        CLASSIX_CO_Acceptor   	\
        CLASSIX_CO_Connector  	\
        CLASSIX_CLD_Connector


TEMPLATE_FILES =

DEFS    = $(addsuffix .h,$(FILES)) Svc_Conf_Tokens.h
DEFS    += $(addsuffix .i,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))
LSRC2   = $(addsuffix .cpp,$(TEMPLATE_FILES))

BUILD   = $(VLIB)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifneq ($(GHS),)
  ifeq ($(CPU),86)
    FILES += gethrtime
    LSRC += gethrtime.cpp
  endif # 86
endif # GHS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#### Disable installs in this ($(ACE_ROOT)/ace) directory, because this
#### is the (default) destination of installs anyways.  And this will prevent
#### creation of a link from the ACE library to itself when the build of
#### the ACE library fails.
INSTALL =
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#### Setting ACELIB to null allows it to be used for building shared libraries,
#### including libACE.$(SOEXT,) on certain platforms.
ACELIB =


ifeq ($(SUPPRESS_DASH_G),1)
#### Build this target without -g on some platforms.
  $(COMPILE-NO_DASH_G.cc) -o $@ $<
endif # SUPPRESS_DASH_G

ifeq ($(CHORUS),1)
  ifeq ($(CC),$(GHS_DIR)/build -driver ch68)
    #### Build this target without -g, because it causes ghs 1.8.8 to core
    #### dump.
    $(VDIR)Select_Reactor.o .obj/Select_Reactor.so .shobj/Select_Reactor.o .shobj/Select_Reactor.so:
    $(COMPILE-NO_DASH_G.cc) -o $@ $<
  endif # CC
endif # CHORUS

ifneq ($(GHS),)
  ifeq ($(CPU),86)
    $(VDIR)gethrtime.$(OBJEXT):
	make $@ ghs=0
  endif # 86
endif # GHS

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
