#	$Id$

include stdmk

EXE = client 	

all: $(EXE)

clean:
	-$(RM) core *.o cubitC.* cubitS.* $(EXE) 
	-$(RM) -r Templates.DB

cubitS.cpp: cubit.idl
	$(ORBCC) cubit.idl

cubitC.cpp: cubit.idl
	$(ORBCC) cubit.idl

client: cubitC.o Profile_Timer.o client.o
	$(CC) -o client cubitC.o Profile_Timer.o client.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)
