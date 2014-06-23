eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";


$server_debug_level = '0';
$client_debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $server_debug_level = '10';
        $client_debug_level = '10';
    }
}

my $iorbase_master = "master.ior";
my $iorbase_slave = "slave.ior";
my $server_master_iorfile = $server->LocalFile ($iorbase_master);
my $server_slave_iorfile = $server->LocalFile ($iorbase_slave);
my $client_master_iorfile = $client->LocalFile ($iorbase_master);
my $client_slave_iorfile = $client->LocalFile ($iorbase_slave);
$server->DeleteFile($iorbase_master);
$server->DeleteFile($iorbase_slave);
$client->DeleteFile($iorbase_master);
$client->DeleteFile($iorbase_slave);

$SV = $server->CreateProcess ("tester", "-ORBdebuglevel $server_debug_level -o $server_slave_iorfile -k file://$server_master_iorfile");

$server_status = $SV->Spawn ();

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    exit 1;
}

if ($server->WaitForFileTimed ($iorbase_slave,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_slave_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($server->GetFile ($iorbase_slave) == -1) {
    print STDERR "ERROR: cannot get file <$server_slave_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase_slave) == -1) {
    print STDERR "ERROR: cannot set file <$client_slave_iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$CL = $client->CreateProcess ("tester",
                              "-ORBdebuglevel $client_debug_level"
                              . " -o $client_master_iorfile"
                              . " -m -k file://$client_slave_iorfile ");

$client_status = $CL->Spawn ();

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

if ($client->WaitForFileTimed ($iorbase_master,
                               $client->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$client_master_iorfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}
if ($client->GetFile ($iorbase_master) == -1) {
    print STDERR "ERROR: cannot get file <$client_master_iorfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}
if ($server->PutFile ($iorbase_master) == -1) {
    print STDERR "ERROR: cannot set file <$server_master_iorfile>\n";
    $CL->Kill (); $CL->TimedWait (1);
    exit 1;
}

$client_status = $CL->WaitKill ($client->ProcessStartWaitInterval());

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$server_status = $SV->WaitKill ($server->ProcessStopWaitInterval());

if ($server_status != 0) {
    print STDERR "ERROR: server returned $server_status\n";
    $status = 1;
}

$server->DeleteFile($iorbase_master);
$server->DeleteFile($iorbase_slave);
$client->DeleteFile($iorbase_master);
$client->DeleteFile($iorbase_slave);

exit $status;
