eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# -*- perl -*-

use lib '../../../../bin';
use PerlACE::Run_Test;

$status = 0;

$iorfile = PerlACE::LocalFile ("ior");

$iorfile_1 = $iorfile."_1";
$iorfile_2 = $iorfile."_2";
$iorfile_3 = $iorfile."_3";
$iorfile_4 = $iorfile."_4";
$iorfile_5 = $iorfile."_5";
$iorfile_6 = $iorfile."_6";

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;
unlink $iorfile_4;
unlink $iorfile_5;
unlink $iorfile_6;

$SV = new PerlACE::Process ("server");
$CL1 = new PerlACE::Process ("client",
                             "-a file://$iorfile_1 -b file://$iorfile_2 -c file://$iorfile_3 -d file://$iorfile_4");
$CL2 = new PerlACE::Process ("client", "-e file://$iorfile_5 -f file://$iorfile_6");


$SV->Spawn ();

# In this example all the files are written out at the same time. So  make a
# check only for the first file
if (PerlACE::waitforfile_timed ($iorfile_1, 5) == -1) {
    print STDERR "ERROR: cannot find ior files\n";
    $SV->Kill (); 
    exit 1;
}

$client = $CL1->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 1 returned $client\n";
    $status = 1;
}

$client = $CL2->SpawnWaitKill (60);

if ($client != 0) {
    print STDERR "ERROR: client 2 returned $client\n";
    $status = 1;
}

$server = $SV->TerminateWaitKill (5);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile_1;
unlink $iorfile_2;
unlink $iorfile_3;
unlink $iorfile_4;
unlink $iorfile_5;
unlink $iorfile_6;

exit $status;
