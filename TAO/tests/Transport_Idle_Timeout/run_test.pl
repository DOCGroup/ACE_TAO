#!/usr/bin/perl

# Test driver for the idle-transport-timeout regression test.
# Follows the standard TAO test-script conventions used throughout
# TAO/tests/ (PerlACE helpers, two-process server/client, IOR handshake).
#
# Scenarios driven:
#   1.  TC-1, TC-2, TC-3 — timeout enabled (3 s), normal close/reconnect/reuse
#   2.  TC-4              — timeout disabled (0), connection must persist

use strict;
use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

my $ior_file1 = "test1.ior";
my $ior_file2 = "test2.ior";
my $timeout_sec = 3;   # Y, must match svc.conf
my $scan_interval_sec = 2; # X, must match svc.conf
my $status = 0;
my $debug_level = '0';
my $cdebug_level = '0';
foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
    if ($i eq '-cdebug') {
      $cdebug_level = '10';
    }
}

sub run_scenario {
    my ($label, $svc_conf, $extra_client_args) = @_;

    print "\n### $label ###\n";

    my $server = PerlACE::TestTarget::create_target (1) || die "Cannot create server target";
    my $client = PerlACE::TestTarget::create_target (2) || die "Cannot create client target";

    my $server_ior = $server->LocalFile ($ior_file1);
    my $client_ior = $client->LocalFile ($ior_file1);

    $server->DeleteFile ($ior_file1);
    $client->DeleteFile ($ior_file1);

    my $SV = $server->CreateProcess (
        "server",
        "-ORBdebuglevel $debug_level -ORBVerboseLogging 1 -ORBSvcConf $svc_conf -o $server_ior"
    );

    my $CL = $client->CreateProcess (
        "client",
        "-ORBdebuglevel $cdebug_level -ORBVerboseLogging 1 -ORBSvcConf $svc_conf -k file://$client_ior -t $timeout_sec -x $scan_interval_sec $extra_client_args"
    );

    # Start server
    my $server_status = $SV->Spawn ();
    if ($server_status != 0) {
        print STDERR "ERROR: server Spawn returned $server_status\n";
        return 1;
    }

    # Wait for IOR file to appear (up to 30 s)
    if ($server->WaitForFileTimed ($ior_file1,
                                   $server->ProcessStartWaitInterval ()) == -1) {
        print STDERR "ERROR: IOR file '$server_ior' not created\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    # Transfer IOR file to client (no-op on single-host runs)
    if ($server->GetFile ($ior_file1) == -1) {
        print STDERR "ERROR: server GetFile '$ior_file1' failed\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }
    if ($client->PutFile ($ior_file1) == -1) {
        print STDERR "ERROR: client PutFile '$ior_file1' failed\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    # Allow spaced reuse pings and Y + X + margin expiry waits.
    my $client_budget = ($extra_client_args =~ /-d/) ? 15 : 60;
    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ()
                                            + $client_budget);
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    # Wait for server to exit (it calls orb->shutdown() on receiving shutdown())
    my $server_exit = $SV->WaitKill ($server->ProcessStopWaitInterval ());
    if ($server_exit != 0) {
        print STDERR "ERROR: server returned $server_exit\n";
        $status = 1;
    }

    $server->DeleteFile ($ior_file1);
    $client->DeleteFile ($ior_file1);

    return $status;
}

sub run_multiple_scenario {
    my ($label, $svc_conf, $extra_client_args) = @_;

    print "\n### $label ###\n";

    my $server1 = PerlACE::TestTarget::create_target (1) || die "Cannot create server1 target";
    my $server2 = PerlACE::TestTarget::create_target (2) || die "Cannot create server2 target";
    my $client = PerlACE::TestTarget::create_target (3) || die "Cannot create client target";

    my $server1_ior = $server1->LocalFile ($ior_file1);
    my $server2_ior = $server2->LocalFile ($ior_file2);
    my $client1_ior = $client->LocalFile ($ior_file1);
    my $client2_ior = $client->LocalFile ($ior_file2);

    $server1->DeleteFile ($ior_file1);
    $server2->DeleteFile ($ior_file2);
    $client->DeleteFile ($ior_file1);
    $client->DeleteFile ($ior_file2);

    my $SV1 = $server1->CreateProcess (
        "server",
        "-ORBdebuglevel $debug_level -ORBVerboseLogging 1 -ORBSvcConf $svc_conf -o $server1_ior"
    );
    my $SV2 = $server2->CreateProcess (
        "server",
        "-ORBdebuglevel $debug_level -ORBVerboseLogging 1 -ORBSvcConf $svc_conf -o $server2_ior"
    );

    my $CL = $client->CreateProcess (
        "client_multiple",
        "-ORBdebuglevel $cdebug_level -ORBVerboseLogging 1 -ORBSvcConf $svc_conf -k file://$client1_ior -l file://$client2_ior -t $timeout_sec -x $scan_interval_sec $extra_client_args"
    );

    # Start server 1
    my $server1_status = $SV1->Spawn ();
    if ($server1_status != 0) {
        print STDERR "ERROR: server1 Spawn returned $server1_status\n";
        return 1;
    }

    # Start server 2
    my $server2_status = $SV2->Spawn ();
    if ($server2_status != 0) {
        print STDERR "ERROR: server2 Spawn returned $server2_status\n";
        return 1;
    }

    # Wait for IOR file 1 to appear (up to 30 s)
    if ($server1->WaitForFileTimed ($ior_file1,
                                   $server1->ProcessStartWaitInterval ()) == -1) {
        print STDERR "ERROR: IOR file '$server1_ior' not created\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }

    # Wait for IOR file 2 to appear (up to 30 s)
    if ($server2->WaitForFileTimed ($ior_file2,
                                   $server2->ProcessStartWaitInterval ()) == -1) {
        print STDERR "ERROR: IOR file '$server2_ior' not created\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        return 1;
    }

    # Transfer IOR file to client (no-op on single-host runs)
    if ($server1->GetFile ($ior_file1) == -1) {
        print STDERR "ERROR: server1 GetFile '$ior_file1' failed\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }
    if ($client->PutFile ($ior_file1) == -1) {
        print STDERR "ERROR: client PutFile '$ior_file1' failed\n";
        $SV1->Kill (); $SV1->TimedWait (1);
        return 1;
    }
    if ($server2->GetFile ($ior_file2) == -1) {
        print STDERR "ERROR: server2 GetFile '$ior_file2' failed\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        return 1;
    }
    if ($client->PutFile ($ior_file2) == -1) {
        print STDERR "ERROR: client PutFile '$ior_file2' failed\n";
        $SV2->Kill (); $SV2->TimedWait (1);
        return 1;
    }

    # Allow spaced reuse pings and Y + X + margin expiry waits.
    my $client_budget = ($extra_client_args =~ /-d/) ? 15 : 60;
    my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ()
                                            + $client_budget);
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    # Wait for server 1 to exit (it calls orb->shutdown() on receiving shutdown())
    my $server_exit1 = $SV1->WaitKill ($server1->ProcessStopWaitInterval ());
    if ($server_exit1 != 0) {
        print STDERR "ERROR: server1 returned $server_exit1\n";
        $status = 1;
    }
    # Wait for server 2 to exit (it calls orb->shutdown() on receiving shutdown())
    my $server_exit2 = $SV2->WaitKill ($server2->ProcessStopWaitInterval ());
    if ($server_exit2 != 0) {
        print STDERR "ERROR: server2 returned $server_exit2\n";
        $status = 1;
    }

    $server1->DeleteFile ($ior_file1);
    $server2->DeleteFile ($ior_file2);
    $client->DeleteFile ($ior_file1);
    $client->DeleteFile ($ior_file2);

    return $status;
}

# ---------------------------------------------------------------------------
# Scenario 1: TC-1, TC-2, TC-3  (timeout enabled)
# ---------------------------------------------------------------------------
$status = 1 if run_scenario (
    "TC-1/TC-2/TC-3: idle timeout enabled (${timeout_sec}s)",
    "svc.conf",
    ""
);

# ---------------------------------------------------------------------------
# Scenario 2: TC-4  (timeout disabled)
# ---------------------------------------------------------------------------
$status = 1 if run_scenario (
    "TC-4: idle timeout disabled",
    "svc_disabled.conf",
    "-d"
);

$status = 1 if run_multiple_scenario (
    "TC-1/TC-2/TC-3: multiple idle timeout enabled (${timeout_sec}s)",
    "svc.conf",
    ""
);
# ---------------------------------------------------------------------------
# Final result
# ---------------------------------------------------------------------------
exit $status;
