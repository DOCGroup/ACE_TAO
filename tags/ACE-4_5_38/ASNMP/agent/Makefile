# Sample SNMPv1 Agent 
# Makefile for Solaris 2.x
# $Id$

OBJS = main.o snmp_agent.o agent_impl.o
INCL = -I$(ACE_ROOT) -I$(ACE_ROOT)/ASNMP
CC=CC
SOL_FLAGS=-mt -R $(ACE_ROOT)/ASNMP/asnmp:$(ACE_ROOT)/ace
DEBUG = -g $(SOL_FLAGS) 
CFLAGS =$(INCL) $(DEBUG)
RUNPATH = 
LIBS = -L $(ACE_ROOT)/ASNMP/asnmp -lasnmp -L $(ACE_ROOT)/ace -lACE -ldl  

agent: $(OBJS)
	$(CC) -o agent $(DEBUG) $(OBJS) $(LIBS) $(RUNPATH)


main.o: main.cpp 
	$(CC) -c $(CFLAGS) main.cpp 

snmp_agent.o: snmp_agent.cpp snmp_agent.h
	$(CC) -c $(CFLAGS) snmp_agent.cpp

agent_impl.o: agent_impl.cpp agent_impl.h
	$(CC) -c $(CFLAGS) agent_impl.cpp

clean:
	-rm *.o agent *~
