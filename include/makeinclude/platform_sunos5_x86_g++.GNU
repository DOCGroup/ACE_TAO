# SunOS 5.x (Solaris 2.x) with g++ *not* using Orbix
#
# SOBUILD - compile into .so directly
#
CC	= gcc 
CXX	= gcc -I. -fno-strict-prototypes -D__ACE_INLINE__ # -frepo 
DLD	= $(CXX)
LDFLAGS	+= -L $(WRAPPER_ROOT)/ace -L ./ # -z muldefs
LIBS	+= -lsocket -ldl -lstdc++ -lgen -lnsl -lthread -lw
PIC	= -fpic
AR	= ar
ARFLAGS	= ruv
RANLIB	= /bin/true
SOFLAGS	= -G $(CPPFLAGS)
SOBUILD = $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.so $<
# SOLINK 	= cp $< $@
# SOLINK 	= ln -s $< $@
# SOLINK	= $(SOLINK.cc) -o $@ $(LDFLAGS) $<
PRELIB	= (echo "int main() { return 0; }" > gcctemp.c && \
	  $(COMPILE.cc) -o gcctemp.o gcctemp.c && \
	  $(LINK.cc) -o gcctemp gcctemp.o $^ $(LDFLAGS) $(LIBS); \
	  status=$$?; rm -f gcctemp*; exit $$status)
