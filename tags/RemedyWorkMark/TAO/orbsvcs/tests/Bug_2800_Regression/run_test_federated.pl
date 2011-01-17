eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# This is a Perl script that runs a Naming Service test.  It starts
# all the servers and clients as necessary.
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use Cwd;

$status = 0;
$debug_level = '0';
$quiet = 0;

if ($ARGV[0] eq '-q') {
    $quiet = 1;
}

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

# Variables for command-line arguments to client and server
# executables.
my $ns_orb_port1 = 9931;
my $ns_orb_port2 = 9932;
my $ns_endpoint1 = "iiop://localhost:$ns_orb_port1";
my $ns_endpoint2 = "iiop://localhost:$ns_orb_port2";
my $ns1_ior = "ns1.ior";
my $ns2_ior = "ns2.ior";
my $iorbase = "test.ior";

my $ns1 = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $ns2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $server = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $client = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $server_iorfile = $server->LocalFile ($iorbase);
my $ns1_iorfile = $ns1->LocalFile ($ns1_ior);
my $ns2_iorfile = $ns2->LocalFile ($ns2_ior);
$server->DeleteFile ($iorbase);
$ns1->DeleteFile ($ns1_ior);
$ns2->DeleteFile ($ns2_ior);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $client->CreateProcess ("client", "");

$NS1 = $ns1->CreateProcess("../../Naming_Service/tao_cosnaming",
                           "-ORBEndPoint $ns_endpoint1 -o $ns1_iorfile -m 0");
$NS2 = $ns2->CreateProcess("../../Naming_Service/tao_cosnaming",
                           "-ORBEndPoint $ns_endpoint2 -o $ns2_iorfile -m 0");

# Run two Naming Servers
$process_status = $NS1->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: ns1 returned $process_status\n";
    exit 1;
}

if ($ns1->WaitForFileTimed ($ns1_ior,
                            $ns1->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns1_iorfile>\n";
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

$process_status = $NS2->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: ns2 returned $process_status\n";
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

if ($ns2->WaitForFileTimed ($ns2_ior,
                            $ns2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns2_iorfile>\n";
    $NS2->Kill (); $NS2->TimedWait (1);
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

$process_status = $SV->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    $NS2->Kill (); $NS2->TimedWait (1);
    $NS1->Kill (); $NS1->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $NS2->Kill (); $NS2->TimedWait (1);
    $NS1->Kill (); $NS1->TimedWait (1);
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

print STDERR "Killing second naming server\n";
$NS2->Kill ();

print STDERR "Starting client\n";
$process_status = $CL->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: client returned $process_status\n";
    $status = 1;
}

sleep (15);

$process_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($process_status != 0) {
    print STDERR "ERROR: server returned $process_status\n";
    $status = 1;
}

$NS1->Kill ();

$process_status = $CL->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($process_status != 0) {
    print STDERR "ERROR: client returned $process_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$ns1->DeleteFile ($ns1_ior);
$ns2->DeleteFile ($ns2_ior);

exit $status;
