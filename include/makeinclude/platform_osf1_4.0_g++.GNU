# This platform macros file is intended to work with Digital UNIX 4.0
# (OSF/1 4.0) and GCC in version 2.7.2.1 (or newer).

CC		= gcc -g -D_REENTRANT
CXX		= gcc -g -D_REENTRANT -fno-strict-prototypes -D__ACE_INLINE__ 
DLD		= $(CXX)
LIBS		+= -lpthread -lmach -lexc -ltli -lstdc++
PIC		= -fpic
ARFLAGS		= ruvZ
# When libraries are archived, a hash index is automatically created so there
# is no need for ranlib
RANLIB		= /usr/bin/true
SOFLAGS		= -shared $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
PRELIB		= (echo "main() { }" > gcctemp.c && \
		  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
		  $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
		  status=$$?; rm -f gcctemp*; exit $$status)

