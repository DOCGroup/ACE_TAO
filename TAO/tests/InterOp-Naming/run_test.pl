eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require Process;
require ACEutils;

$file="test.ior";

$port = ACE::uniqueid () + 10000;

print STDERR "\n\n==== InitRef test\n";

unlink $file;

$SV = Process::Create ($EXEPREFIX."INS_test_server".$EXE_EXT,
                       "-ORBendpoint iiop://localhost:$port "
                       . " -i object_name -o $file");

if (ACE::waitforfile_timed ($file, 3) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " random_service "
                       ."-ORBInitRef random_service="
                       ."iioploc://localhost:$port/object_name");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== Multi endpoint tests\n";

print STDERR "Grace period, waiting for the system to stabilize....";
sleep 5;

$port1 = $port + 1;
$port2 = $port + 2;

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " random_service "
                       . "-ORBInitRef random_service="
                       ."iioploc://"
                       ."localhost:$port1,"
                       ."localhost:$port2,"
                       ."localhost:$port"
                       ."/object_name");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== DefaultInitRef test\n";

print STDERR "Grace period, waiting for the system to stabilize....";
sleep 5;

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " object_name "
                       . "-ORBDefaultInitRef"
                       ."iioploc://"
                       ."localhost:$port1,"
                       ."localhost:$port2,"
                       ."localhost:$port/");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

$SV->Terminate (); $server = $SV->TimedWait (5);
if ($server == -1) {
  print STDERR "ERROR: server timedout\n";
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

unlink $file;
exit 0;
