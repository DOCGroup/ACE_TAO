# -*- perl -*-
# $Id$
#
# This script execute the test programs (usually, other scripts) in
# the RUN_LIST defined below. If it detects any problem it send email.
#
# This script requires ActivePerl for Win32 and libnet from CPAN.
#
# Usage:  run_all_win32.pl <log directory> <admin email address>
#
#     For example: run_all_win32.pl c:\log peter_pan@neverland.org

use Net::SMTP;
use File::Basename;
use FileHandle;
require POSIX;

use lib "$ENV{ACE_ROOT}/bin";
require run_all_list;

# This is the module we will checkout, someday someone could define a
# smaller module.
$MODULE='ACE_wrappers';

# The following are for redirecting execution results to temporary files.
$exe_log_name='run_test.log';   # Name of the tempfile used.

# If using 'sh'.
$sh_cmd="c:/bin/sh ";
$sh_redirection=" > $exe_log_name 2>&1";

#if using '4nt',  4nt is not quite working yet.
$four_nt_cmd="d:/Utils/4NT301/4NT.EXE ";
$four_nt_redirection=" >& $exe_log_name";

# Pick the one your like.
$shell_invoke = $sh_cmd;
$redirect_output  = $sh_redirection;

# We obtain our revision to report errors.
#$REVISION='$Revision$';

# Find out the command name.
$CMD = basename($0);

# Extract configuration information from command line.
# TODO: Some validation and checking should be done here.
$LOGDIR = $ARGV[0];
$ADMIN = $ARGV[1];

# When an error is found we try to die gracefully and send some email
# to ADMIN.

$disable_file = $LOGDIR . '/.disable';

sub mywarn {
  local @msg = @_;

  # Need more error checking here.

  $smtp = Net::SMTP->new('mail.cs.wustl.edu');
  $smtp->mail( "$ADMIN" );      # sender's address
  $smtp->to("$ADMIN");          # recipient's address
  $smtp->data();                # Start the mail

  # Mail Header
  $smtp->datasend("To: $ADMIN\n");
  $smtp->datasend("Subject: Auto run results\n");
  $smtp->datasend("From: $ADMIN\n");
  $smtp->datasend("\n");

  # Now the message body
  $smtp->datasend ('The following message is brought to you by: ', "\n");
  $smtp->datasend ("$CMD,  [ $REVISION ] for $BUILD on $CHECKOUT\n\n");

  local $m;
  foreach $m (@msg) {
      $smtp->datasend ("$m \n");
  }

  $smtp->datasend ("\nPlease check log files for more info\n");

  $smtp->dataend();                   # Finish sending the mail
  $smtp->quit;                        # Close the SMTP connection
}

sub mydie {
  mywarn @_;
  print HIST 'FAILED', "\n";
  unlink $disable_file
    || die "cannot unlink disable file";
  exit 0;
}

### MAIN FUNCTION

### MAIN FUNCTION

$histfile = $LOGDIR . '/history';
open(HIST, '>>' . $histfile)
    # Do not use 'mydie' to report the problem, it tries to use HIST....
    || die "cannot open history file \"$histfile\"\n";

$date = localtime;

print HIST $CMD, ': running at ', $date, ' ';

if (-f $disable_file) {
    print HIST "DISABLED\n";
    exit 0;
}

open (DISABLE, '>' . $disable_file)
    || mydie "cannot open disable file";
print DISABLE "auto_compile <$date> is running\n";
close (DISABLE)
    || mydie "cannot close disable file";

$LOGFILE = $LOGDIR . '/' . POSIX::strftime("%b%d_%Y.log", localtime);
open(LOG, '>' . $LOGFILE)
    || mydie "cannot open log file";

LOG->autoflush ();

foreach $i (@RUN_LIST) {
    local @test_info = split (/\ \,\ /, $i);
    local $directory = $test_info[0];
    local $program = $test_info[1];
    print "$directory, $program\n";

    $date = localtime;
    print LOG "$CMD: =============================================\n";
    print LOG "$CMD: running $program in $directory at ", $date, "\n";
    local $subdir =
        $ENV{ACE_ROOT} . '/'. $directory;
    chdir ($subdir)
        || mydie "cannot chdir to $subdir";

    $run_error = 0;

    system ($shell_invoke . " " . $program . " " . $redirect_output);

    open (RUN, "$exe_log_name") || push @failures, "Can't open execution log file $exe_log_name\n";

    while (<RUN>) {
        print LOG $_;
        if (m/^Error/ || m/FAILED/ || m/EXCEPTION/) {
            $run_error = 1;
        }
    }
    if (close(RUN) == 0) {
        push @failures, "Error when closing log file $program in $directory";
        next;
    }
    $date = localtime;
    print LOG "$CMD: $program finished ", $date, "\n";

    if ($run_error != 0) {
        push @failures,
            "errors detected while running $program in $directory";
    }

    unlink ("$exe_log_name");
}

if ($#failures >= 0) {
    mydie @failures;
}

close(LOG)
    || mydie "cannot close LOGFILE";

print HIST "OK\n";
close(HIST)
    || mydie "cannot close history file";

unlink $disable_file
    || mydie "cannot unlink disable file";

exit 0;
