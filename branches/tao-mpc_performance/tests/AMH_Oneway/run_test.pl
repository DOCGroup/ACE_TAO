eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that tests AMH exceptions

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";
my $client = PerlACE::TestTarget::create_target (2) || die "Create target 2 failed\n";

my $iorbase = "test.ior";
my $server_iorfile = $server->LocalFile ($iorbase);
my $client_iorfile = $client->LocalFile ($iorbase);
$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

$AMH = $server->CreateProcess ("server", "");
$CL = $client->CreateProcess ("client", "");

# Run the AMH server.
$sv = $AMH->Spawn ();

if ($sv != 0) {
   print STDERR "ERROR: AMH server returned $sv\n";
   exit 1;
}

if ($server->WaitForFileTimed ($iorbase,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$server_iorfile>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
}
if ($server->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot get file <$server_iorfile>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
}
if ($client->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$client_iorfile>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
}

# Run the client.
$client_status = $CL->Spawn ();
if ($client_status != 0) {
    print STDERR "ERROR: Client returned $client_status\n";
    $status = 1;
}

# Clean up.

$client_status = $CL->WaitKill ($client->ProcessStopWaitInterval() + 15);
if ($client_status != 0) {
    print STDERR "ERROR: Client returned $client_status\n";
    $status = 1;
}

$amhserver= $AMH->WaitKill ($server->ProcessStopWaitInterval());
if ($amhserver != 0) {
    print STDERR "ERROR: AMH Server returned $amhserver\n";
    $status = 1;
}

$server->DeleteFile($iorbase);
$client->DeleteFile($iorbase);

exit $status;
