# $Id$

# This file should allow to build ACE for mingw32 with mingw tools.
# Edit to change TCPU and M32_BASE below.
#
# Don't forget to define the ACE_ROOT environment variable!

# mingw32 packages as distributed from sourceforge:
# http://sourceforge.net/project/showfiles.php?group_id=2435&release_id=15084
# See also http://www.mingw.org.
#
# You will also need a GNU Make for win32 (so you can actualy
# USE this file :-). Tested with make/sh from cygwin.
#
# Tested with:
#
#	mingw-runtime-0.5-20010221
#	w32api-0.5-20010301
#	gcc-2.95.2-20001116
#	binutils-2.10.91-20010114
#	ld-2.10.91-20010126
#	libbfd-2.10.91-20010121
#
# Caveat:
#
#   If ld complains about not finding crt2.o, dllcrt2.o or gcrt2.o
#   when linking executables, you will need to modify the specs file for gcc:
# 	lib/gcc-lib/mingw32/2.95.2/specs
#   look for the line after the one begining ``*startfile:'', and add
#   full path as prefix for crt2, dllcrt2 and gcrt2
#   (i.e., change
#    ... %{!shared:%{!mdll:crt2%O%s}} ...
#    with
#    ... %{!shared:%{!mdll:c:/mingw32/lib/gcc-lib/mingw32/2.95.2/crt2%O%s}} ...
#    or something similar depending on the location of your instalation).

# Chose your target CPU (uncomment ONE of these).
TCPU = pentiumpro
#TCPU = pentium
#TCPU = i486
#TCPU = i386

# Define here mingw32 instalation location.
# These asumes no fancy hierachy, just all packages above unpacked on
# the same base directory.
# Optionally, you can define it as an environment variable, or set it
# on your platform_macros.GNU before including this file.

ifndef MINGW_BASE
  MINGW_BASE = c:/live/local/mingw32
endif

mingw32 = 1

include $(ACE_ROOT)/include/makeinclude/platform_gnuwin32_common.GNU
