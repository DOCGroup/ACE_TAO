#----------------------------------------------------------------------------
#	$Id$
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

# You can generally find a Makefile in the ACE examples, tests or the library
# itself that will satisfy our application needs.  This one was taken from
# one of the examples.

	# Define the name of the binary we want to create.  There has to be
	# a CPP file $(BIN).cpp but it doesn't necessarily have to have your
	# main() in it.  Most of the time, though, it will.
BIN    = server

	# Few applications will have a single source file.  We use the FILES
	# macro to build up a list of additional files to compile.  Notice
	# that we leave off the extension just as with BIN
FILES  = 
FILES += client_handler
FILES += client_acceptor
FILES += thread_pool

	# The BUILD macro is used by the ACE makefiles.  Basically, it tells
	# the system what to build.  I don't really know what VBIN is other
	# than it is constructed from the value of BIN.  Just go with it...
BUILD	= $(VBIN)

	# Here we use some GNU make extensions to build the SRC macro. Basically,
	# we're just adding .cpp to the value of BIN and for each entry of the
	# FILES macro.
SRC = $(addsuffix .cpp,$(BIN)) $(addsuffix .cpp,$(FILES))

	# This is used by my Indent target below.  It's not a part of standard
	# ACE and you don't need it yourself.
HDR = *.h

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

	# This is where the real power lies!  These included makefile components
	# are similar to the C++ templates in ACE.  That is, they do a tremendous
	# amount of work for you and all you have to do is include them.
	# As a matter of fact, in our project, I created a single file named
	# "app.mk" that includes all of these.  Our project makefiles then just
	# need to include app.mk to get everything they need.

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

	# Sometimes I like to reformat my code to make it more readable.  This is
	# more useful for the comments than anything else.  Unfortunately, the
	# "indent" program doesn't quite grok C++ so I have to post-process it's
	# output just a bit.
Indent : #
	for i in $(SRC) $(HDR) ; do \
		indent -npsl -l80 -fca -fc1 -cli0 -cdb < $$i | \
			sed -e 's/: :/::/g'  \
			    -e 's/^.*\(public:\)/\1/' \
			    -e 's/^.*\(protected:\)/\1/' \
			    -e 's/^.*\(private:\)/\1/' \
			    -e 's/:\(public\)/ : \1/' \
			    -e 's/:\(protected\)/ : \1/' \
			    -e 's/:\(private\)/ : \1/' \
			> $$i~ ;\
		mv $$i~ $$i ;\
	done

	# One of the targets in the ACE makefiles is "depend".  It will invoke
	# your compiler in a way that will generate a list of dependencies for
	# you.  This is a great thing!  Unfortunately, it puts all of that mess
	# directly into the Makefile.  I prefer my Makefile to stay clean and
	# uncluttered.  The perl script referenced here pulls the dependency
	# stuff back out of the Makefile and into a file ".depend" which we then
	# include just like the makefile components above.
Depend : depend
	perl ../fix.Makefile

.depend : #
	touch .depend


HTML : #
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre ; chmod +r *.html

SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr bodies *.pre *.pst > combine.shar && $(RM) hdr bodies *.pre *.pst

UNSHAR : #
	sh combine.shar

CLEAN : realclean
	$(RM) hdr bodies *.pre *.pst .depend

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

	# Don't put anything below here.  Between the "depend" target and fix.Makefile
	# it's guaranteed to be lost!

	# This is inserted by the fix.Makefile script
include .depend
