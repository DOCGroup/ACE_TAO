# $Id$

# SunOS 5.x (Solaris 2.x) with egcs
# Use the g++ platform file, with any overrides following its include.

include $(ACE_ROOT)/include/makeinclude/platform_sunos5_g++.GNU

# This is recommeded by Cygnus in their egcs-1.0 Caveats page.
# http://egcs.cygnus.com/caveats.html
CFLAGS          += -Wno-return-type

#### Uncomment the following line to use shared libstdc++.so, etc.
#### SOFLAGS         = -shared $(CPPFLAGS)
