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

print STDERR "\n\nShutdown EC with clients still attached\n";
$T = Process::Create ($prefix . "Shutdown".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nDisconnect callbacks test\n";
$T = Process::Create ($prefix . "Disconnect".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nMT Disconnects test\n";
$T = Process::Create ($prefix . "MT_Disconnect".$EXE_EXT);
if ($T->TimedWait (240) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nPush Events\n";
$T = Process::Create ($prefix . "Push_Event".$EXE_EXT);
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

print STDERR "\n\nPull-Push Events\n";
$T = Process::Create ($prefix . "Pull_Push_Event".$EXE_EXT,
                      " -ORBSvcConf svc.pull.conf");
if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: Test timedout\n";
  $status = 1;
  $T->Kill (); $T->TimedWait (1);
}

exit $status;
