eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Make the build output prettier.

use Getopt::Std;
use English;
use FileHandle;
use strict;

use vars qw/$opt_b $opt_c $opt_h $opt_i $opt_m $opt_o $opt_p $opt_t/;

if (!getopts ('bc:hi:m:o:pt') || $opt_h) {
    print "Output Beautifier\n";
    print "\n";
    print "make_pretty.pl [-bpthr] [-c compiler] [-m msg] [-i file] [-o file]\n";
    print "    -b           Brief-ify the output (only errors and warnings)\n";
    print "    -c compiler  Specify the compiler type [msvc/makefile]\n";
    print "    -h           Show this help output\n";
    print "    -i file      Use file as input (defaults to stdin)\n";
    print "    -m           Message to attach to the end of the output\n";
    print "    -o file      Use file as output (defaults to stdout)\n";
    print "    -p           Plain text mode (default is html)\n";
    print "    -t           Only give totals for warnings and errors (auto -b)\n";
    exit (1);
}

if (!defined $opt_c) {
  $opt_c = 'makefile';
  $opt_c = 'msvc' if ($OSNAME eq 'Win32');
}

if ($opt_c ne 'makefile' && $opt_c ne 'msvc') {
    print STDERR "Error: Must specify msvc or makefile as the compiler type\n";
    exit (1);
}

my $input;
my $output;

if (!defined $opt_i) {
    $input = *STDIN;
}
else {
    $input = new FileHandle ("$opt_i", "r");
    if (!defined $input) {
        print STDERR "Error: Could not open file <$opt_i> for input\n";
        exit 1;
    }
 }

if (!defined $opt_o) {
    $output = *STDOUT;
}
else {
    $output = new FileHandle ("$opt_o", "w");
    if (!defined $output) {
        print STDERR "Error: Could not open file <$opt_o> for output\n";
        exit 1;
    }
 }

$opt_b = 1 if ($opt_t);

my $header;
my $trailer;
my $bookmark_table_header;
my $bookmark_table_config;
my $bookmark_table_cvs;
my $bookmark_table_compiler;
my $bookmark_table_tests;
my $bookmark_table_footer;
my $bookmark_config;
my $bookmark_cvs;
my $bookmark_compiler;
my $bookmark_tests;
my $pre_section_title;
my $post_section_title;
my $pre_error;
my $post_error;
my $pre_warning;
my $post_warning;
my $pre_config;
my $post_config;
my $line_break;
my $separator;

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

    $bookmark_table_header = "<hr>".
                             "<h2>Quick links</h2>".
                             "<ul>";

    $bookmark_table_config   = "<li><a href=\"#config\">Config</a>";
    $bookmark_table_cvs      = "<li><a href=\"#cvs\">CVS</a>";
    $bookmark_table_compiler = "<li><a href=\"#compiler\">Compilation</a>";
    $bookmark_table_tests    = "<li><a href=\"#tests\">Tests</a>";
                      
    $bookmark_table_footer = "</ul>";

    $bookmark_config = "<a name=\"config\"></a>";
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

    $bookmark_table_header = "";
    $bookmark_table_config = "";
    $bookmark_table_cvs = "";
    $bookmark_table_compiler = "";
    $bookmark_table_tests = "";
    $bookmark_table_footer = "";

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
    return 3 if (m/^ld: \d+\-\d+ WARNING: Duplicate symbol:/);

    # HP-UX uses 'nocompatwarnings' as an option to the compiler.
    return 3 if (m/vnocompatwarnings/);

    # SUN CC 5.0 defines __pthread_cleanup_push as a macro which causes
    # warnings. See /usr/include/pthread.h and
    # $ACE_ROOT/examples/Timer_Queue/Thread_Timer_Queue_Test.cpp for more
    # information.
    return 0 if (m/in call to __pthread_cleanup_push\(extern/);

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
      return 1;
    }

    # IBM's compilers don't say the word "warning" - check for their code
    return 1 if (m/^.*\d+\-\d+:? \(W\)/);

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
    return 0 if (m/^ld: \d+\-\d+ WARNING: Duplicate symbol:/);

    # Linux has this annoying mktemp, mkstemp stuff. Ignore that
    # for the timebeing
    return 0 if (/is dangerous, better use/);

    # Look for lines that also should be color coded, but not counted
    # as errors.
    return 2 if (/Types pointed to are unrelated/
                 || /while compiling class-template/
                 || /requires an explicit cast/);

    if (/^.*:[0-9]+: /
        && !/^.*:[0-9]+: warning:/) {
      return 1;
    }

    # AIX linking errors from ld
    # But don't report the extra "check the map" message
    return 0 if (/^ld: 0711\-345/);
    return 1 if (/^ld: \d+\-\d+/);

    # Look for linking errors too
    return 1 if (/undefined reference to/
                 || /: cannot open/
                 || /: multiple definition of/
                 || /path name does not exist/);

    # Look for possible errors
    return 1 if ((/error/i && !/ error\(s\), / && !/error \(future\)/i)
                 || /^Fatal\:/
                 || /: fatal:/);

    # Again, IBM's compilers speak in code langauge
    if (m/.*\d+\-\d+:? \([SI]\)/) {
        # Ignore licensing messages
        return 3 if (/.*Compilation will proceed shortly./);
        return 1;
    }

    # didn't find anything
    return 0;
}

sub print_build_header ($$)
{
    my $directory = shift;
    my $config = shift;

    print $output "$separator$line_break";
    print $output "$pre_config$directory$post_config$line_break\n";
    print $output "$pre_config$config$post_config$line_break$line_break";
}

################################################################################
sub config_output (@)
{
    if (!$opt_t) {
        print $output $separator;
        print $output $bookmark_config;
        print $output $pre_section_title."Configuration".$post_section_title;
    }
    
    foreach (@_) {
        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        if (m/^\s*MAKE_PRETTY_COMPILER\:\s*(.*)\s*$/) {
            $opt_c = $1;
        }
        elsif (!$opt_b) {
            print $output "$_$line_break";
        }
    }
}

sub cvs_output (@)
{
    my $patched = 0;
    my $updated = 0;
    my $modified = 0;
    my $conflicts = 0;
    my $unknown = 0;

    if (!$opt_t) {
        print $output $separator;
        print $output $bookmark_cvs;
        print $output $pre_section_title."CVS".$post_section_title;
    }

    foreach (@_) {
        my $error = 0;

        ++$patched if (/^P /);
        ++$updated if (/^U /);
        ++$modified if (/^M /);
        ++$conflicts if (/^C /);
        ++$unknown if (/^\? /);

        ### Isn't really a conflict, but easiest place to put it.
        if (/aborted/i || /cannot access/i || /no such file/i) {
            $error = 1;
            ++$conflicts;
        }
        
        if ($opt_t) {
        }
        elsif ($error) {
            print $output "$pre_error$_$post_error$line_break";
        }
        elsif (/^C /) {
            print $output "$pre_error$_$post_error$line_break";
        }
        elsif  (/^M /) {
            print $output "$pre_warning$_$post_warning$line_break";
        }
        elsif (!$opt_b) {
            print $output "$_$line_break";
        }
    }

    print $output $line_break;
    print $output "\nCVS Totals: ";
    print $output " Patched: $patched  Updated:$updated  Modified: $modified ".
                  " Conflicts: $conflicts  Unknown: $unknown\n\n$line_break";
}

sub compiler_output (@)
{
    makefile_compiler_output (@_) if ($opt_c eq 'makefile');
    msvc_compiler_output (@_) if ($opt_c eq 'msvc');
}

sub msvc_compiler_output (@)
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
        print $output $separator;
        print $output $bookmark_compiler;
        print $output $pre_section_title."Compilation".$post_section_title;
    }

    foreach (@_) {
        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        my $is_warning = is_warning ();
        my $is_error = is_error ();

        if (/^Auto_compiling (.*)/) {
            print $output $separator, $line_break, $pre_config, $_, $post_config, $line_break
                if (!$opt_b && !$opt_t);
            $dsp = $1;
            $first_problem = 1;
        }
        elsif (/^--------------------Configuration: (.*) - (.*)--------------------/) {
            print $output $pre_config.$_.$post_config.$line_break
                if (!$opt_b && !$opt_t);
            $project = $1;
            $configuration = $2;
            $first_problem = 1;
        }
        elsif (/\- (.*) error\(s\)\, (.*) warning\(s\)/) {
            my $errors = $1 - $bogus_errors;
            my $warnings = $2 - $bogus_warnings;
            print $output "$project - $errors error(s), $warnings warnings(s)$line_break\n"
                if (!$opt_b || ($opt_t && ($errors > 0 || $warnings > 0)));
            $total_errors = $total_errors + $errors;
            $total_warnings = $total_warnings + $warnings;

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
                print $output $pre_error, $_, $post_error, $line_break;
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
                print $output $pre_warning, $_, $post_warning, $line_break;
            }
        }
        else
        {
            print $output $_, $line_break
                if (!$opt_b && !$opt_t);
        }
    }
    print $output $line_break;
    print $output "\nCompiler Totals: ";
    print $output " Errors: $total_errors  Warnings: $total_warnings\n\n$line_break";
}

sub makefile_compiler_output (@)
{
    use vars qw/$directory $total_warnings $total_errors/;
    use vars qw/$project_warnings $project_errors/;
    local $directory = 'NULL';
    my $first_problem = 1;
    local $total_warnings = 0;
    local $total_errors = 0;
    local $project_warnings = 0;
    local $project_errors = 0;

    if (!$opt_t) {
        print $output $separator;
        print $output $bookmark_compiler;
        print $output $pre_section_title."Compilation".$post_section_title;
    }

    sub print_summary ()
    {
        print $output $_.$line_break if (!$opt_b);
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

    foreach (@_) {
        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        my $is_warning = is_warning ();
        my $is_error = is_error ();

        if (/Entering directory (.*)/) {
            print_summary () if ($directory ne 'NULL');

            print $output $pre_config, $1, $post_config, "\n", $line_break
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
                print $output $pre_error, $_, $post_error, $line_break;
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
                print $output $pre_warning, $_, $post_warning, $line_break;
            }
            if ($is_warning == 1) {
                ++$project_warnings;
            }
        }
        else {
            print $output $_, $line_break
                if (!$opt_b && !$opt_t);
        }
    }
    print_summary () if ($directory ne 'NULL');
    print $output $line_break;
    print $output "\nCompiler Totals: ";
    print $output " Errors: $total_errors  Warnings: $total_warnings\n\n$line_break";
}

sub tests_output (@)
{
    use vars qw/$first_problem $run_test $tests_failed/;
    local $first_problem = 1;
    local $run_test = 'NULL';
    local $tests_failed = 0;

    sub check_result ()
    {
        if ($first_problem == 0) {
            if ($opt_t) {
                print $output $pre_error."Failure in $run_test\n".$post_error.$line_break;
            }
            ++$tests_failed;
        }
    }

    if (!$opt_t) {
        print $output $separator;
        print $output $bookmark_tests;
        print $output $pre_section_title."Tests".$post_section_title;
    }

    foreach (@_) {
        s/</&lt;/g if !$opt_p;
        s/>/&gt;/g if !$opt_p;

        if (/auto_run_tests: (.*)/) {
            check_result ();

            print $output $pre_config.$1.$post_config."\n".$line_break
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
                print $output "$pre_error$_$post_error$line_break";
            }
            $first_problem = 0;
        }
        else
        {
            print $output "$_$line_break"
                if (!$opt_b && !$opt_t);
        }
    }
    check_result ();

    print $output $line_break;
    print $output "\nTest Failures: $tests_failed\n\n$line_break";
}

##############################################################################
if ($opt_p) {
    set_text_output ();
}
else {
    set_html_output ();
}

my @configdata;
my @cvsdata;
my @compiledata;
my @testdata;

my %statemap = (nowhere     => -1,
                unknown     => -2,
                cvsdata     => 1,
                configdata  => 2,
                compiledata => 3,
                testdata    => 4);
             
my $state = 0;

my $endfound = 1;
my $multiplelogs = 0;

# Read in the input

while (<$input>) {
    if (m/^#################### CVS/) {
        $state = %statemap->{cvsdata};
    }
    elsif (m/^#################### Config/) {
        $state = %statemap->{configdata};
    }
    elsif (m/^#################### Compile/) {
        $state = %statemap->{compiledata};
    }
    elsif (m/^#################### Tests/) {
        $state = %statemap->{testdata};
    }
    elsif (m/^#################### End/) {
        $state = %statemap->{nowhere};
        $endfound++;
    }
    elsif (m/^####################/) {
        $state = %statemap->{unknown};
    }
    
    # if we find that we are starting a new log in the
    # same file, delete the 
    if ($state != %statemap->{nowhere} && $endfound > 0) {
        $endfound = 0;
        @cvsdata = ();
        @configdata = ();
        @compiledata = ();
        @testdata = ();
    }
    
    next if (m/^####################/);
        
    if ($state == %statemap->{cvsdata}) {  #CVS
        push @cvsdata, $_;
    }
    elsif ($state == %statemap->{configdata}) {  #Config
        push @configdata, $_;
    }
    elsif ($state == %statemap->{compiledata}) {  #Compile
        push @compiledata, $_;
    }
    elsif ($state == %statemap->{testdata}) {  #Test
        push @testdata, $_;
    }
}

print $output $header;
print $output $bookmark_table_header;
print $output $bookmark_table_config   if ($#configdata >= 0);
print $output $bookmark_table_cvs      if ($#cvsdata >= 0); 
print $output $bookmark_table_compiler if ($#compiledata >= 0);
print $output $bookmark_table_tests    if ($#testdata >= 0);
print $output $bookmark_table_footer;

config_output (@configdata)    if ($#configdata >= 0);
cvs_output (@cvsdata)          ;  # CVS Section isn't optional
compiler_output (@compiledata) if ($#compiledata >= 0);
tests_output (@testdata)       if ($#testdata >= 0);

print $output "$line_break\n\n$opt_m\n\n$line_break" if $opt_m;
print $output $trailer;
