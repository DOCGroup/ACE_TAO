#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for all the ACE+SNMP ``one-button' tests
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

# ACE+SNMP before -lACE

BIN = 	Oid_Test \
	Counter_Test \
	Counter64_Test \
	Integer_Test \
	Octet_Test \
	Gauge_Test \
	Address_Test \
	Target_Test \
	Varbind_Test 
CCFLAGS = -I$(ACE_ROOT)/ASNMP/

LSRC   = $(addsuffix .cpp,$(BIN))
VLDLIBS = -L$(ACE_ROOT)/ASNMP/asnmp -lasnmp $(LDLIBS:%=%$(VAR))

BUILD = $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

Address_Test.o: Address_Test.cpp \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
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
 /home/cs/guest/mrm/ASNMP/asnmp/octet.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Counter64_Test.o: Counter64_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/ctr64.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Counter_Test.o: Counter_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/counter.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Gauge_Test.o: Gauge_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/gauge.h \
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Integer_Test.o: Integer_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/integer.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Octet_Test.o: Octet_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Oid_Test.o: Oid_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smival.h \
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Target_Test.o: Target_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/smi.h \
 /home/cs/guest/mrm/ASNMP/asnmp/target.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Addr.i \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/INET_Addr.i \
 /home/cs/guest/mrm/ASNMP/asnmp/address.h \
 /home/cs/guest/mrm/ASNMP/asnmp/oid.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i
Varbind_Test.o: Varbind_Test.cpp \
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
 /home/cs/guest/mrm/ASNMP/asnmp/address.h test_config.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.h \
 /project/adaptive/ACE_wrappers/build/SunOS5.5/ace/Containers.i

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
