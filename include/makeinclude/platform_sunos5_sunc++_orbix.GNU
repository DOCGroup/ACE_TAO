# $Id$

# SunOS 5.x (Solaris 2.x) with SunC++ 4.x
# *using* MT Orbix
INCLDIRS	= -I$(ORBIX_ROOT)/include

orbix=1

include $(ACE_ROOT)/include/makeinclude/platform_sunos5_sunc++.GNU
