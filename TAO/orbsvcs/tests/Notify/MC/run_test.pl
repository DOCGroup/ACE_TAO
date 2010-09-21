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

my $ns = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $nfs = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $mon = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $sup = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $con = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

$mon->AddLibPath ('../lib');
$sup->AddLibPath ('../lib');
$con->AddLibPath ('../lib');

PerlACE::check_privilege_group();

# define the following as necessary to customize the test
my $static_build = 0;
my $port = $ns->RandomPort ();
my $host = $ns->HostName ();
my $nscorbaloc = "-ORBInitRef NameService=corbaloc:iiop:$host:$port/NameService";
$static_build = new PerlACE::ConfigList->check_config('STATIC');

my $testmonior = "test_monitor.ior";
my $monitorior = "monitor.ior";
my $nsiorfile = "naming.ior";
my $nfsiorfile = "notify.ior";
my $readyfile = "ready.txt";
my $notify_conf = "notify$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $nfs_notify_conf = $nfs->LocalFile ($notify_conf);
my $nfs_monitorior = $nfs->LocalFile ($monitorior);
my $mon_monitorior = $mon->LocalFile ($monitorior);
my $mon_testmonior = $mon->LocalFile ($testmonior);
my $sup_testmonior = $sup->LocalFile ($testmonior);
my $con_testmonior = $con->LocalFile ($testmonior);
my $con_readyfile = $con->LocalFile ($readyfile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$nfs->DeleteFile ($notify_conf);
$nfs->DeleteFile ($monitorior);
$mon->DeleteFile ($monitorior);
$mon->DeleteFile ($testmonior);
$sup->DeleteFile ($testmonior);
$con->DeleteFile ($testmonior);
$con->DeleteFile ($readyfile);

die "oops" if not open(FH, ">$notify_conf");
if($static_build){
    print FH "static TAO_MonitorAndControl \"-o $nfs_monitorior " .
             "-ORBArg -ORBInitRef " .
             "-ORBArg NameService=corbaloc:iiop:$host:$port/NameService\"\n";
    print FH "static TAO_MC_Notify_Service \"-DispatchingThreads 1 \"\n";
} else {
    print FH "dynamic TAO_MC_Notify_Service Service_Object * " .
              "TAO_CosNotification_MC_Ext:_make_TAO_MC_Notify_Service () \"\"\n";
    print FH "static Notify_Default_Event_Manager_Objects_Factory \"-DispatchingThreads 1\"\n";
    print FH "dynamic TAO_MonitorAndControl Service_Object * ".
              "TAO_CosNotification_MC:_make_TAO_MonitorAndControl () \"" .
              "-o $nfs_monitorior " .
              "-ORBArg -ORBInitRef ".
              "-ORBArg NameService=corbaloc:iiop:$host:$port/NameService" .
              "\"\n";
}
close(FH);

if ($nfs->PutFile ($notify_conf) == -1) {
    print STDERR "ERROR: cannot set file <$nfs_notify_conf>\n";
    exit 1;
}

my $NS = $ns->CreateProcess("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://$host:$port -o $nsiorfile");
my $NFS = $nfs->CreateProcess("../../../Notify_Service/tao_cosnotification",
                              "-ORBDebugLevel $debug_level ".
                              "$nscorbaloc " .
                              "-IORoutput $nfs_nfsiorfile " .
                              "-ORBSvcConf $nfs_notify_conf ");
my $MON = $mon->CreateProcess("test_monitor",
                              "-k file://$mon_monitorior -o $mon_testmonior");
my $SUP = $sup->CreateProcess("Structured_Supplier",
                              "$nscorbaloc ".
                              "-k file://$sup_testmonior");
my $CON = $con->CreateProcess("Structured_Consumer",
                              "$nscorbaloc ".
                              "-k file://$con_testmonior -o $con_readyfile");

print $NS->CommandLine()."\n";
$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print $NFS->CommandLine()."\n";
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()*2) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($nfs->WaitForFileTimed ($monitorior,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_monitorior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($nfs->GetFile ($monitorior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nfs_monitorior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($mon->PutFile ($monitorior) == -1) {
    print STDERR "ERROR: cannot set file <$mon_monitorior>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}


print $MON->CommandLine()."\n";
$MON_status = $MON->Spawn ();
if ($MON_status != 0) {
    print STDERR "ERROR: Monitor returned $MON_status\n";
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($mon->WaitForFileTimed ($testmonior,$mon->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$mon_testmonior>\n";
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($mon->GetFile ($testmonior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$mon_testmonior>\n";
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($testmonior) == -1) {
    print STDERR "ERROR: cannot set file <$sup_testmonior>\n";
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($testmonior) == -1) {
    print STDERR "ERROR: cannot set file <$con_testmonior>\n";
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print $CON->CommandLine()."\n";
$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

## Wait for the consumer to create the event channel in
## the Notify_Service and register it with the Name Service
if ($con->WaitForFileTimed ($readyfile,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_readyfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $MON->Kill (); $MON->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

print $SUP->CommandLine()."\n";
$SUP_status = $SUP->SpawnWaitKill ($sup->ProcessStartWaitInterval()+15);
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStartWaitInterval()+45);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $status = 1;
}

$MON_status = $MON->TerminateWaitKill ($mon->ProcessStopWaitInterval());
if ($MON_status != 0) {
    print STDERR "ERROR: Monitor returned $MON_status\n";
    $status = 1;
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notification Service returned $NFS_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$nfs->DeleteFile ($notify_conf);
$nfs->DeleteFile ($monitorior);
$mon->DeleteFile ($monitorior);
$mon->DeleteFile ($testmonior);
$sup->DeleteFile ($testmonior);
$con->DeleteFile ($testmonior);
$con->DeleteFile ($readyfile);

exit $status;
