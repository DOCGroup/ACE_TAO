eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

PerlACE::check_privilege_group();

my $nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $nt_service = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $consumer   = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $supplier   = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
$consumer->AddLibPath ("../../lib");
$supplier->AddLibPath ("../../lib");

my $supplier_ior = "supplier.ior";
my $naming_ior   = "naming.ior";
my $notify_ior   = "notify.ior";
my $notify_conf  = "notify$PerlACE::svcconf_ext";

my $ns_naming_ior  = $nm_service->LocalFile ($naming_ior);
my $ts_notify_ior  = $nt_service->LocalFile ($notify_ior);
my $ts_notify_conf = $nt_service->LocalFile ($notify_conf);
my $supplier_supplier_ior = $supplier->LocalFile ($supplier_ior);
my $consumer_supplier_ior = $consumer->LocalFile ($supplier_ior);
$nm_service->DeleteFile ($naming_ior);
$nt_service->DeleteFile ($notify_ior);
$supplier->DeleteFile ($supplier_ior);
$consumer->DeleteFile ($supplier_ior);

$status = 0;

my $port = $nm_service->RandomPort();
my $host = $nm_service->HostName();

my $initref = "-ORBInitRef NameService=iioploc://$host:$port/NameService ";

$NS = $nm_service->CreateProcess ("../../../../Naming_Service/Naming_Service",
                                  "-ORBEndpoint iiop://$host:$port -o $ns_naming_ior");
$TS = $nt_service->CreateProcess ("../../../../Notify_Service/tao_cosnotification", $initref.
                                  "-IORoutput $ts_notify_ior -ORBSvcConf $ts_notify_conf");
$STS = $supplier->CreateProcess ("Structured_Supplier", $initref.
                                 "-o $supplier_supplier_ior");
$STC = $consumer->CreateProcess ("Structured_Consumer");
$SES = $supplier->CreateProcess ("Sequence_Supplier", $initref."-o $supplier_supplier_ior");
$SEC = $consumer->CreateProcess ("Sequence_Consumer");

my $consumer_args = $initref; # ."-k $consumer_supplier_ior";

sub run_test {
    local($name, $args_suffix, $SP, $CN, $tm) = @_;
    print $name;

    $supplier->DeleteFile ($supplier_ior);
    $consumer->DeleteFile ($supplier_ior);

    my $test_status = 0;

    my $result = $SP->Spawn ();
    if ($result != 0) {
        print STDERR "ERROR: supplier returned $result\n";
        return 1;
    }

    if ($supplier->WaitForFileTimed ($supplier_ior, $supplier->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: waiting for the supplier to start\n";
        $SP->Kill (); $SP->TimedWait (1);
        return 1;
    }

    if ($supplier->GetFile ($supplier_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$supplier_supplier_ior>\n";
        $SP->Kill (); $SP->TimedWait (1);
        return 1;
    }

    if ($consumer->PutFile ($supplier_ior) == -1) {
        print STDERR "ERROR: cannot set file <$consumer_supplier_ior>\n";
        $SP->Kill (); $SP->TimedWait (1);
        return 1;
    }

    $CN->Arguments ($consumer_args.$args_suffix);
    print $tm."\n";
    print $CN->CommandLine ()."\n";
    $result = $CN->SpawnWaitKill ($consumer->ProcessStartWaitInterval() + $tm);
    print "\n";
    if ($result != 0) {
        print STDERR "ERROR: consumer returned $result\n";
        $test_status = 1;
    }

    $result = $SP->WaitKill ($supplier->ProcessStopWaitInterval());

    if ($result != 0) {
        print STDERR "ERROR: supplier returned $result\n";
        $test_status = 1;
    }

    return $test_status;
}

my $result = $NS->Spawn ();

if ($result != 0) {
    print STDERR "ERROR: naming service returned $result\n";
    exit 1;
}


if ($nm_service->WaitForFileTimed ($naming_ior, $nm_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: waiting for the naming service to start\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$result = $TS->Spawn ();

if ($result != 0) {
    print STDERR "ERROR: notify service returned $result\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

if ($nt_service->WaitForFileTimed ($notify_ior, $nt_service->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: waiting for the notify service to start\n";
    $TS->Kill (); $TS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$result = run_test ("**** Structured Supplier -> 2 Structured Consumers ****\n",
                    "", $STS, $STC, 285);
if ($result != 0) {
    $TS->Kill (); $TS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$result = run_test ("**** Structured Supplier -> Structured Consumer (filtered) ****\n",
                    " -f -c 1", $STS, $STC, 45);
if ($result != 0) {
    $TS->Kill (); $TS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$result = run_test ("**** Sequence Supplier -> 2 Sequence Consumers ****\n",
                    "", $SES, $SEC, 45);
if ($result != 0) {
    $TS->Kill (); $TS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$result = run_test ("**** Sequence Supplier -> Sequence Consumer (filtered) ****\n",
                    " -f -c 1", $SES, $SEC, 45);
if ($result != 0) {
    $TS->Kill (); $TS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$TS->Kill (); $TS->TimedWait (1);
$NS->Kill (); $NS->TimedWait (1);

$nm_service->DeleteFile ($naming_ior);
$nt_service->DeleteFile ($notify_ior);
$supplier->DeleteFile ($supplier_ior);
$consumer->DeleteFile ($supplier_ior);

exit $status;
