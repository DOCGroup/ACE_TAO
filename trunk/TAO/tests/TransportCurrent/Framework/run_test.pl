eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use Config;
use PerlACE::TestTarget;

my $status = 0;
my $confmod = "";

my $mode = shift (@ARGV);
if ( $mode =~ /-dynamic/) {
}
elsif  ( $mode =~ /-static/) {
    $confmod = "-static";
}
else {
    print STDERR "Unknown $mode. Specify -static or -dynamic\n";
    exit 1;
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$server->AddLibPath ('../lib');
$client->AddLibPath ('../lib');

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

my $confserverbase = "server$confmod.conf";
my $confserver = $server->LocalFile ("$confserverbase");
my $confclient = $client->LocalFile ("client$confmod.conf");

$SV = $server->CreateProcess ("server", "@ARGV -ORBSvcConf $confserver -o $server_iorfile");
$CL = $client->CreateProcess ("client", "@ARGV -n 1 -ORBSvcConf $confclient -k file://$client_iorfile");

$SV->Spawn ();

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "$0: ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval ());

if ($server_status != 0) {
    print STDERR "$0: ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
