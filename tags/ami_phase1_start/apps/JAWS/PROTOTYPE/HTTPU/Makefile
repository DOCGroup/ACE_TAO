#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the ACE Adapter Web Server (JAWS) client
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

#BIN = test_parse_url test_parse_http_request test_http_headers \
#      test_http_request test_http_response

SHLIB = libHTTPU.so
FILES = parse_url parse_http_request parse_http_response \
        http_request http_response http_base http_status \
	http_headers

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))

CPPFLAGS += -I..

LDLIBS  = -lhttpu -laoutil
BUILD   = $(VSHLIB) $(SHLIBA) $(VLIB)


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include		$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include		$(ACE_ROOT)/include/makeinclude/macros.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include		$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/parse_url.o .obj/parse_url.so .shobj/parse_url.o .shobj/parse_url.so: parse_url.cpp $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i ../HTTPU/parse_url.h \
 ../HTTPU/http_export.h
.obj/parse_http_request.o .obj/parse_http_request.so .shobj/parse_http_request.o .shobj/parse_http_request.so: parse_http_request.cpp \
 ../HTTPU/parse_http_request.h $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i ../HTTPU/http_headers.h \
 $(ACE_ROOT)/ace/RB_Tree.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/RB_Tree.i \
 $(ACE_ROOT)/ace/RB_Tree.cpp \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp ../JAWS/Parse_Headers.h \
 ../JAWS/Export.h $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp ../JAWS/Headers.h \
 ../HTTPU/http_export.h ../HTTPU/http_headers.i \
 ../HTTPU/parse_http_request.i
.obj/parse_http_response.o .obj/parse_http_response.so .shobj/parse_http_response.o .shobj/parse_http_response.so: parse_http_response.cpp \
 ../HTTPU/parse_http_response.h $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i ../HTTPU/http_export.h \
 ../HTTPU/parse_http_response.i
.obj/http_request.o .obj/http_request.so .shobj/http_request.o .shobj/http_request.so: http_request.cpp ../HTTPU/http_request.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp ../JAWS/Parse_Headers.h \
 ../JAWS/Export.h $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp ../JAWS/Headers.h \
 ../HTTPU/http_export.h ../HTTPU/http_base.h ../HTTPU/http_status.h \
 ../HTTPU/http_status.i ../HTTPU/http_headers.h \
 $(ACE_ROOT)/ace/RB_Tree.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/RB_Tree.i \
 $(ACE_ROOT)/ace/RB_Tree.cpp ../HTTPU/http_headers.i \
 ../HTTPU/http_base.i ../HTTPU/parse_http_request.h \
 ../HTTPU/parse_http_request.i ../HTTPU/parse_url.h \
 ../HTTPU/http_request.i
.obj/http_response.o .obj/http_response.so .shobj/http_response.o .shobj/http_response.so: http_response.cpp ../HTTPU/http_response.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp ../JAWS/Parse_Headers.h \
 ../JAWS/Export.h $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp ../JAWS/Headers.h \
 ../HTTPU/http_export.h ../HTTPU/http_base.h ../HTTPU/http_status.h \
 ../HTTPU/http_status.i ../HTTPU/http_headers.h \
 $(ACE_ROOT)/ace/RB_Tree.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/RB_Tree.i \
 $(ACE_ROOT)/ace/RB_Tree.cpp ../HTTPU/http_headers.i \
 ../HTTPU/http_base.i ../HTTPU/parse_http_response.h \
 ../HTTPU/parse_http_response.i ../HTTPU/http_response.i
.obj/http_base.o .obj/http_base.so .shobj/http_base.o .shobj/http_base.so: http_base.cpp ../JAWS/Parse_Headers.h ../JAWS/Export.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp ../JAWS/Headers.h \
 ../HTTPU/http_base.h ../HTTPU/http_export.h ../HTTPU/http_status.h \
 ../HTTPU/http_status.i ../HTTPU/http_headers.h \
 $(ACE_ROOT)/ace/RB_Tree.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/RB_Tree.i \
 $(ACE_ROOT)/ace/RB_Tree.cpp ../HTTPU/http_headers.i \
 ../HTTPU/http_base.i
.obj/http_status.o .obj/http_status.so .shobj/http_status.o .shobj/http_status.so: http_status.cpp ../HTTPU/http_status.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp ../HTTPU/http_export.h \
 ../HTTPU/http_status.i
.obj/http_headers.o .obj/http_headers.so .shobj/http_headers.o .shobj/http_headers.so: http_headers.cpp $(ACE_ROOT)/ace/RB_Tree.h \
 $(ACE_ROOT)/ace/ACE.h $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config-sunos5.6.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/OS.i $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/ACE.i \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/RB_Tree.i \
 $(ACE_ROOT)/ace/RB_Tree.cpp ../HTTPU/http_headers.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp ../JAWS/Parse_Headers.h \
 ../JAWS/Export.h $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Malloc.h $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp ../JAWS/Headers.h \
 ../HTTPU/http_export.h ../HTTPU/http_headers.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
