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
threads = 0
static_libs_only = 1
optimize = 0

include $(ACE_ROOT)/include/makeinclude/platform_gnuwin32_common.GNU
