# Generated automatically from Makefile.in by configure.
#
# WebStone src/Makefile.in, version 2.0a5
# last modified 12 Dec 1995 by mblakele@engr.sgi.com
#

PRODUCT = webstone
VERSION = 2.0b4

DYNAMICDEFS	= -DMCC_HTTPD -DXP_UNIX
DYNAMICINCLUDES	= -I/usr/ns-home/nsapi/include

LD_SHAREDCMD	= ld -shared

CC		= cc
LIBS		= -lsocket -lm -lthread 		#-lm 
LDFLAGS         = 
INCLUDES	= $(DYNAMICINCLUDES)
CFLAGS		= -g
CPPFLAGS	= 
DEFS		= -DHAVE_CONFIG_H $(DYNAMICDEFS)

BINDIR		= ../bin
CP		= cp
RM		= rm -f

EXECUTABLE1	= webmaster
MAIN1		= webmaster.c  
EXECUTABLE2	= webclient
MAIN2		= webclient.c
EXECUTABLE3	= genrand
MAIN3		= genrand.c

MASTERSRCS	= timefunc.c bench.c parse_file_list.c statistics.c \
		  errexit.c sysdep.c

STONESRCS	= timefunc.c get.c \
		  parse_file_list.c bench.c statistics.c errexit.c \
		  sysdep.c

#For Solaris 2.x, remove the comment from the following lines
LIBS		= -lm -lnsl -lsocket
#-L/usr/ucblib -lucb
#INCLUDES       = -I/usr/ns-home/nsapi/include -I/usr/ucbinclude

OPTIMIZATION	= -O
COMPILE         = $(CC) $(CFLAGS) $(DEFS) $(INCLUDES)

MASTEROBJS = webmaster.o $(MASTERSRCS:.c=.o) 
STONEOBJS = webclient.o $(STONESRCS:.c=.o) 
GENRANDOBJS = genrand.o

TARGETS = $(EXECUTABLE1) $(EXECUTABLE2) $(EXECUTABLE3)

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	@echo "COMPILING FILE " $<
	$(COMPILE) -c $< -o $@

all:	$(TARGETS)

install: all
	$(CP) $(TARGETS) $(BINDIR)

pure: all
	purify -mail-to-user=root webclient
	$(CP) webclient.pure $(BINDIR)/webclient
	$(RM) webclient.pure
	purify -mail-to-user=root webmaster
	$(CP) webmaster.pure $(BINDIR)/webmaster
	$(RM) webmaster.pure

clean:	
	$(RM) $(MASTEROBJS) $(STONEOBJS) $(GENRANDOBJS)
	$(RM) webmaster.pure webclient.pure

clobber: clean
	$(RM) config.cache config.log config.status config.h Makefile
	$(RM) $(TARGETS)

webmaster.o: webmaster.c bench.h parse_file_list.h statistics.h \
		sysdep.h timefunc.h debug.h config.h

webclient.o: webclient.c bench.h debug.h get.h \
		parse_file_list.h statistics.h sysdep.h timefunc.h config.h

timefunc.o: timefunc.c timefunc.h bench.h debug.h statistics.h \
		sysdep.h config.h

bench.o: bench.c bench.h debug.h statistics.h sysdep.h timefunc.h config.h

errexit.o: errexit.c sysdep.h config.h

statistics.o: statistics.c statistics.h sysdep.h config.h

cgi: cgi-send.o
	$(COMPILE) cgi-send.o -o cgi-send.cgi

nsapi: nsapi-send.o
	$(LD_SHAREDCMD) nsapi-send.o -o nsapi-send.so

webmaster: $(MASTEROBJS)
	$(COMPILE) $(MASTEROBJS) $(LIBPATH) $(LIBS) -o $(EXECUTABLE1)

webclient: $(STONEOBJS)
	$(COMPILE) $(STONEOBJS) $(LIBPATH) $(LIBS) -lthread -o $(EXECUTABLE2)

genrand: $(GENRANDOBJS)
	$(COMPILE) $(GENRANDOBJS) $(LIBPATH) $(LIBS) -o $(EXECUTABLE3)

#
