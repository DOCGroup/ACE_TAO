#----------------------------------------------------------------------------
#	$Id$
#
#	Makefile for the Reactor Server Logging Daemon
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= server

FILES 	= 

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	=  $(addprefix .shobj/,$(SHOBJ)) 

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VBIN)

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

# In order to make the generation of HTML pages from sourcecode
# easier, I've started putting the comments into *.pre and *.pst files.
# Those are then combined (via the perl script "combine") with the
# source code to create the HTMLs.  In an effort to declutter the
# directory, I archive the component files in a shell-archive (eg --
# shar) file that is commited to the repository.

# Invoke the combine script to pull together the pre-code comments,
# code and post-code comments that makeup a tutorial page.  'combine'
# keys itself off of the *.pre files to know what it should build.  An
# accessory file "bodies" specifies which source files comprise the
# body of each tutorial page.
HTML : #
	[ -f hdr ] || $(MAKE) UNSHAR
	perl ../combine *.pre ; chmod +r *.html

# The SHAR target simply invokes "shar" to create the shell archive.
# It is important to include all "component" files in the shar command
# line so that they will be included in the archive.  It is not
# necessary to include the source code files since they're commited as-is.
SHAR : #
	[ ! -f combine.shar ] || exit 1
	shar -T hdr bodies *.pre *.pst > combine.shar && $(RM) hdr bodies *.pre *.pst

# For orthogonality, we have an UNSHAR to match SHAR.
UNSHAR : #
	sh combine.shar

CLEAN : realclean
	$(RM) hdr bodies *.pre *.pst .depend

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

.obj/server.o .shobj/server.so: server.cpp acceptor.h logger.h

