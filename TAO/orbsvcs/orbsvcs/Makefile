#----------------------------------------------------------------------------
#
# $Id$
#
#----------------------------------------------------------------------------
ifndef TAO_ROOT
  TAO_ROOT = $(ACE_ROOT)/TAO
endif # ! TAO_ROOT

MAKEFILE = Makefile

#----------------------------------------------------------------------------
#       Define local macros
#----------------------------------------------------------------------------

ifndef TAO_ORBSVCS
  #### Please see docs/configurations.html#orbsvcs for documentation of
  #### TAO_ORBSVCS.
  TAO_ORBSVCS = Naming \
                Time \
		LoadBalancing \
                Property \
                Sched \
                Event \
                CosEvent \
		Concurrency

# Old versions of 
  ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
    TAO_ORBSVCS += Event2 \
          NativeCosEvent
  endif # ACE_HAS_GNUG_PRE_2_8

  ifeq ($(minimum_corba),0)

    # Trader uses DynAny which is disabled when compiling with minimum
    # CORBA support.  LifeCycle, Log, AV and Notify depend on Trader
    # so don't build them either.
    TAO_ORBSVCS += Trader LifeCycle Log AV

    ifneq ($(ACE_HAS_GNUG_PRE_2_8),1)
      TAO_ORBSVCS += Notify
    endif # ACE_HAS_GNUG_PRE_2_8

  endif # minimum_corba
endif # TAO_ORBSVCS

# The order in this list is important as some libraries depend on
# others, please keep that in mind when adding or changing libraries.
MKLIST = Makefile.Svc_Utils
ifneq (,$(findstring Naming,$(TAO_ORBSVCS)))
  MKLIST += Makefile.CosNaming
endif
ifneq (,$(findstring  Time, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosTime
endif
ifneq (,$(findstring  LoadBalancing, $(TAO_ORBSVCS)))
  MKLIST += Makefile.LoadBalancing
endif
ifneq (,$(findstring  Concurrency, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosConcurrency
endif
ifneq (,$(findstring  Property, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosProperty
endif
ifneq (,$(findstring  LifeCycle, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosLifeCycle
endif
ifneq (,$(findstring  AV, $(TAO_ORBSVCS)))
  MKLIST += Makefile.av
endif
ifneq (,$(findstring  Sched, $(TAO_ORBSVCS)))
  MKLIST += Makefile.RTSched
endif
ifneq (,$(findstring  NativeCosEvent, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.CosEvent, $(MKLIST)))
    MKLIST += Makefile.CosEvent
  endif
endif
ifneq (,$(findstring  Event, $(TAO_ORBSVCS)))
  MKLIST += Makefile.RTEvent
endif
ifneq (,$(findstring  Event2, $(TAO_ORBSVCS)))
  ifeq (,$(findstring Makefile.RTEvent, $(MKLIST)))
    MKLIST += Makefile.RTEvent
  endif
endif
ifneq (,$(findstring  CosEvent, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosEvent
endif
ifneq (,$(findstring  Trader, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosTrading
endif
ifneq (,$(findstring  Notify, $(TAO_ORBSVCS)))
  MKLIST += Makefile.CosNotification
endif
ifneq (,$(findstring  Log, $(TAO_ORBSVCS)))
  MKLIST += Makefile.DsLogAdmin
endif

include $(TAO_ROOT)/rules.tao.GNU

default: all

#----------------------------------------------------------------------------
#       Include macros and targets
#----------------------------------------------------------------------------

include $(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include $(TAO_ROOT)/taoconfig.mk

#----------------------------------------------------------------------------
#       Local rules
#----------------------------------------------------------------------------

all: ORBSVCS_COMPONENTS

all debug profile optimize install deinstall clean realclean clobber depend rcs_info idl_stubs:
ifneq ($(MKLIST),)
  ifeq (Windows,$(findstring Windows,$(OS)))
	@cmd /c "FOR /D %m IN ($(MKLIST)) DO $(MAKE) -f %m MAKEFILE=$$m $(@:.nested=)"
  else  # ! Windows
	@for m in $(MKLIST); do \
	echo $$m; \
        $(MAKE) -f $$m MAKEFILE=$$m $(@:.nested=); \
        done
  endif # ! Windows
endif # MKLIST

.PHONY: ORBSVCS_COMPONENTS
ORBSVCS_COMPONENTS:
	@sh $(ACE_ROOT)/bin/ace_components --orbsvcs --set ' $(TAO_ORBSVCS) '

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------
