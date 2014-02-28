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

## Specify and endpoint so that we may test the corbaloc in the
## -ORBInitRef.  We retain one -ORBInitRef using the file just to test
## both ways.

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $import_test = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $export_test = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$port = $server->RandomPort();

my $iorbase = "trading.ior";
my $ready = "export_test_ready";

my $server_iorfile = $server->LocalFile ($iorbase);
my $import_iorfile = $import_test->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$import_test->DeleteFile($iorbase);

#For ready file
my $export_ready_file = $export_test->LocalFile ($ready);
$export_test->DeleteFile($ready);

$SV = $server->CreateProcess ("../../Trading_Service/tao_costrading",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBEndpoint iiop://:$port " .
                              "-TSdumpior $server_iorfile");

$CL1 = $export_test->CreateProcess ("export_test",
                                   "-ORBInitRef TradingService=corbaloc:::$port/TradingService " .
                                   "-d $export_ready_file " .
                                   "-quiet");

$CL2 = $import_test->CreateProcess ("import_test",
                                   "-ORBInitRef TradingService=file://$import_iorfile " .
                                   "-quiet");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL1->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($export_test->WaitForFileTimed ($ready,
                               $export_test->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    $CL1->Kill (); $CL1->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($import_test->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$import_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL2->SpawnWaitKill ($import_test->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$CL1->Kill ();
$SV->Kill ();

$server->DeleteFile($iorbase);
$import_test->DeleteFile($iorbase);
$export_test->DeleteFile($ready);

exit $status;
