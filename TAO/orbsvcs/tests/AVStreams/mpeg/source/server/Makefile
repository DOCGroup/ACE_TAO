#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

MAKEFILE = Makefile
BIN =  server

BUILD = $(BIN)

# On non-Windows environment, we should at least define
# the export_include IDL flag.
FILES= server

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))


LIBS 	+= -lTAO -lACE

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT)/include -I$(MPEG_ROOT)/mpeg_shared -I$(MPEG_ROOT)/mpeg_server -I$(MPEG_ROOT)/mpeg_client

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

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

LDFLAGS += -lmpeg_server -lmpeg_shared -lmpeg_mib
MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT)/include -I$(MPEG_ROOT)/mpeg_shared -I$(MPEG_ROOT)/mpeg_server -I$(MPEG_ROOT)/mpeg_client
CPPFLAGS += -DSH_MEM -DNDEBUG -DSunOS -DSTAT -I../include -I../mpeg_shared
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat -I$(TAO_ROOT)/orbsvcs $(TSS_ORB_FLAG)#-H

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

realclean: clean
	-/bin/rm -rf $(addsuffix .h, $(IDL_FILES)) $(addsuffix .i, $(IDL_FILES)) $(addsuffix .cpp, $(IDL_FILES))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.



# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
