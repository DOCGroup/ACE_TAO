#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for TAO
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIBNAME  = libcorba
LIB      = $(LIBNAME).a
SHLIB    = $(LIBNAME).so

# These are components that are only headers and must be installed
CORBA_HDRS = orb sequence stub orbconf objtable optable $(CORBA_SRCS)
CORBA_SRCS = any corbacom except nvlist object orbobj principa \
	request svrrqst boa typecode optable

# These are components that are only headers and must be installed
OTHERS_HDRS = align cdr giop iiopobj iioporb xdr roa connect params factories marshal debug
OTHERS_INLN = any boa cdr connect factories iiopobj iioporb object orbobj params roa svrrqst typecode marshal
# These are components that are either header & source or source-only
OTHERS_SRCS = \
	giop \
	iiopobj \
	iioporb \
	invoke \
	roa \
	params \
	factories \
	objtable \
	connect \
	xdr \
	cdr \
	debug \
	interp \
	marshal \
	encode \
	decode \
	deep_copy \
	deep_free \
	tc_const

FILES	 = $(OTHERS_SRCS) $(CORBA_SRCS)

COMPATDIR = ./compat
COMPATFILES = objbase.h initguid.h

BIN = # t-xdr t-sizes

TEMPLATE_FILES = 

DEFS  	= $(addsuffix .h,$(OTHERS_HDRS))
LSRC	= $(addsuffix .cpp,$(FILES))
LSRC2	= $(addsuffix .cpp,$(TEMPLATE_FILES))

LDLIBS = -lACE
LIBS = -lACE
VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VSHLIB) $(VBIN)


#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(WRAPPER_ROOT)/include/makeinclude/wrapper_macros.GNU
#VSHDIR = obj/
#VSHOBJS = $(LSRC:%.cpp=$(VSHDIR)/$(notdir %).so)
include	$(WRAPPER_ROOT)/include/makeinclude/macros.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.common.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.lib.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.bin.GNU
include	$(WRAPPER_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets (and local hacks)
#----------------------------------------------------------------------------

#RLDFLAGS := $(subst -L,-Wl\,-rpath ,$(LDFLAGS))
CPPFLAGS += -D__IIOP_BUILD -I$(COMPATDIR) -DDEBUG
LDFLAGS += $(RLDFLAGS)

INSINC = ../proto/include
INSLIB = ../proto/lib

# The first value is a nasty hack because $(VSHLIB) was not working
# for some reason :-(
INSTALL = $(INSLIB)/libcorba.so \
	$(addprefix $(INSINC)/,$(addsuffix .h,$(OTHERS_HDRS)) $(addsuffix .i,$(OTHERS_INLN)) $(addsuffix .h,$(basename $(COMPATFILES)) ) ) \
	$(addprefix $(INSINC)/corba/, $(addsuffix .h,$(CORBA_SRCS) $(CORBA_HDRS) debug)) \


#INST = install -c
INST = /pkg/gnu/bin/install -c
INSTALL_BY_COPY = $(INST) $^ $@

$(INSINC)/%:	%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)
$(INSINC)/%:	$(COMPATDIR)/%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)
$(INSLIB)/%:	%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)
$(INSINC)/corba/%:	%
	test -d $(@D) || mkdir -p $(@D)
	$(INSTALL_BY_COPY)

copy:	$(INSTALL)


# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/giop.o .shobj/giop.so: giop.cpp \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i orb.h \
 orbconf.h corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h orbobj.h factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i orbobj.i giop.h stub.h iiopobj.h iiopobj.i
.obj/iiopobj.o .shobj/iiopobj.so: iiopobj.cpp iiopobj.h iiopobj.i
.obj/iioporb.o .shobj/iioporb.so: iioporb.cpp iioporb.h orbobj.h factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i orbobj.i iioporb.i iiopobj.h iiopobj.i
.obj/invoke.o .shobj/invoke.so: invoke.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h giop.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 stub.h iiopobj.h iiopobj.i factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i
.obj/roa.o .shobj/roa.so: roa.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h roa.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.i boa.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 objtable.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.h params.i connect.h \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 connect.i roa.i giop.h stub.h iiopobj.h iiopobj.i factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 factories.i orbobj.h orbobj.i
.obj/params.o .shobj/params.so: params.cpp params.h boa.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i orb.h \
 orbconf.h corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h objtable.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 connect.i
.obj/factories.o .shobj/factories.so: factories.cpp factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h orb.h orbconf.h corbacom.h except.h sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i
.obj/objtable.o .shobj/objtable.so: objtable.cpp objtable.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i orb.h \
 orbconf.h corbacom.h except.h sequence.h principa.h typecode.h any.h \
 any.i typecode.i nvlist.h request.h object.h object.i svrrqst.h cdr.h \
 align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/connect.o .shobj/connect.so: connect.cpp connect.h \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 connect.i roa.h boa.h orb.h orbconf.h corbacom.h except.h sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.h params.i roa.i
.obj/xdr.o .shobj/xdr.so: xdr.cpp xdr.h
.obj/cdr.o .shobj/cdr.so: cdr.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/debug.o .shobj/debug.so: debug.cpp
.obj/interp.o .shobj/interp.so: interp.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/marshal.o .shobj/marshal.so: marshal.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/encode.o .shobj/encode.so: encode.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h giop.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 stub.h iiopobj.h iiopobj.i factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i
.obj/decode.o .shobj/decode.so: decode.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h giop.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 stub.h iiopobj.h iiopobj.i factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i
.obj/deep_copy.o .shobj/deep_copy.so: deep_copy.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h giop.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 stub.h iiopobj.h iiopobj.i factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i
.obj/deep_free.o .shobj/deep_free.so: deep_free.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h giop.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 stub.h iiopobj.h iiopobj.i factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i
.obj/tc_const.o .shobj/tc_const.so: tc_const.cpp
.obj/any.o .shobj/any.so: any.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/corbacom.o .shobj/corbacom.so: corbacom.cpp
.obj/except.o .shobj/except.so: except.cpp \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i
.obj/nvlist.o .shobj/nvlist.so: nvlist.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/object.o .shobj/object.so: object.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h optable.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i stub.h
.obj/orbobj.o .shobj/orbobj.so: orbobj.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h stub.h iioporb.h orbobj.h \
 factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h boa.h objtable.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i orbobj.i iioporb.i roa.h roa.i
.obj/principa.o .shobj/principa.so: principa.cpp
.obj/request.o .shobj/request.so: request.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h stub.h
.obj/svrrqst.o .shobj/svrrqst.so: svrrqst.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/boa.o .shobj/boa.so: boa.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h boa.h \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.h \
 $(WRAPPER_ROOT)/ace/SOCK_IO.h \
 $(WRAPPER_ROOT)/ace/SOCK.h \
 $(WRAPPER_ROOT)/ace/Addr.h \
 $(WRAPPER_ROOT)/ace/Addr.i \
 $(WRAPPER_ROOT)/ace/IPC_SAP.h \
 $(WRAPPER_ROOT)/ace/IPC_SAP.i \
 $(WRAPPER_ROOT)/ace/SOCK.i \
 $(WRAPPER_ROOT)/ace/SOCK_IO.i \
 $(WRAPPER_ROOT)/ace/INET_Addr.h \
 $(WRAPPER_ROOT)/ace/INET_Addr.i \
 $(WRAPPER_ROOT)/ace/SOCK_Stream.i \
 objtable.h \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i boa.i \
 iioporb.h orbobj.h factories.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.h \
 $(WRAPPER_ROOT)/ace/Time_Value.h \
 $(WRAPPER_ROOT)/ace/SOCK_Acceptor.i \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.h \
 $(WRAPPER_ROOT)/ace/SOCK_Connector.i \
 $(WRAPPER_ROOT)/ace/Strategies_T.h \
 $(WRAPPER_ROOT)/ace/Service_Config.h \
 $(WRAPPER_ROOT)/ace/Service_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.h \
 $(WRAPPER_ROOT)/ace/Shared_Object.i \
 $(WRAPPER_ROOT)/ace/Service_Object.i \
 $(WRAPPER_ROOT)/ace/Thread_Manager.h \
 $(WRAPPER_ROOT)/ace/Thread_Manager.i \
 $(WRAPPER_ROOT)/ace/Signal.h \
 $(WRAPPER_ROOT)/ace/Containers.h \
 $(WRAPPER_ROOT)/ace/Containers.i \
 $(WRAPPER_ROOT)/ace/Signal.i \
 $(WRAPPER_ROOT)/ace/Service_Config.i \
 $(WRAPPER_ROOT)/ace/Reactor.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.h \
 $(WRAPPER_ROOT)/ace/Handle_Set.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue.h \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.h \
 $(WRAPPER_ROOT)/ace/Free_List.h \
 $(WRAPPER_ROOT)/ace/Free_List.i \
 $(WRAPPER_ROOT)/ace/Timer_Queue_T.i \
 $(WRAPPER_ROOT)/ace/Token.h \
 $(WRAPPER_ROOT)/ace/Token.i \
 $(WRAPPER_ROOT)/ace/Pipe.h \
 $(WRAPPER_ROOT)/ace/Pipe.i \
 $(WRAPPER_ROOT)/ace/Reactor.i \
 $(WRAPPER_ROOT)/ace/Proactor.h \
 $(WRAPPER_ROOT)/ace/Asynch_IO.h \
 $(WRAPPER_ROOT)/ace/Timer_List.h \
 $(WRAPPER_ROOT)/ace/Timer_List_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap.h \
 $(WRAPPER_ROOT)/ace/Timer_Heap_T.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel.h \
 $(WRAPPER_ROOT)/ace/Timer_Wheel_T.h \
 $(WRAPPER_ROOT)/ace/ReactorEx.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.h \
 $(WRAPPER_ROOT)/ace/Message_Block.h \
 $(WRAPPER_ROOT)/ace/Malloc.h \
 $(WRAPPER_ROOT)/ace/Malloc.i \
 $(WRAPPER_ROOT)/ace/Malloc_T.h \
 $(WRAPPER_ROOT)/ace/Malloc_T.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.h \
 $(WRAPPER_ROOT)/ace/Mem_Map.i \
 $(WRAPPER_ROOT)/ace/Memory_Pool.i \
 $(WRAPPER_ROOT)/ace/Message_Block.i \
 $(WRAPPER_ROOT)/ace/IO_Cntl_Msg.h \
 $(WRAPPER_ROOT)/ace/Strategies.h \
 $(WRAPPER_ROOT)/ace/Message_Queue.i \
 $(WRAPPER_ROOT)/ace/ReactorEx.i \
 $(WRAPPER_ROOT)/ace/Svc_Conf_Tokens.h \
 $(WRAPPER_ROOT)/ace/Synch_Options.h \
 $(WRAPPER_ROOT)/ace/Connector.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.h \
 $(WRAPPER_ROOT)/ace/Map_Manager.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.h \
 $(WRAPPER_ROOT)/ace/Task.h \
 $(WRAPPER_ROOT)/ace/Task.i \
 $(WRAPPER_ROOT)/ace/Task_T.h \
 $(WRAPPER_ROOT)/ace/Task_T.i \
 $(WRAPPER_ROOT)/ace/Svc_Handler.i \
 $(WRAPPER_ROOT)/ace/Connector.i \
 params.h params.i connect.h \
 $(WRAPPER_ROOT)/ace/Acceptor.h \
 $(WRAPPER_ROOT)/ace/Acceptor.i \
 connect.i factories.i orbobj.i iioporb.i roa.h roa.i
.obj/typecode.o .shobj/typecode.so: typecode.cpp orb.h orbconf.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 corbacom.h except.h \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i sequence.h \
 principa.h typecode.h any.h any.i typecode.i nvlist.h request.h \
 object.h object.i svrrqst.h cdr.h align.h marshal.h \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i \
 marshal.i cdr.i svrrqst.i debug.h
.obj/optable.o .shobj/optable.so: optable.cpp optable.h \
 $(WRAPPER_ROOT)/ace/ACE.h \
 $(WRAPPER_ROOT)/ace/OS.h \
 $(WRAPPER_ROOT)/ace/config.h \
 $(WRAPPER_ROOT)/ace/stdcpp.h \
 $(WRAPPER_ROOT)/ace/OS.i \
 $(WRAPPER_ROOT)/ace/Trace.h \
 $(WRAPPER_ROOT)/ace/Log_Msg.h \
 $(WRAPPER_ROOT)/ace/Log_Record.h \
 $(WRAPPER_ROOT)/ace/Log_Priority.h \
 $(WRAPPER_ROOT)/ace/Log_Record.i \
 $(WRAPPER_ROOT)/ace/ACE.i \
 $(WRAPPER_ROOT)/ace/Synch.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.h \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Simple.i \
 $(WRAPPER_ROOT)/ace/SV_Semaphore_Complex.i \
 $(WRAPPER_ROOT)/ace/Synch.i \
 $(WRAPPER_ROOT)/ace/Synch_T.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.h \
 $(WRAPPER_ROOT)/ace/Event_Handler.i \
 $(WRAPPER_ROOT)/ace/Synch_T.i \
 $(WRAPPER_ROOT)/ace/Thread.h \
 $(WRAPPER_ROOT)/ace/Thread.i \
 $(WRAPPER_ROOT)/ace/Hash_Map_Manager.h \
 $(WRAPPER_ROOT)/ace/SString.h \
 $(WRAPPER_ROOT)/ace/SString.i \
 $(WRAPPER_ROOT)/ace/Singleton.h \
 $(WRAPPER_ROOT)/ace/Singleton.i orb.h \
 orbconf.h corbacom.h except.h sequence.h principa.h typecode.h any.h \
 any.i typecode.i nvlist.h request.h object.h object.i svrrqst.h cdr.h \
 align.h marshal.h marshal.i cdr.i svrrqst.i debug.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
