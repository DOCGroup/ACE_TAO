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
endif

LDLIBS = -lorbsvcs -lTAO

PROG_SRCS = \
	ImplRepo.cpp \
	ImplRepo_i.cpp \
	Repository.cpp

SRC = $(PROG_SRCS)

SIMPLE_SVR_OBJS = \
		ImplRepo.o \
		ImplRepo_i.o \
		Repository.o

BIN = ImplRepo_Service
BUILD = $(BIN)
VLDLIBS = $(LDLIBS:%=%$(VAR))
VBIN = $(BIN:%=%$(VAR))

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------


LDFLAGS += -L$(TAO_ROOT)/orbsvcs/orbsvcs -L$(TAO_ROOT)/tao
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

ImplRepo_Service:	$(addprefix $(VDIR),$(SIMPLE_SVR_OBJS))
			$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/ImplRepo.o .obj/ImplRepo.so .shobj/ImplRepo.o .shobj/ImplRepo.so: ImplRepo.cpp ImplRepo_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS.i \
 Repository.h \
 $(ACE_ROOT)/ace/Naming_Context.h \
 $(ACE_ROOT)/ace/Name_Proxy.h \
 $(ACE_ROOT)/ace/Name_Request_Reply.h \
 $(ACE_ROOT)/ace/Name_Space.h \
 $(TAO_ROOT)/tao/TAO.h
.obj/ImplRepo_i.o .obj/ImplRepo_i.so .shobj/ImplRepo_i.o .shobj/ImplRepo_i.so: ImplRepo_i.cpp ImplRepo_i.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/orbsvcs_export.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/PingS.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoC.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS_T.h \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS_T.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS_T.cpp \
 $(TAO_ROOT)/orbsvcs/orbsvcs/ImplRepoS.i \
 Repository.h \
 $(ACE_ROOT)/ace/Naming_Context.h \
 $(ACE_ROOT)/ace/Name_Proxy.h \
 $(ACE_ROOT)/ace/Name_Request_Reply.h \
 $(ACE_ROOT)/ace/Name_Space.h \
 $(TAO_ROOT)/tao/TAO.h \
 $(ACE_ROOT)/ace/Get_Opt.h \
 $(ACE_ROOT)/ace/Get_Opt.i \
 $(ACE_ROOT)/ace/Read_Buffer.h \
 $(ACE_ROOT)/ace/Read_Buffer.i \
 $(ACE_ROOT)/ace/Process.h \
 $(ACE_ROOT)/ace/Process.i \
 $(TAO_ROOT)/tao/IIOP_Profile.h
.obj/Repository.o .obj/Repository.so .shobj/Repository.o .shobj/Repository.so: Repository.cpp Repository.h \
 $(ACE_ROOT)/ace/Naming_Context.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
 $(ACE_ROOT)/ace/config.h \
 $(ACE_ROOT)/ace/config-sunos5.5.h \
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
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
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
 $(ACE_ROOT)/ace/Free_List.h \
 $(ACE_ROOT)/ace/Free_List.i \
 $(ACE_ROOT)/ace/Free_List.cpp \
 $(ACE_ROOT)/ace/Malloc_T.i \
 $(ACE_ROOT)/ace/Malloc_T.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.h \
 $(ACE_ROOT)/ace/Signal.h \
 $(ACE_ROOT)/ace/Signal.i \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(ACE_ROOT)/ace/Mem_Map.h \
 $(ACE_ROOT)/ace/Mem_Map.i \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Name_Proxy.h \
 $(ACE_ROOT)/ace/INET_Addr.h \
 $(ACE_ROOT)/ace/Addr.h \
 $(ACE_ROOT)/ace/Addr.i \
 $(ACE_ROOT)/ace/INET_Addr.i \
 $(ACE_ROOT)/ace/SOCK_Connector.h \
 $(ACE_ROOT)/ace/SOCK_Stream.h \
 $(ACE_ROOT)/ace/SOCK_IO.h \
 $(ACE_ROOT)/ace/SOCK.h \
 $(ACE_ROOT)/ace/IPC_SAP.h \
 $(ACE_ROOT)/ace/IPC_SAP.i \
 $(ACE_ROOT)/ace/SOCK.i \
 $(ACE_ROOT)/ace/SOCK_IO.i \
 $(ACE_ROOT)/ace/SOCK_Stream.i \
 $(ACE_ROOT)/ace/Time_Value.h \
 $(ACE_ROOT)/ace/SOCK_Connector.i \
 $(ACE_ROOT)/ace/Service_Config.h \
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
 $(ACE_ROOT)/ace/Synch_Options.h \
 $(ACE_ROOT)/ace/Synch_Options.i \
 $(ACE_ROOT)/ace/Name_Request_Reply.h \
 $(ACE_ROOT)/ace/Name_Space.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
