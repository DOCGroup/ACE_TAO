#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for drwho
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN = drwho-client drwho-server

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
	     Single_Lookup \
	     SML_Server \
	     SM_Server \
	     PMS_Usr \
	     SL_Server \
	     PMS_Flo \
	     PM_Server \
	     HT_Server \
	     BS_Server \
	     PMS_All \
	     PMS_Ruser \
	     SMR_Server \
	     CM_Client \
	     SM_Client \
	     SMR_Client \
	     PM_Client \
	     HT_Client \
	     BS_Client \
	     PMC_All \
	     PMC_Flo \
	     PMC_Usr \
	     SL_Client \
	     PMC_Ruser \
	     SMR_Client \
	     SML_Client \
	     CM_Server 

SHARED-OBJ = $(addsuffix .o,$(SHARED-SRC))

CLIENT-SRC = client

CLIENT-OBJ = $(addsuffix .o,$(CLIENT-SRC))

SERVER-SRC = server

SERVER-OBJ = $(addsuffix .o,$(SERVER-SRC))

LSRC = client.cpp server.cpp \
       $(addsuffix .cpp,$(SHARED-SRC)) \
       $(addsuffix .cpp,$(CLIENT-SRC)) \
       $(addsuffix .cpp,$(SERVER-SRC))

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

.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp Options.h global.h \
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
 SML_Client.h SM_Client.h PM_Client.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Client.h \
 Select_Manager.h SML_Server.h SM_Server.h PM_Server.h CM_Server.h \
 Comm_Manager.h SMR_Client.h
.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp Options.h global.h \
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
 SMR_Server.h SM_Server.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Server.h \
 Comm_Manager.h Select_Manager.h
.obj/Protocol_Manager.o .obj/Protocol_Manager.so .shobj/Protocol_Manager.o .shobj/Protocol_Manager.so: Protocol_Manager.cpp Options.h global.h \
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
 Protocol_Manager.h Search_Struct.h Protocol_Record.h Drwho_Node.h
.obj/Drwho_Node.o .obj/Drwho_Node.so .shobj/Drwho_Node.o .shobj/Drwho_Node.so: Drwho_Node.cpp Drwho_Node.h global.h \
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
 $(ACE_ROOT)/ace/Log_Record.i
.obj/Rwho_DB_Manager.o .obj/Rwho_DB_Manager.so .shobj/Rwho_DB_Manager.o .shobj/Rwho_DB_Manager.so: Rwho_DB_Manager.cpp global.h \
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
 Options.h Rwho_DB_Manager.h \
 $(ACE_ROOT)/ace/Dirent.h \
 $(ACE_ROOT)/ace/Dirent.i \
 Protocol_Record.h Drwho_Node.h
.obj/Multicast_Manager.o .obj/Multicast_Manager.so .shobj/Multicast_Manager.o .shobj/Multicast_Manager.so: Multicast_Manager.cpp Multicast_Manager.h \
 global.h $(ACE_ROOT)/ace/OS.h \
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
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i
.obj/Protocol_Record.o .obj/Protocol_Record.so .shobj/Protocol_Record.o .shobj/Protocol_Record.so: Protocol_Record.cpp Options.h global.h \
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
 Protocol_Record.h Drwho_Node.h
.obj/Options.o .obj/Options.so .shobj/Options.o .shobj/Options.so: Options.cpp Options.h global.h \
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
 Multicast_Manager.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i
.obj/File_Manager.o .obj/File_Manager.so .shobj/File_Manager.o .shobj/File_Manager.so: File_Manager.cpp File_Manager.h global.h \
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
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i
.obj/Hash_Table.o .obj/Hash_Table.so .shobj/Hash_Table.o .shobj/Hash_Table.so: Hash_Table.cpp Options.h global.h \
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
 Hash_Table.h Search_Struct.h Protocol_Record.h Drwho_Node.h
.obj/Binary_Search.o .obj/Binary_Search.so .shobj/Binary_Search.o .shobj/Binary_Search.so: Binary_Search.cpp Options.h global.h \
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
 Binary_Search.h
.obj/Search_Struct.o .obj/Search_Struct.so .shobj/Search_Struct.o .shobj/Search_Struct.so: Search_Struct.cpp Options.h global.h \
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
 Search_Struct.h Protocol_Record.h Drwho_Node.h
.obj/Single_Lookup.o .obj/Single_Lookup.so .shobj/Single_Lookup.o .shobj/Single_Lookup.so: Single_Lookup.cpp Options.h global.h \
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
 Single_Lookup.h Search_Struct.h Protocol_Record.h Drwho_Node.h
.obj/PM_Client.o .obj/PM_Client.so .shobj/PM_Client.o .shobj/PM_Client.so: PM_Client.cpp Options.h global.h \
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
 PM_Server.h Protocol_Manager.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PM_Client.h
.obj/CM_Client.o .obj/CM_Client.so .shobj/CM_Client.o .shobj/CM_Client.so: CM_Client.cpp global.h \
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
 Options.h Multicast_Manager.h CM_Client.h
.obj/HT_Client.o .obj/HT_Client.so .shobj/HT_Client.o .shobj/HT_Client.so: HT_Client.cpp HT_Client.h Hash_Table.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h global.h \
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
 $(ACE_ROOT)/ace/Log_Record.i
.obj/BS_Client.o .obj/BS_Client.so .shobj/BS_Client.o .shobj/BS_Client.so: BS_Client.cpp Options.h global.h \
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
 File_Manager.h BS_Client.h Binary_Search.h
.obj/PMC_All.o .obj/PMC_All.so .shobj/PMC_All.o .shobj/PMC_All.so: PMC_All.cpp global.h \
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
 Options.h HT_Client.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_All.h PM_Client.h Protocol_Manager.h
.obj/PMC_Flo.o .obj/PMC_Flo.so .shobj/PMC_Flo.o .shobj/PMC_Flo.so: PMC_Flo.cpp Options.h global.h \
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
 BS_Client.h Binary_Search.h PMC_Flo.h PM_Client.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h
.obj/PMC_Usr.o .obj/PMC_Usr.so .shobj/PMC_Usr.o .shobj/PMC_Usr.so: PMC_Usr.cpp Options.h global.h \
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
 SL_Client.h Single_Lookup.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_Usr.h PM_Client.h Protocol_Manager.h
.obj/SL_Client.o .obj/SL_Client.so .shobj/SL_Client.o .shobj/SL_Client.so: SL_Client.cpp Options.h global.h \
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
 SL_Client.h Single_Lookup.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h
.obj/PMC_Ruser.o .obj/PMC_Ruser.so .shobj/PMC_Ruser.o .shobj/PMC_Ruser.so: PMC_Ruser.cpp global.h \
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
 Options.h HT_Client.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_Ruser.h PM_Client.h Protocol_Manager.h
.obj/SM_Client.o .obj/SM_Client.so .shobj/SM_Client.o .shobj/SM_Client.so: SM_Client.cpp Options.h global.h \
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
 PMC_All.h PM_Client.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMC_Flo.h PMC_Usr.h PMC_Ruser.h \
 SM_Client.h CM_Client.h Select_Manager.h
.obj/SMR_Client.o .obj/SMR_Client.so .shobj/SMR_Client.o .shobj/SMR_Client.so: SMR_Client.cpp Options.h global.h \
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
 PMC_All.h PM_Client.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMC_Flo.h PMC_Usr.h PMC_Ruser.h \
 SMR_Client.h SM_Client.h CM_Client.h Select_Manager.h
.obj/SML_Client.o .obj/SML_Client.so .shobj/SML_Client.o .shobj/SML_Client.so: SML_Client.cpp Options.h global.h \
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
 SML_Client.h SM_Client.h PM_Client.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Client.h \
 Select_Manager.h SML_Server.h SM_Server.h PM_Server.h CM_Server.h \
 Comm_Manager.h
.obj/PM_Server.o .obj/PM_Server.so .shobj/PM_Server.o .shobj/PM_Server.so: PM_Server.cpp Options.h global.h \
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
 Rwho_DB_Manager.h \
 $(ACE_ROOT)/ace/Dirent.h \
 $(ACE_ROOT)/ace/Dirent.i \
 Protocol_Record.h Drwho_Node.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h
.obj/CM_Server.o .obj/CM_Server.so .shobj/CM_Server.o .shobj/CM_Server.so: CM_Server.cpp global.h \
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
 Options.h CM_Server.h Comm_Manager.h
.obj/HT_Server.o .obj/HT_Server.so .shobj/HT_Server.o .shobj/HT_Server.so: HT_Server.cpp HT_Server.h Hash_Table.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h global.h \
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
 $(ACE_ROOT)/ace/Log_Record.i
.obj/BS_Server.o .obj/BS_Server.so .shobj/BS_Server.o .shobj/BS_Server.so: BS_Server.cpp BS_Server.h Binary_Search.h
.obj/PMS_All.o .obj/PMS_All.so .shobj/PMS_All.o .shobj/PMS_All.so: PMS_All.cpp Options.h global.h \
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
 HT_Server.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_All.h PM_Server.h Protocol_Manager.h
.obj/PMS_Flo.o .obj/PMS_Flo.so .shobj/PMS_Flo.o .shobj/PMS_Flo.so: PMS_Flo.cpp Options.h global.h \
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
 BS_Server.h Binary_Search.h PMS_Flo.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h
.obj/PMS_Usr.o .obj/PMS_Usr.so .shobj/PMS_Usr.o .shobj/PMS_Usr.so: PMS_Usr.cpp Options.h global.h \
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
 SL_Server.h Single_Lookup.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_Usr.h PM_Server.h Protocol_Manager.h
.obj/SL_Server.o .obj/SL_Server.so .shobj/SL_Server.o .shobj/SL_Server.so: SL_Server.cpp global.h \
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
 SL_Server.h Single_Lookup.h Options.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h
.obj/PMS_Ruser.o .obj/PMS_Ruser.so .shobj/PMS_Ruser.o .shobj/PMS_Ruser.so: PMS_Ruser.cpp Options.h global.h \
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
 HT_Server.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_Ruser.h PM_Server.h Protocol_Manager.h
.obj/SM_Server.o .obj/SM_Server.so .shobj/SM_Server.o .shobj/SM_Server.so: SM_Server.cpp Options.h global.h \
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
 PMS_All.h PM_Server.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMS_Flo.h PMS_Usr.h PMS_Ruser.h \
 SM_Server.h CM_Server.h Comm_Manager.h Select_Manager.h
.obj/SMR_Server.o .obj/SMR_Server.so .shobj/SMR_Server.o .shobj/SMR_Server.so: SMR_Server.cpp Options.h global.h \
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
 SMR_Server.h SM_Server.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Server.h \
 Comm_Manager.h Select_Manager.h
.obj/SMR_Client.o .obj/SMR_Client.so .shobj/SMR_Client.o .shobj/SMR_Client.so: SMR_Client.cpp Options.h global.h \
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
 PMC_All.h PM_Client.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMC_Flo.h PMC_Usr.h PMC_Ruser.h \
 SMR_Client.h SM_Client.h CM_Client.h Select_Manager.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
