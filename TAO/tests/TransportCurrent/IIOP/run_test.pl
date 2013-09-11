eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use Config;
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$server->AddLibPath ('../lib');
$client->AddLibPath ('../lib');

my $iorbase = "server.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$status = 0;

$mode = shift (@ARGV);
if ( $mode =~ /-dynamic/) {
    $base_client_conf = "client_dynamic" . $PerlACE::svcconf_ext;
    $base_server_conf = "server_dynamic" . $PerlACE::svcconf_ext;
    $client_conf_file = $client->LocalFile ("$base_client_conf");
    $server_conf_file = $server->LocalFile ("$base_server_conf");
}
elsif  ( $mode =~ /-static/) {
    $base_client_conf = "client_static" . $PerlACE::svcconf_ext;
    $base_server_conf = "server_static" . $PerlACE::svcconf_ext;
    $client_conf_file = $client->LocalFile ("$base_client_conf");
    $server_conf_file = $server->LocalFile ("$base_server_conf");
}
else {
    print STDERR "Unknown $mode. Specify -static or -dynamic\n";
    exit 1;
}

if ($server->PutFile ($base_server_conf) == -1) {
    print STDERR "ERROR: cannot set file <$server_conf_file>\n";
    exit 1;
}
if ($client->PutFile ($base_client_conf) == -1) {
    print STDERR "ERROR: cannot set file <$client_conf_file>\n";
    exit 1;
}

$SV = $server->CreateProcess ("server",
                              "@ARGV -c 0 -ORBSvcConf $server_conf_file -o $server_iorfile");

$CL = $client->CreateProcess ("client",
                              "@ARGV -n 1 -ORBSvcConf $client_conf_file -k file://$client_iorfile");

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
