# SunOS 4.x (Solaris 1.x) with g++ *not* using Orbix
#
# SOBUILD - compile into .so directly
#
CC	= gcc 
CXX	= gcc -I. -fno-strict-prototypes -D__ACE_INLINE__ # -frepo 
DLD	= /bin/ld
#
# for G++ v2.6.0
LIBS	+= -lstdc++
# LIBS	+= -lnsl -lg++ 
PIC	= -fpic
AR	= ar
ARFLAGS	= ruv
RANLIB	= ranlib
SOFLAGS	= -assert pure-text 
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB	= (echo "int main() { return 0; }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp*; exit $$status)
