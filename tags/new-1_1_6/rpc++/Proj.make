# These are included in every Makefile in the project

CWDPATH = .
CLEANWILDCARDS = core *~ *.o
VERYCLEANWILDCARDS = core *~ *.o
TEXCLEANWILDCARD = *~ *.aux *.log *.bbl *.blg *.toc *.idx *.ind
TEXVERYCLEANWILDCARD = $(TEXCLEANWILDCARD) *.dvi
ifndef SUBDIRS
SUBDIRS =
endif

# Doing all always means doing the subdirs.
# Make subdirs.all a target to allow forced processing
ifneq ("$(SUBDIRS)", "")
ifneq ("$(SUBDIRSALL)", "NO")
all:: subdirs.all
endif

subdirs.all::
	@for d in $(SUBDIRS); do \
	  (cd $$d; \
	   smflags=$(SUBMFLAGS); \
	   echo "Making all in $$d with flags: $$smflags ..." ; \
	   echo "cd `pwd`"; \
	   $(MAKE) $$smflags all); done; \
	echo "cd `pwd`"
else
all::;
endif

ifneq ("$(SUBDIRS)", "")
clean:: subdirs.clean

subdirs.clean::
	@for d in $(SUBDIRS); do \
	  echo "Cleaning all in $$d..." ; \
	  (cd $$d; $(MAKE) clean); done
else
clean::;
endif

ifneq ("$(SUBDIRS)", "")
veryclean:: subdirs.veryclean

subdirs.veryclean::
	@for d in $(SUBDIRS); do \
	  echo "Verycleaning all in $$d..." ; \
	  (cd $$d; $(MAKE) veryclean); done
else
veryclean::;
endif

# dist.list prints a list of files to be included in the distribution
distlist::
ifneq ("$(DISTLIST)", "")
	@for f in $(DISTLIST); do \
	  echo $(CWDPATH)/$$f; done
endif
ifneq ("$(SUBDIRS)", "")
ifneq ("$(SUBDIRSDIST)", "NO")
	@for d in $(SUBDIRS); do \
	  (cd $$d; $(MAKE) distlist CWDPATH=$(CWDPATH)/$$d); done
endif
endif
ifeq ("$(DISTLIST)$(SUBDIRS)","")
	
endif

