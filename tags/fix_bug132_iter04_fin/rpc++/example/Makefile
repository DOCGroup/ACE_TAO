TOP = ..
SUBDIRS = 

CC = gcc
CPPFLAGS = -I..
CFLAGS = -ggdb
C++FLAGS = $(CFLAGS)
# for Sun:
#LOADLIBES = -L.. -lrpc++ -lg++
# for ISC 2.2:
LOADLIBES = -L.. -lrpc++ -lrpclib -lmisc -lg++ -linet -liberty

HDRS = calcsvc.h
SRCS = server.cc client.cc calcsvc.cc

all:: server client

server: server.o calcsvc.o
	$(CC) -o $@ server.o calcsvc.o $(LOADLIBES)

client: client.o calcsvc.o
	$(CC) -o $@ client.o calcsvc.o ../request.o $(LOADLIBES)

DISTLIST = Makefile $(HDRS) $(SRCS)

clean::
	rm -f $(CLEANWILDCARDS) server client

include .dependencies

.dependencies: $(HDRS) $(SRCS)
	gcc -M $(CPPFLAGS) $(SRCS) > .dependencies

distlist::
	@for f in *.[ch] *.cc; do \
	  if expr " $(DISTLIST) " : ".* $$f " >/dev/null; then : ; \
	  else echo 1>&2 "Warning: c- or h-file \"$$f\" not in DISTLIST"; fi; \
	done

include $(TOP)/Proj.make
