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

my $nfs = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $sup = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $con = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $flt = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

my $persistent_test = 0;
my $port = $nfs->RandomPort ();
my $host = $nfs->HostName ();
my $nts_ref = "NotifyService=iioploc://$host:$port/NotifyService";
my $consumer_runtime = 5;
my $svcconf = "";

my $nfsiorfile = "ecf.ior";
#my $nfs2iorfile = "ecf2.ior";
my $nfsconffile = "ecf$PerlACE::svcconf_ext";
#my $persistfile = "persistency.notif.xml";
#my $persistfilebackup = "persistency.notif.000";

my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$nfs->DeleteFile ($nfsiorfile);

if ($#ARGV >= 0 && $ARGV[0] eq '-p') {
    $persistent_test = 1;
    $svcconf = " -ORBSvcConf $nfs_nfsconffile";
    $consumer_runtime = 20;
}

my $NFS = $nfs->CreateProcess ("../../../Notify_Service/Notify_Service",
                               "-ORBDebugLevel $debug_level ".
                               "-NoNameSvc -IORoutput $nfs_nfsiorfile $svcconf " .
                               "-ORBEndpoint iiop://$host:$port");
my $SUP = $sup->CreateProcess ("supplier", "$nts_ref");
my $CON = $con->CreateProcess ("consumer", "$nts_ref -t $consumer_runtime");
my $FLT = $flt->CreateProcess ("filter", "$nts_ref");


print "\n*********** Starting the Notify_Service  ***********\n\n";
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

if ($persistent_test == 0) {
    print "\n*********** Starting the filter test ***********\n\n";
    $FLT_status = $FLT->SpawnWaitKill ($flt->ProcessStartWaitInterval());
    if ($FLT_status != 0) {
        print STDERR "ERROR: Filter returned $FLT_status\n";
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }
    print "\n*********** Filter test passed ***********\n\n";
}

print "\n*********** Starting the notification Consumer ***********\n\n";
print STDERR $CON->CommandLine (). "\n";

$CON_status = $CON->Spawn ();
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

sleep(5);

if ($persistent_test == 1) {
    print "*********** Killing the first Notify_Service   ***********\n";
    $NFS_status = $NFS->TerminateWaitKill ($nfs->ProcessStopWaitInterval());
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notification Service returned $NFS_status\n";
        $CON->Kill (); $CON->TimedWait (1);
        exit 1;
    }
    sleep(1);

    $nfs->DeleteFile ($nfsiorfile);

    print "*********** Starting the second Notify_Service ***********\n";
    print STDERR $NFS->CommandLine (). "\n";
    $NFS_status = $NFS->Spawn ();
    if ($NFS_status != 0) {
        print STDERR "ERROR: Notify Service returned $NFS_status\n";
        $CON->Kill (); $CON->TimedWait (1);
        exit 1;
    }
    if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
        $CON->Kill (); $CON->TimedWait (1);
        $NFS->Kill (); $NFS->TimedWait (1);
        exit 1;
    }

    sleep(5);
}

print "\n*********** Starting the notification Supplier ***********\n\n";
print STDERR $SUP->CommandLine (). "\n";

$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status =1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+$consumer_runtime);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $status =1;
}

$NFS_status = $NFS->TerminateWaitKill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notification Service returned $NFS_status\n";
    $status = 1;
}

$nfs->DeleteFile ($nfsiorfile);

exit $status;
