OBJS = main.o snmp_agent.o agent_impl.o
CC=CC
INCL = -I$(ACE_ROOT) -I$(ACE_ROOT)/ASNMP
DEBUG = -g
CFLAGS =$(INCL) -mt  $(DEBUG)
RUNPATH = -R $(ACE_ROOT)/ace:$(ACE_ROOT)/ASNMP/asnmp 
LIBS = -L $(ACE_ROOT)/ASNMP/asnmp -lasnmp -L $(ACE_ROOT)/ace -lACE -lnsl -lsocket

agent: $(OBJS)
	$(CC) -o agent -mt $(DEBUG) $(OBJS) $(LIBS) $(RUNPATH)


main.o: main.cpp 
	$(CC) -c $(CFLAGS) main.cpp 

snmp_agent.o: snmp_agent.cpp snmp_agent.h
	$(CC) -c $(CFLAGS) snmp_agent.cpp

agent_impl.o: agent_impl.cpp agent_impl.h
	$(CC) -c $(CFLAGS) agent_impl.cpp

clean:
	-rm *.o agent *~
