eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';
$servers = 300;

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);
@CL = ();
$SV = $server->CreateProcess ("master", "-ORBdebuglevel $debug_level -o $server_iorfile -c $servers");

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

for($i = 0; $i < $servers; $i++) {
    $CL[$i] = $client->CreateProcess ("server", "-k file://$client_iorfile");
    $client_status = $CL[$i]->Spawn ($client->ProcessStartWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: client returned $client_status\n";
        $status = 1;
    }
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval() + 500);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

for($i = 0; $i < $servers; $i++) {
    $client_status = $CL[$i]->WaitKill ($client->ProcessStartWaitInterval());

    if ($client_status != 0) {
        print STDERR "ERROR: client $i returned $client_status\n";
        $status = 1;
    }
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
