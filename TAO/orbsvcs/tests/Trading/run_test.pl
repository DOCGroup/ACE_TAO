eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
require Process;

$ior = "trading.ior";
$sleeptime = 20;
$status = 0;

$TS = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Trading_Service".$DIR_SEPARATOR.
                       "Trading_Service".$EXE_EXT,
                       " -TSdumpior $ior ");

if (ACE::waitforfile_timed ($ior, $sleeptime) == -1) {
  print STDERR "ERROR: waiting for trading service IOR file\n";
  $TS->Kill (); $TS->TimedWait (1);
  exit 1;
}

$E = Process::Create ($EXEPREFIX."export_test".$EXE_EXT,
		      "-ORBTradingServiceIOR file://$ior");

sleep $sleeptime;

$I = Process::Create ($EXEPREFIX."import_test".$EXE_EXT,
		      "-ORBNameServiceIOR file://$ior");

if ($I->TimedWait (60) == -1) {
  $status = 1;
  print STDERR "ERROR: import test timedout\n";
  $I->Kill (); $I->TimedWait (1);
}

if ($E->TimedWait (60) == -1) {
  $status =1;
  print STDERR "ERROR: export test timedout\n";
  $E->Kill (); $E->TimedWait (1);
}

$TS->Terminate();
if ($TS->TimedWait (15) == -1) {
  print STDERR "ERROR: couldn't terminate the trading service nicely\n";
  $TS->Kill (); $TS->TimedWait (1);
  $status = 1;
}

unlink $ior;

exit $status;


