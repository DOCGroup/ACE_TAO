eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../../bin';
require Process;
require ACEutils;

$file="test.ior";
$mtfile="mttest.ior";

print STDERR "\n\n==== Single-threaded test\n";

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       "-o $file");
if (ACE::waitforfile_timed ($file, 3) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."client".$Process::EXE_EXT,
		       " -x -k file://$file");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

if ($client == -1 || $server == -1) {
  exit 1;
}

unlink $file;

print STDERR "\n\n==== Multi-threaded test\n";

print STDERR "Grace period, waiting for the system to stabilize....";
sleep 5;

$SV = Process::Create ($EXEPREFIX."server".$Process::EXE_EXT,
                       " -o $mtfile");
if (ACE::waitforfile_timed ($mtfile, 3) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."mt_client".$Process::EXE_EXT,
		       " -x -k file://$mtfile -ORBsvcconf svc.mt.conf");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
}

$server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
}

unlink $mtfile;

if ($client == -1 || $server == -1) {
  exit 1;
}

exit 0;
