FILES = \
	any.cpp cdr.cpp corbacom.cpp debug.cpp except.cpp interp.cpp \
	marshal.cpp object.cpp orbobj.cpp principa.cpp tc_const.cpp \
	typecode.cpp nvlist.cpp request.cpp toa.cpp

ROOT =	../..

include ../Makefile.gen

t-xdr:	t-xdr.cpp
	$(LINK.cc) -o t-xdr t-xdr.cpp \
		-qoption ld -R.. -L.. -lcorba \
		-lsocket -lnsl
