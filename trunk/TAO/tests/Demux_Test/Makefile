#----------------------------------------------------------------------------
#	$Id$
#
#	Top-level Makefile for the TAO_TTCP benchmark
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LDLIBS     	= -lTAO

PROG_SRCS 	= client.cpp tao_demuxC.cpp tao_demuxS.cpp tao_demux_i.cpp server.cpp

LSRC = $(PROG_SRCS)

TAO_DEMUX_CLIENT_OBJS = client.o tao_demuxC.o

TAO_DEMUX_SERVER_OBJS = server.o tao_demuxC.o tao_demuxS.o tao_demux_i.o

BIN = client server

BUILD = $(BIN)

VLDLIBS = $(LDLIBS:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
#include $(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include $(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
# Local modifications to variables imported by includes above.
#----------------------------------------------------------------------------

LDFLAGS    += -lTAO
LDFLAGS += -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat

ifdef quantify
LDFLAGS	+= -L/pkg/purify/quantify-2.1-solaris2/ -lquantify_stubs
endif

ifdef debug
CPPFLAGS += -DDEBUG
endif

server:	$(addprefix $(VDIR),$(TAO_DEMUX_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

client:	$(addprefix $(VDIR),$(TAO_DEMUX_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

clean:
	$(MAKE) realclean
	rm -fr tao_demux* gperf* *.i client server

