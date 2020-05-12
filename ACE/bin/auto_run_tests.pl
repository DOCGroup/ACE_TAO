eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# Executes test list files (*.lst), which contain commands with conditions
# called configurations under which the commands are run.

use strict;

use lib "$ENV{ACE_ROOT}/bin";
if (defined $ENV{srcdir}) {
  use lib "$ENV{srcdir}/bin";
}
use PerlACE::Run_Test;

use Getopt::Long;
use Cwd;

use Env qw(ACE_ROOT PATH TAO_ROOT CIAO_ROOT DANCE_ROOT DDS_ROOT);

if (!defined $TAO_ROOT && -d "$ACE_ROOT/TAO") {
    $TAO_ROOT = "$ACE_ROOT/TAO";
}
if (!defined $CIAO_ROOT && -d "$ACE_ROOT/TAO/CIAO") {
    $CIAO_ROOT = "$ACE_ROOT/TAO/CIAO";
}
if (!defined $DANCE_ROOT && -d "$ACE_ROOT/TAO/DAnCE") {
    $DANCE_ROOT = "$ACE_ROOT/TAO/DAnCE";
}
if (!defined $DDS_ROOT && -d "$ACE_ROOT/TAO/DDS") {
    $DDS_ROOT = "$ACE_ROOT/TAO/DDS";
}

sub run_command {
  my $test = shift;
  my $command = shift;
  my $print_error = shift;

  my $result = 0;
  if (system($command)) {
    $result = $? >> 8;
    if ($print_error) {
      my $signal = $? & 127;
      my $coredump = $? & 128;
      my $error_message;
      if ($? == -1) {
        $error_message = "failed to run: $!";
      }
      elsif ($signal) {
        $error_message = sprintf("exited on signal %d", ($signal));
        $error_message .= " and created coredump" if ($coredump);
      }
      else {
        $error_message = sprintf("returned with status %d", $result);
      }
      print "Error: $test $error_message\n";
    }
  }
  return $result;
}

sub print_help {
  my $fd = shift;
  print $fd
    "auto_run_tests.pl [<options> ...] [<list_file> ...]\n" .
    "\n" .
    "Executes test list files (*.lst), which contain commands with conditions called\n" .
    "configurations under which the commands are run.\n" .
    "\n" .
    "Test lists files can be specified using the -l option or as non-option\n" .
    "arguments. The standard test list files for ACE/TAO and related software are\n" .
    "built-in and can be included via the listed options before. If not test list\n" .
    "files are specified, the script tries to use all the standard test list files\n" .
    "that can be found.\n" .
    "\n" .
    "Options:\n" .
    "    --help | -h              Display this help\n" .

    "    --ace | -a               Include the ACE tests\n" .
    "    --orb | -o               Include the TAO ORB tests\n" .
    "    --tao | -t               Include the TAO non-ORB tests\n" .
    "    --ciao-dance | -C        Include the CIAO and DAnCE tests\n" .
    "    --opendds | -d           Include the OpenDDS tests\n" .

    "    -l <list_file>           Include the tests from <list_file>\n" .

    "    --sandbox | -s <sandbox> Runs each program using a sandbox program\n" .
    "    --root | -r <dir>        Root directory for running the tests\n" .
    "    --dry-run | -z           Just print the commands that would be ran\n" .
    "    --show-configs           Just print possible values for -Config\n" .

    # These two are processed by PerlACE/ConfigList.pm
    "    -Config <cfg>            Include tests with <cfg> configuration\n" .
    "    -Exclude <cfg>           Exclude tests with <cfg> configuration\n" .

    # This one is processed by PerlACE/Process.pm
    "    -ExeSubDir <dir>         Subdirectory for finding the executables\n";
}

################################################################################

# Parse Options
my $help = 0;
my $ace_tests = 0;
my $tao_orb_tests = 0;
my $tao_tests = 0;
my $ciao_dance_tests = 0;
my $opendds_tests = 0;
my @l_options = ();
my $sandbox = '';
my $dry_run = 0;
my $startdir = '';
my $show_configs = 0;
Getopt::Long::Configure('bundling', 'no_auto_abbrev');
my $invalid_arguments = !GetOptions(
  'help|h' => \$help,
  'ace|a' => \$ace_tests,
  'orb|o' => \$tao_orb_tests,
  'tao|t' => \$tao_tests,
  'ciao-dance|C' => \$ciao_dance_tests,
  'opendds|d' => \$opendds_tests,
  'l=s' => \@l_options,
  'sandbox|s=s' => \$sandbox,
  'dry-run|z' => \$dry_run,
  'root|r=s' => \$startdir,
  'show-configs' => \$show_configs,
);
if ($invalid_arguments || $help) {
  print_help($invalid_arguments ? *STDERR : *STDOUT);
  exit($invalid_arguments ? 1 : 0);
}

# Determine what test list files to use
my @main_test_lists = (
  [\$ace_tests, $ACE_ROOT, "bin/ace_tests.lst", "ACE"],
  [\$tao_orb_tests, $TAO_ROOT, "bin/tao_orb_tests.lst", "TAO ORB"],
  [\$tao_tests, $TAO_ROOT, "bin/tao_other_tests.lst", "TAO non-ORB"],
  [\$ciao_dance_tests, $CIAO_ROOT, "bin/ciao_tests.lst", "CIAO"],
  [\$ciao_dance_tests, $DANCE_ROOT, "bin/dance_tests.lst", "DANCE"],
  [\$opendds_tests, $DDS_ROOT, "bin/dcps_tests.lst", "OpenDDS"],
);
my @file_list = ();
my $list_error = 0;
foreach my $i (@main_test_lists) {
  my $enabled_ref = $i->[0];
  my $root = $i->[1];
  my $list_file = $i->[2];
  my $name = $i->[3];
  my $list_file_path = "$root/$list_file";
  if (defined $root && $$enabled_ref) {
    push(@file_list, $list_file_path);
  }
  elsif ($$enabled_ref) {
    $list_error = "option for $name tests passed, but the root enviroment variable isn't set";
  }
}
push(@file_list, @l_options);
push(@file_list, @ARGV);
if (!scalar(@file_list)) {
  foreach my $i (@main_test_lists) {
    my $root = $i->[1];
    my $list_file = $i->[2];
    my $list_file_path = "$root/$list_file";
    if (defined $root && -f $list_file_path) {
      push (@file_list, $list_file_path);
    }
  }
  if (!scalar(@file_list)) {
    $list_error = "no default test lists could be found";
  }
}
if ($list_error) {
  print STDERR "ERROR: $list_error\n";
  exit(1);
}

if ($show_configs) {
  foreach my $test_list (@file_list) {
    my $config_list = new PerlACE::ConfigList;
    $config_list->load($test_list);
    print "$test_list: " . $config_list->list_configs() . "\n";
  }
  exit (0);
}

my $explicit_startdir = 0;
if ($startdir) {
  $explicit_startdir = 1;
}
else {
  $startdir = "$ACE_ROOT";
}

foreach my $test_lst (@file_list) {

    my $config_list = new PerlACE::ConfigList;
    if (-r $ACE_ROOT.$test_lst) {
      $config_list->load($ACE_ROOT.$test_lst);
    }
    elsif (-r "$startdir/$test_lst") {
      $config_list->load("$startdir/$test_lst");
    }
    else {
      $config_list->load($test_lst);
    }

    # Insures that we search for stuff in the current directory.
    $PATH .= $Config::Config{path_sep} . '.';

    foreach my $test ($config_list->valid_entries()) {
        my $directory = ".";
        my $program = ".";

        ## Remove intermediate '.' directories to allow the
        ## scoreboard matrix to read things correctly
        $test =~ s!/./!/!g;

        if ($test =~ /(.*)\/([^\/]*)$/) {
            $directory = $1;
            $program = $2;
        }
        else {
            $program = $test;
        }

        # this is to ensure that we dont print out the time for tests/run_test.pl
        # that test prints out the times for each of the ace tests individually
        my $is_ace_test = ($directory eq "tests");

        if (! $is_ace_test) {
            print "auto_run_tests: $test\n";
            if ($config_list->check_config('Coverity')) {
              $ENV{COVERITY_TEST_NAME} = $test;
              $ENV{COVERITY_SUITE_NAME} = $test_lst;
              $ENV{COVERITY_TEST_SOURCE} = "$directory/$program";
            }
        }

        my($orig_dir) = $directory;
        if ($directory =~ m:^TAO/(.*):) {
          $directory = $1;
        }
        if ($directory =~ m:^CIAO/(.*):) {
          $directory = $1;
        }
        if ($directory =~ m:^DAnCE/(.*):) {
          $directory = $1;
        }
        if ($directory =~ m:^DDS/(.*):) {
          $directory = $1;
        }

        my $status;
        my @dirlist = ($ACE_ROOT."/$directory",
                       $TAO_ROOT."/$directory",
                       $CIAO_ROOT."/$directory",
                       $DANCE_ROOT."/$directory",
                       $DDS_ROOT."/$directory");
        # make sure to *first* check the explicitly specified directory and
        # only when nothing found there check the default dirs
        if ($explicit_startdir) {
          unshift (@dirlist, $startdir."/$directory");
          unshift (@dirlist, $startdir."/$orig_dir");
        }
        foreach my $path (@dirlist) {
          if (-d $path && ($status = chdir($path))) {
            last;
          }
        }

        if (!$status) {
          if ($explicit_startdir) {
            print STDERR "ERROR: Cannot chdir to $startdir/$directory\n";
          } else {
            print STDERR "ERROR: Cannot chdir to $directory\n";
          }
          next;
        }

        if ($program =~ /(.*?) (.*)/) {
            if (! -e $1) {
                print STDERR "ERROR: $directory.$1 does not exist\n";
                next;
            }
        }
        else {
            if (! -e $program) {
                print STDERR "ERROR: $directory.$program does not exist\n";
                next;
            }
        }

        ### Generate the -ExeSubDir and -Config options
        my $inherited_options = " -ExeSubDir $PerlACE::Process::ExeSubDir ";

        foreach my $config ($config_list->my_config_list()) {
             $inherited_options .= " -Config $config ";
        }

        my $cmd = '';
        if ($sandbox) {
            #The Win32 sandbox takes the program and options in quotes, but the
            #posix sandbox takes the program and options as separate args.
            my($q) = ($^O eq 'MSWin32') ? '"' : '';
            $cmd = "$sandbox ${q}perl $program $inherited_options${q}";
        }
        else {
            $cmd = "perl $program$inherited_options";
        }

        if ($dry_run) {
            my $cwd = getcwd();
            print "In \"$cwd\" would run:\n    $cmd\n";
        }
        else {
            my $start_time = time();
            my $result = run_command($test, $cmd, !$is_ace_test);
            my $time = time() - $start_time;

            # see note about tests/run_test.pl printing reports for ace tests individually
            if (!$is_ace_test) {
                print "\nauto_run_tests_finished: $test Time:$time"."s Result:$result\n";
                print "==============================================================================\n";
            }
        }
    }
}
