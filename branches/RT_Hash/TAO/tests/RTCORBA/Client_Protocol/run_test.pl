eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$iorfile1 = PerlACE::LocalFile ("test1.ior");
$iorfile2 = PerlACE::LocalFile ("test2.ior");

unlink $iorfile1;
unlink $iorfile2;

print STDERR "\n********** RTCORBA Client Protocol Policy Unit Test\n\n";

# Arguments are platform-dependent (UIOP not available on Windows).
$server_args =
    "-s $iorfile1 -c $iorfile2 -p 1413566208 "
    ."-ORBendpoint iiop:// -ORBendpoint shmiop:// -ORBEndpoint uiop:// ";
$client_args =
    "-s file://$iorfile1 -c file://$iorfile2 -p 1413566210 -ORBdebuglevel 1 ";

if ($^O eq "MSWin32") {
    $server_args =
        "-s $iorfile1 -c $iorfile2 -p 1413566210 "
        ."-ORBendpoint iiop:// -ORBendpoint shmiop:// ";
    $client_args =
        "-s file://$iorfile1 -c file://$iorfile2 -ORBdebuglevel 1";
}

# Start server.
$SV = new PerlACE::Process ("server", $server_args);
$CL = new PerlACE::Process ("client", $client_args);

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, 10) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
    $SV->Kill ();
    exit 1;
}

# Start client.
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

unlink $iorfile1;
unlink $iorfile2;

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
