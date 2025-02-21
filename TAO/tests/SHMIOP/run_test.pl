eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $status = 0;
my $debug_level = 0;
my $cdebug_level = 0;
my $extra;
foreach my $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = 10;
    }
    elsif ($i eq '-cdebug') {
	$cdebug_level = 10;
    }
    elsif ($i eq 'with_collocated') {
	$extra = "-s temp.ior";
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

my $shm = '-ORBSvcConf static_shmiop.conf -ORBListenEndpoints shmiop://';

my $SV = $server->CreateProcess ("shmiop_test", "-ORBDebugLevel $debug_level $shm -s $server_iorfile");
my $CL = $client->CreateProcess ("shmiop_test", "-ORBDebugLevel $cdebug_level $shm -c $client_iorfile $extra");
my $server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval ()) == -1) {
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

my $client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile ($iorbase);
$client->DeleteFile ($iorbase);

if ($extra ne '') {
    unlink 'temp.ior';
}

exit $status;
