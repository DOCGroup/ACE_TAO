# SunOS 5.x (Solaris 2.x) with SunC++ 4.1
# *not* using Orbix
CC              = cc
CXX             = CC -mt -noex
DLD             = $(CXX) 
LD		= $(CXX)
LDFLAGS         += -R $(WRAPPER_ROOT)/ace 
LIBS            += -lsocket -ldl -lnsl -lgen
PIC             = -PIC
# SUNC++ 4.1 has a bug with the -xar command:
#
# from 'CC -readme' the following bug is reported:
#
# 4. Using -xar to add generated templates
# ----------------------------------------
# 
#        When using -xar to add generated templates to an archive, 
#        please be aware that this command will both update 
#        existing object files in the archive, and add new ones. 
#        Existing object files that may be obsolete will still
#        be present. In these cases, the safest course of action 
#        would be to delete the .a file prior to the invocation of -xar.
#
# so here we remove the archive prior to making the new one
#
AR              = rm -f $@ ; CC
ARFLAGS         = -xar -o
RANLIB          = echo
SOFLAGS         = -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
