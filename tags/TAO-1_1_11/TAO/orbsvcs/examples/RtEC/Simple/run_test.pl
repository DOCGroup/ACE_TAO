eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../../bin';
require Process;
require ACEutils;

$iorfile = "ec.ior";

unlink $iorfile;

$T = Process::Create ($EXEPREFIX."Service".$EXE_EXT,
		      " -ORBsvcconf ec.conf -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $NS->Kill (); $NS->TimedWait (1);
  exit 1;
}

$C = Process::Create ($EXEPREFIX."Consumer".$EXE_EXT,
		      " file://$iorfile");

sleep 5;

$S = Process::Create ($EXEPREFIX."Supplier".$EXE_EXT,
		      " file://$iorfile");

if ($S->TimedWait (120) == -1) {
  print STDERR "ERROR: supplier timedout\n";
  $S->Kill (); $S->TimedWait (1);
  $C->Kill (); $C->TimedWait (1);
  $T->Kill (); $T->TimedWait (1);
  exit 1;
}

if ($C->TimedWait (15) == -1) {
  print STDERR "ERROR: consumer timedout\n";
  $C->Kill (); $C->TimedWait (1);
  $T->Kill (); $T->TimedWait (1);
  exit 1;
}

$T->Terminate (); if ($T->TimedWait (5) == -1) {
  print STDERR "ERROR: cannot terminate service\n";
  $T->Kill (); $T->TimedWait (1);
  exit 1;
}

unlink $iorfile;

exit 0;
