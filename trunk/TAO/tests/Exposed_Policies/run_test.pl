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

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $ior_base = "default.ior";
my $ior_overridden = "overridden.ior";
my $cfg_poa = "POA.cfg";
my $cfg_object = "Object.cfg";
my $cfg_suffix_tru64 = ".tru64";

my $server_ior_base = $server->LocalFile ($ior_base);
my $server_ior_overridden = $server->LocalFile ($ior_overridden);
my $client_ior_base = $client->LocalFile ($ior_base);
my $client_ior_overridden = $client->LocalFile ($ior_overridden);
$server->DeleteFile($ior_base);
$server->DeleteFile($ior_overridden);
$client->DeleteFile($ior_base);
$client->DeleteFile($ior_overridden);

my $server_cfg_poa = $server->LocalFile ($cfg_poa);
my $server_cfg_object = $server->LocalFile ($cfg_object);
my $client_cfg_poa = $client->LocalFile ($cfg_poa);
my $client_cfg_object = $client->LocalFile ($cfg_object);

if ($server->PutFile ($cfg_poa) == -1) {
    print STDERR "ERROR: cannot set file <$server_cfg_poa>\n";
    return 1;
}
if ($server->PutFile ($cfg_object) == -1) {
    print STDERR "ERROR: cannot set file <$server_cfg_object>\n";
    return 1;
}

if ($^O eq "dec_osf") {
    $server_cfg_poa = "$server_cfg_poa$cfg_suffix_tru64";
    $server_cfg_object = "$server_cfg_object$cfg_suffix_tru64";
    $client_cfg_poa = "$client_cfg_poa$cfg_suffix_tru64";
    $client_cfg_object = "$client_cfg_object$cfg_suffix_tru64";
}

$SV = $server->CreateProcess ("server", "-ORBdebuglevel $debug_level ".
                                        "-POAConfigFile $server_cfg_poa ".
                                        "-ObjectConfigFile $server_cfg_object ".
                                        "-BaseObjectIOR $server_ior_base ".
                                        "-OverriddenIOR $server_ior_overridden");

$CL = $client->CreateProcess ("client", "-POAConfigFile $client_cfg_poa ".
                                        "-ObjectConfigFile $client_cfg_object ".
                                        "-BaseObjectIOR $client_ior_base ".
                                        "-OverriddenIOR $client_ior_overridden");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($ior_base,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_ior_base>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($ior_base) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_ior_base>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($ior_overridden) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_ior_overridden>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ior_base) == -1) {
    print STDERR "ERROR: cannot set file <$client_ior_base>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ior_overridden) == -1) {
    print STDERR "ERROR: cannot set file <$client_ior_overridden>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 15);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->TerminateWaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($ior_base);
$server->DeleteFile($ior_overridden);
$client->DeleteFile($ior_base);
$client->DeleteFile($ior_overridden);

exit $status;
