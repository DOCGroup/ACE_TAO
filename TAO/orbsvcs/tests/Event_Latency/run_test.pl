eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$NS_ior = "NameService.ior";
$sleeptime = 3;
$status = 0;

$NS = Process::Create ("..".$DIR_SEPARATOR
		       ."..".$DIR_SEPARATOR
		       ."Naming_Service".$DIR_SEPARATOR
		       ."Naming_Service".$EXE_EXT,
		       " -o $NS_ior ");

if (ACE::waitforfile_timed ($NS_ior, 5) == -1) {
  print STDERR "ERROR: waiting for naming service IOR file\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$ES = Process::Create ("..".$DIR_SEPARATOR
		       ."..".$DIR_SEPARATOR
		       ."Event_Service".$DIR_SEPARATOR
		       ."Event_Service".$EXE_EXT,
		       "-ORBNameServiceIOR file://$NS_ior");

sleep $sleeptime;

$TEST = Process::Create ($EXEPREFIX."Event_Latency".$EXE_EXT,
			 "-ORBNameServiceIOR file://$NS_ior"
			 ." -j -m 100");

if ($TEST->TimedWait (60) == -1) {
  print STDERR "ERROR: test timedout\n";
  $status = 1;
  $TEST->Kill (); $TEST->TimedWait (1);
}

$NS->Kill (); $NS->TimedWait (1);
$ES->Kill (); $ES->TimedWait (1);

unlink $NS_ior;

# @@ Capture any errors from the server too.
exit $status;
