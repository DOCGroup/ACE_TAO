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
  #### The following default definition of TAO_ORBSVCS builds all of TAO's
  #### orbsvcs.  If you'd like to not build some components, then define
  #### your TAO_ORBSVCS appropriately.  You can do that either in:
  #### 1) your $(ACE_ROOT)/include/makeinclude/platform_macros.GNU file,
  #### 2) on the make command line, e.g.,
  ####        make TAO_ORBSVCS=Naming
  ####    or
  #### 3) by setting (and exporting) your TAO_ORBSVCS environment variable.
  ####
  #### NOTE: We currently don't check for interdependencies between
  #### services.  For example, if you build the CosEvent service, you
  #### must also explicitly specify the Sched and Event services, at least.
  ####
  #### NOTE: It would be nice to customize tmplinst-orbsvcs based on the
  #### value of TAO_ORBSVCS.
  ####
  #### NOTE: We currently don't check this macro in each of the orbsvcs
  #### Makefiles, or in their tests.  We'll add those checks soon.
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
                AV
endif # TAO_ORBSVCS


####
#### Macro customization.
####
ifeq ($(findstring g++,$(CXX)),g++)
  CCFLAGS += -Wno-unused

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
