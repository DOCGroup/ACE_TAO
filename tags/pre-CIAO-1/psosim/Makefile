#----------------------------------------------------------------------------
#
# $Id$
#
#	Makefile for the pSOSim system object files
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

CPU_FAMILY	= psosim
CPU		= psosim
CPU_TOOLS	= psosim
BOARD		= psosim

PSS_CFG_DIR     = $(ACE_ROOT)/psosim

PSS_CFG_OBJS	= $(PSS_CFG_DIR)/drv_conf.o $(PSS_CFG_DIR)/prepccfg.o $(PSS_CFG_DIR)/psoscfg.o $(PSS_CFG_DIR)/sysinit.o
                  
PSS_CONFIG      = $(PSS_ROOT)/configs/std
PSS_SYS_DIR	= $(PSS_ROOT)/sys/os
PSS_SYS_LIBS	= $(PSS_SYS_DIR)/prepc.a $(PSS_SYS_DIR)/psos.a $(PSS_SYS_DIR)/pna.a
PSS_BSP		= $(PSS_ROOT)/bsps/psosim
PSS_BSP_LIB	= $(PSS_BSP)/bsp.a

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

# To build multiple executables in the same directory on AIX, it works
# best to wipe out any previously-created tempinc directory.
# The compiler/linker isn't too smart about instantiating templates...
ifdef TEMPINCDIR
COMPILE.cc := $(RM) -rf tempinc; $(COMPILE.cc)
endif

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

all : $(PSS_CFG_DIR)/acepsos.a

$(PSS_CFG_DIR)/acepsos.a : $(PSS_CFG_OBJS)
	$(AR) $(ARFLAGS) $@ $?

$(PSS_CFG_DIR)/sysinit.o : $(PSS_CONFIG)/sysinit.c $(ACE_ROOT)/ace/sys_conf.h $(PSS_BSP)/bsp.h \
	   $(PSS_ROOT)/include/bspfuncs.h $(PSS_ROOT)/include/configs.h
	$(COMPILE.c) -o $@ $(PSS_CONFIG)/sysinit.c

$(PSS_CFG_DIR)/prepccfg.o : $(PSS_CONFIG)/prepccfg.c $(ACE_ROOT)/ace/sys_conf.h $(PSS_BSP)/bsp.h \
	   $(PSS_ROOT)/include/bspfuncs.h $(PSS_ROOT)/include/configs.h
	$(COMPILE.c) -o $@ $(PSS_CONFIG)/prepccfg.c

$(PSS_CFG_DIR)/psoscfg.o : $(PSS_CONFIG)/psoscfg.c $(ACE_ROOT)/ace/sys_conf.h $(PSS_BSP)/bsp.h \
	   $(PSS_ROOT)/include/bspfuncs.h $(PSS_ROOT)/include/configs.h
	$(COMPILE.c) -o $@ $(PSS_CONFIG)/psoscfg.c

$(PSS_CFG_DIR)/drv_conf.o : $(PSS_CFG_DIR)/drv_conf.c $(ACE_ROOT)/ace/sys_conf.h $(PSS_BSP)/bsp.h \
	   $(PSS_ROOT)/include/bspfuncs.h $(PSS_ROOT)/include/configs.h
	$(COMPILE.c) -o $@ $(PSS_CFG_DIR)/drv_conf.c

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
