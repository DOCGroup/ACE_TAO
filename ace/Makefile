#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the entire ACE release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBACE   = libACE
LIB      = $(LIBACE).a
SHLIB    = $(LIBACE).$(SOEXT)

OS_FILES = \
  Basic_Types \
  OS \
  OS_Dirent \
  OS_Memory \
  OS_QoS \
  OS_String \
  OS_TLI \
  OS_Errno \
  Base_Thread_Adapter \
  OS_Thread_Adapter \
  OS_Log_Msg_Attributes \
  Thread_Hook \
  Sched_Params \
  Handle_Set \
  Copy_Disabled
UTILS_FILES = \
  ACE \
  Active_Map_Manager \
  Arg_Shifter \
  ARGV \
  Capabilities \
  Containers \
  Configuration \
  Configuration_Import_Export \
  Dirent \
  Dirent_Selector \
  Dynamic \
  Flag_Manip \
  Framework_Component \
  Functor \
  Get_Opt \
  Hash_Map_Manager \
  Connection_Recycling_Strategy \
  Hashable \
  Notification_Strategy \
  Recyclable \
  Refcountable \
  Handle_Ops \
  Init_ACE \
  Lib_Find \
  Message_Block \
  Method_Request \
  Object_Manager \
  Registry \
  String_Base_Const \
  SString \
  Stats \
  Sample_History
  #### NOTE:  see below for Filecache.
LOGGING_FILES = \
  Dump \
  Log_Msg \
  Log_Msg_Callback \
  Log_Msg_Backend \
  Log_Msg_IPC \
  Log_Msg_NT_Event_Log \
  Log_Msg_UNIX_Syslog \
  Log_Record \
  Logging_Strategy \
  Trace
THREADS_FILES = \
  Activation_Queue \
  Process \
  Process_Manager \
  Synch \
  Synch_Options \
  Process_Semaphore \
  Process_Mutex \
  RW_Process_Mutex \
  File_Lock \
  Thread \
  Thread_Manager \
  Thread_Adapter \
  Thread_Exit \
  Thread_Control \
  Token
DEMUX_FILES = \
  Event_Handler \
  FlReactor \
  Msg_WFMO_Reactor \
  POSIX_Proactor \
  WIN32_Proactor \
  Priority_Reactor \
  Proactor \
  Reactor \
  Select_Reactor \
  Select_Reactor_Base \
  SUN_Proactor \
  TP_Reactor \
  TkReactor \
  WFMO_Reactor \
  XtReactor \
  QtReactor
CONNECTION_FILES = \
  Asynch_IO \
  Asynch_IO_Impl \
  POSIX_Asynch_IO \
  WIN32_Asynch_IO
SOCKETS_FILES = \
  Addr \
  INET_Addr \
  IPC_SAP \
  LSOCK \
  LSOCK_Acceptor \
  LSOCK_CODgram \
  LSOCK_Connector \
  LSOCK_Dgram \
  LSOCK_Stream \
  SOCK \
  SOCK_Acceptor \
  SOCK_CODgram \
  Sock_Connect \
  SOCK_Connector \
  SOCK_Dgram \
  SOCK_Dgram_Bcast \
  SOCK_Dgram_Mcast \
  SOCK_IO \
  SOCK_Stream
IPC_FILES = \
  ATM_Addr \
  ATM_Acceptor \
  ATM_Connector \
  ATM_Params \
  ATM_QoS \
  ATM_Stream \
  XTI_ATM_Mcast \
  DEV \
  DEV_Addr \
  DEV_Connector \
  DEV_IO \
  FIFO \
  FIFO_Recv \
  FIFO_Recv_Msg \
  FIFO_Send \
  FIFO_Send_Msg \
  FILE_Addr \
  FILE \
  FILE_Connector \
  FILE_IO \
  IO_SAP \
  IOStream \
  Pipe \
  Signal \
  SPIPE_Addr \
  SPIPE \
  SPIPE_Acceptor \
  SPIPE_Connector \
  SPIPE_Stream \
  SV_Message \
  SV_Message_Queue \
  SV_Semaphore_Complex \
  SV_Semaphore_Simple \
  SV_Shared_Memory \
  TLI \
  TLI_Acceptor \
  TLI_Connector \
  TLI_Stream \
  TTY_IO \
  UNIX_Addr \
  UPIPE_Acceptor \
  UPIPE_Connector \
  UPIPE_Stream \
  MEM_Acceptor \
  MEM_Addr \
  MEM_Connector \
  MEM_IO \
  MEM_SAP \
  MEM_Stream
SVCCONF_FILES = \
  DLL \
  Dynamic_Service_Base \
  Parse_Node \
  Service_Config \
  Service_Manager \
  Service_Object \
  Service_Repository \
  Service_Types \
  Service_Templates \
  Shared_Object \
  Svc_Conf_l \
  Svc_Conf_y \
  Svc_Conf_Lexer_Guard
STREAMS_FILES = \
  CDR_Base \
  CDR_Stream \
  Codeset_IBM1047 \
  Message_Queue \
  Reactor_Notification_Strategy \
  Task
MEMORY_FILES = \
  Based_Pointer_Repository \
  Malloc \
  PI_Malloc \
  Malloc_Allocator \
  Malloc_Instantiations \
  Mem_Map \
  Memory_Pool \
  Obchunk \
  Obstack \
  Read_Buffer \
  Shared_Memory \
  Shared_Memory_MM \
  Shared_Memory_SV
TIMER_FILES = \
  Basic_Stats \
  High_Res_Timer \
  Profile_Timer \
  System_Time \
  Time_Request_Reply \
  Timeprobe \
  Timer_Hash \
  Timer_Heap \
  Timer_List \
  Timer_Queue \
  Timer_Wheel
TOKEN_FILES = \
  Local_Tokens \
  Remote_Tokens \
  Token_Collection \
  Token_Invariants \
  Token_Manager \
  Token_Request_Reply
OTHER_FILES = \
  Local_Name_Space \
  Name_Proxy \
  Name_Request_Reply \
  Name_Space \
  Naming_Context \
  Registry_Name_Space \
  Remote_Name_Space \

TEMPLATE_FILES = \
           Acceptor \
           Active_Map_Manager_T \
	   Array_Base \
	   Node \
	   Unbounded_Set \
	   Unbounded_Queue \
           Asynch_Acceptor \
           Auto_IncDec_T \
           Auto_Ptr \
           Based_Pointer_T \
	   Bound_Ptr \
           Connector \
           Containers_T \
           Cache_Map_Manager_T \
           Cached_Connect_Strategy_T \
           Caching_Strategies_T \
           Caching_Utility_T \
           Cleanup_Strategies_T \
           Dump_T \
           Dynamic_Service \
           Env_Value_T \
           Event_Handler_T \
	   Framework_Component_T \
           Free_List \
           Functor_T \
           Future \
           Future_Set \
           Hash_Map_Manager_T \
	   Hash_Map_With_Allocator_T \
           Hash_Cache_Map_Manager_T \
           IOStream_T \
	   Intrusive_List \
	   Intrusive_List_Node \
           LOCK_SOCK_Acceptor \
           Local_Name_Space_T \
           Malloc_T \
           Managed_Object \
           Map_Manager \
           Map_T \
           Message_Block_T \
           Message_Queue_T \
           Module \
	   Obstack_T \
           Pair_T \
           Refcounted_Auto_Ptr \
           RB_Tree \
           Select_Reactor_T \
           Singleton \
           Strategies_T \
           Stream \
           Stream_Modules \
	   String_Base \
           Svc_Handler \
           Synch_T \
           Task_T \
           Template_Instantiations \
	   Test_and_Set \
           Timeprobe_T \
           Timer_Hash_T \
           Timer_Heap_T \
           Timer_List_T \
           Timer_Queue_Adapters \
           Timer_Queue_T \
           Timer_Wheel_T \
           Typed_SV_Message \
           Typed_SV_Message_Queue \
	   Vector_T

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

ifneq ($(GHS),)
  ifeq ($(CPU),86)
    #### With Green Hills for x86 target, compile gethrtime.cpp.
    FILES += gethrtime
    LSRC += gethrtime.cpp
  endif # 86
endif # GHS

ifeq ($(ssl),1)
  DIRS += SSL
endif # ssl

ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
  ifneq ($(rmcast),0)
    DIRS += RMCast
  endif # rmcast
endif # ACE_HAS_GNUG_PRE_2_8

## Only build the contents of the QoS directory under UNIX
## if we have the RAPI library installed
ifeq ($(rapi),1)
  DIRS += QoS
endif # rapi

####
#### Build customization.
####
ifndef ACE_COMPONENTS
  #### Please see docs/ACE-subsets.html for documentation.
  ACE_COMPONENTS = \
    OS \
    Utils \
    Logging \
    Threads \
    Demux \
    Connection \
    Sockets \
    IPC \
    Svcconf \
    Streams \
    Memory \
    Timer \
    Token \
    Other
endif # ACE_COMPONENTS

ifeq (FOR_TAO,$(ACE_COMPONENTS))
  #### These ACE components are necessary to support TAO.
  override ACE_COMPONENTS = \
    OS \
    Utils \
    Logging \
    Threads \
    Demux \
    Connection \
    Sockets \
    IPC \
    Svcconf \
    Streams \
    Memory \
    Timer
else  # ! FOR_TAO
  #### TAO doesn't need Filecache.  It costs 14 Kb.
  UTILS_FILES += Filecache
endif # ! FOR_TAO

####
#### ACE_COMPONENTS support.
####
ifneq (,$(findstring OS,$(ACE_COMPONENTS)))
  FILES += $(OS_FILES)
endif # OS

ifneq (,$(findstring Utils,$(ACE_COMPONENTS)))
  FILES += $(UTILS_FILES)
endif # Utils

ifneq (,$(findstring Logging,$(ACE_COMPONENTS)))
  FILES += $(LOGGING_FILES)
endif # Logging

ifneq (,$(findstring Threads,$(ACE_COMPONENTS)))
  FILES += $(THREADS_FILES)
endif # Threads

ifneq (,$(findstring Demux,$(ACE_COMPONENTS)))
  FILES += $(DEMUX_FILES)
endif # Demux

ifneq (,$(findstring Connection,$(ACE_COMPONENTS)))
  FILES += $(CONNECTION_FILES)
endif # Connection

ifneq (,$(findstring Sockets,$(ACE_COMPONENTS)))
  FILES += $(SOCKETS_FILES)
endif # Sockets

ifneq (,$(findstring IPC,$(ACE_COMPONENTS)))
  FILES += $(IPC_FILES)
endif # IPC

ifneq (,$(findstring Timer,$(ACE_COMPONENTS)))
  FILES += $(TIMER_FILES)
endif # ! Timer

ifneq (,$(findstring Svcconf,$(ACE_COMPONENTS)))
  FILES += $(SVCCONF_FILES)
else # ! Svcconf
  CCFLAGS += -DACE_LACKS_ACE_SVCCONF
endif # ! Svcconf

ifneq (,$(findstring Streams,$(ACE_COMPONENTS)))
  FILES += $(STREAMS_FILES)
endif # Streams

ifneq (,$(findstring Memory,$(ACE_COMPONENTS)))
  FILES += $(MEMORY_FILES)
endif # Memory

ifeq (,$(findstring Token,$(ACE_COMPONENTS)))
  CCFLAGS += -DACE_LACKS_ACE_TOKEN
else  # ! Token
  FILES += $(TOKEN_FILES)
endif # ! Token

ifeq (,$(findstring Other,$(ACE_COMPONENTS)))
  CCFLAGS += -DACE_LACKS_ACE_OTHER
else  # ! Other
  FILES += $(OTHER_FILES)
endif # ! Other

ifeq ($(AIX_TEMPLATE_HACK),1)
  FILES = 0_ACE_All_Src
  TEMPLATE_FILES = 0_ACE_All_Tmp
endif

#This is a hack. If the qt_reactor needs to be built we need to run
#the moc (Meta Object Compiler) on QtReactor.h that would generate
#QtReactor_moc.cpp. We need to compile this file and add it to the
#library.
ifneq ($(qt_reactor),)
  DEMUX_FILES += QtReactor_moc
endif #qt_recator

LSRC  = $(addsuffix .cpp,$(FILES))

BUILD += ACE_COMPONENTS

#### Setting ACELIB to null allows it to be used for building shared
#### libraries, including libACE.$(SOEXT,) on certain platforms.
ACELIB =

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#### Disable installs in this ($(ACE_ROOT)/ace) directory, because this
#### is the (default) destination of installs anyways.  This line prevents
#### creation of a link from the ACE library to itself when the build of
#### the ACE library fails.
INSTALL =
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PHONY: ACE_COMPONENTS
ACE_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --ace --set ' $(ACE_COMPONENTS) '

realclean:
	@sh $(ACE_ROOT)/bin/ace_components --ace --remove

# AIX template compilation speedup hack

ACE_All: 0_ACE_All_Src.cpp 0_ACE_All_Tmp.cpp

# Build ACE before descending into subdirectories.
all.nested: all.local

0_ACE_All_Src.h:
	@echo;
	echo  > $@ "// $@";
	echo >> $@ "// This file should speed up compilation for AIX. ";
	echo >> $@ "// It includes all non-template .h-files in one batch.";
	echo >> $@ "// The .h-files #include those .i-files that are needed.";
	echo >> $@;
	for name in $(FILES); do \
	    echo "#include \"$$name.h\""; \
	 done >> $@;
	echo >> $@;
	echo >> $@ "// EOF";

0_ACE_All_Src.cpp: 0_ACE_All_Src.h
	@echo;
	echo >  $@ "// $@";
	echo >> $@ "// This file should speed up compilation for AIX. ";
	echo >> $@ "// It includes all non-template .cpp-files in one batch.";
	echo >> $@ "// The .h-files #include those .i-files that are needed.";
	echo >> $@;
	echo >> $@ "#include \"$*.h\"";
	echo >> $@;
	for name in $(FILES); do \
	    echo "#include \"$$name.cpp\""; \
	  done >> $@;
	echo >> $@;
	echo >> $@ "// EOF";

0_ACE_All_Tmp.cpp:
	@echo;
	echo >  $@ "// $@";
	echo >> $@ "// This file should speed up compilation for AIX. ";
	echo >> $@ "// It includes all _template_ .h-files in one batch. ";
	echo >> $@ "// Each of these will include any needed template";
	echo >> $@ "// implementations from .cpp- and .i-files. ";
	echo >> $@;
	for name in $(TEMPLATE_FILES); do \
	    echo "#include \"$$name.cpp\""; \
	  done >> $@;
	echo >> $@;
	echo >> $@ "// EOF";

Svc_Conf_y.cpp: Svc_Conf.y
	$(YACC) -l -d Svc_Conf.y; \
	sed -e "s/char \*getenv/char *ace_foo/g" \
	    -e "s/= getenv/= ACE_OS::getenv/g" \
	    -e "s/fprintf/ACE_OS::fprintf/g" \
            -e "s/yy/ace_yy/g" \
            -e "s/->ace_yyerrno/->yyerrno/g" \
	    -e "s/YY/ACE_YY/g" \
            -e "s/Svc_Conf\.tab\.c/Svc_Conf_y.cpp/g" < Svc_Conf.tab.c > /tmp/$@; \
	cp /tmp/$@ $@; \
	patch < ../etc/Svc_Conf_y.cpp.diff; \
	echo '// $$'\I\d'$$' > Svc_Conf_Tokens.h; \
	cat Svc_Conf.tab.h >> Svc_Conf_Tokens.h; \
	$(RM) -f /tmp/$@ Svc_Conf.tab.c Svc_Conf.tab.h Svc_Conf_y.cpp.orig;

Svc_Conf_l.cpp: Svc_Conf.l
	$(LEX) -L -t -I Svc_Conf.l > $@; \
	sed -e "s/unistd/stdio/g" \
	    -e "s/yy/ace_yy/g" \
            -e "s/YY/ACE_YY/g" \
            -e "s/free( ptr );/free( ACE_MALLOC_T (ptr) );/g" \
            -e "s/exit( 1 );/ACE_OS::exit( 1 );/g" \
            -e "s/isatty( fileno(file)/ACE_OS::isatty( fileno (file)/g" \
            -e "s/int isatty/int nop_isatty/g" \
            -e "s/realloc( ptr, size );/realloc( ACE_MALLOC_T (ptr), size );/g" \
            -e "s@#include <stdio\.h>@#include \"ace/OS.h\"@" \
            -e "s@#include <@#include /**/ <@" \
            -e "s@ECHO@ACE_SVC_CONF_ECHO@" < $@ > /tmp/$@; \
	cp /tmp/$@ $@; \
	patch < ../etc/Svc_Conf_l.cpp.diff; \
	$(RM) -f /tmp/$@ Svc_Conf_l.cpp.orig;

# Some compilation for the QtReactor stuff. We need to run moc
# ie. (Meta Object compiler) for our QtReactor.h. So, let's do it
# here.

ifneq ($(qt_reactor),)
QtReactor_moc.cpp: QtReactor.h
	$(MOC) QtReactor.h >QtReactor_moc.cpp
endif #qt_reactor

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
	$(MAKE) $@ ghs=0
  endif # 86
endif # GHS

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
