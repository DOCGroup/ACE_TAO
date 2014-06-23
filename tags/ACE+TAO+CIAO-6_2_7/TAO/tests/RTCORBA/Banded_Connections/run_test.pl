eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

$iorbase1 = "test1.ior";
$iorbase2 = "test2.ior";

$client_iorfile1 = $client->LocalFile ($iorbase1);
$client_iorfile2 = $client->LocalFile ($iorbase2);
$server_iorfile1 = $server->LocalFile ($iorbase1);
$server_iorfile2 = $server->LocalFile ($iorbase2);

$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);
$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);

$status = 0;

my $svc_conf = 'svc.conf';

# copy the configuation files
if ($server->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($server->PutFile ($svc_conf.'.xml') == -1) {
    print STDERR "ERROR: cannot set file <".$server->LocalFile ($svc_conf.'.xml').">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf) == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf).">\n";
    exit 1;
}
if ($client->PutFile ($svc_conf.'.xml') == -1) {
    print STDERR "ERROR: cannot set file <".$client->LocalFile ($svc_conf.'.xml').">\n";
    exit 1;
}

print STDERR "\n********** RTCORBA Priority Banded Connections Unit Test\n";

# Different priority values for the bands on each OS.

if ($^O eq "MSWin32") {
    $server_args =
        "-b bands.nt";
}
elsif ($^O eq "dec_osf") {
    $server_args =
        "-b bands.tru64";
}
elsif ($^O eq "hpux") {
    $server_args =
        "-b bands.hpux";
}
elsif ($^O eq "irix") {
    $server_args =
        "-b bands.irix";
}
else {
    $server_args =
        "-b bands.unix";
    # since we only do remote testing on *nix platforms
    if ($server->PutFile ('bands.unix') == -1) {
        print STDERR "ERROR: cannot set file <".$server->LocalFile ('bands.unix').">\n";
        exit 1;
    }
}

$SV = $server->CreateProcess ("server", $server_args . " -n $server_iorfile1 -o $server_iorfile2"),

$CL = $client->CreateProcess ("client", "-n file://$client_iorfile1 -o file://$client_iorfile2");

$server_status = $SV->Spawn();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase2,
                               $server->ProcessStartWaitInterval()) == -1) {
    $server_status = $SV->TimedWait (1);
    if ($server_status == 2) {
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    }
    else {
        print STDERR "ERROR: cannot find file <$client_iorfile2>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval ());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval () + 15);

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$client->DeleteFile ($iorbase1);
$client->DeleteFile ($iorbase2);
$server->DeleteFile ($iorbase1);
$server->DeleteFile ($iorbase2);

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
