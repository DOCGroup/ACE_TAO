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
           AIO_CLD \
           display_logfile

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
