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

my $service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $consumer = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $supplier = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "ec.ior";
my $service_iorfile = $service->LocalFile ($iorbase);
my $consumer_iorfile = $consumer->LocalFile ($iorbase);
my $supplier_iorfile = $supplier->LocalFile ($iorbase);
$service->DeleteFile($iorbase);
$consumer->DeleteFile($iorbase);
$supplier->DeleteFile($iorbase);

$SV = $service->CreateProcess ("Service", "-ORBdebuglevel $debug_level -o $service_iorfile");
$CN = $consumer->CreateProcess ("Consumer", "-k file://$consumer_iorfile");
$SP = $supplier->CreateProcess ("Supplier", "-k file://$supplier_iorfile");

$service_status = $SV->Spawn ();

if ($service_status != 0) {
    print STDERR "ERROR: service Spawn returned $service_status\n";
    exit 1;
}

if ($service->WaitForFileTimed ($iorbase,
                               $service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: service cannot find file <$service_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($service->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: service cannot retrieve file <$service_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($consumer->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: consumer cannot set file <$consumer_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($supplier->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: supplier cannot set file <$supplier_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$consumer_status = $CN->Spawn ();

if ($consumer_status != 0) {
    print STDERR "ERROR: consumer Spawn returned $consumer_status\n";
    $status = 1;
}

sleep 5;

$supplier_status = $SP->SpawnWaitKill ($supplier->ProcessStartWaitInterval() + 105);

if ($supplier_status != 0) {
    print STDERR "ERROR: supplier SpawnWaitKill returned $supplier_status\n";
    $status = 1;
}

$consumer_status = $CN->WaitKill ($consumer->ProcessStopWaitInterval());

if ($consumer_status != 0) {
    print STDERR "ERROR: consumer WaitKill returned $consumer_status\n";
    $status = 1;
}

$service_status = $SV->TerminateWaitKill ($service->ProcessStopWaitInterval());

if ($service_status != 0) {
    print STDERR "ERROR: service TerminateWaitKill returned $service_status\n";
    $status = 1;
}

$service->DeleteFile($iorbase);
$consumer->DeleteFile($iorbase);
$supplier->DeleteFile($iorbase);

exit $status;
