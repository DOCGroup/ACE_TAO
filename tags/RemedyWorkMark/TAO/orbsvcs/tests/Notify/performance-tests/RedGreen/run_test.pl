eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
PerlACE::check_privilege_group();

$status = 0;

@test_configs = ( "reactive.conf", "lookup.conf", "listener.conf" );

my $nm_service = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $test = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $nt_service = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

$test->AddLibPath ('../../lib');

my $notify_ior = "notify.ior";
my $naming_ior = "naming.ior";

my $nm_service_nmiorfile = $nm_service->LocalFile ($naming_ior);
my $nt_service_ntiorfile = $nt_service->LocalFile ($notify_ior);
my $nt_service_nmiorfile = $nt_service->LocalFile ($naming_ior);
my $test_nmiorfile = $test->LocalFile ($naming_ior);

$nm_service->DeleteFile ($naming_ior);
$nt_service->DeleteFile ($notify_ior);
$nt_service->DeleteFile ($naming_ior);
$test->DeleteFile ($naming_ior);

$NM_SV = $nm_service->CreateProcess ("../../../../Naming_Service/tao_cosnaming", "-o $nm_service_nmiorfile");
$T = $test->CreateProcess ("RedGreen_Test", "-ORBInitRef NameService=file://$test_nmiorfile");
$NT_SV = $nt_service->CreateProcess ("../../../../Notify_Service/tao_cosnotification", "");

$nm_service_status = $NM_SV->Spawn ();

if ($nm_service_status != 0) {
    print STDERR "ERROR: Naming service returned $nm_service_status\n";
    exit 1;
}

if ($nm_service->WaitForFileTimed ($naming_ior,
                                   $nm_service->ProcessStartWaitInterval() + 45) == -1) {
    print STDERR "ERROR: cannot find file <$nm_service_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

sleep (10);

if ($nm_service->GetFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nm_service_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($test->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$test_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

if ($nt_service->PutFile ($naming_ior) == -1) {
    print STDERR "ERROR: cannot set file <$nt_service_nmiorfile>\n";
    $NM_SV->Kill (); $NM_SV->TimedWait (1);
    exit 1;
}

for $config (@test_configs) {
    print STDERR "\nTesting Notification Service with config file = $config ....\n\n";

    $NT_SV->Arguments("-ORBInitRef NameService=file://$nt_service_nmiorfile " .
                      "-IORoutput $nt_service_ntiorfile -ORBSvcConf $config");

    $nt_service_status = $NT_SV->Spawn ();

    if ($nt_service_status != 0) {
        print STDERR "ERROR: Notify service returned $nt_service_status\n";
        $NM_SV->Kill (); $NM_SV->TimedWait (1);
        exit 1;
    }

    if ($nt_service->WaitForFileTimed ($notify_ior,
                                       $nt_service->ProcessStartWaitInterval() + 45) == -1) {
        print STDERR "ERROR: cannot find file <$nt_service_ntiorfile>\n";
        $NT_SV->Kill (); $NT_SV->TimedWait (1);
        $NM_SV->Kill (); $NM_SV->TimedWait (1);
        exit 1;
    }

    sleep(10);

    $test_status = $T->SpawnWaitKill ($test->ProcessStartWaitInterval() + 45);

    if ($test_status != 0) {
        print STDERR "ERROR: test returned $test_status\n";
        $status = 1;
    }

    $NT_SV->Kill ();
}

$NT_SV->Kill ();
$NM_SV->Kill ();

$nm_service->DeleteFile ($naming_ior);
$nt_service->DeleteFile ($notify_ior);
$nt_service->DeleteFile ($naming_ior);
$test->DeleteFile ($naming_ior);

exit $status;
