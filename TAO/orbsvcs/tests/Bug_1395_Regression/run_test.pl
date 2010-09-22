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

my $server    = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client    = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $locator   = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";
my $activator = PerlACE::TestTarget::create_target (4) || die "Create target 4 failed\n";
my $tao_imr   = PerlACE::TestTarget::create_target (5) || die "Create target 5 failed\n";

# The location of the implementation repository binaries
my $imr_bin_path  = "../../ImplRepo_Service";
# The location of the tao_imr IMR utility
my $tao_imr_bin_path = "$ENV{ACE_ROOT}/bin";

# Run the IMR locator on a fixed port
my $port = $locator->RandomPort();

my $imr_ior  = "impl.ior";
my $actv_ior = "activator.ior";
my $srv_ior  = "server.ior";

my $server_srv_ior     = $server->LocalFile ($srv_ior);
my $server_imr_ior     = $server->LocalFile ($imr_ior);
my $client_srv_ior     = $client->LocalFile ($srv_ior);
my $locator_imr_ior    = $locator->LocalFile ($imr_ior);
my $tao_imr_imr_ior    = $tao_imr->LocalFile ($imr_ior);
my $activator_actv_ior = $activator->LocalFile ($actv_ior);
my $activator_imr_ior  = $activator->LocalFile ($imr_ior);
$server->DeleteFile($srv_ior);
$server->DeleteFile($imr_ior);
$client->DeleteFile($srv_ior);
$locator->DeleteFile ($imr_ior);
$tao_imr->DeleteFile ($imr_ior);
$activator->DeleteFile ($actv_ior);
$activator->DeleteFile ($imr_ior);

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_srv_ior ".
                                        "-ORBInitRef ImplRepoService=file://$server_imr_ior ".
                                        "-ORBUseIMR 1");
$CL = $client->CreateProcess ("client", "-k file://$client_srv_ior");
$LC = $locator->CreateProcess ("$imr_bin_path/tao_imr_locator", "-o $locator_imr_ior ".
                                                                 "-ORBEndpoint iiop://:$port");
$AC = $activator->CreateProcess ("$imr_bin_path/ImR_Activator",
                                 "-o $activator_actv_ior ".
                                 "-ORBInitRef ImplRepoService=file://$activator_imr_ior");

$TI = $tao_imr->CreateProcess ("$tao_imr_bin_path/tao_imr",
                               "add MyPoa -ORBInitRef ImplRepoService=file://$tao_imr_imr_ior");
sub run_locator {
    $server->DeleteFile($imr_ior);
    $locator->DeleteFile ($imr_ior);
    $tao_imr->DeleteFile ($imr_ior);
    $activator->DeleteFile ($imr_ior);

    $process_status = $LC->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: locator returned $process_status\n";
        return 1;
    }

    if ($locator->WaitForFileTimed ($imr_ior,
                                    $locator->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$locator_imr_ior>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($locator->GetFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$locator_imr_ior>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($server->PutFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot set file <$server_imr_ior>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($tao_imr->PutFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot set file <$tao_imr_imr_ior>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    if ($activator->PutFile ($imr_ior) == -1) {
        print STDERR "ERROR: cannot set file <$activator_imr_ior>\n";
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    return 0;
}

sub run_server {
    $server->DeleteFile($srv_ior);
    $client->DeleteFile($srv_ior);

    $process_status = $SV->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: server returned $process_status\n";
        return 1;
    }

    if ($server->WaitForFileTimed ($srv_ior,
                                   $server->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$server_srv_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($server->GetFile ($srv_ior) == -1) {
        print STDERR "ERROR: cannot retrieve file <$server_srv_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    if ($client->PutFile ($srv_ior) == -1) {
        print STDERR "ERROR: cannot set file <$client_srv_ior>\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }
    return 0;
}

sub test_body {
    # Start the IMR locator to generate an IOR file for the server to use...
    $process_status = run_locator();

    if ($process_status != 0) {
        print STDERR "ERROR: run_locator returned $process_status\n";
        return 1;
    }

    # ...then shut it down so that the server will not be able to contact it
    $process_status = $LC->TerminateWaitKill ($locator->ProcessStopWaitInterval());

    if ($process_status != 0) {
        print STDERR "ERROR: locator returned $process_status\n";
        return 1;
    }

    # Start our server
    $process_status = run_server();

    if ($process_status != 0) {
        print STDERR "ERROR: run_server returned $process_status\n";
        return 1;
    }

    # Use the client to ask the server to try and create a persitent POA
    # We expect this to 'fail' as the IMR is dead
    $process_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 15);

    if ($process_status == 0) {
        print STDERR "ERROR: First create POA attempt succeeded when it shouldn't have done\n";
        $SV->Kill (); $SV->TimedWait (1);
        return 1;
    }

    # Now we restart the IMR locator
    $process_status = run_locator ();

    if ($process_status != 0) {
        print STDERR "ERROR: run_locator returned $process_status\n";
        return 1;
    }

    # Work out the IMR activator command line.
    # Use the '-a' switch if this is a regression for bug #1394, else not
    if ($use_tao_imr_util == 0) {
        my $activator_args = $activator->Arguments();
        $activator->Arguments("$activator_args -a");
    }

   # Start up the activator
    $process_status = $AC->Spawn ();

    if ($process_status != 0) {
        print STDERR "ERROR: activator returned $process_status\n";
        return 1;
    }

    if ($activator->WaitForFileTimed ($actv_ior,
                                      $activator->ProcessStartWaitInterval()) == -1) {
        print STDERR "ERROR: cannot find file <$activator_actv_ior>\n";
        $AC->Kill (); $AC->TimedWait (1);
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    ## Since we have restarted the locator, we need to restart
    ## the server so that it can pick up the new locator IOR
    $SV->Kill (); $SV->TimedWait (1);

    $process_status = run_server();

    if ($process_status != 0) {
        print STDERR "ERROR: run_server returned $process_status\n";
        $AC->Kill (); $AC->TimedWait (1);
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

    # If this is just a regression for bug #1395 we need to register the POA
    # If it is a regression for enhancement bug #1394, we don't need to.
    if ($use_tao_imr_util != 0) {
        # Add the persistent POA name to the IMR
        $process_status = $TI->SpawnWaitKill ($tao_imr->ProcessStartWaitInterval() + 15);

        if ($process_status != 0) {
            print STDERR "ERROR: tao_imr returned $process_status\n";
            $AC->Kill (); $AC->TimedWait (1);
            $LC->Kill (); $LC->TimedWait (1);
            return 1;
        }
    }

    # Use the client to tell the server to attempt to create the POA again
    $process_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 15);

    if ($process_status != 0) {
        print STDERR "ERROR: Second create POA attempt failed when it should have succeeded\n";
        $SV->Kill (); $SV->TimedWait (1);
        $AC->Kill (); $AC->TimedWait (1);
        $LC->Kill (); $LC->TimedWait (1);
        return 1;
    }

   # Tidy up
    $process_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

    if ($process_status != 0) {
        print STDERR "ERROR: server returned $process_status\n";
        $status = 1;
    }

    $process_status = $LC->TerminateWaitKill ($locator->ProcessStopWaitInterval());

    if ($process_status != 0) {
        print STDERR "ERROR: locator returned $process_status\n";
        $status = 1;
    }

    $process_status = $AC->TerminateWaitKill ($activator->ProcessStopWaitInterval());

    if ($process_status != 0) {
        print STDERR "ERROR: activator returned $process_status\n";
        $status = 1;
    }

   return $status;
}

# Run regression for bug #1395
$use_tao_imr_util = 1;
$status = test_body();

if ($status != 0) {
    print STDERR "ERROR: Regression test for Bug #1395 failed\n";
}

# Bug 1394 is an enhancement so will not be submitted until after TAO1.3.1
# Uncomment the following to activate regression after submission and ..
# Run regression for bug #1394
#$use_tao_imr_util = 0;
#$test_result = test_body();
#if ($test_result != 0)
#{
#   print STDERR "ERROR: Regression test for Bug #1394 failed\n";
#}

exit $status;
