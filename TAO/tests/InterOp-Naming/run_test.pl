eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$TARGETHOSTNAME = "localhost";

$file = PerlACE::LocalFile ("test.ior");
$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0

$status = 0;

print STDERR "\n\n==== InitRef test\n";

unlink $file;
$SV = new PerlACE::Process ("INS_test_server",
                       "-ORBEndpoint iiop://1.0@"."$TARGETHOSTNAME:$port "
                       . " -i object_name -o $file -ORBDottedDecimalAddresses 1");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($file, 10) == -1) {
    print STDERR "ERROR: cannot find file <$file>\n";
    $SV->Kill (); 
    exit 1;
}

$CL = new PerlACE::Process ("INS_test_client",
                       " random_service "
                       ."-ORBInitRef random_service="
                       ."corbaloc::1.1@"."$TARGETHOSTNAME:$port/object_name");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "\n\n==== InvalidName test\n";

$CL = new PerlACE::Process ("INS_test_client",
                     " not_a_service "
                       ."-ORBInitRef random_service="
                       ."corbaloc:iiop:1.0@"."$TARGETHOSTNAME:$port/object_name");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "\n\n==== DefaultInitRef test\n";

$CL = new PerlACE::Process ("INS_test_client",
                       " object_name "
                       . "-ORBDefaultInitRef"
                       ." corbaloc:iiop:1.0@"."$TARGETHOSTNAME:$port/");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "\n\n==== Multi endpoint test\n";

$port1 = $port + 1;
$port2 = $port + 2;

$CL = new PerlACE::Process ("INS_test_client",
                       " random_service "
                       . "-ORBInitRef random_service="
                       ."corbaloc:"
                       ."iiop:1.0@"."$TARGETHOSTNAME:$port1,"
                       .":1.0@"."$TARGETHOSTNAME:$port2,"
                       ."iiop:1.0@"."$TARGETHOSTNAME:$port"
                       ."/object_name");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "\n\n==== Multi endpoint test with CORBA::ORB::list_initial_services ()\n";

$port1 = $port + 1;
$port2 = $port + 2;

$CL = new PerlACE::Process ("INS_test_client",
                       " random_service "
                       . " -l "
                       . "-ORBInitRef random_service="
                       . "corbaloc:"
                       . "iiop:1.0@"."$TARGETHOSTNAME:$port1,"
                       . "iiop:1.0@"."$TARGETHOSTNAME:$port2,"
                       . "iiop:1.0@"."$TARGETHOSTNAME:$port"
                       . "/object_name");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "\n\n==== Multi endpoint default ref test\n";

$CL = new PerlACE::Process ("INS_test_client",
                       " object_name "
                       . "-ORBDefaultInitRef "
                       ."corbaloc:"
                       ."iiop:1.0@"."$TARGETHOSTNAME:$port1,"
                       ."iiop:1.0@"."$TARGETHOSTNAME:$port2,"
                       ."iiop:1.0@"."$TARGETHOSTNAME:$port/");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

print STDERR "\n\n==== Default ref with final '/'\n";

$CL = new PerlACE::Process ("INS_test_client",
                         " object_name "
                       . "-ORBDefaultInitRef "
                       ."corbaloc:"
                       ."iiop:1.0@"."$TARGETHOSTNAME:$port/");

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $file;

exit $status;
