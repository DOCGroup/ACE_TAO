eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that runs the client and all the other servers that
# are needed

unshift @INC, '../../../../../bin';
require ACEutils;
require Uniqueid;
use Cwd;

$cwd = getcwd();
ACE::checkForTarget($cwd);
$status = 0;

print STDERR "\n\nReconnect suppliers and consumers,",
  " using disconnect/connect calls\n";
$T = Process::Create ($EXEPREFIX . "Reconnect".$EXE_EXT,
                      " -suppliers 100 -consumers 100 -d 100");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nReconnect suppliers and consumers, using connect calls\n";
$T = Process::Create ($EXEPREFIX . "Reconnect".$EXE_EXT,
                      " -suppliers 100 -consumers 100 -d 100 -s -c");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}


print STDERR "\n\nShutdown EC with clients still attached\n";
$T = Process::Create ($EXEPREFIX . "Shutdown".$EXE_EXT,
                      " -verbose -suppliers 5 -consumers 5");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nGateway test\n";
$T = Process::Create ($EXEPREFIX . "Gateway".$EXE_EXT,
                      " -ORBsvcconf $cwd$DIR_SEPARATOR" . "observer.conf");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nComplex event channel test,",
  "multiple ECs connected through gateways\n";
$T = Process::Create ($EXEPREFIX . "Observer".$EXE_EXT,
                      " -ORBsvcconf $cwd$DIR_SEPARATOR" . "observer.conf"
                      ." -consumer_tshift 0 -consumers 5"
                      ." -supplier_tshift 0 -suppliers 2"
                      ." -burstsize 10 -burstcount 10 -burstpause 0");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nTimeout tests\n";
$T = Process::Create ($EXEPREFIX . "Timeout".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nWildcard tests\n";
$T = Process::Create ($EXEPREFIX . "Wildcard".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nNegation tests\n";
$T = Process::Create ($EXEPREFIX . "Negation".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nBitmask tests\n";
$T = Process::Create ($EXEPREFIX . "Bitmask".$EXE_EXT,
                      " -ORBSvcConf $cwd$DIR_SEPARATOR" . "svc.complex.conf");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nDisconnect callbacks test\n";
$T = Process::Create ($EXEPREFIX . "Disconnect".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nMT Disconnects test\n";
$T = Process::Create ($EXEPREFIX . "MT_Disconnect".$EXE_EXT,
                     " -ORBSvcConf $cwd$DIR_SEPARATOR" . "mt.svc.conf");
if ($T->TimedWait (240) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nAtomic Reconnection test\n";
$T = Process::Create ($EXEPREFIX . "Atomic_Reconnect".$EXE_EXT,
                     " -ORBSvcConf $cwd$DIR_SEPARATOR" . "mt.svc.conf");
if ($T->TimedWait (120) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nComplex filter\n";
$T = Process::Create ($EXEPREFIX . "Complex".$EXE_EXT,
                     " -ORBSvcConf $cwd$DIR_SEPARATOR" . "svc.complex.conf");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nControl test\n";
$T = Process::Create ($EXEPREFIX . "Control".$EXE_EXT,
                     " -ORBSvcConf $cwd$DIR_SEPARATOR" . "control.conf");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

exit $status;
