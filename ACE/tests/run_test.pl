eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-
# This file is for running the tests in the ACE tests directory.
# It is usually used for auto_compiles.

if (defined $ENV{ACE_ROOT}) {
  use lib "$ENV{ACE_ROOT}/bin";
} else {
  use lib '../bin';
}
if (defined $ENV{top_srcdir}) {
  use lib "$ENV{top_srcdir}/bin";
}

use PerlACE::Run_Test;
use PerlACE::TestTarget;

use Cwd;
use English;
use Getopt::Std;
use FileHandle;
use File::Basename;

$config_list = new PerlACE::ConfigList;

PerlACE::add_lib_path("$ENV{ACE_ROOT}/tests");

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

sub run_program ($@)
{
    my $target = shift;
    my $path = shift;
    my $arguments = shift;
    if ($path =~ /^(\S*)\s*(.*)/ ) {
        $path = $1;
        $arguments = $2 . $arguments;
    }

    ## Print it out before we check for the executable
    ## if the executable doesn't exist, the error will show
    ## up as part of the previous test.
    print "auto_run_tests: tests/$path $arguments\n";

    my ($program, $dir, $suffix) = fileparse($path);
    my $start_dir = getcwd ();
    if ($dir ne "" && !chdir $dir) {
        print STDERR "Error: Can\'t chdir to $dir for $path\n";
        return;
    }
    unlink <log/$program*.log>;
    unlink "core";

    my $P = $target->CreateProcess($program,$arguments);

    if ($config_list->check_config ('Valgrind')) {
      $P->IgnoreExeSubDir(1);
    }
    else {
      ### Try to run the program

      if (! -e $P->Executable ()) {
          print STDERR "Error: " . $P->Executable () .
                       " does not exist or is not runnable\n";
          chdir $start_dir;
          return;
       }
    }

    my $start_time = time();
    $status = $P->SpawnWaitKill (400);
    my $time = time() - $start_time;

    ### Check for problems

    if ($status == -1) {
        print STDERR "Error: $program FAILED (time out)\n";
        $P->Kill ();
        $P->TimedWait (1);
    }
    elsif ($status != 0) {
        print STDERR "Error: $program $arguments FAILED with exit status $status\n";
    }

    print "\nauto_run_tests_finished: tests/$program $arguments Time:$time"."s Result:$status\n";

    check_log ($program);

    if ($config_list->check_config ('Codeguard')) {
    	check_codeguard_log ($program);
    }
    chdir $start_dir;
}

################################################################################

sub run_vxworks_command ($)
{
    my $program = shift;

    unlink <log/$program*.log>;
    unlink "core";

    my $P = new PerlACE::ProcessVX ($program);

    ## check module existence
    if (! -e $P->Executable ()) {
        print STDERR "Error: " . $P->Executable() .
                     " does not exist\n";
        return;
    }

    print "auto_run_tests: tests/$program\n";
    my $start_time = time();
    $status = $P->SpawnWaitKill (400);
    my $time = time() - $start_time;

    ### Check for problems

    if ($status == -1) {
        print STDERR "Error: $program FAILED (time out)\n";
        $P->Kill ();
        $P->TimedWait (1);
    }
    elsif ($status != 0) {
        print STDERR "Error: $program FAILED with exit status $status\n";
    }

    print "\nauto_run_tests_finished: test/$program Time:$time"."s Result:$status\n";

    check_log ($program);
}

################################################################################

sub output_vxworks_commands
{
  my($oh)      = shift;
  my($program) = shift;
  my($length)  = length($program) + 2;

  if (defined $ENV{'ACE_RUN_VX_CHECK_RESOURCES'}) {
    print $oh "memShow();\n";
  }

  print $oh "write(2, \"\\n$program\\n\", $length);\n" .
            "ld 1,0, \"" . $program . ".out\"\n" .
            "vx_execae ace_main\n" .
            "unld \"" . $program . ".out\"\n";
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
    local $log_suffix;
    if (defined $ENV{"ACE_WINCE_TEST_CONTROLLER"}) {
        $log_suffix = ".txt";
    }
    else {
        $log_suffix = ".log";
    }

    # Support logs generated by tests in subdirectories, such as tests
    # found in the SSL subdirectory.
    local $the_program = basename($program);
    local $log = "log/".$the_program.$log_suffix;

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
                print STDERR "Error ($log): no line with 'Starting'\n";
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
            local $sublognames = "$program\-.*".$log_suffix;
            @sublogs = grep (/^$sublognames/, readdir (THISDIR));
            closedir (THISDIR);
            my $saw_short_process_manager_child_log = 0;
            foreach $log (@sublogs) {
                # Just like the main log, but note that Process_Manager_Test
                # kills (signal 9) one of its children so the log may get
                # deleted, or it may be incomplete. So let this one go, but
                # only once per Process_Manager_Test.
                if (open (LOG, "<log/".$log) == 0) {
                    print STDERR "Error: Cannot open sublog file $log\n";
                }
                else {
                    my $number_starting = 0;
                    my $number_ending = 0;
                    while (<LOG>) {
                        chomp;
                        if (m/Starting/) {
                            $number_starting++;
                        }
                        if (m/Ending/) {
                            $number_ending++;
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

                    if ($number_starting == 0) {
                       print STDERR "Error ($log): no line with 'Starting'\n";
                       $print_log = 1;
                    }

                    if ($number_ending == 0) {
                       if ($program eq 'Process_Manager_Test' &&
                           $saw_short_process_manager_child_log == 0) {
                           $saw_short_process_manager_child_log = 1;
                           $number_ending = 1;
                       }
                       else {
                           print STDERR "Error ($log): no line with 'Ending'\n";
                           $print_log = 1;
                       }
                    }

                    if ($number_starting != $number_ending) {
                       print STDERR "Error ($log): Number of 'Starting' does not match number of 'Ending' ($number_starting != $number_ending)\n";
                       $print_log = 1;
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

sub check_codeguard_log ($)
{
    my $program = shift;

    ### Check the logs

    local $log = $program.".cgl";

    if (-e $log ) {
       print STDERR "======= Begin Codeguard Log File \n";
       if (open (LOG, "<".$log) == 0) {
           print STDERR "Error: Cannot open codeguard log file $log\n";
       }
       else {
           my @log = <LOG>;
           print STDERR @log;
           close (LOG);
       }
       print STDERR "======= End Codeguard Log File \n";
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
    print "    -o         Put VxWorks commands in <output file>\n";
    print "\n";
    print "Pass in configs using \"-Config XXXXX\"\n";
    print "\n";
    print "Possible Configs: CHECK_RESOURCES Purify Codeguard Valgrind ",
           $config_list->list_configs (), "\n";
    exit (1);
}

## since we can't use "our" to get rid of warnings.
$opt_h = $opt_h if (defined $opt_h);
$opt_t = $opt_t if (defined $opt_t);
$opt_g = $opt_g if (defined $opt_g);

if (!($tmp = $ENV{TMP}) && !($tmp = $ENV{TEMP})) {
    $tmp="/tmp";
}

check_for_more_configs ();

if (PerlACE::is_vxworks_test ()) {
    $opt_v = 1;
}

@tests = ();

if (defined $opt_t) {
    @tests = @ARGV;
}
else {
    $config_list->load ("run_test.lst");
    @tests = $config_list->valid_entries ();
}

if (defined $opt_d) {
    $config_list->dump ();
}

record_resources () if (!defined $opt_d);

my($oh) = \*STDOUT;
if (defined $opt_v && defined $opt_o) {
  $oh = new FileHandle();
  if ($opt_o != 1) {
    if (!open($oh, ">$opt_o")) {
      print STDERR "ERROR: Unable to write to $opt_o\n";
      exit(1);
    }
  }

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

  foreach $test (@tests) {
    output_vxworks_commands ($oh, $test);
  }
}
else {

  my $target = PerlACE::TestTarget::create_target (1);

  foreach $test (@tests) {
    if (defined $opt_d) {
      print "Would run test $test now\n";
    }
    elsif ($config_list->check_config ('Purify')) {
      purify_program ($test);
    }
    if (defined $opt_v) {
      run_vxworks_command ($test);
    }
    else {
      run_program ($target, $test);
    }
    $target->GetStderrLog();
  }
}

check_resources ($oh) if (!defined $opt_d);

delete_temp_files ();
