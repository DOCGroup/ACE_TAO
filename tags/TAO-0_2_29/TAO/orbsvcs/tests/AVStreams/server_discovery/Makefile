#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for TAO
#----------------------------------------------------------------------------
shared_libs_only=1
MAKEFILE = Makefile
LIBNAME  = libavdemo
SHLIB    = $(LIBNAME).$(SOEXT)

BIN = main

IDL_SRCS= Video_Repository

IDL_FILES = \
	$(addsuffix S, $(IDL_SRCS)) \
	$(addsuffix C, $(IDL_SRCS))

SRCS =  $(IDL_FILES) \
	Trader_Client \
	Server_Discovery

FILES	 = $(SRCS)

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

LSRC	= $(addsuffix .cpp,$(FILES))

LDFLAGS += -L$(JAVA_ROOT)/jre/lib/sparc -L$(JAVA_ROOT)/jre/lib/sparc/native_threads

LIBS = -lorbsvcs -lTAO -lACE 
LDLIBS = -lavdemo -ljava -ljvm
VLDLIBS = $(LDLIBS:%=%$(VAR))
CPPFLAGS += -I$(JAVA_ROOT)/include -I$(JAVA_ROOT)/include/solaris \
	    -I$(TAO_ROOT)/orbsvcs -I$(TAO_ROOT) $(TSS_ORB_FLAG)

BUILD	= TAO_VR.module $(VSHLIB) $(VLIB) $(VBIN) 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

idl_stubs: $(addsuffix .h, $(IDL_FILES))

Video_Repository.idl: $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/server/Video_Repository.idl
	cp $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source/server/Video_Repository.idl .

TAO_VR.module: Video_Repository.idl
	$(JAVA_ROOT)/bin/idltojava Video_Repository.idl
	touch TAO_VR.module

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES)) \
	$(addsuffix _T.h, $(IDL_FILES)) $(addsuffix _T.i, $(IDL_FILES)) $(addsuffix _T.cpp, $(IDL_FILES)) 

IDL_EXT=C.h C.i C.cpp S.h S.i S.cpp S_T.h S_T.i S_T.cpp

.PRECIOUS: $(foreach file, $(IDL_SRCS), $(foreach ext, $(IDL_EXT), $(file)$(ext))))

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
