eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
# This file is for running the run_test.pl scripts listed in 
# auto_run_tests.lst.

use English;
use Getopt::Std;

if (!getopts ('ac:s:t') || $opt_h) {
    print "run_test.pl [-a] [-c config] [-h] [-s sandbox] [-t]\n";
    print "\n";
    print "Runs the tests listed in auto_run_tests.lst\n";
    print "\n";
    print "Options:\n";
    print "    -a          ACE tests only\n";
    print "    -c config   Run the tests for the <config> configuration\n";
    print "    -h          display this help\n";
    print "    -s sandbox  Runs each program using a sandbox program\n";
    print "    -t          TAO tests only\n";
    print "\n";
    print "Configs: MSVC\n";
    exit (1);
}

use Cwd;

if (!($ACE_ROOT = $ENV{ACE_ROOT})) {
    my $cd = getcwd ();
    chdir ('..');
    $ACE_ROOT = getcwd ().$DIR_SEPARATOR;
    chdir ($cd);
    warn "ACE_ROOT not defined, defaulting to ACE_ROOT=$ACE_ROOT";
}

unshift @INC, "$ACE_ROOT/bin";
require ACEutils;

$config = 'DEFAULT';

if ($opt_c) {
    $config = $opt_c;
}
else {
    print "Defaulting to configuration: $config\n";
}


################################################################################

@tests = ();

# Loads the list and runs it
sub read_run_test_list ()
{
    my $line;
    open (LIST, "<$ACE_ROOT/bin/auto_run_tests.lst") 
        || die "Cannot open $ACE_ROOT/bin/auto_run_tests.lst";

    while (<LIST>) {
        next if (/^#/ || /^\s*$/);  # ignore comments and blank lines

        chomp;
        my @stuff = split ":";

        if (!/$config/) {
            my $program = shift @stuff;
            $program =~ s/\s*//g;  #remove any extra whitepace
            push @tests, $program;
        }
    }   

    close (LIST);
}

################################################################################

read_run_test_list ();

foreach $test (@tests) {
    my $directory = ".";
    my $program = ".";

    if ($opt_a && $test =~ /^TAO/) {
        next;
    }
    elsif ($opt_t && $test !~ /^TAO/) {
        next;
    }

    if ($test =~ /(.*)\/([^\/]*)$/) {
        $directory = $1;
        $program = $2;
    }
    else {
        $program = $test;
    }

    print "auto_run_tests: $test\n";

    chdir ($ACE_ROOT."/$directory")
        || die "Error: Cannot chdir to $ACE_ROOT/$directory";

    $run_error = 0;

    if (! -e $program) {
        print STDERR "Error: $test does not exist\n";
        next;
    }

    my $result = 0;

    if ($opt_s) {
        $result = system ("$opt_s \"perl $program\"");
    } 
    else { 
        $result = system ($program);
    }

    if ($result > 0) {
        print "Error: $test returned with status $result\n";
    }
}
