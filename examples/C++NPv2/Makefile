#----------------------------------------------------------------------------
#
# $Id$
#
#	Makefile for C++NPv2 example programs.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

EXAMPLES = Reactor_Logging_Server display_logfile

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
