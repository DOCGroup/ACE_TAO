eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use POSIX "sys_wait_h";

$status = 0;

$debug_opts = '';
$srv_debug = '';
foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_opts = '-ORBDebugLevel 10 -ORBVerboseLogging 1 '
                    . '-ORBLogFile client';
        $srv_debug  = '-ORBDebugLevel 5';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $shutdown = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
my $shutdown_iorfile = $shutdown->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$shutdown->DeleteFile ($iorbase);

$SV = $server->CreateProcess ("server", "-o $server_iorfile $srv_debug");
my $threads = int (rand() * 6) + 1;
$CL = $client->CreateProcess ("client", "-k file://$client_iorfile -t $threads $debug_opts");
$SH = $shutdown->CreateProcess ("shutdown", "-k file://$shutdown_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

local $start_time = time();
local $max_running_time = 720;
local $elapsed = time() - $start_time;

if ($ARGV[0] eq '-quick')  {
    $elapsed = 0;
    $max_running_time = 1;
}

my $client_idx = 0;
while (($elapsed < $max_running_time) ) {
    # Start all clients in parallel
    my $args_saved = $CL->Arguments ();
    $CL->Arguments ("$args_saved$client_idx.log") unless $debug_opts eq '';
    my $client_status = $CL->Spawn ();
    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }

    if ($debug_opts ne '') {
        $CL->Arguments ($args_saved);
        print "Spawned client $client_idx\n";
        ++$client_idx;
    }

    if ($client_status == 0) {
        $CL->WaitKill($client->ProcessStartWaitInterval() + 75);
        if ($client_status != 0) {
            print STDERR "ERROR: client returned $client_status\n";
            $status = 1;
        }
    }

    print STDERR "checking server alive\n";

    my $res = $SV->Wait(1);

    if ($res != -1) {
        $server_died = 1;
        last;
    }

    $elapsed = time() - $start_time;
}

if (!$server_died) {
    my $shutdown_status = $SH->SpawnWaitKill($shutdown->ProcessStartWaitInterval());
    if ($shutdown_status != 0) {
        print STDERR "ERROR: shutdown returned $$shutdown_status\n";
        $status = 1;
    }

    $server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 300);
    if ($server_status != 0) {
        print STDERR "ERROR: server returned $server_status\n";
        $status = 1;
    }
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
$shutdown->DeleteFile ($iorbase);

exit $status;
