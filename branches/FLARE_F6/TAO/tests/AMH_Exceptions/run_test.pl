eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that tests AMH exceptions

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target(1) || die "Create target failed\n";
my $host = PerlACE::TestTarget::create_target(2) || die "Create client failed\n";

# File used to pass AMH server ior to its clients.
# This file name is hard-coded in the server.cpp and client.cpp files
$iorbase = "test.ior";
$server_iorfile = $target->LocalFile("$iorbase");
$host_iorfile = $host->LocalFile("$iorbase");
$target->DeleteFile($server_iorfile);
$host->DeleteFile($host_iorfile);

if (PerlACE::is_vxworks_test()) {
    $AMH = new PerlACE::ProcessVX ("server", "");
}
else {
    $AMH = $target->CreateProcess ("server", "");
}

$CL = $host->CreateProcess ("client", "");

# Run the AMH server.
$AMH->Spawn ();

if ($target->WaitForFileTimed ($server_iorfile,
                               $target->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: File containing AMH Server ior,".
        " <$server_iorfile>, cannot be found\n";
    $AMH->Kill ();
    exit 1;
}

if ($target->GetFile ($server_iorfile, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$server_iorfile>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
} 
if ($host->PutFile ($iorbase, $host_iorfile) == -1) {
    print STDERR "ERROR: cannot set file <$host_iorfile>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
} 

# Run client.
$client_status = $CL->SpawnWaitKill (30);

# Clean up.
$amhserver= $AMH->WaitKill (15);

if ($amhserver != 0) {
    print STDERR "ERROR: AMH Server returned $amhserver\n";
    $status = 1;
}

$target->GetStderrLog();
$host->DeleteFile($host_iorfile);
$target->DeleteFile ($server_iorfile);
exit $status;
