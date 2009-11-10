eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;

$port = PerlACE::uniqueid () + 10001;  # This can't be 10000 for Chorus 4.0
$status = 0;

if (PerlACE::is_vxworks_test()) {
  $TARGETHOSTNAME = $ENV{'ACE_RUN_VX_TGTHOST'};
  $SV  = new PerlACE::ProcessVX ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:$port -o $iorfile");
}
else {
  $TARGETHOSTNAME = "localhost";
  $SV  = new PerlACE::Process ("server", "-ORBEndpoint iiop://$TARGETHOSTNAME:$port -o $iorfile");
}

# Client 1 expects OBJECT_NOT_EXIST exception ("-e 1")
$CL1 = new PerlACE::Process ("client", 
                             "-k corbaloc::$TARGETHOSTNAME:$port/Simple_Server -e 1 " 
                             . "-ORBForwardInvocationOnObjectNotExist 0");
                             
# Client 2 does not expects OBJECT_NOT_EXIST exception ("-e 0")
$CL2 = new PerlACE::Process ("client", 
                             "-x -k corbaloc::$TARGETHOSTNAME:$port/Simple_Server -e 0 "
                             . "-ORBForwardInvocationOnObjectNotExist 1");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

print STDERR "==== Running first client with -ORBForwardInvocationOnObjectNotExist 0\n";

$client = $CL1->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 1 returned $client\n";
    $status = 1;
}

print STDERR "==== Running second client with -ORBForwardInvocationOnObjectNotExist 1\n";

$client = $CL2->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 2 returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (15);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
