## $Id$
#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	
#
#	Makefile for MPEG stuff
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

INFO  = README

DIRS  =	mpeg_shared \
	mpeg_mib \
	mpeg_server \
	mpeg_client \
	client \
	server

CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat -I$(TAO_ROOT)/orbsvcs -I$(TSS_ORB_FLAG)#-H

MPEG_ROOT = $(TAO_ROOT)/orbsvcs/tests/AVStreams/mpeg/source
CPPFLAGS += -I$(MPEG_ROOT)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include $(TAO_ROOT)/rules.tao.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nolocal.GNU


