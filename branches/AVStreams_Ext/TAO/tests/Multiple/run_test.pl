eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib "../../../bin";
use PerlACE::Run_Test;

$status = 0;

$direct_colloc = "-ORBCollocationStrategy direct";
$no_colloc = "-ORBCollocation no";

# @todo Test should take -o and -k options to specify iorfile
# Hard coded in test.
$iorfile = "s.ior";

unlink $iorfile;

$SV = new PerlACE::Process ("server");
$CL = new PerlACE::Process ("client");

#
# Test using ThruPOA collocation.
#

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
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

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
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

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
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
