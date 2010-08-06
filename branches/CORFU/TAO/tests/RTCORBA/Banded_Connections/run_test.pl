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

print STDERR "\n********** RTCORBA Priority Banded Connections Unit Test\n";

# Different priority values for the bands on each OS.

if ($^O eq "MSWin32") {
    $server_args =
        "-b bands.nt";
}
elsif ($^O eq "dec_osf") {
    $server_args =
        "-b bands.tru64";
}
elsif ($^O eq "hpux") {
    $server_args =
        "-b bands.hpux";
}
elsif ($^O eq "irix") {
    $server_args =
        "-b bands.irix";
}
else {
    $server_args =
        "-b bands.unix";
}

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", $server_args);
}
else {
    $SV = new PerlACE::Process ("server", $server_args);
}
if (PerlACE::is_vxworks_rtp_test()) {
    $CL = new PerlACE::ProcessVX ("client", "-n file://$iorfile1base -o file://$iorfile2base");
}
else {
    $CL = new PerlACE::Process ("client", "-n file://$iorfile1 -o file://$iorfile2");
}

$SV->Spawn();
if (PerlACE::waitforfile_timed ($iorfile2, $PerlACE::wait_interval_for_process_creation) == -1)
{
    $server = $SV->TimedWait (1);
    if ($server == 2)
    {
        # Mark as no longer running to avoid errors on exit.
        $SV->{RUNNING} = 0;
        exit $status;
    }
    else
    {
        print STDERR "ERROR: cannot find file <$iorfile2>\n";
        $SV->Kill ();
        exit 1;
    }
}

$client = $CL->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (30);

if ($server != 0)
{
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile1;
unlink $iorfile2;

# Clean up SHMIOP files
PerlACE::check_n_cleanup_files ("server_shmiop_*");

exit $status;
