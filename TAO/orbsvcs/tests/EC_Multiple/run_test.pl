eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;

$NS_ior = "NameService.ior";
$sleeptime = 3;
$status = 0;

$NS = Process::Create ("../../Naming_Service/Naming_Service".$EXE_EXT,
		       " -o $NS_ior ");

sleep $sleeptime;

# This is a very simple test, no multiple consumers and no gateways.
$TEST = Process::Create ($EXEPREFIX."EC_Multiple".$EXE_EXT,
                  " -s local");
if ($TEST->TimedWait (60) == -1) {
  print STDERR "ERROR: test timedout\n";
  $status = 1;
  $TEST->Kill (); $TEST->TimedWait (1);
}

$NS->Kill (); $NS->TimedWait (1);

unlink $NS_ior;

# @@ Capture any errors from the server too.
exit $status;
