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

# This is the module we will checkout, someday someone could define a
# smaller module.
$MODULE='ACE_wrappers';

# This are the pairs "sub-directory , script" we run; the separator
# *must* be a space followed by a comma and then another space.

@RUN_LIST =
      ( 'tests , run_tests.bat',
        'TAO/tests/Param_Test , run_test.pl',
        'TAO/tests/Param_Test , run_test.pl -i dii',
        'TAO/performance-tests/Cubit/TAO/IDL_Cubit , run_test.pl',
      'TAO/tests/OctetSeq , run_test.pl',
      'TAO/tests/Multiple_Inheritance , run_test.pl',
      'TAO/tests/MT_Client , run_test.pl',
      'TAO/tests/MT_Server , run_test.pl',
      'TAO/tests/Faults , run_test.pl',
      'TAO/tests/NestedUpcall/Reactor , run_test.pl',
      'TAO/tests/NestedUpcall/MT_Client_Test , run_test.pl',
      'TAO/tests/POA/Identity , run_test.pl',
      'TAO/tests/POA/Destruction , run_test.pl',
      'TAO/tests/IORManipulation , run_test.pl',
        'TAO/examples/POA/Adapter_Activator , run_test.pl',
        'TAO/examples/POA/DSI , run_test.pl',
        'TAO/examples/POA/Default_Servant , run_test.pl',
        'TAO/examples/POA/Explicit_Activation , run_test.pl',
        'TAO/examples/POA/FindPOA , run_test.pl',
        'TAO/examples/POA/Forwarding , run_test.pl',
        'TAO/examples/POA/NewPOA , run_test.pl',
        'TAO/examples/POA/On_Demand_Activation , run_test.pl',
        'TAO/examples/POA/On_Demand_Loading , run_test.pl',
        'TAO/examples/POA/Reference_Counted_Servant , run_test.pl',
      'TAO/examples/Simple/bank , run_test.pl',
      'TAO/examples/Simple/grid , run_test.pl',
      'TAO/examples/Simple/time-date , run_test.pl',
      'TAO/examples/Simple/time , run_test.pl',
      'TAO/orbsvcs/tests/Simple_Naming , run_test.pl',
      'TAO/orbsvcs/tests/Trading , run_test.pl',
      'TAO/orbsvcs/tests/Event/Basic , run_test.pl',
      'TAO/orbsvcs/tests/Event/Performance , run_test.pl',
      'TAO/orbsvcs/tests/EC_Basic , run_test.pl',
      'TAO/orbsvcs/tests/EC_Custom_Marshal , run_test.pl',
      'TAO/orbsvcs/tests/EC_Throughput , run_test.pl',
#      'TAO/orbsvcs/tests/EC_Mcast , run_test.pl',
#      'TAO/orbsvcs/tests/EC_Multiple , run_test.pl',
      'TAO/orbsvcs/tests/Event_Latency , run_test.pl',
      'TAO/examples/Simple/echo , run_test.pl < Echo.idl',
#      'TAO/examples/Simple/chat , run_test.pl',
      'TAO/orbsvcs/tests/Property , run_test.pl',
   'TAO/performance-tests/POA/Object_Creation_And_Registration , run_test.pl',
      'TAO/performance-tests/Cubit/TAO/MT_Cubit , run_test.pl -n 100'
#      'TAO/orbsvcs/tests/ImplRepo , run_test.pl airplane',
#      'TAO/orbsvcs/tests/ImplRepo , run_test.pl airplane_ir',
    );


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
    if (open(RUN, "$program |") == 0) {
	push @failures, "cannot run $program in $directory";
	next;
    }
    while (<RUN>) {
	print LOG $_;
	if (m/^Error/ || m/FAILED/ || m/EXCEPTION/) {
	    $run_error = 1;
	}
    }
    if (close(RUN) == 0) {
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
