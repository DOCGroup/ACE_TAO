eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$threads='8';

$iorfile = "test.ior";
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -ORBsvcconf server.conf"
                       . " -o $iorfile"
		       . " -n $threads");

if (ACE::waitforfile_timed ($iorfile, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL1 = Process::Create ($EXEPREFIX."$client_process$EXE_EXT ",
			. " -k file://$iorfile"
			. " -i 100");
$CL2 = Process::Create ($EXEPREFIX."$client_process$EXE_EXT ",
			. " -k file://$iorfile"
			. " -i 100");
$CL3 = Process::Create ($EXEPREFIX."$client_process$EXE_EXT ",
			. " -k file://$iorfile"
			. " -i 100");

$client1 = $CL1->TimedWait (60);
if ($client1 == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL1->Kill (); $CL1->TimedWait (1);
}

$client2 = $CL2->TimedWait (60);
if ($client2 == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL2->Kill (); $CL2->TimedWait (1);
}

$client3 = $CL3->TimedWait (60);
if ($client3 == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL3->Kill (); $CL3->TimedWait (1);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $iorfile;

if ($server != 0
    || $client1 != 0
    || $client2 != 0
    || $client3 != 0) {
  exit 1;
}

exit 0;
