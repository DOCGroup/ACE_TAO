include ./orbix_defaults.mk

all:	client server
	@echo

	@echo "\"client\" and \"server\" have been compiled."
	@echo
	@echo "To run tests, check the how_to_run_tests file."
	@echo

C++FLAGS += -DWANT_ORBIX_FDS -DLM_RESULTS
IDLFLAGS	+= -c C.cpp -s S.cpp -B
SERVER_OBJS	= ttcpS.o ttcp_i.o
CLIENT_OBJS	= ttcpC.o ttcp_i.o

#QUANTIFY	=
QUANTIFY	= quantify -cache-dir=$(IR)/quantify

client: $(CLIENT_OBJS)
	$(QUANTIFY) $(C++) $(C++FLAGS) -o client $(CLIENT_OBJS) -lITclt $(LDFLAGS)

server: $(SERVER_OBJS)
	$(QUANTIFY) $(C++) $(C++FLAGS) -o server $(SERVER_OBJS)  -lITsrv $(LDFLAGS)

clean:
	rm -f core *.o *~ client server

realclean:
	rm -f core *.o *~ client server

