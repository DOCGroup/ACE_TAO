# -*- Makefile -*-

#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common Makefile rules for all of CIAO
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

ifndef CIAO_ROOT
  CIAO_ROOT = $(TAO_ROOT)/CIAO
endif

# Turn on symbol versioning. The scheme that we follow is to allow
# applications dependent on libraries, with same version numbers (major,
# minor and beta) to run, but applications with dependencies on libraries
# with different minor or major or beta versions to fail.
#
CIAO_MAJOR_VERSION := $(shell awk '/CIAO_MAJOR_VERSION/ { print $$3}' ${CIAO_ROOT}/ciao/Version.h)
CIAO_MINOR_VERSION := $(shell awk '/CIAO_MINOR_VERSION/ { print $$3}' ${CIAO_ROOT}/ciao/Version.h)
CIAO_BETA_VERSION := $(shell awk '/CIAO_BETA_VERSION/ { print $$3}' ${CIAO_ROOT}/ciao/Version.h)

# Version number of the libraries
#
ifeq ($(versioned_so),1)
  # Only set SOVERSION for CIAO if versioning is turned on and SOVERION is
  # blank, which gives users to ability to override it.
  ifeq ($(SOVERSION),)
    SOVERSION = .$(CIAO_MAJOR_VERSION).$(CIAO_MINOR_VERSION).$(CIAO_BETA_VERSION)
  endif # SOVERSION

  # Name that will be written into the dynamic library
  #
  ifneq ($(SONAME),)
    SONAME = $(SHLIB).$(CIAO_MAJOR_VERSION).$(CIAO_MINOR_VERSION).$(CIAO_BETA_VERSION)
  endif # SONAME
endif # versioned_so
