
# These rules define default C++, C++FLAGS and C++SUFFIX.
# C++ is the C++ compiler to use; C++FLAGS are command-line
# flags to the C++ compiler for use in compiling C++ source
# files into objects; C++SUFFIX is the filename suffix
# indicating C++ source. By default, it's set to "C" for AT&T C++,
# and "cc" for GNU g++.

# Feel free to override these in your Makefiles *after*
# including this file.

# IMPORTANT: If the -M switch is specified in IDLFLAGS, the IDL
# compiler appends to the user-specified file. The dependency for
# specified_file.o in the linking target should appear _after_ any use
# of the IDL compiler which takes the -M flag.  Putting it _last_
# is normally the best approach.

# Note that these rule definitions use pattern matching,
# and therefore only work with SunOS make and GNU make.

# They may not work with other vendor's versions of make.
# If they do not, you may wish to try using GNU make, which
# is free software produced by the Free Software Foundation.

# If the version of make you wish to use does not support
# pattern matching, use the sample explicit rule set in
# the comments at the end of this file.

# ------------------------------------------------------------
# Essential: set these to the locations into which you
#		have installed Orbix' components:

ORBIX_BINDIR    = $(ORBIX_ROOT)/bin
ORBIX_LIBDIR    = $(ORBIX_ROOT)/lib
ORBIX_INCDIR    = $(ORBIX_ROOT)/include

C++             = CC
C++FLAGS 	= -I$(ORBIX_INCDIR)
C++SUFFIX       = cpp

LDFLAGS         = -L$(ORBIX_LIBDIR) -R $(ORBIX_LIBDIR) -lnsl -lsocket

IDL             = $(ORBIX_BINDIR)/idl 

IDLFLAGS =


# ------------------------------------------------------------
# The following section defines implicit rules for creating
# *.{client,server}.C files, rules for compiling those
# into objects, and even a rule for compiling C++ source into
# objects (in case one isn't already defined).

# ------------------------------------------------------------
# first, put the new suffixes at the *head* of the suffix list,
# overriding any existing .C compilation method.
.SUFFIXES:
.SUFFIXES: .$(C++SUFFIX) .idl $(SUFFIXES)

# .SUFFIXES: .$(C++SUFFIX) .idl .hh $(SUFFIXES)
# ------------------------------------------------------------
# *[CS].o must be compiled here, and depends
# mostly on the C++ files produced from the IDL file.

%C.o: %C.$(C++SUFFIX)
	$(C++) -c $(C++FLAGS) $<

%S.o: %S.$(C++SUFFIX)
	$(C++) -c $(C++FLAGS) $<

%.o: %.$(C++SUFFIX)
	$(C++) -c $(C++FLAGS) $<

# and here's how to compile C++ files from the IDL file.
# only ONE of these rules will be run at make-time,

%S.$(C++SUFFIX): %.idl
	$(IDL) $(IDLFLAGS) $<

%C.$(C++SUFFIX): %.idl
	$(IDL) $(IDLFLAGS) $<

%.hh: %.idl
	$(IDL) $(IDLFLAGS) $<

