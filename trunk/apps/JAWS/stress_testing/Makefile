#############################################################################
#
#  Makefile for benchmarking client (Solaris)
#
#############################################################################

CXX		= CC
INCPATH		= $(WRAPPER_ROOT)
LIBPATH		= $(WRAPPER_ROOT)/ace
DFLAGS		= -O2
CFLAGS		= $(DFLAGS)  -I$(INCPATH) -L$(LIBPATH) -mt -D_REENTRANT -D__ACE_INLINE__

#############################################################################
# C++ directives

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) $(CFLAGS) -c $<


#############################################################################

all: http_tester benchd

zeus: zeus.o	
	$(CXX) $(CFLAGS) -o $@ zeus.o -lsocket -lnsl	

http_tester: util.o connection.o http_tester.o stats.o
	$(CXX) $(CFLAGS) -o $@ stats.o util.o connection.o http_tester.o -lACE -lsocket -lnsl

benchd: benchd.o
	$(CXX) $(CFLAGS) -o $@ benchd.o -lACE -lsocket -lnsl

clean:
	rm -Rf Templates.DB/ *.o 

