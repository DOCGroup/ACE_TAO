eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$status = 0;
$iorfile = PerlACE::LocalFile ("oneways.ior");

unlink $iorfile;

$SV = new PerlACE::Process ("server", "-o $iorfile");
$CL = new PerlACE::Process ("client");


$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); 
    exit 1;
}

print STDERR "\nFlushing based on implicit message counts\n\n";

$CL->Arguments ("-k file://$iorfile -c 5 -b -1 -f -1 -t -1");

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

print STDERR "\nFlushing based on implicit message bytes\n\n";

$CL->Arguments ("-k file://$iorfile -b 250 -c -1 -f -1 -t -1");

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

print STDERR "\nFlushing based on implicit timeout\n\n";

$CL->Arguments ("-k file://$iorfile -t 5000 -b -1 -c -1 -f -1");

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

print STDERR "\nExplicit queue flushing (and server shutdown)\n\n";

$CL->Arguments ("-k file://$iorfile -f 5 -b -1 -c 100 -t -1 -x");

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1
}

$server = $SV->WaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
