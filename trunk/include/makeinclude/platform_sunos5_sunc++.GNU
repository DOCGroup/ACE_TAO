# SunOS 5.x (Solaris 2.x) with SunC++ 4.0.1 or earlier
# *not* using Orbix
#
# Note:  with the +w option, some ACE files might generate warnings of
# one of these types:
# 1) "template manager : Warning: No valid template database available.
#     Creating default repository "Templates.DB"":
#    self explanatory, and apparently harmless.
# 2) "Warning: Could not find source for " one of the following:
#    default constructor, copy constructor, destructor, or assignment
#    operator.  It appears that with +w, Sun C++ 4.x issues this warning
#    whenever it has to generate one of these functions (because it wasn't
#    defined by the user).  This isn't really a problem.
# 3) "Warning: <foo> hides the function " or "hides the same name in an
#    outer scope".  A useful warning, perhaps, but not really a problem.
# There should be no other warnings from Sun C++ when building ACE.
#
CC		= cc
CXX		= CC +w -mt -pta # -noex This option should solve some evil problems with Solaris
DLD		= $(CXX) 
LD		= $(CXX)
LDFLAGS		+= -R $(WRAPPER_ROOT)/ace 
LIBS		+= -lsocket -ldl -lnsl -lgen # -lposix4
PIC		= -PIC
AR		= CC
ARFLAGS		= -xar -o
RANLIB		= echo
SOFLAGS		= -G $(CPPFLAGS)
SOBUILD		= $(COMPILE.cc) $(PIC) -o $(VSHDIR)$*.o $<; \
		  $(SOLINK.cc) -o $@ -h $@ $(LDFLAGS) $(VSHDIR)$*.o
