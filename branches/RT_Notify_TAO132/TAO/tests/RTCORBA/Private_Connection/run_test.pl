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

print STDERR "\n********** RTCORBA Private Connection Unit Test\n\n";

$SV = new PerlACE::Process ("server", "-o $iorfile1 -p $iorfile2");
$CL = new PerlACE::Process ("client", "-o file://$iorfile1 -p file://$iorfile2 -ORBdebuglevel 3 ");

$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile2, 10) == -1) 
{
    print STDERR "ERROR: cannot find file <$iorfile2>\n";
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
    print STDERR "ERROR: server returned $server \n";
    $status = 1;
}

unlink $iorfile1;
unlink $iorfile2;

exit $status;
