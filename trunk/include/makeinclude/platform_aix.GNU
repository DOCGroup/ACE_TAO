# AIX 4.1.3
# *not* using Orbix
# This file assumes that the user has installed the AIX patch
# containing the dl*() APIs.  To use these APIs, IBM has created a
# separate product (free to AIX licensees) called shared library
# hookable symbols (or slhs/6000).  If they don't have this patch, the
# sv* commands for compiling and linking will not be present on the
# system. 

CC	    = xlC
CXX	    = xlC_r
CFLAGS      += $(DCFLAGS)
CCFLAGS     += $(CFLAGS)
DCFLAGS     += -g
DLD	    = makeC++SharedLib_r
LD	    = $(CXX)
CPPFLAGS    += -qxcall -qtempinc
SHLIBA      = $(SHLIB:lib%.so=lib%shr.a)
ifdef SHLIB
ACELIB      = -lACEshr
endif
LLIBS	    = -lsvld -ltli_r $(ACELIB)
LIBS	    += $(filter-out $(SHLIBA:lib%.a=-l%), $(LLIBS))
ARFLAGS	    = ruv
AR	    = ar
LDFLAGS	    += -bI:/lib/pse.exp
# Should be set to optimize for your particular computer.  This
# is set to be optimized for RS/6000 43P
OCCFLAGS    += -qarch=ppc -qtune=604
RANLIB	    = ranlib
SOFLAGS	    = -p 0
STATLIB     = $(VLIB)
TEMPINCDIR  = tempinc
SOBUILD	    =
