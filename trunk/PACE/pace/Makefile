#----------------------------------------------------------------------------
#       $Id$
#
#       Makefile for the pace/pace C library release
#----------------------------------------------------------------------------

ifndef PACE_ROOT
  PACE_ROOT = $(ACE_ROOT)/pace
endif # ! PACE_ROOT

MAKEFILE = Makefile
LIB      = libPACE.a
SHLIB    = libPACE.$(SOEXT)

VPATH=.:arpa:config:netinet:sys

PACE_FILES = \
  aio \
  dirent \
  fcntl \
  grp \
  mqueue \
  netdb \
  pthread \
  pwd \
  sched \
  semaphore \
  setjmp \
  signal \
  stdio \
  stdlib \
  string \
  strings \
  time \
  unistd \
  utime \
  wait
ARPA_FILES = \
  arpa/inet
CONFIG_FILES = 
NETINET_FILES = \
  netinet/in
SYS_FILES = \
  sys/mman \
  sys/socket \
  sys/stat \
  sys/termios \
  sys/times \
  sys/utsname
POSIX_SP_FILES = \
  unistd \
  time \
  sys/utsname
POSIX_MP_FILES = \
  unistd \
  stdlib \
  sys/times \
  wait

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU

INCLDIRS += -I$(PACE_ROOT)
#INCLDIRS = -I$(PACE_ROOT)

####
#### Build customization.
####
ifndef PACE_COMPONENTS
  #### Do we want PACE subsets as below? If so, we need to add the
  #### documentation referenced below as well.
  #### Please see docs/PACE-subsets.html for documentation. ??
  PACE_COMPONENTS = \
    Pace \
    Arpa \
    Config \
    Netinet \
    Sys
endif # PACE_COMPONENTS

ifeq (FOR_ACE,$(PACE_COMPONENTS))
  #### These PACE components are necessary to support ACE.
  override PACE_COMPONENTS = \
    Pace \
    Arpa \
    Config \
    Netinet \
    Sys
else  # ! FOR_ACE
  #### Is there anything in PACE that ACE doesn't need? Probably not
endif # ! FOR_ACE

####
#### PACE_COMPONENTS support.
####
ifneq (,$(findstring Pace,$(PACE_COMPONENTS)))
  FILES += $(PACE_FILES)
endif # Pace

ifneq (,$(findstring Arpa,$(PACE_COMPONENTS)))
  FILES += $(ARPA_FILES)
endif # Arpa

ifneq (,$(findstring Config,$(PACE_COMPONENTS)))
  FILES += $(CONFIG_FILES)
endif # Config

ifneq (,$(findstring Netinet,$(PACE_COMPONENTS)))
  FILES += $(NETINET_FILES)
endif # Netinet

ifneq (,$(findstring Sys,$(PACE_COMPONENTS)))
  FILES += $(SYS_FILES)
endif # Sys

ifneq (,$(findstring Posix_sp,$(PACE_COMPONENTS)))
  FILES = $(POSIX_SP_FILES)
  LIB   = libPACE_SP.a
  SHLIB = libPACE_SP.$(SOEXT)
endif # Sys

ifneq (,$(findstring Posix_mp,$(PACE_COMPONENTS)))
  FILES = $(POSIX_MP_FILES)
  LIB   = libPACE_MP.a
  SHLIB = libPACE_MP.$(SOEXT)
endif # Sys

LSRC  = $(addsuffix .cpp,$(FILES))

BUILD += PACE_COMPONENTS

#### Setting PACELIB to null allows it to be used for building shared
#### libraries, including libPACE.$(SOEXT,) on certain platforms.
PACELIB =

include $(ACE_ROOT)/include/makeinclude/macros.GNU
include $(ACE_ROOT)/include/makeinclude/rules.common.GNU
include $(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include $(ACE_ROOT)/include/makeinclude/rules.lib.GNU
#### Disable installs in this ($(ACE_ROOT)/pace/pace) directory, because this
#### is the (default) destination of installs anyways.  This line prevents
#### creation of a link from the PACE library to itself when the build of
#### the PACE library fails.
INSTALL =
include $(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

.PHONY: PACE_COMPONENTS
PACE_COMPONENTS:
	@sh $(ACE_ROOT)/bin/pace_components --pace --set ' $(PACE_COMPONENTS) '

realclean:
	@sh $(ACE_ROOT)/bin/pace_components --pace --remove

# AIX template compilation speedup hack
# Not added for PACE - can always be added later if needed

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
