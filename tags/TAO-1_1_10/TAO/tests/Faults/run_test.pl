eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile = "$cwd$DIR_SEPARATOR" . "test.ior";
$middlefile = "$cwd$DIR_SEPARATOR" . "middle.ior";
$dum_core = "$cwd$DIR_SEPARATOR" . "core";
$status = 0;

ACE::checkForTarget($cwd);

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

print STDERR "===== Three-way test, client crashes and server detects\n";

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."server$EXE_EXT ",
                       " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

unlink $middlefile;
$MD = Process::Create ($EXEPREFIX."middle$EXE_EXT ",
                       " -o $middlefile -k file://$iorfile");

if (ACE::waitforfile_timed ($middlefile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$middlefile>\n";
  $MD->Kill (); $MD->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -k file://$middlefile"
                       . " -i 100 -s");

$client = $CL->TimedWait (100);
if ($client == -1) {
  print STDERR "ERROR: client (middle/-s) timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$CL = Process::Create ($EXEPREFIX."client$EXE_EXT ",
                       " -k file://$middlefile"
                       . " -i 10 -x");

$client = $CL->TimedWait (100);
if ($client == -1) {
  print STDERR "ERROR: client (middle/-x) timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (20);
if ($server == -1) {
  print STDERR "ERROR: server (middle/-x) timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

$middle = $MD->TimedWait (20);
if ($middle == -1) {
  print STDERR "ERROR: middle (-x) timedout\n";
  $MD->Kill (); $MD->TimedWait (1);
  $status = 1;
}

if ($middle != 0) {
  print STDERR "ERROR: the middle test failed\n";
  $status = 1;
}

print STDERR "===== Ping-pong test, server crashes but client continues\n";

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."ping$EXE_EXT ",
                       " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 15) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."pong$EXE_EXT ",
                       " -k file://$iorfile"
                       . " -p 100 -i 60 -t 30");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: pong timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (60);
if ($server == -1) {
  print STDERR "ERROR: ping timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}
unlink $dum_core;

exit $status;
