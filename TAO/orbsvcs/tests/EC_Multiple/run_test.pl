eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$NS_ior = "NameService.ior";
$status = 0;

$NS = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Naming_Service".$DIR_SEPARATOR.
                       "Naming_Service".$EXE_EXT,
                       " -o $NS_ior ");
if (ACE::waitforfile_timed ($NS_ior, 5) == -1) {
  print STDERR "ERROR: waiting for naming service IOR file\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

# This is a very simple test, no multiple consumers and no gateways.
$TEST = Process::Create ($EXEPREFIX."EC_Multiple".$EXE_EXT,
			 "-ORBNameServiceIOR file://$NS_ior"
			 ." -s local");
if ($TEST->TimedWait (60) == -1) {
  print STDERR "ERROR: test timedout\n";
  $status = 1;
  $TEST->Kill (); $TEST->TimedWait (1);
}

$NS->Terminate (); if ($NS->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate naming service\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}  

exit $status;
