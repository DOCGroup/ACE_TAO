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

LDLIBS = -lTAO_FTORB -lTAO_IORManip -lTAO_PortableServer -lTAO

PROG_SRCS = IOGRTest.cpp

SRC = $(PROG_SRCS)

SIMPLE_CLT_OBJS = IOGRTest.o
SIMPLE_SVR_OBJS =

BIN_UNCHECKED = IOGRTest

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU

TAO_ORBSVCS := $(shell sh $(ACE_ROOT)/bin/ace_components --orbsvcs)
ifeq (FTORB,$(findstring FTORB,$(TAO_ORBSVCS)))
  BIN = $(BIN_UNCHECKED)
endif # FaultTolerance

include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

LDFLAGS +=  -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace
CPPFLAGS += -I$(TAO_ROOT)/orbsvcs

.PRECIOUS:

IOGRTest: $(addprefix $(VDIR),$(SIMPLE_CLT_OBJS))
	$(LINK.cc) $(LDFLAGS) -o $@ $^ $(VLDLIBS) $(POSTLINK)

realclean: clean
	-$(RM) -rf IOGRTest

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/IOGRTest.o .obj/IOGRTest.so .shobj/IOGRTest.o .shobj/IOGRTest.so: IOGRTest.cpp \
 $(TAO_ROOT)/tao/corbafwd.h \
 $(ACE_ROOT)/ace/pre.h \
 $(ACE_ROOT)/ace/CDR_Base.h \
 $(ACE_ROOT)/ace/post.h \
 $(ACE_ROOT)/ace/ACE_export.h \
 $(ACE_ROOT)/ace/svc_export.h \
 $(ACE_ROOT)/ace/ace_wchar.h \
 $(ACE_ROOT)/ace/ace_wchar.inl \
 $(ACE_ROOT)/ace/OS_Errno.h \
 $(ACE_ROOT)/ace/OS_Export.h \
 $(ACE_ROOT)/ace/OS_Errno.inl \
 $(ACE_ROOT)/ace/Basic_Types.h \
 $(ACE_ROOT)/ace/Basic_Types.i \
 $(ACE_ROOT)/ace/Message_Block.h \
 $(ACE_ROOT)/ace/ACE.h \
 $(ACE_ROOT)/ace/OS.h \
 $(ACE_ROOT)/ace/OS_Dirent.h \
 $(ACE_ROOT)/ace/OS_Dirent.inl \
 $(ACE_ROOT)/ace/OS_String.h \
 $(ACE_ROOT)/ace/OS_String.inl \
 $(ACE_ROOT)/ace/OS_Memory.h \
 $(ACE_ROOT)/ace/OS_Memory.inl \
 $(ACE_ROOT)/ace/OS_TLI.h \
 $(ACE_ROOT)/ace/OS_TLI.inl \
 $(ACE_ROOT)/ace/Default_Constants.h \
 $(ACE_ROOT)/ace/Global_Macros.h \
 $(ACE_ROOT)/ace/Min_Max.h \
 $(ACE_ROOT)/ace/streams.h \
 $(ACE_ROOT)/ace/Trace.h \
 $(ACE_ROOT)/ace/OS.i \
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
 $(ACE_ROOT)/ace/Malloc.h \
 $(ACE_ROOT)/ace/Log_Msg.h \
 $(ACE_ROOT)/ace/Log_Record.h \
 $(ACE_ROOT)/ace/Log_Priority.h \
 $(ACE_ROOT)/ace/Log_Record.i \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.h \
 $(ACE_ROOT)/ace/OS_Log_Msg_Attributes.inl \
 $(ACE_ROOT)/ace/Malloc.i \
 $(ACE_ROOT)/ace/Malloc_T.h \
 $(ACE_ROOT)/ace/Synch.h \
 $(ACE_ROOT)/ace/Synch.i \
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
 $(ACE_ROOT)/ace/Malloc_Base.h \
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
 $(ACE_ROOT)/ace/Node.h \
 $(ACE_ROOT)/ace/Node.cpp \
 $(ACE_ROOT)/ace/Unbounded_Set.inl \
 $(ACE_ROOT)/ace/Unbounded_Set.cpp \
 $(ACE_ROOT)/ace/Memory_Pool.i \
 $(ACE_ROOT)/ace/Message_Block.i \
 $(ACE_ROOT)/ace/Message_Block_T.h \
 $(ACE_ROOT)/ace/Message_Block_T.i \
 $(ACE_ROOT)/ace/Message_Block_T.cpp \
 $(ACE_ROOT)/ace/CDR_Base.inl \
 $(TAO_ROOT)/tao/try_macros.h \
 $(TAO_ROOT)/tao/orbconf.h \
 $(ACE_ROOT)/ace/CORBA_macros.h \
 $(TAO_ROOT)/tao/varbase.h \
 $(TAO_ROOT)/tao/TAO_Export.h \
 $(TAO_ROOT)/tao/corbafwd.i \
 $(TAO_ROOT)/tao/ORB.h \
 $(TAO_ROOT)/tao/Exception.h \
 $(ACE_ROOT)/ace/SString.h \
 $(ACE_ROOT)/ace/String_Base.h \
 $(ACE_ROOT)/ace/String_Base_Const.h \
 $(ACE_ROOT)/ace/String_Base.i \
 $(ACE_ROOT)/ace/String_Base.cpp \
 $(ACE_ROOT)/ace/Auto_Ptr.h \
 $(ACE_ROOT)/ace/Auto_Ptr.i \
 $(ACE_ROOT)/ace/Auto_Ptr.cpp \
 $(ACE_ROOT)/ace/SString.i \
 $(TAO_ROOT)/tao/Exception.i \
 $(TAO_ROOT)/tao/Services.h \
 $(TAO_ROOT)/tao/Sequence.h \
 $(TAO_ROOT)/tao/Managed_Types.h \
 $(TAO_ROOT)/tao/Managed_Types.i \
 $(TAO_ROOT)/tao/Sequence.i \
 $(TAO_ROOT)/tao/Sequence_T.h \
 $(TAO_ROOT)/tao/Sequence_T.i \
 $(TAO_ROOT)/tao/Sequence_T.cpp \
 $(TAO_ROOT)/tao/Environment.h \
 $(TAO_ROOT)/tao/Environment.i \
 $(TAO_ROOT)/tao/Services.i \
 $(TAO_ROOT)/tao/CORBA_String.h \
 $(TAO_ROOT)/tao/CORBA_String.inl \
 $(TAO_ROOT)/tao/PolicyC.h \
 $(TAO_ROOT)/tao/Encodable.h \
 $(TAO_ROOT)/tao/CDR.h \
 $(ACE_ROOT)/ace/CDR_Stream.h \
 $(ACE_ROOT)/ace/CDR_Stream.i \
 $(TAO_ROOT)/tao/CDR.i \
 $(TAO_ROOT)/tao/Remote_Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/Object_Proxy_Impl.h \
 $(TAO_ROOT)/tao/CurrentC.h \
 $(TAO_ROOT)/tao/Object.h \
 $(TAO_ROOT)/tao/Object_Proxy_Broker.h \
 $(TAO_ROOT)/tao/Object.i \
 $(TAO_ROOT)/tao/CurrentC.i \
 $(TAO_ROOT)/tao/PolicyC.i \
 $(TAO_ROOT)/tao/ORB.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/FT_CORBA_ORBC.h \
 $(TAO_ROOT)/tao/corba.h \
 $(TAO_ROOT)/tao/Typecode.h \
 $(TAO_ROOT)/tao/Typecode.i \
 $(TAO_ROOT)/tao/Any.h \
 $(TAO_ROOT)/tao/Any.i \
 $(TAO_ROOT)/tao/NVList.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.h \
 $(ACE_ROOT)/ace/Unbounded_Queue.inl \
 $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
 $(TAO_ROOT)/tao/NVList.i \
 $(TAO_ROOT)/tao/LocalObject.h \
 $(TAO_ROOT)/tao/LocalObject.i \
 $(TAO_ROOT)/tao/Principal.h \
 $(TAO_ROOT)/tao/OctetSeqC.h \
 $(TAO_ROOT)/tao/OctetSeqC.i \
 $(TAO_ROOT)/tao/Principal.i \
 $(TAO_ROOT)/tao/BoundsC.h \
 $(TAO_ROOT)/tao/BoundsC.i \
 $(TAO_ROOT)/tao/ValueBase.h \
 $(TAO_ROOT)/tao/ValueBase.i \
 $(TAO_ROOT)/tao/ValueFactory.h \
 $(TAO_ROOT)/tao/ValueFactory.i \
 $(TAO_ROOT)/tao/DomainC.h \
 $(TAO_ROOT)/tao/DomainC.i \
 $(TAO_ROOT)/tao/WrongTransactionC.h \
 $(TAO_ROOT)/tao/Object_KeyC.h \
 $(TAO_ROOT)/tao/Object_KeyC.i \
 $(TAO_ROOT)/tao/ObjectIDList.h \
 $(TAO_ROOT)/tao/ObjectIDList.i \
 $(TAO_ROOT)/tao/StringSeqC.h \
 $(TAO_ROOT)/tao/StringSeqC.i \
 $(TAO_ROOT)/tao/PortableInterceptorC.h \
 $(TAO_ROOT)/tao/DynamicC.h \
 $(TAO_ROOT)/tao/DynamicC.i \
 $(TAO_ROOT)/tao/MessagingC.h \
 $(TAO_ROOT)/tao/TimeBaseC.h \
 $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.h \
 $(TAO_ROOT)/tao/SmartProxies/smartproxies_export.h \
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
 $(TAO_ROOT)/tao/SmartProxies/Smart_Proxies.inl \
 $(TAO_ROOT)/tao/TimeBaseC.i \
 $(TAO_ROOT)/tao/IOPC.h \
 $(TAO_ROOT)/tao/IOPC.i \
 $(TAO_ROOT)/tao/PollableC.h \
 $(TAO_ROOT)/tao/PollableC.i \
 $(TAO_ROOT)/tao/MessagingC.i \
 $(TAO_ROOT)/tao/ObjectReferenceTemplateC.h \
 $(TAO_ROOT)/tao/ObjectReferenceTemplateC.i \
 $(TAO_ROOT)/tao/PortableInterceptorC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/FaultTolerance/fault_tol_export.h \
 $(TAO_ROOT)/tao/GIOPC.h \
 $(TAO_ROOT)/tao/GIOPC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/FT_CORBA_ORBC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/FaultTolerance/FT_Service_Activate.h \
 $(TAO_ROOT)/tao/Services_Activate.h \
 $(ACE_ROOT)/ace/Service_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.h \
 $(ACE_ROOT)/ace/Shared_Object.i \
 $(ACE_ROOT)/ace/Service_Object.i \
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
 $(TAO_ROOT)/orbsvcs/orbsvcs/FaultTolerance/FT_IOGR_Property.h \
 $(TAO_ROOT)/tao/IORManipulation/IORManip_Loader.h \
 $(TAO_ROOT)/tao/IORManipulation/ior_manip_export.h \
 $(TAO_ROOT)/tao/Object_Loader.h \
 $(TAO_ROOT)/tao/Object_Loader.i \
 $(TAO_ROOT)/tao/IORManipulation/IORC.h \
 $(TAO_ROOT)/tao/IORManipulation/IORC.i \
 $(TAO_ROOT)/orbsvcs/orbsvcs/FaultTolerance/FT_IOGR_Property.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
