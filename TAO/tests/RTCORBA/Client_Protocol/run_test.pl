eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;
$iorfile1base = "test1.ior";
$iorfile2base = "test2.ior";
$iorfile1 = PerlACE::LocalFile ("$iorfile1base");
$iorfile2 = PerlACE::LocalFile ("$iorfile2base");

unlink $iorfile1;
unlink $iorfile2;

print STDERR "\n********** RTCORBA Client Protocol Policy Unit Test\n\n";

# Arguments are platform-dependent (UIOP not available on Windows).
$server_args =
    (PerlACE::is_vxworks_test() ? "" : (($^O eq "MSWin32") ? "-p 1413566210 " : "-p 1413566208 "))
    ."-ORBendpoint iiop:// "
    .(PerlACE::is_vxworks_test() ? "" : "-ORBendpoint shmiop:// ")
    .(($^O eq "MSWin32" || $^O eq "VMS") ? "" : "-ORBEndpoint uiop:// ");
$client_args =
    "-s file://$iorfile1 -c file://$iorfile2 "
    .(($^O eq "MSWin32" || $^O eq "VMS") ? "" : "-p 1413566210 ")
    ."-ORBdebuglevel 1 ";

#if ($^O eq "MSWin32") {
#    $server_args =
#        "-p 1413566210 "
#        ."-ORBendpoint iiop:// -ORBendpoint shmiop:// ";
#    $client_args =
#        "-s file://$iorfile1 -c file://$iorfile2 -ORBdebuglevel 1";
#}

# Start server.
if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-s $iorfile1base -c $iorfile2base $server_args");
}
else {
    $SV = new PerlACE::Process ("server", "-s $iorfile1 -c $iorfile2 $server_args");
}
$CL = new PerlACE::Process ("client", $client_args);

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, $PerlACE::wait_interval_for_process_creation) == -1) {
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
