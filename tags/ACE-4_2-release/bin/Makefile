#############################################################################
#
#  Makefile for assignment 1
#
#############################################################################

CXX		= CC
CFILES		= clone.cpp 
OFILES		= clone.o
DFLAGS		= -g
CFLAGS		= $(DFLAGS) -I$(WRAPPER_ROOT)

#############################################################################
# C++ directives

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) $(CFLAGS) -c $<
#############################################################################

clone: $(OFILES)
	$(CXX) $(CFLAGS) -o $@ $(OFILES)

clean:
	-/bin/rm -f *.o *.out *~ core

realclean: clean 
	-/bin/rm -fr clone

depend:
	g++dep -f Makefile $(CFILES)
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

clone.o : clone.cpp
# IF YOU PUT ANYTHING HERE IT WILL GO AWAY

