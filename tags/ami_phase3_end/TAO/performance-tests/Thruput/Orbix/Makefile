#### $Id$

include orbixsol2s4.mk

all:	client server
	@echo

	@echo "\"client\" and \"server\" have been compiled."
	@echo
	@echo "To run tests, check the how_to_run_tests file."
	@echo

C++FLAGS += -DSYSV -DWANT_ORBIX_FDS -DLM_RESULTS #-DUSE_TIE
SERVER_OBJS	= ttcpS.o ttcp_i.o utils.o Srv_Main.o
CLIENT_OBJS	= ttcpC.o utils.o Client.o

ifdef unpad
C++FLAGS += -DUNPAD
endif

ifeq ($(OSRELEASE),5.4)
C++FLAGS	+= -DSUNOS54
endif

client: $(CLIENT_OBJS)
	$(PRELINK) $(C++) $(C++FLAGS) -o client $(CLIENT_OBJS) $(LDFLAGS) $(ITSRV) $(SYSLIBS)

server: $(SERVER_OBJS)
	$(PRELINK) $(C++) $(C++FLAGS) -o server $(SERVER_OBJS) $(LDFLAGS) $(ITSRV) $(SYSLIBS)

ttcp.h ttcpC.$(C++SUFFIX) ttcpS.$(C++SUFFIX):ttcp.idl
	$(IDL) $(IDLFLAGS) $<

.PHONY:	clean

clean:
	rm -f core *.o *~ client server *C.cpp *S.cpp ttcp.h

checkin:
	ci ttcp.idl
	ci ttcp_i.h
	ci ttcp_i.cpp
	ci Srv_Main.cpp
	ci Client.cpp
	ci ttcp_decl.h
	ci utils.cpp
	rm -f core *.o *~ client server *C.cpp *S.cpp ttcp.h

checkout:
	co -l ttcp.idl
	co -l ttcp_i.h
	co -l ttcp_i.cpp
	co -l Srv_Main.cpp
	co -l Client.cpp
	co -l ttcp_decl.h
	co -l utils.cpp

release:
	test -d $(RDIR)/SII/1way/Orbix || mkdir -p $(RDIR)/SII/1way/Orbix
	cp *.idl *.cpp *.h Makefile run* README* $(RDIR)/SII/1way/Orbix/.
