# $Id$
#
# Skeletal Makefile showing necessary compiler and loader flags needed
# to compile an ACE/TAO application.

CXX		= CC -g
IDL		= $(TAO_ROOT)/TAO_IDL/tao_idl

CPPFLAGS	= -c -I$(ACE_ROOT) -I$(TAO_ROOT) -I$(TAO_ROOT)/orbsvcs -I. -DHIERARCHICAL_BIND -DRECURSIVE_BIND
IDLFLAGS	=

# All ACE/TAO related libraries have links in the $ACE_ROOT/ace
# directory.  Alternately, you may define your LD_LIBRARY_PATH
# environment variable to search this directory.
LDFLAGS		= -L$(ACE_ROOT)/ace

# This is the *minimum* set of libraries required to link against the
# ACE/TAO ORB.  Note that this does *not* include any of the CORBA
# services libraries.
LDLIBS		= -lTAO -lACE -lorbsvcs -lsocket -ldl -lnsl -lgen

# ACE and TAO are built (by default) on the SUN platform *without*
# exception handling support, and with multiprocessing support.  Thus
# the following compiler flags are required. (-pta cause complete
# template class instantiation).
CFLAGS		= -mt -pta -noex
DCFLAGS		= -g

# Define the additional directives for IDL stub code support.
.SUFFIXES : S.h C.h S.i C.i S.cpp C.cpp

%S.h %C.h %S.i %C.i %S.cpp %C.cpp : %.idl
	$(IDL) $(IDLFLAGS) $?

%.o : %.cpp
	$(CXX) $(DCFLAGS) $(CFLAGS) $(CPPFLAGS) $<

% : %.o
	$(CXX) $(DCFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# The rest of this file is application specific
SVROBJS		= ns_tree.o test_objectS.o test_objectC.o

all : ns_tree

ns_tree : $(SVROBJS)

ns_tree.o : ns_tree.cpp test_objectS.h Makefile

clean :
	-rm -rf *.o *C.cpp *S.cpp *C.h *C.i *S.h *S.i Templates.DB
