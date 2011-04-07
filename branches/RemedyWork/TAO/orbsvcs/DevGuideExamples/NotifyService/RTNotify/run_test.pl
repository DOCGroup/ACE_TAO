# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

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
my $srv = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $con = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $cli = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

$nsiorfile   = "ns.ior";
$srviorfile = "Messenger.ior";
$nfsiorfile  = "notify.ior";
$confile = "MessengerConsumer.ready";
$conconf = "notify.conf";
$nscliconf = "nsclient.conf";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $srv_nsiorfile = $srv->LocalFile ($nsiorfile);
my $con_nsiorfile = $con->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $srv_srviorfile = $srv->LocalFile ($srviorfile);
my $cli_srviorfile = $cli->LocalFile ($srviorfile);
my $con_confile = $con->LocalFile ($confile);
my $con_conconf = $con->LocalFile ($conconf);
my $srv_nscliconf = $srv->LocalFile ($nscliconf);
my $con_nscliconf = $con->LocalFile ($nscliconf);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$srv->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);
$con->DeleteFile ($confile);

$NameService = "$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming";
$NS = $ns->CreateProcess ($NameService, " -o $ns_nsiorfile");


$NotifyService = "$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification";
$NFS = $nfs->CreateProcess ($NotifyService, "-ORBInitRef NameService=file://$nfs_nsiorfile ".
                                            "-IORoutput $nfs_nfsiorfile ".
                                            "-ORBSvcConf $con_conconf");
$SRV = $srv->CreateProcess ("MessengerServer", "-ORBdebuglevel $debug_level ".
                                               "-o $srv_srviorfile ".
                                               "-ORBInitRef NameService=file://$srv_nsiorfile ".
                                               "-ORBSvcConf $srv_nscliconf");
$CON = $con->CreateProcess ("MessengerConsumer", "-ORBInitRef NameService=file://$con_nsiorfile ".
                                                 "-ORBSvcConf $con_nscliconf ".
                                                 "-o $con_confile");
$CLI = $cli->CreateProcess ("MessengerClient", "-k file://$cli_srviorfile");

# start Naming Service
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

if ($ns->GetFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ns_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($nfs->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$nfs_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($srv->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$srv_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}
if ($con->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$con_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

# start Notification Service
$NFS_status = $NFS->Spawn ();

if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    exit 1;
}

if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# start MessengerServer
$SRV_status = $SRV->Spawn ();

if ($SRV_status != 0) {
    print STDERR "ERROR: Server returned $SRV_status\n";
    exit 1;
}

if ($srv->WaitForFileTimed ($srviorfile,$srv->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$srv_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

if ($srv->GetFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$srv_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($cli->PutFile ($srviorfile) == -1) {
    print STDERR "ERROR: cannot set file <$cli_srviorfile>\n";
    $SRV->Kill (); $SRV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# start MessengerConsumer
$CON_status = $CON->Spawn ();

if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    exit 1;
}

if ($con->WaitForFileTimed ($confile,$con->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$con_confile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

# start MessengerClient
$CLI_status = $CLI->SpawnWaitKill ($cli->ProcessStartWaitInterval());

if ($CLI_status != 0) {
    print STDERR "ERROR: Messenger Client returned $CLI_status\n";
    $CON->Kill (); $CON->TimedWait (1);
    $SRV->Kill (); $SRV->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $status = 1;
}

$CON_status = $CON->TerminateWaitKill ($con->ProcessStopWaitInterval());

if ($CON_status != 0) {
    print STDERR "ERROR: Messenger Consumer returned $CON_status\n";
    $status = 1;
}

$SRV_status = $SRV->TerminateWaitKill ($srv->ProcessStopWaitInterval());

if ($SRV_status != 0) {
    print STDERR "ERROR: Messenger Server returned $SRV_status\n";
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
$nfs->DeleteFile ($nsiorfile);
$srv->DeleteFile ($nsiorfile);
$con->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$srv->DeleteFile ($srviorfile);
$cli->DeleteFile ($srviorfile);
$con->DeleteFile ($confile);

exit $status;
