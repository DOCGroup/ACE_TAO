# $Id$
#
# This file should allow to build ACE for cygwin32 with cygwin tools.
#
# Edit to change TCPU and w32api defs below.
#
# Don't forget to define the ACE_ROOT environment variable!
#

#
# Chose your target CPU (uncomment ONE of these).
#
TCPU = pentiumpro
#TCPU = pentium
#TCPU = i486
#TCPU = i386

cygwin32 = 1

ifeq (,$(threads))
  threads = 0
endif

# Cygwin doesn't allow to add the version number of ACE to the dll name
# because fe ace.dll.5.2.3 isn't a valid dll name
SONAME=
SOVERSION=

# Disable auto-import warnings. The Cygwin linker has a problem with imports
# See https://sourceforge.net/tracker/?func=detail&atid=102435&aid=683455&group_id=2435
# for the details why to do this.
LDFLAGS += -Wl,--enable-auto-import

include $(ACE_ROOT)/include/makeinclude/platform_gnuwin32_common.GNU
