eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib '../../../bin';
use PerlACE::Run_Test;

$iorfile = PerlACE::LocalFile ("server.ior");
$orbport = 10000 + PerlACE::uniqueid ();
unlink $iorfile;
$status = 0;

$SV = new PerlACE::Process ("server", "-ORBEndpoint iiop://localhost:$orbport");
$CL = new PerlACE::Process ("client", "-h localhost -p $orbport");

print STDERR $CL->CommandLine(), "\n" ;
$SV->Spawn ();

if (PerlACE::waitforfile_timed ($iorfile, 5) == -1) {
    print STDERR "ERROR: cannot find file <$iorfile>\n";
    $SV->Kill (); $SV->TimedWait (1);
    exit 1;
} 

$client = $CL->SpawnWaitKill (300);

if ($client != 0) {
    print STDERR "ERROR: client returned $client\n";
    $status = 1;
}

$server = $SV->WaitKill (120);

if ($server != 0) {
    print STDERR "ERROR: server returned $server\n";
    $status = 1;
}

unlink $iorfile;

exit $status;
