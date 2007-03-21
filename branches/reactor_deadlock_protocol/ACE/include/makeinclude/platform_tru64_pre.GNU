# $Id$

# Common definitions for all versions of Digital Unix.

ifndef ACE_DU_VERSION
  ACE_DU_VERSION := $(shell uname -r)
endif # ! ACE_DU_VERSION

ifndef ACE_DU_SUBVERSION
  ACE_DU_SUBVERSION := $(shell /usr/sbin/sizer -v)
endif # ! ACE_DU_SUBVERSION
