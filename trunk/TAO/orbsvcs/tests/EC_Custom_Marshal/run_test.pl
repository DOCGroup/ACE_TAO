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
                       "Naming_Service".$Process::EXE_EXT,
                       " -o $NS_ior ");
sleep $sleeptime;

$ES = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Event_Service".$DIR_SEPARATOR.
                       "Event_Service".$Process::EXE_EXT, "");

$C = Process::Create ($EXEPREFIX."ECM_Consumer".$Process::EXE_EXT, "");

sleep $sleeptime;

$S = Process::Create ($EXEPREFIX."ECM_Supplier".$Process::EXE_EXT, "");

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

$NS->Kill (); $NS->TimedWait (1);
$ES->Kill (); $ES->TimedWait (1);

unlink $NS_ior;

# @@ Capture the errors from the processes.
exit $status;


