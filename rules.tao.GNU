# -*- Makefile -*-

#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common Makefile rules for all of TAO
#
#----------------------------------------------------------------------------

####
#### Required macros for TAO.
####
TAO_ROOT ?= $(ACE_ROOT)/TAO
TAO_ROOT := $(subst \,/,$(TAO_ROOT))

#This logically belongs in rules.dds.GNU but there are a few projects
#which reference $DDS_ROOT but do not include rules.dds.GNU
ifdef DDS_ROOT
  DDS_ROOT := $(subst \,/,$(DDS_ROOT))
endif

ifeq ($(exceptions),0)
  default:
	echo "ERROR: Emulated exceptions are not safe to use anymore, see the NEWS file"
endif # exceptions

####
#### Macro customization.
####
security ?= 1
ifeq ($(security),1)
  ACE_MAKE_OPTIONS += security
endif # security

ifeq ($(minimum_corba),1)
  ifndef interceptors
    interceptors = 0
  endif  # ! interceptors
  CPPFLAGS += -DTAO_HAS_MINIMUM_CORBA=1
  ACE_MAKE_OPTIONS += minimum_corba
else  # minimum_corba
  override minimum_corba = 0
endif # minimum_corba

ifeq ($(ami),0)
  CPPFLAGS += -DTAO_HAS_AMI=0
else
  override ami = 1
  ACE_MAKE_OPTIONS += ami
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_AMI=1
  ifndef ami_callback
    ami_callback = 1
  endif # ! ami_callback
  ifndef ami_poller
    ami_poller = 1
  endif # ! ami_poller
endif # ami

ifeq ($(ami_poller),0)
  CPPFLAGS += -DTAO_HAS_AMI_POLLER=0
else  # ami_poller
  override ami_poller = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_AMI_POLLER=1
  ACE_MAKE_OPTIONS += ami_poller
endif # ami_poller

ifeq ($(ami_callback),0)
  CPPFLAGS += -DTAO_HAS_AMI_CALLBACK=0
else  # ami_callback
  override ami_callback = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_AMI_CALLBACK=1
  ACE_MAKE_OPTIONS += ami_callback
endif # ami_callback

ifeq ($(corba_messaging),0)
  rt_corba = 0
  CPPFLAGS += -DTAO_HAS_CORBA_MESSAGING=0
else  # corba_messaging
  override corba_messaging = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_CORBA_MESSAGING=1
  ACE_MAKE_OPTIONS += corba_messaging
endif # corba_messaging

ifeq ($(rt_corba),0)
  CPPFLAGS += -DTAO_HAS_RT_CORBA=0
else  # rt_corba
  override rt_corba = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_RT_CORBA=1
  ACE_MAKE_OPTIONS += rt_corba
endif # rt_corba

ifeq ($(interceptors),0)
  CPPFLAGS += -DTAO_HAS_INTERCEPTORS=0
else  # interceptors
  override interceptors = 1
  # The following line is not needed,
  # the $TAO_ROOT/tao/orbconf.h file defines it by default
  # CPPFLAGS += -DTAO_HAS_INTERCEPTORS=1
  ACE_MAKE_OPTIONS += interceptors
endif # interceptors

# The following is more to document the transport_current,
# the $TAO_ROOT/tao/orbconf.h file defines it by default
# CPPFLAGS += -DTAO_HAS_TRANSPORT_CURRENT=1
transport_current ?= 1

ifeq (1,$(CROSS-COMPILE))
  ACE_MAKE_OPTIONS += cross_compile
else
endif # ! CROSS-COMPILE

# Turn on symbol versioning. The scheme that we follow is to allow
# applications dependent on libraries, with same version numbers (major,
# minor and beta) to run, but applications with dependencies on libraries
# with different minor or major or beta versions to fail.
#
ifeq (cmd,$(findstring cmd,$(SHELL)))
TAO_MAJOR_VERSION := $(shell awk "/TAO_MAJOR_VERSION/ { print $$3}" ${TAO_ROOT}/tao/Version.h)
TAO_MINOR_VERSION := $(shell awk "/TAO_MINOR_VERSION/ { print $$3}" ${TAO_ROOT}/tao/Version.h)
TAO_BETA_VERSION := $(shell awk "/TAO_BETA_VERSION/ { print $$3}" ${TAO_ROOT}/tao/Version.h)
else
TAO_MAJOR_VERSION := $(shell awk '/TAO_MAJOR_VERSION/ { print $$3}' ${TAO_ROOT}/tao/Version.h)
TAO_MINOR_VERSION := $(shell awk '/TAO_MINOR_VERSION/ { print $$3}' ${TAO_ROOT}/tao/Version.h)
TAO_BETA_VERSION := $(shell awk '/TAO_BETA_VERSION/ { print $$3}' ${TAO_ROOT}/tao/Version.h)
endif

GNUACE_PROJECT_VERSION ?= $(TAO_MAJOR_VERSION).$(TAO_MINOR_VERSION).$(TAO_BETA_VERSION)
