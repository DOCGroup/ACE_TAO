#----------------------------------------------------------------------------
#	$Id$
#
#	Common makefile definitions for all applications
#
#----------------------------------------------------------------------------

ifndef TAO_ROOT
TAO_ROOT = $(ACE_ROOT)/TAO
endif

ifndef TAO_IDL
TAO_IDL = $(TAO_ROOT)/TAO_IDL/tao_idl
endif

LDFLAGS := $(patsubst -L$(ACE_ROOT)/ace, -L$(TAO_ROOT)/tao -L$(ACE_ROOT)/ace, $(LDFLAGS))
CPPFLAGS += -I$(TAO_ROOT) -I$(TAO_ROOT)/tao/compat $(TSS_ORB_FLAG)#-H

clean:
	-/bin/rm -rf *.o Log $(BIN) obj.* core Templates.DB .make.state

%S.cpp %C.cpp:	%.idl
	$(TAO_IDL) $(IDLFLAGS) $?
