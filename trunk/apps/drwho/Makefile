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


.obj/client.o .obj/client.o .obj/client.o .obj/client.o: client.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h global.h SML_Client.h SM_Client.h PM_Client.h \
 Protocol_Manager.h Search_Struct.h Protocol_Record.h Drwho_Node.h \
 CM_Client.h Comm_Manager.h Select_Manager.h SML_Server.h SM_Server.h \
 PM_Server.h CM_Server.h SMR_Client.h

.obj/server.o .obj/server.o .obj/server.o .obj/server.o: server.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SMR_Server.h SM_Server.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Server.h \
 Comm_Manager.h Select_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/Protocol_Manager.o .obj/Protocol_Manager.o .obj/Protocol_Manager.o .obj/Protocol_Manager.o: Protocol_Manager.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h global.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h

.obj/Drwho_Node.o .obj/Drwho_Node.o .obj/Drwho_Node.o .obj/Drwho_Node.o: Drwho_Node.cpp Drwho_Node.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i

.obj/Rwho_DB_Manager.o .obj/Rwho_DB_Manager.o .obj/Rwho_DB_Manager.o .obj/Rwho_DB_Manager.o: Rwho_DB_Manager.cpp global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h Rwho_DB_Manager.h \
 $(ACE_ROOT)/ace/Dirent.h \
 $(ACE_ROOT)/ace/Dirent.i \
 Protocol_Record.h Drwho_Node.h

.obj/Multicast_Manager.o .obj/Multicast_Manager.o .obj/Multicast_Manager.o .obj/Multicast_Manager.o: Multicast_Manager.cpp Multicast_Manager.h \
 global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Mem_Map.i

.obj/Protocol_Record.o .obj/Protocol_Record.o .obj/Protocol_Record.o .obj/Protocol_Record.o: Protocol_Record.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Protocol_Record.h Drwho_Node.h

.obj/Options.o .obj/Options.o .obj/Options.o .obj/Options.o: Options.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Multicast_Manager.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Get_Opt.i

.obj/File_Manager.o .obj/File_Manager.o .obj/File_Manager.o .obj/File_Manager.o: File_Manager.cpp File_Manager.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i

.obj/Hash_Table.o .obj/Hash_Table.o .obj/Hash_Table.o .obj/Hash_Table.o: Hash_Table.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Hash_Table.h Search_Struct.h Protocol_Record.h Drwho_Node.h

.obj/Binary_Search.o .obj/Binary_Search.o .obj/Binary_Search.o .obj/Binary_Search.o: Binary_Search.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Binary_Search.h Search_Struct.h Protocol_Record.h Drwho_Node.h

.obj/Search_Struct.o .obj/Search_Struct.o .obj/Search_Struct.o .obj/Search_Struct.o: Search_Struct.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Search_Struct.h Protocol_Record.h Drwho_Node.h

.obj/Single_Lookup.o .obj/Single_Lookup.o .obj/Single_Lookup.o .obj/Single_Lookup.o: Single_Lookup.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Single_Lookup.h Search_Struct.h Protocol_Record.h Drwho_Node.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/SML_Server.o .obj/SML_Server.o .obj/SML_Server.o .obj/SML_Server.o: SML_Server.cpp SML_Server.h SM_Server.h PM_Server.h \
 Protocol_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h global.h Search_Struct.h Protocol_Record.h Drwho_Node.h \
 CM_Server.h Comm_Manager.h Select_Manager.h

.obj/SM_Server.o .obj/SM_Server.o .obj/SM_Server.o .obj/SM_Server.o: SM_Server.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 PMS_All.h PM_Server.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMS_Flo.h PMS_Usr.h PMS_Ruser.h \
 SM_Server.h CM_Server.h Comm_Manager.h Select_Manager.h

.obj/PMS_Usr.o .obj/PMS_Usr.o .obj/PMS_Usr.o .obj/PMS_Usr.o: PMS_Usr.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SL_Server.h Single_Lookup.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_Usr.h PM_Server.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/SL_Server.o .obj/SL_Server.o .obj/SL_Server.o .obj/SL_Server.o: SL_Server.cpp global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SL_Server.h Single_Lookup.h Options.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h

.obj/PMS_Flo.o .obj/PMS_Flo.o .obj/PMS_Flo.o .obj/PMS_Flo.o: PMS_Flo.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 BS_Server.h Binary_Search.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_Flo.h PM_Server.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/PM_Server.o .obj/PM_Server.o .obj/PM_Server.o .obj/PM_Server.o: PM_Server.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Rwho_DB_Manager.h \
 $(ACE_ROOT)/ace/Dirent.h \
 $(ACE_ROOT)/ace/Dirent.i \
 Protocol_Record.h Drwho_Node.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/HT_Server.o .obj/HT_Server.o .obj/HT_Server.o .obj/HT_Server.o: HT_Server.cpp HT_Server.h Hash_Table.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/BS_Server.o .obj/BS_Server.o .obj/BS_Server.o .obj/BS_Server.o: BS_Server.cpp BS_Server.h Binary_Search.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i

.obj/PMS_All.o .obj/PMS_All.o .obj/PMS_All.o .obj/PMS_All.o: PMS_All.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 HT_Server.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_All.h PM_Server.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/PMS_Ruser.o .obj/PMS_Ruser.o .obj/PMS_Ruser.o .obj/PMS_Ruser.o: PMS_Ruser.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 HT_Server.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMS_Ruser.h PM_Server.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/SMR_Server.o .obj/SMR_Server.o .obj/SMR_Server.o .obj/SMR_Server.o: SMR_Server.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SMR_Server.h SM_Server.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Server.h \
 Comm_Manager.h Select_Manager.h

.obj/CM_Client.o .obj/CM_Client.o .obj/CM_Client.o .obj/CM_Client.o: CM_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Multicast_Manager.h CM_Client.h Comm_Manager.h

.obj/SM_Client.o .obj/SM_Client.o .obj/SM_Client.o .obj/SM_Client.o: SM_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 PMC_All.h PM_Client.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMC_Flo.h PMC_Usr.h PMC_Ruser.h \
 SM_Client.h CM_Client.h Comm_Manager.h Select_Manager.h

.obj/SMR_Client.o .obj/SMR_Client.o .obj/SMR_Client.o .obj/SMR_Client.o: SMR_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 PMC_All.h PM_Client.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMC_Flo.h PMC_Usr.h PMC_Ruser.h \
 SMR_Client.h SM_Client.h CM_Client.h Comm_Manager.h Select_Manager.h

.obj/PM_Client.o .obj/PM_Client.o .obj/PM_Client.o .obj/PM_Client.o: PM_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 PM_Server.h Protocol_Manager.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PM_Client.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/HT_Client.o .obj/HT_Client.o .obj/HT_Client.o .obj/HT_Client.o: HT_Client.cpp HT_Client.h Hash_Table.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/BS_Client.o .obj/BS_Client.o .obj/BS_Client.o .obj/BS_Client.o: BS_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 File_Manager.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 BS_Client.h Binary_Search.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h

.obj/PMC_All.o .obj/PMC_All.o .obj/PMC_All.o .obj/PMC_All.o: PMC_All.cpp global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h HT_Client.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_All.h PM_Client.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/PMC_Flo.o .obj/PMC_Flo.o .obj/PMC_Flo.o .obj/PMC_Flo.o: PMC_Flo.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 BS_Client.h Binary_Search.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_Flo.h PM_Client.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/PMC_Usr.o .obj/PMC_Usr.o .obj/PMC_Usr.o .obj/PMC_Usr.o: PMC_Usr.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SL_Client.h Single_Lookup.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_Usr.h PM_Client.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/SL_Client.o .obj/SL_Client.o .obj/SL_Client.o .obj/SL_Client.o: SL_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SL_Client.h Single_Lookup.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h

.obj/PMC_Ruser.o .obj/PMC_Ruser.o .obj/PMC_Ruser.o .obj/PMC_Ruser.o: PMC_Ruser.cpp global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h HT_Client.h Hash_Table.h Search_Struct.h Protocol_Record.h \
 Drwho_Node.h PMC_Ruser.h PM_Client.h Protocol_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/SMR_Client.o .obj/SMR_Client.o .obj/SMR_Client.o .obj/SMR_Client.o: SMR_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 PMC_All.h PM_Client.h Protocol_Manager.h Search_Struct.h \
 Protocol_Record.h Drwho_Node.h PMC_Flo.h PMC_Usr.h PMC_Ruser.h \
 SMR_Client.h SM_Client.h CM_Client.h Comm_Manager.h Select_Manager.h

.obj/SML_Client.o .obj/SML_Client.o .obj/SML_Client.o .obj/SML_Client.o: SML_Client.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SML_Client.h SM_Client.h PM_Client.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Client.h \
 Comm_Manager.h Select_Manager.h SML_Server.h SM_Server.h PM_Server.h \
 CM_Server.h

.obj/CM_Server.o .obj/CM_Server.o .obj/CM_Server.o .obj/CM_Server.o: CM_Server.cpp global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h CM_Server.h Comm_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

.obj/client.o .obj/client.o .obj/client.o .obj/client.o: client.cpp \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 Options.h global.h SML_Client.h SM_Client.h PM_Client.h \
 Protocol_Manager.h Search_Struct.h Protocol_Record.h Drwho_Node.h \
 CM_Client.h Comm_Manager.h Select_Manager.h SML_Server.h SM_Server.h \
 PM_Server.h CM_Server.h SMR_Client.h

.obj/server.o .obj/server.o .obj/server.o .obj/server.o: server.cpp Options.h global.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 SMR_Server.h SM_Server.h PM_Server.h Protocol_Manager.h \
 Search_Struct.h Protocol_Record.h Drwho_Node.h CM_Server.h \
 Comm_Manager.h Select_Manager.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/ACE.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
