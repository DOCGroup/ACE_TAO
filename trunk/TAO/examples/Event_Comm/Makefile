#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

LDLIBS = -lorbsvcs -lTAO
PROG_SRCS = Consumer_Handler.cpp \
	    Consumer_Input_Handler.cpp \
	    Event_Comm_i.cpp \
            Notifier_Handler.cpp \
	    Supplier_Input_Handler.cpp \
	    consumer.cpp \
            notifier.cpp \
	    supplier.cpp \
	    Notifier_Server.cpp

IDL_SRC	= Event_CommS.cpp Event_CommC.cpp

LSRC = $(IDL_SRC) $(PROG_SRCS)

CONSUMER_OBJS = Event_CommC.o \
                Event_CommS.o \
		Consumer_Handler.o \
	      	Consumer_Input_Handler.o \
	        Event_Comm_i.o \
		consumer.o

SUPPLIER_OBJS = Event_CommC.o \
		Event_CommS.o \
		Supplier_Input_Handler.o \
	        Event_Comm_i.o \
		Notifier_Handler.o \
		supplier.o

NOTIFIER_OBJS = Event_CommC.o \
		Event_CommS.o \
		Notifier_Server.o \
		Event_Comm_i.o \
		notifier.o

BIN = supplier \
      notifier \
      consumer

BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifeq ($(ACE_HAS_GNUG_PRE_2_8),1)

  # Since older g++ cannot handle nested classes properly, this sample
  # will not build on g++ prior to 2.8.0.
  all clean realclean:
	@echo Event_Comm is not built with older versions of g++

else # ! ACE_HAS_GNUG_PRE_2_8

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao -L$(TAO_ROOT)/orbsvcs/Naming_Service
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS: Event_CommC.cpp Event_CommS.cpp Event_CommC.h Event_CommS.h
#$(IDL_SRC): event.idl
#	$(TAO_ROOT)/TAO_IDL/tao_idl cubit.idl

supplier: $(addprefix $(VDIR),$(SUPPLIER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

consumer: $(addprefix $(VDIR),$(CONSUMER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

notifier: $(addprefix $(VDIR),$(NOTIFIER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-/bin/rm -rf Event_CommS.* Event_CommC.* Event_CommS_T.*

endif # ! ACE_HAS_GNUG_PRE_2_8

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Event_CommS.o .obj/Event_CommS.so .shobj/Event_CommS.o .shobj/Event_CommS.so: Event_CommS.cpp Event_CommS.h Event_CommC.h \
 Event_CommC.i Event_CommS_T.h Event_CommS_T.i Event_CommS_T.cpp \
 Event_CommS.i
.obj/Event_CommC.o .obj/Event_CommC.so .shobj/Event_CommC.o .shobj/Event_CommC.so: Event_CommC.cpp Event_CommC.h Event_CommC.i \
 Event_CommS.h Event_CommS_T.h Event_CommS_T.i Event_CommS_T.cpp \
 Event_CommS.i
.obj/Consumer_Handler.o .obj/Consumer_Handler.so .shobj/Consumer_Handler.o .shobj/Consumer_Handler.so: Consumer_Handler.cpp Consumer_Handler.h \
 Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/Consumer_Input_Handler.o .obj/Consumer_Input_Handler.so .shobj/Consumer_Input_Handler.o .shobj/Consumer_Input_Handler.so: Consumer_Input_Handler.cpp \
 Consumer_Input_Handler.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
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
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Consumer_Handler.h Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/Event_Comm_i.o .obj/Event_Comm_i.so .shobj/Event_Comm_i.o .shobj/Event_Comm_i.so: Event_Comm_i.cpp Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i
.obj/Notifier_Handler.o .obj/Notifier_Handler.so .shobj/Notifier_Handler.o .shobj/Notifier_Handler.so: Notifier_Handler.cpp Notifier_Handler.h \
 $(ACE_ROOT)/ace/CORBA_Handler.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
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
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h
.obj/Supplier_Input_Handler.o .obj/Supplier_Input_Handler.so .shobj/Supplier_Input_Handler.o .shobj/Supplier_Input_Handler.so: Supplier_Input_Handler.cpp Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i Notifier_Handler.h \
 $(ACE_ROOT)/ace/CORBA_Handler.h \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 Supplier_Input_Handler.h
.obj/consumer.o .obj/consumer.so .shobj/consumer.o .shobj/consumer.so: consumer.cpp Consumer_Handler.h Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 Consumer_Input_Handler.h
.obj/notifier.o .obj/notifier.so .shobj/notifier.o .shobj/notifier.so: notifier.cpp Notifier_Server.h Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/tao/TAO.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 notifier.h
.obj/supplier.o .obj/supplier.so .shobj/supplier.o .shobj/supplier.so: supplier.cpp Notifier_Handler.h \
 $(ACE_ROOT)/ace/CORBA_Handler.h \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Event_Handler.h \
 $(ACE_ROOT)/ace/Event_Handler.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.h \
 $(ACE_ROOT)/ace/SV_Semaphore_Simple.i \
 $(ACE_ROOT)/ace/SV_Semaphore_Complex.i \
 $(ACE_ROOT)/ace/Synch.i \
 $(ACE_ROOT)/ace/Synch_T.h \
 $(ACE_ROOT)/ace/Synch_T.i \
 $(ACE_ROOT)/ace/Thread.h \
 $(ACE_ROOT)/ace/Thread.i \
 $(ACE_ROOT)/ace/Atomic_Op.i \
 $(ACE_ROOT)/ace/Synch_T.cpp \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
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
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Pipe.h \
 $(ACE_ROOT)/ace/Pipe.i \
 Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h \
 Supplier_Input_Handler.h supplier.h
.obj/Notifier_Server.o .obj/Notifier_Server.so .shobj/Notifier_Server.o .shobj/Notifier_Server.so: Notifier_Server.cpp Notifier_Server.h \
 Event_Comm_i.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
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
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/SString.i \
 $(ACE_ROOT)/ace/Malloc_Base.h \
 $(ACE_ROOT)/ace/Log_Record.i \
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
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
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
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 Event_CommS.h Event_CommC.h Event_CommC.i Event_CommS_T.h \
 Event_CommS_T.i Event_CommS_T.cpp Event_CommS.i \
 $(TAO_ROOT)/tao/TAO.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/CosNamingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Naming_Utils.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/IOR_Multicast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.h \
 $(ACE_ROOT)/ace/SOCK_Dgram.i \
 $(ACE_ROOT)/ace/SOCK_Dgram_Mcast.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/CosNaming_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/Naming/Entries.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
