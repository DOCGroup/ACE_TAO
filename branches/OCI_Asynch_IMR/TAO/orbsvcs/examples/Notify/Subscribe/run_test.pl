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
my $sub = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

PerlACE::check_privilege_group();

$experiment_timeout = 60;
$startup_timeout = 60;

$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $sub_nsiorfile = $sub->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$sub->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/tao_cosnaming",
                          " -o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/tao_cosnotification");
$Notify_Args = "-ORBInitRef NameService=file://$nfs_nsiorfile -IORoutput $nfs_nfsiorfile ";

$SUB = $sub->CreateProcess ("Subscribe");
$Subscribe_Args = "-ORBInitRef NameService=file://$sub_nsiorfile -ORBDebugLevel $debug_level";

$NS_status = $NS->Spawn ();
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+$startup_timeout) == -1) {
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
if ($sub->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sub_nsiorfile>\n";
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$NFS->Arguments ($Notify_Args);
$args = $NFS->Arguments ();
print STDERR "Running Notification with arguments: $args\n";
$NFS_status = $NFS->Spawn ();

if ($NFS_status != 0) {
    print STDERR "ERROR: Notification Service returned $NFS_status\n";
    exit 1;
}

if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+$startup_timeout) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$SUB->Arguments ($Subscribe_Args);
$args = $SUB->Arguments ();
print STDERR "Running Subscribe with arguments: $args\n";
$SUB_status = $SUB->SpawnWaitKill ($sub->ProcessStartWaitInterval()+$experiment_timeout);
if ($SUB_status != 0) {
    print STDERR "ERROR: Subscriber returned $SUB_status\n";
    $status = 1;
}

$NFS_status = $NFS->TerminateWaitKill ($nfs->ProcessStopWaitInterval());

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
$sub->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);

exit $status;
