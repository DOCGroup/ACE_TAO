# AIX 4.2.0
# *not* using Orbix
# The AIX fileset bos.rte.bind_cmds must be at version 4.2.0.2 in order to
# build libACEshr.a, and at 4.2.0.4 for dl*() routines to work properly. Best
# apply the update available at:
# ftp://service.boulder.ibm.com/aix/fixes/v4/os/bos.rte.bind_cmds.4.2.0.4.bff

CC	    = xlC
CXX	    = xlC_r
DLD	    = makeC++SharedLib_r
LD          = $(CXX)
CPPFLAGS    += -qxcall -qtempinc -qcheck=nobounds:div:null  -M -qlanglvl=ansi -qextchk -qflag=w:w -qinfo -qnoro
SHLIBA      = $(SHLIB:lib%.so=lib%shr.a)
ifdef SHLIB
ACELIB      = -lACEshr
endif
LLIBS	    = -lC_r -lC -lpthreads -lbsd -ltli_r -ldl -lc_r -lm -lc $(ACELIB)
LIBS	    += $(filter-out $(SHLIBA:lib%.a=-l%), $(LLIBS))
ARFLAGS	    = ruv
AR	    = ar
LDFLAGS	    += -bI:/lib/pse.exp 
# Default OCCFLAGS builds common code for all RS/6000 architectures but
# this can be set to optimize for your particular computer. The commented
# out example optimizes for RS/6000 43P
#OCCFLAGS    += -qarch=ppc -qtune=604
OCCFLAGS    += -qarch=com 
RANLIB	    = ranlib
SOFLAGS	    = -p 0
STATLIB     = $(VLIB)
TEMPINCDIR  = tempinc
SOBUILD = 
