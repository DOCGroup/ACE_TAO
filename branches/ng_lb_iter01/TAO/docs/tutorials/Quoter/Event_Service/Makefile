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

LDLIBS = -lTAO_CosEvent -lTAO_CosNaming -lTAO_Svc_Utils -lTAO_IORTable -lTAO_PortableServer -lTAO
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

IDL_FILES = Quoter
IDL_SRC = QuoterC.cpp QuoterS.cpp
BIN2 = client server

#### If the TAO orbsvcs library wasn't built with sufficient components,
#### don't try to build here.
TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (CosEvent,$(findstring CosEvent,$(TAO_ORBSVCS)))
  BIN = $(BIN2)
endif # CosEvent

SRC = $(IDL_SRC) $(addsuffix .cpp, $(BIN) Stock_Consumer Stock_i Stock_Factory_i)

CLIENT_OBJS = $(addsuffix .o, client Stock_Consumer) $(IDL_SRC:.cpp=.o)
SERVER_OBJS = $(addsuffix .o, server Stock_i Stock_Factory_i) $(IDL_SRC:.cpp=.o)

TAO_IDLFLAGS=-in

#----------------------------------------------------------------------------
# Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PRECIOUS: $(foreach ext, $(IDL_EXT), Quoter$(ext))

client: $(addprefix $(VDIR),$(CLIENT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

server: $(addprefix $(VDIR),$(SERVER_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) $(foreach ext, $(IDL_EXT), Quoter$(ext))

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/QuoterC.o .obj/QuoterC.so .shobj/QuoterC.o .shobj/QuoterC.so: QuoterC.cpp QuoterC.h QuoterC.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(ACE_ROOT)/ace/pre.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/Pluggable.i \
 $(ACE_ROOT)/ace/post.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/Profile.h \
 $(TAO_ROOT)/tao/Tagged_Components.h \
 $(TAO_ROOT)/tao/CONV_FRAMEC.h \
 $(TAO_ROOT)/tao/CONV_FRAMEC.i \
 $(TAO_ROOT)/tao/Tagged_Components.i \
 $(TAO_ROOT)/tao/GIOP_Message_State.h \
 $(TAO_ROOT)/tao/GIOP_Message_State.i \
 $(TAO_ROOT)/tao/Profile.i \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/Policy_Manager.h \
 $(TAO_ROOT)/tao/Policy_Manager.i \
 $(TAO_ROOT)/tao/Resource_Factory.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
 $(TAO_ROOT)/tao/Protocol_Factory.h \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
 $(TAO_ROOT)/tao/TAO_Singleton_Manager.inl \
 $(TAO_ROOT)/tao/TAO_Singleton.h \
 $(TAO_ROOT)/tao/TAO_Singleton.inl \
 $(TAO_ROOT)/tao/TAO_Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(TAO_ROOT)/tao/Adapter.h \
 $(TAO_ROOT)/tao/Adapter.i \
 $(TAO_ROOT)/tao/PolicyFactory_Registry.h \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
 $(ACE_ROOT)/ace/Service_Config.h \
 $(ACE_ROOT)/ace/Service_Config.i \
 $(ACE_ROOT)/ace/Reactor.h \
 $(ACE_ROOT)/ace/Handle_Set.h \
 $(ACE_ROOT)/ace/Handle_Set.i \
 $(ACE_ROOT)/ace/Timer_Queue.h \
 $(ACE_ROOT)/ace/Timer_Queue_T.h \
 $(ACE_ROOT)/ace/Test_and_Set.h \
 $(ACE_ROOT)/ace/Test_and_Set.i \
 $(ACE_ROOT)/ace/Test_and_Set.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(TAO_ROOT)/tao/Parser_Registry.h \
 $(TAO_ROOT)/tao/Parser_Registry.i \
 $(TAO_ROOT)/tao/Service_Callbacks.h \
 $(TAO_ROOT)/tao/Service_Callbacks.i \
 $(TAO_ROOT)/tao/Fault_Tolerance_Service.h \
 $(TAO_ROOT)/tao/Fault_Tolerance_Service.i \
 $(TAO_ROOT)/tao/Transport_Cache_Manager.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(TAO_ROOT)/tao/Cache_Entries.h \
 $(TAO_ROOT)/tao/Transport_Descriptor_Interface.h \
 $(TAO_ROOT)/tao/Endpoint.h \
 $(TAO_ROOT)/tao/Endpoint.i \
 $(TAO_ROOT)/tao/Transport_Descriptor_Interface.inl \
 $(TAO_ROOT)/tao/Cache_Entries.inl \
 $(TAO_ROOT)/tao/Transport_Cache_Manager.inl \
 $(TAO_ROOT)/tao/Cleanup_Func_Registry.h \
 $(TAO_ROOT)/tao/Cleanup_Func_Registry.inl \
 $(TAO_ROOT)/tao/Object_Ref_Table.h \
 $(TAO_ROOT)/tao/RT_Policy_i.h \
 $(TAO_ROOT)/tao/RTCORBAC.h \
 $(TAO_ROOT)/tao/RTCORBAC.i \
 $(TAO_ROOT)/tao/RT_Policy_i.i \
 $(TAO_ROOT)/tao/Protocols_Hooks.h \
 $(TAO_ROOT)/tao/BiDir_Adapter.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/Invocation.h \
 $(TAO_ROOT)/tao/Synch_Reply_Dispatcher.h \
 $(TAO_ROOT)/tao/Reply_Dispatcher.h \
 $(TAO_ROOT)/tao/Reply_Dispatcher.i \
 $(TAO_ROOT)/tao/TAOC.h \
 $(TAO_ROOT)/tao/Priority_Mapping_Manager.h \
 $(TAO_ROOT)/tao/Priority_Mapping.h \
 $(TAO_ROOT)/tao/Priority_Mapping.i \
 $(TAO_ROOT)/tao/Priority_Mapping_Manager.i \
 $(TAO_ROOT)/tao/TAOC.i \
 $(TAO_ROOT)/tao/operation_details.h \
 $(TAO_ROOT)/tao/target_specification.h \
 $(TAO_ROOT)/tao/target_specification.i \
 $(TAO_ROOT)/tao/operation_details.i \
 $(TAO_ROOT)/tao/Invocation_Endpoint_Selectors.h \
 $(TAO_ROOT)/tao/Invocation_Endpoint_Selectors.i \
 $(TAO_ROOT)/tao/Invocation.i

.obj/QuoterS.o .obj/QuoterS.so .shobj/QuoterS.o .shobj/QuoterS.so: QuoterS.cpp QuoterS.h QuoterC.h QuoterC.i \
 $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
 $(ACE_ROOT)/ace/pre.h \
 $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
 $(ACE_ROOT)/ace/post.h \
 $(TAO_ROOT)/tao/RTCORBAC.h \
 $(TAO_ROOT)/tao/RTCORBAC.i \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.i \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
 $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
 $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.inl \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i \
 $(TAO_ROOT)/tao/PortableServer/Object_Adapter.h \
 $(TAO_ROOT)/tao/PortableServer/Key_Adapters.h \
 $(ACE_ROOT)/ace/Map.h \
 $(ACE_ROOT)/ace/Map_T.h \
 $(ACE_ROOT)/ace/Pair.h \
 $(ACE_ROOT)/ace/Pair_T.h \
 $(ACE_ROOT)/ace/Pair_T.i \
 $(ACE_ROOT)/ace/Pair_T.cpp \
 $(ACE_ROOT)/ace/Map_Manager.h \
 $(ACE_ROOT)/ace/Map_Manager.i \
 $(ACE_ROOT)/ace/Map_Manager.cpp \
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
 $(ACE_ROOT)/ace/Test_and_Set.h \
 $(ACE_ROOT)/ace/Test_and_Set.i \
 $(ACE_ROOT)/ace/Test_and_Set.cpp \
 $(ACE_ROOT)/ace/Timer_Queue_T.i \
 $(ACE_ROOT)/ace/Timer_Queue_T.cpp \
 $(ACE_ROOT)/ace/Reactor.i \
 $(ACE_ROOT)/ace/Reactor_Impl.h \
 $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager.h \
 $(ACE_ROOT)/ace/Functor.h \
 $(ACE_ROOT)/ace/Functor.i \
 $(ACE_ROOT)/ace/Functor_T.h \
 $(ACE_ROOT)/ace/Functor_T.i \
 $(ACE_ROOT)/ace/Functor_T.cpp \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Hash_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Active_Map_Manager.h \
 $(ACE_ROOT)/ace/Active_Map_Manager.i \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.h \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.i \
 $(ACE_ROOT)/ace/Active_Map_Manager_T.cpp \
 $(ACE_ROOT)/ace/Map_T.i \
 $(ACE_ROOT)/ace/Map_T.cpp \
 $(TAO_ROOT)/tao/PortableServer/Key_Adapters.i \
 $(TAO_ROOT)/tao/PortableServer/poa_macros.h \
 $(TAO_ROOT)/tao/PortableServer/Active_Object_Map.h \
 $(TAO_ROOT)/tao/Server_Strategy_Factory.h \
 $(TAO_ROOT)/tao/PortableServer/Active_Object_Map.i \
 $(TAO_ROOT)/tao/Adapter.h \
 $(TAO_ROOT)/tao/Adapter.i \
 $(TAO_ROOT)/tao/PortableServer/Object_Adapter.i \
 $(TAO_ROOT)/tao/PortableServer/Operation_Table.h \
 $(TAO_ROOT)/tao/TAO_Singleton.h \
 $(TAO_ROOT)/tao/TAO_Singleton.inl \
 $(TAO_ROOT)/tao/TAO_Singleton.cpp \
 $(ACE_ROOT)/ace/Object_Manager.h \
 $(ACE_ROOT)/ace/Object_Manager.i \
 $(ACE_ROOT)/ace/Managed_Object.h \
 $(ACE_ROOT)/ace/Managed_Object.i \
 $(ACE_ROOT)/ace/Managed_Object.cpp \
 $(TAO_ROOT)/tao/TAO_Singleton_Manager.h \
 $(TAO_ROOT)/tao/TAO_Singleton_Manager.inl \
 $(TAO_ROOT)/tao/TAO_Server_Request.h \
 $(TAO_ROOT)/tao/Tagged_Profile.h \
 $(TAO_ROOT)/tao/GIOPC.h \
 $(TAO_ROOT)/tao/GIOPC.i \
 $(TAO_ROOT)/tao/Tagged_Profile.i \
 $(TAO_ROOT)/tao/TAO_Server_Request.i \
 $(TAO_ROOT)/tao/ORB_Core.h \
 $(TAO_ROOT)/tao/Policy_Manager.h \
 $(TAO_ROOT)/tao/Policy_Manager.i \
 $(TAO_ROOT)/tao/Resource_Factory.h \
 $(TAO_ROOT)/tao/Pluggable.h \
 $(TAO_ROOT)/tao/Pluggable.i \
 $(TAO_ROOT)/tao/Protocol_Factory.h \
 $(TAO_ROOT)/tao/params.h \
 $(TAO_ROOT)/tao/params.i \
 $(TAO_ROOT)/tao/PolicyFactory_Registry.h \
 $(TAO_ROOT)/tao/Parser_Registry.h \
 $(TAO_ROOT)/tao/Parser_Registry.i \
 $(TAO_ROOT)/tao/Service_Callbacks.h \
 $(TAO_ROOT)/tao/Service_Callbacks.i \
 $(TAO_ROOT)/tao/Fault_Tolerance_Service.h \
 $(TAO_ROOT)/tao/Fault_Tolerance_Service.i \
 $(TAO_ROOT)/tao/Transport_Cache_Manager.h \
 $(TAO_ROOT)/tao/Cache_Entries.h \
 $(TAO_ROOT)/tao/Transport_Descriptor_Interface.h \
 $(TAO_ROOT)/tao/Endpoint.h \
 $(TAO_ROOT)/tao/Endpoint.i \
 $(TAO_ROOT)/tao/Transport_Descriptor_Interface.inl \
 $(TAO_ROOT)/tao/Cache_Entries.inl \
 $(TAO_ROOT)/tao/Transport_Cache_Manager.inl \
 $(TAO_ROOT)/tao/Cleanup_Func_Registry.h \
 $(TAO_ROOT)/tao/Cleanup_Func_Registry.inl \
 $(TAO_ROOT)/tao/Object_Ref_Table.h \
 $(TAO_ROOT)/tao/RT_Policy_i.h \
 $(TAO_ROOT)/tao/RT_Policy_i.i \
 $(TAO_ROOT)/tao/Protocols_Hooks.h \
 $(TAO_ROOT)/tao/BiDir_Adapter.h \
 $(ACE_ROOT)/ace/Thread_Manager.h \
 $(ACE_ROOT)/ace/Containers.h \
 $(ACE_ROOT)/ace/Containers.i \
 $(ACE_ROOT)/ace/Containers_T.h \
 $(ACE_ROOT)/ace/Containers_T.i \
 $(ACE_ROOT)/ace/Containers_T.cpp \
 $(ACE_ROOT)/ace/Singleton.h \
 $(ACE_ROOT)/ace/Singleton.i \
 $(ACE_ROOT)/ace/Singleton.cpp \
 $(ACE_ROOT)/ace/Thread_Manager.i \
 $(TAO_ROOT)/tao/ORB_Core.i \
 $(TAO_ROOT)/tao/Stub.h \
 $(TAO_ROOT)/tao/MProfile.h \
 $(TAO_ROOT)/tao/Profile.h \
 $(TAO_ROOT)/tao/Tagged_Components.h \
 $(TAO_ROOT)/tao/CONV_FRAMEC.h \
 $(TAO_ROOT)/tao/CONV_FRAMEC.i \
 $(TAO_ROOT)/tao/Tagged_Components.i \
 $(TAO_ROOT)/tao/GIOP_Message_State.h \
 $(TAO_ROOT)/tao/GIOP_Message_State.i \
 $(TAO_ROOT)/tao/Profile.i \
 $(TAO_ROOT)/tao/MProfile.i \
 $(TAO_ROOT)/tao/Stub.i \
 $(TAO_ROOT)/tao/IFR_Client_Adapter.h \
 $(ACE_ROOT)/ace/Dynamic_Service.h \
 $(ACE_ROOT)/ace/Dynamic_Service_Base.h \
 $(ACE_ROOT)/ace/Dynamic_Service.i \
 $(ACE_ROOT)/ace/Dynamic_Service.cpp

.obj/client.o .obj/client.so .shobj/client.o .shobj/client.so: client.cpp Stock_Consumer.h

.obj/server.o .obj/server.so .shobj/server.o .shobj/server.so: server.cpp Stock_Factory_i.h QuoterS.h QuoterC.h QuoterC.i \
 $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
 $(ACE_ROOT)/ace/pre.h \
 $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
 $(ACE_ROOT)/ace/post.h \
 $(TAO_ROOT)/tao/RTCORBAC.h \
 $(TAO_ROOT)/tao/RTCORBAC.i \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.i \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
 $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
 $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.inl \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/Stock_Consumer.o .obj/Stock_Consumer.so .shobj/Stock_Consumer.o .shobj/Stock_Consumer.so: Stock_Consumer.cpp Stock_Consumer.h QuoterC.h \
 QuoterC.i

.obj/Stock_i.o .obj/Stock_i.so .shobj/Stock_i.o .shobj/Stock_i.so: Stock_i.cpp Stock_i.h QuoterS.h QuoterC.h QuoterC.i \
 $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
 $(ACE_ROOT)/ace/pre.h \
 $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
 $(ACE_ROOT)/ace/post.h \
 $(TAO_ROOT)/tao/RTCORBAC.h \
 $(TAO_ROOT)/tao/RTCORBAC.i \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.i \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
 $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
 $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.inl \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i

.obj/Stock_Factory_i.o .obj/Stock_Factory_i.so .shobj/Stock_Factory_i.o .shobj/Stock_Factory_i.so: Stock_Factory_i.cpp Stock_Factory_i.h QuoterS.h \
 QuoterC.h QuoterC.i \
 $(TAO_ROOT)/tao/PortableServer/PortableServer.h \
 $(ACE_ROOT)/ace/pre.h \
 $(TAO_ROOT)/tao/PortableServer/portableserver_export.h \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.h \
 $(TAO_ROOT)/tao/PortableServer/PortableServerC.i \
 $(ACE_ROOT)/ace/post.h \
 $(TAO_ROOT)/tao/RTCORBAC.h \
 $(TAO_ROOT)/tao/RTCORBAC.i \
 $(TAO_ROOT)/tao/PortableServer/RTPortableServerC.i \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.h \
 $(TAO_ROOT)/tao/Abstract_Servant_Base.h \
 $(TAO_ROOT)/tao/PortableServer/Servant_Base.i \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.h \
 $(TAO_ROOT)/tao/PortableServer/Collocated_Object.i \
 $(TAO_ROOT)/tao/PortableServer/ThruPOA_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/Direct_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.h \
 $(TAO_ROOT)/tao/PortableServer/ServerRequestInfo.inl \
 QuoterS_T.h QuoterS_T.i QuoterS_T.cpp QuoterS.i Stock_i.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
