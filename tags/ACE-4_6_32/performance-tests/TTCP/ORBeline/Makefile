include ./stdmk
EXE = 	server client

all: $(EXE)

# Remeber -p for generating mon.out for profiler
#C++FLAGS = -O4 -p -DTEST_SEQUENCE 
#C++FLAGS = -O4  -DTEST_SEQUENCE 
#C++FLAGS = -O4 -p 
#C++FLAGS = -g 
C++FLAGS = -O4 $(CCFLAGS)

realclean:
	-rm -f core *.o client server $(EXE) *~

clean:
	-rm -f core *.o client server $(EXE) *~

ttcp_c.cc: ttcp.idl
	$(ORBCC) ttcp.idl

ttcp_s.cc: ttcp.idl
	$(ORBCC) ttcp.idl

ttcp_i.o: ttcp_i.cpp
	$(CC) $(C++FLAGS) -I./ -I../../include -c ttcp_i.cpp

client : ttcp_s.o ttcp_c.o ttcp_i.o
	$(CC) $(C++FLAGS) -o client ttcp_i.o ttcp_s.o ttcp_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

server : ttcp_s.o ttcp_c.o ttcp_i.o
	$(CC) $(C++FLAGS) -o server ttcp_i.o ttcp_s.o ttcp_c.o $(LIBPATH) $(LIBORB) $(STDCC_LIBS)

