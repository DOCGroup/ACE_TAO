# SunOS 5.x (Solaris 2.x) with SunC++ 4.2 or earlier
# *not* using Orbix
#
# Note:	 some ACE files might generate these kinds of warnings:
# 1) "template manager : Warning: No valid template database available.
#     Creating default repository "Templates.DB"":
#    self explanatory, and apparently harmless.
# 2) "Warning: Could not find source for " one of the following:
#    default constructor, copy constructor, destructor, or assignment
#    operator.	It appears that with +w, Sun C++ 4.x issues this warning
#    whenever it has to generate one of these functions (because it wasn't
#    defined by the user).  This isn't really a problem.
# 3) "Warning: <foo> hides the function " or "hides the same name in an
#    outer scope".  A useful warning, perhaps, but not really a problem.
# There should be no other warnings from Sun C++ when building ACE.
#
CC		= cc
CXX		= CC
CFLAGS		+= $(DCFLAGS) -mt
#### Exception handling breaks on MP machines prior to Sun C++ 4.2
#### with jumbo patch 104631-02, see ace/config-sunos5.5-sunc++-4.x.h.
#### If you are using a version of Sun C++ prior to 4,2, or haven't
#### applied that jumbo patch to your 4.2, then add "-noex" to
#### CCFLAGS below.
#### -pta instantiates all template members, which makes libraries bigger.
#### But, it's supposed to put each member into it's own .o, so executable
#### size should not be penalized.
CCFLAGS		+= $(CFLAGS) -pta
DCFLAGS		+= -g
DLD		= $(CXX) 
LD		= $(CXX)

ifdef orbix
LDFLAGS		+= $(PIC) -L$(ORBIX_ROOT)/lib -R $(WRAPPER_ROOT)/ace -R $(ORBIX_ROOT)/lib
LIBS		+= -lorbixmt -lsocket -ldl -lnsl -lthread -lgen
else
LDFLAGS		+= -R $(WRAPPER_ROOT)/ace 
LIBS		+= -lsocket -ldl -lnsl -lgen
endif # orbix

PIC		= -PIC
AR		= CC
ARFLAGS		= -xar -o
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o

#### The following macro overrides enable creation of fast executables.
#### They _don't_ support fast compilation :-)
#### To use, add fast=1 to your "make" invocation.
####
#### -g is incompatible with -fast.  If you need -g, you can
#### use "-fast -O3".
####
#### Dynamic casting and RTTI can coexist with ACE, but it doesn't
#### use them.	To disable, add: -features=no%castop,no%rtti
#### Sun C++ 4.2 (and later?) only support this option.
ifdef fast
CFLAGS += -fast
CCFLAGS += -features=no%castop,no%rtti
DCFLAGS =
endif # fast
