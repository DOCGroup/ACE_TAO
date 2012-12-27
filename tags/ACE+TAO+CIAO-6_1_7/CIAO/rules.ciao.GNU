# -*- Makefile -*-

#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common Makefile rules for all of CIAO
#
#----------------------------------------------------------------------------

TAO_ROOT ?= $(ACE_ROOT)/TAO
CIAO_ROOT ?= $(TAO_ROOT)/CIAO

TAO_ROOT := $(subst \,/,$(TAO_ROOT))
CIAO_ROOT := $(subst \,/,$(CIAO_ROOT))

ifndef TAO_IDL3_TO_IDL2
  TAO_IDL3_TO_IDL2 = $(CIAO_ROOT)/tools/IDL3_to_IDL2/tao_idl3_to_idl2
  TAO_IDL3_TO_IDL2_DEP = $(TAO_IDL3_TO_IDL2)$(EXEEXT)
else
ifndef TAO_IDL3_TO_IDL2_DEP
  TAO_IDL3_TO_IDL2_DEP = $(TAO_IDL3_TO_IDL2)
endif
endif

ifndef TAO_IDL3_TO_XMI
  TAO_IDL3_TO_XMI = $(CIAO_ROOT)/tools/IDL3_to_XMI/tao_idl3_to_xmi
  TAO_IDL3_TO_XMI_DEP = $(TAO_IDL3_TO_XMI)$(EXEEXT)
else
ifndef TAO_IDL3_TO_XMI_DEP
  TAO_IDL3_TO_XMI_DEP = $(TAO_IDL3_TO_XMI)
endif
endif

ifeq (,$(findstring -L$(CIAO_ROOT)/ciao,$(LDFLAGS)))
  LDFLAGS += -L$(CIAO_ROOT)/ciao
endif
ifeq (,$(findstring -I$(CIAO_ROOT),$(INCLDIRS)))
  INCLDIRS += -I$(CIAO_ROOT)
endif

# Turn on symbol versioning. The scheme that we follow is to allow
# applications dependent on libraries, with same version numbers (major,
# minor and beta) to run, but applications with dependencies on libraries
# with different minor or major or beta versions to fail.
#
ifeq (cmd,$(findstring cmd,$(SHELL)))
CIAO_MAJOR_VERSION := $(shell awk "/CIAO_MAJOR_VERSION/ { print $$3}" ${CIAO_ROOT}/ciao/Version.h)
CIAO_MINOR_VERSION := $(shell awk "/CIAO_MINOR_VERSION/ { print $$3}" ${CIAO_ROOT}/ciao/Version.h)
CIAO_BETA_VERSION := $(shell awk "/CIAO_BETA_VERSION/ { print $$3}" ${CIAO_ROOT}/ciao/Version.h)
else
CIAO_MAJOR_VERSION := $(shell awk '/CIAO_MAJOR_VERSION/ { print $$3}' ${CIAO_ROOT}/ciao/Version.h)
CIAO_MINOR_VERSION := $(shell awk '/CIAO_MINOR_VERSION/ { print $$3}' ${CIAO_ROOT}/ciao/Version.h)
CIAO_BETA_VERSION := $(shell awk '/CIAO_BETA_VERSION/ { print $$3}' ${CIAO_ROOT}/ciao/Version.h)
endif

GNUACE_PROJECT_VERSION ?= $(CIAO_MAJOR_VERSION).$(CIAO_MINOR_VERSION).$(CIAO_BETA_VERSION)
