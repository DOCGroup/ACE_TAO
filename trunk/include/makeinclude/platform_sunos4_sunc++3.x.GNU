# SunOS 4.x (Solaris 1.x) with SunC++ 3.x
CC		= cc
CXX		= CC
DLD		= /bin/ld
LD		= $(CXX)
LIBS		+= 
PIC		= -pic
AR		= ar
ARFLAGS		= ruv
RANLIB		= ranlib
SOFLAGS		= -assert pure-text 
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(RM) -rf $(VSHDIR)pt$*; mkdir $(VSHDIR)pt$*; \
		  YYZ="`$(CXX) $(CCFLAGS) $(CPPFLAGS) \
		  -ptr$(VSHDIR)pt$* -ptr./ptrepository $<`"; \
		  $(RM) -rf a.out; \
		  YYZ="`echo $(VSHDIR)pt$*/*.o`"; \
		  $(SOLINK.cc) -o $@ ./$(VSHDIR)$*.o $(YYZ)
