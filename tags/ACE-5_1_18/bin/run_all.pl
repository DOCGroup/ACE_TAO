eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-
# $Id$
#
# This script execute the test programs (usually, other scripts) in
# the RUN_LIST defined below. If it detects any problem it send email.
#
# This script requires ActivePerl for Win32 and libnet from CPAN.
#
# Usage:  run_all.pl <log directory> <admin email address>
#
#     For example: run_all.pl /tmp/log peter_pan@neverland.org

use Net::SMTP;
use File::Basename;
use FileHandle;
require POSIX;

push @INC, $ENV{'ACE_ROOT'}.'/bin';
require run_all_list;

# Find out the command name.
$CMD = basename($0);

$single_threaded = 0;
$minimum_corba = 0;
$dont_run = 0;
$report_success = 0;
$debug = 0;
$ami=0
@ARGS = ();
while ($#ARGV >= 0) {
  if (!($ARGV[0] =~ m/-/)) {
    push @ARGS, $ARGV[0];
    shift;
  } elsif ($ARGV[0] eq "-single_threaded") {
    $single_threaded = 1;
    shift;
  } elsif ($ARGV[0] eq "-minimum_corba") {
    $minimum_corba = 1;
    shift;
  } elsif ($ARGV[0] eq "-ami") {
      $ami = 1;
      shift;
  } elsif ($ARGV[0] eq "-report_success") {
    $report_success = 1;
    shift;
  } elsif ($ARGV[0] eq "-debug") {
    $debug = 1;
    shift;
  }
}

# We obtain our revision to report errors.
$REVISION='$Revision$ ';

# Extract configuration information from command line.
  # TODO: Some validation and checking should be done here.
$LOGDIR   = $ARGS[0];
$ADMIN    = $ARGS[1];
$MAIL = "mail";
if ($#ARGS >= 2) {
  $MAIL     = $ARGS[2];
}

$disable_file = $LOGDIR . '/.disable';
$histfile = $LOGDIR . '/history';
$LOGBASE = POSIX::strftime("%b%d_%Y.log", localtime);
$LOGFILE = $LOGDIR . '/' . $LOGBASE;
$HOST = `hostname`;
chop $HOST;
$LOG_NAME =  $HOST . "_" . $BUILD . "_" . $LOGBASE;
$STATUS = "OK";

sub report_errors {
  # Now send a summary of the errors to the ADMIN account...
  local $to = $ADMIN;

  open (MAIL, "|".$MAIL.' -s [AUTO_COMPILE] '.$to)
    || die "Cannot open mail pipe for: $_\n";

  print MAIL 'The following message is brought to you by: ', "\n";
  print MAIL $CMD, ' [', $REVISION, "] for $BUILD on $CHECKOUT\n";

  local $m;
  foreach $m (@_) {
    print MAIL $m, "\n";
  }

  print MAIL "\nPlease check the following log for more info:\n\n";
  print MAIL 'http://www.cs.wustl.edu/~bugzilla/auto_compile_logs/',
             $LOG_NAME, "\n\n";

  close (MAIL); # Ignore errors....

#  # Now send the complete log to bugzilla...
#  local $bugs = 'bugzilla'.'@cs.wustl.edu';
#  open (MAIL, "|".$MAIL.' -s AUTO_COMPILE_LOG='.$LOG_NAME.' '.$bugs)
#    || die "Cannot open mail pipe for: $LOG_NAME\n";

#  print MAIL 'This is the log for: ', "\n";
#  print MAIL $CMD, ' [', $REVISION, "] for $BUILD on $CHECKOUT\n";

#  print MAIL "\n================================================================\n";

# if (open (THELOG, "$LOGFILE"))
#    {
#      while (<THELOG>) {
#	print MAIL $_;
#      }
#      close (THELOG);
#    }
#  close (MAIL); # Ignore errors....
}
 
### MAIN FUNCTION

$histfile = $LOGDIR . '/history';
open(HIST, '>>' . $histfile)
    # Do not use 'die' to report the problem, it tries to use HIST....
    || die "cannot open history file \"$histfile\"\n";

$date = localtime;

print HIST $CMD, ': running at ', $date, ' ';

if (-f $disable_file) {
    print HIST "DISABLED\n";
    exit 0;
}

open (DISABLE, '>' . $disable_file)
    || die "cannot open disable file";
print DISABLE "auto_compile <$date> is running\n";
close (DISABLE)
    || die "cannot close disable file";

$LOGFILE = $LOGDIR . '/' . POSIX::strftime("%b%d_%Y.log", localtime);
open(LOG, '>' . $LOGFILE)
    || die "cannot open log file";

LOG->autoflush ();

@failures = ();

if ($dont_run == 0) {
  @LIST = @RUN_LIST;
  if ($single_threaded) {
    @LIST = @SINGLE_THREADED_LIST;
  } elsif ($minimum_corba) {
    @LIST = @MINIMUM_CORBA_LIST;
  } elsif ($ami) {
    @LIST = @AMI_CORBA_LIST;
  }

  $DIR = $ENV{'PWD'};
  foreach $i (@LIST) {
    local @test_info = split (/\ \,\ /, $i);
    local $directory = $test_info[0];
    local $program = $test_info[1];

    $date = localtime;
    print LOG "$CMD: =============================================\n";
    print LOG "$CMD: running $program in $directory at ", $date, "\n";
    local $subdir = $DIR . '/' . $directory;
    chdir ($subdir)
	|| die "cannot chdir to $subdir";

    $run_error = 0;
    if (open(RUN, "perl $program 2>&1 |") == 0) {
	push @failures, "cannot run $program in $directory";
	next;
    }
    while (<RUN>) {
	print LOG $_;
	if (m/Error/
	    || m/FAILED/
	    || m/EXCEPTION/
	    || m/pure virtual /i) {
	  if ($STATUS eq "OK") {
	    $STATUS = "RUNTIME ERROR";
	  }
	  $run_error = 1;
	}
    }
    if (close(RUN) == 0) {
      if ($STATUS eq "OK") {
	$STATUS = "RUNTIME ERROR";
      }
      push @failures, "Error when closing pipe for $program in $directory";
      next;
    }
    $date = localtime;
    print LOG "$CMD: $program finished ", $date, "\n";

    if ($run_error != 0) {
      push @failures,
      "errors detected while running $program in $directory";
    }
  }
}

if ($#failures >= 0) {
  report_errors @failures;
}

close(LOG)
    || die "cannot close LOGFILE";

print HIST "$STATUS\n";
close(HIST)
    || die "cannot close history file";

unlink $disable_file
    || die "cannot unlink disable file";

if ($report_success) {
  report_errors "Congratulations: No errors or warnings detected\n";
}

exit 0;
