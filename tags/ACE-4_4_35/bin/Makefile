#############################################################################
#
#  Makefile for assignment 1
#
#############################################################################

include $(ACE_ROOT)/include/makeinclude/platform_macros.GNU

# CXX		= gcc 
CFILES		= clone.cpp 
OFILES		= clone.o
DFLAGS		= -b elf
CCFLAGS		+= -I$(ACE_ROOT) 

#############################################################################
# C++ directives

.SUFFIXES: .cpp
.cpp.o:
	$(CXX) $(CCFLAGS) -c $<
#############################################################################

clone: $(OFILES)
	$(CXX) $(CCFLAGS) -o $@ $(OFILES)

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

