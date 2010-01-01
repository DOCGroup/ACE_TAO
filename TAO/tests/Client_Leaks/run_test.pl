eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $target = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$iorfile = $target->LocalFile ("server.ior");
$target->DeleteFile ($iorfile);
$status = 0;

$SV = $target->CreateProcess ("server", " -o $iorfile");
$CL = $target->CreateProcess ("client", " -k file://$iorfile");

$SV->Spawn ();

if ($target->WaitForFileTimed($iorfile, $target->ProcessStartWaitInterval ()) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill ($target->ProcessStartWaitInterval () + 585);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill ($target->ProcessStopWaitInterval ());

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

$target->GetStderrLog();
$target->DeleteFile ($iorfile);

exit $status;
