eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

unshift @INC, '../../../../../bin';
require Process;
require Uniqueid;

$prefix = "." . $DIR_SEPARATOR;
$status = 0;

print STDERR "\n\nReconnect suppliers and consumers,",
  " using disconnect/connect calls\n";
$T = Process::Create ($prefix . "Reconnect".$EXE_EXT,
		      " -suppliers 100 -consumers 100 -d 100");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nReconnect suppliers and consumers, using connect calls\n";
$T = Process::Create ($prefix . "Reconnect".$EXE_EXT,
		      " -suppliers 100 -consumers 100 -d 100 -s -c");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nShutdown EC with clients still attached\n";
$T = Process::Create ($prefix . "Shutdown".$EXE_EXT,
		      " -verbose -suppliers 5 -consumers 5");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nComplex event channel test,",
  "multiple ECs connected through gateways\n";
$T = Process::Create ($prefix . "Observer".$EXE_EXT,
		      " -ORBsvcconf observer.conf"
		      ." -consumer_tshift 0 -consumers 5"
		      ." -supplier_tshift 0 -suppliers 2"
		      ." -burstsize 10 -burstcount 10 -burstpause 0");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

exit $status;
