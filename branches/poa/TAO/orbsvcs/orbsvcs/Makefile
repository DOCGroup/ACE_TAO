#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = liborbsvcs
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).$(SOEXT)

IDL_FILES = \
	CosNamingC \
	CosNamingS \
	RtecEventCommC \
	RtecEventCommS \
	RtecSchedulerC \
	RtecSchedulerS \
	RtecEventChannelAdminC \
	RtecEventChannelAdminS \

FILES= $(IDL_FILES) \
	Event_Utilities \
	Scheduler_Factory \
	Runtime_Scheduler \
	Scheduler_Utilities \

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

LIBS 	+= -lTAO -lACE

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)#-H

# @@ Commented out until no more hand-crafting is needed, right now the
# main problem is the ACE_Export macros for NT.

CosNamingC.cpp CosNamingS.cpp CosNamingC.h CosNamingS.h: CosNaming.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl $^

RtecSchedulerC.cpp RtecSchedulerS.cpp RtecSchedulerC.h RtecSchedulerS.h: RtecScheduler.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl $^

RtecEventCommC.cpp RtecEventCommS.cpp RtecEventCommC.h RtecEventCommS.h: RtecEventComm.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl $^

RtecEventChannelAdminC.cpp RtecEventChannelAdminS.cpp RtecEventChannelAdminC.h RtecEventChannelAdminS.h: RtecEventChannelAdmin.idl
	$(TAO_ROOT)/TAO_IDL/tao_idl $^

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state
 
realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
