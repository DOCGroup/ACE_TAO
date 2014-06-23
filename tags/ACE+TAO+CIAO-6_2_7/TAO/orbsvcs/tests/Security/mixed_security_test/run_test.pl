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

my $ior_restricted = "restricted.ior";
my $ior_permitted = "permitted.ior";

my $server_conf = "server$PerlACE::svcconf_ext";
my $client_conf = "client$PerlACE::svcconf_ext";

my $server_iorfile_restricted = $server->LocalFile ($ior_restricted);
my $server_iorfile_permitted = $server->LocalFile ($ior_permitted);
my $client_iorfile_restricted = $client->LocalFile ($ior_restricted);
my $client_iorfile_permitted = $client->LocalFile ($ior_permitted);
$server->DeleteFile($ior_restricted);
$server->DeleteFile($ior_permitted);
$client->DeleteFile($ior_restricted);
$client->DeleteFile($ior_permitted);

# Set the SSL environment
# This doesn't work on Windows.  For some reason,
# environment variables aren't propagated to child processes.
#$ENV{'SSL_CERT_FILE'} = 'cacert.pem';

$SV = $server->CreateProcess ("server", "-ORBSvcConf $server_conf");
$CL = $client->CreateProcess ("client", "-ORBSvcConf $client_conf");

print STDOUT "\n\n==== Running mixed_security_test test\n";

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($ior_permitted,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ior_permitted>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->WaitForFileTimed ($ior_restricted,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$ior_restricted>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($ior_permitted) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile_permitted>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ior_restricted) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile_restricted>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($server->GetFile ($ior_restricted) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile_restricted>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($ior_permitted) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile_permitted>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 45);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($ior_restricted);
$client->DeleteFile($ior_permitted);
$server->DeleteFile($ior_restricted);
$client->DeleteFile($ior_permitted);

exit $status;
