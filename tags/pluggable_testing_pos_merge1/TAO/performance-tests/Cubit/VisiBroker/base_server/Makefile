#	$Id$

include stdmk

EXE = server 	

all: $(EXE)

clean:
	-rm -f core *.o cubitC.* cubitS.* $(EXE) 
	-rm -rf Templates.DB

cubitS.cpp: cubit.idl
	$(ORBCC) cubit.idl

cubitC.cpp: cubit.idl
	$(ORBCC) cubit.idl

server: cubitS.o cubitC.o server.o Profile_Timer.o cubit_impl.o
	$(CC) -o server cubitS.o cubitC.o cubit_impl.o server.o Profile_Timer.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)
