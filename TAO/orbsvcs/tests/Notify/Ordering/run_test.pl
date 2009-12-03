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
my $sts = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $stc = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $ses = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";
my $sec = PerlACE::TestTarget::create_target (6) || die "Create target 6 failed\n";

PerlACE::add_lib_path ('../lib');

PerlACE::check_privilege_group();

$deadline = 0;
$port = $ns->RandomPort ();
$host = $ns->HostName ();

$supiorfile = "supplier.ior";
$nfsiorfile = "notify.ior";
$nsiorfile = "naming.ior";
$nfsconffile = "notify$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $sts_stsiorfile = $sts->LocalFile ($supiorfile);
my $stc_stsiorfile = $stc->LocalFile ($supiorfile);
my $sec_stsiorfile = $sec->LocalFile ($supiorfile);
my $ses_sesiorfile = $ses->LocalFile ($supiorfile);
my $sec_sesiorfile = $sec->LocalFile ($supiorfile);
my $stc_sesiorfile = $stc->LocalFile ($supiorfile);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sts->DeleteFile ($stsiorfile);
$stc->DeleteFile ($stsiorfile);
$sec->DeleteFile ($stsiorfile);
$ses->DeleteFile ($sesiorfile);
$sec->DeleteFile ($sesiorfile);
$stc->DeleteFile ($sesiorfile);

foreach my $arg (@ARGV) {
    if ($arg eq "-d") {
        $deadline = 1;
    }
    else {
        print "Usage: $0 [-d]\n" .
              "       -d specifies that deadline discarding be tested.\n";
        exit(0);
    }
}

$NS = $ns->CreateProcess ("../../../Naming_Service/Naming_Service", 
                            "-ORBEndpoint iiop://$host:$port ".
                            "-o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("../../../Notify_Service/Notify_Service",
                            "-ORBInitRef NameService=iioploc://" .
                            "$host:$port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");

$STS = $sts->CreateProcess ("Structured_Supplier",
                            "-ORBDebugLevel $debug_level ".  
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-o $sts_stsiorfile");

$STC = $stc->CreateProcess ("Structured_Consumer");

$SES = $ses->CreateProcess ("Sequence_Supplier",
                            "-ORBDebugLevel $debug_level ".  
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-o $ses_sesiorfile");

$SEC = $sec->CreateProcess ("Sequence_Consumer");

$CLI_args = "-ORBInitRef NameService=iioploc://".
            "$host:$port/NameService ";

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

if ($deadline) {
    ## @@todo : Add combinations of deadline ordering.
}

# Although the TAO notify service supports OrderPolicy on the supplier side
# QoS, this setting typically serves no practical purpose, and is not testable.
# This is because we have no way to force the supplier-side queue to back up, and
# the OrderPolicy will have no affect.
# Therefore we don't test setting this policy on the supplier side.

print "**** Structured Supplier (fifo) -> Structured Consumer (none) *****\n";
$STS->Arguments($STS->Arguments() . " -d fifo");
$STS_status = $STS->Spawn ();
if ($STS_status != 0) {
    print STDERR "ERROR: Structured Supplier returned $STS_status\n";
    exit 1;
}
if ($sts->WaitForFileTimed ($stsiorfile,$sts->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sts_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sts->GetFile ($stsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sts_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($stc->PutFile ($stsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$stc_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
sleep 2;

$STC->Arguments($CLI_args . " -k file://$stc_stsiorfile -d fifo");
$STC_status = $STC->SpawnWaitKill ($stc->ProcessStartWaitInterval()+5);
if ($STC_status != 0) {
    print STDERR "ERROR: Structured Consumer returned $STC_status\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$STS_status = $STS->WaitKill ($sts->ProcessStopWaitInterval());
if ($STS_status != 0) {
    print STDERR "ERROR: Structured Supplier returned $STS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$sts->DeleteFile ($stsiorfile);
$stc->DeleteFile ($stsiorfile);



print "**** Structured Supplier (fifo) -> Structured Consumer (priority) *****\n";
$STS->Arguments($STS->Arguments() . " -d fifo");
$STS_status = $STS->Spawn ();
if ($STS_status != 0) {
    print STDERR "ERROR: Structured Supplier returned $STS_status\n";
    exit 1;
}
if ($sts->WaitForFileTimed ($stsiorfile,$sts->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sts_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sts->GetFile ($stsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sts_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($stc->PutFile ($stsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$stc_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
sleep 2; 

$STC->Arguments($CLI_args . " -k file://$stc_stsiorfile -d priority -o");
$STC_status = $STC->SpawnWaitKill ($stc->ProcessStartWaitInterval()+5);
if ($STC_status != 0) {
    print STDERR "ERROR: Structured Consumer returned $STC_status\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$STS_status = $STS->WaitKill ($sts->ProcessStopWaitInterval());
if ($STS_status != 0) {
    print STDERR "ERROR: Structured Supplier returned $STS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$sts->DeleteFile ($stsiorfile);
$stc->DeleteFile ($stsiorfile);



print "**** Structured Supplier (fifo) -> Sequence Consumer (priority) *****\n";
$STS->Arguments($STS->Arguments() . " -d fifo");
$STS_status = $STS->Spawn ();
if ($STS_status != 0) {
    print STDERR "ERROR: Structured Supplier returned $STS_status\n";
    exit 1;
}
if ($sts->WaitForFileTimed ($stsiorfile,$sts->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$sts_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sts->GetFile ($stsiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$sts_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sec->PutFile ($stsiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sec_stsiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
sleep 2; 

$SEC->Arguments($CLI_args . " -k file://$sec_stsiorfile -d priority -o");
$SEC_status = $SEC->SpawnWaitKill ($sec->ProcessStartWaitInterval()+5);
if ($SEC_status != 0) {
    print STDERR "ERROR: Sequence Consumer returned $SEC_status\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$STS_status = $STS->WaitKill ($sts->ProcessStopWaitInterval());
if ($STS_status != 0) {
    print STDERR "ERROR: Structured Supplier returned $STS_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$sts->DeleteFile ($stsiorfile);
$sec->DeleteFile ($stsiorfile);



print "**** Sequence Supplier (fifo) -> Structured Consumer (priority) *****\n";
$SES->Arguments($SES->Arguments() . " -d fifo");
$SES_status = $SES->Spawn ();
if ($SES_status != 0) {
    print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
    exit 1;
}
if ($ses->WaitForFileTimed ($sesiorfile,$ses->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ses_sesiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($ses->GetFile ($sesiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ses_sesiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($stc->PutFile ($sesiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$stc_sesiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
sleep 2; 

$STC->Arguments($CLI_args . " -k file://$stc_sesiorfile -d priority -o");
$STC_status = $STC->SpawnWaitKill ($stc->ProcessStartWaitInterval()+5);
if ($STC_status != 0) {
    print STDERR "ERROR: Structured Consumer returned $STC_status\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$SES_status = $SES->WaitKill ($ses->ProcessStopWaitInterval());
if ($SES_status != 0) {
    print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}

$ses->DeleteFile ($sesiorfile);
$stc->DeleteFile ($sesiorfile);


print "**** Sequence Supplier (fifo) -> Sequence Consumer (priority) *****\n";
$SES->Arguments($SES->Arguments() . " -d fifo");
$SES_status = $SES->Spawn ();
if ($SES_status != 0) {
    print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
    exit 1;
}
if ($ses->WaitForFileTimed ($sesiorfile,$ses->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ses_sesiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($ses->GetFile ($sesiorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$ses_sesiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
if ($sec->PutFile ($sesiorfile) == -1) {
    print STDERR "ERROR: cannot set file <$sec_sesiorfile>\n";
    $STS->Kill (); $STS->TimedWait (1);
    $NS->Kill (); $NS->TimedWait (1);
    $NFS->Kill (); $NFS->TimedWait (1);
    exit 1;
}
sleep 2; 

$SEC->Arguments($CLI_args . " -k file://$sec_sesiorfile -d priority -o");
$SEC_status = $SEC->SpawnWaitKill ($sec->ProcessStartWaitInterval()+5);
if ($SEC_status != 0) {
    print STDERR "ERROR: Sequence Consumer returned $SEC_status\n";
    $status = 1;
}

$SES_status = $SES->WaitKill ($ses->ProcessStopWaitInterval());
if ($SES_status != 0) {
    print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
    $status = 1;
}

$NFS_status = $NFS->Kill ($nfs->ProcessStopWaitInterval());
if ($NFS_status != 0) {
    print STDERR "ERROR: Notify Service returned $NFS_status\n";
    $status = 1;
}

$NS_status = $NS->TerminateWaitKill ($ns->ProcessStopWaitInterval());
if ($NS_status != 0) {
    print STDERR "ERROR: Name Service returned $NS_status\n";
    $status = 1;
}

$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sts->DeleteFile ($stsiorfile);
$stc->DeleteFile ($stsiorfile);
$sec->DeleteFile ($stsiorfile);
$ses->DeleteFile ($sesiorfile);
$sec->DeleteFile ($sesiorfile);
$stc->DeleteFile ($sesiorfile);

exit $status;
