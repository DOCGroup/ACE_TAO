# $Id$

# AIX 4.2.0
# *not* using Orbix
# The AIX fileset bos.rte.bind_cmds must be at version 4.2.0.2 in order to
# build libACEshr.a, and at 4.2.0.4 for dl*() routines to work properly. Best
# apply the update available at:
# ftp://service.boulder.ibm.com/aix/fixes/v4/os/bos.rte.bind_cmds.4.2.0.4.bff

debug = 1

CC	    = xlC
CXX	    = xlC_r
CCFLAGS     += $(CFLAGS)
DCFLAGS     += -g
DLD	    = makeC++SharedLib_r
LD          = $(CXX)
CPPFLAGS    += -qxcall -qtempinc -qcheck=nobounds:div:null  -M -qlanglvl=ansi -qextchk -qflag=w:w -qinfo 
SHLIBA      = $(SHLIB:lib%.so=lib%shr.a)
ifdef SHLIB
ACELIB      = -lACEshr
endif
LLIBS	    = -ltli_r -ldl  $(ACELIB)
LIBS	    += $(filter-out $(SHLIBA:lib%.a=-l%), $(LLIBS))
ARFLAGS	    = ruv
AR	    = ar
LDFLAGS	    += -bI:/lib/pse.exp 
# Default OCCFLAGS builds common code for all RS/6000 architectures but
# this can be set to optimize for your particular computer. The commented
# out example optimizes for RS/6000 43P.
# OCCFLAGS is not used by default.  To used it, set optimize=1
# either in this file or on the command line.
#OCCFLAGS    += -qarch=ppc -qtune=604
OCCFLAGS    += -qarch=com 
RANLIB	    = ranlib
SOFLAGS	    = -p 0
STATLIB     = $(VLIB)
TEMPINCDIR  = tempinc
SOBUILD = 
