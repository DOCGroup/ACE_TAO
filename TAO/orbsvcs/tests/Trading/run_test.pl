eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$ior = PerlACE::LocalFile ("trading.ior");
$ready_file = PerlACE::LocalFile ("export_test_ready");
$sleeptime = 20;
$port = PerlACE::random_port();

unlink $ior;
unlink $ready_file;

## Specify and endpoint so that we may test the corbaloc in the
## -ORBInitRef.  We retain one -ORBInitRef using the file just to test
## both ways.
$TS = new PerlACE::Process ("../../Trading_Service/Trading_Service",
                            "-ORBEndpoint iiop://:$port -TSdumpior $ior");
$E = new PerlACE::Process ("export_test",
                           "-ORBInitRef TradingService=corbaloc:::$port/TradingService -quiet");
$I = new PerlACE::Process ("import_test",
                           "-ORBInitRef TradingService=file://$ior -quiet");

if ($TS->Spawn () == -1) {
    exit 1;
}

if (PerlACE::waitforfile_timed ($ior, $sleeptime) == -1) {
    print STDERR "ERROR: waiting for trading service IOR file\n";
    $TS->Kill ();
    exit 1;
}

if ($E->Spawn () == -1) {
    $TS->Kill ();
    exit 1;
}

if (PerlACE::waitforfile_timed ($ready_file, 120) == -1) {
    print STDERR "ERROR: waiting for the export test to finish\n";
    $E->Kill ();
    $TS->Kill ();
    exit 1;
}

$test = $I->SpawnWaitKill (60);
$E->Kill ();
$TS->Kill ();

unlink $ior;
unlink $ready_file;

if ($test != 0) {
    print STDERR "ERROR: import test returned $test\n";
    exit 1;
}

exit 0;
