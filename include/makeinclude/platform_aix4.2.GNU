# $Id$

# AIX 4.2.0
# *not* using Orbix
# The AIX fileset bos.rte.bind_cmds must be at version 4.2.0.2 in order to
# build libACEshr.a, and at 4.2.0.4 for dl*() routines to work properly. Best
# apply the update available at:
# ftp://service.boulder.ibm.com/aix/fixes/v4/os/bos.rte.bind_cmds.4.2.0.4.bff

debug = 1
# xlC builds the shared lib from the static lib, so need to build both.
shared_libs_only = 0
static_libs = 1

# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)

CC	    = xlC
CXX	    = xlC_r

# -qextchk is handy, but produces false type mismatches when linking
# netsvcs with 3.1.4, so it's disabled.  IBM reports this fixed in 3.6.4.
CCFLAGS     += $(CFLAGS) -qtempinc -qflag=w:w -qinfo
CPPFLAGS    += -qlanglvl=ansi -DACE_AIX_MAJOR_VERS=$(AIX_MAJOR_VERS) -DACE_AIX_MINOR_VERS=$(AIX_MINOR_VERS)

# -qinitauto seems useful, but when built with -qinitauto=5F, the
# Process_Strategy_Test went CPU bound in ostream, so I removed it (S. Huston)
DCFLAGS     += -g -qcheck=nobounds:div:null

DLD	    = makeC++SharedLib_r
LD          = $(CXX)
SHLIBA      = $(SHLIB:lib%.so=lib%shr.a)
ifdef SHLIB
ACELIB      = -lACEshr
endif
LIBS	    += -ltli_r -ldl
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
