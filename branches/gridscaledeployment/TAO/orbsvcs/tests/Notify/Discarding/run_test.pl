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

$sts->AddLibPath ('../lib');
$stc->AddLibPath ('../lib');
$ses->AddLibPath ('../lib');
$sec->AddLibPath ('../lib');

PerlACE::check_privilege_group();

$port = $ns->RandomPort ();
$host = $ns->HostName ();
$deadline = 0;

$iorbase = "supplier.ior";
$nfsiorfile = "notify.ior";
$nsiorfile = "naming.ior";
$nfsconffile = "notify$PerlACE::svcconf_ext";

my $ns_nsiorfile = $ns->LocalFile ($nsiorfile);
my $nfs_nfsiorfile = $nfs->LocalFile ($nfsiorfile);
my $sts_iorfile = $sts->LocalFile ($iorbase);
my $stc_iorfile = $stc->LocalFile ($iorbase);
my $sec_iorfile = $sec->LocalFile ($iorbase);
my $ses_iorfile = $ses->LocalFile ($iorbase);
my $nfs_nfsconffile = $nfs->LocalFile ($nfsconffile);
$ns->DeleteFile ($nsiorfile);
$nfs->DeleteFile ($nfsiorfile);
$sts->DeleteFile ($iorbase);
$stc->DeleteFile ($iorbase);
$sec->DeleteFile ($iorbase);
$ses->DeleteFile ($iorbase);

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

$NS = $ns->CreateProcess ("../../../Naming_Service/tao_cosnaming",
                          "-ORBEndpoint iiop://$host:$port -o $ns_nsiorfile");

$NFS = $nfs->CreateProcess ("../../../Notify_Service/tao_cosnotification",
                            "-ORBInitRef NameService=iioploc://" .
                            "$host:$port/NameService " .
                            "-IORoutput $nfs_nfsiorfile -ORBSvcConf " .
                            "$nfs_nfsconffile");

$STS = $sts->CreateProcess ("Structured_Supplier",
                            "-ORBDebugLevel $debug_level ".
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-o $sts_iorfile");

$STC = $stc->CreateProcess ("Structured_Consumer");

$SES = $ses->CreateProcess ("Sequence_Supplier",
                            "-ORBDebugLevel $debug_level ".
                            "-ORBInitRef NameService=iioploc://".
                            "$host:$port/NameService ".
                            "-o $ses_iorfile");

$SEC = $sec->CreateProcess ("Sequence_Consumer");

$CLI_args = "-ORBInitRef NameService=iioploc://$host:$port/NameService ";


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

@policies = ("fifo", "priority", "lifo");
if ($deadline) {
    push(@policies, "deadline");
}

@server_opts = ("", "", "", " -d");
if ($status == 0) {
    for($i = 0; $i <= $#policies; $i++) {
        $discard_policy = $policies[$i];
        print "****** Structured Supplier -> Structured Consumer with the " .
              "$discard_policy policy ******\n";

        $STS->Arguments($STS->Arguments() . $server_opts[$i]);
        $STS_status = $STS->Spawn ();
        if ($STS_status != 0) {
            print STDERR "ERROR: Structured Supplier returned $STS_status\n";
            $status = 1;
            last;
        }
        if ($sts->WaitForFileTimed ($iorbase,$sts->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot1 find file <$sts_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($sts->GetFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$sts_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($stc->PutFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot set file <$stc_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        sleep 2;

        $STC->Arguments($CLI_args . " -k file://$stc_iorfile -d $discard_policy");
        $STC_status = $STC->SpawnWaitKill ($stc->ProcessStartWaitInterval()+5);
        if ($STC_status != 0) {
            print STDERR "ERROR: Structured Consumer returned $STC_status\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }

        $STS_status = $STS->WaitKill ($sts->ProcessStopWaitInterval());
        if ($STS_status != 0) {
            print STDERR "ERROR: Structured Supplier returned $STS_status\n";
            $status = 1;
            last;
        }

        $sts->DeleteFile ($iorbase);
        $stc->DeleteFile ($iorbase);
    }
}
$sts->DeleteFile ($iorbase);
$stc->DeleteFile ($iorbase);

if ($status == 0) {
    for($i = 0; $i <= $#policies; $i++) {
        $discard_policy = $policies[$i];
        print "***** Structured Supplier -> Sequence Consumer with the " .
              "$discard_policy policy *****\n";

        $STS->Arguments($STS->Arguments() . $server_opts[$i]);
        $STS_status = $STS->Spawn ();
        if ($STS_status != 0) {
            print STDERR "ERROR: Structured Supplier returned $STS_status\n";
            $status = 1;
            last;
        }
        if ($sts->WaitForFileTimed ($iorbase,$sts->ProcessStartWaitInterval()) == -1) {
             print STDERR "ERROR: cannot find file <$sts_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($sts->GetFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$sts_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($sec->PutFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot set file <$sec_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        sleep 2;

        $SEC->Arguments($CLI_args . " -k file://$sec_iorfile -d $discard_policy");
        $SEC_status = $SEC->SpawnWaitKill ($sec->ProcessStartWaitInterval()+5);
        if ($SEC_status != 0) {
             print STDERR "ERROR: Sequence Consumer returned $SEC_status\n";
             $STS->Kill (); $STS->TimedWait (1);
             $status = 1;
             last;
        }

        $STS_status = $STS->WaitKill ($sts->ProcessStopWaitInterval());
        if ($STS_status != 0) {
            print STDERR "ERROR: Structured Supplier returned $STS_status\n";
            $status = 1;
            last;
        }

        $sts->DeleteFile ($iorbase);
        $sec->DeleteFile ($iorbase);
    }
}
$sts->DeleteFile ($iorbase);
$sec->DeleteFile ($iorbase);

if ($status == 0) {
    for($i = 0; $i <= $#policies; $i++) {
        $discard_policy = $policies[$i];
        print "**** Sequence Supplier -> Sequence Consumer with the " .
              "$discard_policy policy ****\n";

        $SES->Arguments($SES->Arguments() . $server_opts[$i]);
        $SES_status = $SES->Spawn ();
        if ($SES_status != 0) {
            print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
            $status = 1;
            last;
        }
        if ($ses->WaitForFileTimed ($iorbase,$ses->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot find file <$ses_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($ses->GetFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$ses_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($sec->PutFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot set file <$sec_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        sleep 2;

        $SEC->Arguments($CLI_args . " -k file://$sec_iorfile -d $discard_policy");
        $SEC_status = $SEC->SpawnWaitKill ($sec->ProcessStartWaitInterval()+5);
        if ($SEC_status != 0) {
            print STDERR "ERROR: Sequence Consumer returned $SEC_status\n";
            $SES->Kill (); $SES->TimedWait (1);
            $status = 1;
            last;
         }

         $SES_status = $SES->WaitKill ($ses->ProcessStopWaitInterval());
         if ($SES_status != 0) {
            print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
            $status = 1;
            last;
        }

        $ses->DeleteFile ($iorbase);
        $sec->DeleteFile ($iorbase);
    }
}
$ses->DeleteFile ($iorbase);
$sec->DeleteFile ($iorbase);

if ($status == 0) {
    for($i = 0; $i <= $#policies; $i++) {
        $discard_policy = $policies[$i];
        print "**** Sequence Supplier -> Structured Consumer with the " .
              "$discard_policy policy ****\n";

        $SES->Arguments($SES->Arguments() . $server_opts[$i]);
        $SES_status = $SES->Spawn ();
        if ($SES_status != 0) {
            print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
            $status = 1;
            last;
        }
        if ($ses->WaitForFileTimed ($iorbase,$ses->ProcessStartWaitInterval()) == -1) {
            print STDERR "ERROR: cannot find file <$ses_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($ses->GetFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot retrieve file <$ses_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        if ($stc->PutFile ($iorbase) == -1) {
            print STDERR "ERROR: cannot set file <$stc_iorfile>\n";
            $STS->Kill (); $STS->TimedWait (1);
            $status = 1;
            last;
        }
        sleep 2;

        $STC->Arguments($CLI_args . " -k file://$stc_iorfile  -d $discard_policy");
        $STC_status = $STC->SpawnWaitKill ($stc->ProcessStartWaitInterval()+5);
        if ($STC_status != 0) {
            print STDERR "ERROR: Structured Consumer returned $STC_status\n";
            $status = 1;
            last;
        }

        $SES_status = $SES->WaitKill ($ses->ProcessStopWaitInterval());
        if ($SES_status != 0) {
            print STDERR "ERROR: Sequence Supplier returned $SES_status\n";
            $status = 1;
            last;
        }
        $ses->DeleteFile ($iorbase);
        $stc->DeleteFile ($iorbase);
    }
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
$sts->DeleteFile ($iorbase);
$stc->DeleteFile ($iorbase);
$sec->DeleteFile ($iorbase);
$ses->DeleteFile ($iorbase);

exit $status;
