#----------------------------------------------------------------------------
#
# $Id$
#
#	Makefile for C++NPv2 example programs.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

EXAMPLES = Reactor_Logging_Server \
           Select_Reactor_Logging_Server \
           TP_Reactor_Logging_Server \
           Configurable_Logging_Server \
           SLD \
           SLDex \
           CLD \
           TPLS \
           TPCLS \
           SR_Configurable_Logging_Server \
           AC_CLD \
           display_logfile

all:
	@for ex in $(EXAMPLES); \
	do \
		$(MAKE) -f $$ex.mak; \
	done

clean:
	@for ex in $(EXAMPLES); \
	do \
		$(MAKE) -f $$ex.mak clean; \
	done

realclean:
	@for ex in $(EXAMPLES); \
	do \
		$(MAKE) -f $$ex.mak realclean; \
	done
