# -*- Makefile -*-

#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common Makefile rules for all of DAnCE
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

ifndef DANCE_ROOT
  DANCE_ROOT = $(TAO_ROOT)/DAnCE
endif

DANCE_ROOT := $(subst \,/,$(DANCE_ROOT))


ifeq (,$(findstring -L$(DANCE_ROOT)/DAnCE,$(LDFLAGS)))
  LDFLAGS += -L$(DANCE_ROOT)/DAnCE
endif
ifeq (,$(findstring -I$(DANCE_ROOT),$(INCLDIRS)))
  INCLDIRS += -I$(DANCE_ROOT)
endif

# Turn on symbol versioning. The scheme that we follow is to allow
# applications dependent on libraries, with same version numbers (major,
# minor and beta) to run, but applications with dependencies on libraries
# with different minor or major or beta versions to fail.
#
ifeq (cmd,$(findstring cmd,$(SHELL)))
DANCE_MAJOR_VERSION := $(shell awk "/DANCE_MAJOR_VERSION/ { print $$3}" ${DANCE_ROOT}/DAnCE/Version.h)
DANCE_MINOR_VERSION := $(shell awk "/DANCE_MINOR_VERSION/ { print $$3}" ${DANCE_ROOT}/DAnCE/Version.h)
DANCE_BETA_VERSION := $(shell awk "/DANCE_BETA_VERSION/ { print $$3}" ${DANCE_ROOT}/DAnCE/Version.h)
else
DANCE_MAJOR_VERSION := $(shell awk '/DANCE_MAJOR_VERSION/ { print $$3}' ${DANCE_ROOT}/DAnCE/Version.h)
DANCE_MINOR_VERSION := $(shell awk '/DANCE_MINOR_VERSION/ { print $$3}' ${DANCE_ROOT}/DAnCE/Version.h)
DANCE_BETA_VERSION := $(shell awk '/DANCE_BETA_VERSION/ { print $$3}' ${DANCE_ROOT}/DAnCE/Version.h)
endif

GNUACE_PROJECT_VERSION ?= $(DANCE_MAJOR_VERSION).$(DANCE_MINOR_VERSION).$(DANCE_BETA_VERSION)
