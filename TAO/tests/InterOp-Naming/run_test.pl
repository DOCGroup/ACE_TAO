eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

unshift @INC, '../../../bin';
require Process;
require ACEutils;
use Cwd;

$cwd  = getcwd();
$file = "$cwd$DIR_SEPARATOR" . "test.ior";
$port = ACE::uniqueid () + 10001;  # This must be > 10000 for Chorus 4.0
$hostname = "localhost";

for($i = 0; $i <= $#ARGV; $i++) {
  if ($ARGV[$i] eq '-chorus') {
    $i++;
    if (defined $ARGV[$i]) {
      $EXEPREFIX = "rsh $ARGV[$i] arun $cwd$DIR_SEPARATOR";
      $hostname = $ARGV[$i];
    }
    else {
      print STDERR "The -chorus option requires the hostname of the target\n";
      exit(1);
    }
  }
}

print STDERR "\n\n==== InitRef test\n";

unlink $file;
$SV = Process::Create ($EXEPREFIX."INS_test_server".$EXE_EXT,
                       "-ORBEndpoint iiop://$hostname:$port "
                       . " -i object_name -o $file");

if (ACE::waitforfile_timed ($file, 5) == -1) {
  print STDERR "ERROR: cannot find file <$file>\n";
  $SV->Kill (); $SV->TimedWait (1);
  exit 1;
}

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " random_service "
                       ."-ORBInitRef random_service="
                       ."iioploc://$hostname:$port/object_name");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== InvalidName test\n";

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " not_a_service "
                       ."-ORBInitRef random_service="
                       ."iioploc://$hostname:$port/object_name");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== DefaultInitRef test\n";

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " object_name "
                       . "-ORBDefaultInitRef"
                       ." iioploc://$hostname:$port");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== Multi endpoint test\n";

$port1 = $port + 1;
$port2 = $port + 2;

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " random_service "
                       . "-ORBInitRef random_service="
                       ."iioploc://"
                       ."$hostname:$port1,"
                       ."$hostname:$port2,"
                       ."$hostname:$port"
                       ."/object_name");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== Multi endpoint test with "
  . "CORBA::ORB::list_initial_services ()\n";

$port1 = $port + 1;
$port2 = $port + 2;

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " random_service "
                       . " -l "
                       . "-ORBInitRef random_service="
                       . "iioploc://"
                       . "$hostname:$port1,"
                       . "$hostname:$port2,"
                       . "$hostname:$port"
                       . "/object_name");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== Multi endpoint default ref test\n";

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " object_name "
                       . "-ORBDefaultInitRef "
                       ."iioploc://"
                       ."$hostname:$port1,"
                       ."$hostname:$port2,"
                       ."$hostname:$port");

$client = $CL->TimedWait (60);
if ($client == -1) {
  print STDERR "ERROR: client timedout\n";
  $CL->Kill (); $CL->TimedWait (1);
  $SV->Kill (); $SV->TimedWait (1);
  unlink $file;
  exit 1;
}

print STDERR "\n\n==== Default ref with final '/'\n";

$CL = Process::Create ($EXEPREFIX."INS_test_client".$EXE_EXT,
                       " object_name "
                       . "-ORBDefaultInitRef "
                       ."iioploc://"
                       ."$hostname:$port/");

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
