eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;

$iorfile = "test.ior";
$status = 0;

print STDERR "===== Base test, no crashes\n";
unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
		       " -k file://$iorfile"
		       . " -i 100");

$client = $CL->TimedWait (20);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

print STDERR "===== Client crash (abort) during upcall\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
		       " -k file://$iorfile"
		       . " -i 100 -s");

$client = $CL->TimedWait (20);
if ($client == -1) {
  print STDERR "ERROR: client (-s) timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

print STDERR "===== Client crash during upcall\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
		       " -k file://$iorfile"
		       . " -i 100 -z");

$client = $CL->TimedWait (20);
if ($client == -1) {
  print STDERR "ERROR: client (-z) timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

print STDERR "===== Server crash (abort) during upcall\n";

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
		       " -k file://$iorfile"
		       . " -i 100 -a");

$client = $CL->TimedWait (20);
if ($client == -1) {
  print STDERR "ERROR: client (-a) timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (20);
if ($server == -1) {
  print STDERR "ERROR: server (-a) timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

print STDERR "===== Server crash during upcall\n";

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
		       " -k file://$iorfile"
		       . " -i 100 -c");

$client = $CL->TimedWait (20);
if ($client == -1) {
  print STDERR "ERROR: client (-c) timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (20);
if ($server == -1) {
  print STDERR "ERROR: server (-c) timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

exit $status;
