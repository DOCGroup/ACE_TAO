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

use PerlACE::TestTarget;

use Cwd;
use English;
use Getopt::Std;
use FileHandle;
use File::Basename;

$config_list = new PerlACE::ConfigList;

if (grep(($_ eq 'ARCH'), @PerlACE::ConfigList::Configs)) {
  my $subdir = $PerlACE::Process::ExeSubDir;
  $subdir =~ s/\/$//;
  $ENV{'ACE_EXE_SUB_DIR'} = $subdir;
}


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
        $end_test_resources=`ipcs | egrep $user`;
         if ("$start_test_resources" ne "$end_test_resources") {
            print STDERR "Warning: the ACE tests _may_ have leaked OS ".
                         "resources!\n";
            print STDERR "Warning: Before: $start_test_resources\n";
            print STDERR "Warning: After:  $end_test_resources\n";
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
    $ENV{COVERITY_TEST_NAME} = "tests/$path";

    my ($program, $dir, $suffix) = fileparse($path);
    my $start_dir = getcwd ();
    if ($dir ne "" && !chdir $dir) {
        print STDERR "Error: Can\'t chdir to $dir for $path\n";
        return;
    }

    unlink <log/$program*.log>;
    unlink "core";

    my $P = $target->CreateProcess($program, $arguments);
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
    $status = $P->SpawnWaitKill (300 + $target->ProcessStartWaitInterval());
    my $time = time() - $start_time;

    ### Check for problems

    if ($status == -1) {
        print STDERR "Error: $program FAILED (time out after Time:$time"."s)\n";
        $P->Kill ();
        $P->TimedWait (1);
    }
    elsif ($status != 0) {
        print STDERR "Error: $program $arguments FAILED with exit status $status after Time:$time"."s\n";
    }

    print "\nauto_run_tests_finished: tests/$program $arguments Time:$time"."s Result:$status\n";

    check_log ($target, $program);

    if ($config_list->check_config ('Codeguard')) {
        check_codeguard_log ($program);
    }
    chdir $start_dir;
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
    my $target = shift;
    my $program = shift;

    ### Check the logs
    local $log_suffix = ".log";

    # Support logs generated by tests in subdirectories, such as tests
    # found in the SSL subdirectory.
    local $the_program = basename($program);
    local $log = "log/".$the_program.$log_suffix;

    if ($target->GetFile ($log, $log) == -1) {
        print STDERR "ERROR: cannot retrieve file <$log>\n";
    }

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

if (!getopts ('dhtvo:l:') || $opt_h) {
    print "run_test.pl [-h] [-v] [-o <output file>] [-t file1 file2 ...]\n";
    print "\n";
    print "Runs the tests listed in run_test.lst\n";
    print "\n";
    print "Options:\n";
    print "    -d         Debug mode (do not run tests)\n";
    print "    -h         Display this help\n";
    print "    -t         Runs all the tests passed via the cmd line\n";
    print "    -l list    Load the list and run only those tests\n";
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

@tests = ();

if (defined $opt_t) {
    @tests = @ARGV;
}
elsif (defined $opt_l) {
    $config_list->load ("$opt_l");
    @tests = $config_list->valid_entries ();
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

my $target = PerlACE::TestTarget::create_target (1);

$target->AddLibPath("$ENV{ACE_ROOT}/tests");

# Put needed files in place for targets that require them.
# Service_Config_Test needs service config file.
my $svc_conf_file = "Service_Config_Test.conf";
if ($target->PutFile ($svc_conf_file) == -1) {
    print STDERR "WARNING: Cannot send $svc_conf_file to target\n";
}
# Config_Test needs config ini file.
my $conf_ini_file = "Config_Test_Import_1.ini";
if ($target->PutFile ($conf_ini_file) == -1) {
    print STDERR "WARNING: Cannot send $conf_ini_file to target\n";
}
# Service_Config_Stream_Test needs service config file.
$svc_conf_file = "Service_Config_Stream_Test.conf";
if ($target->PutFile ($svc_conf_file) == -1) {
    print STDERR "WARNING: Cannot send $svc_conf_file to target\n";
}

foreach $test (@tests) {
    if (defined $opt_d) {
        print "Would run test $test now\n";
    }
    elsif ($config_list->check_config ('Purify')) {
        purify_program ($test);
    }
    else {
        run_program ($target, $test);
    }
    $target->GetStderrLog();
}

check_resources ($oh) if (!defined $opt_d);

delete_temp_files ();
