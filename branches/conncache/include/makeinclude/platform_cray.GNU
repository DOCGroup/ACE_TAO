# $Id$

# Initial platform macros for SGI/Cray UNICOS (vector) & UNICOS/mk (mpp) - 
#    UNICOS 10.0.0.2
#    UNICOS/mk 2.0.3.10
#    Cray C++ Version 3.1.0.0
#    Cray Standard C Version 6.1.0.0
#
# Contributed by Doug Anderson <doug@clark.net>

debug = 1
exceptions = 1
static_libs_only = 1
quantify =
purify =

# Cray's autoinstantiation seems to be broken, resulting in
# C++ prelinker: error: instantiation loop
# So, we're using ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
CC              = cc
LDLIB		+= -L$(ACE_ROOT)/ace
CXX		= CC
#CFLAGS		+= 
CCFLAGS		+= -h noautoinstantiate
DCFLAGS		+= -g
DCCFLAGS	+= $(DCFLAGS)
OCFLAGS		+= -O2
OCCFLAGS        += $(OCFLAGS)

ifndef exceptions
  CCFLAGS += -h noexceptions
else
  CCFLAGS += -h exceptions
endif # ! exceptions

DLD             = $(CXX)
LD              = $(CXX)
LIBS            = -lpthread
PIC             =
AR              = ar
ARFLAGS         = ruv
RANLIB          = /bin/true

# This is necessary to get libraries to contain template info, but only
# necessary when using autoinstantiation
# PRELIB     = @echo ""; \
#  echo "int main() { return 0; }" >dummy.cpp; \
#  echo "$(CXX) $(CCFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS) -o dummy dummy.cpp $?";\
#  $(CXX) $(CCFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LIBS) -o dummy dummy.cpp $?;
