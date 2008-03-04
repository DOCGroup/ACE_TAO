eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

PerlACE::add_lib_path ('../lib/IBM1047_ISO8859/.');
PerlACE::add_lib_path ('../lib/UCS4_UTF16/.');

$iorfile = PerlACE::LocalFile ("server.ior");
unlink $iorfile;
$status = 0;

print STDOUT "Client using char translator\n\n";

my $class = (PerlACE::is_vxworks_test() ? 'PerlACE::ProcessVX' :
                                          'PerlACE::Process');
$SV = new $class ("server", " -ORBDottedDecimalAddresses 1");
$CL = new PerlACE::Process ("client", " -ORBSvcConf cs_test.conf");

$server = $SV->Spawn ();
if ($server != 0) {
    print STDERR "ERROR: Starting server returned $server\n";
    exit 1;
}
if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
} 

$client = $CL->SpawnWaitKill (60);
if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (15);
if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

print STDOUT "\nServer using char translator\n\n";

$SV2 = new $class ("server", " -ORBDottedDecimalAddresses 1 -ORBSvcConf cs_test.conf");
$CL2 = new PerlACE::Process ("client");

$server = $SV2->Spawn ();
if ($server != 0) {
    print STDERR "ERROR: Starting server returned $server\n";
    exit 1;
}
if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV2->Kill ();
    exit 1;
} 

$client2 = $CL2->SpawnWaitKill (60);
if ($client2 != 0) {
    print STDERR "ERROR: client returned $client2\n";
    $status = 1;
}

$server2 = $SV2->WaitKill (15);
if ($server2 != 0) {
    print STDERR "ERROR: server returned $server2\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
