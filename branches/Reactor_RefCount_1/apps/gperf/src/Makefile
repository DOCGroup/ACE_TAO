#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for GPERF release
#----------------------------------------------------------------------------

BIN     = gperf

FILES   = Options \
          Iterator \
          Gen_Perf \
          Key_List \
          List_Node \
          Hash_Table \
          Bool_Array \
          Vectors \
          Version

SRC    = $(addsuffix .cpp,$(FILES))
OBJ	= $(addsuffix .o,$(FILES))

BUILD   = $(VBIN)

INSTALL = $(VBIN:%=$(INSBIN)/%$(EXEEXT))

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/Options.o .obj/Options.so .shobj/Options.o .shobj/Options.so: Options.cpp Options.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Get_Opt.h \
  $(ACE_ROOT)/ace/SString.h \
  $(ACE_ROOT)/ace/String_Base.h \
  $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i \
  $(ACE_ROOT)/ace/ACE.i \
  $(ACE_ROOT)/ace/String_Base_Const.h \
  $(ACE_ROOT)/ace/String_Base.i \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/String_Base.cpp \
  $(ACE_ROOT)/ace/Malloc.h \
  $(ACE_ROOT)/ace/Malloc.i \
  $(ACE_ROOT)/ace/Malloc_T.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Allocator.h \
  $(ACE_ROOT)/ace/Malloc_Allocator.i \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Malloc_T.i \
  $(ACE_ROOT)/ace/Malloc_T.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/Signal.h \
  $(ACE_ROOT)/ace/Signal.i \
  $(ACE_ROOT)/ace/Mem_Map.h \
  $(ACE_ROOT)/ace/Mem_Map.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Node.h $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Memory_Pool.i \
  $(ACE_ROOT)/ace/Auto_Ptr.h \
  $(ACE_ROOT)/ace/Auto_Ptr.i \
  $(ACE_ROOT)/ace/Auto_Ptr.cpp \
  $(ACE_ROOT)/ace/SString.i \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Get_Opt.i Iterator.h

.obj/Iterator.o .obj/Iterator.so .shobj/Iterator.o .shobj/Iterator.so: Iterator.cpp Iterator.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl Options.h \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl

.obj/Gen_Perf.o .obj/Gen_Perf.so .shobj/Gen_Perf.o .shobj/Gen_Perf.so: Gen_Perf.cpp Gen_Perf.h Options.h \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl Key_List.h \
  List_Node.h Vectors.h Bool_Array.h

.obj/Key_List.o .obj/Key_List.so .shobj/Key_List.o .shobj/Key_List.so: Key_List.cpp Key_List.h Options.h \
  $(ACE_ROOT)/ace/Log_Msg.h $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl List_Node.h \
  Vectors.h $(ACE_ROOT)/ace/Read_Buffer.h \
  $(ACE_ROOT)/ace/Read_Buffer.i Hash_Table.h

.obj/List_Node.o .obj/List_Node.so .shobj/List_Node.o .shobj/List_Node.so: List_Node.cpp List_Node.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl Options.h \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl Vectors.h

.obj/Hash_Table.o .obj/Hash_Table.so .shobj/Hash_Table.o .shobj/Hash_Table.so: Hash_Table.cpp Hash_Table.h \
  $(ACE_ROOT)/ace/OS.h $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  Options.h $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl List_Node.h \
  $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i \
  $(ACE_ROOT)/ace/ACE.i

.obj/Bool_Array.o .obj/Bool_Array.so .shobj/Bool_Array.o .shobj/Bool_Array.so: Bool_Array.cpp Bool_Array.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl Options.h \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl

.obj/Vectors.o .obj/Vectors.so .shobj/Vectors.o .shobj/Vectors.so: Vectors.cpp Vectors.h $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i

.obj/Version.o .obj/Version.so .shobj/Version.o .shobj/Version.so: Version.cpp $(ACE_ROOT)/ace/ACE.h \
  $(ACE_ROOT)/ace/pre.h $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/Basic_Types.i \
  $(ACE_ROOT)/ace/OS_String.inl \
  $(ACE_ROOT)/ace/OS_Memory.h \
  $(ACE_ROOT)/ace/OS_Memory.inl \
  $(ACE_ROOT)/ace/OS_TLI.h \
  $(ACE_ROOT)/ace/OS_TLI.inl \
  $(ACE_ROOT)/ace/Time_Value.h \
  $(ACE_ROOT)/ace/Time_Value.inl \
  $(ACE_ROOT)/ace/Default_Constants.h \
  $(ACE_ROOT)/ace/Global_Macros.h \
  $(ACE_ROOT)/ace/Min_Max.h \
  $(ACE_ROOT)/ace/streams.h \
  $(ACE_ROOT)/ace/Trace.h $(ACE_ROOT)/ace/OS.i \
  $(ACE_ROOT)/ace/Flag_Manip.h \
  $(ACE_ROOT)/ace/Flag_Manip.i \
  $(ACE_ROOT)/ace/Handle_Ops.h \
  $(ACE_ROOT)/ace/Handle_Ops.i \
  $(ACE_ROOT)/ace/Lib_Find.h \
  $(ACE_ROOT)/ace/Lib_Find.i \
  $(ACE_ROOT)/ace/Init_ACE.h \
  $(ACE_ROOT)/ace/Init_ACE.i \
  $(ACE_ROOT)/ace/Sock_Connect.h \
  $(ACE_ROOT)/ace/Sock_Connect.i \
  $(ACE_ROOT)/ace/ACE.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
