eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $server = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$orb_log = "ORBLog.log";
$server->LocalFile ($orb_log);
$server->DeleteFile ($orb_log);

$SV = $server->CreateProcess ("server", "-l $orb_log");

$SV->Spawn ();
     
$SV->WaitKill ($server->ProcessStopWaitInterval());

exit 0;
