include stdmk

EXE = test

all: $(EXE)

clean:
	-rm -f core *~ *.o *_c.hh *_c.cc *_s.hh *_s.cc $(EXE)
	-rm -rf Templates.DB

remora_c.cc: remora.idl	
	$(ORBCC) remora.idl

remora_s.cc: remora.idl
	$(ORBCC) remora.idl

test: remora_c.o test.o Remora_Import.o Remora_Export.o
	$(CC) -o test test.o remora_c.o Remora_Import.o Remora_Export.o\
		 $(LIBPATH) $(LIBORB) $(STDCC_LIBS) $(LIBACE)
