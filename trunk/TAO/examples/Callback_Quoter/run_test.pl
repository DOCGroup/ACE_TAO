eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require Process;
require ACEutils;

print STDERR "================ Remote test\n";

$ns_ior = "NameService.ior";

unlink $ns_ior;
$NS = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
		       "orbsvcs".$DIR_SEPARATOR.
                       "Naming_Service".$DIR_SEPARATOR.
                       "Naming_Service".$EXE_EXT,
                       " -o $ns_ior ");

if (ACE::waitforfile_timed ($ns_ior, 5) == -1) {
  print STDERR "ERROR: cannot find file <$ns_ior>\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$N = Process::Create (".".$DIR_SEPARATOR.
		      "notifier".$EXE_EXT,
		      " -ORBNameServiceIOR file://$ns_ior ");

sleep 5;

$C = Process::Create (".".$DIR_SEPARATOR.
		      "consumer".$EXE_EXT,
		      " -ORBNameServiceIOR file://$ns_ior "
		      . " -t 12 -a TAO");

sleep 5;

$S = Process::Create (".".$DIR_SEPARATOR.
		      "supplier".$EXE_EXT,
		      " -ORBNameServiceIOR file://$ns_ior "
		      . " -iexample.stocks");

if ($S->TimedWait (60) == -1) {
  print STDERR "ERROR: supplier timedout\n";
  $S->Kill (); $S->TimedWait (1);
  $C->Kill (); $C->TimedWait (1);
  $N->Kill (); $N->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$C->Terminate (); if ($C->TimedWait (5) == -1) {
  print STDERR "ERROR: consumer timedout\n";
  $C->Kill (); $C->TimedWait (1);
  $N->Kill (); $N->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$N->Terminate (); if ($N->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate notifier\n";
  $N->Kill (); $N->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$NS->Terminate (); if ($NS->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate naming service\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

unlink $ns_ior;

exit 0;
