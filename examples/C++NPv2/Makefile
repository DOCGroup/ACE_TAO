#----------------------------------------------------------------------------
#
# $Id$
#
#	Makefile for C++NPv2 example programs.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

# These examples don't require SSL support... those that do are below.
EXAMPLES = Reactor_Logging_Server \
           Select_Reactor_Logging_Server \
           TP_Reactor_Logging_Server \
           Configurable_Logging_Server \
           SLD \
           SLDex \
           CLD \
           TPLS \
           SR_Configurable_Logging_Server \
           display_logfile

ifneq ($(ssl),)
  EXAMPLES += AC_CLD  AIO_CLD  TPCLS
endif # ssl

all:
	@for ex in $(EXAMPLES); \
	do \
		$(MAKE) -f Makefile.$$ex; \
	done

clean:
	@for ex in $(EXAMPLES); \
	do \
		$(MAKE) -f Makefile.$$ex clean; \
	done

realclean:
	@for ex in $(EXAMPLES); \
	do \
		$(MAKE) -f Makefile.$$ex realclean; \
	done
