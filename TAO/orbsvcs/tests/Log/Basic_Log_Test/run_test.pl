eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $ns_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $lg_service = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "ns.ior";
my $ns_iorfile = $ns_service->LocalFile ($iorbase);
my $lg_iorfile = $lg_service->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$ns_service->DeleteFile ($iorbase);
$lg_service->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

$NS = $ns_service->CreateProcess ("../../../Naming_Service/Naming_Service",
                                  "-o $ns_iorfile");
$LS = $lg_service->CreateProcess ("../../../Logging_Service/Basic_Logging_Service/tao_tls_basic",
                                  "-ORBInitRef NameService=file://$lg_iorfile");
$CL = $client->CreateProcess ("client", "-ORBInitRef NameService=file://$client_iorfile");

print STDERR "Starting Naming Service\n";

$process_status = $NS->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: naming service returned $process_status\n";
    exit 1;
}

if ($ns_service->WaitForFileTimed ($iorbase,
                                   $ns_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($ns_service->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($lg_service->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$lg_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print STDERR "Starting Logging Service\n";

$process_status = $LS->Spawn ();

if ($process_status != 0) {
    print STDERR "ERROR: logging service returned $process_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# Give time for logging service to initialize and install its object
# reference in the nameing service.
sleep (10);

print STDERR "Starting client\n";

$process_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($process_status != 0) {
    print STDERR "ERROR: client returned $process_status\n";
    $status = 1;
}

$NS->Kill ();
$LS->Kill ();

$ns_service->DeleteFile ($iorbase);
$lg_service->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

exit $status;
