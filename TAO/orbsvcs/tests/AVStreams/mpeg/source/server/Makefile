#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
BIN =  server vs

BUILD = $(BIN)

AV_SERVER_OBJS = server.o
AV_VS_OBJS = vs.o

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES = server vs

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))


LIBS 	+= -lTAO -lACE -lorbsvcs -lmpeg_server -lmpeg_shared -lmpeg_mib

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

CPPFLAGS += -DSH_MEM -DNDEBUG 
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)#-H

server:$(addprefix $(VDIR),$(AV_SERVER_OBJS))
	$(LINK.cc) $(LDFAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

vs:$(addprefix $(VDIR),$(AV_VS_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)


clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
