#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for repeating token client application
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= proc_sema

FILES 	= proc_sema

LSRC   	= $(addsuffix .cpp,$(FILES))
LOBJ   	= $(LSRC:%.cpp=$(VDIR)%.o)
SHOBJ	= $(addsuffix .so,$(FILES))

BUILD 	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/proc_sema.o .obj/proc_sema.so .shobj/proc_sema.o .shobj/proc_sema.so: proc_sema.cpp \
  $(ACE_ROOT)/ace/Process_Semaphore.h \
  $(ACE_ROOT)/ace/pre.h $(ACE_ROOT)/ace/Synch.h \
  $(ACE_ROOT)/ace/ACE_export.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/OS.h \
  $(ACE_ROOT)/ace/OS_Dirent.h \
  $(ACE_ROOT)/ace/OS_Export.h \
  $(ACE_ROOT)/ace/OS_Errno.h \
  $(ACE_ROOT)/ace/OS_Errno.inl \
  $(ACE_ROOT)/ace/OS_Dirent.inl \
  $(ACE_ROOT)/ace/OS_String.h \
  $(ACE_ROOT)/ace/Basic_Types.h \
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
  $(ACE_ROOT)/ace/Synch.i \
  $(ACE_ROOT)/ace/Synch_T.h \
  $(ACE_ROOT)/ace/Synch_T.i \
  $(ACE_ROOT)/ace/Thread.h \
  $(ACE_ROOT)/ace/Thread_Adapter.h \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
  $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
  $(ACE_ROOT)/ace/Base_Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread_Adapter.inl \
  $(ACE_ROOT)/ace/Thread.i \
  $(ACE_ROOT)/ace/Synch_T.cpp \
  $(ACE_ROOT)/ace/Log_Msg.h \
  $(ACE_ROOT)/ace/Log_Priority.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
  $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
  $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
  $(ACE_ROOT)/ace/Process_Semaphore.inl \
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
  $(ACE_ROOT)/ace/Get_Opt.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
