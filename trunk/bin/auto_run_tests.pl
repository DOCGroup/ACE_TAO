eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
# This file is for running the run_test.pl scripts listed in 
# auto_run_tests.lst.

if (!($ACE_ROOT = $ENV{ACE_ROOT})) {
    my $cd = getcwd ();
    chdir ('..');
    $ACE_ROOT = getcwd ().$DIR_SEPARATOR;
    chdir ($cd);
    warn "ACE_ROOT not defined, defaulting to ACE_ROOT=$ACE_ROOT";
}

unshift @INC, "$ACE_ROOT/bin";
require ACEutils;
require PerlACE::ConfigList;

use English;
use Getopt::Std;
use Cwd;

################################################################################

if (!getopts ('ac:ds:t') || $opt_h) {
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

$config_list = new PerlACE::ConfigList;

$config_list->load ($ACE_ROOT."/bin/auto_run_tests.lst");

if ($#CONFIGS < 0) {
    print "Warning: No configurations selected, defaulting to none.\n";
    print "         Possible Configs: ", $config_list->list_configs (), "\n";
}

foreach $test ($config_list->valid_entries ()) {
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

    if (! -e $program) {
        print STDERR "Error: $test does not exist\n";
        next;
    }

    ### Genrate the -ExeSubDir and -Config options
    my $inherited_options = " -ExeSubDir $EXEPREFIX ";

    foreach my $config (@CONFIGS) {
         $inherited_options .= " -Config $config ";
    }

    $cmd = '';
    if ($opt_s) {
        $cmd = "$opt_s \"perl $program $inherited_options\"";
    } 
    else { 
        $cmd = $program.$inherited_options;
    }


    my $result = 0;

    if (defined $opt_d) {
        print "Running: $cmd\n";
    }
    else {
        $result = system ($cmd);
    }

    if ($result > 0) {
        print "Error: $test returned with status $result\n";
    }
}
