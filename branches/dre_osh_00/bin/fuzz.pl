eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
#   Fuzz is a script whose purpose is to check through ACE/TAO files for
#   easy to spot (by a perl script, at least) problems.

use File::Find;
use File::Basename;
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
# - _narrow() should always have ACE_ENV_ARG_PARAMETER
# - Using ACE_TRY_NEW_ENV (Nanbor suggests using ACE_DECLARE_NEW_CORBA_ENV)
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
@files_changelog = ();

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
    elsif ($name =~ /ChangeLog/i && -f $name) {
        push @files_changelog, ($name);
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
    print "Running \$Id\$ string check\n";
    foreach $file (@files_cpp, @files_inl, @files_h,
                   @files_html, @files_idl, @files_pl) {
        my $found = 0;
        if (open (FILE, $file)) {
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (/\$Id\:/ or /\$Id\$/) {
                    $found = 1;
                    last;
                }
            }
            close (FILE);
            if ($found == 0) {
                print_error ("No \$Id\$ string found in $file");
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

# This test verifies that the same number of "#pragma warning(push)" and
# "#pragma warning(pop)" pragmas are used in a given header.
sub check_for_push_and_pop ()
{
    print "Running #pragma (push)/(pop) test\n";
    foreach $file (@files_h) {
        my $push_count = 0;
        my $pop_count = 0;
        if (open (FILE, $file)) {
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (/FUZZ\: disable check_for_push_and_pop/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_push_and_pop/) {
                    $disable = 0;
                }
                if ($disable == 0) {
                    if (/^\s*#\s*pragma\s*warning\s*\(\s*push[,1-4]*\s*\)/) {
                        ++$push_count;
                    }
                    if (/^\s*#\s*pragma\s*warning\s*\(\s*pop\s*\)/) {
                        ++$pop_count;
                    }
                }
            }
            close (FILE);

            if ($disable == 0 && $push_count != $pop_count) {
	        print_error ("#pragma warning(push)/(pop) mismatch in $file");
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
    foreach $file (@files_h, @files_cpp, @files_inl, @files_idl) {
        if (open (FILE, $file)) {
            my $disable = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                if (m/\@file\s*([^\s]*)/){
					# $file includes complete path, $1 is the name after
					# @file. We must strip the complete path from $file.
					# we do that using the basename function from
					# File::BaseName
					$filename = basename($file,"");
					if (!($filename eq $1)){
                       print_error ("\@file mismatch in $file, found $1");
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

# check for bad run_test
sub check_for_bad_run_test ()
{
    print "Running run_test.pl test\n";
    foreach $file (@files_pl) {
        if (open (FILE, $file)) {
            my $is_run_test = 0;
            my $line = 0;
            my $sub = 0;

            print "Looking at file $file\n" if $opt_d;

            while (<FILE>) {
                ++$line;

                if (m/PerlACE/ || m/ACEutils/) {
                    $is_run_test = 1;
                }

                if ($is_run_test == 1) {
                    if (m/ACEutils/) {
                        print_error ("ACEutils.pm still being used in $file ($line)");
                    }

                    if (m/unshift \@INC/) {
                        print_error ("Still unshifting \@INC, should \"use lib\""
                                     ." instead in $file ($line)");
                    }

                    if (m/\$EXEPREFIX/) {
                        print_error ("Still using \$EXEPREFIX in $file ($line)");
                    }

                    if (m/\$EXE_EXT/) {
                        print_error ("Still using \$EXE_EXT in $file ($line)");
                    }

                    if (m/\$DIR_SEPARATOR/) {
                        print_error ("Still using \$DIR_SEPARATOR in $file ($line)");
                    }
                    if (m/ACE\:\:/ && !m/PerlACE\:\:/) {
                        print_error ("Still using ACE::* in $file ($line)");
                    }

                    if (m/Process\:\:/ && !m/PerlACE\:\:Process\:\:/) {
                        print_error ("Still using Process::* in $file ($line)");
                    }

                    if (m/Process\:\:Create/) {
                        print_error ("Still using Process::Create in $file ($line)");
                    }

                    if ((m/\.ior/ || m/\.conf/) && !m/LocalFile/) {
                        print_error ("Not using PerlACE::LocalFile at $file ($line)");
                    }

                    if (m/^  [^ ]/) {
                        print_warning ("Still using two-space indentation in $file ($line)");
                    }

                    if (m/^\s*\t/) {
                        print_error ("Indenting using tabs in $file ($line)");
                    }

                    if (m/^\s*\{/ && $sub != 1) {
                        print_warning ("Using Curly Brace alone on $file ($line)");
                    }

                    if (m/timedout/i && !m/\#/) {
                        print_error ("timedout message found on $file ($line)");
                    }

                    if (m/^\s*sub/) {
                        $sub = 1;
                    }
                    else {
                        $sub = 0;
                    }
                }
            }

            close (FILE);

            if ($is_run_test) {
                my @output = `perl -wc $file 2>&1`;

                foreach $output (@output) {
                    chomp $output;
                    if ($output =~ m/error/i) {
                        print_error ($output);
                    }
                    elsif ($output !~ m/syntax OK/) {
                        print_warning ($output);
                    }
                }
            }
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}


# Check for links to ~schmidt/ACE_wrappers/, which should not be in the
# documentation
sub check_for_absolute_ace_wrappers()
{
    print "Running absolute ACE_wrappers test\n";
    foreach $file (@files_html) {
        if (open (FILE, $file)) {
            my $line = 0;
            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (m/\~schmidt\/ACE_wrappers\//) {
                    chomp;
                    print_error ("~schmidt/ACE_wrappers found in $file on "
                                 . "line $line");
                    print_error ($_) if (defined $opt_v);
                }
            }
            close (FILE);
        }
        else {
            print STDERR "Error: Could not open $file\n";
        }
    }
}

# Make sure ACE_[OS_]TRACE matches the function/method
sub check_for_bad_ace_trace()
{
    print "Running TRACE test\n";
    foreach $file (@files_inl, @files_cpp) {
        if (open (FILE, $file)) {
            my $line = 0;
            my $class;
            my $function;

            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;

                # look for methods or functions
                if (m/(^[^\s][^\(]*)\:\:([^\:^\(]*[^\s^\(])\s*/) {
                    $class = $1;
                    $function = $2;
                }
                elsif (m/^([^\s^\(^\#]*) \(/i) {
                    $class = "";
                    $function = $1;
                }
                elsif (m/^(operator.*) \(/i) {
                    $class = "";
                    $function = $1;
                }

                # Look for TRACE statements
                if (m/ACE_OS_TRACE\s*\(\s*\"(.*)\"/
                    || m/ACE_TRACE\s*\(\s*\"(.*)\"/) {
                    my $trace = $1;

                    # reduce the classname
                    if ($class =~ m/([^\s][^\<^\s]*)\s*\</) {
                        $class = $1;
                    }

                    if ($class =~ m/([^\s^\&^\*]*)\s*$/) {
                        $class = $1;
                    }

                    if ($trace !~ m/\Q$function\E/
                        || ($trace =~ m/\:\:/ && !($trace =~ m/\Q$class\E/ && $trace =~ m/\Q$function\E/))) {
                        print_error ("Mismatched TRACE in $file on line $line");
                        print_error ("   I see \"$trace\" but I think I'm in \""
                                     . $class . "::" . $function . "\"") if (defined $opt_v);
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



# This test checks missing ACE_ENV_ARG_PARAMETER when using
# resolve_initial_references
sub check_for_missing_rir_env ()
{
    print "Running resolve_initial_references() check\n";
    foreach $file (@files_cpp, @files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            my $disable = 0;
            my $native_try = 0;
            my $in_rir = 0;
            my $found_env = 0;

            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/FUZZ\: disable check_for_missing_rir_env/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_missing_rir_env/) {
                    $disable = 0;
                }
                if ($disable == 0) {
                    next if m/^\s*\/\//;

                    if (m/^\s*try/) {
                        $disable = 1;
                        next;
                    }

                    if (m/[^\:]resolve_initial_references\s*\(/) {
                        $found_env = 0;
                        $in_rir = 1;
                    }

                    if (m/ACE_ENV_ARG_PARAMETER/) {
                        $found_env = 1;
                    }

                    if ($in_rir == 1 && m/\;\s*$/) {
                        $in_rir = 0;
                        if ($found_env != 1) {
                            print_error ("Missing ACE_ENV_ARG_PARAMETER in"
                                         . " resolve_initial_references"
                                         . " in $file ($line)");
                        }
                        $found_env = 0;
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

# This test checks for usage of ACE_CHECK/ACE_TRY_CHECK
sub check_for_ace_check ()
{
    print "Running ACE_CHECK check\n";
    foreach $file (@files_cpp, @files_inl) {
        my $line = 0;
        if (open (FILE, $file)) {
            my $disable = 0;
            my $in_func = 0;
            my $in_return = 0;
            my $found_env = 0;

            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;
                if (/FUZZ\: disable check_for_ace_check/) {
                    $disable = 1;
                }
                if (/FUZZ\: enable check_for_ace_check/) {
                    $disable = 0;
                }

                if (/FUZZ\: ignore check_for_ace_check/) {
                    next;
                }

                next if m/^\s*\/\//;
                next if m/^\s*$/;

                if ($disable == 0) {
                    if (m/[,\(]\s*ACE_TRY_ENV[,\)]/) {
                        $found_env = 1;
                        $in_func = 1;
                        $env_line = $line;
                    }

                    # ignore quoted ACE_TRY_ENV's
                    if (m/^[^\"]*\"[^\"]*ACE_TRY_ENV[^\"]*\"[^\"]*$/) {
                        $found_env = 0;
                    }

                    if (m/ACE_TRY_ENV.*ACE_TRY_ENV/) {
                        print_error ("Multiple ACE_TRY_ENV in $file ($line)");
                    }

                    if ($in_func && m/\)/) {
                        $in_func = 0;
                    }
                    elsif (!$in_func && $found_env) {
                        if (!m/_CHECK/ && !m/^\}/ && !$in_return) {
                            print_error ("Missing ACE_CHECK/ACE_TRY_CHECK for $file ($env_line)");
                        }
                        $found_env = 0;
                    }

                    if (m/^\s*return/) {
                        $in_return = 1;
                    }
                    else {
                        $in_return = 0;
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

# This test checks for broken ChangeLog entries.
sub check_for_changelog_errors ()
{
    print "Running ChangeLog check\n";
    foreach $file (@files_changelog) {
        my $line = 0;
        if (open (FILE, $file)) {
            my $found_backslash = 0;
            my $found_cvs_conflict = 0;

            print "Looking at file $file\n" if $opt_d;
            while (<FILE>) {
                ++$line;

                next if m/^\s*\/\//;
                next if m/^\s*$/;

		# Check for backslashes in paths.
                if (m/\*.*\\[^ ]*:/) {
                    print_error ("Backslashes in file path - $file ($line)");
                }

                # Check for CVS conflict tags
                if (m/^<<<<</ || m/^=====/ || m/^>>>>>/) {
                    print_error ("CVS conflict markers in $file ($line)");
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

use vars qw/$opt_c $opt_d $opt_h $opt_l $opt_m $opt_v/;

if (!getopts ('cdhl:mv') || $opt_h) {
    print "fuzz.pl [-cdhm] [-l level] [file1, file2, ...]\n";
    print "\n";
    print "    -c         only look at the files passed in\n";
    print "    -d         turn on debugging\n";
    print "    -h         display this help\n";
    print "    -l level   set detection level (default = 5)\n";
    print "    -m         only check locally modified files (uses cvs)\n";
    print "    -v         verbose mode\n";
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
check_for_push_and_pop () if ($opt_l >= 4);
check_for_mismatched_filename () if ($opt_l >= 2);
check_for_bad_run_test () if ($opt_l >= 6);
check_for_absolute_ace_wrappers () if ($opt_l >= 3);
check_for_bad_ace_trace () if ($opt_l >= 4);
check_for_missing_rir_env () if ($opt_l >= 5);
check_for_ace_check () if ($opt_l >= 3);
check_for_changelog_errors () if ($opt_l >= 4);

print "\nFuzz.pl - $errors error(s), $warnings warning(s)\n";

exit (1) if $errors > 0;
