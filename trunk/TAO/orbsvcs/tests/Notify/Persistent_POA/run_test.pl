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
my $sup = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $con = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";

$sup->AddLibPath ('../lib');
$con->AddLibPath ('../lib');

$port = $ns->RandomPort ();
$host = $ns->HostName ();
$nfs_port = $nfs->RandomPort ();
$nfs_host = $nfs->HostName ();

$supiorfile = "supplier.ior";
$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";
$nfs2iorfile = "notify2.ior";
$nfsconffile = "../Blocking/notify$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $nfs_nfs2iorfile = $nfs->LocalFile ($nfs2iorfile);
my $sup_nfs2iorfile = $sup->LocalFile ($nfs2iorfile);
my $sup_supiorfile = $sup->LocalFile ($supiorfile);
my $con_supiorfile = $con->LocalFile ($supiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$nfs->DeleteFile ($nfs2iorfile);
$sup->DeleteFile ($nfs2iorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

$NS = $ns->CreateProcess ("../../../Naming_Service/Naming_Service",
                            "-ORBEndpoint iiop://$host:$port ".
                            "-o $ns_nsiorfile");
$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                            "-ORBEndpoint iiop://$nfs_host:$nfs_port " .
                            "-ORBInitRef NameService=iioploc://" .
                            "$host:$port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");
$SUP = $sup->CreateProcess ("Structured_Supplier",
                            "-ORBDebugLevel $debug_level ".
                             "-ORBInitRef NameService=iioploc://" .
                             "$host:$port/NameService ".
                             "-o $sup_supiorfile ".
                             "-f $sup_nfs2iorfile");
$CON = $con->CreateProcess ("../Blocking/Structured_Consumer",
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-k file://$con_supiorfile");

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "*********** Starting the first Notify Service  ***********\n";
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($nfs->GetFile ($nfs2iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$nfs_nfs2iorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($nfs2iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sup_nfs2iorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "*********** Starting the notification supplier ***********\n";
$SUP_status = $SUP->Spawn ();
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "*********** Allowing time to register the ec   ***********\n";
sleep(3);

print "*********** Killing the first Notify Service   ***********\n";
$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
sleep(1);

print "*********** Starting the second Notify Service ***********\n";
my $args = $NFS->Arguments();
my $l = index($args, $nfs_nfsiorfile);
substr($args, $l, length($nfs_nfsiorfile)) = $nfs_nfs2iorfile;
$NFS->Arguments($args);
$NFS_status = $NFS->Spawn ();
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($nfs->WaitForFileTimed ($nfs2iorfile,$nfs->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfs2iorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

if ($sup->WaitForFileTimed ($supiorfile,$sup->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->GetFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sup_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($supiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_supiorfile>\n";
    $SUP->Kill (); $SUP->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

print "*********** Starting the notification consumer ***********\n";
$CON_status = $CON->SpawnWaitKill ($con->ProcessStartWaitInterval()+5);
if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $status = 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval());
if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$nfs->DeleteFile ($nfs2iorfile);
$sup->DeleteFile ($supiorfile);
$con->DeleteFile ($supiorfile);

exit $status;
