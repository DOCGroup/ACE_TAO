#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the entire ACE release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libCLASSIX.a

FILES    = \
        CLASSIX_OS            	\
        CLASSIX_Select_Reactor	\
        CLASSIX_Addr          	\
        CLASSIX_Port_Core     	\
        CLASSIX_Port          	\
        CLASSIX_Port_Default  	\
        CLASSIX_Group         	\
        CLASSIX_Group_Stamp   	\
        CLASSIX_Group_Dynamic 	\
        CLASSIX_SAP           	\
        CLASSIX_Peer_SAP      	\
        CLASSIX_Stream        	\
        CLASSIX_Dgram_Mcast   	\
        CLASSIX_CO_Acceptor   	\
        CLASSIX_CO_Connector  	\
        CLASSIX_CLD_Connector


TEMPLATE_FILES =

DEFS    = $(addsuffix .h,$(FILES)) Svc_Conf_Tokens.h
DEFS    += $(addsuffix .i,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))
LSRC2   = $(addsuffix .cpp,$(TEMPLATE_FILES))

BUILD   = $(VLIB)

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifneq ($(GHS),)
  ifeq ($(CPU),86)
    FILES += gethrtime
    LSRC += gethrtime.cpp
  endif # 86
endif # GHS

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#### Disable installs in this ($(ACE_ROOT)/ace) directory, because this
#### is the (default) destination of installs anyways.  And this will prevent
#### creation of a link from the ACE library to itself when the build of
#### the ACE library fails.
INSTALL =
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

#### Setting ACELIB to null allows it to be used for building shared libraries,
#### including libACE.$(SOEXT,) on certain platforms.
ACELIB =


ifeq ($(SUPPRESS_DASH_G),1)
#### Build this target without -g on some platforms.
  $(COMPILE-NO_DASH_G.cc) -o $@ $<
endif # SUPPRESS_DASH_G

ifeq ($(CHORUS),1)
  ifeq ($(CC),$(GHS_DIR)/build -driver ch68)
    #### Build this target without -g, because it causes ghs 1.8.8 to core
    #### dump.
    $(VDIR)Select_Reactor.o .obj/Select_Reactor.so .shobj/Select_Reactor.o .shobj/Select_Reactor.so:
    $(COMPILE-NO_DASH_G.cc) -o $@ $<
  endif # CC
endif # CHORUS

ifneq ($(GHS),)
  ifeq ($(CPU),86)
    $(VDIR)gethrtime.$(OBJEXT):
	make $@ ghs=0
  endif # 86
endif # GHS

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/CLASSIX_OS.o .obj/CLASSIX_OS.so .shobj/CLASSIX_OS.o .shobj/CLASSIX_OS.so: CLASSIX_OS.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.h \
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
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
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
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Select_Reactor.h \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_SAP.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_SAP.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Select_Reactor.i
.obj/CLASSIX_Select_Reactor.o .obj/CLASSIX_Select_Reactor.so .shobj/CLASSIX_Select_Reactor.o .shobj/CLASSIX_Select_Reactor.so: CLASSIX_Select_Reactor.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Select_Reactor.h \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Synch.h \
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
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_SAP.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_SAP.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Select_Reactor.i
.obj/CLASSIX_Addr.o .obj/CLASSIX_Addr.so .shobj/CLASSIX_Addr.o .shobj/CLASSIX_Addr.so: CLASSIX_Addr.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.i
.obj/CLASSIX_Port_Core.o .obj/CLASSIX_Port_Core.so .shobj/CLASSIX_Port_Core.o .shobj/CLASSIX_Port_Core.so: CLASSIX_Port_Core.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.h \
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
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.h \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.i
.obj/CLASSIX_Port.o .obj/CLASSIX_Port.so .shobj/CLASSIX_Port.o .shobj/CLASSIX_Port.so: CLASSIX_Port.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
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
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.h \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.i
.obj/CLASSIX_Port_Default.o .obj/CLASSIX_Port_Default.so .shobj/CLASSIX_Port_Default.o .shobj/CLASSIX_Port_Default.so: CLASSIX_Port_Default.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Default.h \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Synch.h \
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
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.h \
 $(ACE_ROOT)/ace/Select_Reactor.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Token.h \
 $(ACE_ROOT)/ace/Token.i \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Select_Reactor.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Core.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_OS.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port_Default.i
.obj/CLASSIX_Group.o .obj/CLASSIX_Group.so .shobj/CLASSIX_Group.o .shobj/CLASSIX_Group.so: CLASSIX_Group.cpp \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Group.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.h \
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
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Addr.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.h \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Port.i \
 $(ACE_ROOT)/ace/CLASSIX/CLASSIX_Group.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
