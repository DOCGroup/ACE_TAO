eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

print STDERR "================ Collocated tests, single threaded\n";

$T = Process::Create ($EXEPREFIX."ECT_Throughput".$EXE_EXT,
		      " -ORBsvcconf ec.conf -m new -u 10000 -n 1 -t 0"
		      . " -c 4");

if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: test timedout\n";
  $T->Kill (); $T->TimedWait (1);
  exit 1;
}

print STDERR "================ Collocated tests, single threaded\n";

$T = Process::Create ($EXEPREFIX."ECT_Throughput".$EXE_EXT,
		      " -ORBsvcconf ec.mt.conf -m new -u 10000 -n 1 -t 0"
		      . " -c 4");

if ($T->TimedWait (60) == -1) {
  print STDERR "ERROR: test timedout\n";
  $T->Kill (); $T->TimedWait (1);
  exit 1;
}

print STDERR "================ Remote test\n";

$ns_ior = "NameService.ior";

unlink $ns_ior;
$NS = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Naming_Service".$DIR_SEPARATOR.
                       "Naming_Service".$EXE_EXT,
                       " -o $ns_ior ");

if (ACE::waitforfile_timed ($ns_ior, 5) == -1) {
  print STDERR "ERROR: cannot find file <$ns_ior>\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$ES = Process::Create ("..".$DIR_SEPARATOR.
                       "..".$DIR_SEPARATOR.
                       "Event_Service".$DIR_SEPARATOR.
                       "Event_Service".$EXE_EXT,
                       " -ORBNameServiceIOR file://$ns_ior "
                       ." -ORBSvcConf ec.conf "
		       . " -t NEW");

sleep 5;

$C = Process::Create ($EXEPREFIX."ECT_Consumer".$EXE_EXT,
		      " -ORBNameServiceIOR file://$ns_ior "
		      . " -c 4 -s 1");

$S = Process::Create ($EXEPREFIX."ECT_Supplier".$EXE_EXT,
		      " -ORBNameServiceIOR file://$ns_ior "
		      . " -s 1 -u 10000 -n 1 -t 0");

if ($S->TimedWait (60) == -1) {
  print STDERR "ERROR: supplier timedout\n";
  $S->Kill (); $S->TimedWait (1);
  $C->Kill (); $C->TimedWait (1);
  $ES->Kill (); $ES->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

if ($C->TimedWait (60) == -1) {
  print STDERR "ERROR: consumer timedout\n";
  $C->Kill (); $C->TimedWait (1);
  $ES->Kill (); $ES->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

if ($ES->TimedWait (5) == -1) {
  print STDERR "ERROR: event channel timedout\n";
  $ES->Kill (); $ES->TimedWait (1);
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$NS->Terminate (); if ($NS->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate naming service\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

unlink $ns_ior;

# @@ Capture the errors from the processes.
exit 0;
