# -*- Makefile -*-
# $Id$
#
# This file should allow to build ACE for cygwin32 with cygwin tools.
#

cygwin32 = 1

# Cygwin doesn't have rwho
rwho = 0

include $(ACE_ROOT)/include/makeinclude/platform_gnuwin32_common.GNU
