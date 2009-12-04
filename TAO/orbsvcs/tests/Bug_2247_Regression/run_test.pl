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

my $file1_ior = "file1.ior";
my $file2_ior = "file2.ior";
my $output_ior = "output.ior";

my $server1   = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $server2   = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $managers_number = 4;
my @managers = ();
for ($i = 0; $i < $managers_number; $i++) {
    $managers[$i] = PerlACE::TestTarget::create_target ($i + 3) || die "Create target $i + 3 failed\n";
}

my $server1_file1_ior = $server1->LocalFile($file1_ior);
my $server2_file2_ior = $server2->LocalFile($file2_ior);
my @managers_file1_ior = ();
my @managers_file2_ior = ();
my @managers_output_ior = ();
for ($i = 0; $i < $managers_number; $i++) {
    $managers_file1_ior[$i] = $managers[$i]->LocalFile($file1_ior);
    $managers_file2_ior[$i] = $managers[$i]->LocalFile($file2_ior);
    $managers_output_ior[$i] = $managers[$i]->LocalFile($output_ior);
}

$SV1 = $server1->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server1_file1_ior -k KEY1");
$SV2 = $server2->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server2_file2_ior -k KEY2");

my @MNS_ARG_SUFFIX = ("", " -s", " -m", " -m -s");

@MNS = ();
for ($i = 0; $i < $managers_number; $i++) {
    $MNS[$i] = $managers[$i]->CreateProcess ("Manager", "-a file://$managers_file1_ior[$i] -k KEY1 ".
                                                        "-b file://$managers_file2_ior[$i] -l KEY2 ".
                                                        "-c $managers_output_ior[$i]".$MNS_ARG_SUFFIX[$i]);
}

sub clean_all {
    $server1->DeleteFile($file1_ior);
    $server2->DeleteFile($file2_ior);
    for ($i = 0; $i < $managers_number; $i++) {
        $managers[$i]->DeleteFile($file1_ior);
        $managers[$i]->DeleteFile($file2_ior);
        $managers[$i]->DeleteFile($output_ior);
    }
}

sub run_single {
    clean_all();

    my $name = $_[0];
    my $mn_id = $_[1];

    print STDERR "Starting $name test\n";
    print STDERR "Starting Server 1\n";
    my $server_result = $SV1->Spawn ();
    if ($server_result != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        return 1;
    }

    if ($server1->WaitForFileTimed ($file1_ior,
                                    $server1->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server1_file1_ior>\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($server1->GetFile ($file1_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server1_file1_ior>\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($managers[$mn_id]->PutFile ($file1_ior) == -1) {
        print STDERR "ERROR: cannot set file <$managers_file1_ior[$mn_id]>\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    print STDERR "Starting $name Manager\n";
    my $mn_status = $MNS[$mn_id]->SpawnWaitKill ($managers[$mn_id]->ProcessStartWaitInterval() + 15);

    if ($mn_status != 0) {
        print STDERR "ERROR: Manager $mn_id returned $mn_status\n";
        $status = 1;
    }

    $server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval());

    if ($server1_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    return 0;
}

sub run_merged {
    clean_all();

    my $name = $_[0];
    my $mn_id = $_[1];

    print STDERR "Starting MERGED $name test\n";
    print STDERR "Starting Server 1\n";
    my $server_result = $SV1->Spawn ();
    if ($server_result != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        return 1;
    }

    if ($server1->WaitForFileTimed ($file1_ior,
                                    $server1->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server1_file1_ior>\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($server1->GetFile ($file1_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server1_file1_ior>\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($managers[$mn_id]->PutFile ($file1_ior) == -1) {
        print STDERR "ERROR: cannot set file <$managers_file1_ior[$mn_id]>\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    print STDERR "Starting Server 2\n";
    $server_result = $SV2->Spawn ();
    if ($server_result != 0) {
        print STDERR "ERROR: server 2 returned $server_status\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($server2->WaitForFileTimed ($file2_ior,
                                    $server2->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server2_file2_ior>\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($server2->GetFile ($file2_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server2_file2_ior>\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    if ($managers[$mn_id]->PutFile ($file2_ior) == -1) {
        print STDERR "ERROR: cannot set file <$managers_file2_ior[$mn_id]>\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    print STDERR "Starting $name Manager\n";
    my $mn_status = $MNS[$mn_id]->SpawnWaitKill ($managers[$mn_id]->ProcessStartWaitInterval() + 15);

    if ($mn_status != 0) {
        print STDERR "ERROR: Manager $mn_id returned $mn_status\n";
        $status = 1;
    }

    $server_status = $SV1->WaitKill ($server1->ProcessStopWaitInterval() + 5);

    if ($server1_status != 0) {
        print STDERR "ERROR: server 1 returned $server_status\n";
        $status = 1;
    }

    $server_status = $SV2->WaitKill ($server2->ProcessStopWaitInterval() + 5);

    if ($server_status != 0) {
        print STDERR "ERROR: server 2 returned $server_status\n";
        $status = 1;
    }

    return $status;
}

if (run_single("ABORT", 0) != 0) {
    exit 1;
}

if (run_single("SHUTDOWN", 1) != 0) {
    exit 1;
}

if (run_merged("ABORT", 2) != 0) {
    exit 1;
}

if (run_merged("SHUTDOWN", 3) != 0) {
    exit 1;
}

clean_all();

exit $status
