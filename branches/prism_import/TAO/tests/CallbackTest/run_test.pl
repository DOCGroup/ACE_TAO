eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);

$iorfile = $target->LocalFile ("server.ior");
$target->DeleteFile ($iorfile);
$status = 0;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o server.ior");
}
else {
    $SV = $target->CreateProcess ("server", "-ORBid Callback_server -o $iorfile");
}
$CL = $target->CreateProcess ("client", " -k file://$iorfile -ORBDottedDecimalAddresses 1");

$SV->Spawn ();

if ($target->WaitForFileTimed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

# The client crashes, therefore it normally exists with status != 0,
# but a status of -1 would be a serious error.
if ($client == -1) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$target->GetStderrLog();
$target->DeleteFile ($iorfile);

exit $status;
