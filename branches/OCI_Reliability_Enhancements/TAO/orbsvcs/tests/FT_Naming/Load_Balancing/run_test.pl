eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

## The NamingManager needs to register signals with the ORB's reactor (on
## Windows only) and thus can not use the TP Reactor since it doesn't
## support that kind of thing.  So, we swith to the Select MT Reactor.
$lm_conf = "windows" . $PerlACE::svcconf_ext;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $naming_manager = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2 = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $nm_iorfile = "lm.ior";
my $ns_iorfile = "ns.ior";
my $server_obj_group_iorfile = "obj.ior";

#Files used by the FT Naming Manager
my $naming_mgr_nm_iorfile = $naming_manager->LocalFile ($nm_iorfile);
my $naming_mgr_ns_iorfile = $naming_manager->LocalFile ($ns_iorfile);
$naming_manager->DeleteFile($nm_iorfile);
$naming_manager->DeleteFile($ns_iorfile);

#Files used by the server2
my $server2_nm_iorfile = $server2->LocalFile ($nm_iorfile);
$server2->DeleteFile($server2_nm_iorfile);
my $server2_ns_iorfile = $server2->LocalFile ($ns_iorfile);
$server2->DeleteFile($server2_ns_iorfile);
my $server2_obj_group_iorfile = $server2->LocalFile ($server_obj_group_iorfile);
$server2->DeleteFile($server_obj_group_iorfile);

#Files which used by client
my $client_ns_iorfile = $client->LocalFile ($ns_iorfile);
$client->DeleteFile($client_ns_iorfile);


$SV1 = $naming_manager->CreateProcess ("../../../../orbsvcs/Naming_Service/tao_ft_naming",
                              "-ORBdebuglevel $debug_level " .
                              "-f FT_Naming.dat " .
                              "-o $ns_iorfile " .
                              "-g $nm_iorfile " .
                              ($^O eq 'MSWin32' ?
                                           " -ORBSvcConf $lm_conf" : ''));

$SV2 = $server2->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-ORBInitRef NameService=file://$server2_ns_iorfile " .
                              "-ORBInitRef NamingManager=file://$server2_nm_iorfile " .
                              "-o $server2_obj_group_iorfile");

$CL = $client->CreateProcess ("client",
                              "-ORBInitRef NameService=file://$client_ns_iorfile");

print STDERR "\n\n======== Running Application Controlled Membership Test================\n";
print STDERR "\n";

print STDERR "This test uses the RoundRobin Load Balancing strategy \n";

print STDERR "6 servers are created and added into a Object Group\n";

print STDERR "When the client accesses an object group through the naming service, the";
print STDERR "FT_Naming_Service will apply the RoundRobin load balancing strategy\n";
print STDERR "selects one of the servers and then the client makes an invocations on\n";
print STDERR "the server. This is done 15 times to ensure we support the wraparround\n";
print STDERR "condition.  When done, the application has to delete the object from the\n";
print STDERR "object group. This is called the application controlled membership of\n";
print STDERR "object group.\n";
print STDERR "\n";

$server_status = $SV1->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($naming_manager->WaitForFileTimed ($nm_iorfile,
                               $naming_manager->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$naming_mgr_nm_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($naming_manager->GetFile ($nm_iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$naming_mgr_nm_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

if ($server2->PutFile ($server_obj_group_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$server2_obj_group_iorfile>\n";
    $SV1->Kill (); $SV1->TimedWait (1);
    exit 1;
}

$server_status = $SV2->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sub KillServers{
    $SV1->Kill (); $SV1->TimedWait (1);
    $SV2->Kill (); $SV2->TimedWait (1);
}

if ($server2->WaitForFileTimed ($server_obj_group_iorfile,
                               $server2->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server2_obj_group_iorfile>\n";
    KillServers ();
    exit 1;
}

if ($server2->GetFile ($server_obj_group_iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server2_obj_group_iorfile>\n";
    KillServers ();
    exit 1;
}

if ($client->PutFile ($server_obj_group_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_ns_iorfile>\n";
    KillServers ();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 85);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server_status = $SV1->TerminateWaitKill ($naming_manager->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$naming_manager->DeleteFile($nm_iorfile);
$server2->DeleteFile($nm_iorfile);
$server2->DeleteFile($server_obj_group_iorfile);
$client->DeleteFile($server_obj_group_iorfile);

exit $status;
