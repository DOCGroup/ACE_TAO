eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("test.ior");

unlink $iorfile;

print STDERR "\n********** RTCORBA Explicit Binding Unit Test\n\n";


$SV = new PerlACE::Process ("server", "-o $iorfile -ORBendpoint iiop:// -ORBendpoint shmiop://");
$CL = new PerlACE::Process ("client", "-o file://$iorfile -ORBdebuglevel 1");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (60);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
