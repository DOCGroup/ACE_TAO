eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

# add dynamically loaded runtime lib dependencies
$server->AddRuntimeLibrary ('TAO_PI');
$server->AddRuntimeLibrary ('TAO_CodecFactory');

$orb_log = "ORBLog.log";
$orb_logfile = $server->LocalFile ($orb_log);
$server->DeleteFile ($orb_log);

$SV = $server->CreateProcess ("server", "-l $orb_logfile");

$SV->Spawn ();

if ($server->WaitForFileTimed ($orb_log,
                               $server->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$orb_logfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
}

$SV->WaitKill ($server->ProcessStopWaitInterval());

exit 0;
