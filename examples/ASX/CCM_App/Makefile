#----------------------------------------------------------------------------
#
# $Id$
#
#	Makefile for CCM tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

LIBS   += -lACEXML_Parser -lACEXML

ACE_SHLIBS += $(ACELIB)

BIN     = SC_Client \
	  SC_Server

FILES	= CCM_App
SHLIB	= libCCM_App.$(SOEXT)

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

BUILD   = $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU

ifdef shared_libs
  ifndef VXWORKS
    #### Build libCCM_App library first,
    #### so that they don't see the template instantiations from all
    #### of the tests.
    BUILD := \
      libCCM_App.$(SOEXT) \
      $(BUILD)
  endif # ! VXWORKS
endif # shared_libs

include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
libCCM_App.$(SOEXT): CCM_App.cpp
	@$(MAKE) -f Makefile.CCM_App

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.


.obj/CCM_App.o .obj/CCM_App.so .shobj/CCM_App.o .shobj/CCM_App.so: CCM_App.cpp $(ACE_ROOT)/ace/Stream.h \
  $(ACE_ROOT)/ace/pre.h \
  $(ACE_ROOT)/ace/post.h \
  $(ACE_ROOT)/ace/ace_wchar.h \
  $(ACE_ROOT)/ace/ace_wchar.inl \
  $(ACE_ROOT)/ace/IO_Cntl_Msg.h \
  $(ACE_ROOT)/ace/ACE_export.h \
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
  $(ACE_ROOT)/ace/Message_Block.h \
  $(ACE_ROOT)/ace/Message_Block.i \
  $(ACE_ROOT)/ace/Message_Block_T.h \
  $(ACE_ROOT)/ace/Message_Block_T.i \
  $(ACE_ROOT)/ace/Message_Block_T.cpp \
  $(ACE_ROOT)/ace/Malloc_Base.h \
  $(ACE_ROOT)/ace/Module.h \
  $(ACE_ROOT)/ace/Task_T.h \
  $(ACE_ROOT)/ace/Message_Queue.h \
  $(ACE_ROOT)/ace/Synch.h $(ACE_ROOT)/ace/Synch.i \
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
  $(ACE_ROOT)/ace/Message_Queue_T.h \
  $(ACE_ROOT)/ace/Message_Queue_T.i \
  $(ACE_ROOT)/ace/Message_Queue_T.cpp \
  $(ACE_ROOT)/ace/Notification_Strategy.h \
  $(ACE_ROOT)/ace/Event_Handler.h \
  $(ACE_ROOT)/ace/Event_Handler.i \
  $(ACE_ROOT)/ace/Notification_Strategy.inl \
  $(ACE_ROOT)/ace/Message_Queue.i \
  $(ACE_ROOT)/ace/Task.h \
  $(ACE_ROOT)/ace/Service_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.h \
  $(ACE_ROOT)/ace/Shared_Object.i \
  $(ACE_ROOT)/ace/Svc_Conf_Tokens.h \
  $(ACE_ROOT)/ace/DLL.h \
  $(ACE_ROOT)/ace/Service_Object.i \
  $(ACE_ROOT)/ace/Thread_Manager.h \
  $(ACE_ROOT)/ace/Thread_Exit.h \
  $(ACE_ROOT)/ace/Thread_Control.h \
  $(ACE_ROOT)/ace/Thread_Control.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.h \
  $(ACE_ROOT)/ace/Node.h $(ACE_ROOT)/ace/Node.cpp \
  $(ACE_ROOT)/ace/Unbounded_Queue.inl \
  $(ACE_ROOT)/ace/Unbounded_Queue.cpp \
  $(ACE_ROOT)/ace/Containers.h \
  $(ACE_ROOT)/ace/Containers.i \
  $(ACE_ROOT)/ace/Containers_T.h \
  $(ACE_ROOT)/ace/Array_Base.h \
  $(ACE_ROOT)/ace/Array_Base.inl \
  $(ACE_ROOT)/ace/Array_Base.cpp \
  $(ACE_ROOT)/ace/Unbounded_Set.h \
  $(ACE_ROOT)/ace/Unbounded_Set.inl \
  $(ACE_ROOT)/ace/Unbounded_Set.cpp \
  $(ACE_ROOT)/ace/Containers_T.i \
  $(ACE_ROOT)/ace/Containers_T.cpp \
  $(ACE_ROOT)/ace/Free_List.h \
  $(ACE_ROOT)/ace/Free_List.i \
  $(ACE_ROOT)/ace/Free_List.cpp \
  $(ACE_ROOT)/ace/Singleton.h \
  $(ACE_ROOT)/ace/Singleton.i \
  $(ACE_ROOT)/ace/Singleton.cpp \
  $(ACE_ROOT)/ace/Object_Manager.h \
  $(ACE_ROOT)/ace/Object_Manager.i \
  $(ACE_ROOT)/ace/Managed_Object.h \
  $(ACE_ROOT)/ace/Managed_Object.i \
  $(ACE_ROOT)/ace/Managed_Object.cpp \
  $(ACE_ROOT)/ace/Framework_Component.h \
  $(ACE_ROOT)/ace/Framework_Component.inl \
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
  $(ACE_ROOT)/ace/Framework_Component_T.h \
  $(ACE_ROOT)/ace/Framework_Component_T.inl \
  $(ACE_ROOT)/ace/Framework_Component_T.cpp \
  $(ACE_ROOT)/ace/Thread_Manager.i \
  $(ACE_ROOT)/ace/Task.i $(ACE_ROOT)/ace/Task_T.i \
  $(ACE_ROOT)/ace/Task_T.cpp \
  $(ACE_ROOT)/ace/Module.i \
  $(ACE_ROOT)/ace/Module.cpp \
  $(ACE_ROOT)/ace/Stream_Modules.h \
  $(ACE_ROOT)/ace/Stream_Modules.cpp \
  $(ACE_ROOT)/ace/Stream.i \
  $(ACE_ROOT)/ace/Stream.cpp \
  $(ACE_ROOT)/ace/svc_export.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
