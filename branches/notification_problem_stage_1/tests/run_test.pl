eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
# This file is for running the tests in the ACE tests directory.
# It is usually used for auto_compiles.

use lib '../bin';
use PerlACE::Run_Test;

use Cwd;
use English;
use Getopt::Std;
use FileHandle;

$config_list = new PerlACE::ConfigList;

################################################################################

sub check_for_more_configs ()
{
    my $fh = new FileHandle;

    if ($fh->open ("< ../ace/ACE_COMPONENTS.list")) {
        while (<$fh>) {
            if (m/ Other /) {
                print "Adding 'Other' as my config\n" if defined $opt_d;
                $config_list->add_one_config ('OTHER');
            }
            if (m/ Token /) {
                print "Adding 'Token' as my config\n" if defined $opt_d;
                $config_list->add_one_config ('TOKEN');
            }
        }

        $fh->close ();
    }
    elsif (defined $opt_d) {
       print "Could not open ACE_COMPONENTS.list file\n" if defined $opt_d;
       print "Assuming TOKEN and OTHER subsets are included\n" if defined $opt_d;
       $config_list->add_one_config ('OTHER');
       $config_list->add_one_config ('TOKEN');
    }

    my $P = new PerlACE::Process ("../netsvcs/servers/main");

    if (!-x $P->Executable ()) {
        $config_list->add_one_config ('missing_netsvcs');
    }

    if (defined $opt_v) {
      $config_list->add_one_config ('VxWorks');
    }
}

################################################################################

sub record_resources ()
{
    if ($config_list->check_config ('CHECK_RESOURCES')) {
        if (!defined $ENV{'LOGNAME'}) {
            $user=`whoami`;
        }
        else {
            $user = $ENV{'LOGNAME'};
        }

        $start_test_resources=`ipcs | egrep $user`;
    }
}

################################################################################

sub check_resources
{
    my($oh) = shift;
    if ($config_list->check_config ('CHECK_RESOURCES')) {
        if (defined $opt_v) {
            print $oh "memShow();\n";
        }
        else {
            $end_test_resources=`ipcs | egrep $user`;

            if ("$start_test_resources" ne "$end_test_resources") {
                print STDERR "Warning: the ACE tests _may_ have leaked OS ".
                             "resources!\n";
                print STDERR "Warning: Before: $start_test_resources\n";
                print STDERR "Warning: After:  $end_test_resources\n";
            }
        }
    }
}

################################################################################

sub run_program ($)
{
    my $program = shift;

    unlink <log/$program*.log>;
    unlink "core";

    my $P = new PerlACE::Process ($program);

    ### Try to run the program

    if (! -x $P->Executable ()) {
        print STDERR "Error: " . $P->Executable () .
                     " does not exist or is not runnable\n";
        return;
    }

    print STDERR "Running $program\n";
    $status = $P->SpawnWaitKill (300);

    ### Check for problems

    if ($status == -1) {
        print STDERR "Error: $program FAILED (time out)\n";
        $P->Kill ();
        $P->TimedWait (1);
    }
    elsif ($status != 0) {
        print STDERR "Error: $program FAILED with exit status $status\n";
    }

    check_log ($program);
}

################################################################################

sub output_vxworks_commands
{
  my($oh)      = shift;
  my($program) = shift;
  my($length)  = length($program) + 2;
  if ($config_list->check_config ('CHECK_RESOURCES')) {
    print $oh "memShow();\n";
  }
  print $oh "write(2, \"\\n$program\\n\", $length);\n" .
            "ld < $program\n" .
            "ace_main (0, 0);\n" .
            "unld \"$program\"\n";
}

################################################################################

sub purify_program ($)
{
    ### @todo

    my $program = shift;

    $program_exe = $program;

    print STDERR "Purifying $program\n";

    system ("purify ".
            "/run ".
            "/save-data=purify_results\$program.pfy ".
            "/save-text-data=purify_results\$program.txt ".
            "/AllocCallStackLength=20 ".
            "/ErrorCallStackLength=20 ".
            "/HandlesInUseAtExit ".
            "/InUseAtExit ".
            "/LeaksAtExit ".
            "$program_exe");
}

################################################################################

sub check_log ($)
{
    my $program = shift;

    ### Check the logs

    local $log = "log/".$program.".log";

    if (-e "core") {
        print STDERR "Error: $program dumped core\n";
        unlink "core";
    }

    if (! -e $log ) {
        print STDERR "Error: No log file ($log) is present\n";
    }
    else {
        if (open (LOG, "<".$log) == 0) {
            print STDERR "Error: Cannot open log file $log\n";
        }
        else {
            my $print_log = 0;
            my $starting_matched = 0;
            my $ending_matched = 0;

            while (<LOG>) {
                chomp;

                if (m/Starting/) {
                    $starting_matched = 1;
                }

                if (m/Ending/) {
                    $ending_matched = 1;
                }

                if (/LM\_ERROR\@(.*)$/) {
                    print STDERR "Error: ($log): $1\n";
                    $print_log = 1;
                }
                if (/LM\_WARNING\@(.*)$/) {
                    print STDERR "Warning: ($log): $1\n";
                    $print_log = 1;
                }
            }

            close (LOG); # ignore errors

            if ($starting_matched == 0) {
                print STDERR "Error ($log): no line with 'starting'\n";
                $print_log = 1;
            }

            if ($ending_matched == 0) {
                print STDERR "Error ($log): no line with 'Ending'\n";
                $print_log = 1;
            }

            if ($print_log == 1) {
                print STDERR "======= Begin Log File \n";
                if (open (LOG, "<".$log) == 0) {
                    print STDERR "Error: Cannot open log file $log\n";
                }
                else {
                    my @log = <LOG>;
                    print STDERR @log;
                    close (LOG);
                }
                print STDERR "======= End Log File \n";
            }

            # Now check for any sub-logs. If either the main log or a
            # sub-log has an error, print the sub-log.
            opendir (THISDIR, "log");
            local $sublognames = "$program\-.*\.log";
            @sublogs = grep (/$sublognames/, readdir (THISDIR));
            closedir (THISDIR);
            foreach $log (@sublogs) {
                # Just like the main log, but no start/end check
                if (open (LOG, "<log/".$log) == 0) {
                    print STDERR "Error: Cannot open sublog file $log\n";
                }
                else {
                    while (<LOG>) {
                        chomp;
                        if (/LM\_ERROR\@(.*)$/) {
                            print STDERR "Error: ($log): $1\n";
                            $print_log = 1;
                        }
                        if (/LM\_WARNING\@(.*)$/) {
                            print STDERR "Warning: ($log): $1\n";
                            $print_log = 1;
                        }
                    }

                    close (LOG); # ignore errors
                    if ($print_log == 1) {
                        print STDERR "======= Begin Sublog File ".$log."\n";
                        if (open (LOG, "<log/".$log) == 0) {
                            print STDERR "Error: Cannot open sublog file $log\n";
                        }
                        else {
                            my @log = <LOG>;
                            print STDERR @log;
                            close (LOG);
                        }
                        print STDERR "======= End Sublog File \n";
                    }
		}
            }
	}
    }
}

################################################################################

sub delete_temp_files ()
{
    my @files = ('ace_pipe_name', 'pattern');
    my $file = '';

    if (!opendir (DIR, $tmp)) {
        warn "Cannot open temp directory $tmp\n";
        return;
    }

    foreach $file (readdir (DIR)) {
      if ($file =~ /^ace_temp_file/ || $file =~ /^Naming_Test/) {
          push @files, $tmp . '/' . $file;
      }
    }
    closedir (DIR);

    PerlACE::check_n_cleanup_files ('MEM_Acceptor_*');
    PerlACE::check_n_cleanup_files ('backing_store_*');
}

################################################################################

$config_list->load ("run_test.lst");

if (!getopts ('dhtvo:') || $opt_h) {
    print "run_test.pl [-h] [-v] [-o <output file>] [-t file1 file2 ...]\n";
    print "\n";
    print "Runs the tests listed in run_test.lst\n";
    print "\n";
    print "Options:\n";
    print "    -d         Debug mode (do not run tests)\n";
    print "    -h         Display this help\n";
    print "    -t         Runs all the tests passed via the cmd line\n";
    print "    -v         Generate commands for VxWorks\n";
    print "    -o         Put VxWorks commands in output file\n";
    print "\n";
    print "Pass in configs using \"-Config XXXXX\"\n";
    print "\n";
    print "Possible Configs: CHECK_RESOURCES Purify",
           $config_list->list_configs (), "\n";
    exit (1);
}

## since we can't use "our" to get rid of warnings.
$opt_h = $opt_h if (defined $opt_h);
$opt_t = $opt_t if (defined $opt_t);

if (!($tmp = $ENV{TMP}) && !($tmp = $ENV{TEMP})) {
    $tmp="/tmp";
}

check_for_more_configs ();

@tests = ();

if (defined $opt_t) {
    @tests = @ARGV;
}
else {
    @tests = $config_list->valid_entries ();
}

if (defined $opt_d) {
    $config_list->dump ();
}

record_resources () if (!defined $opt_d);

my($oh) = \*STDOUT;
if (defined $opt_v && defined $opt_o) {
  $oh = new FileHandle();
  if (!open($oh, ">$opt_o")) {
    print STDERR "ERROR: Unable to write to $opt_o\n";
    exit(1);
  }
}

if (defined $opt_v) {
  print $oh "#\n" .
            "# ACE one-button test for VxWorks 5.x.\n" .
            "# To use:  -> < run_test.vxworks > run_test.log\n" .
            "#\n" .
            "# NOTE: if you build with a shared ACE library, be sure to load\n" .
            "# that first:\n" .
            "#  -> ld < ../ace/libACE.so\n" .
            "# and unld it after running the tests.\n" .
            "#\n" .
            "# The output logs can be checked from a Unix host:\n" .
            "#    % ./run_tests.check log/*.log\n\n";
}

foreach $test (@tests) {
    if (defined $opt_d) {
        print "Would run test $test now\n";
    }
    elsif ($config_list->check_config ('Purify')) {
        purify_program ($test);
    }
    elsif (defined $opt_v) {
        output_vxworks_commands ($oh, $test);
    }
    else {
        run_program ($test);
    }
}

check_resources ($oh) if (!defined $opt_d);

delete_temp_files ();
