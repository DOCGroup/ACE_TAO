# $Id$

# AIX 4.2.x and higher using the Visual Age C++ environment.
# *not* using Orbix
#
# On 4.2, the AIX fileset bos.rte.bind_cmds must be at version 4.2.0.2 or
# higher in order to build libACEshr.a, and at 4.2.0.4 or higher for dl*()
# routines to work properly. Best to apply the update available at:
# ftp://service.boulder.ibm.com/aix/fixes/v4/os/bos.rte.bind_cmds.4.2.0.4.bff
# or a successor.

ifeq (,$(debug))
  debug = 1
endif
ifeq (,$(distrib))
  distrib = 0
endif
ifeq (,$(optimize))
  optimize = 0
endif

# In case anything here or in the config depends on OS version number,
# grab it here and pass it all to the compiler as well.
AIX_MAJOR_VERS := $(shell uname -v)
AIX_MINOR_VERS := $(shell uname -r)

# Visual Age C++ does everything - maintain dependency information, build
# commands, and results. The Makefile scheme here is primarily for cases
# where multiple targets are to be built (such as in the 'tests' directory)
# and multiple configurations are used for it. Set a variable to say that
# VACPP is in charge, and the build scheme then runs vacbld rather than
# go through the whole dependency check.

using_aix_vacpp = 1

