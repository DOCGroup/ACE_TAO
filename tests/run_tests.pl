eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../bin';
require ACEutils;

use English;

$tmp="/tmp";
$compilation_log="log/compilations.log";

$ACE_ROOT = $ENV{'ACE_ROOT'}  ||  '..';
$LD_VAR = 'LD_LIBRARY_PATH';

if ($OSNAME eq "HP-UX") {
  $LD_VAR = 'SHLIB_PATH';
} elsif ($OSNAME eq 'AIX') {
  $LD_VAR = 'LIBPATH';
}

local $path = $ENV{$LD_VAR};
if ($path eq '') {
  $path = '/usr/lib';
}
$ENV{$LD_VAR} = $ACE_ROOT."/ace:".$path;

####
#### Run each process
####
sub run
{
  local $program = shift;

  unlink "core";
  local $log = "log/".$program.".log";
  unlink $log;

  local $remove_exe_after_test = 0;
  if ( ! -x $program) {
    print STDERR "Making $program . . .\n";
    $remove_exe_after_test = 1;
    system ("make BIN=$program >>$compilation_log 2>&1");
  }

  print STDERR "Running $program\n";
  $P = Process::Create ($EXEPREFIX.$program.$EXE_EXT, "");
  $status = $P->TimedWait (300);

  if ($status == -1) {
    print STDERR "$program FAILED (time out)\n";
    $P->Kill (); $P->TimedWait (1);
  } elsif ($status != 0) {
    print STDERR "$program FAILED with exit status $status!!!!\n";
  }

  if ( -e "core" ) {
    print STDERR "$program dumped core!!!!\n";
  }
  if ( ! -e $log ) {
    print STDERR "No log file (log/$program.log) is present\n";
  } else {
    if (open (LOG, "<". $log) == 0) {
      print STDERR "Cannot open log file $log\n";
    } else {
      $starting_matched = 0;
      $ending_matched = 0;
      $bad_matched = 0;
      while (<LOG>) {
        chop;
        if (m/Starting/) {
          $starting_matched = 1;
        }
        if (m/Ending/) {
          $ending_matched = 1;
        }
        if (m/assertion failed|timeout|Bad file number/) {
          $bad_matched = 1;
        }
        # On Linux the /etc/hosts files contains IPV6 addresses that
        # tend to confuse the heck out of this test
        if (($program ne "SOCK_Connector_Test"
             || $^O ne "linux")
            && m/Invalid argument/) {
          $bad_matched = 1;
        }
        if ($program ne "Cached_Accept_Conn_Test"
            && m/No such file or directory/) {
          $bad_matched = 1;
        }
        if (m/not supported/) {
          print STDERR $_, "\n";
        }
      }
      if ($starting_matched == 0) {
        print STDERR "Error in $program log: no line with 'starting'\n";
      }
      if ($ending_matched == 0) {
        print STDERR "Error in $program log: no line with 'Ending'\n";
      }
      if ($bad_matched == 1) {
        print STDERR "Error in $program log: unexpected output\n";
      }
      close (LOG); # ignore errors
    }
  }

  if ($remove_exe_after_test == 1) {
    unlink $program;
  }
}

$ACE_BUILD_COMPONENTS=`$ACE_ROOT/bin/ace_components --ace`;
$OTHER = 1;
if (grep {/ Other /} $ACE_BUILD_COMPONENTS) {
  $OTHER = 0;
}
$TOKEN = 1;
if (grep {/ Token /} $ACE_BUILD_COMPONENTS) {
  $TOKEN = 0;
}
$netsvcs_main = 1;
if (-f "../netsvcs/servers/main" ) {
  $netsvcs_main = 0;
}
$libDLL_Test = 1;
if (-f "libDLL_Test.so" ) {
  $libDLL_Test = 0;
}

# Testing for this flag always disables the test.
$DISABLED = 1;

# @@ Can we obtain reasonable values for this features?
$chorus = 0;
$LynxOS = 0;
$Unicos = 0;

if ($chorus eq "" || $chorus == 0) {
  if (!defined $ENV{'LOGNAME'}) {
    $user=`whoami`;
  } else {
    $user = $ENV{'LOGNAME'};
  }
  $start_test_resources=`ipcs | egrep $user`;
}

$ace_version=`head -1 ../VERSION | sed -e 's/.*version \\([0-9.]*\\).*\$/\\1/'`;
chop $ace_version;

print STDERR "Starting ACE version $ace_version tests . . .\n";
open (LIST, '<run_tests.lst')
  || die "Can't read program list\n";
PROGRAM: while (<LIST>) {
  chop;
  local @fields = split (/\//, $_);
  local $program = $fields[$#fields];


  for ($i = 0; $i < $#fields; $i++) {
    $var = $fields[$i];
####    print "var = $var -> $program\n";
    local $e = $$var;
    if ($e == 1) {
      print STDERR "Skipping $program on this platform\n"
        unless $var = 'DISABLED';
      next PROGRAM;
    }
  }
  run $program;
}
close (LIST)
  || die "Can't close program list\n";
print STDERR "Finished ACE version $ace_version tests.\n";

unlink "ace_pipe_name";
unlink "pattern";
unlink "$tmp/ace_temp_file";
# unlink "$tmp/ace_temp_file*"; # How to make this work?
# unlink "$tmp/Naming_Test*";

if ($chorus eq "" || $chorus == 0) {
  $end_test_resources=`ipcs | egrep $user`;
  if ("$start_test_resources" ne "$end_test_resources") {
    print STDERR "WARNING: the ACE tests _may_ have leaked OS resources!\n";
    print STDERR "$end_test_resources";
  }
}

exit 0;
