eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Make the build output prettier.

use Getopt::Std;
use English;

if (!getopts ('bc:hm:pt') || $opt_h) {
    print "Output Beautifier\n";
    print "\n";
    print "make_pretty.pl [-bpthr] [-c compiler] [-m msg]\n";
    print "    -b           Brief-ify the output (only errors and warnings)\n";
    print "    -c compiler  Specify the compiler type [msvc (default)/makefile]\n";
    print "    -h           Show this help output\n";
    print "    -m           Message to attach to the end of the output\n";
    print "    -p           Plain text mode (default is html)\n";
    print "    -t           Only give totals for warnings and errors (auto -b)\n";
    exit (1);
}

if (!$opt_c) {
  $opt_c = 'makefile';
  $opt_c = 'msvc' if ($OSNAME eq 'Win32');
}

if ($opt_c ne 'makefile' && $opt_c ne 'msvc') {
    print STDERR "Error: Must specify msvc or makefile as the compiler type\n";
    exit (1);
}

$opt_b = 1 if ($opt_t);

###############################################################################
sub set_html_output ()
{
    $header = "<html>".
              "<head>".
              "<title>Error/Warning Summary of Daily Build</title></head>".
              "<body bgcolor=\"white\">".
              "<h1>Error/Warning Summary of Daily Build</h1>".
              "<TT>\n";

    $trailer = "</TT></body></html>";

    $bookmark_table = "<hr>".
                      "<h2>Quick links</h2>".
                      "<ul>".
                      "<li><a href=\"#cvs\">CVS</a>".
                      "<li><a href=\"#compiler\">Compilation</a>".
                      "<li><a href=\"#tests\">Tests</a>".
                      "</ul>";

    $bookmark_cvs = "<a name=\"cvs\"></a>";
    $bookmark_compiler = "<a name=\"compiler\"></a>";
    $bookmark_tests = "<a name=\"tests\"></a>";

    $pre_section_title = "<H2>";
    $post_section_title = "</H2>";

    $pre_error = "<FONT COLOR=\"FF0000\">";
    $post_error = "</FONT>";

    $pre_warning = "<FONT COLOR=\"FF7700\">";
    $post_warning = "</FONT>";

    $pre_config = "";
    $post_config = "";

    $line_break = "<BR>";
    $separator = "<HR>";
}

sub set_text_output ()
{
    $header = "" ;
    $trailer = "" ;

    $bookmark_table = "";

    $bookmark_cvs = "";
    $bookmark_compiler = "";
    $bookmark_tests = "";

    $pre_section_title = "\n================================================".
                         "================================\n";
    $post_section_title = "\n================================================".
                          "================================\n";

    $pre_error = "";
    $post_error = "";

    $pre_warning = "";
    $post_warning = "";

    $pre_config = "";
    $post_config = "";

    $line_break = "";
    $separator = "------------------------------------------------".
                 "--------------------------------\n";
}

# is_warning checks for warnings.  It returns
# 0 = no warning
# 1 = warning line
# 2 = possible warning
# 3 = false warning
sub is_warning ()
{
    # Look for any warnings we should ignore
    return 3 if (/^LINK : warning LNK4089:/);

    # For some reason, there's a false license warning
    return 3 if (/\(W\).*Compilation will proceed shortly./);

    # AIX reports a bazillion multiple defines when doing templates.
    return 3 if ($^O eq 'aix'
                 && m/^ld: \d+-\d+ WARNING: Duplicate symbol:/);

    # Look for lines that also should be color coded, but not counted
    # as warnings.
    return 2 if (/see declaration of/);

    # Look for warnings

    return 1 if ((/warning/i && !/ warning\(s\)/)
                 || /info: /i
                 || /^make.*\*\*\*/
                 || /^error \(future\)/i);

    if (/^.*\.h: /
        || /^.*\.i: /
        || /^.*\.inl: /
        || /^.*\.cpp: /
        || /^.*\.java: /) {
      return 1 if ($^O ne 'aix');
    }

    # IBM's compilers don't say the word "warning" - check for their code
    return 1 if ($^O eq 'aix' && m/\d+-\d+ \(W\)/);

    # didn't find anything
    return 0;
}

# is_error checks for errors. It returns
# 0 = no error
# 1 = error line
# 2 = possible error
# 3 = false errors
sub is_error ()
{
    # Look for any errors we should ignore

    # AIX reports a bazillion multiple defines when doing templates; some
    # have the word 'error' in the symbol name - ignore those.
    return 0 if ($^O eq 'aix'
                 && m/^ld: \d+-\d+ WARNING: Duplicate symbol:/);

    # Look for lines that also should be color coded, but not counted
    # as errors.
    return 2 if (/Types pointed to are unrelated/
                 || /while compiling class-template/
                 || /requires an explicit cast/);

    if (/^.*:[0-9]+: /
        && !/^.*:[0-9]+: warning:/) {
      return 1;
    }

    # Look for linking errors too
    return 1 if (/undefined reference to/
                 || /: cannot open/
                 || /: multiple definition of/);

    # Look for possible errors
    return 1 if ((/error/i && !/ error\(s\), / && !/error \(future\)/i)
                 || /^Fatal\:/
                 || /: fatal:/);

    # Again, IBM's compilers speak in code langauge
    return 1 if ($^O eq 'aix' && m/\d+-\d+ \([SI]\)/);

    # didn't find anything
    return 0;
}

sub print_build_header ($$)
{
    my $directory = shift;
    my $config = shift;

    print "$separator$line_break";
    print "$pre_config$directory$post_config$line_break\n";
    print "$pre_config$config$post_config$line_break$line_break";
}

################################################################################
sub cvs_output ()
{
    my $patched = 0;
    my $updated = 0;
    my $modified = 0;
    my $conflicts = 0;
    my $unknown = 0;

    if (!$opt_t) {
        print $separator;
        print $bookmark_cvs;
        print $pre_section_title."CVS".$post_section_title;
    }

    LOOP: while (<>) {
        ++$patched if (/^P /);
        ++$updated if (/^U /);
        ++$modified if (/^M /);
        ++$conflicts if (/^C /);
        ++$unknown if (/^\? /);

        ### Isn't really a conflict, but easiest place to put it.
        ++$conflicts if (/aborted/);

        last LOOP if (/^####################/);

        if ($opt_t) {
        }
        elsif (/aborted/) {
            print "$pre_error$_$post_error$line_break";
        }
        elsif (/^C /) {
            print "$pre_error$_$post_error$line_break";
        }
        elsif  (/^M /) {
            print "$pre_warning$_$post_warning$line_break";
        }
        elsif (!$opt_b) {
            print "$_$line_break";
        }
    }

    print $line_break;
    print "\nCVS Totals: ";
    print " Patched: $patched  Updated:$updated  Modified: $modified ".
          " Conflicts: $conflicts Unknown: $unknown\n\n$line_break";
}

sub msvc_compiler_output ()
{
    my $project = "NULL";
    my $configuration = "NULL";
    my $dsp = "NULL";
    my $first_problem = 1;
    my $total_warnings = 0;
    my $total_errors = 0;
    my $bogus_warnings = 0;
    my $bogus_errors = 0;

    if (!$opt_t) {
        print $separator;
        print $bookmark_compiler;
        print $pre_section_title."Compilation".$post_section_title;
    }

    LOOP: while (<>)
    {
        last LOOP if (/^####################/);

        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        my $is_warning = is_warning ();
        my $is_error = is_error ();

        if (/^Auto_compiling (.*)/) {
            print "$separator$line_break$pre_config$_$post_config$line_break"
                if (!$opt_b && !$opt_t);
            $dsp = $1;
            $first_problem = 1;
        }
        elsif (/^--------------------Configuration: (.*) - (.*)--------------------/) {
            print $pre_config.$_.$post_config.$line_break
                if (!$opt_b && !$opt_t);
            $project = $1;
            $configuration = $2;
            $first_problem = 1;
        }
        elsif (/\- (.*) error\(s\)\, (.*) warning\(s\)/) {
            my $errors = $1 - $bogus_errors;
            my $warnings = $2 - $bogus_warnings;
            print "$project - $errors error(s), $warnings warnings(s)$line_break\n"
                if (!$opt_b || ($opt_t && ($errors > 0 || $warnings > 0)));
            $total_errors = $total_errors + $errors;
            $total_warnings = $total_warnings +$warnings;

            $bogus_warnings = 0;
            $bogus_errors = 0;
        }
        elsif ($is_error == 3) {
            ++$bogus_errors;
        }
        elsif ($is_error == 2 || $is_error == 1) {
            if (!$opt_t) {
                print_build_header ($dsp, "$project: $configuration")
                    if ($first_problem && $opt_b);
                $first_problem = 0;
                print "$pre_error$_$post_error$line_break";
            }
        }
        elsif ($is_warning == 3) {
            ++$bogus_warnings;
        }
        elsif ($is_warning == 2 || $is_warning == 1) {
            if (!$opt_t) {
                print_build_header ($dsp, "$project: $configuration")
                    if ($first_problem && $opt_b);
                $first_problem = 0;
                print "$pre_warning$_$post_warning$line_break";
            }
        }
        else
        {
            print "$_$line_break"
                if (!$opt_b && !$opt_t);
        }
    }
    print $line_break;
    print "\nCompiler Totals: ";
    print " Errors: $total_errors  Warnings: $total_warnings\n\n$line_break";
}

sub makefile_compiler_output ()
{
    local $directory = 'NULL';
    my $first_problem = 1;
    local $total_warnings = 0;
    local $total_errors = 0;
    local $project_warnings = 0;
    local $project_errors = 0;

    if (!$opt_t) {
        print $separator;
        print $bookmark_compiler;
        print $pre_section_title."Compilation".$post_section_title;
    }

    sub print_summary ()
    {
        print $_.$line_break if (!$opt_b);
        $total_errors = $total_errors + $project_errors;
        $total_warnings = $total_warnings + $project_warnings;
        $project_errors = 0;
        $project_warnings = 0;
        $directory = 'NULL';
    }

    sub set_directory ($)
    {
        my $dir = shift;

        # Strip off the ACE_wrappers stuff
        if ($dir =~ /ACE_wrappers\/build\/[^\/]*\/(.*)/) {
            $dir = $1;
        }
        elsif ($dir =~ /ACE_wrappers\/(.*)/) {
            $dir = $1;
        }

        # Strip of trailing '

        if ($dir =~ /(.*)\'$/) {
            $dir = $1;
        }

        $directory = $dir;
    }

    LOOP: while (<>)
    {
        last LOOP if (/^####################/);

        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        my $is_warning = is_warning ();
        my $is_error = is_error ();

        if (/Entering directory (.*)/) {
            print_summary () if ($directory ne 'NULL');

            print $pre_config.$1.$post_config."\n".$line_break
                if (!$opt_b && !$opt_t);
            set_directory ($1);
            $first_problem = 1;

        }
        elsif ($is_error == 3) {
            # Do Nothing
        }
        elsif ($is_error == 2 || $is_error == 1) {
            if (!$opt_t) {
                print_build_header ($directory, "")
                    if ($first_problem && $opt_b);
                $first_problem = 0;
                print "$pre_error$_$post_error$line_break";
            }
            if ($is_error == 1) {
                ++$project_errors;
            }
        }
        elsif ($is_warning == 3) {
            # Do Nothing
        }
        elsif ($is_warning == 2 || $is_warning == 1) {
            if (!$opt_t) {
                print_build_header ($directory, "")
                    if ($first_problem && $opt_b);
                $first_problem = 0;
                print "$pre_warning$_$post_warning$line_break";
            }
            if ($is_warning == 1) {
                ++$project_warnings;
            }
        }
        else
        {
            print "$_$line_break"
                if (!$opt_b && !$opt_t);
        }
    }
    print_summary () if ($directory ne 'NULL');
    print $line_break;
    print "\nCompiler Totals: ";
    print " Errors: $total_errors  Warnings: $total_warnings\n\n$line_break";
}

sub tests_output ()
{
    local $first_problem = 1;
    local $run_test = 'NULL';
    local $tests_failed = 0;

    sub check_result ()
    {
        if ($first_problem == 0) {
            if ($opt_t) {
                print $pre_error."Failure in $run_test\n".$post_error.$line_break;
            }
            ++$tests_failed;
        }
    }

    if (!$opt_t) {
        print $separator;
        print $bookmark_tests;
        print $pre_section_title."Tests".$post_section_title;
    }

    LOOP: while (<>) {
        last LOOP if (/^####################/);

        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        if (/auto_run_tests: (.*)/) {
            check_result ();

            print $pre_config.$1.$post_config."\n".$line_break
                if (!$opt_b && !$opt_t);
            $run_test = $1;
            $first_problem = 1;
        }
        elsif (m/Error/
               || m/ERROR/
               || m/FAILED/
               || m/EXCEPTION/
               || m/pure virtual /i) {
            if (!$opt_t) {
                print_build_header ($run_test, "") if ($first_problem && $opt_b);
                print "$pre_error$_$post_error$line_break";
            }
            $first_problem = 0;
        }
        else
        {
            print "$_$line_break"
                if (!$opt_b && !$opt_t);
        }
    }
    check_result ();

    print $line_break;
    print "\nTest Failures: $tests_failed\n\n$line_break";
}

##############################################################################
if ($opt_p) {
    set_text_output ();
}
else {
    set_html_output ();
}

print $header;
print $bookmark_table;

LOOP: while (<>) {
   last LOOP if (/^####################/);
}

cvs_output ();

makefile_compiler_output () if ($opt_c eq 'makefile');
msvc_compiler_output () if ($opt_c eq 'msvc');

tests_output ();

print "$line_break\n\n$opt_m\n\n$line_break" if $opt_m;
print $trailer;
