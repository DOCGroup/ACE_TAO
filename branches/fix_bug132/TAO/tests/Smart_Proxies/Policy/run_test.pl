eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$iorfile1 = "$cwd$DIR_SEPARATOR" . "test1.ior";
$iorfile2 = "$cwd$DIR_SEPARATOR" . "test2.ior";

ACE::checkForTarget($cwd);

unlink $iorfile1;
unlink $iorfile2;

print STDERR "\nrunning Smart Proxy test consisting of the client and two servers with <one-shot> policy\n\n";

$SV1 = Process::Create ($EXEPREFIX."server".$EXE_EXT,
		       " -o $iorfile1");

if (ACE::waitforfile_timed ($iorfile1, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile1>\n";
  $SV1->Kill (); $SV1->TimedWait (1);
  exit 1;
}

$SV2 = Process::Create ($EXEPREFIX."server".$EXE_EXT,
		       " -o $iorfile2");

if (ACE::waitforfile_timed ($iorfile2, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile2>\n";
  $SV2->Kill (); $SV2->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ($EXEPREFIX."client$EXE_EXT",
                        "-i file://$iorfile1 -j file://$iorfile2");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server1 = $SV1->TimedWait (10);
if ($server1 == -1) {
  print STDERR "ERROR: server1 timedout\n";
  $SV1->Kill (); $SV1->TimedWait (1);
}

unlink $iorfile1;

$server2 = $SV2->TimedWait (10);
if ($server2 == -1) {
  print STDERR "ERROR: server2 timedout\n";
  $SV2->Kill (); $SV2->TimedWait (1);
}

unlink $iorfile2;


print STDERR "\nrunning Smart Proxy test consisting of the client and two servers with <one-shot> policy disabled\n\n";

$SV1 = Process::Create ($EXEPREFIX."server".$EXE_EXT,
		       " -o $iorfile1");

if (ACE::waitforfile_timed ($iorfile1, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile1>\n";
  $SV1->Kill (); $SV1->TimedWait (1);
  exit 1;
}

$SV2 = Process::Create ($EXEPREFIX."server".$EXE_EXT,
		       " -o $iorfile2");

if (ACE::waitforfile_timed ($iorfile2, 5) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile2>\n";
  $SV2->Kill (); $SV2->TimedWait (1);
  exit 1;
}

$CL  = Process::Create ($EXEPREFIX."client$EXE_EXT",
                        "-f0 -i file://$iorfile1 -j file://$iorfile2");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server1 = $SV1->TimedWait (10);
if ($server1 == -1) {
  print STDERR "ERROR: server1 timedout\n";
  $SV1->Kill (); $SV1->TimedWait (1);
}

unlink $iorfile1;

$server2 = $SV2->TimedWait (10);
if ($server2 == -1) {
  print STDERR "ERROR: server2 timedout\n";
  $SV2->Kill (); $SV2->TimedWait (1);
}

unlink $iorfile2;

if ($client != 0 || server != 0) {
  exit 1;
}
