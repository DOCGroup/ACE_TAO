eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
#   Fuzz is a script whose purpose is to check through ACE/TAO files for
#   easy to spot (by a perl script, at least) problems.

use File::Find;
use Getopt::Std;

###### TODO
#
# Add tests for these:
#
# - LPTSTR, TCHAR, etc. in any file
# - Using TAO_TRY macros instead of ACE_TRY
# - no relative path to tao_idl in the .dsp files
# - Linking to wrong type of library in dsp's
# - not setting up the release configs correctly in dsp files
# - files not ending with newline
# - Guards in .h files
# - no global functions
# - other commit_check checks, missing LF at eof, tabs, trailing spaces.
#
# And others in ACE_Guidelines and Design Rules
#
# Also add a -g flag to ignore tao_idl generated files
#
###### END TODO

# Lists of all the files
@files_cpp = ();
@files_inl = ();
@files_h = ();
@files_dsp = ();


##############################################################################

# Find_Modified_Files will use 'cvs -nq' to get a list of locally modified 
# files to look through
sub find_mod_files ()
{
    unless (open (CVS, "cvs -nq up |")) {
        print STDERR "Error: Could not run cvs\n";
        exit (1);
    }

    while (<CVS>) {
        if (/^[M|A] (.*)/) {
            store_file ($1);
        }
    }
    close (CVS);
}



# Find_Files will search for files with certain extensions in the 
# directory tree
sub find_files ()
{
    # wanted is only used for the File::Find
    sub wanted
    {
        store_file ($File::Find::name);
    }

    find (\&wanted, '.');
}

# 
sub store_file ($)
{
    my $name = shift;
    if ($name =~ /\.cpp$/i) {
        push @files_cpp, ($name);
    }
    elsif ($name =~ /\.(inl|i)$/i) {
        push @files_inl, ($name);
    }
    elsif ($name =~ /\.h$/i) {
        push @files_h, ($name);
    }
    elsif ($name =~ /\.dsp$/i) {
        push @files_dsp, ($name);
    }
}


##############################################################################

# The point of this test is to check for the existence of ACE_INLINE
# or ASYS_INLINE in a .cpp file.  This is most commonly caused by 
# copy/pasted code from a .inl/.i file
sub check_for_inline_in_cpp ()
{
    print "Running ACE_INLINE/ASYS_INLINE check\n";
    foreach $file (@files_cpp) {
        my $line = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/^ACE_INLINE/) {
                    print "Error: ACE_INLINE found in $file on line $line\n";
                }
                if (/^ASYS_INLINE/) {
                    print "Error: ASYS_INLINE found in $file on line $line\n";
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}

# This test checks to make sure files have the $Id$ string in them.
# Commit_check should find these when checking in files, but this can
# be used locally or to check for files 
sub check_for_id_string ()
{
    print "Running \$ID check\n";
    foreach $file (@files_cpp, @files_inl, @files_h) {
        my $found = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (/\$Id/) {
                    $found = 1;
                }
            }
            close (FILE);
            if ($found == 0) {
                print "Error: No \$Id string found in $file\n";
            }
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# This test checks for the use of "inline" instead of ACE_INLINE
sub check_for_inline ()
{
    print "Running inline check\n";
    foreach $file (@files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/^[\s+]inline/) {
                    print "Error: inline found in $file on line $line\n";
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# This test checks for the inclusion of math.h.  math.h should be avoided
# since on some platforms, "exceptions" is defined as a struct, which will
# cause problems with exception handling
sub check_for_math_include ()
{
    print "Running math.h test\n";
    foreach $file (@files_h, @files_cpp, @files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/^\s*#\s*include\s*[\/\*\*\/]?\s*\<math\.h\>/) {
                    print "Error: math.h included in $file on line $line\n";
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}

# For general readability, lines should not contain more than 80 characters
sub check_for_line_length ()
{
    print "Running line length test\n";
    foreach $file (@files_h, @files_cpp, @files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/.{80,}?/) {
                    print "Error: Over 80 chars on line $line in $file\n";
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# For preprocessor directives, only the old C style comments (/* */) 
# should be used, not the newer // style.  
sub check_for_preprocessor_comments ()
{
    print "Running preprocessor comment test\n";
    foreach $file (@files_h, @files_cpp, @files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/^\#.*\/\//) {
                    print "Error: C++ comment in directive on "
                          . "line $line in $file\n";
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


##############################################################################

our ($opt_d, $opt_h, $opt_l, $opt_m);

if (!getopts ('dhl:m') || $opt_h) {
    print "fuzz.pl [-dhm] [-l level]\n";
    print "\n";
    print "    -d         turn on debugging\n";
    print "    -h         display this help\n";
    print "    -l level   set detection level (default = 5)\n";
    print "    -m         only check locally modified files (uses cvs)\n";
    exit (1);
}

if (!$opt_l) {
    $opt_l = 5;
}

if ($opt_m) {
    find_mod_files ();
}
else {
    find_files ();
}

check_for_inline_in_cpp () if ($opt_l >= 2);
check_for_id_string () if ($opt_l >= 2);
check_for_inline () if ($opt_l >= 2);
check_for_math_include () if ($opt_l >= 3);
check_for_line_length () if ($opt_l >= 6);
check_for_preprocessor_comments () if ($opt_l >= 3);
