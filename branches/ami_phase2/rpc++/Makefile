TOP = .
SUBDIRS = StdHdrs example

INSTROOT = ../..

CC = gcc
CFLAGS = -ggdb
C++FLAGS = $(CFLAGS)

LIBHDRS = rpc++/xdr++.h rpc++/request.h rpc++/service.h rpc++/stub.h \
	  rpc++/callback.h version.h
LIBSRCS = xdr++.cc service.cc stub.cc request.cc callback.cc

LIBOBJS = $(LIBSRCS:%.cc=%.o) $(GENSRCS:%.cc=%.o)

all:: librpc++.a

subdirs.all:: librpc++.a

librpc++.a: $(LIBOBJS)
	rm -f $@
	ar cq $@ $(LIBOBJS)
	if [ -x /bin/ranlib -o -x /usr/bin/ranlib ]; then ranlib $@; fi

install:: librpc++.a
	install -d $(INSTROOT)/lib
	if cmp -s librpc++.a $(INSTROOT)/lib/librpc++.a; then : ; \
	else rm -f $(INSTROOT)/lib/librpc++.a; \
	  cp -p librpc++.a $(INSTROOT)/lib; \
	  chmod 444 $(INSTROOT)/lib/librpc++.a; \
	  for f in rpc++/*.h; do \
	    rm -f $(INSTROOT)/include/$$f; done; fi
	install -d $(INSTROOT)/include/rpc++
	for f in rpc++/*.h; do \
	  cmp -s $$f $(INSTROOT)/include/$$f \
	  || install -c -m 444 $$f $(INSTROOT)/include/rpc++; done

DISTLIST = Makefile README.ORIG README COPYING Proj.make rpc++.texi \
	   gcc-2.2.2.fix $(LIBHDRS) $(LIBSRCS)

clean::
	rm -f $(CLEANWILDCARDS) librpc++.a

include .dependencies

.dependencies: $(LIBHDRS) $(LIBSRCS)
	gcc -M $(CPPFLAGS) $(LIBSRCS) > .dependencies

distlist::
	@for f in *.[ch] *.cc; do \
	  if expr " $(DISTLIST) " : ".* $$f " >/dev/null; then : ; \
	  else echo 1>&2 "Warning: c- or h-file \"$$f\" not in DISTLIST"; fi; \
	done

include $(TOP)/Proj.make
