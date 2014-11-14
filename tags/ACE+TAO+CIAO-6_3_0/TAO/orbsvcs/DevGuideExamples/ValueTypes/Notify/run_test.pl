# $Id$

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

$conf = $PerlACE::svcconf_ext;
$hostname = $nfs->HostName();
$port = "8888";

my $eciorfile = "ec.ior";
my $nfsiorfile = "notify.ior";
my $nfsconf = "notify$conf";

my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $con_eciorfile = $con->LocalFile ($eciorfile);
my $sup_eciorfile = $sup->LocalFile ($eciorfile);
my $nfs_nfsconf = $nfs->LocalFile ($nfsconf);
$nfs->DeleteFile ($nfsiorfile);
$con->DeleteFile ($eciorfile);
$sup->DeleteFile ($eciorfile);

$NotifyService = "$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification";
$NFS = $nfs->CreateProcess ($NotifyService, " -boot ".
                                            "-orblistenendpoints iiop://:$port ".
                                            "-NoNameSvc ".
                                            "-IORoutput $nfs_nfsiorfile ".
                                            "-ORBSvcConf $nfs_nfsconf");
$SUP = $sup->CreateProcess ("supplier", "-k $sup_eciorfile -h $hostname -p $port");
$CON = $con->CreateProcess ("consumer", "-o $con_eciorfile -h $hostname -p $port");

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

print "****** Running consumer ******\n";

## The consumer takes one argument indicating
## how many events to receive before disconnecting.

$CON->Arguments("5");
$CON_status = $CON->Spawn ();

if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    exit 1;
}

if ($con->WaitForFileTimed ($eciorfile,$con->ProcessStartWaitInterval()+5) == -1) {
    print STDERR "ERROR: cannot find file <$con_eciorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

if ($con->GetFile ($eciorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$con_eciorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sup->PutFile ($eciorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sup_eciorfile>\n";
    $CON->Kill (); $CON->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

## The supplier takes two arguments.
## The first indicates how many events to push.
## The second indicates the payload size. (The payload is sequence<int>)

print "****** Running supplier ******\n";

$SUP->Arguments("10 5");
$SUP_status = $SUP->Spawn ();

if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$CON_status = $CON->WaitKill ($con->ProcessStopWaitInterval()+15);

if ($CON_status != 0) {
    print STDERR "ERROR: Consumer returned $CON_status\n";
    $status = 1;
}

$SUP_status = $SUP->WaitKill ($sup->ProcessStopWaitInterval()+15);

if ($SUP_status != 0) {
    print STDERR "ERROR: Supplier returned $SUP_status\n";
    $status = 1;
}

$NFS->Kill (); $NFS->TimedWait (1);

$nfs->DeleteFile ($nfsiorfile);
$con->DeleteFile ($eciorfile);
$sup->DeleteFile ($eciorfile);

exit $status;
