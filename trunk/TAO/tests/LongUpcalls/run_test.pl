eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require ACEutils;
use Cwd;

$cwd = getcwd();
$status = 0;

ACE::checkForTarget($cwd);

$iorfile = "$cwd$DIR_SEPARATOR" ."test.ior";
$svcfile = $cwd . $DIR_SEPARATOR . "svc.conf";

print STDERR "==== Server upcall waits for operations on other threads\n";

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."blocking_server$EXE_EXT",
                       " -ORBSvcConf " . $svcfile
                       . " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 30) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."blocking_client$EXE_EXT",
                       " -ORBSvcConf " . $svcfile
                       . " -k file://$iorfile ");

$client = $CL->TimedWait (240);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (30);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

print STDERR "==== Server upcall waits for AMI operations on other threads\n";

unlink $iorfile;
$SV = Process::Create ($EXEPREFIX."ami_server$EXE_EXT",
                       " -ORBSvcConf " . $svcfile
                       . " -o $iorfile");

if (ACE::waitforfile_timed ($iorfile, 30) == -1) {
  print STDERR "ERROR: cannot find file <$iorfile>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."blocking_client$EXE_EXT",
                       " -ORBSvcConf " . $svcfile
                       . " -k file://$iorfile ");

$client = $CL->TimedWait (240);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $status = 1;
}

$server = $SV->TimedWait (30);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  $status = 1;
}

unlink $iorfile;

exit $status;
