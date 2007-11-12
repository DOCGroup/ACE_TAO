eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$iorbase = "test.ior";
$svcconfbase = "svc$PerlACE::svcconf_ext";
$iorfile = PerlACE::LocalFile ("$iorbase");
$svcfile = PerlACE::LocalFile ("$svcconfbase");

if (PerlACE::is_vxworks_test()) {
$BSV = new PerlACE::ProcessVX ("blocking_server",
                 "-ORBSvcConf $svcconfbase -o $iorbase");
}
else {
$BSV = new PerlACE::Process ("blocking_server",
                 "-ORBSvcConf $svcfile -o $iorfile");
}

$ASV = new PerlACE::Process ("ami_server",
			     "-ORBSvcConf $svcfile -o $iorfile");
$BCL = new PerlACE::Process ("blocking_client",
			     "-ORBSvcConf $svcfile -k file://$iorfile");
$ACL = new PerlACE::Process ("ami_client",
			     "-ORBSvcConf $svcfile -k file://$iorfile");

print STDERR "==== Server upcall waits for operations on other threads\n";

unlink $iorfile;

$BSV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $BSV->Kill ();
    exit 1;
}

$client = $BCL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $BSV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "==== Server upcall waits for AMI operations on other threads\n";

unlink $iorfile;
$ASV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $ASV->Kill ();
    exit 1;
}

$client = $BCL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $ASV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "==== AMI Client, Server upcall waits for operations on other threads\n";

unlink $iorfile;
$BSV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $BSV->Kill ();
    exit 1;
}

$client = $ACL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $BSV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

print STDERR "==== AMI Client, Server upcall waits AMI operations\n";

unlink $iorfile;
$ASV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $ASV->Kill ();
    exit 1;
}

$client = $ACL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $ASV->WaitKill (30);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
