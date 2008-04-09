#
# $Id:$
#

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $target = PerlACE::TestTarget::create_target ($PerlACE::TestConfig);

$iorbase = "server.ior";
$iorfile = $target->LocalFile ("$iorbase");
$target->DeleteFile($iorfile);

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-ORBDebuglevel $debug_level -o $iorbase");
}
else {
    $SV = $target->CreateProcess ("server", " -ORBid AMI_Forward_server -o $iorfile");
}
$CL = $target->CreateProcess ("client", "-ORBdebuglevel $debug_level -ORBid AMI_Forward_client -k file://$iorfile");

$server = $SV->Spawn ();
if (PerlACE::waitforfile_timed ($iorfile,
             $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$pidfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$client = $CL->SpawnWaitKill (300);

$server = $SV->Kill ();

$target->DeleteFile($iorfile);

exit $status;
