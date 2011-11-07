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

my $in_server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $md_server = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target (3) || die "Create target 3 failed\n";

my $inner_ior = "inner.ior";
my $middle_ior =  "middle.ior";

# Files which used by inner_server
my $in_server_inner_ior = $in_server->LocalFile ($inner_ior);
$in_server->DeleteFile($inner_ior);

# Files which used by middle server
my $md_server_inner_ior = $md_server->LocalFile ($inner_ior);
my $md_server_middle_ior = $md_server->LocalFile ($middle_ior);
$md_server->DeleteFile($inner_ior);
$md_server->DeleteFile($middle_ior);

# Files which used by inner_server
my $client_iorfile = $client->LocalFile ($middle_ior);
$client->DeleteFile($middle_ior);

$IS = $in_server->CreateProcess ("inner_server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $in_server_inner_ior");

$MD = $md_server->CreateProcess ("middle_server",
                              "-ORBdebuglevel $debug_level " .
                              "-o $md_server_middle_ior " .
                              "-i file://$md_server_inner_ior");

$CL = $client->CreateProcess ("client", "-k file://$client_iorfile");

$server_status = $IS->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($in_server->WaitForFileTimed ($inner_ior,
                               $in_server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$in_server_inner_ior>\n";
    $IS->Kill (); $IS->TimedWait (1);
    exit 1;
}

if ($in_server->GetFile ($inner_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$in_server_inner_ior>\n";
    $IS->Kill (); $IS->TimedWait (1);
    exit 1;
}
if ($md_server->PutFile ($inner_ior) == -1) {
    print STDERR "ERROR: cannot set file <$md_server_inner_ior>\n";
    $IS->Kill (); $IS->TimedWait (1);
    exit 1;
}

$server_status = $MD->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($md_server->WaitForFileTimed ($middle_ior,
                               $md_server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$md_server_middle_ior>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $IS->Kill (); $IS->TimedWait (1);
    exit 1;
}

if ($md_server->GetFile ($middle_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$md_server_middle_ior>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $IS->Kill (); $IS->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($middle_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $IS->Kill (); $IS->TimedWait (1);
    exit 1;
}

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 30);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$IS->Kill ();
$MD->Kill ();

$in_server->DeleteFile($inner_ior);
$md_server->DeleteFile($inner_ior);
$md_server->DeleteFile($middle_ior);
$client->DeleteFile($middle_ior);

exit $status;
