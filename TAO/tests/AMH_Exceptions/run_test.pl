eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

# This is a Perl script that tests AMH exceptions

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

# Amount of delay (in seconds) between starting a server and a client.
$sleeptime = $PerlACE::wait_interval_for_process_creation;

my $target = PerlACE::TestTarget::create_target($PerlACE::TestConfig) || die "Create target failed\n";
my $host = new PerlACE::TestTarget;

# File used to pass AMH server ior to its clients.
# This file name is hard-coded in the server.cpp and client.cpp files
$iorbase = "test.ior";
$iorfile = $target->LocalFile("$iorbase");
$target->DeleteFile($iorfile);
$host->DeleteFile($iorbase);

if (PerlACE::is_vxworks_test()) {
    $AMH = new PerlACE::ProcessVX ("server", "");
}
else {
    $AMH = $target->CreateProcess ("server", "");
#    $AMH = new PerlACE::Process ("server", "");
}

$CL = $host->CreateProcess ("client", "");

# Run the AMH server.
$AMH->Spawn ();

#if (PerlACE::waitforfile_timed ($iorfile, $sleeptime) == -1) {
if ($target->WaitForFileTimed ($iorfile, $sleeptime) == -1) {
    print STDERR "ERROR: File containing AMH Server ior,".
        " <$iorfile>, cannot be found\n";
    $AMH->Kill ();
    exit 1;
}

if ($target->GetFile ($iorfile, $iorbase) == -1) {
    print STDERR "ERROR: cannot retrieve file <$iorfile>\n";
    $AMH->Kill (); $AMH->TimedWait (1);
    exit 1;
} 

# Run client.
$client = $CL->SpawnWaitKill (30);

# Clean up.
$amhserver= $AMH->WaitKill (15);

if ($amhserver != 0) {
    print STDERR "ERROR: AMH Server returned $amhserver\n";
    $status = 1;
}

$target->GetStderrLog();
#unlink $iorfile;
$host->DeleteFile($iorbase);
$target->DeleteFile ($iorfile);
exit $status;
