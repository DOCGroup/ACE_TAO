#----------------------------------------------------------------------------
#
#	$Id$
#
#	Common rules for all of TAO
#
#----------------------------------------------------------------------------

ifndef TAO_IDL
TAO_IDL = $(TAO_ROOT)/TAO_IDL/tao_idl
endif

.SUFFIXES:	S.h C.h S.i C.i S.cpp C.cpp

%S.h %Ser.h %C.h %Cli.h %S.i %Ser.i  %C.i %Cli.i %S.cpp %Ser.cpp %C.cpp %Cli.cpp:	%.idl $(TAO_IDL)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<

ifeq ($(findstring g++,$(CXX)),g++)
  CCFLAGS += -Wno-unused
endif # g++
