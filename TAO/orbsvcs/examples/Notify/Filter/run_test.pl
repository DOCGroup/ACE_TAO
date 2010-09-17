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
my $flt = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

PerlACE::check_privilege_group();

$nsiorfile = "naming.ior";
$nfsiorfile = "notify.ior";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nsiorfile = $nfs->LocalFile ($nsiorfile);
my $flt_nsiorfile = $flt->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nsiorfile);
$flt->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);

$NS = $ns->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Naming_Service/Naming_Service",
                          " -o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("$ENV{TAO_ROOT}/orbsvcs/Notify_Service/Notify_Service",
                          " -o $ns_nsiorfile");
$Notify_Args = "-ORBInitRef NameService=file://$nfs_nsiorfile -IORoutput $nfs_nfsiorfile ";

$FLT = $flt->CreateProcess ("Filter",
                          " -o $ns_nsiorfile");
$Filter_Args = "-ORBInitRef NameService=file://$flt_nsiorfile -ORBDebugLevel $debug_level";

$NS_status = $NS->Spawn ();

if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    exit 1;
}

if ($ns->WaitForFileTimed ($nsiorfile,$ns->ProcessStartWaitInterval()+45) == -1) {
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
if ($flt->PutFile ($nsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$flt_nsiorfile>\n";
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

if ($nfs->WaitForFileTimed ($nfsiorfile,$nfs->ProcessStartWaitInterval()+45) == -1) {
    print STDERR "ERROR: cannot find file <$nfs_nfsiorfile>\n";
    $NFS->Kill (); $NFS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    exit 1;
}

$FLT->Arguments ($Filter_Args);
$args = $FLT->Arguments ();
print STDERR "Running Filter with arguments: $args\n";
$FLT_status = $FLT->SpawnWaitKill ($flt->ProcessStartWaitInterval()+45);
if ($FLT_status != 0) {
    print STDERR "ERROR: Filter returned $FLT_status\n";
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
$flt->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);

exit $status;
