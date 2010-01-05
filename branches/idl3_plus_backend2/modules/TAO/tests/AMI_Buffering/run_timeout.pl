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
my $admin = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $iorfile_admin = "admin.ior";
my $iorfile = "server.ior";

#Files which used by server
my $server_iorfile = $server->LocalFile ($iorfile);
my $server_iorfile_admin = $server->LocalFile ($iorfile_admin);
$server->DeleteFile($iorfile);
$server->DeleteFile($iorfile_admin);

#Files which used by client
my $client_iorfile = $client->LocalFile ($iorfile);
my $client_iorfile_admin = $client->LocalFile ($iorfile_admin);
$client->DeleteFile($iorfile);
$client->DeleteFile($iorfile_admin);

#Files which used by admin
my $admin_iorfile_admin = $admin->LocalFile ($iorfile_admin);
$admin->DeleteFile($iorfile_admin);

$AD = $admin->CreateProcess ("admin",
                              "-ORBdebuglevel $debug_level " .
                              "-o $admin_iorfile_admin");

$SV = $server->CreateProcess ("server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $server_iorfile " .
                              "-k file://$server_iorfile_admin");

$CL = $client->CreateProcess ("client",
                              "-k file://$client_iorfile " .
                              "-a file://$client_iorfile_admin " .
                              "-t");

$admin_status = $AD->Spawn ();

if ($admin_status != 0) {
    print STDERR "ERROR: admin returned $admin_status\n";
    exit 1;
}

if ($admin->WaitForFileTimed ($iorfile_admin,
                               $admin->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile_admin>\n";
    $AD->Kill (); $AD->TimedWait (1);
    exit 1;
}
if ($admin->GetFile ($iorfile_admin) == -1) {
    print STDERR "ERROR: cannot retrieve file <$admin_iorfile_admin>\n";
    $AD->Kill (); $AD->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorfile_admin) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile_admin>\n";
    $AD->Kill (); $AD->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($iorfile_admin) == -1) {
    print STDERR "ERROR: cannot set file <$server_iorfile_admin>\n";
    $AD->Kill (); $AD->TimedWait (1);
    exit 1;
}

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

sub KillServers{
    $SV->Kill (); $SV->TimedWait (1);
    $AD->Kill (); $AD->TimedWait (1);
}

if ($server->WaitForFileTimed ($iorfile,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    KillServers();
    exit 1;
}

if ($server->GetFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    KillServers();
    exit 1;
}
if ($client->PutFile ($iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    KillServers();
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$admin_status = $AD->WaitKill ($admin->ProcessStopWaitInterval());

if ($admin_status != 0) {
    print STDERR "ERROR: admin returned $admin_status\n";
    $status = 1;
}

$server->DeleteFile($iorfile);
$client->DeleteFile($iorfile);
$client->DeleteFile($iorfile_admin);
$server->DeleteFile($iorfile_admin);
$admin->DeleteFile($iorfile_admin);

exit $status;
