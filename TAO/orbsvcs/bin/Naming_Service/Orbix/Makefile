#
# COS Naming Service Implementation
#

#//include /home/cs/student/marina/CONFIG_MT/orb.mk
#############################################################
#
# This is a standard file to be included in Makefiles used
# to compile programs using Orbix
#
#############################################################



ORBIX_BINDIR    = /opt/Orbix_2.2MT/bin
ORBIX_LIBDIR    = /opt/Orbix_2.2MT/corba2/lib
ORBIX_INCDIR    = /opt/Orbix_2.2MT/corba2/include



X11BASE         = /usr/openwin
X11INCDIR       = $(X11BASE)/include
X11LIBDIR       = $(X11BASE)/lib

C++             = CC
C++FLAGS        = -g -I$(ORBIX_INCDIR)  -D_REENTRANT
C++SUFFIX       = cpp

# ------------------------------------------------------------
# ITCLT, IRCLT and ITSRV can be either statically or dynamically linked
# Use -Bdynamic or -Bstatic as appropriate.
# ------------------------------------------------------------
ITCLT           = -Bdynamic -lorbixmt
ITSRV		= -Bdynamic -lorbixmt
IRCLT           = -Bdynamic -lIRcltmt
ITIIOP		= -Bdynamic -liiopmt
ITDSI		= -Bdynamic -lDSImt 
LDFLAGS         = -L$(ORBIX_LIBDIR)
ITIFR		= -Bdynamic -lifrmt
ITINI		= -Bdynamic -lITinimt
# ------------------------------------------------------------
# SYSLIBS must be dynamically linked; otherwise you can expect to
# get linkage errors for symbols in the nsl library
# ------------------------------------------------------------
SYSLIBS         = -Bdynamic -mt -lnsl -lsocket

IDL             = $(ORBIX_BINDIR)/idl 
IDLFLAGS        = -A -B -c C.cpp -s S.cpp
#------------------------------------------------------------
# The following section defines implicit rules for creating
# *.{C,S}.C files, rules for compiling those
# into objects, and even a rule for compiling C++ source into
# objects (in case one isn't already defined).

# ------------------------------------------------------------
# first, put the new suffixes at the *head* of the suffix list,
# overriding any existing .C compilation method.
.SUFFIXES:
.SUFFIXES: .$(C++SUFFIX) .idl $(SUFFIXES)

# .SUFFIXES: .$(C++SUFFIX) .idl .hh $(SUFFIXES)
# ------------------------------------------------------------
# *[CS].o must be compiled here, and depends
# mostly on the C++ files produced from the IDL file.

%C.o: %C.$(C++SUFFIX)
	$(C++) -c $(C++FLAGS) $<

%S.o: %S.$(C++SUFFIX)
	$(C++) -c $(C++FLAGS) $<

%.o: %.$(C++SUFFIX)
	$(C++) -c $(C++FLAGS) $<

# and here's how to compile C++ files from the IDL file.
# only ONE of these rules will be run at make-time,

%S.$(C++SUFFIX): %.idl
	$(IDL) $(IDLFLAGS) $<

%C.$(C++SUFFIX): %.idl
	$(IDL) $(IDLFLAGS) $<

%.hh: %.idl
	$(IDL) $(IDLFLAGS) $<

####################################################################
#####################################################################
LOGGER_OBJS     = loggerS.o logger_tie.o logger-main.o CosNamingC.o
SERVER_OBJS	= CosNamingS.o NS_CosNaming.o server-main.o 
CLIENT_OBJS	= CosNamingC.o loggerC.o Client.o 
C++FLAGS	+= -I$(ACE_ROOT) 
LDFLAGS 	+= -lACE

all:	nameclient nameserver logger_factory

logger_factory: $(LOGGER_OBJS)
	$(C++) $(C++FLAGS) -o logger_factory $(LOGGER_OBJS) $(LDFLAGS) $(ITSRV)\
					$(SYSLIBS)


nameclient: $(CLIENT_OBJS)
	$(C++) $(C++FLAGS) -o nameclient $(CLIENT_OBJS) $(LDFLAGS) $(ITCLT)\
					$(SYSLIBS)

nameserver: $(SERVER_OBJS)
	$(C++) $(C++FLAGS) -o nameserver $(SERVER_OBJS) $(LDFLAGS) $(ITSRV) \
					$(SYSLIBS)

#purify -cache-dir=/tmp
clean:
	rm -rf CosNamingC.* CosNamingS.* CosNaming.hh logger.hh loggerC.* loggerS.*
	rm -rf Templates.DB core *.o nameclient nameserver logger_factory *~



