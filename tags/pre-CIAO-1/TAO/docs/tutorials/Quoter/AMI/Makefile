#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#       Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

IDL_FILES = Quoter
IDL_SRC = QuoterC QuoterS
BIN = client server

LDLIBS = -lTAO_Messaging -lTAO_PortableServer -lTAO 

SRC = $(addsuffix .cpp, $(BIN) $(IDL_SRC) Handler_i Stock_i Stock_Factory_i)

CLIENT_OBJS = $(addsuffix .o, client Handler_i $(IDL_SRC))
SERVER_OBJS = $(addsuffix .o, server Stock_i Stock_Factory_i $(IDL_SRC))

TAO_IDLFLAGS=-GC -in

#----------------------------------------------------------------------------
# Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), Quoter$(ext))

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Quoter$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp Handler_i.h QuoterS.h QuoterC.h QuoterC.i \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp Stock_Factory_i.h Stock_i.h QuoterS.h QuoterC.h \
 QuoterC.i QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/QuoterC.o .obj/QuoterC.so .shobj/QuoterC.o .shobj/QuoterC.so: QuoterC.cpp QuoterC.h QuoterC.i

.obj/QuoterS.o .obj/QuoterS.so .shobj/QuoterS.o .shobj/QuoterS.so: QuoterS.cpp QuoterS.h QuoterC.h QuoterC.i QuoterS_T.h \
 QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/Handler_i.o .obj/Handler_i.so .shobj/Handler_i.o .shobj/Handler_i.so: Handler_i.cpp Handler_i.h QuoterS.h QuoterC.h QuoterC.i \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/Stock_i.o .obj/Stock_i.so .shobj/Stock_i.o .shobj/Stock_i.so: Stock_i.cpp Stock_i.h QuoterS.h QuoterC.h QuoterC.i \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/Stock_Factory_i.o .obj/Stock_Factory_i.so .shobj/Stock_Factory_i.o .shobj/Stock_Factory_i.so: Stock_Factory_i.cpp Stock_Factory_i.h Stock_i.h \
 QuoterS.h QuoterC.h QuoterC.i QuoterS_T.h QuoterS_T.i QuoterS_T.cpp \
 QuoterS.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
