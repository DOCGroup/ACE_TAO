#----------------------------------------------------------------------------
#
#	$Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lTAO

IDL_SRC	= FooC.cpp FooS.cpp
PROG_SRCS =	$(IDL_SRC) server.cpp client.cpp Foo_i.cpp

LSRC = $(PROG_SRCS)

FILE_SVR_OBJS = FooC.o FooS.o server.o Foo_i.o
FILE_CLT_OBJS = FooC.o FooS.o client.o

BIN = server client
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

# Since older g++ cannot handle nested classes properly, this test will not
# build on g++ prior to 2.8.0.
ifeq ($(findstring g++,$(CXX)),g++)
  ifeq ($(CXX),g++)
    #### Determine if the g++ version is <= 2.7.x.
    OLD_G++ := \
      $(shell \
        if $(CXX) --version | egrep '^(cygnus-)?2\.[0-7]' > /dev/null; \
        then echo 1; fi)
  # else, CXX is something like g++386.  Assume that it can't handle
  # the build in this directory.  The shell command above won't
  # work if CXX isn't on the users PATH.
  endif # plain g++
endif # g++

ifeq ($(OLD_G++),1)
  all clean realclean:
	@echo POA/TIE is not built with older versions of g++
else # ! old g++
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
#include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

.PRECIOUS: FooC.h FooC.i FooC.cpp FooS.h FooS.i FooS.cpp

server:	$(addprefix $(VDIR),$(FILE_SVR_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

client:	$(addprefix $(VDIR),$(FILE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf FooC.* FooS.*

endif # ! old g++

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
