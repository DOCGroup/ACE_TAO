eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that tests AMH exceptions

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $target = PerlACE::TestTarget::create_target(1) || die "Create target 1 failed\n";
my $host = PerlACE::TestTarget::create_target(2) || die "Create targer 2 failed\n";

# File used to pass AMH server ior to its clients.
# This file name is hard-coded in the server.cpp and client.cpp files
$iorbase = "test.ior";
my $server_iorfile = $target->LocalFile ($iorbase);
my $client_iorfile = $host->LocalFile ($iorbase);
$target->DeleteFile($iorbase);
$host->DeleteFile($iorbase);

$AMH = $target->CreateProcess ("server", "-ORBdebuglevel $debug_level -o $server_iorfile");
$CL = $host->CreateProcess ("client", "-k file://$client_iorfile");

# Run the AMH server.
$AMH->Spawn ();

if ($target->WaitForFileTimed ($iorbase,
                               $target->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: File containing AMH Server ior,".
        " <$iorbase>, cannot be found\n";
    $AMH->Kill ();
    exit 1;
}

if ($target->GetFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorbase>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
}
if ($host->PutFile ($iorbase) == -1) {
    print STDERR "ERROR: cannot set file <$iorbase>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
}

# Run client.
$client_status = $CL->SpawnWaitKill ($host->ProcessStartWaitInterval() + 15);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

# Clean up.
$amhserver= $AMH->WaitKill ($target->ProcessStopWaitInterval());

if ($amhserver != 0) {
    print STDERR "ERROR: AMH Server returned $amhserver\n";
    $status = 1;
}

$target->GetStderrLog();
$host->DeleteFile($iorbase);
$target->DeleteFile ($iorbase);
exit $status;
