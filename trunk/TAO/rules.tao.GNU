#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common rules for all of TAO
#
#----------------------------------------------------------------------------

ifndef TAO_IDL
  TAO_IDL = $(TAO_ROOT)/TAO_IDL/tao_idl
endif

.SUFFIXES:      S.h C.h S.i C.i S.cpp C.cpp

%S.h %Ser.h %C.h %Cli.h %S.i %Ser.i  %C.i %Cli.i %S.cpp %Ser.cpp %C.cpp %Cli.cpp:       %.idl $(TAO_IDL)
        $(TAO_IDL) $(TAO_IDLFLAGS) $<

ifeq ($(findstring g++,$(CXX)),g++)
  CCFLAGS += -Wno-unused

  ifneq ($(findstring egcs-2.91,$(shell g++ --version)),)
    #### egcs 2.91.57 seems to go overboard:
    ####   DynAny_i.cpp:565: warning: `class TAO_DynAny_i * this' might be
    ####   used uninitialized in this function
    CCFLAGS += -Wno-uninitialized
  endif # egcs 2.91.x
endif # g++
