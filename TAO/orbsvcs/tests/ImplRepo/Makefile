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

LDLIBS = -lorbsvcs -lTAO
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs \
            -I$(TAO_ROOT)

IDL_SRC = AirplaneS.cpp AirplaneC.cpp NesteaS.cpp NesteaC.cpp
PROG_SRCS = \
            Repository_Test.cpp \
            airplane_client.cpp \
            airplane_server.cpp \
            airplane_client_i.cpp \
            airplane_server_i.cpp \
            airplane_i.cpp \
            nestea_client.cpp \
            nestea_server.cpp \
            nestea_client_i.cpp \
            nestea_server_i.cpp \
            nestea_i.cpp \
            IR_Helper.cpp

SRC=$(IDL_SRC) $(PROG_SRCS)

AIRPLANE_CLIENT_OBJS =  AirplaneC.o \
                        AirplaneS.o \
                        airplane_client.o \
                        airplane_client_i.o

AIRPLANE_SERVER_OBJS =  AirplaneC.o \
                        AirplaneS.o \
                        airplane_server.o \
                        airplane_server_i.o \
                        airplane_i.o \
                        IR_Helper.o

NESTEA_CLIENT_OBJS =  NesteaC.o \
                      NesteaS.o \
                      nestea_client.o \
                      nestea_client_i.o

NESTEA_SERVER_OBJS =  NesteaC.o \
                      NesteaS.o \
                      nestea_server.o \
                      nestea_server_i.o \
                      nestea_i.o \
                      IR_Helper.o

REPOSITORY_TEST_OBJS =  Repository_Test.o \
                        Repository.o \
                        Options.o

VLDLIBS = $(LDLIBS:%=%$(VAR))

BIN2 = airplane_server \
       airplane_client \
       nestea_server \
       nestea_client \
       Repository_Test

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (ImplRepo,$(findstring ImplRepo,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # ImplRepo

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

IDL_EXT=C.h C.i C.cpp S.h S.i S.cpp S_T.h S_T.i S_T.cpp

.PRECIOUS: $(foreach ext, $(IDL_EXT), Airplane$(ext))

airplane_server: $(addprefix $(VDIR),$(AIRPLANE_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

airplane_client: $(addprefix $(VDIR),$(AIRPLANE_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

nestea_server: $(addprefix $(VDIR),$(NESTEA_SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

nestea_client: $(addprefix $(VDIR),$(NESTEA_CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

Repository_Test: $(addprefix $(VDIR),$(REPOSITORY_TEST_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Airplane$(ext)) \
               $(foreach ext, $(IDL_EXT), Nestea$(ext))

$(VDIR)Repository.$(OBJEXT): ../../ImplRepo_Service/Repository.cpp
	$(COMPILE.cc) -o $@ $<
	${MVCMD}

$(VDIR)Options.$(OBJEXT): ../../ImplRepo_Service/Options.cpp
	$(COMPILE.cc) -o $@ $<
	${MVCMD}


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Repository_Test.o .obj/Repository_Test.so .shobj/Repository_Test.o .shobj/Repository_Test.so: Repository_Test.cpp \
 ../../ImplRepo_Service/Repository.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/inc_user_config.h \
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
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
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
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(ACE_ROOT)/ace/Signal.h \
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
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
