eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
require Process;

$NS_ior = "NameService.ior";
$sleeptime = 5;
$status = 0;

$NS = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Naming_Service".$DIR_SEPARATOR.
                       "Naming_Service".$EXE_EXT,
                       " -o $NS_ior ");
sleep $sleeptime;

$ES = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Event_Service".$DIR_SEPARATOR.
                       "Event_Service".$EXE_EXT,
		       "-ORBNameServiceIOR file://$NS_ior");

$C = Process::Create ($EXEPREFIX."ECM_Consumer".$EXE_EXT,
		      "-ORBNameServiceIOR file://$NS_ior");

sleep $sleeptime;

$S = Process::Create ($EXEPREFIX."ECM_Supplier".$EXE_EXT,
		      "-ORBNameServiceIOR file://$NS_ior");

if ($C->TimedWait (60) == -1) {
  $status = 1;
  print STDERR "ERROR: consumer timedout\n";
  $C->Kill (); $C->TimedWait (1);
}

if ($S->TimedWait (60) == -1) {
  $status =1;
  print STDERR "ERROR: supplier timedout\n";
  $S->Kill (); $S->TimedWait (1);
}

$NS->Terminate();
$ES->Terminate();
if ($NS->TimedWait (5) == -1 || $ES->TimedWait (5) == -1) {
  print STDERR "ERROR: couldn't terminate the services nicely\n";
  $NS->Kill (); $NS->TimedWait (1);
  $ES->Kill (); $ES->TimedWait (1);
  $status = 1;
}

unlink $NS_ior;

# @@ Capture the errors from the processes.
exit $status;


