#----------------------------------------------------------------------------
#
#       $Id$
#
#       Common rules for all of TAO
#
#----------------------------------------------------------------------------

####
#### Required macros for TAO.
####
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif

ifndef TAO_IDL
  TAO_IDL = $(TAO_ROOT)/TAO_IDL/tao_idl
endif


####
#### Build customization.
####
ifndef TAO_ORBSVCS
  #### Please see docs/configurations.html#orbsvcs for documentation of
  #### TAO_ORBSVCS.
  TAO_ORBSVCS = Naming \
                Time \
                Log \
                Concurrency \
                LifeCycle \
                ImplRepo \
                Property \
                Trader \
                Sched \
                Event \
                CosEvent \
                AV \
		#Event2
endif # TAO_ORBSVCS


####
#### Macro customization.
####
ifeq ($(findstring g++,$(CXX)),g++)
  ifneq ($(findstring egcs-2.91,$(shell $(CXX) --version)),)

    #### egcs 2.91.57 seems to go overboard:
    ####   DynAny_i.cpp:565: warning: `class TAO_DynAny_i * this' might be
    ####   used uninitialized in this function
    CCFLAGS += -Wno-uninitialized
  endif # egcs 2.91.x
endif # g++


####
#### Build rules.
####
.SUFFIXES:      S.h C.h S.i C.i S.cpp C.cpp

%S.h %Ser.h %C.h %Cli.h %S.i %Ser.i  %C.i %Cli.i %S.cpp %Ser.cpp %C.cpp %Cli.cpp:       %.idl $(TAO_IDL)
	$(TAO_IDL) $(TAO_IDLFLAGS) $<
