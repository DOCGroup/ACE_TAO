eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
# This file is for running the run_test.pl scripts listed in
# auto_run_tests.lst.

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

use English;
use Getopt::Std;
use Cwd;

use Env qw(ACE_ROOT PATH);

################################################################################
if (!getopts ('ac:dos:t') || $opt_h) {
    print "auto_run_tests.pl [-a] [-c config] [-h] [-s sandbox] [-o] [-t]\n";
    print "\n";
    print "Runs the tests listed in auto_run_tests.lst\n";
    print "\n";
    print "Options:\n";
    print "    -a          ACE tests only\n";
    print "    -c config   Run the tests for the <config> configuration\n";
    print "    -h          display this help\n";
    print "    -s sandbox  Runs each program using a sandbox program\n";
    print "    -o          ORB test only\n";
    print "    -t          TAO tests (other than ORB tests) only\n";
    print "\n";
    print "Configs: " . $config_list->list_configs () . "\n";
    exit (1);
}

my @file_list;

if ($opt_a) {
push (@file_list, "/bin/ace_tests.lst");
}

if ($opt_o) {
push (@file_list, "/bin/tao_orb_tests.lst");
}

if ($opt_t) {
push (@file_list, "/bin/tao_other_tests.lst");
}

if (scalar(@file_list) == 0) {
push (@file_list, "/bin/ace_tests.lst");
push (@file_list, "/bin/tao_orb_tests.lst");
push (@file_list, "/bin/tao_other_tests.lst");
}

foreach my$test_lst (@file_list) {

my $config_list = new PerlACE::ConfigList;

$config_list->load ($ACE_ROOT.$test_lst);

# Insures that we search for stuff in the current directory.
$PATH .= $Config::Config{path_sep} . '.';

foreach $test ($config_list->valid_entries ()) {
    my $directory = ".";
    my $program = ".";

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

    if ($program =~ /(.*?) (.*)/)
      {
        if (! -e $1)
          {
            print STDERR "Error: $directory.$1 does not exist\n";
            next;
          }
      }
    else
      {
        if (! -e $program)
          {
            print STDERR "Error: $directory.$program does not exist\n";
            next;
          }
      }

    ### Genrate the -ExeSubDir and -Config options
    my $inherited_options = " -ExeSubDir $PerlACE::Process::ExeSubDir ";

    foreach my $config ($config_list->my_config_list ()) {
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
}
