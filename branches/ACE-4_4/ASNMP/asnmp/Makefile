#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the SNMP+ACE Release
#----------------------------------------------------------------------------

MAKEFILE = Makefile
LIB      = libasnmp.a
SHLIB	 = libasnmp.$(SOEXT)

FILES	 = address \
	asn1 \
	counter \
	ctr64 \
	gauge \
	integer \
	octet \
	oid \
	pdu \
	snmp \
	target \
	timetick \
	transaction \
	vb \
        wpdu \
        sagent

TEMPLATE_FILES = 

DEFS    = $(addsuffix .h,$(FILES)) 
DEFS    += $(addsuffix .i,$(FILES))
LSRC    = $(addsuffix .cpp,$(FILES))
#LSRC2   = $(addsuffix .cpp,$(TEMPLATE_FILES))

#### Build the SNMP++ shared library _before_ the static library.
#### This works around an apparent problem with template
#### instantiation with Sun C++ 4.1 and 4.2 (the symptom is link
#### error due to ACE_TimeValue::normalize () not being found).
BUILD	= $(VSHLIB) $(SHLIBA) $(VLIB)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU
 
#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

CCFLAGS += -I$(ACE_ROOT)/ASNMP/

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

address.o: address.cpp /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h
asn1.o: asn1.cpp /home/cs/guest/mrm/ASNMP/asnmp/asn1.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/snmp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Atomic_Op.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Time_Value.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.i \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/vb.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmperrs.h \
 /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /home/cs/guest/mrm/ASNMP/asnmp/pdu.h
counter.o: counter.cpp /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h
ctr64.o: ctr64.cpp /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i
gauge.o: gauge.cpp /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i
integer.o: integer.cpp /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i
octet.o: octet.cpp \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h
oid.o: oid.cpp /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h
pdu.o: pdu.cpp /home/cs/guest/mrm/ASNMP/asnmp/snmp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Atomic_Op.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Time_Value.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.i \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/vb.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmperrs.h \
 /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /home/cs/guest/mrm/ASNMP/asnmp/pdu.h
snmp.o: snmp.cpp /home/cs/guest/mrm/ASNMP/asnmp/snmp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Atomic_Op.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Time_Value.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.i \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/vb.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmperrs.h \
 /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /home/cs/guest/mrm/ASNMP/asnmp/pdu.h \
 /home/cs/guest/mrm/ASNMP/asnmp/enttraps.h \
 /home/cs/guest/mrm/ASNMP/asnmp/asn1.h \
 /home/cs/guest/mrm/ASNMP/asnmp/wpdu.h \
 /home/cs/guest/mrm/ASNMP/asnmp/transaction.h
target.o: target.cpp /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h
timetick.o: timetick.cpp /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i
transaction.o: transaction.cpp \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Atomic_Op.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Time_Value.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.i \
 /home/cs/guest/mrm/ASNMP/asnmp/transaction.h \
 /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /home/cs/guest/mrm/ASNMP/asnmp/pdu.h \
 /home/cs/guest/mrm/ASNMP/asnmp/vb.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmperrs.h \
 /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/wpdu.h \
 /home/cs/guest/mrm/ASNMP/asnmp/asn1.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.i
vb.o: vb.cpp /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/vb.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmperrs.h \
 /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h
wpdu.o: wpdu.cpp /home/cs/guest/mrm/ASNMP/asnmp/wpdu.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/stdcpp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/OS.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Trace.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Msg.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/ACE.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Priority.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Log_Record.i \
 /home/cs/guest/mrm/ASNMP/asnmp/asn1.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmp.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Handle_Set.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Simple.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SV_Semaphore_Complex.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Event_Handler.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Synch_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Thread.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Atomic_Op.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Time_Value.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Free_List.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Timer_Queue_T.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Signal.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Token.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Pipe.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/IPC_SAP.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_IO.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Stream.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Reactor.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/SOCK_Dgram.i \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/vb.h \
 /home/cs/guest/mrm/ASNMP/asnmp/snmperrs.h \
 /home/cs/guest/mrm/ASNMP/asnmp/timetick.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /home/cs/guest/mrm/ASNMP/asnmp/pdu.h \
 /home/cs/guest/mrm/ASNMP/asnmp/enttraps.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
