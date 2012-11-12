eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;

my $name_manager = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client      = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

## The LoadManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$NM_conf = $name_manager->LocalFile ("windows" . $PerlACE::svcconf_ext);

my $name_mgr_iorbase = "lm.ior";
my $name_iorbase = "nm.ior";

my $name_manager_iorfile = $name_manager->LocalFile ($name_mgr_iorbase);
my $name_server_iorfile = $name_manager->LocalFile ($name_iorbase);

my $naming_mgr_client_iorfile = $client->LocalFile ($name_mgr_iorbase);
my $name_client_iorfile = $client->LocalFile ($name_iorbase);

$name_manager->DeleteFile($name_mgr_iorbase);
$name_manager->DeleteFile($name_iorbase);
$client->DeleteFile($name_mgr_iorbase);
$client->DeleteFile($name_iorbase);


$NM = $name_manager->CreateProcess ("../../orbsvcs/Naming_Service/tao_ft_naming",
        "-f persist.dat -g $name_manager_iorfile -o $name_server_iorfile " .
        "-ORBDebugLevel 0 " .
        ($^O eq 'MSWin32' ? " -ORBSvcConf $NM_conf" : ''));


#$NS = $name_server->CreateProcess ("../../orbsvcs/Naming_Service/tao_cosnaming",
#        "-o $name_server_iorfile " .
#        ($^O eq 'MSWin32' ? " -ORBSvcConf $NS_conf" : ''));

$NM_REF       = "-ORBInitRef NameService=file://$name_client_iorfile";
$RM_REF       = "-ORBInitRef NamingManager=file://$naming_mgr_client_iorfile";
$DEBUG_LEVEL  = "-ORBDebugLevel 0";
$LOAD_ARG     = "$NM_REF $RM_REF $DEBUG_LEVEL";

##  group_create  -group <group> -policy <round | rand | least> -type_id <type_id>
$CL1 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "group_create " .
        "-group group1 " .
        "-policy round " .
        "-type_id IDL:omg.org/CORBA/Object:1.0");

##  group_bind -group <group> -namepath <path>
$CL2 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "group_bind -group group1 -n group_name");

##  group_modify -group <group> -policy <round | rand | least>
$CL3 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "group_modify -group group1 -policy rand");
##  member_add -group <group> -member <member> -location <location> -ior <IOR>
$CL4 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "member_add -group group1 -member member1 -location dhcp34.ociweb.com " .
        "-ior IOR:010000002300000049444c3a4741562f4f626a6563745265666572656e6365466163746f72793a312e3000000100000000000000b2000000010102000e00000031302e33352e3232352e313530005046530000003a3e0232311c756e636c6173735f757374696c2e6363692d612e6c6f636174696f6e0d476961734761746577617931002249444c3a4741562f4f626a6563745265666572656e6365466163746f72793a312e300003000000000000000800000001000000415f544901000000180000000100000001000100000000000401010001000000090101000600000006000000010000002f00"
        );
##  member_list -group <group>
$CL5 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "member_list -group group1");

##  group_list -location <location>
$CL6 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "group_list");

##  member_show -group <group> -member <member> -location <location>
$CL7 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "member_show -group group1 -member member1 -location dhcp34.ociweb.com");

$CL8 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "member_show -group group2 -member member2 -location nowhere junk_command");

##  member_remove -group <group> -member <member> -location <location>
$CL9 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "member_remove -group group1 -member member1 -location dhcp34.ociweb.com");

##  group_remove -group <group>
$CL10 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "group_remove -group group1");

##  group_list -location <location>
$CL11 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup",
        "$LOAD_ARG " .
        "group_list -location dhcp34.ociweb.com");

##  --help
$CL12 = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup","--help");

print STDERR "\n\n======== Running tao_nsgroup Test================\n";
print STDERR "\n";

print STDERR "This test will check the methods of the tao_nsgroup\n";
print STDERR "\n";

################################################################################
## Start LoadBalancer Service
################################################################################
$name_manager_status = $NM->Spawn ();

if ($name_manager_status != 0) {
    print STDERR "ERROR: server returned $name_manager_status\n";
    exit 1;
}
if ($name_manager->WaitForFileTimed ($name_mgr_iorbase,
        $name_manager->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$name_manager_iorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}

if ($name_manager->GetFile ($name_mgr_iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$name_manager_iorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($name_mgr_iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$naming_mgr_client_iorfile>\n";
    $NM->Kill (); $NM->TimedWait (1);
    exit 1;
}


################################################################################
## Run Commandline Interface Program
################################################################################
$client_status = $CL1->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL2->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL3->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL4->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL5->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL6->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL7->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL8->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL9->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL10->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL11->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$client_status = $CL12->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$name_manager_status = $NM->TerminateWaitKill ($name_manager->ProcessStopWaitInterval());

if ($name_manager_status != 0) {
    print STDERR "ERROR: server returned $name_manager_status\n";
    $status = 1;
}

$name_manager->DeleteFile($name_mgr_iorbase);
$name_manager->DeleteFile($name_iorbase);
$client->DeleteFile($name_mgr_iorbase);
$client->DeleteFile($name_iorbase);

exit $status;
