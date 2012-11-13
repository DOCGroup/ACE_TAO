eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = 0;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

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
        "-ORBDebugLevel $debug_level " .
        "-ORBDottedDecimalAddresses 1" .
        ($^O eq 'MSWin32' ? " -ORBSvcConf $NM_conf" : ''));


#$NS = $name_server->CreateProcess ("../../orbsvcs/Naming_Service/tao_cosnaming",
#        "-o $name_server_iorfile " .
#        ($^O eq 'MSWin32' ? " -ORBSvcConf $NS_conf" : ''));

$NM_REF       = "-ORBInitRef NameService=file://$name_client_iorfile";
$RM_REF       = "-ORBInitRef NamingManager=file://$naming_mgr_client_iorfile";
$DEBUG_LEVEL  = "-ORBDebugLevel $debug_level";
$LOAD_ARG     = "$NM_REF $RM_REF $DEBUG_LEVEL";

$CL   = $client->CreateProcess ("$ENV{ACE_ROOT}/bin/tao_nsgroup");

sub run_client ($)
{
    my $args = shift;

    my $arglist = "$LOAD_ARG $args";

    print STDERR "\n\n======== Running Test================\n";
    print STDERR "$args\n";

    $CL->Arguments ($arglist);

    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

    if ($client_status != 0) {
        my $time = localtime;
        print STDERR "ERROR: client returned $client_status at $time\n";
        $status = 1;
    }
}

sub run_clients ()
{
    #create -type rr GiasGateway
    #bind -og_name GiasGateway GiasGateway
    #add_member -og_name GiasGateway -member_name mcknerney2-lnx5-dev/GiasGateway1 IOR:010000002300000049444c3a4741562f4f626a6563745265666572656e6365466163746f72793a312e3000000100000000000000b2000000010102000e00000031302e33352e3232352e313530005046530000003a3e0232311c756e636c6173735f757374696c2e6363692d612e6c6f636174696f6e0d476961734761746577617931002249444c3a4741562f4f626a6563745265666572656e6365466163746f72793a312e300003000000000000000800000001000000415f544901000000180000000100000001000100000000000401010001000000090101000600000006000000010000002f00
    #add_member -og_name GiasGateway -member_name mcknerney2-lnx5-dev/GiasGateway2 IOR:010000002300000049444c3a4741562f4f626a6563745265666572656e6365466163746f72793a312e3000000100000000000000b2000000010102000e00000031302e33352e3232352e313530005046530000003a3e0232311c756e636c6173735f757374696c2e6363692d612e6c6f636174696f6e0d476961734761746577617932002249444c3a4741562f4f626a6563745265666572656e6365466163746f72793a312e300003000000000000000800000001000000415f544901000000180000000100000001000100000000000401010001000000090101000600000006000000010000002f00
    my $IOR = "IOR:010000002100000049444c3a6f6d672e6f72672f46542f4e616d696e674d616e616765723a312e300000000001000000000000006c000000010102000e00000031302e3230312e3230302e36340005e71b00000014010f00525354caaaa250c6ba0e000000000001000000010000004e02000000000000000800000001000000004f41540100000018000000010000000100010001000000010001050901010000000000";

    run_client ("group_create -group GiasGateway -policy round -type_id IDL:omg.org/FT/NamingManager:1.0");
    run_client ("group_create -group group2 -policy round -type_id IDL:omg.org/FT/NamingManager:1.0");
    run_client ("group_list");
    run_client ("group_bind -group GiasGateway -n GiasGateway");
    run_client ("group_modify -group group1 -policy rand");
    run_client ("member_add -group GiasGateway -location 10.201.200.64 -ior $IOR");
    run_client ("member_add -group GiasGateway -location dhcp34.ociweb.com -ior $IOR");
    run_client ("member_list -group group1");
    run_client ("member_show -group group1 -location dhcp34.ociweb.com");
    run_client ("member_show -group group2 -location nowhere junk_command");
    run_client ("member_remove -group group1 -location dhcp34.ociweb.com");
    run_client ("group_remove -group GiasGateway");
    run_client ("group_list");
    run_client ("group_remove -group group2");
    run_client ("group_list");
    run_client ("group_remove -group groupN");
    run_client ("group_list");
    run_client ("--help");
}

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

run_clients();


print STDERR "\n\n====================================================\n";
print STDERR "\n";
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
