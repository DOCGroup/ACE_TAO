#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for drwho
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

SHARED-SRC = Protocol_Manager \
	     Drwho_Node \
	     Rwho_DB_Manager \
	     Multicast_Manager \
	     Protocol_Record \
	     Options \
	     File_Manager \
	     Hash_Table \
	     Binary_Search \
	     Search_Struct \
	     Single_Lookup 

SHARED-OBJ = $(addsuffix .o,$(SHARED-SRC))

CLIENT-SRC = PM_Client \
	     CM_Client \
	     HT_Client \
	     BS_Client \
	     PMC_All \
	     PMC_Flo \
	     PMC_Usr \
	     SL_Client \
	     PMC_Ruser \
	     SM_Client \
	     SMR_Client \
	     SML_Client 
CLIENT-OBJ = $(addsuffix .o,$(CLIENT-SRC))

SERVER-SRC = PM_Server \
	     CM_Server \
	     HT_Server \
	     BS_Server \
	     PMS_All \
	     PMS_Flo \
	     PMS_Usr \
	     SL_Server \
	     PMS_Ruser \
	     SM_Server \
	     SMR_Server \
	     SMR_Client 
SERVER-OBJ = $(addsuffix .o,$(SERVER-SRC))

LSRC = client.cpp server.cpp \
       $(addsuffix .cpp,$(SHARED-SRC)) \
       $(addsuffix .cpp,$(CLIENT-SRC)) \
       $(addsuffix .cpp,$(SERVER-SRC))

BIN   	= drwho-client drwho-server
BUILD   = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN    = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

drwho-client: $(addprefix $(VDIR),$(CLIENT-OBJ)) $(addprefix $(VDIR),$(SHARED-OBJ))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

drwho-server: $(addprefix $(VDIR),$(SERVER-OBJ)) $(addprefix $(VDIR),$(SHARED-OBJ))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS)

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h SML_Client.h SM_Client.h PM_Client.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h CM_Client.h \
 Select_Manager.h SML_Server.h SM_Server.h PM_Server.h CM_Server.h \
 Comm_Manager.h SMR_Client.h
.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h SMR_Server.h SM_Server.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h CM_Server.h \
 Comm_Manager.h Select_Manager.h
.obj/Protocol_Manager.o .obj/Protocol_Manager.so .shobj/Protocol_Manager.o .shobj/Protocol_Manager.so: Protocol_Manager.cpp Options.h new.h \
 Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h
.obj/Drwho_Node.o .obj/Drwho_Node.so .shobj/Drwho_Node.o .shobj/Drwho_Node.so: Drwho_Node.cpp Drwho_Node.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 global.h
.obj/Protocol_Record.o .obj/Protocol_Record.so .shobj/Protocol_Record.o .shobj/Protocol_Record.so: Protocol_Record.cpp Options.h Protocol_Record.h \
 Drwho_Node.h $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 global.h
.obj/File_Manager.o .obj/File_Manager.so .shobj/File_Manager.o .shobj/File_Manager.so: File_Manager.cpp global.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Mem_Map.i \
 File_Manager.h
.obj/Hash_Table.o .obj/Hash_Table.so .shobj/Hash_Table.o .shobj/Hash_Table.so: Hash_Table.cpp Options.h new.h Hash_Table.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 global.h
.obj/Binary_Search.o .obj/Binary_Search.so .shobj/Binary_Search.o .shobj/Binary_Search.so: Binary_Search.cpp Options.h Binary_Search.h
.obj/Search_Struct.o .obj/Search_Struct.so .shobj/Search_Struct.o .shobj/Search_Struct.so: Search_Struct.cpp Options.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 global.h
.obj/new.o .obj/new.so .shobj/new.o .shobj/new.so: new.cpp \
 \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h
.obj/CM_Client.o .obj/CM_Client.so .shobj/CM_Client.o .shobj/CM_Client.so: CM_Client.cpp global.h Options.h Multicast_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 CM_Client.h
.obj/BS_Client.o .obj/BS_Client.so .shobj/BS_Client.o .shobj/BS_Client.so: BS_Client.cpp Options.h new.h File_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 BS_Client.h Binary_Search.h
.obj/SM_Client.o .obj/SM_Client.so .shobj/SM_Client.o .shobj/SM_Client.so: SM_Client.cpp Options.h new.h PMC_All.h PM_Client.h \
 Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h PMC_Flo.h \
 PMC_Usr.h PMC_Ruser.h SM_Client.h CM_Client.h Select_Manager.h
.obj/SMR_Client.o .obj/SMR_Client.so .shobj/SMR_Client.o .shobj/SMR_Client.so: SMR_Client.cpp Options.h new.h PMC_All.h PM_Client.h \
 Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h PMC_Flo.h \
 PMC_Usr.h PMC_Ruser.h SMR_Client.h SM_Client.h CM_Client.h \
 Select_Manager.h
.obj/SML_Client.o .obj/SML_Client.so .shobj/SML_Client.o .shobj/SML_Client.so: SML_Client.cpp Options.h SML_Client.h SM_Client.h \
 PM_Client.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h CM_Client.h \
 Select_Manager.h SML_Server.h SM_Server.h PM_Server.h CM_Server.h \
 Comm_Manager.h
.obj/CM_Server.o .obj/CM_Server.so .shobj/CM_Server.o .shobj/CM_Server.so: CM_Server.cpp global.h Options.h CM_Server.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Comm_Manager.h
.obj/BS_Server.o .obj/BS_Server.so .shobj/BS_Server.o .shobj/BS_Server.so: BS_Server.cpp BS_Server.h Binary_Search.h new.h
.obj/SM_Server.o .obj/SM_Server.so .shobj/SM_Server.o .shobj/SM_Server.so: SM_Server.cpp new.h Options.h PMS_All.h PM_Server.h \
 Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h PMS_Flo.h \
 PMS_Usr.h PMS_Ruser.h SM_Server.h CM_Server.h Comm_Manager.h \
 Select_Manager.h
.obj/SMR_Server.o .obj/SMR_Server.so .shobj/SMR_Server.o .shobj/SMR_Server.so: SMR_Server.cpp Options.h SMR_Server.h SM_Server.h \
 PM_Server.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h CM_Server.h \
 Comm_Manager.h Select_Manager.h
.obj/SMR_Client.o .obj/SMR_Client.so .shobj/SMR_Client.o .shobj/SMR_Client.so: SMR_Client.cpp Options.h new.h PMC_All.h PM_Client.h \
 Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-g++-common.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h global.h PMC_Flo.h \
 PMC_Usr.h PMC_Ruser.h SMR_Client.h SM_Client.h CM_Client.h \
 Select_Manager.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
