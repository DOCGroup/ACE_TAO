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
# - Using TAO_TRY macros instead of ACE_TRY
# - no relative path to tao_idl in the .dsp files
# - Linking to wrong type of library in dsp's
# - not setting up the release configs correctly in dsp files
# - Guards in .h files
# - no global functions
# - other commit_check checks, tabs, trailing spaces.
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
@files_html = ();
@files_dsp = ();
@files_idl = ();
@files_pl = ();

# To keep track of errors and warnings
$errors = 0;
$warnings = 0;

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
    if ($name =~ /\.(c|cc|cpp|cxx)$/i) {
        push @files_cpp, ($name);
    }
    elsif ($name =~ /\.(inl|i)$/i) {
        push @files_inl, ($name);
    }
    elsif ($name =~ /\.(h|hh|hpp|hxx)$/i) {
        push @files_h, ($name);
    }
    elsif ($name =~ /\.(htm|html)$/i) {
        push @files_html, ($name);
    }
    elsif ($name =~ /\.(dsp|vcp)$/i) {
        push @files_dsp, ($name);
    }
    elsif ($name =~ /\.(pidl|idl)$/i) {
        push @files_idl, ($name);
    }
    elsif ($name =~ /\.pl$/i) {
        push @files_pl, ($name);
    }
}

##############################################################################
## Just messages

sub print_error ($)
{
    my $msg = shift;
    print "Error: $msg\n";
    ++$errors;
}


sub print_warning ($)
{
    my $msg = shift;
    print "Warning: $msg\n";
    ++$warnings;
}


##############################################################################
## Tests

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
                    print_error ("ACE_INLINE found in $file on line $line");
                }
                if (/^ASYS_INLINE/) {
                    print_error ("ASYS_INLINE found in $file on line $line");
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}

# This test checks to make sure files have the $Id string in them.
# Commit_check should find these when checking in files, but this can
# be used locally or to check for files 
sub check_for_id_string ()
{
    print "Running \$ID\$ string check\n";
    foreach $file (@files_cpp, @files_inl, @files_h, 
                   @files_html, @files_idl, @files_pl) {
        my $found = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (/\$Id\:/ or /\$Id\$/) {
                    $found = 1;
                }
            }
            close (FILE);
            if ($found == 0) {
                print_error ("No \$Id string found in $file");
            }
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# This test checks for the newline at the end of a file
sub check_for_newline ()
{
    print "Running newline check\n";
    foreach $file (@files_cpp, @files_inl, @files_h, 
                   @files_html, @files_idl, @files_pl) {
        if (open (FILE, $file)) {
            my $line;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                $line = $_
            }
            close (FILE);
            if ($line !~ /\n$/) {
                print_error ("No ending newline found in $file");
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
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/FUZZ\: disable check_for_inline/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_inline/) {
                    $disable = 0;
                }
                if ($disable == 0 and m/^\s*inline/) {
                    print_error ("inline found in $file on line $line");
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
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/FUZZ\: disable check_for_math_include/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_math_include/) {
                    $disable = 0;
                }
                if ($disable == 0 
                    and /^\s*#\s*include\s*(\/\*\*\/){0,1}\s*\<math\.h\>/) {
                    print_error ("math.h included in $file on line $line");
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

                # Make sure to ignore ACE_RCSID lines, since they
                # are difficult to get under 80 chars.
                if (/.{80,}/ and !/^ACE_RCSID/) {
                    print_error ("Over 80 chars on line $line in $file");
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
                    print_error ("C++ comment in directive on "
                                 ."line $line in $file");
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# This test checks for the use of the Win32 Unicode string defines
# or outdated ASYS_* macros
# We should only be using the ACE_TCHAR, ACE_TEXT macros instead.
sub check_for_tchar
{
    print "Running TCHAR test\n";
    foreach $file (@files_h, @files_cpp, @files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/FUZZ\: disable check_for_tchar/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_tchar/) {
                    $disable = 0;
                }
                if ($disable == 0) {
                    if (/LPTSTR/) {
                        print_error ("LPTSTR found on line $line in $file");
                    }
    
                    if (/LPCTSTR/) {
                        print_error ("LPCTSTR found on line $line in $file");
                    }
    
                    if (/ASYS_TCHAR/) {
                        print_error ("ASYS_TCHAR found on "
                                     ."line $line in $file");
                    }
                    elsif (/TCHAR/ and !/ACE_TCHAR/) {
                        ### Do a double check, since some macros do have TCHAR
                        ### (like DEFAULTCHARS)
                        if (/^TCHAR[^\w_]/ or /[^\w_]TCHAR[^\w_]/) {
                            print_error ("TCHAR on line $line in $file");
                        }
                    }
    
                    if (/ASYS_TEXT/) {
                        print_error ("ASYS_TEXT on line $line in $file");
                    }
                    elsif (/TEXT/ and !/ACE_TEXT/) {
                        ### Do a double check, since there are several macros
                        ### that end with TEXT
                        if (/^TEXT\s*\(/ or /[^\w_]TEXT\s*\(/) {
	                    print_error ("TEXT found on line $line in $file");
                        }
                    }
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}



# This checks to make sure files include ace/post.h if ace/pre.h is included
# and vice versa.
sub check_for_pre_and_post ()
{
    print "Running pre.h/post.h test\n";
    foreach $file (@files_h) {
        my $pre = 0;
        my $post = 0;
        if (open (FILE, $file)) {
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (/FUZZ\: disable check_for_pre_and_post/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_pre_and_post/) {
                    $disable = 0;
                }
                if ($disable == 0) {
                    if (/^\s*#\s*include\s*\"ace\/pre\.h\"/) {
                        ++$pre;
                    }
                    if (/^\s*#\s*include\s*\"ace\/post\.h\"/) {
                        ++$post;
                    }
                }
            }
            close (FILE);

            if ($disable == 0 && $pre != $post) {
                print_error ("pre.h/post.h mismatch in $file");
            }
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# Check doxygen @file comments
sub check_for_mismatched_filename () 
{
    print "Running doxygen \@file test\n";
    foreach $file (@files_h, @files_cpp, @files_inl) {
        if (open (FILE, $file)) {
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (m/\@file\s*([^\s]*)/ && $file !~ m/$1$/) {
                    print_error ("\@file mismatch in $file");
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}

# check for 
sub check_for_old_run_test ()
{
    print "Running old run_test.pl test\n";
    foreach $file (@files_pl) {
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (m/ACEutils/) {
                    print_error ("ACEutils.pm still being used in $file");
                }
                
                if (m/unshift \@INC/) {
                    print_error ("Still unshifting \@INC, should \"use lib\""
                                 ." instead in $file");
                }
                
                if (m/ACE\:\:/ && !m/PerlACE\:\:/) {
                    print_error ("Still using ACE::* in $file");
                }

                if (m/Process\:\:/ && !m/PerlACE\:\:Process\:\:/) {
                    print_error ("Still using Process::* in $file");
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

#our ($opt_c, $opt_d, $opt_h, $opt_l, $opt_m);

if (!getopts ('cdhl:m') || $opt_h) {
    print "fuzz.pl [-cdhm] [-l level] [file1, file2, ...]\n";
    print "\n";
    print "    -c         only look at the files passed in\n";
    print "    -d         turn on debugging\n";
    print "    -h         display this help\n";
    print "    -l level   set detection level (default = 5)\n";
    print "    -m         only check locally modified files (uses cvs)\n";
    exit (1);
}

if (!$opt_l) {
    $opt_l = 5;
}

if ($opt_c) {
    foreach $file (@ARGV) {
        store_file ($file);
    }   
}
elsif ($opt_m) {
    find_mod_files ();
}
else {
    find_files ();
}

print "--------------------Configuration: Fuzz - Level ",$opt_l,
      "--------------------\n";

check_for_inline_in_cpp () if ($opt_l >= 2);
check_for_id_string () if ($opt_l >= 1);
check_for_newline () if ($opt_l >= 1);
check_for_inline () if ($opt_l >= 2);
check_for_math_include () if ($opt_l >= 3);
check_for_line_length () if ($opt_l >= 8);
check_for_preprocessor_comments () if ($opt_l >= 7);
check_for_tchar () if ($opt_l >= 4);
check_for_pre_and_post () if ($opt_l >= 4);
check_for_mismatched_filename () if ($opt_l >= 2);
check_for_old_run_test () if ($opt_l >= 6);

print "\nFuzz.pl - $errors error(s), $warnings warning(s)\n";

exit (1) if $errors > 0;
