#----------------------------------------------------------------------------
#
#       $Id$
#
#       Makefile for the TAO examples
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

DIRS  = Callback_Quoter \
        POA \
        Logging \
        Simple \
        Event_Comm \
        OBV \
        Load_Balancing \
	Persistent_Grid \
	AMI \
	AMH \
	Quoter \
	Load_Balancing_persistent \
	Simulator \
	Content_Server \
        PluggableUDP \
	TypeCode_Creation \
	ior_corbaloc \
	RTCORBA

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

ifeq ($(corba_messaging),1)

DIRS += \
	Buffered_AMI \
	Buffered_Oneways

endif # corba_messaging

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
