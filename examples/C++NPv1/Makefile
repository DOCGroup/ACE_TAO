#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the example programs in "C++ Network Programming,
#	Vol. 1, Resolving Complexity Using Patterns and ACE"
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

EXAMPLES = Logging_Client \
       Iterative_Logging_Server \
       Process_Per_Connection_Logging_Server \
       Thread_Per_Connection_Logging_Server

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
