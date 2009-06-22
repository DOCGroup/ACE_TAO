eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::Run_Test;

$status = 0;

$iorbase = "server.ior";
$iorfile = PerlACE::LocalFile ("$iorbase");

unlink $iorfile;

if (PerlACE::is_vxworks_test()) {
    $SV = new PerlACE::ProcessVX ("server", "-o $iorbase");
    $direct_colloc = "-ORBCollocationStrategy direct -o $iorbase";
    $no_colloc = "-ORBCollocation no -o $iorbase";

}
else {
    $SV = new PerlACE::Process ("server", "-o $iorfile");
    $direct_colloc = "-ORBCollocationStrategy direct -o $iorfile";
    $no_colloc = "-ORBCollocation no -o $iorfile";
}
$CL = new PerlACE::Process ("client", "-k file://$iorfile");

#
# Test using ThruPOA collocation.
#

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

$SV->Kill ();

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

#
# Test using Direct Collocation
#

unlink $iorfile;

$SV->Arguments ($direct_colloc);

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

$SV->Kill ();

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

#
# Test using No Collocation
#

unlink $iorfile;

$SV->Arguments ($no_colloc);

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile,
                        $PerlACE::wait_interval_for_process_creation) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill ();
    exit 1;
}

$client = $CL->SpawnWaitKill (60);

$SV->Kill ();

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

unlink $iorfile;

exit $status
